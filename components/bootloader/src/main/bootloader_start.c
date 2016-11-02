// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include "esp_attr.h"
#include "esp_log.h"

#include "rom/cache.h"
#include "rom/ets_sys.h"
#include "rom/spi_flash.h"
#include "rom/crc.h"
#include "rom/rtc.h"

#include "soc/soc.h"
#include "soc/cpu.h"
#include "soc/dport_reg.h"
#include "soc/io_mux_reg.h"
#include "soc/efuse_reg.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/timer_group_reg.h"

#include "sdkconfig.h"

#include "bootloader_config.h"

extern int _bss_start;
extern int _bss_end;

static const char* TAG = "boot";
/*
We arrive here after the bootloader finished loading the program from flash. The hardware is mostly uninitialized,
flash cache is down and the app CPU is in reset. We do have a stack, so we can do the initialization in C.
*/

// TODO: make a nice header file for ROM functions instead of adding externs all over the place
extern void Cache_Flush(int);

void bootloader_main();
void unpack_load_app(const esp_partition_pos_t *app_node);
void print_flash_info(const esp_image_header_t* pfhdr);
void IRAM_ATTR set_cache_and_start_app(uint32_t drom_addr,
    uint32_t drom_load_addr,
    uint32_t drom_size,
    uint32_t irom_addr,
    uint32_t irom_load_addr,
    uint32_t irom_size,
    uint32_t entry_addr);
static void update_flash_config(const esp_image_header_t* pfhdr);


void IRAM_ATTR call_start_cpu0()
{
    cpu_configure_region_protection();

    //Clear bss
    memset(&_bss_start, 0, (&_bss_end - &_bss_start) * sizeof(_bss_start));

    /* completely reset MMU for both CPUs
       (in case serial bootloader was running) */
    Cache_Read_Disable(0);
    Cache_Read_Disable(1);
    Cache_Flush(0);
    Cache_Flush(1);
    mmu_init(0);
    REG_SET_BIT(DPORT_APP_CACHE_CTRL1_REG, DPORT_APP_CACHE_MMU_IA_CLR);
    mmu_init(1);
    REG_CLR_BIT(DPORT_APP_CACHE_CTRL1_REG, DPORT_APP_CACHE_MMU_IA_CLR);
    /* (above steps probably unnecessary for most serial bootloader
       usage, all that's absolutely needed is that we unmask DROM0
       cache on the following two lines - normal ROM boot exits with
       DROM0 cache unmasked, but serial bootloader exits with it
       masked. However can't hurt to be thorough and reset
       everything.)

       The lines which manipulate DPORT_APP_CACHE_MMU_IA_CLR bit are
       necessary to work around a hardware bug.
    */
    REG_CLR_BIT(DPORT_PRO_CACHE_CTRL1_REG, DPORT_PRO_CACHE_MASK_DROM0);
    REG_CLR_BIT(DPORT_APP_CACHE_CTRL1_REG, DPORT_APP_CACHE_MASK_DROM0);

    bootloader_main();
}

/**
 *  @function :     get_bin_len
 *  @description:   get bin's length 
 *
 *  @inputs:        pos     bin locate address in flash
 *  @return:        uint32  length of bin,if bin MAGIC error return 0
 */

uint32_t get_bin_len(uint32_t pos)
{   
    uint32_t len = 8 + 16;
    uint8_t i;
    ESP_LOGD(TAG, "pos %d %x",pos,*(uint8_t *)pos);
    if(0xE9 != *(uint8_t *)pos) {
        return 0;
    }
    for (i = 0; i < *(uint8_t *)(pos + 1); i++) {
        len += *(uint32_t *)(pos + len + 4) + 8;
    }
    if (len % 16 != 0) {
        len = (len / 16 + 1) * 16;
    } else {
        len += 16;
    }
    ESP_LOGD(TAG, "bin length = %d", len);
    return len;
}

/** 
 *  @function :     boot_cache_redirect
 *  @description:   Configure several pages in flash map so that `size` bytes 
 *                  starting at `pos` are mapped to 0x3f400000.
 *                  This sets up mapping only for PRO CPU.
 *
 *  @inputs:        pos     address in flash
 *                  size    size of the area to map, in bytes
 */
void boot_cache_redirect( uint32_t pos, size_t size )
{
    uint32_t pos_aligned = pos & 0xffff0000;
    uint32_t count = (size + 0xffff) / 0x10000;
    Cache_Read_Disable( 0 );
    Cache_Flush( 0 );
    ESP_LOGD(TAG, "mmu set paddr=%08x count=%d", pos_aligned, count );
    cache_flash_mmu_set( 0, 0, 0x3f400000, pos_aligned, 64, count );
    Cache_Read_Enable( 0 );
}

/**
 *  @function :     load_partition_table
 *  @description:   Parse partition table, get useful data such as location of 
 *                  OTA info sector, factory app sector, and test app sector.
 *
 *  @inputs:        bs     bootloader state structure used to save the data
 *                  addr   address of partition table in flash
 *  @return:        return true, if the partition table is loaded (and MD5 checksum is valid)
 *
 */
bool load_partition_table(bootloader_state_t* bs, uint32_t addr)
{
    esp_partition_info_t partition;
    uint32_t end = addr + 0x1000;
    int index = 0;
    char *partition_usage;

    ESP_LOGI(TAG, "Partition Table:");
    ESP_LOGI(TAG, "## Label            Usage          Type ST Offset   Length");

    while (addr < end) {
        ESP_LOGD(TAG, "load partition table entry from %x(%08x)", addr, MEM_CACHE(addr));
        memcpy(&partition, MEM_CACHE(addr), sizeof(partition));
        ESP_LOGD(TAG, "type=%x subtype=%x", partition.type, partition.subtype);
        partition_usage = "unknown";

        if (partition.magic == ESP_PARTITION_MAGIC) { /* valid partition definition */
            switch(partition.type) {
            case PART_TYPE_APP: /* app partition */
                switch(partition.subtype) {
                case PART_SUBTYPE_FACTORY: /* factory binary */
                    bs->factory = partition.pos;
                    partition_usage = "factory app";
                    break;
                case PART_SUBTYPE_TEST: /* test binary */
                    bs->test = partition.pos;
                    partition_usage = "test app";
                    break;
                default:
                    /* OTA binary */
                    if ((partition.subtype & ~PART_SUBTYPE_OTA_MASK) == PART_SUBTYPE_OTA_FLAG) {
                        bs->ota[partition.subtype & PART_SUBTYPE_OTA_MASK] = partition.pos;
                        ++bs->app_count;
                        partition_usage = "OTA app";
                    }
                    else {
                        partition_usage = "Unknown app";
                    }
                    break;
                }
                break; /* PART_TYPE_APP */
            case PART_TYPE_DATA: /* data partition */
                switch(partition.subtype) {
                case PART_SUBTYPE_DATA_OTA: /* ota data */
                    bs->ota_info = partition.pos;
                    partition_usage = "OTA data";
                    break;
                case PART_SUBTYPE_DATA_RF:
                    partition_usage = "RF data";
                    break;
                case PART_SUBTYPE_DATA_WIFI:
                    partition_usage = "WiFi data";
                    break;
                default:
                    partition_usage = "Unknown data";
                    break;
                }
                break; /* PARTITION_USAGE_DATA */
            default: /* other partition type */
                break;
            }
        }
        /* invalid partition magic number */
        else {
            break; /* todo: validate md5 */
        }

        /* print partition type info */
        ESP_LOGI(TAG, "%2d %-16s %-16s %02x %02x %08x %08x", index, partition.label, partition_usage,
                 partition.type, partition.subtype,
                 partition.pos.offset, partition.pos.size);
        index++;
        addr += sizeof(partition);
    }

    ESP_LOGI(TAG,"End of partition table");
    return true;
}

static uint32_t ota_select_crc(const esp_ota_select_entry_t *s)
{
  return crc32_le(UINT32_MAX, (uint8_t*)&s->ota_seq, 4);
}

static bool ota_select_valid(const esp_ota_select_entry_t *s)
{
  return s->ota_seq != UINT32_MAX && s->crc == ota_select_crc(s);
}

/**
 *  @function :     bootloader_main
 *  @description:   entry function of 2nd bootloader
 *
 *  @inputs:        void
 */

void bootloader_main()
{
    ESP_LOGI(TAG, "Espressif ESP32 2nd stage bootloader v. %s", BOOT_VERSION);

    esp_image_header_t fhdr;
    bootloader_state_t bs;
    SpiFlashOpResult spiRet1,spiRet2;    
    esp_ota_select_entry_t sa,sb;
    memset(&bs, 0, sizeof(bs));

    ESP_LOGI(TAG, "compile time " __TIME__ );
    /* disable watch dog here */
    REG_CLR_BIT( RTC_CNTL_WDTCONFIG0_REG, RTC_CNTL_WDT_FLASHBOOT_MOD_EN );
    REG_CLR_BIT( TIMG_WDTCONFIG0_REG(0), TIMG_WDT_FLASHBOOT_MOD_EN );
    SPIUnlock();
    /*register first sector in drom0 page 0 */
    boot_cache_redirect( 0, 0x5000 );

    memcpy((unsigned int *) &fhdr, MEM_CACHE(0x1000), sizeof(esp_image_header_t) );

    print_flash_info(&fhdr);

    update_flash_config(&fhdr);

    if (!load_partition_table(&bs, ESP_PARTITION_TABLE_ADDR)) {
        ESP_LOGE(TAG, "load partition table error!");
        return;
    }

    esp_partition_pos_t load_part_pos;

    if (bs.ota_info.offset != 0) {              // check if partition table has OTA info partition
        //ESP_LOGE("OTA info sector handling is not implemented");
        boot_cache_redirect(bs.ota_info.offset, bs.ota_info.size );
        memcpy(&sa,MEM_CACHE(bs.ota_info.offset & 0x0000ffff),sizeof(sa));
        memcpy(&sb,MEM_CACHE((bs.ota_info.offset + 0x1000)&0x0000ffff) ,sizeof(sb));
        if(sa.ota_seq == 0xFFFFFFFF && sb.ota_seq == 0xFFFFFFFF) {
            // init status flash
            load_part_pos = bs.ota[0];
            sa.ota_seq = 0x01;
            sa.crc = ota_select_crc(&sa);
            sb.ota_seq = 0x00;
            sb.crc = ota_select_crc(&sb);

            Cache_Read_Disable(0);  
            spiRet1 = SPIEraseSector(bs.ota_info.offset/0x1000);
            spiRet2 = SPIEraseSector(bs.ota_info.offset/0x1000+1);
            if (spiRet1 != SPI_FLASH_RESULT_OK || spiRet2 != SPI_FLASH_RESULT_OK ) {  
                ESP_LOGE(TAG, SPI_ERROR_LOG);
                return;
            } 
            spiRet1 = SPIWrite(bs.ota_info.offset,(uint32_t *)&sa,sizeof(esp_ota_select_entry_t));
            spiRet2 = SPIWrite(bs.ota_info.offset + 0x1000,(uint32_t *)&sb,sizeof(esp_ota_select_entry_t));
            if (spiRet1 != SPI_FLASH_RESULT_OK || spiRet2 != SPI_FLASH_RESULT_OK ) {  
                ESP_LOGE(TAG, SPI_ERROR_LOG);
                return;
            } 
            Cache_Read_Enable(0);  
            //TODO:write data in ota info
        } else  {
            if(ota_select_valid(&sa) && ota_select_valid(&sb)) {
                load_part_pos = bs.ota[(((sa.ota_seq > sb.ota_seq)?sa.ota_seq:sb.ota_seq) - 1)%bs.app_count];
            }else if(ota_select_valid(&sa)) {
                load_part_pos = bs.ota[(sa.ota_seq - 1) % bs.app_count];
            }else if(ota_select_valid(&sb)) {
                load_part_pos = bs.ota[(sb.ota_seq - 1) % bs.app_count];
            }else {
                ESP_LOGE(TAG, "ota data partition info error");
                return;
            }
        }
    } else if (bs.factory.offset != 0) {        // otherwise, look for factory app partition
        load_part_pos = bs.factory;
    } else if (bs.test.offset != 0) {           // otherwise, look for test app parition
        load_part_pos = bs.test;
    } else {                                    // nothing to load, bail out
        ESP_LOGE(TAG, "nothing to load");
        return;
    }

    ESP_LOGI(TAG, "Loading app partition at offset %08x", load_part_pos);
    if(fhdr.secure_boot_flag == 0x01) {
        /* protect the 2nd_boot  */    
        if(false == secure_boot()){
            ESP_LOGE(TAG, "secure boot failed");
            return;
        }
    }

    if(fhdr.encrypt_flag == 0x01) {
        /* encrypt flash */            
        if (false == flash_encrypt(&bs)) {
           ESP_LOGE(TAG, "flash encrypt failed");
           return;
        }
    }

    // copy sections to RAM, set up caches, and start application
    unpack_load_app(&load_part_pos);
}


void unpack_load_app(const esp_partition_pos_t* partition)
{
    boot_cache_redirect(partition->offset, partition->size);

    uint32_t pos = 0;
    esp_image_header_t image_header;
    memcpy(&image_header, MEM_CACHE(pos), sizeof(image_header));
    pos += sizeof(image_header);

    uint32_t drom_addr = 0;
    uint32_t drom_load_addr = 0;
    uint32_t drom_size = 0;
    uint32_t irom_addr = 0;
    uint32_t irom_load_addr = 0;
    uint32_t irom_size = 0;

    /* Reload the RTC memory sections whenever a non-deepsleep reset
       is occurring */
    bool load_rtc_memory = rtc_get_reset_reason(0) != DEEPSLEEP_RESET;

    ESP_LOGD(TAG, "bin_header: %u %u %u %u %08x", image_header.magic,
             image_header.blocks,
             image_header.spi_mode,
             image_header.spi_size,
             (unsigned)image_header.entry_addr);

    for (uint32_t section_index = 0;
            section_index < image_header.blocks;
            ++section_index) {
        esp_image_section_header_t section_header = {0};
        memcpy(&section_header, MEM_CACHE(pos), sizeof(section_header));
        pos += sizeof(section_header);

        const uint32_t address = section_header.load_addr;
        bool load = true;
        bool map = false;
        if (address == 0x00000000) {        // padding, ignore block
            load = false;
        }
        if (address == 0x00000004) {
            load = false;                   // md5 checksum block
            // TODO: actually check md5
        }

        if (address >= DROM_LOW && address < DROM_HIGH) {
            ESP_LOGD(TAG, "found drom section, map from %08x to %08x", pos,
                      section_header.load_addr);
            drom_addr = partition->offset + pos - sizeof(section_header);
            drom_load_addr = section_header.load_addr;
            drom_size = section_header.data_len + sizeof(section_header);
            load = false;
            map = true;
        }

        if (address >= IROM_LOW && address < IROM_HIGH) {
            ESP_LOGD(TAG, "found irom section, map from %08x to %08x", pos,
                      section_header.load_addr);
            irom_addr = partition->offset + pos - sizeof(section_header);
            irom_load_addr = section_header.load_addr;
            irom_size = section_header.data_len + sizeof(section_header);
            load = false;
            map = true;
        }

        if (!load_rtc_memory && address >= RTC_IRAM_LOW && address < RTC_IRAM_HIGH) {
            ESP_LOGD(TAG, "Skipping RTC code section at %08x\n", pos);
            load = false;
        }

        if (!load_rtc_memory && address >= RTC_DATA_LOW && address < RTC_DATA_HIGH) {
            ESP_LOGD(TAG, "Skipping RTC data section at %08x\n", pos);
            load = false;
        }

        ESP_LOGI(TAG, "section %d: paddr=0x%08x vaddr=0x%08x size=0x%05x (%6d) %s", section_index, pos,
                 section_header.load_addr, section_header.data_len, section_header.data_len, (load)?"load":(map)?"map":"");

        if (!load) {
            pos += section_header.data_len;
            continue;
        }

        memcpy((void*) section_header.load_addr, MEM_CACHE(pos), section_header.data_len);
        pos += section_header.data_len;
    }
    
    set_cache_and_start_app(drom_addr,
        drom_load_addr,
        drom_size,
        irom_addr,
        irom_load_addr,
        irom_size,
        image_header.entry_addr);
}

void IRAM_ATTR set_cache_and_start_app(
    uint32_t drom_addr,
    uint32_t drom_load_addr,
    uint32_t drom_size,
    uint32_t irom_addr,
    uint32_t irom_load_addr,
    uint32_t irom_size, 
    uint32_t entry_addr)
{
    ESP_LOGD(TAG, "configure drom and irom and start");
    Cache_Read_Disable( 0 );
    Cache_Read_Disable( 1 );
    Cache_Flush( 0 );
    Cache_Flush( 1 );
    //hack for gbfemto
//    uint32_t drom_page_count = (drom_size + 64*1024 - 1) / (64*1024); // round up to 64k
    uint32_t drom_page_count = (1024*1024*4) / (64*1024); // round up to 64k
    ESP_LOGV(TAG, "d mmu set paddr=%08x vaddr=%08x size=%d n=%d", drom_addr & 0xffff0000, drom_load_addr & 0xffff0000, drom_size, drom_page_count );
    int rc = cache_flash_mmu_set( 0, 0, drom_load_addr & 0xffff0000, drom_addr & 0xffff0000, 64, drom_page_count );
    ESP_LOGV(TAG, "rc=%d", rc );
    rc = cache_flash_mmu_set( 1, 0, drom_load_addr & 0xffff0000, drom_addr & 0xffff0000, 64, drom_page_count );
    ESP_LOGV(TAG, "rc=%d", rc );
    uint32_t irom_page_count = (irom_size + 64*1024 - 1) / (64*1024); // round up to 64k
    ESP_LOGV(TAG, "i mmu set paddr=%08x vaddr=%08x size=%d n=%d", irom_addr & 0xffff0000, irom_load_addr & 0xffff0000, irom_size, irom_page_count );
    rc = cache_flash_mmu_set( 0, 0, irom_load_addr & 0xffff0000, irom_addr & 0xffff0000, 64, irom_page_count );
    ESP_LOGV(TAG, "rc=%d", rc );
    rc = cache_flash_mmu_set( 1, 0, irom_load_addr & 0xffff0000, irom_addr & 0xffff0000, 64, irom_page_count );
    ESP_LOGV(TAG, "rc=%d", rc );
    REG_CLR_BIT( DPORT_PRO_CACHE_CTRL1_REG, (DPORT_PRO_CACHE_MASK_IRAM0) | (DPORT_PRO_CACHE_MASK_IRAM1 & 0) | (DPORT_PRO_CACHE_MASK_IROM0 & 0) | DPORT_PRO_CACHE_MASK_DROM0 | DPORT_PRO_CACHE_MASK_DRAM1 );
    REG_CLR_BIT( DPORT_APP_CACHE_CTRL1_REG, (DPORT_APP_CACHE_MASK_IRAM0) | (DPORT_APP_CACHE_MASK_IRAM1 & 0) | (DPORT_APP_CACHE_MASK_IROM0 & 0) | DPORT_APP_CACHE_MASK_DROM0 | DPORT_APP_CACHE_MASK_DRAM1 );
    Cache_Read_Enable( 0 );
    Cache_Read_Enable( 1 );

    ESP_LOGD(TAG, "start: 0x%08x", entry_addr);
    typedef void (*entry_t)(void);
    entry_t entry = ((entry_t) entry_addr);

    // TODO: we have used quite a bit of stack at this point.
    // use "movsp" instruction to reset stack back to where ROM stack starts.
    (*entry)();
}

static void update_flash_config(const esp_image_header_t* pfhdr)
{
    uint32_t size;
    switch(pfhdr->spi_size) {
        case ESP_IMAGE_FLASH_SIZE_1MB:
            size = 1;
            break;
        case ESP_IMAGE_FLASH_SIZE_2MB:
            size = 2;
            break;
        case ESP_IMAGE_FLASH_SIZE_4MB:
            size = 4;
            break;
        case ESP_IMAGE_FLASH_SIZE_8MB:
            size = 8;
            break;
        case ESP_IMAGE_FLASH_SIZE_16MB:
            size = 16;
            break;
        default:
            size = 2;
    }
    Cache_Read_Disable( 0 );
    // Set flash chip size
    SPIParamCfg(g_rom_flashchip.deviceId, size * 0x100000, 0x10000, 0x1000, 0x100, 0xffff);
    // TODO: set mode
    // TODO: set frequency
    Cache_Flush(0);
    Cache_Read_Enable( 0 );
}

void print_flash_info(const esp_image_header_t* phdr)
{
#if (BOOT_LOG_LEVEL >= BOOT_LOG_LEVEL_NOTICE)

    ESP_LOGD(TAG, "magic %02x", phdr->magic );
    ESP_LOGD(TAG, "blocks %02x", phdr->blocks );
    ESP_LOGD(TAG, "spi_mode %02x", phdr->spi_mode );
    ESP_LOGD(TAG, "spi_speed %02x", phdr->spi_speed );
    ESP_LOGD(TAG, "spi_size %02x", phdr->spi_size );

    const char* str;
    switch ( phdr->spi_speed ) {
    case ESP_IMAGE_SPI_SPEED_40M:
        str = "40MHz";
        break;
    case ESP_IMAGE_SPI_SPEED_26M:
        str = "26.7MHz";
        break;
    case ESP_IMAGE_SPI_SPEED_20M:
        str = "20MHz";
        break;
    case ESP_IMAGE_SPI_SPEED_80M:
        str = "80MHz";
        break;
    default:
        str = "20MHz";
        break;
    }
    ESP_LOGI(TAG, "SPI Speed      : %s", str );

    switch ( phdr->spi_mode ) {
    case ESP_IMAGE_SPI_MODE_QIO:
        str = "QIO";
        break;
    case ESP_IMAGE_SPI_MODE_QOUT:
        str = "QOUT";
        break;
    case ESP_IMAGE_SPI_MODE_DIO:
        str = "DIO";
        break;
    case ESP_IMAGE_SPI_MODE_DOUT:
        str = "DOUT";
        break;
    case ESP_IMAGE_SPI_MODE_FAST_READ:
        str = "FAST READ";
        break;
    case ESP_IMAGE_SPI_MODE_SLOW_READ:
        str = "SLOW READ";
        break;
    default:
        str = "DIO";
        break;
    }
    ESP_LOGI(TAG, "SPI Mode       : %s", str );

    switch ( phdr->spi_size ) {
    case ESP_IMAGE_FLASH_SIZE_1MB:
        str = "1MB";
        break;
    case ESP_IMAGE_FLASH_SIZE_2MB:
        str = "2MB";
        break;
    case ESP_IMAGE_FLASH_SIZE_4MB:
        str = "4MB";
        break;
    case ESP_IMAGE_FLASH_SIZE_8MB:
        str = "8MB";
        break;
    case ESP_IMAGE_FLASH_SIZE_16MB:
        str = "16MB";
        break;
    default:
        str = "2MB";
        break;
    }
    ESP_LOGI(TAG, "SPI Flash Size : %s", str );
#endif
}
