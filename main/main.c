#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "nofrendo.h"


#define NESROM_FLASHPOS 0x100000
#define NESROM_MMUPOS (0x3f400000+NESROM_FLASHPOS-0x20000)

unsigned char *nesromdata=(unsigned char *)(NESROM_MMUPOS);


char *osd_getromdata() {
    return (char*)nesromdata;
}


esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

int app_main(void)
{
	printf("NoFrendo start!\n");
	nofrendo_main(0, NULL);
	printf("NoFrendo died? WtF?\n");
	asm("break.n 1");
    return 0;
}

