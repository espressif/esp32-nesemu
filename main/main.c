#include <driver/gpio.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_partition.h>
#include <esp_spiffs.h>
#include <freertos/FreeRTOS.h>
#include <nvs_flash.h>

#include "nofrendo.h"

char *osd_getromdata()
{
    char *romdata;
    const esp_partition_t *part;
    spi_flash_mmap_handle_t hrom;
    esp_err_t err;
    nvs_flash_init();
    part = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_1, NULL);
    if (part == 0)
        printf("Couldn't find rom part!\n");
    err = esp_partition_mmap(part, 0, 0x140000, SPI_FLASH_MMAP_DATA, (const void **)&romdata, &hrom);
    if (err != ESP_OK)
        printf("Couldn't map rom part!\n");
    printf("Initialized. ROM@%p\n", romdata);
    return (char *)romdata;
}

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void spiffs_init(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            printf("Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            printf("Failed to find SPIFFS partition");
        }
        else
        {
            printf("Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK)
    {
        printf("Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    }
    else
    {
        printf("Partition size: total: %d, used: %d", total, used);
    }
}

int app_main(void)
{
    spiffs_init();

    printf("NoFrendo start!\n");
    nofrendo_main(0, NULL);
    printf("NoFrendo died? Oh no!\n");
    asm("break.n 1");
    return 0;
}
