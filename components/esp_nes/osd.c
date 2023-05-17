/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

/*******************************************************************************
* Includes
*******************************************************************************/

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <string.h>

#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_partition.h"
#include "spi_flash_mmap.h"

static const char *TAG = "OSD";

#define LOBYTE(u16)     ((uint8_t)(((uint16_t)(u16)) & 0xff))
#define HIBYTE(u16)     ((uint8_t)((((uint16_t)(u16))>>8) & 0xff))

//Nes stuff wants to define this as well...
#undef false
#undef true
#undef bool

#include "esp_log.h"
#include "noftypes.h"
#include "nofconfig.h"
#include "nofrendo.h"
#include "bitmap.h"
#include "osd.h"
#include "event.h"
#include "nesinput.h"
#include "bsp/esp-bsp.h"
#include "usb/hid_host.h"
#include "usb/hid_usage_keyboard.h"
#include "usb/hid_usage_mouse.h"

#define APP_DEFAULT_WIDTH  256
#define APP_DEFAULT_HEIGHT 240
#define APP_DEFAULT_SCALE  (BSP_LCD_V_RES/APP_DEFAULT_HEIGHT)

// LVGL image declare
LV_IMG_DECLARE(img_supermario)
LV_IMG_DECLARE(img_donkeykong)
LV_IMG_DECLARE(img_qbert)

typedef struct {
    hid_host_device_handle_t hid_device_handle;
    hid_host_driver_event_t event;
    void *arg;
} app_usb_hid_event_t;

typedef struct
{    
    bool    btn_select:1;
    bool    btn_start:1;
    bool    btn_up:1;
    bool    btn_down:1;
    bool    btn_right:1;
    bool    btn_left:1;

    bool    btn_a:1;
    bool    btn_b:1;
} gamepad_t;

/*******************************************************************************
* Function definitions
*******************************************************************************/
/* Video functions */
static int app_osd_video_init(int width, int height);
static void app_osd_video_shutdown(void);
static int app_osd_video_set_mode(int width, int height);
static void app_osd_video_set_palette(rgb_t *palette);
static void app_osd_video_clear(uint8 color);
static bitmap_t *app_osd_video_lock_write(void);
static void app_osd_video_free_write(int num_dirties, rect_t *dirty_rects);
static void app_osd_video_custom_blit(bitmap_t *primary, int num_dirties, rect_t *dirty_rects);
/* Audio functions */

/* USB functions */
static void app_hid_init(void);

/* Game selection */
static void app_lvgl_game_selection(void);
/*******************************************************************************
* Local variables
*******************************************************************************/

static viddriver_t vid_driver = {
   .name = "Vid Driver",
   .init = app_osd_video_init,
   .shutdown = app_osd_video_shutdown,
   .set_mode = app_osd_video_set_mode,
   .set_palette = app_osd_video_set_palette,
   .clear = app_osd_video_clear,
   .lock_write = app_osd_video_lock_write,
   .free_write = app_osd_video_free_write,
   .custom_blit = app_osd_video_custom_blit,
   .invalidate = false,
};

static TimerHandle_t timer; 

/* Video variables */
static uint16 colorPalette[256];
static bitmap_t *vidBitmap = NULL;
static lv_obj_t * lvgl_video_canvas = NULL;
static uint8_t * vid_buffer = NULL;

/* USB variables */
static QueueHandle_t hid_queue = NULL;
static gamepad_t app_gamepad;
static gamepad_t app_gamepad_tmp;

/* GAME */
static const char * APP_GAME_SUPERMARIO = "nesrom1";
static const char * APP_GAME_DONKEYKONG = "nesrom2";
static const char * APP_GAME_QBERT = "nesrom3";
/* Selected partion with game */
static const char * app_selected_game = NULL;
static lv_obj_t * game_selection = NULL;
/*******************************************************************************
* Public API functions
*******************************************************************************/

char *osd_getromdata() 
{
	/* Wait for select the game */
	while(app_selected_game == NULL)
	{
		vTaskDelay(pdMS_TO_TICKS(100));
	};

	char* romdata;
	const esp_partition_t* part;
	spi_flash_mmap_handle_t hrom;
	esp_err_t err;
	nvs_flash_init();
	part=esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, app_selected_game);
	if (part==0) printf("Couldn't find rom part!\n");
	err=esp_partition_mmap(part, 0, 1024*1024, SPI_FLASH_MMAP_DATA, (const void**)&romdata, &hrom);
	if (err!=ESP_OK) printf("Couldn't map rom part!\n");
	printf("Initialized. ROM@%p\n", romdata);
   return (char*)romdata;
}

void osd_getvideoinfo(vidinfo_t *info)
{
   info->default_width = APP_DEFAULT_WIDTH;
   info->default_height = APP_DEFAULT_HEIGHT;
   info->driver = &vid_driver;
}

void osd_getsoundinfo(sndinfo_t *info)
{
}

void osd_setsound(void (*playfunc)(void *buffer, int size))
{
}

int osd_init(void)
{
   uint32_t buf_size = APP_DEFAULT_SCALE * APP_DEFAULT_WIDTH * APP_DEFAULT_SCALE * APP_DEFAULT_HEIGHT * sizeof(uint16_t);
   vid_buffer = heap_caps_malloc(buf_size, MALLOC_CAP_DEFAULT);
   assert(vid_buffer);
   memset(vid_buffer, 0x00, buf_size);

   app_hid_init();

   bsp_display_lock(0);
   lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
   lvgl_video_canvas = lv_canvas_create(lv_scr_act());
   lv_canvas_set_buffer(lvgl_video_canvas, vid_buffer, APP_DEFAULT_SCALE*APP_DEFAULT_WIDTH, APP_DEFAULT_SCALE*APP_DEFAULT_HEIGHT, LV_IMG_CF_TRUE_COLOR);
   lv_obj_center(lvgl_video_canvas);
   bsp_display_unlock();
    
	/* Show selection of the game */
	app_lvgl_game_selection();

	return 0;
}

void osd_shutdown(void)
{
}

/* This is os-specific part of main() */
int osd_main(int argc, char *argv[])
{
   config.filename = "config.cfg";
   return main_loop("rom", system_autodetect);
}

int osd_installtimer(int frequency, void *func, int funcsize, void *counter, int countersize)
{
	printf("Timer install, freq=%d\n", frequency);
	timer=xTimerCreate("nes",configTICK_RATE_HZ/frequency, pdTRUE, NULL, func);
	xTimerStart(timer, 0);
   return 0;
}

/* File system interface */
void osd_fullname(char *fullname, const char *shortname)
{
   strncpy(fullname, shortname, PATH_MAX);
}

/* This gives filenames for storage of saves */
char *osd_newextension(char *string, char *ext)
{
   return string;
}

void osd_getinput(void)
{
	event_t evh = NULL;
   int pressed = 0;

   if(app_gamepad_tmp.btn_select != app_gamepad.btn_select)
   {
      pressed = (app_gamepad.btn_select ? INP_STATE_MAKE : INP_STATE_BREAK);
      evh = event_get(event_joypad1_select);
      app_gamepad_tmp.btn_select = app_gamepad.btn_select;

      if(evh) {
         evh(pressed);
      }
   }
   
   if(app_gamepad_tmp.btn_start != app_gamepad.btn_start)
   {
      pressed = (app_gamepad.btn_start ? INP_STATE_MAKE : INP_STATE_BREAK);
      evh = event_get(event_joypad1_start);
      app_gamepad_tmp.btn_start = app_gamepad.btn_start;

      if(evh) {
         evh(pressed);
      }
   }
   
   if(app_gamepad_tmp.btn_up != app_gamepad.btn_up)
   {
      pressed = (app_gamepad.btn_up ? INP_STATE_MAKE : INP_STATE_BREAK);
      evh = event_get(event_joypad1_up);
      app_gamepad_tmp.btn_up = app_gamepad.btn_up;

      if(evh) {
         evh(pressed);
      }
   }
   
   if(app_gamepad_tmp.btn_down != app_gamepad.btn_down)
   {
      pressed = (app_gamepad.btn_down ? INP_STATE_MAKE : INP_STATE_BREAK);
      evh = event_get(event_joypad1_down);
      app_gamepad_tmp.btn_down = app_gamepad.btn_down;

      if(evh) {
         evh(pressed);
      }
   }
   
   if(app_gamepad_tmp.btn_right != app_gamepad.btn_right)
   {
      pressed = (app_gamepad.btn_right ? INP_STATE_MAKE : INP_STATE_BREAK);
      evh = event_get(event_joypad1_right);
      app_gamepad_tmp.btn_right = app_gamepad.btn_right;

      if(evh) {
         evh(pressed);
      }
   }
   
   if(app_gamepad_tmp.btn_left != app_gamepad.btn_left)
   {
      pressed = (app_gamepad.btn_left ? INP_STATE_MAKE : INP_STATE_BREAK);
      evh = event_get(event_joypad1_left);
      app_gamepad_tmp.btn_left = app_gamepad.btn_left;

      if(evh) {
         evh(pressed);
      }
   }
   
   if(app_gamepad_tmp.btn_a != app_gamepad.btn_a)
   {
      pressed = (app_gamepad.btn_a ? INP_STATE_MAKE : INP_STATE_BREAK);
      evh = event_get(event_joypad1_a);
      app_gamepad_tmp.btn_a = app_gamepad.btn_a;

      if(evh) {
         evh(pressed);
      }
   }
   
   if(app_gamepad_tmp.btn_b != app_gamepad.btn_b)
   {
      pressed = (app_gamepad.btn_b ? INP_STATE_MAKE : INP_STATE_BREAK);
      evh = event_get(event_joypad1_b);
      app_gamepad_tmp.btn_b = app_gamepad.btn_b;

      if(evh) {
         evh(pressed);
      }
   }
}

void osd_getmouse(int *x, int *y, int *button)
{
}

/* This gives filenames for storage of PCX snapshots */
int osd_makesnapname(char *filename, int len)
{
   return -1;
}

/*******************************************************************************
* Private API functions
*******************************************************************************/

static void app_lvgl_btn_supermario(lv_event_t *e)
{
   lv_obj_del(game_selection);
	app_selected_game = APP_GAME_SUPERMARIO;
}

static void app_lvgl_btn_donkeykong(lv_event_t *e)
{
   lv_obj_del(game_selection);
	app_selected_game = APP_GAME_DONKEYKONG;
}

static void app_lvgl_btn_qbert(lv_event_t *e)
{
   lv_obj_del(game_selection);
	app_selected_game = APP_GAME_QBERT;
}

static void app_lvgl_btn_reset(lv_event_t *e)
{
   esp_restart();
}

static void app_lvgl_game_selection(void)
{
   uint8_t scale = 480/BSP_LCD_V_RES;
   bsp_display_lock(0);
   lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);

	lv_obj_t * btn_rst = lv_btn_create(lv_scr_act());
   lv_obj_t * lbl = lv_label_create(btn_rst);
   lv_label_set_text_static(lbl, LV_SYMBOL_PREV"");
   lv_obj_add_event_cb(btn_rst, app_lvgl_btn_reset, LV_EVENT_CLICKED, NULL);

   game_selection = lv_obj_create(lv_scr_act());
   lv_obj_set_size(game_selection, BSP_LCD_H_RES, 2*(BSP_LCD_V_RES/3));
   lv_obj_set_flex_flow(game_selection, LV_FLEX_FLOW_COLUMN);
   lv_obj_set_style_bg_color(game_selection, lv_color_black(), 0);
   lv_obj_set_style_border_color(game_selection, lv_color_black(), 0);
   lv_obj_align(game_selection, LV_ALIGN_CENTER, 0, 0);
   lv_obj_set_style_pad_top(game_selection, 0, 0);
   lv_obj_set_style_pad_bottom(game_selection, 0, 0);
   lv_obj_set_style_pad_left(game_selection, 0, 0);
   lv_obj_set_style_pad_right(game_selection, 0, 0);
   lv_obj_set_flex_align(game_selection, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

   lv_obj_t * label = lv_label_create(game_selection);
   lv_obj_set_style_text_color(label, lv_color_white(), 0);
   lv_label_set_text_static(label, "SELECT THE GAME");

   lv_obj_t * games = lv_obj_create(game_selection);
   lv_obj_set_style_bg_color(games, lv_color_black(), 0);
   lv_obj_set_style_border_color(games, lv_color_black(), 0);
   lv_obj_set_size(games, BSP_LCD_H_RES-10, BSP_LCD_V_RES/2);
   lv_obj_align(games, LV_ALIGN_CENTER, 0, 0);
   lv_obj_set_flex_flow(games, LV_FLEX_FLOW_ROW);
   lv_obj_set_style_pad_top(games, 0, 0);
   lv_obj_set_style_pad_bottom(games, 0, 0);
   lv_obj_set_style_pad_left(games, 0, 0);
   lv_obj_set_style_pad_right(games, 0, 0);
   lv_obj_set_flex_align(games, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

	lv_obj_t * btn_1 = lv_btn_create(games);
   lv_obj_set_size(btn_1, img_supermario.header.w/scale+15, img_supermario.header.h/scale+15);
   lv_obj_t *img = lv_img_create(btn_1);
   lv_img_set_src(img, &img_supermario);
   lv_img_set_zoom(img, 256/scale);
   lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
   lv_obj_set_style_radius(btn_1, 0, 0);
   lv_obj_set_style_pad_top(btn_1, 15, 0);
   lv_obj_set_style_pad_bottom(btn_1, 15, 0);
   lv_obj_set_style_pad_left(btn_1, 15, 0);
   lv_obj_set_style_pad_right(btn_1, 15, 0);
   lv_obj_add_event_cb(btn_1, app_lvgl_btn_supermario, LV_EVENT_CLICKED, NULL);

	lv_obj_t * btn_2 = lv_btn_create(games);
   lv_obj_set_size(btn_2, img_donkeykong.header.w/scale+15, img_donkeykong.header.h/scale+15);
   img = lv_img_create(btn_2);
   lv_img_set_src(img, &img_donkeykong);
   lv_img_set_zoom(img, 256/scale);
   lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
   lv_obj_set_style_radius(btn_2, 0, 0);
   lv_obj_set_style_pad_top(btn_2, 15, 0);
   lv_obj_set_style_pad_bottom(btn_2, 15, 0);
   lv_obj_set_style_pad_left(btn_2, 15, 0);
   lv_obj_set_style_pad_right(btn_2, 15, 0);
   lv_obj_add_event_cb(btn_2, app_lvgl_btn_donkeykong, LV_EVENT_CLICKED, NULL);

	lv_obj_t * btn_3 = lv_btn_create(games);
   lv_obj_set_size(btn_3, img_qbert.header.w/scale+15, img_qbert.header.h/scale+15);
   img = lv_img_create(btn_3);
   lv_img_set_src(img, &img_qbert);
   lv_img_set_zoom(img, 256/scale);
   lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
   lv_obj_set_style_radius(btn_3, 0, 0);
   lv_obj_set_style_pad_top(btn_3, 15, 0);
   lv_obj_set_style_pad_bottom(btn_3, 15, 0);
   lv_obj_set_style_pad_left(btn_3, 15, 0);
   lv_obj_set_style_pad_right(btn_3, 15, 0);
   lv_obj_add_event_cb(btn_3, app_lvgl_btn_qbert, LV_EVENT_CLICKED, NULL);
	
#if 1
   /* Input device group */
    lv_indev_t *indev = bsp_display_get_input_dev();
    if (indev && indev->driver && indev->driver->type == LV_INDEV_TYPE_ENCODER) {
        lv_group_t *main_group = lv_group_create();
        lv_group_add_obj(main_group, btn_1);
        lv_group_add_obj(main_group, btn_2);
        lv_group_add_obj(main_group, btn_3);
        lv_indev_set_group(indev, main_group);
        ESP_LOGI(TAG, "Input device group was set.");
    }
#endif

   bsp_display_unlock();
}

static int app_osd_video_init(int width, int height)
{
	return 0;
}

static void app_osd_video_shutdown(void)
{

}

/* set a video mode */
static int app_osd_video_set_mode(int width, int height)
{
   return 0;
}

/* copy nes palette over to hardware */
static void app_osd_video_set_palette(rgb_t *palette)
{
	uint16 c;

   for (int i = 0; i < 256; i++)
   {
      c=(palette[i].b>>3)+((palette[i].g>>2)<<5)+((palette[i].r>>3)<<11);
      colorPalette[i]=c;
   }
}

/* clear all frames to a particular color */
static void app_osd_video_clear(uint8 color)
{
   bsp_display_lock(0);

   for(int x=0; x<APP_DEFAULT_WIDTH; x++)
   {
      for(int y=0; y<APP_DEFAULT_HEIGHT; y++)
      {
         vid_buffer[(x*APP_DEFAULT_HEIGHT*2) + (y*2)] = HIBYTE(colorPalette[color]);
         vid_buffer[(x*APP_DEFAULT_HEIGHT*2) + (y*2)+1] = LOBYTE(colorPalette[color]);
      }
   }

   lv_obj_invalidate(lvgl_video_canvas);
   bsp_display_unlock();
}

/* acquire the directbuffer for writing */
static bitmap_t *app_osd_video_lock_write(void)
{
   vidBitmap = bmp_create(APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT, APP_DEFAULT_WIDTH*sizeof(uint16));
   return vidBitmap;
}

static void app_osd_video_free_write(int num_dirties, rect_t *dirty_rects)
{
   bmp_destroy(&vidBitmap);
   vidBitmap = NULL;
}

static void app_osd_video_custom_blit(bitmap_t *primary, int num_dirties, rect_t *dirty_rects)
{	
   const uint8_t * data = primary->data;

   bsp_display_lock(0);

   for (int y=0; y<APP_DEFAULT_HEIGHT; y++) {
      for (int x=0; x<APP_DEFAULT_WIDTH; x++) {
         for (int z=0; z<APP_DEFAULT_SCALE; z++) {
            for (int pix=0; pix<APP_DEFAULT_SCALE; pix++) {
               vid_buffer[((x)*sizeof(uint16_t)*APP_DEFAULT_SCALE) + (((APP_DEFAULT_SCALE*y)+z)*APP_DEFAULT_SCALE*sizeof(uint16_t)*APP_DEFAULT_WIDTH) + (pix*2)] = HIBYTE(colorPalette[data[0]]);
               vid_buffer[((x)*sizeof(uint16_t)*APP_DEFAULT_SCALE) + (((APP_DEFAULT_SCALE*y)+z)*APP_DEFAULT_SCALE*sizeof(uint16_t)*APP_DEFAULT_WIDTH) + (pix*2 + 1)] = LOBYTE(colorPalette[data[0]]);
            }
          }
         data++;
      }
   }
   
   lv_obj_invalidate(lvgl_video_canvas);
   bsp_display_unlock();
}

static void app_usb_hid_host_interface_callback(hid_host_device_handle_t hid_device_handle, const hid_host_interface_event_t event, void *arg)
{
   hid_host_dev_params_t *dev = hid_host_device_get_params(hid_device_handle);
   uint8_t *data = NULL;
   unsigned int data_length = 0;

   assert(dev != NULL);

   switch (event) {
   case HID_HOST_INTERFACE_EVENT_INPUT_REPORT:
      data = hid_host_device_get_data(hid_device_handle, &data_length);
      if (dev->proto == HID_PROTOCOL_NONE) {
         if (data_length < 8) {
               break;
         }

         app_gamepad.btn_up = (data[1] == 0x00 ? true : false);
         app_gamepad.btn_down = (data[1] == 0xFF ? true : false);
         app_gamepad.btn_left = (data[0] == 0x00 ? true : false);
         app_gamepad.btn_right = (data[0] == 0xFF ? true : false);

         app_gamepad.btn_select = (data[6]&0x10 ? true : false);
         app_gamepad.btn_start = (data[6]&0x20 ? true : false);
         app_gamepad.btn_a = (data[5]&0x20 ? true : false);
         app_gamepad.btn_b = (data[5]&0x40 ? true : false);

         osd_getinput();
         //ESP_LOGW(TAG, "Gamepad... len: %d; 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", data_length, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
      }
      break;
   case HID_HOST_INTERFACE_EVENT_TRANSFER_ERROR:
      break;
   case HID_HOST_INTERFACE_EVENT_DISCONNECTED:
      hid_host_device_close(hid_device_handle);
      break;
   default:
      break;
   }
}

static void app_usb_hid_task(void *arg)
{
    hid_host_device_handle_t hid_device_handle = NULL;
    app_usb_hid_event_t msg;

    while (1) {
        if (xQueueReceive(hid_queue, &msg, pdMS_TO_TICKS(50))) {
            hid_device_handle = msg.hid_device_handle;
            hid_host_dev_params_t *dev = hid_host_device_get_params(hid_device_handle);

            assert(dev != NULL);

            switch (msg.event) {
            case HID_HOST_DRIVER_EVENT_CONNECTED:
               /* Handle mouse or keyboard */
               if (dev->proto == HID_PROTOCOL_NONE) {
                  const hid_host_device_config_t dev_config = {
                     .callback = app_usb_hid_host_interface_callback,
                  };

                  ESP_ERROR_CHECK( hid_host_device_open(hid_device_handle, &dev_config) );
                  ESP_ERROR_CHECK( hid_class_request_set_idle(hid_device_handle, 0, 0) );
                  ESP_ERROR_CHECK( hid_class_request_set_protocol(hid_device_handle, HID_REPORT_PROTOCOL_BOOT) );
                  ESP_ERROR_CHECK( hid_host_device_start(hid_device_handle) );
               } else {
                  ESP_LOGE(TAG, "Other HID device connected! Proto: %d", dev->proto);
               }
               break;
            default:
               ESP_LOGE(TAG, "Not handled HID event!");
               break;
            }
        }
    }

    xQueueReset(hid_queue);
    vQueueDelete(hid_queue);

    hid_host_uninstall();

    vTaskDelete(NULL);
}

static void app_usb_hid_callback(hid_host_device_handle_t hid_device_handle, const hid_host_driver_event_t event, void *arg)
{
    const app_usb_hid_event_t msg = {
        .hid_device_handle = hid_device_handle,
        .event = event,
        .arg = arg
    };

    xQueueSend(hid_queue, &msg, 0);
}

static void app_hid_init(void)
{
   esp_err_t ret;

   /* USB HID host driver config */
   const hid_host_driver_config_t hid_host_driver_config = {
      .create_background_task = true,
      .task_priority = 5,
      .stack_size = 4096,
      .core_id = 0,
      .callback = app_usb_hid_callback,
   };

   ret = hid_host_install(&hid_host_driver_config);
   if (ret != ESP_OK) {
      ESP_LOGE(TAG, "USB HID install failed!");
      assert(0);
   }

   hid_queue = xQueueCreate(10, sizeof(app_usb_hid_event_t));
   xTaskCreate(&app_usb_hid_task, "hid_task", 4 * 1024, NULL, 2, NULL);
}