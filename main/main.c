#include <freertos/FreeRTOS.h>

#include <driver/gpio.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_event_loop.h>
#include <esp_heap_caps.h>
#include <esp_log.h>
#include <esp_partition.h>
#include <esp_spiffs.h>
#include <nvs_flash.h>

#include "nofconfig.h"
#include "nofrendo.h"

void *mem_alloc(int size, bool fast_mem)
{
   if (fast_mem)
   {
      return heap_caps_malloc(size, MALLOC_CAP_8BIT);
   }
   else
   {
      return heap_caps_malloc_prefer(size, MALLOC_CAP_SPIRAM, MALLOC_CAP_DEFAULT);
   }
}

void spiffs_init(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = FSROOT,
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            nofrendo_log_printf("Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            nofrendo_log_printf("Failed to find SPIFFS partition");
        }
        else
        {
            nofrendo_log_printf("Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK)
    {
        nofrendo_log_printf("Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    }
    else
    {
        nofrendo_log_printf("Partition size: total: %d, used: %d\n", total, used);
    }
}

int app_main(void)
{
    spiffs_init();

    nofrendo_log_printf("NoFrendo start!\n");
    nofrendo_main(0, NULL);
    nofrendo_log_printf("NoFrendo died? Oh no!\n");
    asm("break.n 1");
    return 0;
}
