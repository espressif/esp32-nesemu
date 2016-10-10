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

#ifndef _DRIVER_GPIO_H_
#define _DRIVER_GPIO_H_

#include <esp_types.h>

#include "soc/gpio_reg.h"
#include "soc/rtc_io_reg.h"

#include "soc/io_mux_reg.h"
#include "esp_attr.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const uint32_t GPIO_PIN_MUX_REG[40];

#define GPIO_Pin_0              (BIT(0))  /* Pin 0 selected */
#define GPIO_Pin_1              (BIT(1))  /* Pin 1 selected */
#define GPIO_Pin_2              (BIT(2))  /* Pin 2 selected */
#define GPIO_Pin_3              (BIT(3))  /* Pin 3 selected */
#define GPIO_Pin_4              (BIT(4))  /* Pin 4 selected */
#define GPIO_Pin_5              (BIT(5))  /* Pin 5 selected */
#define GPIO_Pin_6              (BIT(6))  /* Pin 6 selected */
#define GPIO_Pin_7              (BIT(7))  /* Pin 7 selected */
#define GPIO_Pin_8              (BIT(8))  /* Pin 8 selected */
#define GPIO_Pin_9              (BIT(9))  /* Pin 9 selected */
#define GPIO_Pin_10             (BIT(10)) /* Pin 10 selected */
#define GPIO_Pin_11             (BIT(11)) /* Pin 11 selected */
#define GPIO_Pin_12             (BIT(12)) /* Pin 12 selected */
#define GPIO_Pin_13             (BIT(13)) /* Pin 13 selected */
#define GPIO_Pin_14             (BIT(14)) /* Pin 14 selected */
#define GPIO_Pin_15             (BIT(15)) /* Pin 15 selected */
#define GPIO_Pin_16             (BIT(16)) /* Pin 16 selected */
#define GPIO_Pin_17             (BIT(17)) /* Pin 17 selected */
#define GPIO_Pin_18             (BIT(18)) /* Pin 18 selected */
#define GPIO_Pin_19             (BIT(19)) /* Pin 19 selected */
#define GPIO_Pin_20             (BIT(20)) /* Pin 20 selected */
#define GPIO_Pin_21             (BIT(21)) /* Pin 21 selected */
#define GPIO_Pin_22             (BIT(22)) /* Pin 22 selected */
#define GPIO_Pin_23             (BIT(23)) /* Pin 23 selected */
//#define GPIO_Pin_24             (BIT(24)) /* Pin 24 selected */
#define GPIO_Pin_25             (BIT(25)) /* Pin 25 selected */
#define GPIO_Pin_26             (BIT(26)) /* Pin 26 selected */
#define GPIO_Pin_27             (BIT(27)) /* Pin 27 selected */
//#define GPIO_Pin_28             (BIT(28)) /* Pin 28 selected */
//#define GPIO_Pin_29             (BIT(29)) /* Pin 29 selected */
//#define GPIO_Pin_30             (BIT(30)) /* Pin 30 selected */
//#define GPIO_Pin_31             (BIT(31)) /* Pin 31 selected */
#define GPIO_Pin_32             ((uint64_t)(((uint64_t)1)<<32))  /* Pin 32 selected */
#define GPIO_Pin_33             ((uint64_t)(((uint64_t)1)<<33))  /* Pin 33 selected */
#define GPIO_Pin_34             ((uint64_t)(((uint64_t)1)<<34))  /* Pin 34 selected */
#define GPIO_Pin_35             ((uint64_t)(((uint64_t)1)<<35))  /* Pin 35 selected */
#define GPIO_Pin_36             ((uint64_t)(((uint64_t)1)<<36))  /* Pin 36 selected */
#define GPIO_Pin_37             ((uint64_t)(((uint64_t)1)<<37))  /* Pin 37 selected */
#define GPIO_Pin_38             ((uint64_t)(((uint64_t)1)<<38))  /* Pin 38 selected */
#define GPIO_Pin_39             ((uint64_t)(((uint64_t)1)<<39))  /* Pin 39 selected */

#define GPIO_PIN_REG_0          PERIPHS_IO_MUX_GPIO0_U
#define GPIO_PIN_REG_1          PERIPHS_IO_MUX_U0TXD_U
#define GPIO_PIN_REG_2          PERIPHS_IO_MUX_GPIO2_U
#define GPIO_PIN_REG_3          PERIPHS_IO_MUX_U0RXD_U
#define GPIO_PIN_REG_4          PERIPHS_IO_MUX_GPIO4_U
#define GPIO_PIN_REG_5          PERIPHS_IO_MUX_GPIO5_U
#define GPIO_PIN_REG_6          PERIPHS_IO_MUX_SD_CLK_U
#define GPIO_PIN_REG_7          PERIPHS_IO_MUX_SD_DATA0_U
#define GPIO_PIN_REG_8          PERIPHS_IO_MUX_SD_DATA1_U
#define GPIO_PIN_REG_9          PERIPHS_IO_MUX_SD_DATA2_U
#define GPIO_PIN_REG_10         PERIPHS_IO_MUX_SD_DATA3_U
#define GPIO_PIN_REG_11         PERIPHS_IO_MUX_SD_CMD_U
#define GPIO_PIN_REG_12         PERIPHS_IO_MUX_MTDI_U
#define GPIO_PIN_REG_13         PERIPHS_IO_MUX_MTCK_U
#define GPIO_PIN_REG_14         PERIPHS_IO_MUX_MTMS_U
#define GPIO_PIN_REG_15         PERIPHS_IO_MUX_MTDO_U
#define GPIO_PIN_REG_16         PERIPHS_IO_MUX_GPIO16_U
#define GPIO_PIN_REG_17         PERIPHS_IO_MUX_GPIO17_U
#define GPIO_PIN_REG_18         PERIPHS_IO_MUX_GPIO18_U
#define GPIO_PIN_REG_19         PERIPHS_IO_MUX_GPIO19_U
#define GPIO_PIN_REG_20         PERIPHS_IO_MUX_GPIO20_U
#define GPIO_PIN_REG_21         PERIPHS_IO_MUX_GPIO21_U
#define GPIO_PIN_REG_22         PERIPHS_IO_MUX_GPIO22_U
#define GPIO_PIN_REG_23         PERIPHS_IO_MUX_GPIO23_U
#define GPIO_PIN_REG_25         PERIPHS_IO_MUX_GPIO25_U
#define GPIO_PIN_REG_26         PERIPHS_IO_MUX_GPIO26_U
#define GPIO_PIN_REG_27         PERIPHS_IO_MUX_GPIO27_U
#define GPIO_PIN_REG_32         PERIPHS_IO_MUX_GPIO32_U
#define GPIO_PIN_REG_33         PERIPHS_IO_MUX_GPIO33_U
#define GPIO_PIN_REG_34         PERIPHS_IO_MUX_GPIO34_U
#define GPIO_PIN_REG_35         PERIPHS_IO_MUX_GPIO35_U
#define GPIO_PIN_REG_36         PERIPHS_IO_MUX_GPIO36_U
#define GPIO_PIN_REG_37         PERIPHS_IO_MUX_GPIO37_U
#define GPIO_PIN_REG_38         PERIPHS_IO_MUX_GPIO38_U
#define GPIO_PIN_REG_39         PERIPHS_IO_MUX_GPIO39_U



#define GPIO_FUNC_IN_SEL_CFG_REG(sig_idx)  ( GPIO_FUNC0_IN_SEL_CFG_REG + (sig_idx)*4)
    #define GPIO_SIG_IN_SEL (BIT(7))
    #define GPIO_SIG_IN_SEL_M (BIT(7))
    #define GPIO_SIG_IN_SEL_V 0x1
    #define GPIO_SIG_IN_SEL_S 7
    #define GPIO_FUNC_IN_INV_SEL (BIT(6))
    #define GPIO_FUNC_IN_INV_SEL_M (BIT(6))
    #define GPIO_FUNC_IN_INV_SEL_V 0x1
    #define GPIO_FUNC_IN_INV_SEL_S 6
    #define GPIO_FUNC_IN_SEL 0x0000003F
    #define GPIO_FUNC_IN_SEL_M ((GPIO_FUNC_IN_SEL_V)<<(GPIO_FUNC_IN_SEL_S))
    #define GPIO_FUNC_IN_SEL_V 0x0000003F
    #define GPIO_FUNC_IN_SEL_S 0
    	#define GPIO_GET_SIG_IN_SEL(sig_idx)	   GET_PERI_REG_BITS2(GPIO_FUNC_IN_SEL_CFG_REG(sig_idx),GPIO_SIG_IN_SEL_V,GPIO_SIG_IN_SEL_S)
    	#define GPIO_SET_SIG_IN_SEL(sig_idx,val) SET_PERI_REG_BITS(GPIO_FUNC_IN_SEL_CFG_REG(sig_idx),GPIO_SIG_IN_SEL_V,val,GPIO_SIG_IN_SEL_S)
    	#define GPIO_GET_FUNC_IN_INV_SEL(sig_idx) 	GET_PERI_REG_BITS2(GPIO_FUNC_IN_SEL_CFG_REG(sig_idx),GPIO_FUNC_IN_INV_SEL_V,GPIO_FUNC_IN_INV_SEL_S)
    	#define GPIO_SET_FUNC_IN_INV_SEL(sig_idx,val) SET_PERI_REG_BITS(GPIO_FUNC_IN_SEL_CFG_REG(sig_idx),GPIO_FUNC_IN_INV_SEL_V,val,GPIO_FUNC_IN_INV_SEL_S)
    	#define GPIO_GET_FUNC_IN_SEL(sig_idx) 	GET_PERI_REG_BITS2(GPIO_FUNC_IN_SEL_CFG_REG(sig_idx),GPIO_FUNC_IN_SEL_V,GPIO_FUNC_IN_SEL_S)
    	#define GPIO_SET_FUNC_IN_SEL(sig_idx,val) SET_PERI_REG_BITS(GPIO_FUNC_IN_SEL_CFG_REG(sig_idx),GPIO_FUNC_IN_SEL_V,val,GPIO_FUNC_IN_SEL_S)


#define GPIO_FUNC_OUT_SEL_CFG_REG(io_num)  ( GPIO_FUNC0_OUT_SEL_CFG_REG+(io_num)*4 )
    #define GPIO_FUNC_OEN_INV_SEL (BIT(11))
    #define GPIO_FUNC_OEN_INV_SEL_M (BIT(11))
    #define GPIO_FUNC_OEN_INV_SEL_V 0x1
    #define GPIO_FUNC_OEN_INV_SEL_S 11
    #define GPIO_FUNC_OEN_SEL (BIT(10))
    #define GPIO_FUNC_OEN_SEL_M (BIT(10))
    #define GPIO_FUNC_OEN_SEL_V 0x1
    #define GPIO_FUNC_OEN_SEL_S 10
    #define GPIO_FUNC_OUT_INV_SEL (BIT(9))
    #define GPIO_FUNC_OUT_INV_SEL_M (BIT(9))
    #define GPIO_FUNC_OUT_INV_SEL_V 0x1
    #define GPIO_FUNC_OUT_INV_SEL_S 9
    #define GPIO_FUNC_OUT_SEL 0x000001FF
    #define GPIO_FUNC_OUT_SEL_M ((GPIO_FUNC_OUT_SEL_V)<<(GPIO_FUNC_OUT_SEL_S))
    #define GPIO_FUNC_OUT_SEL_V 0x000001FF
    #define GPIO_FUNC_OUT_SEL_S 0
    	#define GPIO_GET_FUNC_OEN_INV_SEL(io_num)	 GET_PERI_REG_BITS2(GPIO_FUNC_OUT_SEL_CFG_REG(io_num),GPIO_FUNC_OEN_INV_SEL_V,GPIO_FUNC_OEN_INV_SEL_S)
    	#define GPIO_SET_FUNC_OEN_INV_SEL(io_num,val) SET_PERI_REG_BITS(GPIO_FUNC_OUT_SEL_CFG_REG(io_num),GPIO_FUNC_OEN_INV_SEL_V,val,GPIO_FUNC_OEN_INV_SEL_S)
    	#define GPIO_GET_FUNC_OEN_SEL(io_num)	 GET_PERI_REG_BITS2(GPIO_FUNC_OUT_SEL_CFG_REG(io_num),GPIO_FUNC_OEN_SEL_V,GPIO_FUNC_OEN_SEL_S)
    	#define GPIO_SET_FUNC_OEN_SEL(io_num,val) SET_PERI_REG_BITS(GPIO_FUNC_OUT_SEL_CFG_REG(io_num),GPIO_FUNC_OEN_SEL_V,val,GPIO_FUNC_OEN_SEL_S)
    	#define GPIO_GET_FUNC_OUT_INV_SEL(io_num)	 GET_PERI_REG_BITS2(GPIO_FUNC_OUT_SEL_CFG_REG(io_num),GPIO_FUNC_OUT_INV_SEL_V,GPIO_FUNC_OUT_INV_SEL_S)
    	#define GPIO_SET_FUNC_OUT_INV_SEL(io_num,val) SET_PERI_REG_BITS(GPIO_FUNC_OUT_SEL_CFG_REG(io_num),GPIO_FUNC_OUT_INV_SEL_V,val,GPIO_FUNC_OUT_INV_SEL_S)
    	#define GPIO_GET_FUNC_OUT_SEL(io_num)	 GET_PERI_REG_BITS2(GPIO_FUNC_OUT_SEL_CFG_REG(io_num),GPIO_FUNC_OUT_SEL_V,GPIO_FUNC_OUT_SEL_S)
    	#define GPIO_SET_FUNC_OUT_SEL(io_num,val) SET_PERI_REG_BITS(GPIO_FUNC_OUT_SEL_CFG_REG(io_num),GPIO_FUNC_OUT_SEL_V,val,GPIO_FUNC_OUT_SEL_S)



#define GPIO_REG(io_num)      (GPIO_PIN0_REG + (io_num)*0x4)
    #define GPIO_PIN_INT_ENA 0x0000001F
    #define GPIO_PIN_INT_ENA_M ((GPIO_PIN_INT_ENA_V)<<(GPIO_PIN_INT_ENA_S))
    #define GPIO_PIN_INT_ENA_V 0x0000001F
    #define GPIO_PIN_INT_ENA_S 13
    #define GPIO_PIN_CONFIG 0x00000003
    #define GPIO_PIN_CONFIG_M ((GPIO_PIN_CONFIG_V)<<(GPIO_PIN_CONFIG_S))
    #define GPIO_PIN_CONFIG_V 0x00000003
    #define GPIO_PIN_CONFIG_S 11
    #define GPIO_PIN_WAKEUP_ENABLE (BIT(10))
    #define GPIO_PIN_WAKEUP_ENABLE_M (BIT(10))
    #define GPIO_PIN_WAKEUP_ENABLE_V 0x1
    #define GPIO_PIN_WAKEUP_ENABLE_S 10
    #define GPIO_PIN_INT_TYPE 0x00000007
    #define GPIO_PIN_INT_TYPE_M ((GPIO_PIN_INT_TYPE_V)<<(GPIO_PIN_INT_TYPE_S))
    #define GPIO_PIN_INT_TYPE_V 0x00000007
    #define GPIO_PIN_INT_TYPE_S 7
    #define GPIO_PIN_PAD_DRIVER (BIT(2))
    #define GPIO_PIN_PAD_DRIVER_M (BIT(2))
    #define GPIO_PIN_PAD_DRIVER_V 0x1
    #define GPIO_PIN_PAD_DRIVER_S 2
        #define GPIO_GET_PIN_INT_ENA(io_num)     GET_PERI_REG_BITS2(GPIO_REG(io_num),GPIO_PIN_INT_ENA_V,GPIO_PIN_INT_ENA_S)
        #define GPIO_SET_PIN_INT_ENA(io_num,val) SET_PERI_REG_BITS(GPIO_REG(io_num),GPIO_PIN_INT_ENA_V,val,GPIO_PIN_INT_ENA_S)
        #define GPIO_GET_PIN_CONFIG(io_num)     GET_PERI_REG_BITS2(GPIO_REG(io_num),GPIO_PIN_CONFIG_V,GPIO_PIN_CONFIG_S)
        #define GPIO_SET_PIN_CONFIG(io_num,val) SET_PERI_REG_BITS(GPIO_REG(io_num),GPIO_PIN_CONFIG_V,val,GPIO_PIN_CONFIG_S)
        #define GPIO_GET_PIN_WAKEUP_ENABLE(io_num)     GET_PERI_REG_BITS2(GPIO_REG(io_num),GPIO_PIN_WAKEUP_ENABLE_V,GPIO_PIN_WAKEUP_ENABLE_S)
        #define GPIO_SET_PIN_WAKEUP_ENABLE(io_num,val) SET_PERI_REG_BITS(GPIO_REG(io_num),GPIO_PIN_WAKEUP_ENABLE_V,val,GPIO_PIN_WAKEUP_ENABLE_S)
        #define GPIO_GET_PIN_INT_TYPE(io_num)     GET_PERI_REG_BITS2(GPIO_REG(io_num),GPIO_PIN_INT_TYPE_V,GPIO_PIN_INT_TYPE_S)
        #define GPIO_SET_PIN_INT_TYPE(io_num,val) SET_PERI_REG_BITS(GPIO_REG(io_num),GPIO_PIN_INT_TYPE_V,val,GPIO_PIN_INT_TYPE_S)
        #define GPIO_GET_PIN_PAD_DRIVER(io_num)     GET_PERI_REG_BITS2(GPIO_REG(io_num),GPIO_PIN_PAD_DRIVER_V,GPIO_PIN_PAD_DRIVER_S)
        #define GPIO_SET_PIN_PAD_DRIVER(io_num,val) SET_PERI_REG_BITS(GPIO_REG(io_num),GPIO_PIN_PAD_DRIVER_V,val,GPIO_PIN_PAD_DRIVER_S)





#define GPIO_REG_READ(reg)          READ_PERI_REG(reg)
#define GPIO_REG_WRITE(reg, val)    WRITE_PERI_REG(reg, val)
#define GPIO_PIN_COUNT              40
#define GPIO_ID_PIN0                0
#define GPIO_ID_PIN(n)              (GPIO_ID_PIN0 + (n))
#define GPIO_PIN_ADDR(i)            (GPIO_PIN0_REG + i * 4)

#define GPIO_ID_IS_PIN_REGISTER(reg_id) \
    ((reg_id >= GPIO_ID_PIN0) && (reg_id <= GPIO_ID_PIN(GPIO_PIN_COUNT - 1)))

#define GPIO_REGID_TO_PINIDX(reg_id) ((reg_id) - GPIO_ID_PIN0)

typedef enum{
	GPIO_NUM_0=0,
	GPIO_NUM_1=1,
	GPIO_NUM_2=2,
	GPIO_NUM_3=3,
	GPIO_NUM_4=4,
	GPIO_NUM_5=5,
	GPIO_NUM_6=6,
	GPIO_NUM_7=7,
	GPIO_NUM_8=8,
	GPIO_NUM_9=9,
	GPIO_NUM_10=10,
	GPIO_NUM_11=11,
	GPIO_NUM_12=12,
	GPIO_NUM_13=13,
	GPIO_NUM_14=14,
	GPIO_NUM_15=15,
	GPIO_NUM_16=16,
	GPIO_NUM_17=17,
	GPIO_NUM_18=18,
	GPIO_NUM_19=19,
	GPIO_NUM_20=20,
	GPIO_NUM_21=21,
	GPIO_NUM_22=22,
	GPIO_NUM_23=23,

	GPIO_NUM_25=25,
	GPIO_NUM_26=26,
	GPIO_NUM_27=27,


	
	GPIO_NUM_32=32,
	GPIO_NUM_33=33,
	GPIO_NUM_34=34,//only input
	GPIO_NUM_35=35,//only input
	GPIO_NUM_36=36,//only input
	GPIO_NUM_37=37,//only input
	GPIO_NUM_38=38,//only input
	GPIO_NUM_39=39,//only input


}enum_gpio_num_t;
typedef enum {
    GPIO_PIN_INTR_DISABLE = 0,      /**< disable GPIO interrupt */
    GPIO_PIN_INTR_POSEDGE = 1,      /**< GPIO interrupt type : rising edge */
    GPIO_PIN_INTR_NEGEDGE = 2,      /**< GPIO interrupt type : falling edge */
    GPIO_PIN_INTR_ANYEDGE = 3,      /**< GPIO interrupt type : bothe rising and falling edge */
    GPIO_PIN_INTR_LOW_LEVEL=4,
    GPIO_PIN_INTR_HIGH_LEVEL=5,
} enum_gpio_int_type_t;

typedef enum {
    GPIO_Mode_Input=0,          /**< GPIO mode : Input */
    GPIO_Mode_Out_OD=1,               /**< GPIO mode : Output_OD */
    GPIO_Mode_Output=2,              /**< GPIO mode : Output */
    GPIO_Mode_Input_OutOD=3,
    GPIO_Mode_Input_OutPut=4,
    //GPIO_Mode_Sigma_Delta ,         /**< GPIO mode : Sigma_Delta */
} enum_gpio_mode_t;

typedef enum {
    GPIO_PullUp_DIS = 0x0,          /**< disable GPIO pull up */
    GPIO_PullUp_EN  = 0x1,          /**< enable GPIO pull up */
} enum_gpio_pullup_t;

typedef enum {
    GPIO_PullDown_DIS = 0x0,        /**< disable GPIO pull down */
    GPIO_PullDown_EN  = 0x1,        /**< enable GPIO pull down */
} enum_gpio_pulldown_t;

typedef struct {
    uint64_t           GPIO_Pin;      /**< GPIO pin */
    enum_gpio_mode_t GPIO_Mode;     /**< GPIO mode */
    enum_gpio_pullup_t   GPIO_Pullup;   /**< GPIO pullup */
    enum_gpio_pulldown_t GPIO_Pulldown; /**< GPIO pulldown */
    enum_gpio_int_type_t    GPIO_IntrType; /**< GPIO interrupt type */
} gpio_config_t;

/** \defgroup Driver_APIs Driver APIs
  * @brief Driver APIs
  */

/** @addtogroup Driver_APIs
  * @{
  */

/** \defgroup GPIO_Driver_APIs GPIO Driver APIs
  * @brief GPIO APIs
  */

/** @addtogroup GPIO_Driver_APIs
  * @{
  */

#define pin_func_select(gpio_num,fun_num) \
	    do{\
        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[gpio_num],fun_num);\
    }while(0)
#define pin_func_as_gpio(gpio_num) \
    do{\
        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[gpio_num],2);\
    }while(0)
#define gpio_pull_up_enable(gpio_num) \
    do{\
        PIN_PULLUP_EN(GPIO_PIN_MUX_REG[gpio_num]);\
    }while(0)
#define gpio_pull_up_disable(gpio_num) \
 	do{\
 	   PIN_PULLUP_DIS(GPIO_PIN_MUX_REG[gpio_num]);\
 	}while(0)
#define gpio_pull_down_enable(gpio_num) \
    do{\
        PIN_PULLDWN_EN(GPIO_PIN_MUX_REG[gpio_num]);\
    }while(0)
#define gpio_pull_down_disable(gpio_num) \
    do{\
        PIN_PULLDWN_DIS(GPIO_PIN_MUX_REG[gpio_num]);\
    }while(0)

/*-------------------------------------------------------
                   GPIO COMMON CONFIG API
-------------------------------------------------------*/
void gpio_config(gpio_config_t *pGPIOConfig);

void gpio_output_disable(enum_gpio_num_t gpio_num);
void gpio_output_enable(enum_gpio_num_t gpio_num);
void gpio_set_output_level(enum_gpio_num_t gpio_num,uint8_t level);

void gpio_input_enable(enum_gpio_num_t gpio_num);
void gpio_input_diable(enum_gpio_num_t gpio_num);
uint32_t gpio_get_input_level(enum_gpio_num_t gpio_num);

void gpio_enable_intrs(enum_gpio_num_t gpio_num);
void gpio_disable_intrs(enum_gpio_num_t gpio_num);
void gpio_intr_handler_register(void *fn, void *arg);

/*-------------------------------------------------------
                   GPIO SLEEP API
-------------------------------------------------------*/

void gpio_pin_wakeup_enable(enum_gpio_num_t gpio_num, enum_gpio_int_type_t intr_state);
void gpio_pin_wakeup_disable(enum_gpio_num_t gpio_num);

/*-------------------------------------------------------
                   GPIO MATRIX API
-------------------------------------------------------*/
void gpio_matrix_in(uint32_t gpio, uint32_t signal_idx, bool inv) ROMFN_ATTR;
void gpio_matrix_out(uint32_t gpio, uint32_t signal_idx, bool out_inv, bool oen_inv) ROMFN_ATTR;



/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _DRIVER_GPIO_H_ */
