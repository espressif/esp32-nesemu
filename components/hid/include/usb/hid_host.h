/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <wchar.h>
#include <stdint.h>
#include "esp_err.h"
#include <freertos/FreeRTOS.h>

#include "hid.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hid_interface *hid_host_device_handle_t;    /**< Device Handle. Handle to a particular HID interface */

// ------------------------ USB HID Host events --------------------------------
/**
 * @brief USB HID HOST Device event id
*/
typedef enum {
    HID_HOST_DRIVER_EVENT_CONNECTED = 0x00,        /**< HID Device has been found in connected USB device (at least one) */
} hid_host_driver_event_t;

/**
 * @brief USB HID HOST Interface event id
*/
typedef enum {
    HID_HOST_INTERFACE_EVENT_INPUT_REPORT = 0x00,     /**< HID Device input report */
    HID_HOST_INTERFACE_EVENT_TRANSFER_ERROR,          /**< HID Device transfer error */
    HID_HOST_INTERFACE_EVENT_DISCONNECTED,            /**< HID Device has been disconnected */
} hid_host_interface_event_t;

/**
 * @brief USB HID Host device parameters
*/
typedef struct {
    uint8_t addr;                       /**< USB Address of connected HID device.*/
    uint8_t iface_num;                  /**< HID Interface Number */
    uint8_t sub_class;                  /**< HID Interface SubClass */
    uint8_t proto;                      /**< HID Interface Protocol */
} hid_host_dev_params_t;

// ------------------------ USB HID Host callbacks -----------------------------

/**
 * @brief USB HID driver event callback.
 *
 * @param[in] hid_handle  HID device handle (HID Interface)
 * @param[in] event       HID driver event
 * @param[in] arg         User argument from HID driver configuration structure
*/
typedef void (*hid_host_driver_event_cb_t)(hid_host_device_handle_t hid_device_handle,
        const hid_host_driver_event_t event,
        void *arg);

/**
 * @brief USB HID Interface event callback.
 *
 * @param[in] hid_device_handle     HID device handle (HID Interface)
 * @param[in] event                 HID Interface event
 * @param[in] arg                   User argument
*/
typedef void (*hid_host_interface_event_cb_t)(hid_host_device_handle_t hid_device_handle,
        const hid_host_interface_event_t event,
        void *arg);

// ----------------------------- Public ---------------------------------------
/**
 * @brief HID configuration structure.
*/
typedef struct {
    bool create_background_task;            /**< When set to true, background task handling USB events is created.
                                         Otherwise user has to periodically call hid_host_event_handler_task function */
    size_t task_priority;                   /**< Task priority of created background task */
    size_t stack_size;                      /**< Stack size of created background task */
    BaseType_t core_id;                     /**< Select core on which background task will run or tskNO_AFFINITY  */
    hid_host_driver_event_cb_t callback;    /**< Callback invoked when HID driver event occurs. Must not be NULL. */
    void *callback_arg;                     /**< User provided argument passed to callback */
} hid_host_driver_config_t;

/**
 * @brief HID device configuration structure (HID Interface)
*/
typedef struct {
    hid_host_interface_event_cb_t callback;     /**< Callback invoked when HID Interface event occurs */
    void *callback_arg;                         /**< User provided argument passed to callback */
} hid_host_device_config_t;

/**
 * @brief USB HID Host install USB Host HID Class driver
 *
 * @param[in] config configuration structure HID to create
 * @return esp_err_r
 */
esp_err_t hid_host_install(const hid_host_driver_config_t *config);

/**
 * @brief USB HID Host uninstall HID Class driver
 * @return esp_err_t
 */
esp_err_t hid_host_uninstall(void);

/**
 * @brief USB HID Host open a device with specific device parameters
 *
 * @param[in] iface_handle     Pointer to an opened HID Device
 * @param[in] config           Configuration structure HID device to open
 * @return esp_err_t
 */
esp_err_t hid_host_device_open(hid_host_device_handle_t hid_dev_handle,
                               const hid_host_device_config_t *config);

/**
 * @brief USB HID Host close device
 *
 * @param[in] hid_dev_handle   Device handle obtained from hid_host_open_device function
 * @return esp_err_t
 */
esp_err_t hid_host_device_close(hid_host_device_handle_t hid_dev_handle);

/**
 * @brief HID Host USB event handler
 *
 * If HID Host install was made with create_background_task=false configuration,
 * application needs to handle USB Host events itself.
 * Do not used if HID host install was made with create_background_task=true configuration
 *
 * @param[in]  timeout_ms  Timeout in milliseconds
 * @return esp_err_t
 */
esp_err_t hid_host_handle_events(uint32_t timeout_ms);

/**
 * @brief HID Device get parameters by handle.
 *
 * @param[in] hid_dev_handle    HID Device handle
 *
 * @return dev_params           Pointer to a device parameters (USB port, Interface number, SubClass and Proto)
 */
hid_host_dev_params_t *hid_host_device_get_params(hid_host_device_handle_t hid_dev_handle);

/**
 * @brief HID Host get device transferred data pointer by handle
 *
 * @param[in] hid_dev_handle    HID Device handle
 * @param[out] data_length  Length of transferred data
 *
 * @return a uint8_t pointer to a data buffer
 */
uint8_t *hid_host_device_get_data(hid_host_device_handle_t hid_dev_handle,
                                  unsigned int *data_length);

// ------------------------ USB HID Host driver API ----------------------------

/**
 * @brief HID Host enable device by handle. Calls an Interface callback when the event is occured.
 *
 * @param[in] hid_dev_handle  HID Device handle
 * @return esp_err_t
 */
esp_err_t hid_host_device_start(hid_host_device_handle_t hid_dev_handle);

/**
 * @brief HID Host stop device
 *
 * @param[in] hid_dev_handle  HID Device handle
 *
 * @return esp_err_t
 */
esp_err_t hid_host_device_stop(hid_host_device_handle_t hid_dev_handle);

/**
 * @brief HID Host Get Report Descriptor
 *
 * @param[in] hid_dev_handle   HID Device handle
 * @param[out] report_desc_len Length of report descriptor
 *
 * @return a uint8_t pointer to report descriptor data
 */
uint8_t *hid_host_get_report_descriptor(hid_host_device_handle_t hid_dev_handle,
                                        size_t *report_desc_len);

/**
 * @brief HID class specific request GET REPORT
 *
 * @param[in] hid_dev_handle    HID Device handle
 * @param[in] report_id         Report ID
 * @param[out] report           Pointer to buffer for a report data
 * @param[in/out] report_length Report data length
 *
 * @return esp_err_t
 */
esp_err_t hid_class_request_get_report(hid_host_device_handle_t hid_dev_handle,
                                       uint8_t report_type,
                                       uint8_t report_id,
                                       uint8_t *report,
                                       size_t *report_length);

/**
 * @brief HID class specific request GET IDLE
 *
 * @param[in] hid_dev_handle    HID Device handle
 * @param[in] report_id         ReportID
 * @param[out] idle_rate        Idle rate
 *
 * @return esp_err_t
 */
esp_err_t hid_class_request_get_idle(hid_host_device_handle_t hid_dev_handle,
                                     uint8_t report_id,
                                     uint8_t *idle_rate);

/**
 * @brief HID class specific request GET PROTOCOL
 *
 * @param[in] hid_dev_handle    HID Device handle
 * @param[out] protocol         Pointer to HID report protocol (boot or report) of device
 *
 * @return esp_err_t
 */
esp_err_t hid_class_request_get_protocol(hid_host_device_handle_t hid_dev_handle,
        hid_report_protocol_t *protocol);

/**
* @brief HID class specific request SET REPORT
*
* @param[in] hid_dev_handle     HID Device handle
* @param[in] report             Pointer to a buffer with report data
* @param[in] report_length      Report data length
*
* @return esp_err_t
*/
esp_err_t hid_class_request_set_report(hid_host_device_handle_t hid_dev_handle,
                                       uint8_t report_type,
                                       uint8_t report_id,
                                       uint8_t *report,
                                       size_t report_length);

/**
 * @brief HID class specific request SET IDLE
 *
 * @param[in] hid_dev_handle    HID Device handle
 * @param[in] duration          0 (zero) for the indefinite duration, non-zero, then a fixed duration used.
 * @param[in] report_id         If 0 (zero) the idle rate applies to all input reports generated by the device, otherwise ReportID
 * @return esp_err_t
 */
esp_err_t hid_class_request_set_idle(hid_host_device_handle_t hid_dev_handle,
                                     uint8_t duration,
                                     uint8_t report_id);

/**
 * @brief HID class specific request SET PROTOCOL
 *
 * @param[in] hid_dev_handle    HID Device handle
 * @param[in] protocol          HID report protocol (boot or report)
 * @return esp_err_t
 */
esp_err_t hid_class_request_set_protocol(hid_host_device_handle_t hid_dev_handle,
        uint8_t protocol);

#ifdef __cplusplus
}
#endif //__cplusplus
