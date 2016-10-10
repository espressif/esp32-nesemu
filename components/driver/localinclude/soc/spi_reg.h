// Copyright 2010-2016 Espressif Systems (Shanghai) PTE LTD
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

#ifndef _SOC_SPI_REG_H_
#define _SOC_SPI_REG_H_

#include "soc/soc.h"


#define SPI_CMD_REG(i)          (REG_SPI_BASE(i) + 0x0)
    /* SPI_FLASH_READ : R/W ;bitpos:[31] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_READ  (BIT(31))
    #define SPI_FLASH_READ_M  (BIT(31))
    #define SPI_FLASH_READ_V  0x1
    #define SPI_FLASH_READ_S  31
    /* SPI_FLASH_WREN : R/W ;bitpos:[30] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_WREN  (BIT(30))
    #define SPI_FLASH_WREN_M  (BIT(30))
    #define SPI_FLASH_WREN_V  0x1
    #define SPI_FLASH_WREN_S  30
    /* SPI_FLASH_WRDI : R/W ;bitpos:[29] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_WRDI  (BIT(29))
    #define SPI_FLASH_WRDI_M  (BIT(29))
    #define SPI_FLASH_WRDI_V  0x1
    #define SPI_FLASH_WRDI_S  29
    /* SPI_FLASH_RDID : R/W ;bitpos:[28] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_RDID  (BIT(28))
    #define SPI_FLASH_RDID_M  (BIT(28))
    #define SPI_FLASH_RDID_V  0x1
    #define SPI_FLASH_RDID_S  28
    /* SPI_FLASH_RDSR : R/W ;bitpos:[27] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_RDSR  (BIT(27))
    #define SPI_FLASH_RDSR_M  (BIT(27))
    #define SPI_FLASH_RDSR_V  0x1
    #define SPI_FLASH_RDSR_S  27
    /* SPI_FLASH_WRSR : R/W ;bitpos:[26] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_WRSR  (BIT(26))
    #define SPI_FLASH_WRSR_M  (BIT(26))
    #define SPI_FLASH_WRSR_V  0x1
    #define SPI_FLASH_WRSR_S  26
    /* SPI_FLASH_PP : R/W ;bitpos:[25] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_PP  (BIT(25))
    #define SPI_FLASH_PP_M  (BIT(25))
    #define SPI_FLASH_PP_V  0x1
    #define SPI_FLASH_PP_S  25
    /* SPI_FLASH_SE : R/W ;bitpos:[24] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_SE  (BIT(24))
    #define SPI_FLASH_SE_M  (BIT(24))
    #define SPI_FLASH_SE_V  0x1
    #define SPI_FLASH_SE_S  24
    /* SPI_FLASH_BE : R/W ;bitpos:[23] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_BE  (BIT(23))
    #define SPI_FLASH_BE_M  (BIT(23))
    #define SPI_FLASH_BE_V  0x1
    #define SPI_FLASH_BE_S  23
    /* SPI_FLASH_CE : R/W ;bitpos:[22] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_CE  (BIT(22))
    #define SPI_FLASH_CE_M  (BIT(22))
    #define SPI_FLASH_CE_V  0x1
    #define SPI_FLASH_CE_S  22
    /* SPI_FLASH_DP : R/W ;bitpos:[21] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_DP  (BIT(21))
    #define SPI_FLASH_DP_M  (BIT(21))
    #define SPI_FLASH_DP_V  0x1
    #define SPI_FLASH_DP_S  21
    /* SPI_FLASH_RES : R/W ;bitpos:[20] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_RES  (BIT(20))
    #define SPI_FLASH_RES_M  (BIT(20))
    #define SPI_FLASH_RES_V  0x1
    #define SPI_FLASH_RES_S  20
    /* SPI_FLASH_HPM : R/W ;bitpos:[19] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_HPM  (BIT(19))
    #define SPI_FLASH_HPM_M  (BIT(19))
    #define SPI_FLASH_HPM_V  0x1
    #define SPI_FLASH_HPM_S  19
    /* SPI_USR : R/W ;bitpos:[18] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR  (BIT(18))
    #define SPI_USR_M  (BIT(18))
    #define SPI_USR_V  0x1
    #define SPI_USR_S  18
    /* SPI_FLASH_PES : R/W ;bitpos:[17] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_PES  (BIT(17))
    #define SPI_FLASH_PES_M  (BIT(17))
    #define SPI_FLASH_PES_V  0x1
    #define SPI_FLASH_PES_S  17
    /* SPI_FLASH_PER : R/W ;bitpos:[16] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FLASH_PER  (BIT(16))
    #define SPI_FLASH_PER_M  (BIT(16))
    #define SPI_FLASH_PER_V  0x1
    #define SPI_FLASH_PER_S  16



#define SPI_ADDR_REG(i)          (REG_SPI_BASE(i) + 0x4)
    /* SPI_USR_ADDR_VALUE : R/W ;bitpos:[31:0] ;default: 32'h0 ; */
    /*description: */
    #define SPI_USR_ADDR_VALUE  0xFFFFFFFF
    #define SPI_USR_ADDR_VALUE_M  ((SPI_USR_ADDR_VALUE_V)<<(SPI_USR_ADDR_VALUE_S))
    #define SPI_USR_ADDR_VALUE_V  0xFFFFFFFF
    #define SPI_USR_ADDR_VALUE_S  0



#define SPI_CTRL_REG(i)          (REG_SPI_BASE(i) + 0x8)
    /* SPI_WR_BIT_ORDER : R/W ;bitpos:[26] ;default: 1'b0 ; */
    /*description: */
    #define SPI_WR_BIT_ORDER  (BIT(26))
    #define SPI_WR_BIT_ORDER_M  (BIT(26))
    #define SPI_WR_BIT_ORDER_V  0x1
    #define SPI_WR_BIT_ORDER_S  26
    /* SPI_RD_BIT_ORDER : R/W ;bitpos:[25] ;default: 1'b0 ; */
    /*description: */
    #define SPI_RD_BIT_ORDER  (BIT(25))
    #define SPI_RD_BIT_ORDER_M  (BIT(25))
    #define SPI_RD_BIT_ORDER_V  0x1
    #define SPI_RD_BIT_ORDER_S  25
    /* SPI_FREAD_QIO : R/W ;bitpos:[24] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FREAD_QIO  (BIT(24))
    #define SPI_FREAD_QIO_M  (BIT(24))
    #define SPI_FREAD_QIO_V  0x1
    #define SPI_FREAD_QIO_S  24
    /* SPI_FREAD_DIO : R/W ;bitpos:[23] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FREAD_DIO  (BIT(23))
    #define SPI_FREAD_DIO_M  (BIT(23))
    #define SPI_FREAD_DIO_V  0x1
    #define SPI_FREAD_DIO_S  23
    /* SPI_WRSR_2B : R/W ;bitpos:[22] ;default: 1'b0 ; */
    /*description: */
    #define SPI_WRSR_2B  (BIT(22))
    #define SPI_WRSR_2B_M  (BIT(22))
    #define SPI_WRSR_2B_V  0x1
    #define SPI_WRSR_2B_S  22
    /* SPI_WP_REG : R/W ;bitpos:[21] ;default: 1'b1 ; */
    /*description: */
    #define SPI_WP_REG  (BIT(21))
    #define SPI_WP_REG_M  (BIT(21))
    #define SPI_WP_REG_V  0x1
    #define SPI_WP_REG_S  21
    /* SPI_FREAD_QUAD : R/W ;bitpos:[20] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FREAD_QUAD  (BIT(20))
    #define SPI_FREAD_QUAD_M  (BIT(20))
    #define SPI_FREAD_QUAD_V  0x1
    #define SPI_FREAD_QUAD_S  20
    /* SPI_RESANDRES : R/W ;bitpos:[15] ;default: 1'b1 ; */
    /*description: */
    #define SPI_RESANDRES  (BIT(15))
    #define SPI_RESANDRES_M  (BIT(15))
    #define SPI_RESANDRES_V  0x1
    #define SPI_RESANDRES_S  15
    /* SPI_FREAD_DUAL : R/W ;bitpos:[14] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FREAD_DUAL  (BIT(14))
    #define SPI_FREAD_DUAL_M  (BIT(14))
    #define SPI_FREAD_DUAL_V  0x1
    #define SPI_FREAD_DUAL_S  14
    /* SPI_FASTRD_MODE : R/W ;bitpos:[13] ;default: 1'b1 ; */
    /*description: */
    #define SPI_FASTRD_MODE  (BIT(13))
    #define SPI_FASTRD_MODE_M  (BIT(13))
    #define SPI_FASTRD_MODE_V  0x1
    #define SPI_FASTRD_MODE_S  13
    /* SPI_WAIT_FLASH_IDLE_EN : R/W ;bitpos:[12] ;default: 1'b0 ; */
    /*description: */
    #define SPI_WAIT_FLASH_IDLE_EN  (BIT(12))
    #define SPI_WAIT_FLASH_IDLE_EN_M  (BIT(12))
    #define SPI_WAIT_FLASH_IDLE_EN_V  0x1
    #define SPI_WAIT_FLASH_IDLE_EN_S  12
    /* SPI_TX_CRC_EN : R/W ;bitpos:[11] ;default: 1'b0 ; */
    /*description: */
    #define SPI_TX_CRC_EN  (BIT(11))
    #define SPI_TX_CRC_EN_M  (BIT(11))
    #define SPI_TX_CRC_EN_V  0x1
    #define SPI_TX_CRC_EN_S  11
    /* SPI_FCS_CRC_EN : R/W ;bitpos:[10] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FCS_CRC_EN  (BIT(10))
    #define SPI_FCS_CRC_EN_M  (BIT(10))
    #define SPI_FCS_CRC_EN_V  0x1
    #define SPI_FCS_CRC_EN_S  10



#define SPI_CTRL1_REG(i)          (REG_SPI_BASE(i) + 0xC)
    /* SPI_CS_HOLD_DELAY : R/W ;bitpos:[31:28] ;default: 4'h5 ; */
    /*description: */
    #define SPI_CS_HOLD_DELAY  0x0000000F
    #define SPI_CS_HOLD_DELAY_M  ((SPI_CS_HOLD_DELAY_V)<<(SPI_CS_HOLD_DELAY_S))
    #define SPI_CS_HOLD_DELAY_V  0xF
    #define SPI_CS_HOLD_DELAY_S  28
    /* SPI_CS_HOLD_DELAY_RES : R/W ;bitpos:[27:16] ;default: 12'hfff ; */
    /*description: */
    #define SPI_CS_HOLD_DELAY_RES  0x00000FFF
    #define SPI_CS_HOLD_DELAY_RES_M  ((SPI_CS_HOLD_DELAY_RES_V)<<(SPI_CS_HOLD_DELAY_RES_S))
    #define SPI_CS_HOLD_DELAY_RES_V  0xFFF
    #define SPI_CS_HOLD_DELAY_RES_S  16



#define SPI_RD_STATUS_REG(i)          (REG_SPI_BASE(i) + 0x10)
    /* SPI_STATUS_EXT : R/W ;bitpos:[31:24] ;default: 8'h00 ; */
    /*description: */
    #define SPI_STATUS_EXT  0x000000FF
    #define SPI_STATUS_EXT_M  ((SPI_STATUS_EXT_V)<<(SPI_STATUS_EXT_S))
    #define SPI_STATUS_EXT_V  0xFF
    #define SPI_STATUS_EXT_S  24
    /* SPI_WB_MODE : R/W ;bitpos:[23:16] ;default: 8'h00 ; */
    /*description: */
    #define SPI_WB_MODE  0x000000FF
    #define SPI_WB_MODE_M  ((SPI_WB_MODE_V)<<(SPI_WB_MODE_S))
    #define SPI_WB_MODE_V  0xFF
    #define SPI_WB_MODE_S  16
    /* SPI_STATUS : R/W ;bitpos:[15:0] ;default: 16'b0 ; */
    /*description: */
    #define SPI_STATUS  0x0000FFFF
    #define SPI_STATUS_M  ((SPI_STATUS_V)<<(SPI_STATUS_S))
    #define SPI_STATUS_V  0xFFFF
    #define SPI_STATUS_S  0

#define SPI_CTRL2_REG(i)          (REG_SPI_BASE(i) + 0x14)
    /* SPI_CS_DELAY_NUM : R/W ;bitpos:[31:28] ;default: 4'h0 ; */
    /*description: */
    #define SPI_CS_DELAY_NUM  0x0000000F
    #define SPI_CS_DELAY_NUM_M  ((SPI_CS_DELAY_NUM_V)<<(SPI_CS_DELAY_NUM_S))
    #define SPI_CS_DELAY_NUM_V  0xF
    #define SPI_CS_DELAY_NUM_S  28
    /* SPI_CS_DELAY_MODE : R/W ;bitpos:[27:26] ;default: 2'h0 ; */
    /*description: */
    #define SPI_CS_DELAY_MODE  0x00000003
    #define SPI_CS_DELAY_MODE_M  ((SPI_CS_DELAY_MODE_V)<<(SPI_CS_DELAY_MODE_S))
    #define SPI_CS_DELAY_MODE_V  0x3
    #define SPI_CS_DELAY_MODE_S  26
    /* SPI_MOSI_DELAY_NUM : R/W ;bitpos:[25:23] ;default: 3'h0 ; */
    /*description: */
    #define SPI_MOSI_DELAY_NUM  0x00000007
    #define SPI_MOSI_DELAY_NUM_M  ((SPI_MOSI_DELAY_NUM_V)<<(SPI_MOSI_DELAY_NUM_S))
    #define SPI_MOSI_DELAY_NUM_V  0x7
    #define SPI_MOSI_DELAY_NUM_S  23
    /* SPI_MOSI_DELAY_MODE : R/W ;bitpos:[22:21] ;default: 2'h0 ; */
    /*description: */
    #define SPI_MOSI_DELAY_MODE  0x00000003
    #define SPI_MOSI_DELAY_MODE_M  ((SPI_MOSI_DELAY_MODE_V)<<(SPI_MOSI_DELAY_MODE_S))
    #define SPI_MOSI_DELAY_MODE_V  0x3
    #define SPI_MOSI_DELAY_MODE_S  21
    /* SPI_MISO_DELAY_NUM : R/W ;bitpos:[20:18] ;default: 3'h0 ; */
    /*description: */
    #define SPI_MISO_DELAY_NUM  0x00000007
    #define SPI_MISO_DELAY_NUM_M  ((SPI_MISO_DELAY_NUM_V)<<(SPI_MISO_DELAY_NUM_S))
    #define SPI_MISO_DELAY_NUM_V  0x7
    #define SPI_MISO_DELAY_NUM_S  18
    /* SPI_MISO_DELAY_MODE : R/W ;bitpos:[17:16] ;default: 2'h0 ; */
    /*description: */
    #define SPI_MISO_DELAY_MODE  0x00000003
    #define SPI_MISO_DELAY_MODE_M  ((SPI_MISO_DELAY_MODE_V)<<(SPI_MISO_DELAY_MODE_S))
    #define SPI_MISO_DELAY_MODE_V  0x3
    #define SPI_MISO_DELAY_MODE_S  16
    /* SPI_CK_OUT_HIGH_MODE : R/W ;bitpos:[15:12] ;default: 4'h0 ; */
    /*description: */
    #define SPI_CK_OUT_HIGH_MODE  0x0000000F
    #define SPI_CK_OUT_HIGH_MODE_M  ((SPI_CK_OUT_HIGH_MODE_V)<<(SPI_CK_OUT_HIGH_MODE_S))
    #define SPI_CK_OUT_HIGH_MODE_V  0xF
    #define SPI_CK_OUT_HIGH_MODE_S  12
    /* SPI_CK_OUT_LOW_MODE : R/W ;bitpos:[11:8] ;default: 4'h0 ; */
    /*description: */
    #define SPI_CK_OUT_LOW_MODE  0x0000000F
    #define SPI_CK_OUT_LOW_MODE_M  ((SPI_CK_OUT_LOW_MODE_V)<<(SPI_CK_OUT_LOW_MODE_S))
    #define SPI_CK_OUT_LOW_MODE_V  0xF
    #define SPI_CK_OUT_LOW_MODE_S  8
    /* SPI_HOLD_TIME : R/W ;bitpos:[7:4] ;default: 4'h1 ; */
    /*description: */
    #define SPI_HOLD_TIME  0x0000000F
    #define SPI_HOLD_TIME_M  ((SPI_HOLD_TIME_V)<<(SPI_HOLD_TIME_S))
    #define SPI_HOLD_TIME_V  0xF
    #define SPI_HOLD_TIME_S  4
    /* SPI_SETUP_TIME : R/W ;bitpos:[3:0] ;default: 4'h1 ; */
    /*description: */
    #define SPI_SETUP_TIME  0x0000000F
    #define SPI_SETUP_TIME_M  ((SPI_SETUP_TIME_V)<<(SPI_SETUP_TIME_S))
    #define SPI_SETUP_TIME_V  0xF
    #define SPI_SETUP_TIME_S  0



#define SPI_CLOCK_REG(i)          (REG_SPI_BASE(i) + 0x18)
    /* SPI_CLK_EQU_SYSCLK : R/W ;bitpos:[31] ;default: 1'b1 ; */
    /*description: */
    #define SPI_CLK_EQU_SYSCLK  (BIT(31))
    #define SPI_CLK_EQU_SYSCLK_M  (BIT(31))
    #define SPI_CLK_EQU_SYSCLK_V  0x1
    #define SPI_CLK_EQU_SYSCLK_S  31
    /* SPI_CLKDIV_PRE : R/W ;bitpos:[30:18] ;default: 13'b0 ; */
    /*description: */
    #define SPI_CLKDIV_PRE  0x00001FFF
    #define SPI_CLKDIV_PRE_M  ((SPI_CLKDIV_PRE_V)<<(SPI_CLKDIV_PRE_S))
    #define SPI_CLKDIV_PRE_V  0x1FFF
    #define SPI_CLKDIV_PRE_S  18
    /* SPI_CLKCNT_N : R/W ;bitpos:[17:12] ;default: 6'h3 ; */
    /*description: */
    #define SPI_CLKCNT_N  0x0000003F
    #define SPI_CLKCNT_N_M  ((SPI_CLKCNT_N_V)<<(SPI_CLKCNT_N_S))
    #define SPI_CLKCNT_N_V  0x3F
    #define SPI_CLKCNT_N_S  12
    /* SPI_CLKCNT_H : R/W ;bitpos:[11:6] ;default: 6'h1 ; */
    /*description: */
    #define SPI_CLKCNT_H  0x0000003F
    #define SPI_CLKCNT_H_M  ((SPI_CLKCNT_H_V)<<(SPI_CLKCNT_H_S))
    #define SPI_CLKCNT_H_V  0x3F
    #define SPI_CLKCNT_H_S  6
    /* SPI_CLKCNT_L : R/W ;bitpos:[5:0] ;default: 6'h3 ; */
    /*description: */
    #define SPI_CLKCNT_L  0x0000003F
    #define SPI_CLKCNT_L_M  ((SPI_CLKCNT_L_V)<<(SPI_CLKCNT_L_S))
    #define SPI_CLKCNT_L_V  0x3F
    #define SPI_CLKCNT_L_S  0



#define SPI_USER_REG(i)          (REG_SPI_BASE(i) + 0x1C)
    /* SPI_USR_COMMAND : R/W ;bitpos:[31] ;default: 1'b1 ; */
    /*description: */
    #define SPI_USR_COMMAND  (BIT(31))
    #define SPI_USR_COMMAND_M  (BIT(31))
    #define SPI_USR_COMMAND_V  0x1
    #define SPI_USR_COMMAND_S  31
    /* SPI_USR_ADDR : R/W ;bitpos:[30] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_ADDR  (BIT(30))
    #define SPI_USR_ADDR_M  (BIT(30))
    #define SPI_USR_ADDR_V  0x1
    #define SPI_USR_ADDR_S  30
    /* SPI_USR_DUMMY : R/W ;bitpos:[29] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_DUMMY  (BIT(29))
    #define SPI_USR_DUMMY_M  (BIT(29))
    #define SPI_USR_DUMMY_V  0x1
    #define SPI_USR_DUMMY_S  29
    /* SPI_USR_MISO : R/W ;bitpos:[28] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_MISO  (BIT(28))
    #define SPI_USR_MISO_M  (BIT(28))
    #define SPI_USR_MISO_V  0x1
    #define SPI_USR_MISO_S  28
    /* SPI_USR_MOSI : R/W ;bitpos:[27] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_MOSI  (BIT(27))
    #define SPI_USR_MOSI_M  (BIT(27))
    #define SPI_USR_MOSI_V  0x1
    #define SPI_USR_MOSI_S  27
    /* SPI_USR_DUMMY_IDLE : R/W ;bitpos:[26] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_DUMMY_IDLE  (BIT(26))
    #define SPI_USR_DUMMY_IDLE_M  (BIT(26))
    #define SPI_USR_DUMMY_IDLE_V  0x1
    #define SPI_USR_DUMMY_IDLE_S  26
    /* SPI_USR_MOSI_HIGHPART : R/W ;bitpos:[25] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_MOSI_HIGHPART  (BIT(25))
    #define SPI_USR_MOSI_HIGHPART_M  (BIT(25))
    #define SPI_USR_MOSI_HIGHPART_V  0x1
    #define SPI_USR_MOSI_HIGHPART_S  25
    /* SPI_USR_MISO_HIGHPART : R/W ;bitpos:[24] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_MISO_HIGHPART  (BIT(24))
    #define SPI_USR_MISO_HIGHPART_M  (BIT(24))
    #define SPI_USR_MISO_HIGHPART_V  0x1
    #define SPI_USR_MISO_HIGHPART_S  24
    /* SPI_USR_PREP_HOLD : R/W ;bitpos:[23] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_PREP_HOLD  (BIT(23))
    #define SPI_USR_PREP_HOLD_M  (BIT(23))
    #define SPI_USR_PREP_HOLD_V  0x1
    #define SPI_USR_PREP_HOLD_S  23
    /* SPI_USR_CMD_HOLD : R/W ;bitpos:[22] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_CMD_HOLD  (BIT(22))
    #define SPI_USR_CMD_HOLD_M  (BIT(22))
    #define SPI_USR_CMD_HOLD_V  0x1
    #define SPI_USR_CMD_HOLD_S  22
    /* SPI_USR_ADDR_HOLD : R/W ;bitpos:[21] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_ADDR_HOLD  (BIT(21))
    #define SPI_USR_ADDR_HOLD_M  (BIT(21))
    #define SPI_USR_ADDR_HOLD_V  0x1
    #define SPI_USR_ADDR_HOLD_S  21
    /* SPI_USR_DUMMY_HOLD : R/W ;bitpos:[20] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_DUMMY_HOLD  (BIT(20))
    #define SPI_USR_DUMMY_HOLD_M  (BIT(20))
    #define SPI_USR_DUMMY_HOLD_V  0x1
    #define SPI_USR_DUMMY_HOLD_S  20
    /* SPI_USR_DIN_HOLD : R/W ;bitpos:[19] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_DIN_HOLD  (BIT(19))
    #define SPI_USR_DIN_HOLD_M  (BIT(19))
    #define SPI_USR_DIN_HOLD_V  0x1
    #define SPI_USR_DIN_HOLD_S  19
    /* SPI_USR_DOUT_HOLD : R/W ;bitpos:[18] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_DOUT_HOLD  (BIT(18))
    #define SPI_USR_DOUT_HOLD_M  (BIT(18))
    #define SPI_USR_DOUT_HOLD_V  0x1
    #define SPI_USR_DOUT_HOLD_S  18
    /* SPI_USR_HOLD_POL : R/W ;bitpos:[17] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_HOLD_POL  (BIT(17))
    #define SPI_USR_HOLD_POL_M  (BIT(17))
    #define SPI_USR_HOLD_POL_V  0x1
    #define SPI_USR_HOLD_POL_S  17
    /* SPI_SIO : R/W ;bitpos:[16] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SIO  (BIT(16))
    #define SPI_SIO_M  (BIT(16))
    #define SPI_SIO_V  0x1
    #define SPI_SIO_S  16
    /* SPI_FWRITE_QIO : R/W ;bitpos:[15] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FWRITE_QIO  (BIT(15))
    #define SPI_FWRITE_QIO_M  (BIT(15))
    #define SPI_FWRITE_QIO_V  0x1
    #define SPI_FWRITE_QIO_S  15
    /* SPI_FWRITE_DIO : R/W ;bitpos:[14] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FWRITE_DIO  (BIT(14))
    #define SPI_FWRITE_DIO_M  (BIT(14))
    #define SPI_FWRITE_DIO_V  0x1
    #define SPI_FWRITE_DIO_S  14
    /* SPI_FWRITE_QUAD : R/W ;bitpos:[13] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FWRITE_QUAD  (BIT(13))
    #define SPI_FWRITE_QUAD_M  (BIT(13))
    #define SPI_FWRITE_QUAD_V  0x1
    #define SPI_FWRITE_QUAD_S  13
    /* SPI_FWRITE_DUAL : R/W ;bitpos:[12] ;default: 1'b0 ; */
    /*description: */
    #define SPI_FWRITE_DUAL  (BIT(12))
    #define SPI_FWRITE_DUAL_M  (BIT(12))
    #define SPI_FWRITE_DUAL_V  0x1
    #define SPI_FWRITE_DUAL_S  12
    /* SPI_WR_BYTE_ORDER : R/W ;bitpos:[11] ;default: 1'b0 ; */
    /*description: */
    #define SPI_WR_BYTE_ORDER  (BIT(11))
    #define SPI_WR_BYTE_ORDER_M  (BIT(11))
    #define SPI_WR_BYTE_ORDER_V  0x1
    #define SPI_WR_BYTE_ORDER_S  11
    /* SPI_RD_BYTE_ORDER : R/W ;bitpos:[10] ;default: 1'b0 ; */
    /*description: */
    #define SPI_RD_BYTE_ORDER  (BIT(10))
    #define SPI_RD_BYTE_ORDER_M  (BIT(10))
    #define SPI_RD_BYTE_ORDER_V  0x1
    #define SPI_RD_BYTE_ORDER_S  10
    /* SPI_CK_OUT_EDGE : R/W ;bitpos:[7] ;default: 1'b0 ; */
    /*description: */
    #define SPI_CK_OUT_EDGE  (BIT(7))
    #define SPI_CK_OUT_EDGE_M  (BIT(7))
    #define SPI_CK_OUT_EDGE_V  0x1
    #define SPI_CK_OUT_EDGE_S  7
    /* SPI_CK_I_EDGE : R/W ;bitpos:[6] ;default: 1'b1 ; */
    /*description: */
    #define SPI_CK_I_EDGE  (BIT(6))
    #define SPI_CK_I_EDGE_M  (BIT(6))
    #define SPI_CK_I_EDGE_V  0x1
    #define SPI_CK_I_EDGE_S  6
    /* SPI_CS_SETUP : R/W ;bitpos:[5] ;default: 1'b0 ; */
    /*description: */
    #define SPI_CS_SETUP  (BIT(5))
    #define SPI_CS_SETUP_M  (BIT(5))
    #define SPI_CS_SETUP_V  0x1
    #define SPI_CS_SETUP_S  5
    /* SPI_CS_HOLD : R/W ;bitpos:[4] ;default: 1'b0 ; */
    /*description: */
    #define SPI_CS_HOLD  (BIT(4))
    #define SPI_CS_HOLD_M  (BIT(4))
    #define SPI_CS_HOLD_V  0x1
    #define SPI_CS_HOLD_S  4
    /* SPI_DOUTDIN : R/W ;bitpos:[0] ;default: 1'b0 ; */
    /*description: */
    #define SPI_DOUTDIN  (BIT(0))
    #define SPI_DOUTDIN_M  (BIT(0))
    #define SPI_DOUTDIN_V  0x1
    #define SPI_DOUTDIN_S  0



#define SPI_USER1_REG(i)          (REG_SPI_BASE(i) + 0x20)
    /* SPI_USR_ADDR_BITLEN : RO ;bitpos:[31:26] ;default: 6'd23 ; */
    /*description: */
    #define SPI_USR_ADDR_BITLEN  0x0000003F
    #define SPI_USR_ADDR_BITLEN_M  ((SPI_USR_ADDR_BITLEN_V)<<(SPI_USR_ADDR_BITLEN_S))
    #define SPI_USR_ADDR_BITLEN_V  0x3F
    #define SPI_USR_ADDR_BITLEN_S  26
    /* SPI_USR_DUMMY_CYCLELEN : R/W ;bitpos:[7:0] ;default: 8'd7 ; */
    /*description: */
    #define SPI_USR_DUMMY_CYCLELEN  0x000000FF
    #define SPI_USR_DUMMY_CYCLELEN_M  ((SPI_USR_DUMMY_CYCLELEN_V)<<(SPI_USR_DUMMY_CYCLELEN_S))
    #define SPI_USR_DUMMY_CYCLELEN_V  0xFF
    #define SPI_USR_DUMMY_CYCLELEN_S  0



#define SPI_USER2_REG(i)          (REG_SPI_BASE(i) + 0x24)
    /* SPI_USR_COMMAND_BITLEN : R/W ;bitpos:[31:28] ;default: 4'd7 ; */
    /*description: */
    #define SPI_USR_COMMAND_BITLEN  0x0000000F
    #define SPI_USR_COMMAND_BITLEN_M  ((SPI_USR_COMMAND_BITLEN_V)<<(SPI_USR_COMMAND_BITLEN_S))
    #define SPI_USR_COMMAND_BITLEN_V  0xF
    #define SPI_USR_COMMAND_BITLEN_S  28
    /* SPI_USR_COMMAND_VALUE : R/W ;bitpos:[15:0] ;default: 16'b0 ; */
    /*description: */
    #define SPI_USR_COMMAND_VALUE  0x0000FFFF
    #define SPI_USR_COMMAND_VALUE_M  ((SPI_USR_COMMAND_VALUE_V)<<(SPI_USR_COMMAND_VALUE_S))
    #define SPI_USR_COMMAND_VALUE_V  0xFFFF
    #define SPI_USR_COMMAND_VALUE_S  0



#define SPI_MOSI_DLEN_REG(i)          (REG_SPI_BASE(i) + 0x28)
    /* SPI_USR_MOSI_DBITLEN : R/W ;bitpos:[23:0] ;default: 24'h0 ; */
    /*description: */
    #define SPI_USR_MOSI_DBITLEN  0x00FFFFFF
    #define SPI_USR_MOSI_DBITLEN_M  ((SPI_USR_MOSI_DBITLEN_V)<<(SPI_USR_MOSI_DBITLEN_S))
    #define SPI_USR_MOSI_DBITLEN_V  0xFFFFFF
    #define SPI_USR_MOSI_DBITLEN_S  0



#define SPI_MISO_DLEN_REG(i)          (REG_SPI_BASE(i) + 0x2C)
    /* SPI_USR_MISO_DBITLEN : R/W ;bitpos:[23:0] ;default: 24'h0 ; */
    /*description: */
    #define SPI_USR_MISO_DBITLEN  0x00FFFFFF
    #define SPI_USR_MISO_DBITLEN_M  ((SPI_USR_MISO_DBITLEN_V)<<(SPI_USR_MISO_DBITLEN_S))
    #define SPI_USR_MISO_DBITLEN_V  0xFFFFFF
    #define SPI_USR_MISO_DBITLEN_S  0



#define SPI_SLV_WR_STATUS_REG(i)          (REG_SPI_BASE(i) + 0x30)
    /* SPI_SLV_WR_ST : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_SLV_WR_ST  0xFFFFFFFF
    #define SPI_SLV_WR_ST_M  ((SPI_SLV_WR_ST_V)<<(SPI_SLV_WR_ST_S))
    #define SPI_SLV_WR_ST_V  0xFFFFFFFF
    #define SPI_SLV_WR_ST_S  0



#define SPI_PIN_REG(i)          (REG_SPI_BASE(i) + 0x34)
    /* SPI_CS_KEEP_ACTIVE : R/W ;bitpos:[30] ;default: 1'b0 ; */
    /*description: */
    #define SPI_CS_KEEP_ACTIVE  (BIT(30))
    #define SPI_CS_KEEP_ACTIVE_M  (BIT(30))
    #define SPI_CS_KEEP_ACTIVE_V  0x1
    #define SPI_CS_KEEP_ACTIVE_S  30
    /* SPI_CK_IDLE_EDGE : R/W ;bitpos:[29] ;default: 1'b0 ; */
    /*description: */
    #define SPI_CK_IDLE_EDGE  (BIT(29))
    #define SPI_CK_IDLE_EDGE_M  (BIT(29))
    #define SPI_CK_IDLE_EDGE_V  0x1
    #define SPI_CK_IDLE_EDGE_S  29
    /* SPI_CSCK_SEL : R/W ;bitpos:[19] ;default: 1'b0 ; */
    /*description: */
    #define SPI_CSCK_SEL  (BIT(19))
    #define SPI_CSCK_SEL_M  (BIT(19))
    #define SPI_CSCK_SEL_V  0x1
    #define SPI_CSCK_SEL_S  19
    /* SPI_MASTER_CK_SEL : R/W ;bitpos:[15:11] ;default: 5'b0 ; */
    /*description: */
    #define SPI_MASTER_CK_SEL  0x0000001F
    #define SPI_MASTER_CK_SEL_M  ((SPI_MASTER_CK_SEL_V)<<(SPI_MASTER_CK_SEL_S))
    #define SPI_MASTER_CK_SEL_V  0x1F
    #define SPI_MASTER_CK_SEL_S  11
    /* SPI_MASTER_CS_POL : R/W ;bitpos:[10:6] ;default: 5'b0 ; */
    /*description: */
    #define SPI_MASTER_CS_POL  0x0000001F
    #define SPI_MASTER_CS_POL_M  ((SPI_MASTER_CS_POL_V)<<(SPI_MASTER_CS_POL_S))
    #define SPI_MASTER_CS_POL_V  0x1F
    #define SPI_MASTER_CS_POL_S  6
    /* SPI_CK_DIS : R/W ;bitpos:[5] ;default: 1'b0 ; */
    /*description: */
    #define SPI_CK_DIS  (BIT(5))
    #define SPI_CK_DIS_M  (BIT(5))
    #define SPI_CK_DIS_V  0x1
    #define SPI_CK_DIS_S  5
    /* SPI_CS2_DIS : R/W ;bitpos:[2] ;default: 1'b1 ; */
    /*description: */
    #define SPI_CS2_DIS  (BIT(2))
    #define SPI_CS2_DIS_M  (BIT(2))
    #define SPI_CS2_DIS_V  0x1
    #define SPI_CS2_DIS_S  2
    /* SPI_CS1_DIS : R/W ;bitpos:[1] ;default: 1'b1 ; */
    /*description: */
    #define SPI_CS1_DIS  (BIT(1))
    #define SPI_CS1_DIS_M  (BIT(1))
    #define SPI_CS1_DIS_V  0x1
    #define SPI_CS1_DIS_S  1
    /* SPI_CS0_DIS : R/W ;bitpos:[0] ;default: 1'b0 ; */
    /*description: */
    #define SPI_CS0_DIS  (BIT(0))
    #define SPI_CS0_DIS_M  (BIT(0))
    #define SPI_CS0_DIS_V  0x1
    #define SPI_CS0_DIS_S  0



#define SPI_SLAVE_REG(i)          (REG_SPI_BASE(i) + 0x38)
    /* SPI_SYNC_RESET : R/W ;bitpos:[31] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SYNC_RESET  (BIT(31))
    #define SPI_SYNC_RESET_M  (BIT(31))
    #define SPI_SYNC_RESET_V  0x1
    #define SPI_SYNC_RESET_S  31
    /* SPI_SLAVE_MODE : R/W ;bitpos:[30] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLAVE_MODE  (BIT(30))
    #define SPI_SLAVE_MODE_M  (BIT(30))
    #define SPI_SLAVE_MODE_V  0x1
    #define SPI_SLAVE_MODE_S  30
    /* SPI_SLV_WR_RD_BUF_EN : R/W ;bitpos:[29] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLV_WR_RD_BUF_EN  (BIT(29))
    #define SPI_SLV_WR_RD_BUF_EN_M  (BIT(29))
    #define SPI_SLV_WR_RD_BUF_EN_V  0x1
    #define SPI_SLV_WR_RD_BUF_EN_S  29
    /* SPI_SLV_WR_RD_STA_EN : R/W ;bitpos:[28] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLV_WR_RD_STA_EN  (BIT(28))
    #define SPI_SLV_WR_RD_STA_EN_M  (BIT(28))
    #define SPI_SLV_WR_RD_STA_EN_V  0x1
    #define SPI_SLV_WR_RD_STA_EN_S  28
    /* SPI_SLV_CMD_DEFINE : R/W ;bitpos:[27] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLV_CMD_DEFINE  (BIT(27))
    #define SPI_SLV_CMD_DEFINE_M  (BIT(27))
    #define SPI_SLV_CMD_DEFINE_V  0x1
    #define SPI_SLV_CMD_DEFINE_S  27
    /* SPI_TRANS_CNT : RO ;bitpos:[26:23] ;default: 4'b0 ; */
    /*description: */
    #define SPI_TRANS_CNT  0x0000000F
    #define SPI_TRANS_CNT_M  ((SPI_TRANS_CNT_V)<<(SPI_TRANS_CNT_S))
    #define SPI_TRANS_CNT_V  0xF
    #define SPI_TRANS_CNT_S  23
    /* SPI_SLV_LAST_STATE : RO ;bitpos:[22:20] ;default: 3'b0 ; */
    /*description: */
    #define SPI_SLV_LAST_STATE  0x00000007
    #define SPI_SLV_LAST_STATE_M  ((SPI_SLV_LAST_STATE_V)<<(SPI_SLV_LAST_STATE_S))
    #define SPI_SLV_LAST_STATE_V  0x7
    #define SPI_SLV_LAST_STATE_S  20
    /* SPI_SLV_LAST_COMMAND : RO ;bitpos:[19:17] ;default: 3'b0 ; */
    /*description: */
    #define SPI_SLV_LAST_COMMAND  0x00000007
    #define SPI_SLV_LAST_COMMAND_M  ((SPI_SLV_LAST_COMMAND_V)<<(SPI_SLV_LAST_COMMAND_S))
    #define SPI_SLV_LAST_COMMAND_V  0x7
    #define SPI_SLV_LAST_COMMAND_S  17
    /* SPI_CS_I_MODE : R/W ;bitpos:[11:10] ;default: 2'b0 ; */
    /*description: */
    #define SPI_CS_I_MODE  0x00000003
    #define SPI_CS_I_MODE_M  ((SPI_CS_I_MODE_V)<<(SPI_CS_I_MODE_S))
    #define SPI_CS_I_MODE_V  0x3
    #define SPI_CS_I_MODE_S  10
    /* SPI_INT_EN : R/W ;bitpos:[9:5] ;default: 5'b1_0000 ; */
    /*description: */
    #define SPI_INT_EN  0x0000001F
    #define SPI_INT_EN_M  ((SPI_INT_EN_V)<<(SPI_INT_EN_S))
    #define SPI_INT_EN_V  0x1F
    #define SPI_INT_EN_S  5
    /* SPI_TRANS_DONE : R/W ;bitpos:[4] ;default: 1'b0 ; */
    /*description: */
    #define SPI_TRANS_DONE  (BIT(4))
    #define SPI_TRANS_DONE_M  (BIT(4))
    #define SPI_TRANS_DONE_V  0x1
    #define SPI_TRANS_DONE_S  4
    /* SPI_SLV_WR_STA_DONE : R/W ;bitpos:[3] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLV_WR_STA_DONE  (BIT(3))
    #define SPI_SLV_WR_STA_DONE_M  (BIT(3))
    #define SPI_SLV_WR_STA_DONE_V  0x1
    #define SPI_SLV_WR_STA_DONE_S  3
    /* SPI_SLV_RD_STA_DONE : R/W ;bitpos:[2] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLV_RD_STA_DONE  (BIT(2))
    #define SPI_SLV_RD_STA_DONE_M  (BIT(2))
    #define SPI_SLV_RD_STA_DONE_V  0x1
    #define SPI_SLV_RD_STA_DONE_S  2
    /* SPI_SLV_WR_BUF_DONE : R/W ;bitpos:[1] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLV_WR_BUF_DONE  (BIT(1))
    #define SPI_SLV_WR_BUF_DONE_M  (BIT(1))
    #define SPI_SLV_WR_BUF_DONE_V  0x1
    #define SPI_SLV_WR_BUF_DONE_S  1
    /* SPI_SLV_RD_BUF_DONE : R/W ;bitpos:[0] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLV_RD_BUF_DONE  (BIT(0))
    #define SPI_SLV_RD_BUF_DONE_M  (BIT(0))
    #define SPI_SLV_RD_BUF_DONE_V  0x1
    #define SPI_SLV_RD_BUF_DONE_S  0



#define SPI_SLAVE1_REG(i)          (REG_SPI_BASE(i) + 0x3C)
    /* SPI_SLV_STATUS_BITLEN : R/W ;bitpos:[31:27] ;default: 5'b0 ; */
    /*description: */
    #define SPI_SLV_STATUS_BITLEN  0x0000001F
    #define SPI_SLV_STATUS_BITLEN_M  ((SPI_SLV_STATUS_BITLEN_V)<<(SPI_SLV_STATUS_BITLEN_S))
    #define SPI_SLV_STATUS_BITLEN_V  0x1F
    #define SPI_SLV_STATUS_BITLEN_S  27
    /* SPI_SLV_STATUS_FAST_EN : R/W ;bitpos:[26] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLV_STATUS_FAST_EN  (BIT(26))
    #define SPI_SLV_STATUS_FAST_EN_M  (BIT(26))
    #define SPI_SLV_STATUS_FAST_EN_V  0x1
    #define SPI_SLV_STATUS_FAST_EN_S  26
    /* SPI_SLV_STATUS_READBACK : R/W ;bitpos:[25] ;default: 1'b1 ; */
    /*description: */
    #define SPI_SLV_STATUS_READBACK  (BIT(25))
    #define SPI_SLV_STATUS_READBACK_M  (BIT(25))
    #define SPI_SLV_STATUS_READBACK_V  0x1
    #define SPI_SLV_STATUS_READBACK_S  25
    /* SPI_SLV_RD_ADDR_BITLEN : R/W ;bitpos:[15:10] ;default: 6'h0 ; */
    /*description: */
    #define SPI_SLV_RD_ADDR_BITLEN  0x0000003F
    #define SPI_SLV_RD_ADDR_BITLEN_M  ((SPI_SLV_RD_ADDR_BITLEN_V)<<(SPI_SLV_RD_ADDR_BITLEN_S))
    #define SPI_SLV_RD_ADDR_BITLEN_V  0x3F
    #define SPI_SLV_RD_ADDR_BITLEN_S  10
    /* SPI_SLV_WR_ADDR_BITLEN : R/W ;bitpos:[9:4] ;default: 6'h0 ; */
    /*description: */
    #define SPI_SLV_WR_ADDR_BITLEN  0x0000003F
    #define SPI_SLV_WR_ADDR_BITLEN_M  ((SPI_SLV_WR_ADDR_BITLEN_V)<<(SPI_SLV_WR_ADDR_BITLEN_S))
    #define SPI_SLV_WR_ADDR_BITLEN_V  0x3F
    #define SPI_SLV_WR_ADDR_BITLEN_S  4
    /* SPI_SLV_WRSTA_DUMMY_EN : R/W ;bitpos:[3] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLV_WRSTA_DUMMY_EN  (BIT(3))
    #define SPI_SLV_WRSTA_DUMMY_EN_M  (BIT(3))
    #define SPI_SLV_WRSTA_DUMMY_EN_V  0x1
    #define SPI_SLV_WRSTA_DUMMY_EN_S  3
    /* SPI_SLV_RDSTA_DUMMY_EN : R/W ;bitpos:[2] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLV_RDSTA_DUMMY_EN  (BIT(2))
    #define SPI_SLV_RDSTA_DUMMY_EN_M  (BIT(2))
    #define SPI_SLV_RDSTA_DUMMY_EN_V  0x1
    #define SPI_SLV_RDSTA_DUMMY_EN_S  2
    /* SPI_SLV_WRBUF_DUMMY_EN : R/W ;bitpos:[1] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLV_WRBUF_DUMMY_EN  (BIT(1))
    #define SPI_SLV_WRBUF_DUMMY_EN_M  (BIT(1))
    #define SPI_SLV_WRBUF_DUMMY_EN_V  0x1
    #define SPI_SLV_WRBUF_DUMMY_EN_S  1
    /* SPI_SLV_RDBUF_DUMMY_EN : R/W ;bitpos:[0] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SLV_RDBUF_DUMMY_EN  (BIT(0))
    #define SPI_SLV_RDBUF_DUMMY_EN_M  (BIT(0))
    #define SPI_SLV_RDBUF_DUMMY_EN_V  0x1
    #define SPI_SLV_RDBUF_DUMMY_EN_S  0



#define SPI_SLAVE2_REG(i)          (REG_SPI_BASE(i) + 0x40)
    /* SPI_SLV_WRBUF_DUMMY_CYCLELEN : R/W ;bitpos:[31:24] ;default: 8'b0 ; */
    /*description: */
    #define SPI_SLV_WRBUF_DUMMY_CYCLELEN  0x000000FF
    #define SPI_SLV_WRBUF_DUMMY_CYCLELEN_M  ((SPI_SLV_WRBUF_DUMMY_CYCLELEN_V)<<(SPI_SLV_WRBUF_DUMMY_CYCLELEN_S))
    #define SPI_SLV_WRBUF_DUMMY_CYCLELEN_V  0xFF
    #define SPI_SLV_WRBUF_DUMMY_CYCLELEN_S  24
    /* SPI_SLV_RDBUF_DUMMY_CYCLELEN : R/W ;bitpos:[23:16] ;default: 8'h0 ; */
    /*description: */
    #define SPI_SLV_RDBUF_DUMMY_CYCLELEN  0x000000FF
    #define SPI_SLV_RDBUF_DUMMY_CYCLELEN_M  ((SPI_SLV_RDBUF_DUMMY_CYCLELEN_V)<<(SPI_SLV_RDBUF_DUMMY_CYCLELEN_S))
    #define SPI_SLV_RDBUF_DUMMY_CYCLELEN_V  0xFF
    #define SPI_SLV_RDBUF_DUMMY_CYCLELEN_S  16
    /* SPI_SLV_WRSTA_DUMMY_CYCLELEN : R/W ;bitpos:[15:8] ;default: 8'h0 ; */
    /*description: */
    #define SPI_SLV_WRSTA_DUMMY_CYCLELEN  0x000000FF
    #define SPI_SLV_WRSTA_DUMMY_CYCLELEN_M  ((SPI_SLV_WRSTA_DUMMY_CYCLELEN_V)<<(SPI_SLV_WRSTA_DUMMY_CYCLELEN_S))
    #define SPI_SLV_WRSTA_DUMMY_CYCLELEN_V  0xFF
    #define SPI_SLV_WRSTA_DUMMY_CYCLELEN_S  8
    /* SPI_SLV_RDSTA_DUMMY_CYCLELEN : R/W ;bitpos:[7:0] ;default: 8'h0 ; */
    /*description: */
    #define SPI_SLV_RDSTA_DUMMY_CYCLELEN  0x000000FF
    #define SPI_SLV_RDSTA_DUMMY_CYCLELEN_M  ((SPI_SLV_RDSTA_DUMMY_CYCLELEN_V)<<(SPI_SLV_RDSTA_DUMMY_CYCLELEN_S))
    #define SPI_SLV_RDSTA_DUMMY_CYCLELEN_V  0xFF
    #define SPI_SLV_RDSTA_DUMMY_CYCLELEN_S  0



#define SPI_SLAVE3_REG(i)          (REG_SPI_BASE(i) + 0x44)
    /* SPI_SLV_WRSTA_CMD_VALUE : R/W ;bitpos:[31:24] ;default: 8'b0 ; */
    /*description: */
    #define SPI_SLV_WRSTA_CMD_VALUE  0x000000FF
    #define SPI_SLV_WRSTA_CMD_VALUE_M  ((SPI_SLV_WRSTA_CMD_VALUE_V)<<(SPI_SLV_WRSTA_CMD_VALUE_S))
    #define SPI_SLV_WRSTA_CMD_VALUE_V  0xFF
    #define SPI_SLV_WRSTA_CMD_VALUE_S  24
    /* SPI_SLV_RDSTA_CMD_VALUE : R/W ;bitpos:[23:16] ;default: 8'b0 ; */
    /*description: */
    #define SPI_SLV_RDSTA_CMD_VALUE  0x000000FF
    #define SPI_SLV_RDSTA_CMD_VALUE_M  ((SPI_SLV_RDSTA_CMD_VALUE_V)<<(SPI_SLV_RDSTA_CMD_VALUE_S))
    #define SPI_SLV_RDSTA_CMD_VALUE_V  0xFF
    #define SPI_SLV_RDSTA_CMD_VALUE_S  16
    /* SPI_SLV_WRBUF_CMD_VALUE : R/W ;bitpos:[15:8] ;default: 8'b0 ; */
    /*description: */
    #define SPI_SLV_WRBUF_CMD_VALUE  0x000000FF
    #define SPI_SLV_WRBUF_CMD_VALUE_M  ((SPI_SLV_WRBUF_CMD_VALUE_V)<<(SPI_SLV_WRBUF_CMD_VALUE_S))
    #define SPI_SLV_WRBUF_CMD_VALUE_V  0xFF
    #define SPI_SLV_WRBUF_CMD_VALUE_S  8
    /* SPI_SLV_RDBUF_CMD_VALUE : R/W ;bitpos:[7:0] ;default: 8'b0 ; */
    /*description: */
    #define SPI_SLV_RDBUF_CMD_VALUE  0x000000FF
    #define SPI_SLV_RDBUF_CMD_VALUE_M  ((SPI_SLV_RDBUF_CMD_VALUE_V)<<(SPI_SLV_RDBUF_CMD_VALUE_S))
    #define SPI_SLV_RDBUF_CMD_VALUE_V  0xFF
    #define SPI_SLV_RDBUF_CMD_VALUE_S  0



#define SPI_SLV_WRBUF_DLEN_REG(i)          (REG_SPI_BASE(i) + 0x48)
    /* SPI_SLV_WRBUF_DBITLEN : R/W ;bitpos:[23:0] ;default: 24'h0 ; */
    /*description: */
    #define SPI_SLV_WRBUF_DBITLEN  0x00FFFFFF
    #define SPI_SLV_WRBUF_DBITLEN_M  ((SPI_SLV_WRBUF_DBITLEN_V)<<(SPI_SLV_WRBUF_DBITLEN_S))
    #define SPI_SLV_WRBUF_DBITLEN_V  0xFFFFFF
    #define SPI_SLV_WRBUF_DBITLEN_S  0



#define SPI_SLV_RDBUF_DLEN_REG(i)          (REG_SPI_BASE(i) + 0x4C)
    /* SPI_SLV_RDBUF_DBITLEN : R/W ;bitpos:[23:0] ;default: 24'h0 ; */
    /*description: */
    #define SPI_SLV_RDBUF_DBITLEN  0x00FFFFFF
    #define SPI_SLV_RDBUF_DBITLEN_M  ((SPI_SLV_RDBUF_DBITLEN_V)<<(SPI_SLV_RDBUF_DBITLEN_S))
    #define SPI_SLV_RDBUF_DBITLEN_V  0xFFFFFF
    #define SPI_SLV_RDBUF_DBITLEN_S  0



#define SPI_CACHE_FCTRL_REG(i)          (REG_SPI_BASE(i) + 0x50)
    /* SPI_CACHE_FLASH_PES_EN : R/W ;bitpos:[3] ;default: 1'b0 ; */
    /*description: */
    #define SPI_CACHE_FLASH_PES_EN  (BIT(3))
    #define SPI_CACHE_FLASH_PES_EN_M  (BIT(3))
    #define SPI_CACHE_FLASH_PES_EN_V  0x1
    #define SPI_CACHE_FLASH_PES_EN_S  3
    /* SPI_CACHE_FLASH_USR_CMD : R/W ;bitpos:[2] ;default: 1'b0 ; */
    /*description: */
    #define SPI_CACHE_FLASH_USR_CMD  (BIT(2))
    #define SPI_CACHE_FLASH_USR_CMD_M  (BIT(2))
    #define SPI_CACHE_FLASH_USR_CMD_V  0x1
    #define SPI_CACHE_FLASH_USR_CMD_S  2
    /* SPI_CACHE_USR_CMD_4BYTE : R/W ;bitpos:[1] ;default: 1'b0 ; */
    /*description: */
    #define SPI_CACHE_USR_CMD_4BYTE  (BIT(1))
    #define SPI_CACHE_USR_CMD_4BYTE_M  (BIT(1))
    #define SPI_CACHE_USR_CMD_4BYTE_V  0x1
    #define SPI_CACHE_USR_CMD_4BYTE_S  1
    /* SPI_CACHE_REQ_EN : R/W ;bitpos:[0] ;default: 1'b0 ; */
    /*description: */
    #define SPI_CACHE_REQ_EN  (BIT(0))
    #define SPI_CACHE_REQ_EN_M  (BIT(0))
    #define SPI_CACHE_REQ_EN_V  0x1
    #define SPI_CACHE_REQ_EN_S  0



#define SPI_CACHE_SCTRL_REG(i)          (REG_SPI_BASE(i) + 0x54)
    /* SPI_CACHE_SRAM_USR_WCMD : R/W ;bitpos:[28] ;default: 1'b1 ; */
    /*description: */
    #define SPI_CACHE_SRAM_USR_WCMD  (BIT(28))
    #define SPI_CACHE_SRAM_USR_WCMD_M  (BIT(28))
    #define SPI_CACHE_SRAM_USR_WCMD_V  0x1
    #define SPI_CACHE_SRAM_USR_WCMD_S  28
    /* SPI_SRAM_ADDR_BITLEN : R/W ;bitpos:[27:22] ;default: 6'd23 ; */
    /*description: */
    #define SPI_SRAM_ADDR_BITLEN  0x0000003F
    #define SPI_SRAM_ADDR_BITLEN_M  ((SPI_SRAM_ADDR_BITLEN_V)<<(SPI_SRAM_ADDR_BITLEN_S))
    #define SPI_SRAM_ADDR_BITLEN_V  0x3F
    #define SPI_SRAM_ADDR_BITLEN_S  22
    /* SPI_SRAM_DUMMY_CYCLELEN : R/W ;bitpos:[21:14] ;default: 8'b1 ; */
    /*description: */
    #define SPI_SRAM_DUMMY_CYCLELEN  0x000000FF
    #define SPI_SRAM_DUMMY_CYCLELEN_M  ((SPI_SRAM_DUMMY_CYCLELEN_V)<<(SPI_SRAM_DUMMY_CYCLELEN_S))
    #define SPI_SRAM_DUMMY_CYCLELEN_V  0xFF
    #define SPI_SRAM_DUMMY_CYCLELEN_S  14
    /* SPI_SRAM_BYTES_LEN : R/W ;bitpos:[13:6] ;default: 8'b32 ; */
    /*description: */
    #define SPI_SRAM_BYTES_LEN  0x000000FF
    #define SPI_SRAM_BYTES_LEN_M  ((SPI_SRAM_BYTES_LEN_V)<<(SPI_SRAM_BYTES_LEN_S))
    #define SPI_SRAM_BYTES_LEN_V  0xFF
    #define SPI_SRAM_BYTES_LEN_S  6
    /* SPI_CACHE_SRAM_USR_RCMD : R/W ;bitpos:[5] ;default: 1'b1 ; */
    /*description: */
    #define SPI_CACHE_SRAM_USR_RCMD  (BIT(5))
    #define SPI_CACHE_SRAM_USR_RCMD_M  (BIT(5))
    #define SPI_CACHE_SRAM_USR_RCMD_V  0x1
    #define SPI_CACHE_SRAM_USR_RCMD_S  5
    /* SPI_USR_RD_SRAM_DUMMY : R/W ;bitpos:[4] ;default: 1'b1 ; */
    /*description: */
    #define SPI_USR_RD_SRAM_DUMMY  (BIT(4))
    #define SPI_USR_RD_SRAM_DUMMY_M  (BIT(4))
    #define SPI_USR_RD_SRAM_DUMMY_V  0x1
    #define SPI_USR_RD_SRAM_DUMMY_S  4
    /* SPI_USR_WR_SRAM_DUMMY : R/W ;bitpos:[3] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_WR_SRAM_DUMMY  (BIT(3))
    #define SPI_USR_WR_SRAM_DUMMY_M  (BIT(3))
    #define SPI_USR_WR_SRAM_DUMMY_V  0x1
    #define SPI_USR_WR_SRAM_DUMMY_S  3
    /* SPI_USR_SRAM_QIO : R/W ;bitpos:[2] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_SRAM_QIO  (BIT(2))
    #define SPI_USR_SRAM_QIO_M  (BIT(2))
    #define SPI_USR_SRAM_QIO_V  0x1
    #define SPI_USR_SRAM_QIO_S  2
    /* SPI_USR_SRAM_DIO : R/W ;bitpos:[1] ;default: 1'b0 ; */
    /*description: */
    #define SPI_USR_SRAM_DIO  (BIT(1))
    #define SPI_USR_SRAM_DIO_M  (BIT(1))
    #define SPI_USR_SRAM_DIO_V  0x1
    #define SPI_USR_SRAM_DIO_S  1



#define SPI_SRAM_CMD_REG(i)          (REG_SPI_BASE(i) + 0x58)
    /* SPI_SRAM_RSTIO : R/W ;bitpos:[4] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SRAM_RSTIO  (BIT(4))
    #define SPI_SRAM_RSTIO_M  (BIT(4))
    #define SPI_SRAM_RSTIO_V  0x1
    #define SPI_SRAM_RSTIO_S  4
    /* SPI_SRAM_QIO : R/W ;bitpos:[1] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SRAM_QIO  (BIT(1))
    #define SPI_SRAM_QIO_M  (BIT(1))
    #define SPI_SRAM_QIO_V  0x1
    #define SPI_SRAM_QIO_S  1
    /* SPI_SRAM_DIO : R/W ;bitpos:[0] ;default: 1'b0 ; */
    /*description: */
    #define SPI_SRAM_DIO  (BIT(0))
    #define SPI_SRAM_DIO_M  (BIT(0))
    #define SPI_SRAM_DIO_V  0x1
    #define SPI_SRAM_DIO_S  0



#define SPI_SRAM_DRD_CMD_REG(i)          (REG_SPI_BASE(i) + 0x5C)
    /* SPI_CACHE_SRAM_USR_RD_CMD_BITLEN : R/W ;bitpos:[31:28] ;default: 4'h0 ; */
    /*description: */
    #define SPI_CACHE_SRAM_USR_RD_CMD_BITLEN  0x0000000F
    #define SPI_CACHE_SRAM_USR_RD_CMD_BITLEN_M  ((SPI_CACHE_SRAM_USR_RD_CMD_BITLEN_V)<<(SPI_CACHE_SRAM_USR_RD_CMD_BITLEN_S))
    #define SPI_CACHE_SRAM_USR_RD_CMD_BITLEN_V  0xF
    #define SPI_CACHE_SRAM_USR_RD_CMD_BITLEN_S  28
    /* SPI_CACHE_SRAM_USR_RD_CMD_VALUE : R/W ;bitpos:[15:0] ;default: 16'h0 ; */
    /*description: */
    #define SPI_CACHE_SRAM_USR_RD_CMD_VALUE  0x0000FFFF
    #define SPI_CACHE_SRAM_USR_RD_CMD_VALUE_M  ((SPI_CACHE_SRAM_USR_RD_CMD_VALUE_V)<<(SPI_CACHE_SRAM_USR_RD_CMD_VALUE_S))
    #define SPI_CACHE_SRAM_USR_RD_CMD_VALUE_V  0xFFFF
    #define SPI_CACHE_SRAM_USR_RD_CMD_VALUE_S  0



#define SPI_SRAM_DWR_CMD_REG(i)          (REG_SPI_BASE(i) + 0x60)
    /* SPI_CACHE_SRAM_USR_WR_CMD_BITLEN : R/W ;bitpos:[31:28] ;default: 4'h0 ; */
    /*description: */
    #define SPI_CACHE_SRAM_USR_WR_CMD_BITLEN  0x0000000F
    #define SPI_CACHE_SRAM_USR_WR_CMD_BITLEN_M  ((SPI_CACHE_SRAM_USR_WR_CMD_BITLEN_V)<<(SPI_CACHE_SRAM_USR_WR_CMD_BITLEN_S))
    #define SPI_CACHE_SRAM_USR_WR_CMD_BITLEN_V  0xF
    #define SPI_CACHE_SRAM_USR_WR_CMD_BITLEN_S  28
    /* SPI_CACHE_SRAM_USR_WR_CMD_VALUE : R/W ;bitpos:[15:0] ;default: 16'h0 ; */
    /*description: */
    #define SPI_CACHE_SRAM_USR_WR_CMD_VALUE  0x0000FFFF
    #define SPI_CACHE_SRAM_USR_WR_CMD_VALUE_M  ((SPI_CACHE_SRAM_USR_WR_CMD_VALUE_V)<<(SPI_CACHE_SRAM_USR_WR_CMD_VALUE_S))
    #define SPI_CACHE_SRAM_USR_WR_CMD_VALUE_V  0xFFFF
    #define SPI_CACHE_SRAM_USR_WR_CMD_VALUE_S  0



#define SPI_SLV_RD_BIT_REG(i)          (REG_SPI_BASE(i) + 0x64)
    /* SPI_SLV_RDATA_BIT : RW ;bitpos:[23:0] ;default: 24'b0 ; */
    /*description: */
    #define SPI_SLV_RDATA_BIT  0x00FFFFFF
    #define SPI_SLV_RDATA_BIT_M  ((SPI_SLV_RDATA_BIT_V)<<(SPI_SLV_RDATA_BIT_S))
    #define SPI_SLV_RDATA_BIT_V  0xFFFFFF
    #define SPI_SLV_RDATA_BIT_S  0



#define SPI_W0_REG(i)          (REG_SPI_BASE(i) + 0x80)
    /* SPI_BUF0 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF0  0xFFFFFFFF
    #define SPI_BUF0_M  ((SPI_BUF0_V)<<(SPI_BUF0_S))
    #define SPI_BUF0_V  0xFFFFFFFF
    #define SPI_BUF0_S  0



#define SPI_W1_REG(i)          (REG_SPI_BASE(i) + 0x84)
    /* SPI_BUF1 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF1  0xFFFFFFFF
    #define SPI_BUF1_M  ((SPI_BUF1_V)<<(SPI_BUF1_S))
    #define SPI_BUF1_V  0xFFFFFFFF
    #define SPI_BUF1_S  0



#define SPI_W2_REG(i)          (REG_SPI_BASE(i) + 0x88)
    /* SPI_BUF2 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF2  0xFFFFFFFF
    #define SPI_BUF2_M  ((SPI_BUF2_V)<<(SPI_BUF2_S))
    #define SPI_BUF2_V  0xFFFFFFFF
    #define SPI_BUF2_S  0



#define SPI_W3_REG(i)          (REG_SPI_BASE(i) + 0x8C)
    /* SPI_BUF3 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF3  0xFFFFFFFF
    #define SPI_BUF3_M  ((SPI_BUF3_V)<<(SPI_BUF3_S))
    #define SPI_BUF3_V  0xFFFFFFFF
    #define SPI_BUF3_S  0



#define SPI_W4_REG(i)          (REG_SPI_BASE(i) + 0x90)
    /* SPI_BUF4 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF4  0xFFFFFFFF
    #define SPI_BUF4_M  ((SPI_BUF4_V)<<(SPI_BUF4_S))
    #define SPI_BUF4_V  0xFFFFFFFF
    #define SPI_BUF4_S  0



#define SPI_W5_REG(i)          (REG_SPI_BASE(i) + 0x94)
    /* SPI_BUF5 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF5  0xFFFFFFFF
    #define SPI_BUF5_M  ((SPI_BUF5_V)<<(SPI_BUF5_S))
    #define SPI_BUF5_V  0xFFFFFFFF
    #define SPI_BUF5_S  0



#define SPI_W6_REG(i)          (REG_SPI_BASE(i) + 0x98)
    /* SPI_BUF6 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF6  0xFFFFFFFF
    #define SPI_BUF6_M  ((SPI_BUF6_V)<<(SPI_BUF6_S))
    #define SPI_BUF6_V  0xFFFFFFFF
    #define SPI_BUF6_S  0



#define SPI_W7_REG(i)          (REG_SPI_BASE(i) + 0x9C)
    /* SPI_BUF7 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF7  0xFFFFFFFF
    #define SPI_BUF7_M  ((SPI_BUF7_V)<<(SPI_BUF7_S))
    #define SPI_BUF7_V  0xFFFFFFFF
    #define SPI_BUF7_S  0



#define SPI_W8_REG(i)          (REG_SPI_BASE(i) + 0xA0)
    /* SPI_BUF8 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF8  0xFFFFFFFF
    #define SPI_BUF8_M  ((SPI_BUF8_V)<<(SPI_BUF8_S))
    #define SPI_BUF8_V  0xFFFFFFFF
    #define SPI_BUF8_S  0



#define SPI_W9_REG(i)          (REG_SPI_BASE(i) + 0xA4)
    /* SPI_BUF9 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF9  0xFFFFFFFF
    #define SPI_BUF9_M  ((SPI_BUF9_V)<<(SPI_BUF9_S))
    #define SPI_BUF9_V  0xFFFFFFFF
    #define SPI_BUF9_S  0



#define SPI_W10_REG(i)          (REG_SPI_BASE(i) + 0xA8)
    /* SPI_BUF10 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF10  0xFFFFFFFF
    #define SPI_BUF10_M  ((SPI_BUF10_V)<<(SPI_BUF10_S))
    #define SPI_BUF10_V  0xFFFFFFFF
    #define SPI_BUF10_S  0



#define SPI_W11_REG(i)          (REG_SPI_BASE(i) + 0xAC)
    /* SPI_BUF11 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF11  0xFFFFFFFF
    #define SPI_BUF11_M  ((SPI_BUF11_V)<<(SPI_BUF11_S))
    #define SPI_BUF11_V  0xFFFFFFFF
    #define SPI_BUF11_S  0



#define SPI_W12_REG(i)          (REG_SPI_BASE(i) + 0xB0)
    /* SPI_BUF12 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF12  0xFFFFFFFF
    #define SPI_BUF12_M  ((SPI_BUF12_V)<<(SPI_BUF12_S))
    #define SPI_BUF12_V  0xFFFFFFFF
    #define SPI_BUF12_S  0



#define SPI_W13_REG(i)          (REG_SPI_BASE(i) + 0xB4)
    /* SPI_BUF13 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF13  0xFFFFFFFF
    #define SPI_BUF13_M  ((SPI_BUF13_V)<<(SPI_BUF13_S))
    #define SPI_BUF13_V  0xFFFFFFFF
    #define SPI_BUF13_S  0



#define SPI_W14_REG(i)          (REG_SPI_BASE(i) + 0xB8)
    /* SPI_BUF14 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF14  0xFFFFFFFF
    #define SPI_BUF14_M  ((SPI_BUF14_V)<<(SPI_BUF14_S))
    #define SPI_BUF14_V  0xFFFFFFFF
    #define SPI_BUF14_S  0



#define SPI_W15_REG(i)          (REG_SPI_BASE(i) + 0xBC)
    /* SPI_BUF15 : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_BUF15  0xFFFFFFFF
    #define SPI_BUF15_M  ((SPI_BUF15_V)<<(SPI_BUF15_S))
    #define SPI_BUF15_V  0xFFFFFFFF
    #define SPI_BUF15_S  0



#define SPI_TX_CRC_REG(i)          (REG_SPI_BASE(i) + 0xC0)
    /* SPI_TX_CRC_DATA : R/W ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_TX_CRC_DATA  0xFFFFFFFF
    #define SPI_TX_CRC_DATA_M  ((SPI_TX_CRC_DATA_V)<<(SPI_TX_CRC_DATA_S))
    #define SPI_TX_CRC_DATA_V  0xFFFFFFFF
    #define SPI_TX_CRC_DATA_S  0



#define SPI_EXT0_REG(i)          (REG_SPI_BASE(i) + 0xF0)
    /* SPI_T_PP_ENA : R/W ;bitpos:[31] ;default: 1'b1 ; */
    /*description: */
    #define SPI_T_PP_ENA  (BIT(31))
    #define SPI_T_PP_ENA_M  (BIT(31))
    #define SPI_T_PP_ENA_V  0x1
    #define SPI_T_PP_ENA_S  31
    /* SPI_T_PP_SHIFT : R/W ;bitpos:[19:16] ;default: 4'd10 ; */
    /*description: */
    #define SPI_T_PP_SHIFT  0x0000000F
    #define SPI_T_PP_SHIFT_M  ((SPI_T_PP_SHIFT_V)<<(SPI_T_PP_SHIFT_S))
    #define SPI_T_PP_SHIFT_V  0xF
    #define SPI_T_PP_SHIFT_S  16
    /* SPI_T_PP_TIME : R/W ;bitpos:[11:0] ;default: 12'd80 ; */
    /*description: */
    #define SPI_T_PP_TIME  0x00000FFF
    #define SPI_T_PP_TIME_M  ((SPI_T_PP_TIME_V)<<(SPI_T_PP_TIME_S))
    #define SPI_T_PP_TIME_V  0xFFF
    #define SPI_T_PP_TIME_S  0



#define SPI_EXT1_REG(i)          (REG_SPI_BASE(i) + 0xF4)
    /* SPI_T_ERASE_ENA : R/W ;bitpos:[31] ;default: 1'b1 ; */
    /*description: */
    #define SPI_T_ERASE_ENA  (BIT(31))
    #define SPI_T_ERASE_ENA_M  (BIT(31))
    #define SPI_T_ERASE_ENA_V  0x1
    #define SPI_T_ERASE_ENA_S  31
    /* SPI_T_ERASE_SHIFT : R/W ;bitpos:[19:16] ;default: 4'd10 ; */
    /*description: */
    #define SPI_T_ERASE_SHIFT  0x0000000F
    #define SPI_T_ERASE_SHIFT_M  ((SPI_T_ERASE_SHIFT_V)<<(SPI_T_ERASE_SHIFT_S))
    #define SPI_T_ERASE_SHIFT_V  0xF
    #define SPI_T_ERASE_SHIFT_S  16
    /* SPI_T_ERASE_TIME : R/W ;bitpos:[11:0] ;default: 12'd80 ; */
    /*description: */
    #define SPI_T_ERASE_TIME  0x00000FFF
    #define SPI_T_ERASE_TIME_M  ((SPI_T_ERASE_TIME_V)<<(SPI_T_ERASE_TIME_S))
    #define SPI_T_ERASE_TIME_V  0xFFF
    #define SPI_T_ERASE_TIME_S  0



#define SPI_EXT2_REG(i)          (REG_SPI_BASE(i) + 0xF8)
    /* SPI_ST : RO ;bitpos:[2:0] ;default: 3'b0 ; */
    /*description: */
    #define SPI_ST  0x00000007
    #define SPI_ST_M  ((SPI_ST_V)<<(SPI_ST_S))
    #define SPI_ST_V  0x7
    #define SPI_ST_S  0



#define SPI_EXT3_REG(i)          (REG_SPI_BASE(i) + 0xFC)
    /* SPI_INT_HOLD_ENA : R/W ;bitpos:[1:0] ;default: 2'b0 ; */
    /*description: */
    #define SPI_INT_HOLD_ENA  0x00000003
    #define SPI_INT_HOLD_ENA_M  ((SPI_INT_HOLD_ENA_V)<<(SPI_INT_HOLD_ENA_S))
    #define SPI_INT_HOLD_ENA_V  0x3
    #define SPI_INT_HOLD_ENA_S  0



#define SPI_DMA_CONF_REG(i)          (REG_SPI_BASE(i) + 0x100)
    /* SPI_DMA_CONTINUE : R/W ;bitpos:[16] ;default: 1'b0 ; */
    /*description: */
    #define SPI_DMA_CONTINUE  (BIT(16))
    #define SPI_DMA_CONTINUE_M  (BIT(16))
    #define SPI_DMA_CONTINUE_V  0x1
    #define SPI_DMA_CONTINUE_S  16
    /* SPI_DMA_TX_STOP : R/W ;bitpos:[15] ;default: 1'b0 ; */
    /*description: */
    #define SPI_DMA_TX_STOP  (BIT(15))
    #define SPI_DMA_TX_STOP_M  (BIT(15))
    #define SPI_DMA_TX_STOP_V  0x1
    #define SPI_DMA_TX_STOP_S  15
    /* SPI_DMA_RX_STOP : R/W ;bitpos:[14] ;default: 1'b0 ; */
    /*description: */
    #define SPI_DMA_RX_STOP  (BIT(14))
    #define SPI_DMA_RX_STOP_M  (BIT(14))
    #define SPI_DMA_RX_STOP_V  0x1
    #define SPI_DMA_RX_STOP_S  14
    /* SPI_OUT_DATA_BURST_EN : R/W ;bitpos:[12] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_DATA_BURST_EN  (BIT(12))
    #define SPI_OUT_DATA_BURST_EN_M  (BIT(12))
    #define SPI_OUT_DATA_BURST_EN_V  0x1
    #define SPI_OUT_DATA_BURST_EN_S  12
    /* SPI_INDSCR_BURST_EN : R/W ;bitpos:[11] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INDSCR_BURST_EN  (BIT(11))
    #define SPI_INDSCR_BURST_EN_M  (BIT(11))
    #define SPI_INDSCR_BURST_EN_V  0x1
    #define SPI_INDSCR_BURST_EN_S  11
    /* SPI_OUTDSCR_BURST_EN : R/W ;bitpos:[10] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUTDSCR_BURST_EN  (BIT(10))
    #define SPI_OUTDSCR_BURST_EN_M  (BIT(10))
    #define SPI_OUTDSCR_BURST_EN_V  0x1
    #define SPI_OUTDSCR_BURST_EN_S  10
    /* SPI_OUT_EOF_MODE : R/W ;bitpos:[9] ;default: 1'b1 ; */
    /*description: */
    #define SPI_OUT_EOF_MODE  (BIT(9))
    #define SPI_OUT_EOF_MODE_M  (BIT(9))
    #define SPI_OUT_EOF_MODE_V  0x1
    #define SPI_OUT_EOF_MODE_S  9
    /* SPI_OUT_AUTO_WRBACK : R/W ;bitpos:[8] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_AUTO_WRBACK  (BIT(8))
    #define SPI_OUT_AUTO_WRBACK_M  (BIT(8))
    #define SPI_OUT_AUTO_WRBACK_V  0x1
    #define SPI_OUT_AUTO_WRBACK_S  8
    /* SPI_OUT_LOOP_TEST : R/W ;bitpos:[7] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_LOOP_TEST  (BIT(7))
    #define SPI_OUT_LOOP_TEST_M  (BIT(7))
    #define SPI_OUT_LOOP_TEST_V  0x1
    #define SPI_OUT_LOOP_TEST_S  7
    /* SPI_IN_LOOP_TEST : R/W ;bitpos:[6] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_LOOP_TEST  (BIT(6))
    #define SPI_IN_LOOP_TEST_M  (BIT(6))
    #define SPI_IN_LOOP_TEST_V  0x1
    #define SPI_IN_LOOP_TEST_S  6
    /* SPI_AHBM_RST : R/W ;bitpos:[5] ;default: 1'b0 ; */
    /*description: */
    #define SPI_AHBM_RST  (BIT(5))
    #define SPI_AHBM_RST_M  (BIT(5))
    #define SPI_AHBM_RST_V  0x1
    #define SPI_AHBM_RST_S  5
    /* SPI_AHBM_FIFO_RST : R/W ;bitpos:[4] ;default: 1'b0 ; */
    /*description: */
    #define SPI_AHBM_FIFO_RST  (BIT(4))
    #define SPI_AHBM_FIFO_RST_M  (BIT(4))
    #define SPI_AHBM_FIFO_RST_V  0x1
    #define SPI_AHBM_FIFO_RST_S  4
    /* SPI_OUT_RST : R/W ;bitpos:[3] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_RST  (BIT(3))
    #define SPI_OUT_RST_M  (BIT(3))
    #define SPI_OUT_RST_V  0x1
    #define SPI_OUT_RST_S  3
    /* SPI_IN_RST : R/W ;bitpos:[2] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_RST  (BIT(2))
    #define SPI_IN_RST_M  (BIT(2))
    #define SPI_IN_RST_V  0x1
    #define SPI_IN_RST_S  2



#define SPI_DMA_OUT_LINK_REG(i)          (REG_SPI_BASE(i) + 0x104)
    /* SPI_OUTLINK_RESTART : R/W ;bitpos:[30] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUTLINK_RESTART  (BIT(30))
    #define SPI_OUTLINK_RESTART_M  (BIT(30))
    #define SPI_OUTLINK_RESTART_V  0x1
    #define SPI_OUTLINK_RESTART_S  30
    /* SPI_OUTLINK_START : R/W ;bitpos:[29] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUTLINK_START  (BIT(29))
    #define SPI_OUTLINK_START_M  (BIT(29))
    #define SPI_OUTLINK_START_V  0x1
    #define SPI_OUTLINK_START_S  29
    /* SPI_OUTLINK_STOP : R/W ;bitpos:[28] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUTLINK_STOP  (BIT(28))
    #define SPI_OUTLINK_STOP_M  (BIT(28))
    #define SPI_OUTLINK_STOP_V  0x1
    #define SPI_OUTLINK_STOP_S  28
    /* SPI_OUTLINK_ADDR : R/W ;bitpos:[19:0] ;default: 20'h0 ; */
    /*description: */
    #define SPI_OUTLINK_ADDR  0x000FFFFF
    #define SPI_OUTLINK_ADDR_M  ((SPI_OUTLINK_ADDR_V)<<(SPI_OUTLINK_ADDR_S))
    #define SPI_OUTLINK_ADDR_V  0xFFFFF
    #define SPI_OUTLINK_ADDR_S  0



#define SPI_DMA_IN_LINK_REG(i)          (REG_SPI_BASE(i) + 0x108)
    /* SPI_INLINK_RESTART : R/W ;bitpos:[30] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INLINK_RESTART  (BIT(30))
    #define SPI_INLINK_RESTART_M  (BIT(30))
    #define SPI_INLINK_RESTART_V  0x1
    #define SPI_INLINK_RESTART_S  30
    /* SPI_INLINK_START : R/W ;bitpos:[29] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INLINK_START  (BIT(29))
    #define SPI_INLINK_START_M  (BIT(29))
    #define SPI_INLINK_START_V  0x1
    #define SPI_INLINK_START_S  29
    /* SPI_INLINK_STOP : R/W ;bitpos:[28] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INLINK_STOP  (BIT(28))
    #define SPI_INLINK_STOP_M  (BIT(28))
    #define SPI_INLINK_STOP_V  0x1
    #define SPI_INLINK_STOP_S  28
    /* SPI_INLINK_AUTO_RET : R/W ;bitpos:[20] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INLINK_AUTO_RET  (BIT(20))
    #define SPI_INLINK_AUTO_RET_M  (BIT(20))
    #define SPI_INLINK_AUTO_RET_V  0x1
    #define SPI_INLINK_AUTO_RET_S  20
    /* SPI_INLINK_ADDR : R/W ;bitpos:[19:0] ;default: 20'h0 ; */
    /*description: */
    #define SPI_INLINK_ADDR  0x000FFFFF
    #define SPI_INLINK_ADDR_M  ((SPI_INLINK_ADDR_V)<<(SPI_INLINK_ADDR_S))
    #define SPI_INLINK_ADDR_V  0xFFFFF
    #define SPI_INLINK_ADDR_S  0



#define SPI_DMA_STATUS_REG(i)          (REG_SPI_BASE(i) + 0x10C)
    /* SPI_DMA_TX_EN : RO ;bitpos:[1] ;default: 1'b0 ; */
    /*description: */
    #define SPI_DMA_TX_EN  (BIT(1))
    #define SPI_DMA_TX_EN_M  (BIT(1))
    #define SPI_DMA_TX_EN_V  0x1
    #define SPI_DMA_TX_EN_S  1
    /* SPI_DMA_RX_EN : RO ;bitpos:[0] ;default: 1'b0 ; */
    /*description: */
    #define SPI_DMA_RX_EN  (BIT(0))
    #define SPI_DMA_RX_EN_M  (BIT(0))
    #define SPI_DMA_RX_EN_V  0x1
    #define SPI_DMA_RX_EN_S  0



#define SPI_DMA_INT_ENA_REG(i)          (REG_SPI_BASE(i) + 0x110)
    /* SPI_OUT_TOTAL_EOF_INT_ENA : R/W ;bitpos:[8] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_TOTAL_EOF_INT_ENA  (BIT(8))
    #define SPI_OUT_TOTAL_EOF_INT_ENA_M  (BIT(8))
    #define SPI_OUT_TOTAL_EOF_INT_ENA_V  0x1
    #define SPI_OUT_TOTAL_EOF_INT_ENA_S  8
    /* SPI_OUT_EOF_INT_ENA : R/W ;bitpos:[7] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_EOF_INT_ENA  (BIT(7))
    #define SPI_OUT_EOF_INT_ENA_M  (BIT(7))
    #define SPI_OUT_EOF_INT_ENA_V  0x1
    #define SPI_OUT_EOF_INT_ENA_S  7
    /* SPI_OUT_DONE_INT_ENA : R/W ;bitpos:[6] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_DONE_INT_ENA  (BIT(6))
    #define SPI_OUT_DONE_INT_ENA_M  (BIT(6))
    #define SPI_OUT_DONE_INT_ENA_V  0x1
    #define SPI_OUT_DONE_INT_ENA_S  6
    /* SPI_IN_SUC_EOF_INT_ENA : R/W ;bitpos:[5] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_SUC_EOF_INT_ENA  (BIT(5))
    #define SPI_IN_SUC_EOF_INT_ENA_M  (BIT(5))
    #define SPI_IN_SUC_EOF_INT_ENA_V  0x1
    #define SPI_IN_SUC_EOF_INT_ENA_S  5
    /* SPI_IN_ERR_EOF_INT_ENA : R/W ;bitpos:[4] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_ERR_EOF_INT_ENA  (BIT(4))
    #define SPI_IN_ERR_EOF_INT_ENA_M  (BIT(4))
    #define SPI_IN_ERR_EOF_INT_ENA_V  0x1
    #define SPI_IN_ERR_EOF_INT_ENA_S  4
    /* SPI_IN_DONE_INT_ENA : R/W ;bitpos:[3] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_DONE_INT_ENA  (BIT(3))
    #define SPI_IN_DONE_INT_ENA_M  (BIT(3))
    #define SPI_IN_DONE_INT_ENA_V  0x1
    #define SPI_IN_DONE_INT_ENA_S  3
    /* SPI_INLINK_DSCR_ERROR_INT_ENA : R/W ;bitpos:[2] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INLINK_DSCR_ERROR_INT_ENA  (BIT(2))
    #define SPI_INLINK_DSCR_ERROR_INT_ENA_M  (BIT(2))
    #define SPI_INLINK_DSCR_ERROR_INT_ENA_V  0x1
    #define SPI_INLINK_DSCR_ERROR_INT_ENA_S  2
    /* SPI_OUTLINK_DSCR_ERROR_INT_ENA : R/W ;bitpos:[1] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUTLINK_DSCR_ERROR_INT_ENA  (BIT(1))
    #define SPI_OUTLINK_DSCR_ERROR_INT_ENA_M  (BIT(1))
    #define SPI_OUTLINK_DSCR_ERROR_INT_ENA_V  0x1
    #define SPI_OUTLINK_DSCR_ERROR_INT_ENA_S  1
    /* SPI_INLINK_DSCR_EMPTY_INT_ENA : R/W ;bitpos:[0] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INLINK_DSCR_EMPTY_INT_ENA  (BIT(0))
    #define SPI_INLINK_DSCR_EMPTY_INT_ENA_M  (BIT(0))
    #define SPI_INLINK_DSCR_EMPTY_INT_ENA_V  0x1
    #define SPI_INLINK_DSCR_EMPTY_INT_ENA_S  0



#define SPI_DMA_INT_RAW_REG(i)          (REG_SPI_BASE(i) + 0x114)
    /* SPI_OUT_TOTAL_EOF_INT_RAW : RO ;bitpos:[8] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_TOTAL_EOF_INT_RAW  (BIT(8))
    #define SPI_OUT_TOTAL_EOF_INT_RAW_M  (BIT(8))
    #define SPI_OUT_TOTAL_EOF_INT_RAW_V  0x1
    #define SPI_OUT_TOTAL_EOF_INT_RAW_S  8
    /* SPI_OUT_EOF_INT_RAW : RO ;bitpos:[7] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_EOF_INT_RAW  (BIT(7))
    #define SPI_OUT_EOF_INT_RAW_M  (BIT(7))
    #define SPI_OUT_EOF_INT_RAW_V  0x1
    #define SPI_OUT_EOF_INT_RAW_S  7
    /* SPI_OUT_DONE_INT_RAW : RO ;bitpos:[6] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_DONE_INT_RAW  (BIT(6))
    #define SPI_OUT_DONE_INT_RAW_M  (BIT(6))
    #define SPI_OUT_DONE_INT_RAW_V  0x1
    #define SPI_OUT_DONE_INT_RAW_S  6
    /* SPI_IN_SUC_EOF_INT_RAW : RO ;bitpos:[5] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_SUC_EOF_INT_RAW  (BIT(5))
    #define SPI_IN_SUC_EOF_INT_RAW_M  (BIT(5))
    #define SPI_IN_SUC_EOF_INT_RAW_V  0x1
    #define SPI_IN_SUC_EOF_INT_RAW_S  5
    /* SPI_IN_ERR_EOF_INT_RAW : RO ;bitpos:[4] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_ERR_EOF_INT_RAW  (BIT(4))
    #define SPI_IN_ERR_EOF_INT_RAW_M  (BIT(4))
    #define SPI_IN_ERR_EOF_INT_RAW_V  0x1
    #define SPI_IN_ERR_EOF_INT_RAW_S  4
    /* SPI_IN_DONE_INT_RAW : RO ;bitpos:[3] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_DONE_INT_RAW  (BIT(3))
    #define SPI_IN_DONE_INT_RAW_M  (BIT(3))
    #define SPI_IN_DONE_INT_RAW_V  0x1
    #define SPI_IN_DONE_INT_RAW_S  3
    /* SPI_INLINK_DSCR_ERROR_INT_RAW : RO ;bitpos:[2] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INLINK_DSCR_ERROR_INT_RAW  (BIT(2))
    #define SPI_INLINK_DSCR_ERROR_INT_RAW_M  (BIT(2))
    #define SPI_INLINK_DSCR_ERROR_INT_RAW_V  0x1
    #define SPI_INLINK_DSCR_ERROR_INT_RAW_S  2
    /* SPI_OUTLINK_DSCR_ERROR_INT_RAW : RO ;bitpos:[1] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUTLINK_DSCR_ERROR_INT_RAW  (BIT(1))
    #define SPI_OUTLINK_DSCR_ERROR_INT_RAW_M  (BIT(1))
    #define SPI_OUTLINK_DSCR_ERROR_INT_RAW_V  0x1
    #define SPI_OUTLINK_DSCR_ERROR_INT_RAW_S  1
    /* SPI_INLINK_DSCR_EMPTY_INT_RAW : RO ;bitpos:[0] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INLINK_DSCR_EMPTY_INT_RAW  (BIT(0))
    #define SPI_INLINK_DSCR_EMPTY_INT_RAW_M  (BIT(0))
    #define SPI_INLINK_DSCR_EMPTY_INT_RAW_V  0x1
    #define SPI_INLINK_DSCR_EMPTY_INT_RAW_S  0



#define SPI_DMA_INT_ST_REG(i)          (REG_SPI_BASE(i) + 0x118)
    /* SPI_OUT_TOTAL_EOF_INT_ST : RO ;bitpos:[8] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_TOTAL_EOF_INT_ST  (BIT(8))
    #define SPI_OUT_TOTAL_EOF_INT_ST_M  (BIT(8))
    #define SPI_OUT_TOTAL_EOF_INT_ST_V  0x1
    #define SPI_OUT_TOTAL_EOF_INT_ST_S  8
    /* SPI_OUT_EOF_INT_ST : RO ;bitpos:[7] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_EOF_INT_ST  (BIT(7))
    #define SPI_OUT_EOF_INT_ST_M  (BIT(7))
    #define SPI_OUT_EOF_INT_ST_V  0x1
    #define SPI_OUT_EOF_INT_ST_S  7
    /* SPI_OUT_DONE_INT_ST : RO ;bitpos:[6] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_DONE_INT_ST  (BIT(6))
    #define SPI_OUT_DONE_INT_ST_M  (BIT(6))
    #define SPI_OUT_DONE_INT_ST_V  0x1
    #define SPI_OUT_DONE_INT_ST_S  6
    /* SPI_IN_SUC_EOF_INT_ST : RO ;bitpos:[5] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_SUC_EOF_INT_ST  (BIT(5))
    #define SPI_IN_SUC_EOF_INT_ST_M  (BIT(5))
    #define SPI_IN_SUC_EOF_INT_ST_V  0x1
    #define SPI_IN_SUC_EOF_INT_ST_S  5
    /* SPI_IN_ERR_EOF_INT_ST : RO ;bitpos:[4] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_ERR_EOF_INT_ST  (BIT(4))
    #define SPI_IN_ERR_EOF_INT_ST_M  (BIT(4))
    #define SPI_IN_ERR_EOF_INT_ST_V  0x1
    #define SPI_IN_ERR_EOF_INT_ST_S  4
    /* SPI_IN_DONE_INT_ST : RO ;bitpos:[3] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_DONE_INT_ST  (BIT(3))
    #define SPI_IN_DONE_INT_ST_M  (BIT(3))
    #define SPI_IN_DONE_INT_ST_V  0x1
    #define SPI_IN_DONE_INT_ST_S  3
    /* SPI_INLINK_DSCR_ERROR_INT_ST : RO ;bitpos:[2] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INLINK_DSCR_ERROR_INT_ST  (BIT(2))
    #define SPI_INLINK_DSCR_ERROR_INT_ST_M  (BIT(2))
    #define SPI_INLINK_DSCR_ERROR_INT_ST_V  0x1
    #define SPI_INLINK_DSCR_ERROR_INT_ST_S  2
    /* SPI_OUTLINK_DSCR_ERROR_INT_ST : RO ;bitpos:[1] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUTLINK_DSCR_ERROR_INT_ST  (BIT(1))
    #define SPI_OUTLINK_DSCR_ERROR_INT_ST_M  (BIT(1))
    #define SPI_OUTLINK_DSCR_ERROR_INT_ST_V  0x1
    #define SPI_OUTLINK_DSCR_ERROR_INT_ST_S  1
    /* SPI_INLINK_DSCR_EMPTY_INT_ST : RO ;bitpos:[0] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INLINK_DSCR_EMPTY_INT_ST  (BIT(0))
    #define SPI_INLINK_DSCR_EMPTY_INT_ST_M  (BIT(0))
    #define SPI_INLINK_DSCR_EMPTY_INT_ST_V  0x1
    #define SPI_INLINK_DSCR_EMPTY_INT_ST_S  0



#define SPI_DMA_INT_CLR_REG(i)          (REG_SPI_BASE(i) + 0x11C)
    /* SPI_OUT_TOTAL_EOF_INT_CLR : R/W ;bitpos:[8] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_TOTAL_EOF_INT_CLR  (BIT(8))
    #define SPI_OUT_TOTAL_EOF_INT_CLR_M  (BIT(8))
    #define SPI_OUT_TOTAL_EOF_INT_CLR_V  0x1
    #define SPI_OUT_TOTAL_EOF_INT_CLR_S  8
    /* SPI_OUT_EOF_INT_CLR : R/W ;bitpos:[7] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_EOF_INT_CLR  (BIT(7))
    #define SPI_OUT_EOF_INT_CLR_M  (BIT(7))
    #define SPI_OUT_EOF_INT_CLR_V  0x1
    #define SPI_OUT_EOF_INT_CLR_S  7
    /* SPI_OUT_DONE_INT_CLR : R/W ;bitpos:[6] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUT_DONE_INT_CLR  (BIT(6))
    #define SPI_OUT_DONE_INT_CLR_M  (BIT(6))
    #define SPI_OUT_DONE_INT_CLR_V  0x1
    #define SPI_OUT_DONE_INT_CLR_S  6
    /* SPI_IN_SUC_EOF_INT_CLR : R/W ;bitpos:[5] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_SUC_EOF_INT_CLR  (BIT(5))
    #define SPI_IN_SUC_EOF_INT_CLR_M  (BIT(5))
    #define SPI_IN_SUC_EOF_INT_CLR_V  0x1
    #define SPI_IN_SUC_EOF_INT_CLR_S  5
    /* SPI_IN_ERR_EOF_INT_CLR : R/W ;bitpos:[4] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_ERR_EOF_INT_CLR  (BIT(4))
    #define SPI_IN_ERR_EOF_INT_CLR_M  (BIT(4))
    #define SPI_IN_ERR_EOF_INT_CLR_V  0x1
    #define SPI_IN_ERR_EOF_INT_CLR_S  4
    /* SPI_IN_DONE_INT_CLR : R/W ;bitpos:[3] ;default: 1'b0 ; */
    /*description: */
    #define SPI_IN_DONE_INT_CLR  (BIT(3))
    #define SPI_IN_DONE_INT_CLR_M  (BIT(3))
    #define SPI_IN_DONE_INT_CLR_V  0x1
    #define SPI_IN_DONE_INT_CLR_S  3
    /* SPI_INLINK_DSCR_ERROR_INT_CLR : R/W ;bitpos:[2] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INLINK_DSCR_ERROR_INT_CLR  (BIT(2))
    #define SPI_INLINK_DSCR_ERROR_INT_CLR_M  (BIT(2))
    #define SPI_INLINK_DSCR_ERROR_INT_CLR_V  0x1
    #define SPI_INLINK_DSCR_ERROR_INT_CLR_S  2
    /* SPI_OUTLINK_DSCR_ERROR_INT_CLR : R/W ;bitpos:[1] ;default: 1'b0 ; */
    /*description: */
    #define SPI_OUTLINK_DSCR_ERROR_INT_CLR  (BIT(1))
    #define SPI_OUTLINK_DSCR_ERROR_INT_CLR_M  (BIT(1))
    #define SPI_OUTLINK_DSCR_ERROR_INT_CLR_V  0x1
    #define SPI_OUTLINK_DSCR_ERROR_INT_CLR_S  1
    /* SPI_INLINK_DSCR_EMPTY_INT_CLR : R/W ;bitpos:[0] ;default: 1'b0 ; */
    /*description: */
    #define SPI_INLINK_DSCR_EMPTY_INT_CLR  (BIT(0))
    #define SPI_INLINK_DSCR_EMPTY_INT_CLR_M  (BIT(0))
    #define SPI_INLINK_DSCR_EMPTY_INT_CLR_V  0x1
    #define SPI_INLINK_DSCR_EMPTY_INT_CLR_S  0



#define SPI_IN_ERR_EOF_DES_ADDR_REG(i)          (REG_SPI_BASE(i) + 0x120)
    /* SPI_DMA_IN_ERR_EOF_DES_ADDR : RO ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_DMA_IN_ERR_EOF_DES_ADDR  0xFFFFFFFF
    #define SPI_DMA_IN_ERR_EOF_DES_ADDR_M  ((SPI_DMA_IN_ERR_EOF_DES_ADDR_V)<<(SPI_DMA_IN_ERR_EOF_DES_ADDR_S))
    #define SPI_DMA_IN_ERR_EOF_DES_ADDR_V  0xFFFFFFFF
    #define SPI_DMA_IN_ERR_EOF_DES_ADDR_S  0



#define SPI_IN_SUC_EOF_DES_ADDR_REG(i)          (REG_SPI_BASE(i) + 0x124)
    /* SPI_DMA_IN_SUC_EOF_DES_ADDR : RO ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_DMA_IN_SUC_EOF_DES_ADDR  0xFFFFFFFF
    #define SPI_DMA_IN_SUC_EOF_DES_ADDR_M  ((SPI_DMA_IN_SUC_EOF_DES_ADDR_V)<<(SPI_DMA_IN_SUC_EOF_DES_ADDR_S))
    #define SPI_DMA_IN_SUC_EOF_DES_ADDR_V  0xFFFFFFFF
    #define SPI_DMA_IN_SUC_EOF_DES_ADDR_S  0



#define SPI_INLINK_DSCR_REG(i)          (REG_SPI_BASE(i) + 0x128)
    /* SPI_DMA_INLINK_DSCR : RO ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_DMA_INLINK_DSCR  0xFFFFFFFF
    #define SPI_DMA_INLINK_DSCR_M  ((SPI_DMA_INLINK_DSCR_V)<<(SPI_DMA_INLINK_DSCR_S))
    #define SPI_DMA_INLINK_DSCR_V  0xFFFFFFFF
    #define SPI_DMA_INLINK_DSCR_S  0



#define SPI_INLINK_DSCR_BF0_REG(i)          (REG_SPI_BASE(i) + 0x12C)
    /* SPI_DMA_INLINK_DSCR_BF0 : RO ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_DMA_INLINK_DSCR_BF0  0xFFFFFFFF
    #define SPI_DMA_INLINK_DSCR_BF0_M  ((SPI_DMA_INLINK_DSCR_BF0_V)<<(SPI_DMA_INLINK_DSCR_BF0_S))
    #define SPI_DMA_INLINK_DSCR_BF0_V  0xFFFFFFFF
    #define SPI_DMA_INLINK_DSCR_BF0_S  0



#define SPI_INLINK_DSCR_BF1_REG(i)          (REG_SPI_BASE(i) + 0x130)
    /* SPI_DMA_INLINK_DSCR_BF1 : RO ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_DMA_INLINK_DSCR_BF1  0xFFFFFFFF
    #define SPI_DMA_INLINK_DSCR_BF1_M  ((SPI_DMA_INLINK_DSCR_BF1_V)<<(SPI_DMA_INLINK_DSCR_BF1_S))
    #define SPI_DMA_INLINK_DSCR_BF1_V  0xFFFFFFFF
    #define SPI_DMA_INLINK_DSCR_BF1_S  0



#define SPI_OUT_EOF_BFR_DES_ADDR_REG(i)          (REG_SPI_BASE(i) + 0x134)
    /* SPI_DMA_OUT_EOF_BFR_DES_ADDR : RO ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_DMA_OUT_EOF_BFR_DES_ADDR  0xFFFFFFFF
    #define SPI_DMA_OUT_EOF_BFR_DES_ADDR_M  ((SPI_DMA_OUT_EOF_BFR_DES_ADDR_V)<<(SPI_DMA_OUT_EOF_BFR_DES_ADDR_S))
    #define SPI_DMA_OUT_EOF_BFR_DES_ADDR_V  0xFFFFFFFF
    #define SPI_DMA_OUT_EOF_BFR_DES_ADDR_S  0



#define SPI_OUT_EOF_DES_ADDR_REG(i)          (REG_SPI_BASE(i) + 0x138)
    /* SPI_DMA_OUT_EOF_DES_ADDR : RO ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_DMA_OUT_EOF_DES_ADDR  0xFFFFFFFF
    #define SPI_DMA_OUT_EOF_DES_ADDR_M  ((SPI_DMA_OUT_EOF_DES_ADDR_V)<<(SPI_DMA_OUT_EOF_DES_ADDR_S))
    #define SPI_DMA_OUT_EOF_DES_ADDR_V  0xFFFFFFFF
    #define SPI_DMA_OUT_EOF_DES_ADDR_S  0



#define SPI_OUTLINK_DSCR_REG(i)          (REG_SPI_BASE(i) + 0x13C)
    /* SPI_DMA_OUTLINK_DSCR : RO ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_DMA_OUTLINK_DSCR  0xFFFFFFFF
    #define SPI_DMA_OUTLINK_DSCR_M  ((SPI_DMA_OUTLINK_DSCR_V)<<(SPI_DMA_OUTLINK_DSCR_S))
    #define SPI_DMA_OUTLINK_DSCR_V  0xFFFFFFFF
    #define SPI_DMA_OUTLINK_DSCR_S  0



#define SPI_OUTLINK_DSCR_BF0_REG(i)          (REG_SPI_BASE(i) + 0x140)
    /* SPI_DMA_OUTLINK_DSCR_BF0 : RO ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_DMA_OUTLINK_DSCR_BF0  0xFFFFFFFF
    #define SPI_DMA_OUTLINK_DSCR_BF0_M  ((SPI_DMA_OUTLINK_DSCR_BF0_V)<<(SPI_DMA_OUTLINK_DSCR_BF0_S))
    #define SPI_DMA_OUTLINK_DSCR_BF0_V  0xFFFFFFFF
    #define SPI_DMA_OUTLINK_DSCR_BF0_S  0



#define SPI_OUTLINK_DSCR_BF1_REG(i)          (REG_SPI_BASE(i) + 0x144)
    /* SPI_DMA_OUTLINK_DSCR_BF1 : RO ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_DMA_OUTLINK_DSCR_BF1  0xFFFFFFFF
    #define SPI_DMA_OUTLINK_DSCR_BF1_M  ((SPI_DMA_OUTLINK_DSCR_BF1_V)<<(SPI_DMA_OUTLINK_DSCR_BF1_S))
    #define SPI_DMA_OUTLINK_DSCR_BF1_V  0xFFFFFFFF
    #define SPI_DMA_OUTLINK_DSCR_BF1_S  0



#define SPI_DMA_RSTATUS_REG(i)          (REG_SPI_BASE(i) + 0x148)
    /* SPI_DMA_OUT_STATUS : RO ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_DMA_OUT_STATUS  0xFFFFFFFF
    #define SPI_DMA_OUT_STATUS_M  ((SPI_DMA_OUT_STATUS_V)<<(SPI_DMA_OUT_STATUS_S))
    #define SPI_DMA_OUT_STATUS_V  0xFFFFFFFF
    #define SPI_DMA_OUT_STATUS_S  0



#define SPI_DMA_TSTATUS_REG(i)          (REG_SPI_BASE(i) + 0x14C)
    /* SPI_DMA_IN_STATUS : RO ;bitpos:[31:0] ;default: 32'b0 ; */
    /*description: */
    #define SPI_DMA_IN_STATUS  0xFFFFFFFF
    #define SPI_DMA_IN_STATUS_M  ((SPI_DMA_IN_STATUS_V)<<(SPI_DMA_IN_STATUS_S))
    #define SPI_DMA_IN_STATUS_V  0xFFFFFFFF
    #define SPI_DMA_IN_STATUS_S  0



#define SPI_REG_DATE_REG(i)          (REG_SPI_BASE(i) + 0x3FC)
    /* SPI_DATE : RO ;bitpos:[27:0] ;default: 32'h1604270 ; */
    /*description: */
    #define SPI_DATE  0x0FFFFFFF
    #define SPI_DATE_M  ((SPI_DATE_V)<<(SPI_DATE_S))
    #define SPI_DATE_V  0xFFFFFFF
    #define SPI_DATE_S  0
#define SPI_DATE_VERSION 0x1604290
 
#endif /* _SOC_SPI_REG_H_ */
