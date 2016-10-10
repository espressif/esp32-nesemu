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

#include <esp_types.h>
#include "rom/ets_sys.h"
#include "gpio.h"
#include "freertos/xtensa_api.h"


#define GPIO_DBG_ENABLE    (1)
#define GPIO_WARING_ENABLE (1)
#define GPIO_ERROR_ENABLE  (1)
#define GPIO_INFO_ENABLE   (1)


//DBG INFOR 
#if GPIO_INFO_ENABLE
    #define GPIO_INFO ets_printf
#else
    #define GPIO_INFO(...)
#endif
#if GPIO_WARING_ENABLE
    #define GPIO_WARING(format,...) do{\
		    ets_printf("[waring][%s#%u]",__FUNCTION__,__LINE__);\
		    ets_printf(format,##__VA_ARGS__);\
	    }while(0)
#else 
     #define GPIO_WARING(...)
#endif
#if GPIO_ERROR_ENABLE
    #define GPIO_ERROR(format,...) do{\
		ets_printf("[error][%s#%u]",__FUNCTION__,__LINE__);\
		ets_printf(format,##__VA_ARGS__);\
	}while(0)
#else 
    #define GPIO_ERROR(...)
#endif 

#define PRO_CPU_NUM (0)
#define APP_CPU_NUM (1)

const uint32_t GPIO_PIN_MUX_REG[40] = {
    GPIO_PIN_REG_0,
    GPIO_PIN_REG_1,
    GPIO_PIN_REG_2,
    GPIO_PIN_REG_3,
    GPIO_PIN_REG_4,
    GPIO_PIN_REG_5,
    GPIO_PIN_REG_6,
    GPIO_PIN_REG_7,
    GPIO_PIN_REG_8,
    GPIO_PIN_REG_9,
    GPIO_PIN_REG_10,
    GPIO_PIN_REG_11,
    GPIO_PIN_REG_12,
    GPIO_PIN_REG_13,
    GPIO_PIN_REG_14,
    GPIO_PIN_REG_15,
    GPIO_PIN_REG_16,
    GPIO_PIN_REG_17,
    GPIO_PIN_REG_18,
    GPIO_PIN_REG_19,
    GPIO_PIN_REG_20,
    GPIO_PIN_REG_21,
    GPIO_PIN_REG_22,
    GPIO_PIN_REG_23,
    0,
    GPIO_PIN_REG_25,
    GPIO_PIN_REG_26,
    GPIO_PIN_REG_27,
    0,
    0,
    0,
    0,
    GPIO_PIN_REG_32,
    GPIO_PIN_REG_33,
    GPIO_PIN_REG_34,
    GPIO_PIN_REG_35,
    GPIO_PIN_REG_36,
    GPIO_PIN_REG_37,
    GPIO_PIN_REG_38,
    GPIO_PIN_REG_39
};
LOCAL void gpio_output_conf(uint64_t set_mask, uint64_t clear_mask, uint64_t enable_mask, uint64_t disable_mask)
{
   //GPIO0-GPIO31
    WRITE_PERI_REG(GPIO_OUT_W1TS_REG, set_mask&0XFFFFFFFF);
    WRITE_PERI_REG(GPIO_OUT_W1TC_REG, clear_mask&0XFFFFFFFF);
    WRITE_PERI_REG(GPIO_ENABLE_W1TS_REG,enable_mask&0XFFFFFFFF);
    WRITE_PERI_REG(GPIO_ENABLE_W1TC_REG, disable_mask&0XFFFFFFFF);   
	//GPIO32-GPIO39
	WRITE_PERI_REG(GPIO_OUT1_W1TS_REG, (set_mask>>32)&0XFF);
    WRITE_PERI_REG(GPIO_OUT1_W1TC_REG, (clear_mask>>32)&0XFF);
    WRITE_PERI_REG(GPIO_ENABLE1_W1TS_REG, (enable_mask>>32)&0XFF);
    WRITE_PERI_REG(GPIO_ENABLE1_W1TC_REG, (disable_mask>>32)&0XFF);
}

LOCAL uint64_t gpio_input_get()
{
    uint64_t inputBits=0;
	inputBits=(READ_PERI_REG(GPIO_IN_REG))|(((uint64_t)(READ_PERI_REG(GPIO_IN1_REG))<<32));
	return inputBits;
}
LOCAL void gpio_set_intr_types(enum_gpio_num_t gpio_num,uint8_t intr_type)
{
     if(gpio_num>=GPIO_PIN_COUNT||0==GPIO_PIN_MUX_REG[gpio_num]){
        GPIO_ERROR("io_num=%d not exits\n",gpio_num);
		return;
	}
	if(intr_type>GPIO_PIN_INTR_HIGH_LEVEL){
        GPIO_ERROR("Unkonw GPIO intr:%u\n",intr_type);
		return;
	}
	SET_PERI_REG_BITS(GPIO_REG(gpio_num),GPIO_PIN_INT_TYPE,intr_type,GPIO_PIN_INT_TYPE_S);
}
void gpio_enable_intrs(enum_gpio_num_t gpio_num)
{
   if(gpio_num>=GPIO_PIN_COUNT||0==GPIO_PIN_MUX_REG[gpio_num]){
        GPIO_ERROR("io_num=%d not exits\n",gpio_num);
		return;
	}
   SET_PERI_REG_BITS(GPIO_REG(gpio_num),GPIO_PIN_INT_ENA,0x04,GPIO_PIN_INT_ENA_S);//Enable Pro CPU Intr
}
void gpio_disable_intrs(enum_gpio_num_t gpio_num)
{
    if(gpio_num>=GPIO_PIN_COUNT||0==GPIO_PIN_MUX_REG[gpio_num]){
        GPIO_ERROR("io_num=%d not exits\n",gpio_num);
		return;
	}
   SET_PERI_REG_BITS(GPIO_REG(gpio_num),GPIO_PIN_INT_ENA,0x00,GPIO_PIN_INT_ENA_S);//Disable Pro CPU Intr
}
void gpio_input_enable(enum_gpio_num_t gpio_num)
{
    if(gpio_num>=GPIO_PIN_COUNT||0==GPIO_PIN_MUX_REG[gpio_num]){
        GPIO_ERROR("io_num=%d not exits\n",gpio_num);
		return;
	}
	
    PIN_INPUT_ENABLE(GPIO_PIN_MUX_REG[gpio_num]);			
}
void gpio_input_diable(enum_gpio_num_t gpio_num)
{
    if(gpio_num>=GPIO_PIN_COUNT||0==GPIO_PIN_MUX_REG[gpio_num]){
        GPIO_ERROR("io_num=%d not exits\n",gpio_num);
		return;
	}
	
    PIN_INPUT_DISABLE(GPIO_PIN_MUX_REG[gpio_num]);	

}
void gpio_output_disable(enum_gpio_num_t gpio_num)
{
    if(gpio_num>=GPIO_PIN_COUNT||0==GPIO_PIN_MUX_REG[gpio_num]){
        GPIO_ERROR("io_num=%d not exits\n",gpio_num);
		return;
	}
    gpio_output_conf(0x00,0x00,0x00,(uint64_t)(((uint64_t)(1))<<gpio_num));
}
void gpio_output_enable(enum_gpio_num_t gpio_num)
{
    if(gpio_num>=GPIO_PIN_COUNT||0==GPIO_PIN_MUX_REG[gpio_num]){
        GPIO_ERROR("io_num=%d not exits\n",gpio_num);
		return;
	}
	if(gpio_num>=34){
        GPIO_ERROR("io_num=%d only input\n",gpio_num);
		return;
	}
    gpio_output_conf(0x00,0x00,(uint64_t)(((uint64_t)(1))<<gpio_num),0x00);

}
void gpio_set_output_level(enum_gpio_num_t gpio_num,uint8_t level)
{
    if(gpio_num>=GPIO_PIN_COUNT||0==GPIO_PIN_MUX_REG[gpio_num]){
        GPIO_ERROR("io_num=%d not exits\n",gpio_num);
		return;
	}
    level?gpio_output_conf((uint64_t)(((uint64_t)1)<<gpio_num),0x00,(uint64_t)(((uint64_t)1)<<gpio_num),0x00):gpio_output_conf(0x00,(uint64_t)(((uint64_t)1)<<gpio_num),(uint64_t)(((uint64_t)1)<<gpio_num),0x00);
}
uint32_t  gpio_get_input_level(enum_gpio_num_t gpio_num)
{
    if(gpio_num>=GPIO_PIN_COUNT||0==GPIO_PIN_MUX_REG[gpio_num]){
        GPIO_ERROR("io_num=%d not exits\n",gpio_num);
		return 2;
	}
	gpio_input_enable(gpio_num);
    uint64_t gpio_input_bits=gpio_input_get();
	uint32_t level=((gpio_input_bits>>gpio_num)&BIT(0));
    return level;
}
void gpio_config(gpio_config_t *pGPIOConfig)
{
    uint64_t gpio_pin_mask=(pGPIOConfig->GPIO_Pin);
    uint32_t io_reg=0;
    uint8_t io_num = 0;
    uint32_t pin_reg=0;
    uint64_t bit_valid=0;
    if(pGPIOConfig->GPIO_Pin==0){
        GPIO_ERROR("Gpio_pin=0 \n");
		return ;
	}
    if ((pGPIOConfig->GPIO_Mode == GPIO_Mode_Output)||(pGPIOConfig->GPIO_Mode == GPIO_Mode_Input_OutPut)) {
        gpio_output_conf(0x00,0x00,gpio_pin_mask,0x00);
		//gpio 34 35 36 37 38 39 only input
		if((gpio_pin_mask&GPIO_Pin_34)||(gpio_pin_mask&GPIO_Pin_35)||(gpio_pin_mask&GPIO_Pin_36)||\
			(gpio_pin_mask&GPIO_Pin_37)||(gpio_pin_mask&GPIO_Pin_38)||(gpio_pin_mask&GPIO_Pin_39)){
             GPIO_ERROR("gpio 34-39  can only input\n");
		}
    }

    do {
       // bit_valid = (io_num >= 32 ? (gpio_pin_mask_high & (0x1 << (io_num - 32))) : (gpio_pin_mask & (0x1 << io_num)));
        bit_valid=(gpio_pin_mask&(uint64_t)(((uint64_t)0x1)<<io_num));
        if (bit_valid && (io_reg = GPIO_PIN_MUX_REG[io_num])) {
			GPIO_INFO("Gpio%02d |",io_num);
			GPIO_INFO("Mode:");
            if (GPIO_Mode_Input==(pGPIOConfig->GPIO_Mode)) {
			   GPIO_INFO("%s ","Input ");
               gpio_input_enable(io_num);
			   gpio_output_disable(io_num);
            }
            else if (GPIO_Mode_Out_OD==(pGPIOConfig->GPIO_Mode)) {
                //portENTER_CRITICAL();              
                GPIO_INFO("%s ","Out_OD ");
                GPIO_SET_PIN_PAD_DRIVER(io_num,0x01);//0X01  Open drain
                //portEXIT_CRITICAL();
            }
			else if(GPIO_Mode_Output==(pGPIOConfig->GPIO_Mode)){
			   GPIO_SET_PIN_PAD_DRIVER(io_num,0x0);//0x00 Normal gpio output
               GPIO_INFO("%s ","OutPut ");
			}
			else if(GPIO_Mode_Input_OutOD==(pGPIOConfig->GPIO_Mode)){
               GPIO_INFO("%s ","Input_OutOD ");
               gpio_input_enable(io_num);
			   GPIO_SET_PIN_PAD_DRIVER(io_num,0x01);//0X01  Open drain
			}
			else if(GPIO_Mode_Input_OutPut==(pGPIOConfig->GPIO_Mode))
			{
			     GPIO_INFO("%s ","Input_OutPut ");
                 gpio_input_enable(io_num);
				 GPIO_SET_PIN_PAD_DRIVER(io_num,0x0);//0x00 Normal gpio output
			}
            GPIO_INFO("|");
		   	
            //for ESP32 function, 2 of every pad is allways GPIO func
            PIN_FUNC_SELECT(io_reg, 2);

            if (pGPIOConfig->GPIO_Pullup) {
                PIN_PULLUP_EN(io_reg);
            } else {
                PIN_PULLUP_DIS(io_reg);
            }

            if (pGPIOConfig->GPIO_Pulldown) {
                PIN_PULLDWN_EN(io_reg);
            } else {
                PIN_PULLDWN_DIS(io_reg);
            }
            GPIO_INFO("Intr:%d |\n",pGPIOConfig->GPIO_IntrType);
            gpio_set_intr_types(io_num, pGPIOConfig->GPIO_IntrType);
			pGPIOConfig->GPIO_IntrType?gpio_enable_intrs(io_num):gpio_disable_intrs(io_num);
        }
      else if(bit_valid&&(io_reg==0)){
          GPIO_WARING("io_num=%d not exits\n",io_num);
	  }
        io_num++;
    } while (io_num < GPIO_PIN_COUNT);
}
void gpio_intr_handler_register(void *fn, void *arg)
{
	 intr_matrix_set(PRO_CPU_NUM,ETS_GPIO_SOURCE, ETS_GPIO_INUM);
	 //ets_isr_attach(ETS_GPIO_INUM, fn, arg);
	 xt_set_interrupt_handler(ETS_GPIO_INUM, fn, arg);

}

/*
  only highlevel and lowlevel intr can use for wakeup
*/
void gpio_pin_wakeup_enable(enum_gpio_num_t gpio_num, enum_gpio_int_type_t intr_state)
{

    uint32_t pin_reg;
 
    if ((intr_state == GPIO_PIN_INTR_LOW_LEVEL) || (intr_state == GPIO_PIN_INTR_HIGH_LEVEL)) {
        //portENTER_CRITICAL();
        GPIO_SET_PIN_INT_TYPE(GPIO_REG(gpio_num),intr_state);
		GPIO_SET_PIN_WAKEUP_ENABLE(GPIO_REG(gpio_num),0x01);
        //portEXIT_CRITICAL();
    }
	else{
       GPIO_ERROR("GPIO Wakeup only support Level mode,but you set edge mode gpio_num:%u\n",gpio_num);
	}

}

void gpio_pin_wakeup_disable(enum_gpio_num_t gpio_num)
{

    GPIO_SET_PIN_WAKEUP_ENABLE(GPIO_REG(gpio_num),0x00);

}



