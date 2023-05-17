/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "unity.h"
#include "unity_test_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "usb/usb_host.h"

#include "usb/hid_host.h"
#include "usb/hid_usage_keyboard.h"
#include "usb/hid_usage_mouse.h"

#include "test_hid_basic.h"

TaskHandle_t test_usb_event_task_handle;
SemaphoreHandle_t dev_connect_sem;
SemaphoreHandle_t dev_disconnect_sem;
void *test_user_arg = NULL;
QueueHandle_t hid_host_test_event_queue;
TaskHandle_t hid_test_task_handle;

#define HID_TEST_SEMAPHORE_TIMEOUT_MS (5000)
#define WAIT_HID_TEST_SEMAPHORE_TIMEOUT(sem, ms)   xSemaphoreTake((sem), pdMS_TO_TICKS((ms)))
#define WAIT_HID_TEST_SEMAPHORE(sem)   xSemaphoreTake((sem), portMAX_DELAY)
#define RELEASE_HID_TEST_SEMAPHORE(sem)  xSemaphoreGive((sem))

static const char *test_hid_sub_class_names[] = {
    "NO_SUBCLASS",
    "BOOT_INTERFACE",
};

static const char *test_hid_proto_names[] = {
    "NONE",
    "KEYBOARD",
    "MOUSE"
};

typedef struct {
    uint8_t any_var;
    uint32_t any_var32;
} test_hid_struct_t;

typedef struct {
    hid_host_device_handle_t hid_device_handle;
    hid_host_driver_event_t event;
    void *arg;
} hid_host_test_event_queue_t;

void hid_host_test_interface_callback(hid_host_device_handle_t hid_device_handle,
                                      const hid_host_interface_event_t event,
                                      void *arg)
{
    uint8_t *data = NULL;
    unsigned int data_length = 0;

    TEST_ASSERT_EQUAL_MESSAGE(test_user_arg, arg, "User argument has lost");

    hid_host_dev_params_t *dev_params = hid_host_device_get_params(hid_device_handle);

    switch (event) {
    case HID_HOST_INTERFACE_EVENT_INPUT_REPORT:
        printf("USB port %d, Interface num %d: ",
               dev_params->addr,
               dev_params->iface_num);

        data = hid_host_device_get_data(hid_device_handle,
                                        &data_length);

        for (int i = 0; i < data_length; i++) {
            printf("%02x ", data[i]);
        }
        printf("\n");
        break;
    case HID_HOST_INTERFACE_EVENT_DISCONNECTED:
        printf("USB port %d, iface num %d removed\n",
               dev_params->addr,
               dev_params->iface_num);
        TEST_ASSERT_EQUAL(ESP_OK, hid_host_device_close(hid_device_handle) );
        RELEASE_HID_TEST_SEMAPHORE(dev_disconnect_sem);
        break;
    case HID_HOST_INTERFACE_EVENT_TRANSFER_ERROR:
        break;
    default:
        break;
    }
}

void hid_host_test_callback(hid_host_device_handle_t hid_device_handle,
                            const hid_host_driver_event_t event,
                            void *arg)
{
    hid_host_dev_params_t *dev_params = hid_host_device_get_params(hid_device_handle);

    TEST_ASSERT_NOT_NULL_MESSAGE(dev_params, "Device parameters not found");
    TEST_ASSERT_EQUAL_MESSAGE(test_user_arg, arg, "User argument has lost");

    switch (event) {
    case HID_HOST_DRIVER_EVENT_CONNECTED:
        printf("USB port %d, interface %d, '%s', '%s'\n",
               dev_params->addr,
               dev_params->iface_num,
               test_hid_sub_class_names[dev_params->sub_class],
               test_hid_proto_names[dev_params->proto]);

        const hid_host_device_config_t dev_config = {
            .callback = hid_host_test_interface_callback,
            .callback_arg = test_user_arg
        };

        TEST_ASSERT_EQUAL(ESP_OK,  hid_host_device_open(hid_device_handle, &dev_config) );
        TEST_ASSERT_EQUAL(ESP_OK,  hid_host_device_start(hid_device_handle) );

        RELEASE_HID_TEST_SEMAPHORE(dev_connect_sem);

        break;
    default:
        break;
    }
}

void hid_host_test_queue(hid_host_device_handle_t hid_device_handle,
                         const hid_host_driver_event_t event,
                         void *arg)
{
    const hid_host_test_event_queue_t evt_queue = {
        .hid_device_handle = hid_device_handle,
        .event = event,
        .arg = arg
    };
    // fill mailbox
    xQueueSend(hid_host_test_event_queue, &evt_queue, 0);
}

void hid_host_test_requests_callback(hid_host_device_handle_t hid_device_handle,
                                     const hid_host_driver_event_t event,
                                     void *arg)
{
    hid_host_dev_params_t *dev_params = hid_host_device_get_params(hid_device_handle);

    TEST_ASSERT_NOT_NULL_MESSAGE(dev_params, "Device parameters not found");
    TEST_ASSERT_EQUAL_MESSAGE(test_user_arg, arg, "User argument has lost");

    uint8_t *test_buffer = NULL;
    unsigned int test_length = 0;

    switch (event) {
    case HID_HOST_DRIVER_EVENT_CONNECTED:
        printf("USB port %d, interface %d, '%s', '%s'\n",
               dev_params->addr,
               dev_params->iface_num,
               test_hid_sub_class_names[dev_params->sub_class],
               test_hid_proto_names[dev_params->proto]);

        const hid_host_device_config_t dev_config = {
            .callback = hid_host_test_interface_callback,
            .callback_arg = test_user_arg
        };

        TEST_ASSERT_EQUAL(ESP_OK,  hid_host_device_open(hid_device_handle, &dev_config) );

        // Class device requests
        // hid_host_get_report_descriptor
        test_buffer = hid_host_get_report_descriptor(hid_device_handle, &test_length);

        if (test_buffer) {
            printf("HID Report descriptor length: %d\n", test_length);
        }

        if (dev_params->proto != HID_PROTOCOL_NONE) {
            // hid_class_request_get_protocol
            hid_report_protocol_t proto;
            if (ESP_OK == hid_class_request_get_protocol(hid_device_handle, &proto)) {
                printf("HID protocol: %d\n", proto);
            }

            if (dev_params->proto == HID_PROTOCOL_KEYBOARD) {
                uint8_t idle_rate;
                // hid_class_request_get_idle
                if (ESP_OK == hid_class_request_get_idle(hid_device_handle,
                        0, &idle_rate)) {
                    printf("HID idle rate: %d\n", idle_rate);
                }
                // hid_class_request_set_idle
                if (ESP_OK == hid_class_request_set_idle(hid_device_handle,
                        0, 0)) {
                    printf("HID idle rate set to 0\n");
                }

                // hid_class_request_get_report
                uint8_t tmp[10] = { 0 };
                size_t rep_len = 8;
                if (ESP_OK == hid_class_request_get_report(hid_device_handle,
                        HID_REPORT_TYPE_INPUT, 0, tmp, &rep_len)) {
                    printf("HID get report type %d, id %d, length: %d\n",
                           HID_REPORT_TYPE_INPUT, 0, rep_len);
                }

                // hid_class_request_set_report
                uint8_t rep[1] = { 0x00 };
                if (ESP_OK == hid_class_request_set_report(hid_device_handle,
                        HID_REPORT_TYPE_OUTPUT, 0, rep, 1)) {
                    printf("HID set report type %d, id %d\n", HID_REPORT_TYPE_OUTPUT, 0);
                }
            }

            // hid_class_request_set_protocol
            if (ESP_OK == hid_class_request_set_protocol(hid_device_handle,
                    HID_REPORT_PROTOCOL_BOOT)) {
                printf("HID protocol change to BOOT: %d\n", proto);
            }
        }

        TEST_ASSERT_EQUAL(ESP_OK,  hid_host_device_start(hid_device_handle) );

        RELEASE_HID_TEST_SEMAPHORE(dev_connect_sem);

        break;
    default:
        break;
    }
}

void hid_host_test_task(void *pvParameters)
{
    hid_host_test_event_queue_t evt_queue;

    do {
        // read mailbox
        if (xQueueReceive(hid_host_test_event_queue, &evt_queue, pdMS_TO_TICKS(50))) {
            hid_host_test_requests_callback(evt_queue.hid_device_handle,
                                            evt_queue.event,
                                            evt_queue.arg);
        }
    } while (1);
}

/**
 * @brief Handle common USB host library events
 *
 * @param[in] args  Pointer to arguments, does not used
 */
static void test_handle_usb_events(void *args)
{
    while (1) {
        uint32_t event_flags;
        usb_host_lib_handle_events(portMAX_DELAY, &event_flags);

        // Release devices once all clients has deregistered
        if (event_flags & USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS) {
            usb_host_device_free_all();
        }
        // Give ready_to_uninstall_usb semaphore to indicate that USB Host library
        // can be deinitialized, and terminate this task.
        if (event_flags & USB_HOST_LIB_EVENT_FLAGS_ALL_FREE) {
            // ...
        }
    }

    vTaskDelete(NULL);
}

// ----------------------- Public -------------------------

/**
 * @brief Setups HID
 *
 * - Create events handle
 * - Create usb_events task
 * - Install USB Host driver
 * - Install HID Host driver
 */
void test_hid_setup(hid_host_driver_event_cb_t device_callback)
{
    const usb_host_config_t host_config = {
        .skip_phy_setup = false,
        .intr_flags = ESP_INTR_FLAG_LEVEL1,
    };

    TEST_ASSERT_EQUAL(ESP_OK, usb_host_install(&host_config) );

    xTaskCreate(test_handle_usb_events, "usb_events", 4096, NULL, 2, &test_usb_event_task_handle);

    test_user_arg = calloc(1, sizeof(test_hid_struct_t));

    TEST_ASSERT_NOT_NULL_MESSAGE(test_user_arg, "Allocate error");

    // hid host driver config
    const hid_host_driver_config_t hid_host_driver_config = {
        .create_background_task = true,
        .task_priority = 5,
        .stack_size = 4096,
        .core_id = 0,
        .callback = device_callback,
        .callback_arg = test_user_arg
    };

    TEST_ASSERT_EQUAL(ESP_OK, hid_host_install(&hid_host_driver_config) );
}

/**
 * @brief Teardowns HID
 *
 * - Uninstall HID Host driver
 * - Uninstall USB Host driver
 * - Delete usb_events task
 * - Delete events handle
 */
void test_hid_teardown(void)
{
    TEST_ASSERT_EQUAL(ESP_OK, hid_host_uninstall() );
    free(test_user_arg);
    TEST_ASSERT_EQUAL(ESP_OK, usb_host_uninstall() );
    vTaskDelete(test_usb_event_task_handle);
}

// ------------------------- HID Test ------------------------------------------

static void test_setup_hid_basic(hid_host_driver_event_cb_t cb)
{
    test_hid_setup(cb);
    dev_connect_sem = xSemaphoreCreateBinary();
    dev_disconnect_sem = xSemaphoreCreateBinary();
}

static void test_setup_hid_task(void)
{
    // create queue
    hid_host_test_event_queue = xQueueCreate(10, sizeof(hid_host_test_event_queue_t));
    // create process
    xTaskCreate(&hid_host_test_task,
                "hid_task",
                4 * 1024,
                NULL,
                2,
                &hid_test_task_handle);
}

static void test_delete_hid_task(void)
{
    // delete queue
    xQueueReset(hid_host_test_event_queue);
    vQueueDelete(hid_host_test_event_queue);
    // delete process
    vTaskDelete(hid_test_task_handle);
}

static void test_teardown_hid_basic(void)
{
    vSemaphoreDelete(dev_connect_sem);
    vSemaphoreDelete(dev_disconnect_sem);
    test_hid_teardown();
    //Short delay to allow task to be cleaned up
    vTaskDelay(pdMS_TO_TICKS(10));
}

TEST_CASE("(HID Host) Memory leakage basic", "[auto][hid_host]")
{
    test_setup_hid_basic(hid_host_test_callback);
    test_teardown_hid_basic();
    vTaskDelay(20);
}

TEST_CASE("(HID Host) Memory leakage with HID device connection", "[hid_host]")
{
    test_setup_hid_basic(hid_host_test_callback);
    printf("Please, insert HID device (%d ms)...\n", HID_TEST_SEMAPHORE_TIMEOUT_MS);
    WAIT_HID_TEST_SEMAPHORE_TIMEOUT(dev_connect_sem, HID_TEST_SEMAPHORE_TIMEOUT_MS);
    printf("Please, remove HID device (%d ms)...\n", HID_TEST_SEMAPHORE_TIMEOUT_MS);
    WAIT_HID_TEST_SEMAPHORE_TIMEOUT(dev_disconnect_sem, HID_TEST_SEMAPHORE_TIMEOUT_MS);
    test_teardown_hid_basic();
    vTaskDelay(20);
}

TEST_CASE("(HID Host) HID Device RAW input report", "[hid_host]")
{
    test_setup_hid_basic(hid_host_test_callback);
    printf("Please, insert HID device (%d ms)...\n", HID_TEST_SEMAPHORE_TIMEOUT_MS);
    WAIT_HID_TEST_SEMAPHORE_TIMEOUT(dev_connect_sem, HID_TEST_SEMAPHORE_TIMEOUT_MS);
    printf("Remove HID Device any time. Input report will be shown as a raw HEX data.\n");
    WAIT_HID_TEST_SEMAPHORE(dev_disconnect_sem);
    test_teardown_hid_basic();
    vTaskDelay(20);
}

TEST_CASE("(HID Host) HID Specific Class requests", "[hid_host]")
{
    test_setup_hid_basic(hid_host_test_queue);
    test_setup_hid_task();
    printf("Please, insert HID device (%d ms)...\n", HID_TEST_SEMAPHORE_TIMEOUT_MS);
    WAIT_HID_TEST_SEMAPHORE_TIMEOUT(dev_connect_sem, HID_TEST_SEMAPHORE_TIMEOUT_MS);
    printf("Remove HID Device any time. Input report will be shown as a raw HEX data.\n");
    WAIT_HID_TEST_SEMAPHORE(dev_disconnect_sem);
    test_teardown_hid_basic();
    test_delete_hid_task();
    vTaskDelay(20);
}

TEST_CASE("(HID Host) USB uninstall (while device is present)", "[hid_host]")
{
    test_setup_hid_basic(hid_host_test_callback);
    printf("Please, insert HID device (%d ms)...\n", HID_TEST_SEMAPHORE_TIMEOUT_MS);
    WAIT_HID_TEST_SEMAPHORE_TIMEOUT(dev_connect_sem, HID_TEST_SEMAPHORE_TIMEOUT_MS);
    // Uninstall USB HID while device still present
    test_teardown_hid_basic();
    vTaskDelay(20);
}
