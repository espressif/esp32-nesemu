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

#ifndef _DRIVER_SIGMADELTA_H_
#define _DRIVER_SIGMADELTA_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
SIGMADELTA_CHANNEL0=0,
SIGMADELTA_CHANNEL1,
SIGMADELTA_CHANNEL2,
SIGMADELTA_CHANNEL3,
SIGMADELTA_CHANNEL4,
SIGMADELTA_CHANNEL5,
SIGMADELTA_CHANNEL6,
SIGMADELTA_CHANNEL7,
}enum_sigmadelta_chanl_t;

/*---------------------------------------------------------------
                      sigmadelta  api
----------------------------------------------------------------*/
void sigmadelta_bind_gpio_init(enum_sigmadelta_chanl_t sigmadelta_chal ,uint8_t gpio_num);
void sigmadelta_set_duty(enum_sigmadelta_chanl_t sigmadelta_chal,int8_t duty);
void sigmadelta_set_prescale(enum_sigmadelta_chanl_t sigmadelta_chal,uint8_t  prescale);
void sigmadelta_start(enum_sigmadelta_chanl_t sigmadelta_chal);
void sigmadelta_stop(enum_sigmadelta_chanl_t sigmadelta_chal);
/*----------------------------------------------------------------
                      sigmadelta dbg api
----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* _DRIVER_SIGMADELTA_H_ */
