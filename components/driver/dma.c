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

#include "dma.h"
#include "rom/ets_sys.h"
#include "string.h"
#include "stdlib.h"

#define QUEUE_BLOCK_LENGTH  (4096L)

#if 0
// For test
static uint32 flag = 0;
dma_queue_t quespi0, quespi1;
dma_queue_t quespi10, quespi11;
ping_pong_buf_t PingPongBuf0spi;
uint8_t bufspi[2048] = {0};

ping_pong_buf_t PingPongBuf1spi;
uint8_t bufspi1[2048] = {0};

dma_element_t dmaEle0, dmaEle1;
dma_element_t dmaEle10, dmaEle11;
#endif
void SHOWQUEUESPI(ping_pong_buf_t *pcfg)
{
    uint32_t i;
    ets_printf("obj=%x pingbuffer=%x pongbuf=%x \r\n",
              pcfg, pcfg->ping->bufAdr, pcfg->pong->bufAdr);
    ets_printf("pingQue first=%x %08x last=%x %08x\r\n",
              pcfg->ping->firstQueue, *pcfg->ping->firstQueue, pcfg->ping->lastQueue, *pcfg->ping->lastQueue);
    ets_printf("pongQue first=%x %08x last=%x %08x\r\n",
              pcfg->pong->firstQueue, *pcfg->ping->firstQueue, pcfg->pong->lastQueue, *pcfg->pong->lastQueue);
    dma_queue_t *addr = (dma_queue_t*)pcfg->ping->firstQueue;
    dma_queue_t *addr1 = (dma_queue_t*)pcfg->pong->firstQueue;
    for (i = 0; i < pcfg->queueCnt; ++i) {
        ets_printf("ping queue%d buf:%08x,len=%d,size=%d,cut_link:%08x,next_link:%08x\r\n", i,
                  addr->buf_ptr, addr->datalen, addr->blocksize, addr, addr->next_link_ptr);
        addr = (dma_queue_t*)addr->next_link_ptr;
    }
    for (i = 0; i < pcfg->queueCnt; ++i) {
        ets_printf("pong queue%d buf:%08x,len=%d,size=%d,cut_link:%08x,next_link:%08x\r\n", i,
                  addr1->buf_ptr, addr1->datalen, addr1->blocksize, addr1, addr1->next_link_ptr);
        addr1 = (dma_queue_t*)addr1->next_link_ptr;
    }
}

/**
 * @brief Fill a link
 *
 */
static void fill_one_link(uint8_t own, uint8_t eof, uint8_t sub_sof, uint16_t size, uint16_t length,
                          uint32_t *buf_ptr, dma_queue_t *nxt_ptr, dma_queue_t *i2s_queue)
{
    i2s_queue->owner = own;
    i2s_queue->eof = eof;
    i2s_queue->sub_sof = sub_sof;
    i2s_queue->datalen = 0x0FFF & length;
    i2s_queue->blocksize = size ;
    i2s_queue->buf_ptr = (uint32_t)buf_ptr;
    i2s_queue->next_link_ptr = (uint32_t)nxt_ptr;
    i2s_queue->unused = 0;
}

/**
 * @brief Fill the queue
 *
 */
static int dma_queue_fill(uint32_t cnt, uint32_t len, ping_pong_buf_t *cfg)
{
    if (0 == cnt) {
        return -1;
    }
    // ping queue list
    dma_queue_t *pingAry[cnt];
    // pong queue list
    dma_queue_t *pongAry[cnt];
    uint32_t i, j;
    memset(&pingAry, 0, sizeof(pingAry));
    memset(&pongAry, 0, sizeof(pongAry));
    cnt = 1;
    for (i = 0; i < cnt; ++i) {
        // if (flag == 0)
        //    pingAry[i] = &quespi0;
        // else
        //    pingAry[i] = &quespi10;
        pingAry[i] = (dma_queue_t*)malloc(sizeof(dma_queue_t));
        // LOGD("ping queue list%d=%x %x %x\r\n", i, pingAry[i], &pingAry[i]->buf_ptr, &pingAry[i]->next_link_ptr);
        if (NULL == pingAry[i]) {
            for (j = 0; j < i; ++j) {
                free(pingAry[j]);
                pingAry[j] = NULL;
            }
            return -2;
        }
    }
    for (i = 0; i < cnt; ++i) {
        // if (flag == 0)
        //     pongAry[i] = &quespi1;
        // else
        //     pongAry[i] = &quespi11;

        pongAry[i] = (dma_queue_t*)malloc(sizeof(dma_queue_t));
        //LOGD("pong queue list%d=%x %x %x\r\n", i, pongAry[i], &pongAry[i]->buf_ptr, &pongAry[i]->next_link_ptr);
        if (NULL == pongAry[i]) {
            for (j = 0; j < cnt; ++j) {
                free(pingAry[j]);
                pingAry[j] = NULL;
            }
            for (j = 0; j < i; ++j) {
                free(pongAry[j]);
                pongAry[j] = NULL;
            }
            return -2;
        }
    }

    cfg->ping->firstQueue = pingAry[0];
    cfg->pong->firstQueue = pongAry[0];
    if (1 == cnt) {
        cfg->ping->lastQueue = pingAry[0];
        cfg->pong->lastQueue = pongAry[0];
    } else {
        cfg->ping->lastQueue = pingAry[cnt - 1];
        cfg->pong->lastQueue = pongAry[cnt - 1];
    }
    uint32_t remainSize = len;
    uint32_t bufSize = QUEUE_BLOCK_LENGTH;
    for (i = 0; i < cnt; ++i) {
        if (1 == cnt) {
            // Queue list include only one link, and set up eof bit.
            if (QUEUE_BLOCK_LENGTH == len) {
                bufSize = len - 1;
            } else {
                bufSize = len;
            }
            fill_one_link(1, 1, 0, bufSize, bufSize, cfg->ping->bufAdr, pongAry[i], pingAry[i]);
            fill_one_link(1, 1, 0, bufSize, bufSize, cfg->pong->bufAdr, pingAry[i], pongAry[i]);
        } else {
            if (i == (cnt - 1)) {
                // ping/pong queue list last link connect to the pong/ping first link, and set up eof bit.
                bufSize = remainSize;
                fill_one_link(1, 1, 0, bufSize, bufSize, cfg->ping->bufAdr + ((QUEUE_BLOCK_LENGTH / sizeof(uint32_t)) * i),
                              pongAry[0], pingAry[i]);
                fill_one_link(1, 1, 0, bufSize, bufSize, cfg->pong->bufAdr + ((QUEUE_BLOCK_LENGTH / sizeof(uint32_t)) * i),
                              pingAry[0], pongAry[i]);
            } else {
                // Conncet the next link.
                fill_one_link(1, 0, 0, bufSize - 1, bufSize - 1, cfg->ping->bufAdr + ((QUEUE_BLOCK_LENGTH / sizeof(uint32_t)) * i), pingAry[i + 1], pingAry[i]);
                fill_one_link(1, 0, 0, bufSize - 1, bufSize - 1, cfg->pong->bufAdr + ((QUEUE_BLOCK_LENGTH / sizeof(uint32_t)) * i), pongAry[i + 1], pongAry[i]);
            }
        }
        remainSize -= bufSize;
    }
    return 0;
}


/**
 * @brief Create a ping-pong buffer object used by DMA.
 *
 */
ping_pong_buf_t* dma_buf_create(uint32_t bufLen)
{
    if (0 == bufLen) {
        return NULL;
    }
    uint32_t i, j;
    uint32_t queueCnt ;
    uint8_t * pBuf = NULL;
    i = bufLen / QUEUE_BLOCK_LENGTH;
    j = bufLen % QUEUE_BLOCK_LENGTH;
    if (0 == j) {
        queueCnt = i;
    } else {
        queueCnt = i + 1;
    }
    // if (flag > 1) {
    //     flag = 0;
    // }
    ets_printf("\r\nbufLen=%d queueCnt=%d\r\n", bufLen, queueCnt);

    ping_pong_buf_t* pcfg;
    pcfg = (ping_pong_buf_t*)malloc(sizeof(ping_pong_buf_t));
    // if (flag == 0)
    //     pcfg = &PingPongBuf0spi;//
    // else
    //     pcfg = &PingPongBuf1spi;//

    if (NULL == pcfg) {
        return NULL;
    }
//    if (flag == 0) {
//        pBuf = bufspi; // ((uint8_t*)malloc(bufLen * 2)); // buflen is number of bytes buffer.malloc ping and pong buffer.
//        pcfg->ping = &dmaEle0;
//        pcfg->pong = &dmaEle1;
//    } else {
//        pBuf = bufspi1; // ((uint8_t*)malloc(bufLen * 2)); // buflen is number of bytes buffer.malloc ping and pong buffer.
//        pcfg->ping = &dmaEle10;
//        pcfg->pong = &dmaEle11;
//    }
    pBuf = ((uint8_t*)malloc(bufLen * 2)); // buflen is number of bytes buffer.malloc ping and pong buffer.
    pcfg->ping = (dma_element_t*)malloc(sizeof(dma_element_t));
    pcfg->pong = (dma_element_t*)malloc(sizeof(dma_element_t));
    if ((NULL == pBuf)
        || (NULL == pcfg->pong)
        || (NULL == pcfg->ping)) {

        if (pBuf) {free(pBuf); pBuf = NULL;}
        if (pcfg->pong){ free(pcfg->pong); pcfg->pong = NULL;}
        if (pcfg->ping){ free(pcfg->ping); pcfg->ping = NULL;}
        if (pcfg) {free(pcfg); pcfg = NULL;}
        ets_printf("Malloc ping->bufAdr failed");
        return NULL;
    }
    //
    memset(pBuf, 0, (bufLen * 2));
    pcfg->ping->bufAdr = (uint32_t*)pBuf;
    pcfg->pong->bufAdr = (uint32_t*)(pBuf + bufLen);

    if (dma_queue_fill(queueCnt, bufLen, pcfg) < 0) {
        free(pcfg->ping->bufAdr);
        pcfg->ping->bufAdr = NULL;
        pcfg->pong->bufAdr = NULL;
        if (pBuf) {free(pBuf); pBuf = NULL;}
        if (pcfg->pong){ free(pcfg->pong); pcfg->pong = NULL;}
        if (pcfg->ping){ free(pcfg->ping); pcfg->ping = NULL;}
        if (pcfg) {free(pcfg); pcfg = NULL;}
        return NULL;
    }
    pcfg->len = bufLen;         // Buffer length
    pcfg->queueCnt = queueCnt;  // Number of queue
 //   flag++;

    SHOWQUEUESPI(pcfg);
    return pcfg;
}

/**
 * @brief Reset the dma buffer length.
 *
 */
int dma_buf_len_reset(ping_pong_buf_t *obj)
{
    if (NULL == obj) {
        return -1;
    }
    dma_queue_t *dmaQueCur = obj->ping->firstQueue;
    dma_queue_t *dmaQueNext = NULL;

    ets_printf("next_link_ptr=%x \r\n", obj->backupQue.next_link_ptr);
    if (obj->backupQue.next_link_ptr) {
        while (dmaQueNext != obj->ping->firstQueue) {
            dmaQueNext = (dma_queue_t*)dmaQueCur->next_link_ptr;
            if ((dma_queue_t*)obj->backupQue.next_link_ptr == dmaQueNext) {
                ets_printf("find next_link_ptr=%x \r\n", dmaQueNext);
                break;
            }
            dmaQueCur = dmaQueNext;
        }
        memcpy(dmaQueCur, &obj->backupQue, sizeof(obj->backupQue));
        memset(&obj->backupQue, 0, sizeof(obj->backupQue));
    }
    SHOWQUEUESPI(obj);
    return 0;
}

/**
 * @brief Set the buffer length before the start.
 *
 */
int dma_buf_len_set(ping_pong_buf_t *obj, uint32_t len)
{
    if (NULL == obj) {
        return -1;
    }
    if (len < obj->len) {
        int i, k, cnt;
        i = len / QUEUE_BLOCK_LENGTH;
        k = len % QUEUE_BLOCK_LENGTH;
        if (0 == k) {
            cnt = i;
        } else {
            cnt = i + 1;
        }
        dma_queue_t *dmaQueCur = obj->ping->firstQueue;
        dma_queue_t *dmaQueNext = NULL;
        while (--cnt) {
            dmaQueNext = (dma_queue_t*)dmaQueCur->next_link_ptr;
            dmaQueCur = dmaQueNext;
        }
        memcpy(&obj->backupQue, dmaQueCur, sizeof(obj->backupQue));
        dmaQueCur->next_link_ptr = 0;
        dmaQueCur->datalen = k;
    }
    return len;
}

/**
 * @brief Destroy the ping-pong buffer instance.
 *
 */
void dma_buf_destroy(ping_pong_buf_t *obj)
{
    ping_pong_buf_t *temp = obj;
    SHOWQUEUESPI(temp);
    if (NULL != temp) {
        // Free the link list
        uint32_t i = 0;
        dma_queue_t *curtCfg = temp->ping->firstQueue;
        dma_queue_t *nextCfg = NULL;
        for (i = 0; i < temp->queueCnt; ++i) {
            nextCfg = (dma_queue_t*)curtCfg->next_link_ptr;
            //  LOGD("i=%d curtCfg=%x,nextCfg=0x%x", i, curtCfg, nextCfg);
            free(curtCfg);
            curtCfg =  NULL;
            curtCfg = nextCfg;
        }
        curtCfg = temp->pong->firstQueue;
        nextCfg = NULL;
        for (i = 0; i < temp->queueCnt; ++i) {
            nextCfg = (dma_queue_t*)curtCfg->next_link_ptr;
            // LOGD("i=%d curtCfg=%x,nextCfg=0x%x", i, curtCfg, nextCfg);
              free(curtCfg);
            curtCfg =  NULL;
            curtCfg = nextCfg;
        }
        // Free the buffer
        free(temp->ping->bufAdr);
        temp->ping->bufAdr = NULL;
        free(temp->ping);
        temp->ping = NULL;
        free(temp->pong);
        temp->pong = NULL;
        free(temp);
        temp = NULL;
    }
}
