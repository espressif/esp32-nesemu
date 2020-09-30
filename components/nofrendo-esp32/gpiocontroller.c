#include <stdio.h>

#include <driver/adc.h>
#include <driver/gpio.h>
#include <esp_adc_cal.h>
#include <esp_log.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <soc/adc_channel.h>

#include <sdkconfig.h>

#ifdef CONFIG_HW_CONTROLLER_GPIO

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

int gpioReadInput()
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

void gpiocontrollerInit()
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

#else

void gpiocontrollerInit()
{
  printf("GPIO controller disabled in menuconfig; no input enabled.\n");
}

int gpioReadInput()
{
  return 0xFFFF;
}

#endif