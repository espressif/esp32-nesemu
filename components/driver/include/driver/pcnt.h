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

#ifndef _DRIVER_PCNT_H_
#define _DRIVER_PCNT_H_

#include <stdbool.h>
#include "soc/pcnt_reg.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PulseCntChannel0=0,
    PulseCntChannel1=1,
    PulseCntChannel2=2,
    PulseCntChannel3=3,
    PulseCntChannel4=4,
    PulseCntChannel5=5,
    PulseCntChannel6=6,
    PulseCntChannel7=7,
}enum_pcnt_channel_t;
//Control Mode
enum pcnt_ctrl_high_level{
    HIGH_LEVEL_INCREASE=0,
    HIGH_LEVEL_DECREASE=1,
    HIGH_LEVEL_FORBIDDEN=2,
};
enum pcnt_ctrl_low_level{
    LOW_LEVEL_INCREASE=0,
    LOW_LEVEL_DECREASE=1,
    LOW_LEVEL_FORBIIDEN=2,
};
//Sig Mode
enum pcnt_sig_positive_edge{
    POSITIVE_EDGE_INCREASE=1,
    POSITIVE_EDGE_DECREASE=2,
    POSITIVE_EDGE_FORBIDDEN=0,
};
enum pcnt_sig_negative_edge{
    NEGATIVE_EDGE_INCREASE=1,
    NEGATIVE_EDGE_DECREASE=2,
    NEGATIVE_EDGE_FORBIIDEN=0,
};

//Gate Config
typedef struct {
    uint8_t ctrl_pin_num;
    uint8_t sig_pin_num;
    enum pcnt_ctrl_high_level ctrl_high_level;
    enum pcnt_ctrl_low_level ctrl_low_level;
    enum pcnt_sig_positive_edge sig_positive_edge;
    enum pcnt_sig_negative_edge sig_negative_edge;
}pcnt_gate_t;
//Channel Config
typedef struct {
    enum_pcnt_channel_t channel;
    int16_t L_limit;
    int16_t H_limit;
    uint16_t Filter_tick;
    pcnt_gate_t * Gate0;
    pcnt_gate_t * Gate1;
}Pcnt_channel_config;

enum pcnt_intr_type{
    COUNTER_EQUAL_TO_THRES1=BIT(2),
    COUNTER_EQUAL_TO_THRES0=BIT(3),
    COUNTER_LESS_THAN_LIMIL=BIT(4),
    COUNTER_MORE_THAN_LMITH=BIT(5),
    COUNTER_ABOUT_ZERO=BIT(6),

};
enum pcnt_count_zero_intr_type{
    COUNTER_INCREASE_TO_ZERO=0,
    COUNTER_DECREASE_TO_ZERO=1,
    COUNTER_NEGETIVE=2,
    COUNTER_ACTIVE=3,
};
enum pcnt_intr_mask{
    ENABLE_THRES1_INTR=PCNT_THR_THRES1_EN_U0,
    ENABLE_THRES0_INTR=PCNT_THR_THRES0_EN_U0,
    ENABLE_L_LIMIT_INTR=PCNT_THR_L_LIM_EN_U0,
    ENABLE_H_LIMIT_INTR=PCNT_THR_H_LIM_EN_U0,
    ENABLE_THR_ZERO_INTR=PCNT_THR_ZERO_EN_U0,
};
typedef struct{
    enum_pcnt_channel_t channel;
    int16_t thres1;
    int16_t thres0;
    uint32_t PcntIntrEnMask;
}Pcnt_intr_config;


typedef void (*xt_handler)(void *);

//if user not use pin param ,please give this define
#define PCNT_INVALID_PIN_PARAM (40)




//register address redefine,just only use simple
#define PCNT_UX_CONF0(Channel) (PCNT_U0_CONF0_REG+(Channel)*12)
#define PCNT_UX_CONF1(Channel) (PCNT_U0_CONF1_REG+(Channel)*12)
#define PCNT_UX_CONF2(Channel) (PCNT_U0_CONF2_REG+(Channel)*12)
#define PCNT_CNT_CURRENT_VALUE(Channel) ( PCNT_U0_CNT_REG+((Channel)*4))
#define PCNT_UX_STATUS(Channel) (PCNT_U0_STATUS_REG+(Channel)*4)



/*-------------------------------------------------------------
                  PCNT COUNTER COMMON CONFIG API
-------------------------------------------------------------*/
bool pcnt_reset_counter(enum_pcnt_channel_t channel);
void pcnt_start_counter(enum_pcnt_channel_t channel);
int16_t pcnt_get_counter_value(enum_pcnt_channel_t channel);
/*-------------------------------------------------------------
                       PCNT INTR API
-------------------------------------------------------------*/
void pcnt_enable_channel_intr(enum_pcnt_channel_t channel);
void pcnt_disable_channel_intr(enum_pcnt_channel_t channel);
void pcnt_clear_channel_intr(enum_pcnt_channel_t channel);
uint8_t pcnt_get_channel_intr(void);
void pcnt_intr_register(void (* fn)(void* ),void * arg);
void pcnt_intr_handle_demo(void* arg);
void pcnt_set_intr(Pcnt_intr_config* intr_config);
/*-------------------------------------------------------------
                       PCNT INIT API
-------------------------------------------------------------*/
void pcnt_init_channel(Pcnt_channel_config *channel_config);
/*-------------------------------------------------------------
                       PCNT DBG API
-------------------------------------------------------------*/
void pcnt_check_pin_level(uint8_t ctrl_pin_num,uint8_t sig_pin_num);
void pcnt_check_channel_reg(enum_pcnt_channel_t channel);

#ifdef __cplusplus
}
#endif

#endif /* _DRIVER_PCNT_H_ */
