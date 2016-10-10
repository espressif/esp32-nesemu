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

#ifndef _DRIVER_UART_H_
#define _DRIVER_UART_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "soc/uart_reg.h"
#include <esp_types.h>


//#define ETS_UART_INTR_ENABLE()  xt_ints_on(1 << ETS_UART_INUM)
//#define ETS_UART_INTR_DISABLE() xt_ints_off(1 << ETS_UART_INUM)
#define UART_FIFO_CHAR_CNT     (128)//Do Not Change it
#define UART_INTR_MASK          0x1ff
#define UART_LINE_INV_MASK      (0x3f << 19)

typedef enum {
    UART_WordLength_5b = 0x0,
    UART_WordLength_6b = 0x1,
    UART_WordLength_7b = 0x2,
    UART_WordLength_8b = 0x3
} UART_WordLength;

typedef enum {
    USART_StopBits_1   = 0x1,
    USART_StopBits_1_5 = 0x2,
    USART_StopBits_2   = 0x3,
} UART_StopBits;

typedef enum {
    UART0 = 0x0,
    UART1 = 0x1,
    UART2 = 0x2,
    UART_MAX,
} UART_Port;

typedef enum {
    USART_Parity_None = 0x2,
    USART_Parity_Even = 0x0,
    USART_Parity_Odd  = 0x1
} UART_ParityMode;

typedef enum {
    PARITY_DIS = 0x0,
    PARITY_EN  = 0x2
} UartExistParity;

typedef enum {
    BIT_RATE_300     = 300,
    BIT_RATE_600     = 600,
    BIT_RATE_1200    = 1200,
    BIT_RATE_2400    = 2400,
    BIT_RATE_4800    = 4800,
    BIT_RATE_9600    = 9600,
    BIT_RATE_19200   = 19200,
    BIT_RATE_38400   = 38400,
    BIT_RATE_57600   = 57600,
    BIT_RATE_74880   = 74880,
    BIT_RATE_115200  = 115200,
    BIT_RATE_230400  = 230400,
    BIT_RATE_460800  = 460800,
    BIT_RATE_921600  = 921600,
    BIT_RATE_1843200 = 1843200,
    BIT_RATE_3686400 = 3686400,
} UART_BautRate; //you can add any rate you need in this range

typedef enum {
    USART_HardwareFlowControl_None    = 0x0,
    USART_HardwareFlowControl_RTS     = 0x1,
    USART_HardwareFlowControl_CTS     = 0x2,
    USART_HardwareFlowControl_CTS_RTS = 0x3
} UART_HwFlowCtrl;

typedef enum {
    UART_None_Inverse = 0x0,
    UART_Rxd_Inverse  = (uint32_t)UART_RXD_INV,
    UART_CTS_Inverse  = (uint32_t)UART_CTS_INV,
    UART_Txd_Inverse  = (uint32_t)UART_TXD_INV,
    UART_RTS_Inverse  = (uint32_t)UART_RTS_INV,
} UART_LineLevelInverse;

typedef struct {
    UART_BautRate   baud_rate;
    UART_WordLength data_bits;
    UART_ParityMode parity;    // chip size in byte
    UART_StopBits   stop_bits;
    UART_HwFlowCtrl flow_ctrl;
    uint8_t           UART_RxFlowThresh ;
    uint32_t          UART_InverseMask;
} UART_ConfigTypeDef;

typedef struct {
    uint32_t UART_IntrEnMask;
    uint8_t  UART_RX_TimeOutIntrThresh;
    uint8_t  UART_TX_FifoEmptyIntrThresh;
    uint8_t  UART_RX_FifoFullIntrThresh;
} UART_IntrConfTypeDef;
typedef enum{
    UART_TX_INTR,
    UART_RX_INTR,
}uart_event_type;

typedef struct _uart_event_message_{
    UART_Port uart_port;
	uart_event_type event_type;
	char* buffer;
	uint8_t buffer_cnt;
}uart_event_message;

typedef void (*uart_event_callback)(uart_event_message*);

//=======================================

/** \defgroup Driver_APIs Driver APIs
  * @brief Driver APIs
  */

/** @addtogroup Driver_APIs
  * @{
  */

/** \defgroup UART_Driver_APIs UART Driver APIs
  * @brief UART driver APIs
  */

/** @addtogroup UART_Driver_APIs
  * @{
  */


/**
  * @brief   Set UART baud rate.
  *
  * Example : uart_div_modify(uart_no, UART_CLK_FREQ / (UartDev.baut_rate));
  *
  * @param   UART_Port uart_no : UART0 or UART1
  * @param   uint16 div : frequency divider 
  *
  * @return  null
  */
void uart_div_modify(UART_Port uart_no, uint16_t div);

/**
  * @brief   Wait uart tx fifo empty, do not use it if tx flow control enabled.
  *
  * @param   UART_Port uart_no : UART0 or UART1
  *
  * @return  null
  */
void UART_WaitTxFifoEmpty(UART_Port uart_no); //do not use if tx flow control enabled

/**
  * @brief   Clear uart tx fifo and rx fifo.
  *
  * @param   UART_Port uart_no : UART0 or UART1
  *
  * @return  null
  */
void UART_ResetFifo(UART_Port uart_no);



/**
  * @brief   Config from which serial output printf function.
  *
  * @param   UART_Port uart_no : UART0 or UART1
  *
  * @return  null
  */
void UART_SetPrintPort(UART_Port uart_no);

/**
  * @brief   Config Common parameters of serial ports.
  *
  * @param   UART_Port uart_no : UART0 or UART1
  * @param   UART_ConfigTypeDef *pUARTConfig : parameters structure
  *
  * @return  null
  */
void UART_ParamConfig(UART_Port uart_no,  UART_ConfigTypeDef *pUARTConfig);

/**
  * @brief   Config types of uarts.
  *
  * @param   UART_Port uart_no : UART0 or UART1
  * @param   UART_IntrConfTypeDef *pUARTIntrConf : parameters structure
  *
  * @return  null
  */
void UART_IntrConfig(UART_Port uart_no,  UART_IntrConfTypeDef *pUARTIntrConf);

/**
  * @brief   Config the length of the uart communication data bits.
  *
  * @param   UART_Port uart_no : UART0 or UART1
  * @param   UART_WordLength len : the length of the uart communication data bits
  *
  * @return  null
  */
void UART_SetWordLength(UART_Port uart_no, UART_WordLength len);

/**
  * @brief   Config the length of the uart communication stop bits.
  *
  * @param   UART_Port uart_no : UART0 or UART1
  * @param   UART_StopBits bit_num : the length uart communication stop bits
  *
  * @return  null
  */
void UART_SetStopBits(UART_Port uart_no, UART_StopBits bit_num);

/**
  * @brief   Configure whether to open the parity.
  *
  * @param   UART_Port uart_no : UART0 or UART1
  * @param   UART_ParityMode Parity_mode : the enum of uart parity configuration
  *
  * @return  null
  */
void UART_SetParity(UART_Port uart_no, UART_ParityMode Parity_mode) ;

/**
  * @brief   Configure the Baud rate.
  *
  * @param   UART_Port uart_no : UART0 or UART1
  * @param   uint32_t baud_rate : the Baud rate
  *
  * @return  null
  */
void UART_SetBaudrate(UART_Port uart_no, uint32_t baud_rate);

/**
  * @brief   Configure Hardware flow control.
  *
  * @param   UART_Port uart_no : UART0 or UART1
  * @param   UART_HwFlowCtrl flow_ctrl : Hardware flow control mode
  * @param   uint8_t rx_thresh : threshold of Hardware flow control
  *
  * @return  null
  */
void UART_SetFlowCtrl(UART_Port uart_no, UART_HwFlowCtrl flow_ctrl, uint8_t rx_thresh);

/**
  * @brief   Configure trigging signal of uarts.
  *
  * @param   UART_Port uart_no : UART0 or UART1
  * @param   UART_LineLevelInverse inverse_mask : Choose need to flip the IO
  *
  * @return  null
  */
void UART_SetLineInverse(UART_Port uart_no, UART_LineLevelInverse inverse_mask) ;

/**
  * @brief   An example illustrates how to configure the serial port.
  *
  * @param   null
  *
  * @return  null
  */
uint32_t uart_tx_chars(UART_Port uart_no,char* buffer,uint32_t len);
void Uart_EnableRxIntr(UART_Port uart_no);
void Uart_DisableRxIntr(UART_Port uart_no);
void Uart_EnableTxIntr(UART_Port uart_no);
void Uart_DisableTxIntr(UART_Port uart_no);


void uart_init_default(void);

void UART_register_event_callback(uart_event_callback func);




/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
