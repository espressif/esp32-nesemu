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

#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <driver/adc.h>
#include <driver/gpio.h>
#include <driver/i2c.h>
#include <esp_adc_cal.h>
#include <esp_log.h>
#include <esp_system.h>
#include <soc/adc_channel.h>
#include <soc/gpio_struct.h>

#include "sdkconfig.h"

#if defined(CONFIG_HW_CONTROLLER_GPIO)

// Set buttons with pins
#define UP CONFIG_HW_GPIO_UP
#define DOWN CONFIG_HW_GPIO_DOWN
#define LEFT CONFIG_HW_GPIO_LEFT
#define RIGHT CONFIG_HW_GPIO_RIGHT
#define MENU CONFIG_HW_GPIO_MENU
#define AUDIO CONFIG_HW_GPIO_AUDIO
#define SELECT CONFIG_HW_GPIO_SELECT
#define START CONFIG_HW_GPIO_START
#define A CONFIG_HW_GPIO_A
#define B CONFIG_HW_GPIO_B

#if (LEFT == RIGHT) && (UP == DOWN)
#define ANALOG_JOYSTICK

#if (LEFT == ADC1_CHANNEL_0_GPIO_NUM)
#define X_CHANNEL ADC1_CHANNEL_0
#endif
#if (LEFT == ADC1_CHANNEL_1_GPIO_NUM)
#define X_CHANNEL ADC1_CHANNEL_1
#endif
#if (LEFT == ADC1_CHANNEL_2_GPIO_NUM)
#define X_CHANNEL ADC1_CHANNEL_2
#endif
#if (LEFT == ADC1_CHANNEL_3_GPIO_NUM)
#define X_CHANNEL ADC1_CHANNEL_3
#endif
#if (LEFT == ADC1_CHANNEL_4_GPIO_NUM)
#define X_CHANNEL ADC1_CHANNEL_4
#endif
#if (LEFT == ADC1_CHANNEL_5_GPIO_NUM)
#define X_CHANNEL ADC1_CHANNEL_5
#endif
#if (LEFT == ADC1_CHANNEL_6_GPIO_NUM)
#define X_CHANNEL ADC1_CHANNEL_6
#endif
#if (LEFT == ADC1_CHANNEL_7_GPIO_NUM)
#define X_CHANNEL ADC1_CHANNEL_7
#endif

#if (UP == ADC1_CHANNEL_0_GPIO_NUM)
#define Y_CHANNEL ADC1_CHANNEL_0
#endif
#if (UP == ADC1_CHANNEL_1_GPIO_NUM)
#define Y_CHANNEL ADC1_CHANNEL_1
#endif
#if (UP == ADC1_CHANNEL_2_GPIO_NUM)
#define Y_CHANNEL ADC1_CHANNEL_2
#endif
#if (UP == ADC1_CHANNEL_3_GPIO_NUM)
#define Y_CHANNEL ADC1_CHANNEL_3
#endif
#if (UP == ADC1_CHANNEL_4_GPIO_NUM)
#define Y_CHANNEL ADC1_CHANNEL_4
#endif
#if (UP == ADC1_CHANNEL_5_GPIO_NUM)
#define Y_CHANNEL ADC1_CHANNEL_5
#endif
#if (UP == ADC1_CHANNEL_6_GPIO_NUM)
#define Y_CHANNEL ADC1_CHANNEL_6
#endif
#if (UP == ADC1_CHANNEL_7_GPIO_NUM)
#define Y_CHANNEL ADC1_CHANNEL_7
#endif

#define DEFAULT_VREF 1100
static esp_adc_cal_characteristics_t characteristics;
#endif

void controller_init()
{
  //Configure button
  gpio_config_t btn_config;
  btn_config.intr_type = GPIO_INTR_ANYEDGE; //Enable interrupt on both rising and falling edges
  btn_config.mode = GPIO_MODE_INPUT;        //Set as Input
#ifdef ANALOG_JOYSTICK
  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_11db);
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_11db, ADC_WIDTH_BIT_12, DEFAULT_VREF, &characteristics);

  btn_config.pin_bit_mask = (uint64_t) //Bitmask
      (((uint64_t)(((uint64_t)1) << MENU)) | ((uint64_t)(((uint64_t)1) << AUDIO)) | ((uint64_t)(((uint64_t)1) << SELECT)) | ((uint64_t)(((uint64_t)1) << START)) | ((uint64_t)(((uint64_t)1) << A)) | ((uint64_t)(((uint64_t)1) << B)));
  btn_config.pull_up_en = GPIO_PULLUP_ENABLE;      //Disable pullup
  btn_config.pull_down_en = GPIO_PULLDOWN_DISABLE; //Enable pulldown
  gpio_config(&btn_config);
#else
  btn_config.pin_bit_mask = (uint64_t) //Bitmask
      (((uint64_t)(((uint64_t)1) << UP)) | ((uint64_t)(((uint64_t)1) << DOWN)) | ((uint64_t)(((uint64_t)1) << LEFT)) | ((uint64_t)(((uint64_t)1) << RIGHT)) | ((uint64_t)(((uint64_t)1) << MENU)) | ((uint64_t)(((uint64_t)1) << AUDIO)) | ((uint64_t)(((uint64_t)1) << SELECT)) | ((uint64_t)(((uint64_t)1) << START)) | ((uint64_t)(((uint64_t)1) << A)) | ((uint64_t)(((uint64_t)1) << B)));
  btn_config.pull_up_en = GPIO_PULLUP_ENABLE;      //Disable pullup
  btn_config.pull_down_en = GPIO_PULLDOWN_DISABLE; //Enable pulldown
  gpio_config(&btn_config);
#endif
}

int controller_read_input()
{
  int u, d, l, r, s, t, a, b, m, i;
#ifdef ANALOG_JOYSTICK
  int joyX = adc1_get_raw(X_CHANNEL);
  int joyY = adc1_get_raw(Y_CHANNEL);
  if (joyX > 2048 + 1024)
  {
    l = 0;
    r = 1;
  }
  else if (joyX > 1024)
  {
    l = 1;
    r = 0;
  }
  else
  {
    l = 1;
    r = 1;
  }
  if (joyY > 2048 + 1024)
  {
    u = 0;
    d = 1;
  }
  else if (joyY > 1024)
  {
    u = 1;
    d = 0;
  }
  else
  {
    u = 1;
    d = 1;
  }
#else
  l = gpio_get_level(LEFT);
  r = gpio_get_level(RIGHT);
  u = gpio_get_level(UP);
  d = gpio_get_level(DOWN);
#endif

  s = gpio_get_level(SELECT);
  t = gpio_get_level(START);
  a = gpio_get_level(A);
  b = gpio_get_level(B);
  m = gpio_get_level(MENU);
  i = gpio_get_level(AUDIO);

  return 0xFFFF ^ ((!u << 4) | (!d << 6) | (!l << 7) | (!r << 5) | (!s << 0) | (!t << 3) | (!a << 13) | (!b << 14) | (!m << 1) | (!i << 2));
}

#elif defined(CONFIG_HW_CONTROLLER_PSX)

#define PSX_CLK CONFIG_HW_PSX_CLK
#define PSX_DAT CONFIG_HW_PSX_DAT
#define PSX_ATT CONFIG_HW_PSX_ATT
#define PSX_CMD CONFIG_HW_PSX_CMD

#define DELAY() asm("nop; nop; nop; nop;nop; nop; nop; nop;nop; nop; nop; nop;nop; nop; nop; nop;")

/* Sends and receives a byte from/to the PSX controller using SPI */
static int psxSendRecv(int send)
{
  int x;
  int ret = 0;
  volatile int delay;

#if 0
	while(1) {
		GPIO.out_w1ts=(1<<PSX_CMD);
		GPIO.out_w1ts=(1<<PSX_CLK);
		GPIO.out_w1tc=(1<<PSX_CMD);
		GPIO.out_w1tc=(1<<PSX_CLK);
	}
#endif

  GPIO.out_w1tc = (1 << PSX_ATT);
  for (delay = 0; delay < 100; delay++)
    ;
  for (x = 0; x < 8; x++)
  {
    if (send & 1)
    {
      GPIO.out_w1ts = (1 << PSX_CMD);
    }
    else
    {
      GPIO.out_w1tc = (1 << PSX_CMD);
    }
    DELAY();
    for (delay = 0; delay < 100; delay++)
      ;
    GPIO.out_w1tc = (1 << PSX_CLK);
    for (delay = 0; delay < 100; delay++)
      ;
    GPIO.out_w1ts = (1 << PSX_CLK);
    ret >>= 1;
    send >>= 1;
    if (GPIO.in & (1 << PSX_DAT))
      ret |= 128;
  }
  return ret;
}

static void psxDone()
{
  DELAY();
  GPIO_REG_WRITE(GPIO_OUT_W1TS_REG, (1 << PSX_ATT));
}

void controller_init()
{
  volatile int delay;
  int t;
  gpio_config_t gpioconf[2] = {
      {.pin_bit_mask = (1 << PSX_CLK) | (1 << PSX_CMD) | (1 << PSX_ATT),
       .mode = GPIO_MODE_OUTPUT,
       .pull_up_en = GPIO_PULLUP_DISABLE,
       .pull_down_en = GPIO_PULLDOWN_DISABLE,
       .intr_type = GPIO_PIN_INTR_DISABLE},
      {.pin_bit_mask = (1 << PSX_DAT),
       .mode = GPIO_MODE_INPUT,
       .pull_up_en = GPIO_PULLUP_ENABLE,
       .pull_down_en = GPIO_PULLDOWN_DISABLE,
       .intr_type = GPIO_PIN_INTR_DISABLE}};
  gpio_config(&gpioconf[0]);
  gpio_config(&gpioconf[1]);

  //Send a few dummy bytes to clean the pipes.
  psxSendRecv(0);
  psxDone();
  for (delay = 0; delay < 500; delay++)
    DELAY();
  psxSendRecv(0);
  psxDone();
  for (delay = 0; delay < 500; delay++)
    DELAY();
  //Try and detect the type of controller, so we can give the user some diagnostics.
  psxSendRecv(0x01);
  t = psxSendRecv(0x00);
  psxDone();
  if (t == 0 || t == 0xff)
  {
    nofrendo_log_printf("No PSX/PS2 controller detected (0x%X). You will not be able to control the game.\n", t);
  }
  else
  {
    nofrendo_log_printf("PSX controller type 0x%X\n", t);
  }
}

int controller_read_input()
{
  int b1, b2;

  psxSendRecv(0x01);      //wake up
  psxSendRecv(0x42);      //get data
  psxSendRecv(0xff);      //should return 0x5a
  b1 = psxSendRecv(0xff); //buttons byte 1
  b2 = psxSendRecv(0xff); //buttons byte 2
  psxDone();
  return (b2 << 8) | b1;
}

#elif defined(CONFIG_HW_CONTROLLER_I2C_GP)

#define I2C_GAMEPAD_SCL_IO 22        /*!< gpio number for I2C master clock */
#define I2C_GAMEPAD_SDA_IO 21        /*!< gpio number for I2C master data  */
#define I2C_GAMEPAD_NUM I2C_NUM_0    /*!< I2C port number for master dev */
#define I2C_GAMEPAD_TX_BUF_DISABLE 0 /*!< I2C master do not need buffer */
#define I2C_GAMEPAD_RX_BUF_DISABLE 0 /*!< I2C master do not need buffer */
#define I2C_GAMEPAD_FREQ_HZ 50000    /*!< I2C master clock frequency */
#define I2C_GAMEPAD_ADDR 0x08

#define READ_BIT I2C_MASTER_READ /*!< I2C master read */
#define ACK_CHECK_EN 0x1         /*!< I2C master will check ack from slave*/
#define NACK_VAL 0x1             /*!< I2C nack value */

void controller_init()
{
  int i2c_master_port = I2C_GAMEPAD_NUM;
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = I2C_GAMEPAD_SDA_IO;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_io_num = I2C_GAMEPAD_SCL_IO;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = I2C_GAMEPAD_FREQ_HZ;
  i2c_param_config(i2c_master_port, &conf);
  i2c_driver_install(i2c_master_port, conf.mode,
                     I2C_GAMEPAD_RX_BUF_DISABLE,
                     I2C_GAMEPAD_TX_BUF_DISABLE, 0);
}

/**
 * @brief test code to write esp-i2c-slave
 *
 * 1. set mode
 * _________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write 1 byte + ack  | stop |
 * --------|---------------------------|---------------------|------|
 * 2. wait more than 24 ms
 * 3. read data
 * ______________________________________________________________________________________
 * | start | slave_addr + rd_bit + ack | read 1 byte + ack  | read 1 byte + nack | stop |
 * --------|---------------------------|--------------------|--------------------|------|
 */
int controller_read_input()
{
  uint8_t data;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (I2C_GAMEPAD_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
  i2c_master_read_byte(cmd, &data, NACK_VAL);
  i2c_master_stop(cmd);
  esp_err_t ret = i2c_master_cmd_begin(I2C_GAMEPAD_NUM, cmd, 1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd);
  if (ret == 0)
  {
    nofrendo_log_printf("I2C read %d, return %d\n", data, ret);

    return 0xFFFF ^ ((((0x01 & data) > 0) << 4) | (((0x02 & data) > 0) << 6) | (((0x04 & data) > 0) << 7) | (((0x08 & data) > 0) << 5) | (((0x10 & data) > 0) << 0) | (((0x20 & data) > 0) << 3) | (((0x40 & data) > 0) << 13) | (((0x80 & data) > 0) << 14));
  }
  else
  {
    return 0xFFFF;
  }
}

#elif defined(CONFIG_HW_CONTROLLER_I2C_M5KB)

#define I2C_KEYBOARD_SCL_IO 22        /*!< gpio number for I2C master clock */
#define I2C_KEYBOARD_SDA_IO 21        /*!< gpio number for I2C master data  */
#define I2C_KEYBOARD_NUM I2C_NUM_0    /*!< I2C port number for master dev */
#define I2C_KEYBOARD_TX_BUF_DISABLE 0 /*!< I2C master do not need buffer */
#define I2C_KEYBOARD_RX_BUF_DISABLE 0 /*!< I2C master do not need buffer */
#define I2C_KEYBOARD_FREQ_HZ 100000   /*!< I2C master clock frequency */
#define I2C_KEYBOARD_ADDR 0x5f

#define READ_BIT I2C_MASTER_READ /*!< I2C master read */
#define ACK_CHECK_EN 0x1         /*!< I2C master will check ack from slave*/
#define NACK_VAL 0x1             /*!< I2C nack value */

void controller_init()
{
  int i2c_master_port = I2C_KEYBOARD_NUM;
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = I2C_KEYBOARD_SDA_IO;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_io_num = I2C_KEYBOARD_SCL_IO;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = I2C_KEYBOARD_FREQ_HZ;
  i2c_param_config(i2c_master_port, &conf);
  i2c_driver_install(i2c_master_port, conf.mode,
                     I2C_KEYBOARD_RX_BUF_DISABLE,
                     I2C_KEYBOARD_TX_BUF_DISABLE, 0);
}

/**
 * @brief test code to write esp-i2c-slave
 *
 * 1. set mode
 * _________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write 1 byte + ack  | stop |
 * --------|---------------------------|---------------------|------|
 * 2. wait more than 24 ms
 * 3. read data
 * ______________________________________________________________________________________
 * | start | slave_addr + rd_bit + ack | read 1 byte + ack  | read 1 byte + nack | stop |
 * --------|---------------------------|--------------------|--------------------|------|
 */
int controller_read_input()
{
  uint8_t data;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (I2C_KEYBOARD_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
  i2c_master_read_byte(cmd, &data, NACK_VAL);
  i2c_master_stop(cmd);
  esp_err_t ret = i2c_master_cmd_begin(I2C_KEYBOARD_NUM, cmd, 1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd);
  if ((ret == 0) && (data > 0))
  {
    nofrendo_log_printf("I2C read %d, return %d\n", data, ret);

    switch (data)
    {
    case 181: // up
      return 0xFFFF ^ (1 << 4);
      break;
    case 182: // down
      return 0xFFFF ^ (1 << 6);
      break;
    case 180: // left
      return 0xFFFF ^ (1 << 7);
      break;
    case 183: // right
      return 0xFFFF ^ (1 << 5);
      break;
    case 32: // space -> select
      return 0xFFFF ^ (1 << 0);
      break;
    case 13: // enter -> start
      return 0xFFFF ^ (1 << 3);
      break;
    case 108: // L -> A
      return 0xFFFF ^ (1 << 13);
      break;
    case 107: // K -> B
      return 0xFFFF ^ (1 << 14);
      break;
    }
  }

  return 0xFFFF;
}

#else

void controller_gpio_init()
{
  nofrendo_log_printf("GPIO controller disabled in menuconfig; no input enabled.\n");
}

int controller_gpio_read_input()
{
  return 0xFFFF;
}

#endif