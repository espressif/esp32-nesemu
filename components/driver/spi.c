// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string.h>

#include "spi.h"
#include "soc/spi_register.h"
#include "soc/rtc_cntl_reg.h"

#include "rom/ets_sys.h"

#include "esp_intr.h"

//*****************************************************************************
//
// Make sure all of the definitions in this header have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Defines slave commands.Default value based on slave ESP8266 & ESP32.
 */
#define MASTER_WRITE_DATA_TO_SLAVE_CMD                      2
#define MASTER_READ_DATA_FROM_SLAVE_CMD                     3
#define MASTER_WRITE_STATUS_TO_SLAVE_CMD                    1
#define MASTER_READ_STATUS_FROM_SLAVE_CMD                   4


static void spi_intr_func_set(void * isr, spi_num_e spiNum)
{
     if (SpiNum_SPI1 == spiNum) {
        intr_matrix_set(0, ETS_SPI1_SOURCE, ETS_SPI1_INUM);
        ESP_SPI1_INTR_ATTACH(isr, NULL);
        // enable intr in cpu
        ESP_SPI1_INTR_ENABLE();
    } else if (SpiNum_SPI2 == spiNum) {
        intr_matrix_set(0,ETS_SPI2_SOURCE, ETS_SPI2_INUM);
        ESP_SPI2_INTR_ATTACH(isr, NULL);
        // enable intr in cpu
        ESP_SPI2_INTR_ENABLE();
    } else if (SpiNum_SPI3 == spiNum) {
        intr_matrix_set(0,ETS_SPI3_SOURCE, ETS_SPI3_INUM);
        ESP_SPI3_INTR_ATTACH(isr, NULL);
        // enable intr in cpu
        ESP_SPI3_INTR_ENABLE();
    } else {
        // To do nothing
    }
}
#if 1

/**
 * @brief Get SPI ping buffer address.
 *
 */
uint32_t* spi_dma_ping_buf_get(spi_dma_attr_t *obj)
{
    if (NULL == obj) {
        return NULL;
    }
    return obj->buf->ping->bufAdr;
}

/**
 * @brief Get SPI ping buffer address.
 *
 */
uint32_t* spi_dma_pong_buf_get(spi_dma_attr_t *obj)
{
    if (NULL == obj) {
        return NULL;
    }
    return obj->buf->pong->bufAdr;
}

/**
 * @brief Get without work ping-pong buffer address.
 *
 */
uint32_t* spi_dma_status_get(spi_dma_attr_t *obj)
{
    if ((NULL == obj)
        || (obj->spiNum > SpiNum_Max)) {
        return NULL;
    }
    switch (obj->dir) {
    case SpiDmaDir_Out:
        if (READ_PERI_REG(SPI_OUT_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32_t)obj->buf->ping->lastQueue))  {
            return obj->buf->ping->bufAdr;
        } else if (READ_PERI_REG(SPI_OUT_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32_t)obj->buf->pong->lastQueue)) {
            return obj->buf->pong->bufAdr;
        }
        break;
    case SpiDmaDir_In:
        if (READ_PERI_REG(SPI_IN_SUC_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32_t)obj->buf->ping->lastQueue)) {
            return obj->buf->ping->bufAdr;
        } else if (READ_PERI_REG(SPI_IN_SUC_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32_t)obj->buf->pong->lastQueue)) {
            return obj->buf->pong->bufAdr;
        }
        break;
    default:
        break;
    }
#if 0
    if ((SpiMode_Slave == obj->mode)) {
        // Data stream receive
        //  os_printf("RX DES=%x,pingQueue=%x,pongQueue=%x,obj=%x,pingbuf=%x,pongbuf=%x\r\n",
        //  READ_PERI_REG(SPI_IN_SUC_EOF_DES_ADDR_REG(spiNum)), obj->buf->ping->firstQueue, obj->buf->pong->lastQueue,
        //               obj->buf, obj->buf->ping->bufAdr, obj->buf->pong->bufAdr);
        if (READ_PERI_REG(SPI_IN_SUC_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32)obj->buf->ping->lastQueue)) {
            return obj->buf->ping->bufAdr;
        } else if (READ_PERI_REG(SPI_IN_SUC_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32)obj->buf->pong->lastQueue)) {
            return obj->buf->pong->bufAdr;
        }
    } else if ((SpiMode_Master == obj->mode)) {
        // Data stream send
        // os_printf("TX DES=%x,pingQueue=%x,pongQueue=%x,obj=%x,pingbuf=%x,pongbuf=%x\r\n",
        // READ_PERI_REG(SPI_OUT_EOF_DES_ADDR_REG(spiNum)), obj->buf->ping->firstQueue, obj->buf->pong->firstQueue,
        //             obj->buf, obj->buf->ping->bufAdr, obj->buf->pong->bufAdr);
        if (READ_PERI_REG(SPI_OUT_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32)obj->buf->ping->lastQueue))  {
            return obj->buf->ping->bufAdr;
        } else if (READ_PERI_REG(SPI_OUT_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32)obj->buf->pong->lastQueue)) {
            return obj->buf->pong->bufAdr;
        }
    }
#endif
    return NULL;
}

/**
 * @brief In master mode,get data out;Slave mode, get data in.
 *
 */
int32_t spi_dma_memcpy(spi_dma_attr_t *obj, void *src, uint32_t len)
{
    if ((NULL == obj)
        || (NULL == src)) {
        return -1;
    }
    uint32_t *bufAddress = NULL;
    switch (obj->dir) {
    case SpiDmaDir_Out:
        if (READ_PERI_REG(SPI_OUT_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32_t)obj->buf->ping->lastQueue))  {
            bufAddress = obj->buf->ping->bufAdr;
            dma_buf_len_set(obj->buf, len);
        } else if (READ_PERI_REG(SPI_OUT_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32_t)obj->buf->pong->lastQueue)) {
            bufAddress = obj->buf->pong->bufAdr;
            dma_buf_len_set(obj->buf, len);
        }
        if (bufAddress) {
            memcpy(bufAddress, src, len);
        }
        break;
    case SpiDmaDir_In:
        if (READ_PERI_REG(SPI_IN_SUC_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32_t)obj->buf->ping->lastQueue)) {
            bufAddress = obj->buf->ping->bufAdr;
            dma_buf_len_set(obj->buf, obj->bufSize);
        } else if (READ_PERI_REG(SPI_IN_SUC_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32_t)obj->buf->pong->lastQueue)) {
            bufAddress = obj->buf->pong->bufAdr;
            dma_buf_len_set(obj->buf, obj->bufSize);
        }
        if (bufAddress) {
            memcpy(src, bufAddress, obj->bufSize);
        }
        break;
    default:
        len = 0;
        break;
    }
#if 0
    if ((SpiMode_Slave == obj->mode)) {
        // Data stream receive
        //  os_printf("RX DES=%x,pingQueue=%x,pongQueue=%x,obj=%x,pingbuf=%x,pongbuf=%x\r\n",
        //  READ_PERI_REG(SPI_IN_SUC_EOF_DES_ADDR_REG(spiNum)), obj->buf->ping->firstQueue, obj->buf->pong->lastQueue,
        //               obj->buf, obj->buf->ping->bufAdr, obj->buf->pong->bufAdr);
        if (READ_PERI_REG(SPI_IN_SUC_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32)obj->buf->ping->lastQueue)) {
            bufAddress = obj->buf->ping->bufAdr;
            dma_buf_len_set(obj->buf->ping, obj->bufSize);
        } else if (READ_PERI_REG(SPI_IN_SUC_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32)obj->buf->pong->lastQueue)) {
            bufAddress = obj->buf->pong->bufAdr;
            dma_buf_len_set(obj->buf->pong, obj->bufSize);
        }
        if (bufAddress) {
            memcpy(src, bufAddress, obj->bufSize);
        }
    } else if ((SpiMode_Master == obj->mode)) {
        // Data stream send
        // os_printf("TX DES=%x,pingQueue=%x,pongQueue=%x,obj=%x,pingbuf=%x,pongbuf=%x\r\n",
        // READ_PERI_REG(SPI_OUT_EOF_DES_ADDR_REG(spiNum)), obj->buf->ping->firstQueue, obj->buf->pong->firstQueue,
        //             obj->buf, obj->buf->ping->bufAdr, obj->buf->pong->bufAdr);
        if (READ_PERI_REG(SPI_OUT_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32)obj->buf->ping->lastQueue))  {
            bufAddress = obj->buf->ping->bufAdr;
            dma_buf_len_set(obj->buf->ping, len);
        } else if (READ_PERI_REG(SPI_OUT_EOF_DES_ADDR_REG(obj->spiNum)) == ((uint32)obj->buf->pong->lastQueue)) {
            bufAddress = obj->buf->pong->bufAdr;
            dma_buf_len_set(obj->buf->pong, len);
        }
        if (bufAddress) {
            memcpy(bufAddress, src, len);
        }
    }
#endif
    return len;

}

/**
 * @brief Configrate the Ping-Pong buffer to the destination
 *
 */
void spi_dma_dest_add_set(spi_dma_attr_t *obj)
{
    if ((NULL == obj)
        || (obj->spiNum > SpiNum_Max)) {
        return;
    }
    if (SpiDmaDir_In == obj->dir) {
        SET_PERI_REG_BITS(SPI_DMA_IN_LINK_REG(obj->spiNum), SPI_INLINK_ADDR, ((uint32_t)(obj->buf->ping->firstQueue)), SPI_INLINK_ADDR_S);
    } else if (SpiDmaDir_Out == obj->dir) {
        SET_PERI_REG_BITS(SPI_DMA_OUT_LINK_REG(obj->spiNum), SPI_OUTLINK_ADDR, ((uint32_t)(obj->buf->ping->firstQueue)), SPI_OUTLINK_ADDR_S);
    }
}

/**
 * @brief Reset SPI ping buffer address.
 *
 */
void spi_dma_rest(spi_dma_attr_t *obj)
{
    if ((NULL == obj)
        || (obj->spiNum > SpiNum_Max)) {
        return;
    }
    SET_PERI_REG_MASK(SPI_DMA_CONF_REG(obj->spiNum), SPI_OUT_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST);
    CLEAR_PERI_REG_MASK(SPI_DMA_CONF_REG(obj->spiNum), SPI_OUT_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST);

    dma_buf_len_reset(obj->buf);
}

/**
 * @brief Initialize DMA and create a SPI DMA instance.
 *
 */
int spi_dma_init(spi_dma_attr_t *obj, void *isr)
{
    if ((NULL == obj)
        || (obj->spiNum > SpiNum_Max)) {
        return -1;
    }
    obj->buf = dma_buf_create(obj->bufSize);
    if (NULL == obj->buf) {
        return -2;
    }
    // Reset DMA
    SET_PERI_REG_MASK(SPI_DMA_CONF_REG(obj->spiNum), SPI_OUT_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST);
    CLEAR_PERI_REG_MASK(SPI_DMA_OUT_LINK_REG(obj->spiNum), SPI_OUTLINK_START);
    CLEAR_PERI_REG_MASK(SPI_DMA_IN_LINK_REG(obj->spiNum), SPI_INLINK_START);
    CLEAR_PERI_REG_MASK(SPI_DMA_CONF_REG(obj->spiNum), SPI_OUT_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST);

    // Select DMA channel.
    SET_PERI_REG_BITS(ETS_SPI_DMA_CHAN_SEL, 3, obj->channel, ((obj->spiNum - 1) * 2));

    SET_PERI_REG_MASK(SPI_USER_REG(obj->spiNum), SPI_USR_MOSI);//////add
    if ((SpiMode_Master == obj->mode)) {
        // enable send intr
        SET_PERI_REG_MASK(SPI_DMA_INT_ENA_REG(obj->spiNum ), SpiIntSrc_OneBufSendDone);
    } else if ((SpiMode_Slave == obj->mode)) {
        SET_PERI_REG_MASK(SPI_SLV_RDBUF_DLEN_REG(obj->spiNum), ((obj->bufSize << 3) - 1));
        SET_PERI_REG_MASK(SPI_SLV_WRBUF_DLEN_REG(obj->spiNum), ((obj->bufSize << 3) - 1));
        // enable receive intr
        SET_PERI_REG_MASK(SPI_DMA_INT_ENA_REG(obj->spiNum), SpiIntSrc_OneBufRecvDone);
    }
    // Clear all of interrupt source
    spi_int_clear(obj->spiNum);

    spi_intr_func_set(isr, obj->spiNum);
    return 0;
}

/**
 * @brief Destroy the SPI DMA instance
 *
 */
int spi_dma_uninit(spi_dma_attr_t *obj)
{
    if (NULL == obj) {
        return -1;
    }
    dma_buf_destroy(obj->buf);
    return 0;
}

/**
 * @brief Enable the SPI DMA work mode.
 *
 */
static void spi_dma_enable(spi_dma_attr_t *obj)
{
    if ((NULL == obj)
        || (obj->spiNum > SpiNum_Max)) {
        return;
    }
    if (SpiDmaDir_In == obj->dir) {
        SET_PERI_REG_MASK(SPI_DMA_IN_LINK_REG(obj->spiNum), SPI_INLINK_START);
    } else if (SpiDmaDir_Out == obj->dir) {
        SET_PERI_REG_MASK(SPI_DMA_OUT_LINK_REG(obj->spiNum), SPI_OUTLINK_START);
    }
}

/**
 * @brief Disable the SPI DMA work mode.
 *
 */
static void spi_dma_disable(spi_dma_attr_t *obj)
{
    if ((NULL == obj)
        || (obj->spiNum > SpiNum_Max)) {
        return;
    }
    if (SpiDmaDir_In == obj->dir) {
        CLEAR_PERI_REG_MASK(SPI_DMA_IN_LINK_REG(obj->spiNum), SPI_INLINK_STOP);
    } else if (SpiDmaDir_Out == obj->dir) {
        CLEAR_PERI_REG_MASK(SPI_DMA_OUT_LINK_REG(obj->spiNum), SPI_OUTLINK_STOP);
    }
}

/**
 * @brief Enable SPI DMA interrupt source.
 *
 */
void  spi_dma_int_enable(spi_num_e spiNum, spi_int_src_e intSrc)
{
    if (spiNum > SpiNum_Max) {
        return;
    }
    SET_PERI_REG_MASK(SPI_DMA_INT_ENA_REG(spiNum), intSrc);
}

/**
 * @brief Disable SPI DMA interrupt source.
 *
 */
void  spi_dma_int_disable(spi_num_e spiNum, spi_int_src_e intSrc)
{
    if (spiNum > SpiNum_Max) {
        return;
    }
    SET_PERI_REG_MASK(SPI_DMA_INT_ENA_REG(spiNum), intSrc);
}

/**
 * @brief Clear all of SPI DMA interrupt source.
 *
 */
void  spi_dma_int_clear(spi_num_e spiNum)
{
    if (spiNum > SpiNum_Max) {
        return;
    }
    SET_PERI_REG_MASK(SPI_DMA_INT_CLR_REG(spiNum), 0x1FF);

}

/**
 * @brief Get the SPI DMA interrupt status.
 *
 */
int32_t spi_dma_int_status_get(spi_num_e spiNum)
{
    if (spiNum > SpiNum_Max) {
        return -1;
    }
    return READ_PERI_REG(SPI_DMA_INT_ST_REG(spiNum));
}

/**
 * @brief Start SPI work by DMA
 *
 */
void spi_dma_start(spi_dma_attr_t *obj, uint32_t len)
{
    if ((NULL == obj)
        || (obj->spiNum > SpiNum_Max)) {
        return;
    }
    if (SpiMode_Master == obj->mode) {
        CLEAR_PERI_REG_MASK(SPI_DMA_CONF_REG(obj->spiNum), SPI_DMA_CONTINUE);
        SET_PERI_REG_BITS(SPI_MOSI_DLEN_REG(obj->spiNum), SPI_USR_MOSI_DBITLEN, ((len << 3) - 1), SPI_USR_MOSI_DBITLEN_S);
        SET_PERI_REG_BITS(SPI_MISO_DLEN_REG(obj->spiNum), SPI_USR_MISO_DBITLEN, ((len << 3) - 1), SPI_USR_MISO_DBITLEN_S);
    } else if (SpiMode_Slave == obj->mode) {
        if (0 == len) {
            SET_PERI_REG_MASK(SPI_DMA_CONF_REG(obj->spiNum), SPI_DMA_CONTINUE);
        } else {
            CLEAR_PERI_REG_MASK(SPI_DMA_CONF_REG(obj->spiNum), SPI_DMA_CONTINUE);
        }
    }
    // 1.Enable DMA
    spi_dma_enable(obj);
    // 2.Start SPI
    SET_PERI_REG_MASK(SPI_CMD_REG(obj->spiNum), SPI_USR);
}

/**
 * @brief Stop SPI work by DMA
 *
 */
void spi_dma_stop(spi_dma_attr_t *obj)
{
    if ((NULL == obj)
        || (obj->spiNum > SpiNum_Max)) {
        return;
    }
    if (SpiMode_Master == obj->mode) {
        SET_PERI_REG_MASK(SPI_DMA_CONF_REG(obj->spiNum), SPI_DMA_TX_STOP);
    } else if (SpiMode_Slave == obj->mode) {
        SET_PERI_REG_MASK(SPI_DMA_CONF_REG(obj->spiNum), SPI_DMA_RX_STOP);
    }
    spi_dma_disable(obj);
}
#endif

/**
 * @brief Based on pAttr initialize SPI module.
 *
 */
void spi_init(spi_num_e spiNum, spi_attr_t* pAttr)
{
    if ((spiNum > SpiNum_Max)
        || (NULL == pAttr)) {
        return;
    }

    CLEAR_PERI_REG_MASK(SPI_SLAVE_REG(spiNum), SPI_TRANS_DONE << 5);
    SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_CS_SETUP);

    // SPI_CPOL & SPI_CPHA
    switch (pAttr->subMode) {
    case SpiSubMode_1:
        CLEAR_PERI_REG_MASK(SPI_PIN_REG(spiNum), SPI_CK_IDLE_EDGE);
        SET_PERI_REG_MASK(SPI_USER_REG(spiNum),  SPI_CK_OUT_EDGE); // CHPA_FALLING_EDGE_SAMPLE
        break;
    case SpiSubMode_2:
        SET_PERI_REG_MASK(SPI_PIN_REG(spiNum), SPI_CK_IDLE_EDGE);
        SET_PERI_REG_MASK(SPI_USER_REG(spiNum),  SPI_CK_OUT_EDGE); // CHPA_FALLING_EDGE_SAMPLE
        break;
    case SpiSubMode_3:
        SET_PERI_REG_MASK(SPI_PIN_REG(spiNum), SPI_CK_IDLE_EDGE);
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum),  SPI_CK_OUT_EDGE);
        break;
    case SpiSubMode_0:
    default:
        CLEAR_PERI_REG_MASK(SPI_PIN_REG(spiNum), SPI_CK_IDLE_EDGE);
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum),  SPI_CK_OUT_EDGE);
        // To do nothing
        break;
    }

    // SPI bit order
    if (SpiBitOrder_MSBFirst == pAttr->bitOrder) {
        CLEAR_PERI_REG_MASK(SPI_CTRL_REG(spiNum), SPI_WR_BIT_ORDER);
        CLEAR_PERI_REG_MASK(SPI_CTRL_REG(spiNum), SPI_RD_BIT_ORDER);
    } else if (SpiBitOrder_LSBFirst == pAttr->bitOrder) {
        SET_PERI_REG_MASK(SPI_CTRL_REG(spiNum), SPI_WR_BIT_ORDER);
        SET_PERI_REG_MASK(SPI_CTRL_REG(spiNum), SPI_RD_BIT_ORDER);
    } else {
        // To do nothing
    }

    // SPI bit order
    if (SpiWorkMode_Half == pAttr->halfMode) {
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_DOUTDIN);
    } else if (SpiWorkMode_Full == pAttr->halfMode) {
        SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_DOUTDIN);
    }
    // May be not must to do.
    WRITE_PERI_REG(SPI_USER1_REG(spiNum), 0);
    // SPI mode type
    if (SpiMode_Master == pAttr->mode) {
        // SPI mode type
        SET_PERI_REG_BITS(SPI_CTRL2_REG(spiNum), SPI_MISO_DELAY_MODE, 0, SPI_MISO_DELAY_MODE_S); ////??????
        // SPI_SET_MISO_DELAY_NUM(spiNum,0);////???????
        //SET_PERI_REG_BITS(SPI_CTRL2_REG(spiNum), SPI_MISO_DELAY_NUM,0,SPI_MISO_DELAY_NUM_S);////??????

        CLEAR_PERI_REG_MASK(SPI_SLAVE_REG(spiNum), SPI_SLAVE_MODE);
        // SPI Send buffer
        // CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MISO_HIGHPART );// By default slave send buffer C0-C7
        // SPI Speed
        if (1 < (pAttr->speed)) {
            uint8_t i, k;
            i = (pAttr->speed / 40) ? (pAttr->speed / 40) : 1;

            k = pAttr->speed / i;
            CLEAR_PERI_REG_MASK(SPI_CLOCK_REG(spiNum), SPI_CLK_EQU_SYSCLK);
            WRITE_PERI_REG(SPI_CLOCK_REG(spiNum),
                           (((i - 1) & SPI_CLKDIV_PRE) << SPI_CLKDIV_PRE_S) |
                           (((k - 1) & SPI_CLKCNT_N) << SPI_CLKCNT_N_S) |
                           ((((k + 1) / 2 - 1) & SPI_CLKCNT_H) << SPI_CLKCNT_H_S) |
                           (((k - 1) & SPI_CLKCNT_L) << SPI_CLKCNT_L_S)); //clear bit 31,set SPI clock div
        } else {
            WRITE_PERI_REG(SPI_CLOCK_REG(spiNum), SPI_CLK_EQU_SYSCLK); // 80Mhz speed
        }
        // Enable MOSI
        SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_CS_SETUP | SPI_CS_HOLD | SPI_USR_MOSI);

        // CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_CS_HOLD);/////////////add
        SET_PERI_REG_MASK(SPI_CTRL2_REG(spiNum), ((0x4 & SPI_MISO_DELAY_NUM) << SPI_MISO_DELAY_NUM_S)); //delay num

    } else if (SpiMode_Slave == pAttr->mode) {

        // SPI mode type
        SET_PERI_REG_MASK(SPI_SLAVE_REG(spiNum), SPI_SLAVE_MODE);
        // SPI mode type
        SET_PERI_REG_MASK(SPI_SLAVE_REG(spiNum), SPI_SLV_WR_RD_BUF_EN);
        // SPI Send buffer
        // SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MISO_HIGHPART);// By default slave send buffer C8-C15

        // If do not set delay cycles, slave not working,master cann't get the data.
        SET_PERI_REG_MASK(SPI_CTRL2_REG(spiNum), ((0x2 & SPI_MOSI_DELAY_NUM) << SPI_MOSI_DELAY_NUM_S)); //delay num
        // SPI Speed
        WRITE_PERI_REG(SPI_CLOCK_REG(spiNum), 0);

        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_CS_SETUP);/////////////add
        SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MOSI);

        // By default format::CMD(8bits)+ADDR(8bits)+DATA(32bytes)
        // set pAttr->cmdLen bit slave recieve command length
        // set 1 bytes status buffer length
        // set pAttr->addrLen bit slave recieve read address length
        // set pAttr->addrLen bit slave recieve write address length
        // set 32 bytes slave recieve buffer length
        SET_PERI_REG_BITS(SPI_USER2_REG(spiNum), SPI_USR_COMMAND_BITLEN,
                          (7), SPI_USR_COMMAND_BITLEN_S);
        SET_PERI_REG_BITS(SPI_SLAVE1_REG(spiNum), SPI_SLV_STATUS_BITLEN,
                          (7), SPI_SLV_STATUS_BITLEN_S);
        SET_PERI_REG_BITS(SPI_SLAVE1_REG(spiNum), SPI_SLV_WR_ADDR_BITLEN,
                          (7), SPI_SLV_WR_ADDR_BITLEN_S);
        SET_PERI_REG_BITS(SPI_SLAVE1_REG(spiNum), SPI_SLV_RD_ADDR_BITLEN,
                          (7), SPI_SLV_RD_ADDR_BITLEN_S);
        SET_PERI_REG_BITS(SPI_SLV_WRBUF_DLEN_REG(spiNum), SPI_SLV_WRBUF_DBITLEN,
                          (32 * 8 - 1), SPI_SLV_WRBUF_DBITLEN_S);
        SET_PERI_REG_BITS(SPI_SLV_RDBUF_DLEN_REG(spiNum), SPI_SLV_RDBUF_DBITLEN,
                          (32 * 8 - 1), SPI_SLV_RDBUF_DBITLEN_S);
    } else {
        // To do nothing
    }

    char i;
    for (i = 0; i < 16; ++i) {
        WRITE_PERI_REG((SPI_W0_REG(spiNum) + (i << 2)), 0);
    }
}

/**
 * @brief Set address value by master mode.
 *
 */
void spi_master_cfg_addr(spi_num_e spiNum, uint32_t addr)
{
    if (spiNum > SpiNum_Max) {
        return;
    }
    // Set address
    SET_PERI_REG_BITS(SPI_ADDR_REG(spiNum), SPI_USR_ADDR_VALUE, addr, SPI_USR_ADDR_VALUE_S);
}

/**
 * @brief Set command value by master mode.
 *
 */
void  spi_master_cfg_cmd(spi_num_e spiNum, uint32_t cmd)
{
    if (spiNum > SpiNum_Max) {
        return;
    }
    // SPI_USER2 bit28-31 is cmd length,cmd bit length is value(0-15)+1,
    // bit15-0 is cmd value.
    SET_PERI_REG_BITS(SPI_USER2_REG(spiNum), SPI_USR_COMMAND_VALUE, cmd, SPI_USR_COMMAND_VALUE_S);
}

/**
 * @brief Send data to slave.
 *
 */
int  spi_master_send_data(spi_num_e spiNum, spi_data_t* pInData)
{
    char idx = 0;
    if ((spiNum > SpiNum_Max)
        || (NULL == pInData)
        || (64 < pInData->txDataLen)) {
        return -1;
    }
    uint32_t *value = pInData->txData;
    while (READ_PERI_REG(SPI_CMD_REG(spiNum))&SPI_USR);
    // Set command by user.
    if (pInData->cmdLen != 0) {
        // Max command length 16 bits.
        SET_PERI_REG_BITS(SPI_USER2_REG(spiNum), SPI_USR_COMMAND_BITLEN,
                          ((pInData->cmdLen << 3) - 1), SPI_USR_COMMAND_BITLEN_S);
        // Enable command
        SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_COMMAND);
        // Load command
        spi_master_cfg_cmd(spiNum, pInData->cmd);
    } else {
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_COMMAND);
        SET_PERI_REG_BITS(SPI_USER2_REG(spiNum), SPI_USR_COMMAND_BITLEN,
                          0, SPI_USR_COMMAND_BITLEN_S);
    }
    // Set Address by user.
    if (pInData->addrLen == 0) {
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_ADDR);
        SET_PERI_REG_BITS(SPI_USER1_REG(spiNum), SPI_USR_ADDR_BITLEN,
                          0, SPI_USR_ADDR_BITLEN_S);
    } else {
        if (NULL == pInData->addr) {
            return -1;
        }
        SET_PERI_REG_BITS(SPI_USER1_REG(spiNum), SPI_USR_ADDR_BITLEN,
                          ((pInData->addrLen << 3) - 1), SPI_USR_ADDR_BITLEN_S);
        // Enable address
        SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_ADDR);
        // Load address
        spi_master_cfg_addr(spiNum, *pInData->addr);
    }
    // Set data by user.
    if (pInData->txDataLen != 0) {
        if (NULL == value) {
            return -1;
        }
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MISO);
        // Enable MOSI
        SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MOSI);
        // Load send buffer
        do {
            WRITE_PERI_REG((SPI_W0_REG(spiNum) + (idx << 2)), *value++);
        } while (++idx < ((pInData->txDataLen / 4) + ((pInData->txDataLen % 4) ? 1 : 0)));

        // Set data send buffer length.Max data length 64 bytes.
        SET_PERI_REG_BITS(SPI_MOSI_DLEN_REG(spiNum), SPI_USR_MOSI_DBITLEN, ((pInData->txDataLen << 3) - 1), SPI_USR_MOSI_DBITLEN_S);

        SET_PERI_REG_BITS(SPI_MISO_DLEN_REG(spiNum), SPI_USR_MISO_DBITLEN, ((pInData->rxDataLen << 3) - 1), SPI_USR_MISO_DBITLEN_S);
    } else {
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MOSI);
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MISO);
        SET_PERI_REG_BITS(SPI_MOSI_DLEN_REG(spiNum), SPI_USR_MOSI_DBITLEN,
                          0, SPI_USR_MOSI_DBITLEN_S);

    }
    // Start send data
    SET_PERI_REG_MASK(SPI_CMD_REG(spiNum), SPI_USR);
    while (!(READ_PERI_REG(SPI_SLAVE_REG(spiNum))&SPI_TRANS_DONE));
    CLEAR_PERI_REG_MASK(SPI_SLAVE_REG(spiNum), SPI_TRANS_DONE);
    return 0;
}

/**
 * @brief Receive data from slave.
 *
 */
int  spi_master_recv_data(spi_num_e spiNum, spi_data_t* pData)
{
    char idx = 0;
    if ((spiNum > SpiNum_Max)
        || (NULL == pData)) {
        return -1;
    }
    uint32_t *value = pData->rxData;
    while (READ_PERI_REG(SPI_CMD_REG(spiNum))&SPI_USR);
    // Set command by user.
    if (pData->cmdLen != 0) {
        // Max command length 16 bits.
        SET_PERI_REG_BITS(SPI_USER2_REG(spiNum), SPI_USR_COMMAND_BITLEN,
                          ((pData->cmdLen << 3) - 1), SPI_USR_COMMAND_BITLEN_S);
        // Enable command
        SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_COMMAND);
        // Load command
        spi_master_cfg_cmd(spiNum, pData->cmd);
    } else {
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_COMMAND);
        SET_PERI_REG_BITS(SPI_USER2_REG(spiNum), SPI_USR_COMMAND_BITLEN,
                          0, SPI_USR_COMMAND_BITLEN_S);
    }
    // Set Address by user.
    if (pData->addrLen == 0) {
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_ADDR);
        SET_PERI_REG_BITS(SPI_USER1_REG(spiNum), SPI_USR_ADDR_BITLEN,
                          0, SPI_USR_ADDR_BITLEN_S);
    } else {
        if (NULL == pData->addr) {
            return -1;
        }
        SET_PERI_REG_BITS(SPI_USER1_REG(spiNum), SPI_USR_ADDR_BITLEN,
                          ((pData->addrLen << 3) - 1), SPI_USR_ADDR_BITLEN_S);
        // Enable address
        SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_ADDR);
        // Load address
        spi_master_cfg_addr(spiNum, *pData->addr);
    }
    // Set data by user.
    if (pData->rxDataLen != 0) {
        if (NULL == value) {
            return -1;
        }
        // Clear MOSI enable
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MOSI);
        // Enable MOSI
        SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MISO);
        // Set data send buffer length.Max data length 64 bytes.
        SET_PERI_REG_BITS(SPI_MISO_DLEN_REG(spiNum), SPI_USR_MISO_DBITLEN, ((pData->rxDataLen << 3) - 1), SPI_USR_MISO_DBITLEN_S);
    } else {
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MOSI);
        CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MISO);
        SET_PERI_REG_BITS(SPI_MISO_DLEN_REG(spiNum), SPI_USR_MISO_DBITLEN, 0, SPI_USR_MISO_DBITLEN_S);
    }
    // Start send data
    SET_PERI_REG_MASK(SPI_CMD_REG(spiNum), SPI_USR);

    while (READ_PERI_REG(SPI_CMD_REG(spiNum))&SPI_USR);
    // Read data out
    do {
        *value++ =  READ_PERI_REG(SPI_W0_REG(spiNum) + (idx << 2));
    } while (++idx < ((pData->rxDataLen / 4) + ((pData->rxDataLen % 4) ? 1 : 0)));

    return 0;
}

/**
 * @brief Load data to send buffer by slave mode.

 *
 */
int  spi_slave_send_data(spi_num_e spiNum, uint32_t *pOutData, uint8_t outLen)
{
    if (NULL == pOutData) {
        return -1;
    }
    char i;
    uint32_t *value  = pOutData;
    for (i = 0; i < outLen; ++i) {
        WRITE_PERI_REG((SPI_W0_REG(spiNum) + (i << 2)), *value++);
    }
    return 0;
}

/**
 * @brief Configurate slave prepare for receive data.
 *
 */
int  spi_slave_recv_data(spi_num_e spiNum, void(*isrFunc)(void*))
{
    char idx = 0;
    if (spiNum > SpiNum_Max) {
        return -1;
    }

    spi_int_enable(spiNum, SpiIntSrc_WrStaDone
                 | SpiIntSrc_RdStaDone | SpiIntSrc_WrBufDone | SpiIntSrc_RdBufDone);
    spi_int_disable(spiNum, SpiIntSrc_TransDone);

    spi_intr_func_set(isrFunc, spiNum);
    return 0;
}

/**
 * @brief Send data to slave(ESP32,RD_STATUS or WR_STATUS).
 *
 */
void  spi_master_send_status(spi_num_e spiNum, uint8_t data)
{
    if (spiNum > SpiNum_Max) {
        return;
    }
    while (READ_PERI_REG(SPI_CMD_REG(spiNum))&SPI_USR);
    // enable MOSI
    SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MOSI);
    CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MISO | SPI_USR_DUMMY | SPI_USR_ADDR);

    // 8bits cmd, 0x04 is eps32 slave write cmd value
    WRITE_PERI_REG(SPI_USER2_REG(spiNum),
                   ((7 & SPI_USR_COMMAND_BITLEN) << SPI_USR_COMMAND_BITLEN_S)
                   | MASTER_WRITE_STATUS_TO_SLAVE_CMD);
    // Set data send buffer length.
    SET_PERI_REG_BITS(SPI_MOSI_DLEN_REG(spiNum), SPI_USR_MOSI_DBITLEN,
                      ((sizeof(data) << 3) - 1), SPI_USR_MOSI_DBITLEN_S);

    WRITE_PERI_REG(SPI_W0_REG(spiNum), (uint32_t)(data));
    // start SPI
    SET_PERI_REG_MASK(SPI_CMD_REG(spiNum), SPI_USR);
}

/**
 * @brief Receive data from slave(ESP32).
 *
 */
int  spi_master_recv_status(spi_num_e spiNum)
{
    if (spiNum > SpiNum_Max) {
        return -1;
    }

    while (READ_PERI_REG(SPI_CMD_REG(spiNum))&SPI_USR);
    // enable MISO
    SET_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MISO);
    CLEAR_PERI_REG_MASK(SPI_USER_REG(spiNum), SPI_USR_MOSI | SPI_USR_DUMMY | SPI_USR_ADDR);

    // 8bits cmd, 0x06 is eps32 slave read cmd value
    WRITE_PERI_REG(SPI_USER2_REG(spiNum),
                   ((7 & SPI_USR_COMMAND_BITLEN) << SPI_USR_COMMAND_BITLEN_S)
                   | MASTER_READ_STATUS_FROM_SLAVE_CMD);
    // Set revcive buffer length.
    SET_PERI_REG_BITS(SPI_MISO_DLEN_REG(spiNum), SPI_USR_MISO_DBITLEN,
                      7, SPI_USR_MISO_DBITLEN_S);

    // start spi module.
    SET_PERI_REG_MASK(SPI_CMD_REG(spiNum), SPI_USR);

    while (READ_PERI_REG(SPI_CMD_REG(spiNum))&SPI_USR);

    uint8_t data = (uint8_t)(READ_PERI_REG(SPI_W0_REG(spiNum)) & 0xff);
    return (uint8_t)(READ_PERI_REG(SPI_W0_REG(spiNum)) & 0xff);
}


/**
 * @brief Enable SPI interrupt source.
 *
 */
void  spi_int_enable(spi_num_e spiNum, spi_int_src_e intSrc)
{
    if (spiNum > SpiNum_Max) {
        return;
    }
    SET_PERI_REG_MASK(SPI_SLAVE_REG(spiNum), intSrc << 4);
}

/**
 * @brief Disable SPI interrupt source.
 *
 */
void  spi_int_disable(spi_num_e spiNum, spi_int_src_e intSrc)
{
    if (spiNum > SpiNum_Max) {
        return;
    }
    CLEAR_PERI_REG_MASK(SPI_SLAVE_REG(spiNum), intSrc);
}

/**
 * @brief Clear all of SPI interrupt source.
 *
 */
void  spi_int_clear(spi_num_e spiNum)
{
    if (spiNum > SpiNum_Max) {
        return;
    }
    CLEAR_PERI_REG_MASK(SPI_SLAVE_REG(spiNum), SpiIntSrc_TransDone
                        | SpiIntSrc_WrStaDone
                        | SpiIntSrc_RdStaDone
                        | SpiIntSrc_WrBufDone
                        | SpiIntSrc_RdBufDone);
}

/**
 * @brief Get the SPI interrupt status.
 *
 */
int32_t spi_int_status_get(spi_num_e i2sNum)
{
    if (i2sNum > SpiNum_Max) {
        return -1;
    }
    return READ_PERI_REG(SPI_SLAVE_REG(i2sNum));
}

#ifdef __cplusplus
}
#endif
