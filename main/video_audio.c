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

#include <math.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include <driver/i2s.h>

#include "bitmap.h"
#include "event.h"
#include "noftypes.h"
#include "gui.h"
#include "log.h"
#include "nes.h"
#include "nes_pal.h"
#include "nesinput.h"
#include "osd.h"
#include "stdint.h"
#include "spi_lcd.h"

#include "controller.h"

#include "nofconfig.h"
#include "sdkconfig.h"

#define DEFAULT_SAMPLERATE 32000
#define DEFAULT_FRAGSIZE 128

TimerHandle_t timer;

//Seemingly, this will be called only once. Should call func with a freq of frequency,
int osd_installtimer(int frequency, void *func, int funcsize, void *counter, int countersize)
{
	printf("Timer install, configTICK_RATE_HZ=%d, freq=%d\n", configTICK_RATE_HZ, frequency);
	timer = xTimerCreate("nes", configTICK_RATE_HZ / frequency, pdTRUE, NULL, func);
	xTimerStart(timer, 0);
	return 0;
}

/*
** Audio
*/
static void (*audio_callback)(void *buffer, int length) = NULL;
#if CONFIG_SOUND_ENA
QueueHandle_t queue;
static int16_t *audio_frame;
#endif

static void do_audio_frame()
{

#if CONFIG_SOUND_ENA
	int left = DEFAULT_SAMPLERATE / NES_REFRESH_RATE;
	while (left)
	{
		int n = DEFAULT_FRAGSIZE;
		if (n > left)
			n = left;
		audio_callback(audio_frame, n); //get more data

		//16 bit mono -> 32-bit (16 bit r+l)
		int16_t *mono_ptr = audio_frame + n;
		int16_t *stereo_ptr = audio_frame + n + n;
		int i = n;
		while (i--)
		{
			// audio_frame[i] = audio_frame[i] + 0x8000;
			int16_t a = (*(--mono_ptr) >> 3);
			*(--stereo_ptr) = 0x8000 + a;
			*(--stereo_ptr) = 0x8000 - a;
		}

		size_t i2s_bytes_write;
		i2s_write(I2S_NUM_0, (const char *)audio_frame, 4 * n, &i2s_bytes_write, portMAX_DELAY);
		left -= i2s_bytes_write / 4;
	}
#endif
}

void osd_setsound(void (*playfunc)(void *buffer, int length))
{
	//Indicates we should call playfunc() to get more data.
	audio_callback = playfunc;
}

static void osd_stopsound(void)
{
	audio_callback = NULL;
}

static int osd_init_sound(void)
{
#if CONFIG_SOUND_ENA
	audio_frame = nofrendo_malloc(4 * DEFAULT_FRAGSIZE);
	i2s_config_t cfg = {
		.mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
		.sample_rate = DEFAULT_SAMPLERATE,
		.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
		.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
		.communication_format = I2S_COMM_FORMAT_PCM | I2S_COMM_FORMAT_I2S_MSB,
		.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
		.dma_buf_count = 6,
		.dma_buf_len = 512,
		.use_apll = false};
	i2s_driver_install(I2S_NUM_0, &cfg, 2, &queue);
	i2s_set_pin(I2S_NUM_0, NULL);
	i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
#endif

	audio_callback = NULL;

	return 0;
}

void osd_getsoundinfo(sndinfo_t *info)
{
	info->sample_rate = DEFAULT_SAMPLERATE;
	info->bps = 16;
}

/*
** Video
*/

static int init(int width, int height);
static void shutdown(void);
static int set_mode(int width, int height);
static void set_palette(rgb_t *pal);
static void clear(uint8 color);
static bitmap_t *lock_write(void);
static void free_write(int num_dirties, rect_t *dirty_rects);
static void custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects);
static char fb[1]; //dummy

QueueHandle_t vidQueue;

viddriver_t sdlDriver =
	{
		"Simple DirectMedia Layer", /* name */
		init,						/* init */
		shutdown,					/* shutdown */
		set_mode,					/* set_mode */
		set_palette,				/* set_palette */
		clear,						/* clear */
		lock_write,					/* lock_write */
		free_write,					/* free_write */
		custom_blit,				/* custom_blit */
		false						/* invalidate flag */
};

bitmap_t *myBitmap;

void osd_getvideoinfo(vidinfo_t *info)
{
	info->default_width = NES_VISIBLE_WIDTH;
	info->default_height = NES_VISIBLE_HEIGHT;
	info->driver = &sdlDriver;
}

/* flip between full screen and windowed */
void osd_togglefullscreen(int code)
{
}

/* initialise video */
static int init(int width, int height)
{
	return 0;
}

static void shutdown(void)
{
}

/* set a video mode */
static int set_mode(int width, int height)
{
	return 0;
}

uint16 myPalette[256];

/* copy nes palette over to hardware */
static void set_palette(rgb_t *pal)
{
	uint16 c;

	int i;

	for (i = 0; i < 256; i++)
	{
		c = (pal[i].b >> 3) + ((pal[i].g >> 2) << 5) + ((pal[i].r >> 3) << 11);
		//myPalette[i]=(c>>8)|((c&0xff)<<8);
		myPalette[i] = c;
	}
}

/* clear all frames to a particular color */
static void clear(uint8 color)
{
	//   SDL_FillRect(mySurface, 0, color);
}

/* acquire the directbuffer for writing */
static bitmap_t *lock_write(void)
{
	//   SDL_LockSurface(mySurface);
	myBitmap = bmp_createhw((uint8 *)fb, NES_VISIBLE_WIDTH, NES_VISIBLE_HEIGHT, NES_VISIBLE_WIDTH * 2);
	return myBitmap;
}

/* release the resource */
static void free_write(int num_dirties, rect_t *dirty_rects)
{
	bmp_destroy(&myBitmap);
}

static void custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects)
{
	xQueueSend(vidQueue, &bmp, 0);
	do_audio_frame();
}

//This runs on core 1.
static void videoTask(void *arg)
{
	int x, y;
	bitmap_t *bmp = NULL;
	x = (320 - NES_VISIBLE_WIDTH) / 2;
	y = ((240 - NES_VISIBLE_HEIGHT) / 2);
	while (1)
	{
		//		xQueueReceive(vidQueue, &bmp, portMAX_DELAY);//skip one frame to drop to 30
		xQueueReceive(vidQueue, &bmp, portMAX_DELAY);
		lcd_write_frame(x, y, NES_VISIBLE_WIDTH, NES_VISIBLE_HEIGHT, (const uint8_t **)bmp->line);
	}
}

/*
** Input
*/

static void osd_initinput()
{
	controller_init();
}

void osd_getinput(void)
{
	const int ev[16] = {
		event_joypad1_select, event_state_save, event_state_load, event_joypad1_start, event_joypad1_up, event_joypad1_right, event_joypad1_down, event_joypad1_left,
		0, 0, 0, 0, event_soft_reset, event_joypad1_a, event_joypad1_b, event_hard_reset};
	static int oldb = 0xffff;
	int b = controller_read_input();
	int chg = b ^ oldb;
	int x;
	oldb = b;
	event_t evh;
	// printf("Input: %x\n", b);
	for (x = 0; x < 16; x++)
	{
		if (chg & 1)
		{
			evh = event_get(ev[x]);
			if (evh)
				evh((b & 1) ? INP_STATE_BREAK : INP_STATE_MAKE);
		}
		chg >>= 1;
		b >>= 1;
	}
}

static void osd_freeinput(void)
{
}

void osd_getmouse(int *x, int *y, int *button)
{
}

/*
** Shutdown
*/

/* this is at the bottom, to eliminate warnings */
void osd_shutdown()
{
	osd_stopsound();
	osd_freeinput();
}

static int logprint(const char *string)
{
	return printf("%s", string);
}

/*
** Startup
*/

int osd_init()
{
	log_chain_logfunc(logprint);

	if (osd_init_sound())
		return -1;

	lcd_init();
	lcd_write_frame(0, 0, 320, 240, NULL);
	vidQueue = xQueueCreate(1, sizeof(bitmap_t *));
	xTaskCreatePinnedToCore(&videoTask, "videoTask", 2048, NULL, 5, NULL, 1);
	osd_initinput();
	return 0;
}
