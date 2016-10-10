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

#ifndef _DRIVER_UART_STRUCT_H_
#define _DRIVER_UART_STRUCT_H_
#include "soc/uart_reg.h"
typedef struct UART_DEV{
    union {
        struct {
            unsigned int rxfifo_rd_byte: 8;
            unsigned int reserved8: 24;
        } fld;
        uint32_t val;
    }FIFO;
    union {
        struct {
            unsigned int rxfifo_full_int_raw: 1;
            unsigned int txfifo_empty_int_raw: 1;
            unsigned int parity_err_int_raw: 1;
            unsigned int frm_err_int_raw: 1;
            unsigned int rxfifo_ovf_int_raw: 1;
            unsigned int dsr_chg_int_raw: 1;
            unsigned int cts_chg_int_raw: 1;
            unsigned int brk_det_int_raw: 1;
            unsigned int rxfifo_tout_int_raw: 1;
            unsigned int sw_xon_int_raw: 1;
            unsigned int sw_xoff_int_raw: 1;
            unsigned int glitch_det_int_raw: 1;
            unsigned int tx_brk_done_int_raw: 1;
            unsigned int tx_brk_idle_done_int_raw: 1;
            unsigned int tx_done_int_raw: 1;
            unsigned int rs485_parity_err_int_raw: 1;
            unsigned int rs485_frm_err_int_raw: 1;
            unsigned int rs485_clash_int_raw: 1;
            unsigned int at_cmd_char_det_int_raw: 1;
            unsigned int reserved19: 13;
        } fld;
        uint32_t val;
    }INT_RAW;
    union {
        struct {
            unsigned int rxfifo_full_int_st: 1;
            unsigned int txfifo_empty_int_st: 1;
            unsigned int parity_err_int_st: 1;
            unsigned int frm_err_int_st: 1;
            unsigned int rxfifo_ovf_int_st: 1;
            unsigned int dsr_chg_int_st: 1;
            unsigned int cts_chg_int_st: 1;
            unsigned int brk_det_int_st: 1;
            unsigned int rxfifo_tout_int_st: 1;
            unsigned int sw_xon_int_st: 1;
            unsigned int sw_xoff_int_st: 1;
            unsigned int glitch_det_int_st: 1;
            unsigned int tx_brk_done_int_st: 1;
            unsigned int tx_brk_idle_done_int_st: 1;
            unsigned int tx_done_int_st: 1;
            unsigned int rs485_parity_err_int_st: 1;
            unsigned int rs485_frm_err_int_st: 1;
            unsigned int rs485_clash_int_st: 1;
            unsigned int at_cmd_char_det_int_st: 1;
            unsigned int reserved19: 13;
        } fld;
        uint32_t val;
    }INT_ST;
    union {
        struct {
            unsigned int rxfifo_full_int_ena: 1;
            unsigned int txfifo_empty_int_ena: 1;
            unsigned int parity_err_int_ena: 1;
            unsigned int frm_err_int_ena: 1;
            unsigned int rxfifo_ovf_int_ena: 1;
            unsigned int dsr_chg_int_ena: 1;
            unsigned int cts_chg_int_ena: 1;
            unsigned int brk_det_int_ena: 1;
            unsigned int rxfifo_tout_int_ena: 1;
            unsigned int sw_xon_int_ena: 1;
            unsigned int sw_xoff_int_ena: 1;
            unsigned int glitch_det_int_ena: 1;
            unsigned int tx_brk_done_int_ena: 1;
            unsigned int tx_brk_idle_done_int_ena: 1;
            unsigned int tx_done_int_ena: 1;
            unsigned int rs485_parity_err_int_ena: 1;
            unsigned int rs485_frm_err_int_ena: 1;
            unsigned int rs485_clash_int_ena: 1;
            unsigned int at_cmd_char_det_int_ena: 1;
            unsigned int reserved19: 13;
        } fld;
        uint32_t val;
    }INT_ENA;
    union {
        struct {
            unsigned int rxfifo_full_int_clr: 1;
            unsigned int txfifo_empty_int_clr: 1;
            unsigned int parity_err_int_clr: 1;
            unsigned int frm_err_int_clr: 1;
            unsigned int rxfifo_ovf_int_clr: 1;
            unsigned int dsr_chg_int_clr: 1;
            unsigned int cts_chg_int_clr: 1;
            unsigned int brk_det_int_clr: 1;
            unsigned int rxfifo_tout_int_clr: 1;
            unsigned int sw_xon_int_clr: 1;
            unsigned int sw_xoff_int_clr: 1;
            unsigned int glitch_det_int_clr: 1;
            unsigned int tx_brk_done_int_clr: 1;
            unsigned int tx_brk_idle_done_int_clr: 1;
            unsigned int tx_done_int_clr: 1;
            unsigned int rs485_parity_err_int_clr: 1;
            unsigned int rs485_frm_err_int_clr: 1;
            unsigned int rs485_clash_int_clr: 1;
            unsigned int at_cmd_char_det_int_clr: 1;
            unsigned int reserved19: 13;
        } fld;
        uint32_t val;
    }INT_CLR;
    union {
        struct {
            unsigned int uart_clkdiv: 20;
            unsigned int uart_clkdiv_frag: 4;
            unsigned int reserved24: 8;
        } fld;
        uint32_t val;
    }CLKDIV;
    union {
        struct {
            unsigned int autobaud_en: 1;
            unsigned int reserved1: 7;
            unsigned int glitch_filt: 8;
            unsigned int reserved16: 16;
        } fld;
        uint32_t val;
    }AUTOBAUD;
    union {
        struct {
            unsigned int rxfifo_cnt: 8;
            unsigned int st_urx_out: 4;
            unsigned int reserved12: 1;
            unsigned int dsrn: 1;
            unsigned int ctsn: 1;
            unsigned int rxd: 1;
            unsigned int txfifo_cnt: 8;
            unsigned int st_utx_out: 4;
            unsigned int reserved28: 1;
            unsigned int dtrn: 1;
            unsigned int rtsn: 1;
            unsigned int txd: 1;
        } fld;
        uint32_t val;
    }STATUS;
    union {
        struct {
            unsigned int parity: 1;
            unsigned int parity_en: 1;
            unsigned int bit_num: 2;
            unsigned int stop_bit_num: 2;
            unsigned int sw_rts: 1;
            unsigned int sw_dtr: 1;
            unsigned int txd_brk: 1;
            unsigned int irda_dplx: 1;
            unsigned int irda_tx_en: 1;
            unsigned int irda_wctl: 1;
            unsigned int irda_tx_inv: 1;
            unsigned int irda_rx_inv: 1;
            unsigned int uart_loopback: 1;
            unsigned int tx_flow_en: 1;
            unsigned int irda_en: 1;
            unsigned int rxfifo_rst: 1;
            unsigned int txfifo_rst: 1;
            unsigned int uart_rxd_inv: 1;
            unsigned int uart_cts_inv: 1;
            unsigned int uart_dsr_inv: 1;
            unsigned int uart_txd_inv: 1;
            unsigned int uart_rts_inv: 1;
            unsigned int uart_dtr_inv: 1;
            unsigned int reg_clk_en: 1;
            unsigned int uart_err_wr_mask: 1;
            unsigned int uart_tick_ref_always_on: 1;
            unsigned int reserved28: 4;
        } fld;
        uint32_t val;
    }CONF0;
    union {
        struct {
            unsigned int rxfifo_full_thrhd: 7;
            unsigned int reserved7: 1;
            unsigned int txfifo_empty_thrhd: 7;
            unsigned int reserved15: 1;
            unsigned int rx_flow_thrhd: 7;
            unsigned int rx_flow_en: 1;
            unsigned int rx_tout_thrhd: 7;
            unsigned int rx_tout_en: 1;
        } fld;
        uint32_t val;
    }CONF1;
    union {
        struct {
            unsigned int lowpulse_min_cnt: 20;
            unsigned int reserved20: 12;
        } fld;
        uint32_t val;
    }LOWPULSE;
    union {
        struct {
            unsigned int highpulse_min_cnt: 20;
            unsigned int reserved20: 12;
        } fld;
        uint32_t val;
    }HIGHPULSE;
    union {
        struct {
            unsigned int rxd_edge_cnt: 10;
            unsigned int reserved10: 22;
        } fld;
        uint32_t val;
    }RXD_CNT;
    union {
        struct {
            unsigned int uart_sw_flow_con_en: 1;
            unsigned int uart_xonoff_del: 1;
            unsigned int uart_force_xon: 1;
            unsigned int uart_force_xoff: 1;
            unsigned int uart_send_xon: 1;
            unsigned int uart_send_xoff: 1;
            unsigned int reserved6: 26;
        } fld;
        uint32_t val;
    }FLOW_CONF;
    union {
        struct {
            unsigned int uart_active_threshold: 10;
            unsigned int reserved10: 22;
        } fld;
        uint32_t val;
    }SLEEP_CONF;
    union {
        struct {
            unsigned int uart_xon_threshold: 8;
            unsigned int uart_xoff_threshold: 8;
            unsigned int uart_xon_char: 8;
            unsigned int uart_xoff_char: 8;
        } fld;
        uint32_t val;
    }SWFC_CONF;
    union {
        struct {
            unsigned int rx_idle_thrhd: 10;
            unsigned int uart_tx_idle_num: 10;
            unsigned int uart_tx_brk_num: 8;
            unsigned int reserved28: 4;
        } fld;
        uint32_t val;
    }IDLE_CONF;
    union {
        struct {
            unsigned int rs485_en: 1;
            unsigned int dl0_en: 1;
            unsigned int dl1_en: 1;
            unsigned int rs485tx_rx_en: 1;
            unsigned int rs485rxby_tx_en: 1;
            unsigned int rs485_rx_dly_num: 1;
            unsigned int rs485_tx_dly_num: 4;
            unsigned int reserved10: 22;
        } fld;
        uint32_t val;
    }RS485_CONF;
    union {
        struct {
            unsigned int pre_idle_num: 24;
            unsigned int reserved24: 8;
        } fld;
        uint32_t val;
    }AT_CMD_PRECNT;
    union {
        struct {
            unsigned int post_idle_num: 24;
            unsigned int reserved24: 8;
        } fld;
        uint32_t val;
    }AT_CMD_POSTCNT;
    union {
        struct {
            unsigned int rx_gap_tout: 24;
            unsigned int reserved24: 8;
        } fld;
        uint32_t val;
    }AT_CMD_GAPTOUT;
    union {
        struct {
            unsigned int at_cmd_char: 8;
            unsigned int char_num: 8;
            unsigned int reserved16: 16;
        } fld;
        uint32_t val;
    }AT_CMD_CHAR;
    union {
        struct {
            unsigned int reg_mem_pd: 1;
            unsigned int reserved1: 1;
            unsigned int reserved2: 1;
            unsigned int reg_rx_size: 4;
            unsigned int reg_tx_size: 4;
            unsigned int reserved11: 4;
            unsigned int rx_flow_thrhd_h3: 3;
            unsigned int rx_tout_thrhd_h3: 3;
            unsigned int uart_xon_threshold_h2: 2;
            unsigned int uart_xoff_threshold_h2: 2;
            unsigned int rx_mem_full_thrhd: 3;
            unsigned int tx_mem_empty_thrhd: 3;
            unsigned int reserved31: 1;
        } fld;
        uint32_t val;
    }MEM_CONF;
    union {
        struct {
            unsigned int mem_tx_status: 24;
            unsigned int reserved24: 8;
        } fld;
        uint32_t val;
    }MEM_TX_STATUS;
    union {
        struct {
            unsigned int mem_rx_status: 24;
            unsigned int reserved24: 8;
        } fld;
        uint32_t val;
    }MEM_RX_STATUS;
    union {
        struct {
            unsigned int rx_mem_cnt: 3;
            unsigned int tx_mem_cnt: 3;
            unsigned int reserved6: 26;
        } fld;
        uint32_t val;
    }MEM_CNT_STATUS;
    union {
        struct {
            unsigned int posedge_min_cnt: 20;
            unsigned int reserved20: 12;
        } fld;
        uint32_t val;
    }POSPULSE;
    union {
        struct {
            unsigned int negedge_min_cnt: 20;
            unsigned int reserved20: 12;
        } fld;
        uint32_t val;
    }NEGPULSE;
    uint32_t reserved_70;
    uint32_t reserved_74;
    union {
        struct {
            unsigned int uart_date: 32;
        } fld;
        uint32_t val;
    }DATE;
    union {
        struct {
            unsigned int uart_id: 32;
        } fld;
        uint32_t val;
    }ID;
}UART_TypeDef;

#define UART(i)   ((UART_TypeDef*)REG_UART_BASE(i))

#endif
