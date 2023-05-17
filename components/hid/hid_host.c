/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/queue.h>
#include <sys/param.h>
#include "esp_log.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "usb/usb_host.h"

#include "usb/hid_host.h"

// HID spinlock
static portMUX_TYPE hid_lock = portMUX_INITIALIZER_UNLOCKED;
#define HID_ENTER_CRITICAL()    portENTER_CRITICAL(&hid_lock)
#define HID_EXIT_CRITICAL()     portEXIT_CRITICAL(&hid_lock)

// HID verification macroses
#define HID_GOTO_ON_FALSE_CRITICAL(exp, err)    \
    do {                                        \
        if(!(exp)) {                            \
            HID_EXIT_CRITICAL();                \
            ret = err;                          \
            goto fail;                          \
        }                                       \
    } while(0)

#define HID_RETURN_ON_FALSE_CRITICAL(exp, err)  \
    do {                                        \
        if(!(exp)) {                            \
            HID_EXIT_CRITICAL();                \
            return err;                         \
        }                                       \
    } while(0)

#define HID_GOTO_ON_ERROR(exp, msg) ESP_GOTO_ON_ERROR(exp, fail, TAG, msg)

#define HID_GOTO_ON_FALSE(exp, err, msg) ESP_GOTO_ON_FALSE( (exp), err, fail, TAG, msg )

#define HID_RETURN_ON_ERROR(exp, msg) ESP_RETURN_ON_ERROR((exp), TAG, msg)

#define HID_RETURN_ON_FALSE(exp, err, msg) ESP_RETURN_ON_FALSE( (exp), (err), TAG, msg)

#define HID_RETURN_ON_INVALID_ARG(exp) ESP_RETURN_ON_FALSE((exp) != NULL, ESP_ERR_INVALID_ARG, TAG, "Argument error")

static const char *TAG = "hid-host";

#define DEFAULT_TIMEOUT_MS  (5000)

/**
 * @brief HID Device structure.
 *
 */
typedef struct hid_host_device {
    STAILQ_ENTRY(hid_host_device) tailq_entry;  /**< HID device queue */
    SemaphoreHandle_t device_busy;              /**< HID device main mutex */
    SemaphoreHandle_t ctrl_xfer_done;           /**< Control transfer semaphore */
    usb_transfer_t *ctrl_xfer;                  /**< Pointer to control transfer buffer */
    usb_device_handle_t dev_hdl;                /**< USB device handle */
    uint8_t dev_addr;                           /**< USB devce address */
} hid_device_t;

/**
 * @brief HID Interface state
*/
typedef enum {
    HID_INTERFACE_STATE_NOT_INITIALIZED = 0x00, /**< HID Interface not initialized */
    HID_INTERFACE_STATE_IDLE,                   /**< HID Interface has been found in connected USB device */
    HID_INTERFACE_STATE_READY,                  /**< HID Interface opened and ready to start transfer */
    HID_INTERFACE_STATE_ACTIVE,                 /**< HID Interface is in use */
    HID_INTERFACE_STATE_WAIT_USER_DELETION,     /**< HID Interface wait user to be removed */
    HID_INTERFACE_STATE_MAX
} hid_iface_state_t;

/**
 * @brief HID Interface structure in device to interact with. After HID device opening keeps the interface configuration
 *
 */
typedef struct hid_interface {
    STAILQ_ENTRY(hid_interface) tailq_entry;
    hid_device_t *parent;                   /**< Parent USB HID device */
    hid_host_dev_params_t dev_params;       /**< USB device parameters */
    uint8_t ep_in;                          /**< Interrupt IN EP number */
    uint16_t ep_in_mps;                     /**< Interrupt IN max size */
    uint8_t country_code;                   /**< Country code */
    uint16_t report_desc_size;              /**< Size of Report */
    uint8_t *report_desc;                   /**< Pointer to HID Report */
    usb_transfer_t *in_xfer;                /**< Pointer to IN transfer buffer */
    hid_host_interface_event_cb_t user_cb;  /**< Interface application callback */
    void *user_cb_arg;                      /**< Interface application callback arg */
    hid_iface_state_t state;                /**< Interface ready for claiming */
} hid_iface_t;

/**
 * @brief HID driver default context
 *
 * This context is created during HID Host install.
 */
typedef struct {
    usb_host_client_handle_t client_handle;     /**< Client task handle */
    hid_host_driver_event_cb_t user_cb;         /**< User application callback */
    void *user_arg;                             /**< User application callback args */
    SemaphoreHandle_t all_events_handled;       /**< Events handler semaphore */
    volatile bool end_client_event_handling;    /**< Client event handling flag */
} hid_driver_t;

static hid_driver_t *s_hid_driver;              /**< Internal pointer to HID driver */

STAILQ_HEAD(devices, hid_host_device) devices_tailq;
STAILQ_HEAD(interfaces, hid_interface) ifaces_tailq;

// ----------------------- Private Prototypes ----------------------------------

static esp_err_t hid_host_install_device(uint8_t dev_addr,
        usb_device_handle_t dev_hdl,
        hid_device_t **hid_device);


static esp_err_t hid_host_uninstall_device(hid_device_t *hid_device);

// --------------------------- Internal Logic ----------------------------------
/**
 * @brief USB struct for request SET
*/
typedef struct usb_request_struct {
    uint8_t bRequest;               /**< bRequest  */
    uint16_t wValue;                /**< wValue: Report Type and Report ID */
    uint16_t wIndex;                /**< wIndex: Interface */
    uint16_t wLength;               /**< wLength: Report Length */
    uint8_t *data;                  /**< Pointer to data */
} usb_request_struct_t;

/**
 * @brief Get Next Interface descriptor
 *
 * @param[in] desc   Pointer to Descriptor buffer (usually starts from Configuration descriptor)
 * @param[in] len    Total length
 * @param[in] offset Pointer to offset descriptor buffer
 * @return const usb_standard_desc_t* Pointer to Interface descriptor
 */
static inline const usb_standard_desc_t *next_interface_desc(const usb_standard_desc_t *desc, size_t len, size_t *offset)
{
    return usb_parse_next_descriptor_of_type(desc, len, USB_W_VALUE_DT_INTERFACE, (int *)offset);
}

/**
 * @brief Get Next HID descriptor
 *
 * @param[in] desc   Pointer to the Descriptor buffer
 * @param[in] len    Total length
 * @param[in] offset Pointer to offset in descriptor buffer
 * @return const usb_standard_desc_t* Pointer to HID descriptor
 */
static inline const usb_standard_desc_t *next_hid_descriptor(const usb_standard_desc_t *desc, size_t len, size_t *offset)
{
    return usb_parse_next_descriptor_of_type(desc, len, HID_CLASS_DESCRIPTOR_TYPE_HID, (int *)offset);
}

/**
 * @brief Get Next Endpoint descriptor
 *
 * @param[in] desc   Pointer to Descriptor buffer
 * @param[in] len    Total length
 * @param[in] offset Pointer to offset in descriptor buffer
 * @return const usb_standard_desc_t* Pointer to Endpoint descriptor
 */
static inline const usb_standard_desc_t *next_endpoint_desc(const usb_standard_desc_t *desc, size_t len, size_t *offset)
{
    return usb_parse_next_descriptor_of_type(desc, len, USB_B_DESCRIPTOR_TYPE_ENDPOINT, (int *)offset);
}

/**
 * @brief USB Event handler
 *
 * Handle all USB related events such as USB host (usbh) events or hub events from USB hardware
 *
 * @param[in] arg   Argument, does not used
 */
static void event_handler_task(void *arg)
{
    while (1) {
        /* Here wee need a timeout 50 ms to handle end_client_event_handling flag
         * during situation when all devices were removed and it is time to remove
         * and destroy everything.
         */
        usb_host_client_handle_events(s_hid_driver->client_handle, pdMS_TO_TICKS(50));

        if (s_hid_driver->end_client_event_handling) {
            break;
        }
    }
    usb_host_client_unblock(s_hid_driver->client_handle);
    ESP_ERROR_CHECK( usb_host_client_deregister(s_hid_driver->client_handle) );
    xSemaphoreGive(s_hid_driver->all_events_handled);
    vTaskDelete(NULL);
}

/**
 * @brief Return HID device in devices list by USB device handle
 *
 * @param[in] usb_device_handle_t   USB device handle
 * @return hid_device_t Pointer to device, NULL if device not present
 */
static hid_device_t *get_hid_device_by_handle(usb_device_handle_t usb_handle)
{
    hid_device_t *device = NULL;

    HID_ENTER_CRITICAL();
    STAILQ_FOREACH(device, &devices_tailq, tailq_entry) {
        if (usb_handle == device->dev_hdl) {
            HID_EXIT_CRITICAL();
            return device;
        }
    }
    HID_EXIT_CRITICAL();
    return NULL;
}

/**
 * @brief Get HID Interface pointer by Endpoint address
 *
 * @param[in] ep_addr      Endpoint address
 * @return hid_iface_t     Pointer to HID Interface configuration structure
 */
static hid_iface_t *get_interface_by_ep(uint8_t ep_addr)
{
    hid_iface_t *interface = NULL;

    HID_ENTER_CRITICAL();
    STAILQ_FOREACH(interface, &ifaces_tailq, tailq_entry) {
        if (ep_addr == interface->ep_in) {
            HID_EXIT_CRITICAL();
            return interface;
        }
    }

    HID_EXIT_CRITICAL();
    return NULL;
}

/**
 * @brief Get Interface structure from the RAM list
 *
 * @param[in] proto  HID Protocol
 * @return hid_iface_t  Pointer to an interface structure
 */
static hid_iface_t *get_interface_by_proto(hid_protocol_t proto)
{
    hid_iface_t *interface = NULL;

    HID_ENTER_CRITICAL();
    STAILQ_FOREACH(interface, &ifaces_tailq, tailq_entry) {
        if (proto == interface->dev_params.proto) {
            HID_EXIT_CRITICAL();
            return interface;
        }
    }
    HID_EXIT_CRITICAL();
    return NULL;
}
/**
 * @brief Verify presence of Interface in the RAM list
 *
 * @param[in] iface         Pointer to an Interface structure
 * @return true             Interface is in the list
 * @return false            Interface is not in the list
 */
static inline bool is_interface_in_list(hid_iface_t *iface)
{
    hid_iface_t *interface = NULL;

    HID_ENTER_CRITICAL();
    STAILQ_FOREACH(interface, &ifaces_tailq, tailq_entry) {
        if (iface == interface) {
            HID_EXIT_CRITICAL();
            return true;
        }
    }

    HID_EXIT_CRITICAL();
    return false;
}

/**
 * @brief Get HID Interface pointer by external HID Device handle with verification in RAM list
 *
 * @param[in] hid_dev_handle HID Device handle
 * @return hid_iface_t       Pointer to an Interface structure
 */
static hid_iface_t *get_iface_by_handle(hid_host_device_handle_t hid_dev_handle)
{
    hid_iface_t *hid_iface = (hid_iface_t *) hid_dev_handle;

    if (!is_interface_in_list(hid_iface)) {
        ESP_LOGE(TAG, "HID device handle not found");
        return NULL;
    }

    return hid_iface;
}

/**
 * @brief Get Interface Descriptor
 *
 * @param[in] config_desc   Pointer to Configuration Descriptor
 * @param[in] offset        Offset
 * @param[in] proto         Interface Protocol field to search, or set to `HID_PROTOCOL_NONE` to ignore
 * @return esp_err_t
 */
static const usb_intf_desc_t *get_hid_interface_desc(const usb_config_desc_t *config_desc,
        size_t *offset)
{
    size_t total_length = config_desc->wTotalLength;
    const usb_standard_desc_t *next_desc = (const usb_standard_desc_t *)config_desc;

    next_desc = next_interface_desc(next_desc, total_length, offset);

    while ( next_desc ) {

        const usb_intf_desc_t *ifc_desc = (const usb_intf_desc_t *)next_desc;

        if ( ifc_desc->bInterfaceClass == USB_CLASS_HID) {
            return ifc_desc;
        }

        next_desc = next_interface_desc(next_desc, total_length, offset);
    };
    return NULL;
}

/**
 * @brief HID Interface user callback function.
 *
 * @param[in] hid_iface   Pointer to an Interface structure
 * @param[in] event_id    HID Interface event
 */
static inline void hid_host_user_interface_callback(hid_iface_t *hid_iface,
        const hid_host_interface_event_t event)
{
    assert(hid_iface);

    hid_host_dev_params_t *dev_params = &hid_iface->dev_params;

    assert(dev_params);

    if (hid_iface->user_cb) {
        hid_iface->user_cb(hid_iface, event, hid_iface->user_cb_arg);
    }
}

/**
 * @brief HID Device user callback function.
 *
 * @param[in] event_id    HID Device event
 * @param[in] dev_params  HID Device parameters
 */
static inline void hid_host_user_device_callback(hid_iface_t *hid_iface,
        const hid_host_driver_event_t event)
{
    assert(hid_iface);

    hid_host_dev_params_t *dev_params = &hid_iface->dev_params;

    assert(dev_params);

    if (s_hid_driver && s_hid_driver->user_cb) {
        s_hid_driver->user_cb(hid_iface, event, s_hid_driver->user_arg);
    }
}

/**
 * @brief Add interface in a list
 *
 * @param[in] hid_device    HID device handle
 * @param[in] iface_desc  Pointer to an Interface descriptor
 * @param[in] hid_desc    Pointer to an HID device descriptor
 * @param[in] ep_desc     Pointer to an EP descriptor
 * @return esp_err_t
 */
static esp_err_t hid_host_add_interface(hid_device_t *hid_device,
                                        const usb_intf_desc_t *iface_desc,
                                        const hid_descriptor_t *hid_desc,
                                        const usb_ep_desc_t *ep_in_desc)
{
    hid_iface_t *hid_iface = calloc(1, sizeof(hid_iface_t));

    HID_RETURN_ON_FALSE(hid_iface,
                        ESP_ERR_NO_MEM,
                        "Unable to allocate memory");

    HID_ENTER_CRITICAL();
    hid_iface->parent = hid_device;
    hid_iface->state = HID_INTERFACE_STATE_NOT_INITIALIZED;
    hid_iface->dev_params.addr = hid_device->dev_addr;

    if (iface_desc) {
        hid_iface->dev_params.iface_num = iface_desc->bInterfaceNumber;
        hid_iface->dev_params.sub_class = iface_desc->bInterfaceSubClass;
        hid_iface->dev_params.proto = iface_desc->bInterfaceProtocol;
    }

    if (hid_desc) {
        hid_iface->country_code = hid_desc->bCountryCode;
        hid_iface->report_desc_size = hid_desc->wReportDescriptorLength;
    }

    // EP IN && INT Type
    if (ep_in_desc) {
        if ( (ep_in_desc->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK) &&
                (ep_in_desc->bmAttributes & USB_B_ENDPOINT_ADDRESS_EP_NUM_MASK) ) {
            hid_iface->ep_in = ep_in_desc->bEndpointAddress;
            hid_iface->ep_in_mps = USB_EP_DESC_GET_MPS(ep_in_desc);
        } else {
            ESP_LOGE(TAG, "HID device EP IN %#X configuration error",
                     ep_in_desc->bEndpointAddress);
        }
    }

    if (iface_desc && hid_desc && ep_in_desc) {
        hid_iface->state = HID_INTERFACE_STATE_IDLE;
    }

    STAILQ_INSERT_TAIL(&ifaces_tailq, hid_iface, tailq_entry);
    HID_EXIT_CRITICAL();

    return ESP_OK;
}

/**
 * @brief Notify user about the connected Interfaces
 *
 * @param[in] hid_device  Pointer to HID device structure
 */
static void hid_host_notify_interface_connected(hid_device_t *hid_device)
{
    hid_iface_t *iface = NULL;
    for (int proto = HID_PROTOCOL_NONE; proto < HID_PROTOCOL_MAX; proto++) {
        iface = get_interface_by_proto(proto);
        if (iface && iface->parent && (iface->parent->dev_addr == hid_device->dev_addr)) {
            hid_host_user_device_callback(iface, HID_HOST_DRIVER_EVENT_CONNECTED);
        }
    }
}

/**
 * @brief Create a list of available interfaces in RAM
 *
 * @param[in] hid_device  Pointer to HID device structure
 * @param[in] dev_addr    USB device physical address
 * @param[in] sub_class   USB HID SubClass value
 * @return esp_err_t
 */
static esp_err_t hid_host_interface_list_create(hid_device_t *hid_device,
        const usb_config_desc_t *config_desc)
{
    const usb_intf_desc_t *iface_desc = NULL;
    const hid_descriptor_t *hid_desc = NULL;
    const usb_ep_desc_t *ep_in_desc = NULL;
    size_t offset = 0;

    const usb_standard_desc_t *next_desc = (const usb_standard_desc_t *)config_desc;
    size_t total_length = config_desc->wTotalLength;

    // For every Interface
    next_desc = next_interface_desc(next_desc, total_length, &offset);
    for ( ; next_desc ; next_desc = next_interface_desc(next_desc, total_length, &offset)) {

        iface_desc = (const usb_intf_desc_t *)next_desc;
        hid_desc = NULL;
        ep_in_desc = NULL;

        if (USB_CLASS_HID == iface_desc->bInterfaceClass) {
            // HID descriptor
            next_desc = next_hid_descriptor(next_desc, total_length, &offset);
            if (next_desc) {
                hid_desc = (const hid_descriptor_t *)next_desc;
            }
            // EP descriptors for Interface
            for (int i = 0; i < iface_desc->bNumEndpoints; i++) {

                next_desc = next_endpoint_desc(next_desc, total_length, &offset);

                if (USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK & ((const usb_ep_desc_t *)next_desc)->bEndpointAddress) {
                    if (!ep_in_desc) {
                        ep_in_desc = (const usb_ep_desc_t *)next_desc;
                    } else {
                        ESP_LOGW(TAG, "Found two EP IN in configuration. Stop.");
                        return ESP_ERR_NOT_FINISHED;
                    }
                } // EP IN
            } // for every EP within Interface

            // Add Interface to the list
            HID_RETURN_ON_ERROR( hid_host_add_interface(hid_device,
                                 iface_desc,
                                 hid_desc,
                                 ep_in_desc),
                                 "Unable to add HID Interface to the RAM list");
        } // USB_CLASS_HID
    }

    hid_host_notify_interface_connected(hid_device);

    return ESP_OK;
}

/**
 * @brief HID Host initialize device attempt
 *
 * @param[in] dev_addr   USB device physical address
 * @return true USB device contain HID Interface and device was initialized
 * @return false USB does not contain HID Interface
 */
static bool hid_host_device_init_attempt(uint8_t dev_addr)
{
    bool is_hid_device = false;
    usb_device_handle_t dev_hdl;
    const usb_config_desc_t *config_desc = NULL;
    hid_device_t *hid_device = NULL;
    size_t offset = 0;

    if ( ESP_OK == usb_host_device_open(s_hid_driver->client_handle, dev_addr, &dev_hdl)) {
        if ( usb_host_get_active_config_descriptor(dev_hdl, &config_desc) == ESP_OK ) {
            if (get_hid_interface_desc(config_desc, &offset)) {
                is_hid_device = true;
            }
        }
    }

    // Create HID interfaces list in RAM, connected to the particular USB dev
    if (is_hid_device) {
        // Proceed, add HID device to the list, get handle if necessary
        ESP_ERROR_CHECK( hid_host_install_device(dev_addr, dev_hdl, &hid_device) );
        // Create Interfaces list for a possibility to claim Interface
        ESP_ERROR_CHECK( hid_host_interface_list_create(hid_device, config_desc) );
    } else {
        usb_host_device_close(s_hid_driver->client_handle, dev_hdl);
        ESP_LOGW(TAG, "No HID device at USB port %d", dev_addr);
    }

    return is_hid_device;
}

/**
 * @brief Destroy a list of available interfaces in RAM
 *
 * @return esp_err_t
 */
static esp_err_t hid_host_interface_list_destroy(void)
{
    bool active_shutdown = false;

    hid_iface_t *iface = STAILQ_FIRST(&ifaces_tailq);
    hid_iface_t *tmp = NULL;

    while (iface != NULL) {
        tmp = STAILQ_NEXT(iface, tailq_entry);
        // free interface
        if (HID_INTERFACE_STATE_NOT_INITIALIZED != iface->state) {
            active_shutdown = true;
            ESP_ERROR_CHECK( hid_host_device_close(iface) );
        }

        HID_ENTER_CRITICAL();
        iface->state = HID_INTERFACE_STATE_NOT_INITIALIZED;
        STAILQ_REMOVE(&ifaces_tailq, iface, hid_interface, tailq_entry);
        free(iface);
        HID_EXIT_CRITICAL();

        iface = tmp;
    }

    if (active_shutdown) {
        ESP_LOGE(TAG, "Shutdown active HID device.");
    }

    return ESP_OK;
}

/**
 * @brief Deinit USB device by handle
 *
 * @param[in] dev_hdl   USB device handle
 * @return uint8_t USB device address
 */
static uint8_t hid_host_device_disconnected(usb_device_handle_t dev_hdl)
{
    uint8_t dev_addr = 0;
    hid_device_t *hid_device = get_hid_device_by_handle(dev_hdl);
    // Device should be in the list
    assert(hid_device);
    // Keep USB address to return it
    dev_addr = hid_device->dev_addr;

    hid_iface_t *iface = NULL;
    for (int proto = HID_PROTOCOL_NONE; proto < HID_PROTOCOL_MAX; proto ++) {
        iface = get_interface_by_proto(proto);
        if (iface && iface->parent && (iface->parent->dev_addr == dev_addr)) {
            ESP_ERROR_CHECK( hid_host_device_close(iface) );
            // Change state to user deletion
            iface->state = HID_INTERFACE_STATE_WAIT_USER_DELETION;
            hid_host_user_interface_callback(iface, HID_HOST_INTERFACE_EVENT_DISCONNECTED);
        }
    }

    // Delete HID compliant device
    ESP_ERROR_CHECK( hid_host_uninstall_device(hid_device) );

    return dev_addr;
}

/**
 * @brief USB Host Client's event callback
 *
 * @param[in] event    Client event message
 * @param[in] arg      Argument, does not used
 */
static void client_event_cb(const usb_host_client_event_msg_t *event, void *arg)
{
    if (event->event == USB_HOST_CLIENT_EVENT_NEW_DEV) {
        hid_host_device_init_attempt(event->new_dev.address);
    } else if (event->event == USB_HOST_CLIENT_EVENT_DEV_GONE) {
        hid_host_device_disconnected(event->dev_gone.dev_hdl);
    }
}

/**
 * @brief HID Host claim Interface and prepare transfer, change state to READY
 *
 * @param[in] iface       Pointer to Interface structure,
 * @return esp_err_t
 */
static esp_err_t hid_host_interface_claim_and_prepare_transfer(hid_iface_t *iface)
{
    HID_RETURN_ON_ERROR( usb_host_interface_claim( s_hid_driver->client_handle,
                         iface->parent->dev_hdl,
                         iface->dev_params.iface_num, 0),
                         "Unable to claim Interface");

    HID_RETURN_ON_ERROR( usb_host_transfer_alloc(iface->ep_in_mps, 0, &iface->in_xfer),
                         "Unable to allocate transfer buffer for EP IN");

    // Change state
    iface->state = HID_INTERFACE_STATE_READY;
    return ESP_OK;
}

/**
 * @brief HID Host release Interface and free transfer, change state to IDLE
 *
 * @param[in] iface       Pointer to Interface structure,
 * @return esp_err_t
 */
static esp_err_t hid_host_interface_release_and_free_transfer(hid_iface_t *iface)
{
    HID_RETURN_ON_INVALID_ARG(iface);
    HID_RETURN_ON_INVALID_ARG(iface->parent);

    HID_RETURN_ON_FALSE(is_interface_in_list(iface),
                        ESP_ERR_NOT_FOUND,
                        "Interface handle not found");

    HID_RETURN_ON_ERROR( usb_host_interface_release(s_hid_driver->client_handle,
                         iface->parent->dev_hdl,
                         iface->dev_params.iface_num),
                         "Unable to release HID Interface");

    ESP_ERROR_CHECK( usb_host_transfer_free(iface->in_xfer) );

    // Change state
    iface->state = HID_INTERFACE_STATE_IDLE;
    return ESP_OK;
}

/**
 * @brief Disable active interface
 *
 * @param[in] iface       Pointer to Interface structure
 * @return esp_err_t
 */
static esp_err_t hid_host_disable_interface(hid_iface_t *iface)
{
    HID_RETURN_ON_INVALID_ARG(iface);
    HID_RETURN_ON_INVALID_ARG(iface->parent);

    HID_RETURN_ON_FALSE(is_interface_in_list(iface),
                        ESP_ERR_NOT_FOUND,
                        "Interface handle not found");

    HID_RETURN_ON_FALSE((HID_INTERFACE_STATE_ACTIVE == iface->state),
                        ESP_ERR_INVALID_STATE,
                        "Interface wrong state");

    HID_RETURN_ON_ERROR( usb_host_endpoint_halt(iface->parent->dev_hdl, iface->ep_in),
                         "Unable to HALT EP");
    HID_RETURN_ON_ERROR( usb_host_endpoint_flush(iface->parent->dev_hdl, iface->ep_in),
                         "Unable to FLUSH EP");
    usb_host_endpoint_clear(iface->parent->dev_hdl, iface->ep_in);

    iface->state = HID_INTERFACE_STATE_READY;

    return ESP_OK;
}

/**
 * @brief HID transfer complete status verification
 *
 * @param[in] transfer   Pointer to USB transfer struct
 * @return true Transfer completed successfully
 * @return false Transfer completed with error
 */
static bool transfer_complete_status_verify(usb_transfer_t *transfer)
{
    bool completed = false;

    assert(transfer);

    switch (transfer->status) {
    case USB_TRANSFER_STATUS_COMPLETED:
        completed = true;
        break;
    case USB_TRANSFER_STATUS_NO_DEVICE: // User is notified about device disconnection from usb_event_cb
    case USB_TRANSFER_STATUS_CANCELED:
        break;
    case USB_TRANSFER_STATUS_ERROR:
    case USB_TRANSFER_STATUS_TIMED_OUT:
    case USB_TRANSFER_STATUS_STALL:
    case USB_TRANSFER_STATUS_OVERFLOW:
    case USB_TRANSFER_STATUS_SKIPPED:
    default:
        // Transfer was not completed or canceled by user.
        ESP_LOGE(TAG, "Transfer failed, status %d", transfer->status);
    }
    return completed;
}

/**
 * @brief HID IN Transfer complete callback
 *
 * @param[in] transfer  Pointer to transfer data structure
 */
static void in_xfer_done(usb_transfer_t *in_xfer)
{
    assert(in_xfer);

    hid_iface_t *iface = get_interface_by_ep(in_xfer->bEndpointAddress);

    assert(iface);

    if (!transfer_complete_status_verify(in_xfer)) {
        hid_host_user_interface_callback(iface, HID_HOST_INTERFACE_EVENT_TRANSFER_ERROR);
        return;
    }

    // Notify user
    hid_host_user_interface_callback(iface, HID_HOST_INTERFACE_EVENT_INPUT_REPORT);
    // Relaunch transfer
    usb_host_transfer_submit(in_xfer);
}

static void hid_device_release(hid_device_t *hid_device)
{
    xSemaphoreGive(hid_device->device_busy);
}

static esp_err_t hid_device_claim(hid_device_t *hid_device, uint32_t timeout_ms)
{
    return ( xSemaphoreTake(hid_device->device_busy, pdMS_TO_TICKS(timeout_ms))
             ? ESP_OK
             : ESP_ERR_TIMEOUT );
}

static esp_err_t hid_device_wait_ready(hid_device_t *hid_device, uint32_t timeout_ms)
{
    HID_RETURN_ON_ERROR( hid_device_claim(hid_device, timeout_ms),
                         "Unable to lock semaphore on HID Device"); // Potential concurrent problem here
    hid_device_release(hid_device);
    return ESP_OK;
}

/**
 * @brief HID Control transfer complete callback
 *
 * @param[in] ctrl_xfer  Pointer to transfer data structure
 */
static void ctrl_xfer_done(usb_transfer_t *ctrl_xfer)
{
    assert(ctrl_xfer);
    hid_device_t *hid_device = (hid_device_t *)ctrl_xfer->context;
    xSemaphoreGive(hid_device->ctrl_xfer_done);
}

/**
 * @brief HID control transfer synchronous.
 *
 * @note  Passes interface and endpoint descriptors to obtain:

 *        - interface number, IN endpoint, OUT endpoint, max. packet size
 *
 * @param[in] hid_device  Pointer to HID device structure
 * @param[in] ctrl_xfer   Pointer to the Transfer structure
 * @param[in] len         Number of bytes to transfer
 * @param[in] timeout_ms  Timeout in ms
 * @return esp_err_t
 */
static esp_err_t hid_control_transfer(hid_device_t *hid_device,
                                      usb_transfer_t *ctrl_xfer,
                                      size_t len,
                                      uint32_t timeout_ms)
{
    ctrl_xfer->device_handle = hid_device->dev_hdl;
    ctrl_xfer->callback = ctrl_xfer_done;
    ctrl_xfer->context = hid_device;
    ctrl_xfer->bEndpointAddress = 0;
    ctrl_xfer->timeout_ms = timeout_ms;
    ctrl_xfer->num_bytes = len;

    HID_RETURN_ON_ERROR( usb_host_transfer_submit_control(s_hid_driver->client_handle, ctrl_xfer),
                         "Unable to submit control transfer");

    BaseType_t received = xSemaphoreTake(hid_device->ctrl_xfer_done, pdMS_TO_TICKS(ctrl_xfer->timeout_ms));

    if (received != pdTRUE) {
        // Transfer was not finished, error in USB LIB. Reset the endpoint
        ESP_LOGE(TAG, "Control Transfer Timeout");

        HID_RETURN_ON_ERROR( usb_host_endpoint_halt(hid_device->dev_hdl, ctrl_xfer->bEndpointAddress),
                             "Unable to HALT EP");
        HID_RETURN_ON_ERROR( usb_host_endpoint_flush(hid_device->dev_hdl, ctrl_xfer->bEndpointAddress),
                             "Unable to FLUSH EP");
        usb_host_endpoint_clear(hid_device->dev_hdl, ctrl_xfer->bEndpointAddress);
        return ESP_ERR_TIMEOUT;
    }

    ESP_LOG_BUFFER_HEXDUMP(TAG, ctrl_xfer->data_buffer, ctrl_xfer->actual_num_bytes, ESP_LOG_DEBUG);

    return ESP_OK;
}

/**
 * @brief USB class standard request get descriptor
 *
 * @param[in] hidh_device Pointer to HID device structure
 * @param[in] bRequest    Value to fill bRequest filed in request
 * @param[in] wValue      Value to fill wValue filed in request
 * @param[in] wIndex      Value to fill wIndex filed in request
 * @param[in] data        Pointer to date buffer to put received data, should be not less than length
 * @param[in] length      Length of data to receive
 * @return esp_err_t
 */
static esp_err_t usb_class_request_get_descriptor(hid_device_t *hid_device,
        uint8_t bRequest,
        uint16_t wValue,
        uint16_t wIndex,
        uint8_t *data,
        uint32_t length)
{
    usb_transfer_t *ctrl_xfer = hid_device->ctrl_xfer;

    HID_RETURN_ON_INVALID_ARG(hid_device);
    HID_RETURN_ON_INVALID_ARG(hid_device->ctrl_xfer);
    HID_RETURN_ON_INVALID_ARG(data);

    if (hid_device->ctrl_xfer->data_buffer_size <
            (USB_SETUP_PACKET_SIZE + length)) {
        usb_device_info_t dev_info;
        ESP_ERROR_CHECK(usb_host_device_info(hid_device->dev_hdl, &dev_info));
        // reallocate the ctrl xfer buffer for new length
        ESP_LOGI(TAG, "Change HID ctrl xfer size from %d to %d",
                 hid_device->ctrl_xfer->data_buffer_size,
                 (int) (dev_info.bMaxPacketSize0 + length));

        usb_host_transfer_free(hid_device->ctrl_xfer);
        HID_RETURN_ON_ERROR( usb_host_transfer_alloc(USB_SETUP_PACKET_SIZE + length,
                             0,
                             &hid_device->ctrl_xfer),
                             "Unable to allocate transfer buffer for EP0");
    }

    HID_RETURN_ON_ERROR( hid_device_wait_ready(hid_device, DEFAULT_TIMEOUT_MS),
                         "HID Device is busy by other task");
    HID_RETURN_ON_ERROR( hid_device_claim(hid_device, DEFAULT_TIMEOUT_MS),
                         "HID Device is busy by other task");

    usb_setup_packet_t *setup = (usb_setup_packet_t *)ctrl_xfer->data_buffer;

    setup->bmRequestType = USB_BM_REQUEST_TYPE_DIR_IN |
                           USB_BM_REQUEST_TYPE_TYPE_STANDARD |
                           USB_BM_REQUEST_TYPE_RECIP_INTERFACE;
    setup->bRequest = bRequest;
    setup->wValue = wValue;
    setup->wIndex = wIndex;
    setup->wLength = length;

    HID_RETURN_ON_ERROR( hid_control_transfer(hid_device,
                         ctrl_xfer,
                         USB_SETUP_PACKET_SIZE + length,
                         DEFAULT_TIMEOUT_MS),
                         "Control transfer failure");

    ctrl_xfer->actual_num_bytes -= USB_SETUP_PACKET_SIZE;

    if (ctrl_xfer->actual_num_bytes <= length) {
        memcpy(data, ctrl_xfer->data_buffer + USB_SETUP_PACKET_SIZE, ctrl_xfer->actual_num_bytes);
    }

    hid_device_release(hid_device);

    return ESP_OK;
}

/**
 * @brief HID Host Request Report Descriptor
 *
 * @param[in] hidh_iface      Pointer to HID Interface configuration structure
 * @return esp_err_t
 */
esp_err_t hid_class_request_report_descriptor(hid_iface_t *iface)
{
    HID_RETURN_ON_INVALID_ARG(iface);

    HID_RETURN_ON_FALSE((HID_INTERFACE_STATE_READY == iface->state),
                        ESP_ERR_INVALID_STATE,
                        "Unable to request report decriptor. Interface is not ready");

    iface->report_desc = calloc(1, iface->report_desc_size);
    HID_RETURN_ON_FALSE(iface->report_desc,
                        ESP_ERR_NO_MEM,
                        "Unable to allocate memory");

    esp_err_t ret = usb_class_request_get_descriptor(iface->parent,
                    USB_B_REQUEST_GET_DESCRIPTOR,
                    (HID_CLASS_DESCRIPTOR_TYPE_REPORT << 8),
                    iface->dev_params.iface_num,
                    iface->report_desc,
                    iface->report_desc_size);

    return ret;
}

/**
 * @brief HID class specific request Set
 *
 * @param[in] hid_device Pointer to HID device structure
 * @param[in] req           Pointer to a set request struct
 * @return esp_err_t
 */
static esp_err_t hid_class_request_set(hid_device_t *hid_device,
                                       const usb_request_struct_t *req)
{
    esp_err_t ret;
    usb_transfer_t *ctrl_xfer = hid_device->ctrl_xfer;
    HID_RETURN_ON_INVALID_ARG(hid_device);
    HID_RETURN_ON_INVALID_ARG(hid_device->ctrl_xfer);

    HID_RETURN_ON_ERROR( hid_device_wait_ready(hid_device, DEFAULT_TIMEOUT_MS),
                         "HID Device is busy by other task");
    HID_RETURN_ON_ERROR( hid_device_claim(hid_device, DEFAULT_TIMEOUT_MS),
                         "HID Device is busy by other task");

    usb_setup_packet_t *setup = (usb_setup_packet_t *)ctrl_xfer->data_buffer;
    setup->bmRequestType = USB_BM_REQUEST_TYPE_DIR_OUT |
                           USB_BM_REQUEST_TYPE_TYPE_CLASS |
                           USB_BM_REQUEST_TYPE_RECIP_INTERFACE;
    setup->bRequest = req->bRequest;
    setup->wValue = req->wValue;
    setup->wIndex = req->wIndex;
    setup->wLength = req->wLength;

    if (req->wLength && req->data) {
        memcpy(ctrl_xfer->data_buffer + USB_SETUP_PACKET_SIZE, req->data, req->wLength);
    }

    ret = hid_control_transfer(hid_device, ctrl_xfer, USB_SETUP_PACKET_SIZE + setup->wLength, DEFAULT_TIMEOUT_MS);

    hid_device_release(hid_device);

    return ret;
}

/**
 * @brief HID class specific request Get
 *
 * @param[in] hid_device    Pointer to HID device structure
 * @param[in] req           Pointer to a get request struct
 * @param[out] out_length   Length of the response in data buffer of req struct
 * @return esp_err_t
 */
static esp_err_t hid_class_request_get(hid_device_t *hid_device,
                                       const usb_request_struct_t *req,
                                       size_t *out_length)
{
    esp_err_t ret;
    HID_RETURN_ON_INVALID_ARG(hid_device);
    HID_RETURN_ON_INVALID_ARG(hid_device->ctrl_xfer);

    usb_transfer_t *ctrl_xfer = hid_device->ctrl_xfer;

    HID_RETURN_ON_ERROR( hid_device_wait_ready(hid_device, DEFAULT_TIMEOUT_MS),
                         "HID Device is busy by other task");
    HID_RETURN_ON_ERROR( hid_device_claim(hid_device, DEFAULT_TIMEOUT_MS),
                         "HID Device is busy by other task");

    usb_setup_packet_t *setup = (usb_setup_packet_t *)ctrl_xfer->data_buffer;

    setup->bmRequestType = USB_BM_REQUEST_TYPE_DIR_IN |
                           USB_BM_REQUEST_TYPE_TYPE_CLASS |
                           USB_BM_REQUEST_TYPE_RECIP_INTERFACE;
    setup->bRequest = req->bRequest;
    setup->wValue = req->wValue;
    setup->wIndex = req->wIndex;
    setup->wLength = req->wLength;

    ret = hid_control_transfer(hid_device, ctrl_xfer, USB_SETUP_PACKET_SIZE + setup->wLength, DEFAULT_TIMEOUT_MS);

    if (ESP_OK == ret) {
        // We do not need the setup data, which is still in the transfer data buffer
        ctrl_xfer->actual_num_bytes -= USB_SETUP_PACKET_SIZE;
        // Copy data if the size is ok
        if (ctrl_xfer->actual_num_bytes <= req->wLength) {
            memcpy(req->data, ctrl_xfer->data_buffer + USB_SETUP_PACKET_SIZE, ctrl_xfer->actual_num_bytes);
        }
        // return actual num bytes of response
        if (out_length) {
            *out_length = ctrl_xfer->actual_num_bytes;
        }
    }

    hid_device_release(hid_device);

    return ret;
}

// ---------------------------- Private ---------------------------------------

esp_err_t hid_host_install_device(uint8_t dev_addr,
                                  usb_device_handle_t dev_hdl,
                                  hid_device_t **hid_device_handle)
{
    esp_err_t ret;
    hid_device_t *hid_device;

    HID_GOTO_ON_FALSE( hid_device = calloc(1, sizeof(hid_device_t)),
                       ESP_ERR_NO_MEM,
                       "Unable to allocate memory for HID Device");

    hid_device->dev_addr = dev_addr;
    hid_device->dev_hdl = dev_hdl;

    HID_GOTO_ON_FALSE( hid_device->ctrl_xfer_done = xSemaphoreCreateBinary(),
                       ESP_ERR_NO_MEM,
                       "Unable to create semaphore");
    HID_GOTO_ON_FALSE( hid_device->device_busy =  xSemaphoreCreateMutex(),
                       ESP_ERR_NO_MEM,
                       "Unable to create semaphore");

    // dev_info.bMaxPacketSize0 + 1 because of the GET PROTOCOL specific class request
    usb_device_info_t dev_info;
    ESP_ERROR_CHECK(usb_host_device_info(hid_device->dev_hdl, &dev_info));
    // TIP: Usually, we need to allocate dev_info.bMaxPacketSize0 + 1 here.
    // To take report descriptor of HID device into a consideration, we need to allocate more here. E.g. 512 bytes.
    HID_GOTO_ON_ERROR(usb_host_transfer_alloc(512, 0, &hid_device->ctrl_xfer),
                      "Unable to allocate transfer buffer");


    HID_ENTER_CRITICAL();
    HID_GOTO_ON_FALSE_CRITICAL( s_hid_driver, ESP_ERR_INVALID_STATE );
    HID_GOTO_ON_FALSE_CRITICAL( s_hid_driver->client_handle, ESP_ERR_INVALID_STATE );
    STAILQ_INSERT_TAIL(&devices_tailq, hid_device, tailq_entry);
    HID_EXIT_CRITICAL();

    if (hid_device_handle) {
        *hid_device_handle = hid_device;
    }

    return ESP_OK;

fail:
    hid_host_uninstall_device(hid_device);
    return ret;
}

esp_err_t hid_host_uninstall_device(hid_device_t *hid_device)
{
    HID_RETURN_ON_INVALID_ARG(hid_device);

    HID_RETURN_ON_ERROR( usb_host_transfer_free(hid_device->ctrl_xfer),
                         "Unablet to free transfer buffer for EP0");
    HID_RETURN_ON_ERROR( usb_host_device_close(s_hid_driver->client_handle,
                         hid_device->dev_hdl),
                         "Unable to close USB host");

    if (hid_device->ctrl_xfer_done) {
        vSemaphoreDelete(hid_device->ctrl_xfer_done);
    }

    if (hid_device->device_busy) {
        vSemaphoreDelete(hid_device->device_busy);
    }

    HID_ENTER_CRITICAL();
    STAILQ_REMOVE(&devices_tailq, hid_device, hid_host_device, tailq_entry);
    HID_EXIT_CRITICAL();

    free(hid_device);
    return ESP_OK;
}

// ----------------------------- Public ----------------------------------------

esp_err_t hid_host_install(const hid_host_driver_config_t *config)
{
    esp_err_t ret;

    HID_RETURN_ON_INVALID_ARG(config);
    HID_RETURN_ON_INVALID_ARG(config->callback);

    if ( config->create_background_task ) {
        HID_RETURN_ON_FALSE(config->stack_size != 0,
                            ESP_ERR_INVALID_ARG,
                            "Wrong stack size value");
        HID_RETURN_ON_FALSE(config->task_priority != 0,
                            ESP_ERR_INVALID_ARG,
                            "Wrong task priority value");
    }

    HID_RETURN_ON_FALSE(!s_hid_driver,
                        ESP_ERR_INVALID_STATE,
                        "HID Host driver is already installed");

    // Create HID driver structure
    hid_driver_t *driver = heap_caps_calloc(1, sizeof(hid_driver_t), MALLOC_CAP_DEFAULT);
    HID_RETURN_ON_FALSE(driver,
                        ESP_ERR_NO_MEM,
                        "Unable to allocate memory");

    driver->user_cb = config->callback;
    driver->user_arg = config->callback_arg;

    usb_host_client_config_t client_config = {
        .is_synchronous = false,
        .async.client_event_callback = client_event_cb,
        .async.callback_arg = NULL,
        .max_num_event_msg = 10,
    };

    driver->end_client_event_handling = false;
    driver->all_events_handled = xSemaphoreCreateBinary();
    HID_GOTO_ON_FALSE(driver->all_events_handled,
                      ESP_ERR_NO_MEM,
                      "Unable to create semaphore");

    HID_GOTO_ON_ERROR( usb_host_client_register(&client_config,
                       &driver->client_handle),
                       "Unable to register USB Host client");

    HID_ENTER_CRITICAL();
    HID_GOTO_ON_FALSE_CRITICAL(!s_hid_driver, ESP_ERR_INVALID_STATE);
    s_hid_driver = driver;
    STAILQ_INIT(&devices_tailq);
    STAILQ_INIT(&ifaces_tailq);
    HID_EXIT_CRITICAL();

    if (config->create_background_task) {
        BaseType_t task_created = xTaskCreatePinnedToCore(
                                      event_handler_task,
                                      "USB HID Host",
                                      config->stack_size,
                                      NULL,
                                      config->task_priority,
                                      NULL,
                                      config->core_id);
        HID_GOTO_ON_FALSE(task_created,
                          ESP_ERR_NO_MEM,
                          "Unable to create USB HID Host task");
    }

    return ESP_OK;

fail:
    s_hid_driver = NULL;
    if (driver->client_handle) {
        usb_host_client_deregister(driver->client_handle);
    }
    if (driver->all_events_handled) {
        vSemaphoreDelete(driver->all_events_handled);
    }
    free(driver);
    return ret;
}

esp_err_t hid_host_uninstall(void)
{
    // Make sure hid driver is installed,
    // not being uninstalled from other task
    // and no hid device is registered

    // Flush Interface list if needed
    HID_RETURN_ON_ERROR(hid_host_interface_list_destroy(),
                        "Unable to flush Interfaces list");

    HID_ENTER_CRITICAL();
    HID_RETURN_ON_FALSE_CRITICAL( s_hid_driver != NULL, ESP_ERR_INVALID_STATE );
    HID_RETURN_ON_FALSE_CRITICAL( !s_hid_driver->end_client_event_handling, ESP_ERR_INVALID_STATE );
    HID_RETURN_ON_FALSE_CRITICAL( STAILQ_EMPTY(&devices_tailq), ESP_ERR_INVALID_STATE );
    HID_RETURN_ON_FALSE_CRITICAL( STAILQ_EMPTY(&ifaces_tailq), ESP_ERR_INVALID_STATE );
    s_hid_driver->end_client_event_handling = true;
    HID_EXIT_CRITICAL();

    xSemaphoreTake(s_hid_driver->all_events_handled, portMAX_DELAY);
    vSemaphoreDelete(s_hid_driver->all_events_handled);
    free(s_hid_driver);
    s_hid_driver = NULL;
    return ESP_OK;
}

esp_err_t hid_host_device_open(hid_host_device_handle_t hid_dev_handle,
                               const hid_host_device_config_t *config)
{
    HID_RETURN_ON_FALSE(s_hid_driver,
                        ESP_ERR_INVALID_STATE,
                        "HID Driver is not installed");

    hid_iface_t *hid_iface = get_iface_by_handle(hid_dev_handle);

    HID_RETURN_ON_INVALID_ARG(hid_iface);

    HID_RETURN_ON_FALSE((hid_iface->dev_params.proto >= HID_PROTOCOL_NONE)
                        && (hid_iface->dev_params.proto < HID_PROTOCOL_MAX),
                        ESP_ERR_INVALID_ARG,
                        "HID device protocol not supported");

    HID_RETURN_ON_FALSE((HID_INTERFACE_STATE_IDLE == hid_iface->state),
                        ESP_ERR_INVALID_STATE,
                        "Interface wrong state");

    // Claim interface, allocate xfer and save report callback
    HID_RETURN_ON_ERROR( hid_host_interface_claim_and_prepare_transfer(hid_iface),
                         "Unable to claim interface");

    // Save HID Interface callback
    hid_iface->user_cb = config->callback;
    hid_iface->user_cb_arg = config->callback_arg;

    return ESP_OK;
}

esp_err_t hid_host_device_close(hid_host_device_handle_t hid_dev_handle)
{
    hid_iface_t *hid_iface = get_iface_by_handle(hid_dev_handle);

    HID_RETURN_ON_INVALID_ARG(hid_iface);

    if (HID_INTERFACE_STATE_ACTIVE == hid_iface->state) {
        HID_RETURN_ON_ERROR( hid_host_disable_interface(hid_iface),
                             "Unable to disable HID Interface");
    }

    if (HID_INTERFACE_STATE_READY == hid_iface->state) {
        HID_RETURN_ON_ERROR( hid_host_interface_release_and_free_transfer(hid_iface),
                             "Unable to release HID Interface");

        // If the device is closing by user before device detached we need to flush user callback here
        free(hid_iface->report_desc);
        hid_iface->report_desc = NULL;
    }

    if (HID_INTERFACE_STATE_WAIT_USER_DELETION == hid_iface->state) {
        hid_iface->user_cb = NULL;
        hid_iface->user_cb_arg = NULL;

        /* Remove Interface from the list */
        HID_ENTER_CRITICAL();
        hid_iface->state = HID_INTERFACE_STATE_NOT_INITIALIZED;
        STAILQ_REMOVE(&ifaces_tailq, hid_iface, hid_interface, tailq_entry);
        HID_EXIT_CRITICAL();
        free(hid_iface);
    }

    return ESP_OK;
}

esp_err_t hid_host_handle_events(uint32_t timeout_ms)
{
    HID_RETURN_ON_FALSE(s_hid_driver,
                        ESP_ERR_INVALID_STATE,
                        "HID Driver is not installed");

    return usb_host_client_handle_events(s_hid_driver->client_handle, timeout_ms);
}

hid_host_dev_params_t *hid_host_device_get_params(hid_host_device_handle_t hid_dev_handle)
{
    hid_iface_t *iface = get_iface_by_handle(hid_dev_handle);

    return (iface) ? &iface->dev_params : NULL;
}

uint8_t *hid_host_device_get_data(hid_host_device_handle_t hid_dev_handle,
                                  unsigned int *data_length)
{
    hid_iface_t *iface = get_iface_by_handle(hid_dev_handle);

    if (NULL == iface) {
        return NULL;
    }

    *data_length = iface->in_xfer->actual_num_bytes;
    return iface->in_xfer->data_buffer;
}

// ------------------------ USB HID Host driver API ----------------------------

esp_err_t hid_host_device_start(hid_host_device_handle_t hid_dev_handle)
{
    hid_iface_t *iface = get_iface_by_handle(hid_dev_handle);

    HID_RETURN_ON_INVALID_ARG(iface);
    HID_RETURN_ON_INVALID_ARG(iface->in_xfer);
    HID_RETURN_ON_INVALID_ARG(iface->parent);

    HID_RETURN_ON_FALSE(is_interface_in_list(iface),
                        ESP_ERR_NOT_FOUND,
                        "Interface handle not found");

    HID_RETURN_ON_FALSE ((HID_INTERFACE_STATE_READY == iface->state),
                         ESP_ERR_INVALID_STATE,
                         "Interface wrong state");

    // prepare transfer
    iface->in_xfer->device_handle = iface->parent->dev_hdl;
    iface->in_xfer->callback = in_xfer_done;
    iface->in_xfer->context = iface->parent;
    iface->in_xfer->timeout_ms = DEFAULT_TIMEOUT_MS;
    iface->in_xfer->bEndpointAddress = iface->ep_in;
    iface->in_xfer->num_bytes = iface->ep_in_mps;

    iface->state = HID_INTERFACE_STATE_ACTIVE;

    // start data transfer
    return usb_host_transfer_submit(iface->in_xfer);
}

esp_err_t hid_host_device_stop(hid_host_device_handle_t hid_dev_handle)
{
    hid_iface_t *iface = get_iface_by_handle(hid_dev_handle);

    HID_RETURN_ON_INVALID_ARG(iface);

    return hid_host_disable_interface(iface);
}

uint8_t *hid_host_get_report_descriptor(hid_host_device_handle_t hid_dev_handle,
                                        size_t *report_desc_len)
{
    hid_iface_t *iface = get_iface_by_handle(hid_dev_handle);

    if (NULL == iface) {
        return NULL;
    }

    // Report Descriptor was already requested, return pointer
    if (iface->report_desc) {
        *report_desc_len = iface->report_desc_size;
        return iface->report_desc;
    }

    // Request Report Descriptor
    if (ESP_OK == hid_class_request_report_descriptor(iface)) {
        *report_desc_len = iface->report_desc_size;
        return iface->report_desc;
    }

    return NULL;
}

esp_err_t hid_class_request_get_report(hid_host_device_handle_t hid_dev_handle,
                                       uint8_t report_type,
                                       uint8_t report_id,
                                       uint8_t *report,
                                       size_t *report_length)
{
    hid_iface_t *iface = get_iface_by_handle(hid_dev_handle);

    HID_RETURN_ON_INVALID_ARG(iface);
    HID_RETURN_ON_INVALID_ARG(report);

    const usb_request_struct_t get_request = {
        .bRequest = HID_CLASS_SPECIFIC_REQ_GET_REPORT,
        .wValue = (report_type << 8) | report_id,
        .wIndex = iface->dev_params.iface_num,
        .wLength = *report_length,
        .data = report
    };

    return hid_class_request_get(iface->parent, &get_request, report_length);
}

esp_err_t hid_class_request_get_idle(hid_host_device_handle_t hid_dev_handle,
                                     uint8_t report_id,
                                     uint8_t *idle_rate)
{
    hid_iface_t *iface = get_iface_by_handle(hid_dev_handle);

    HID_RETURN_ON_INVALID_ARG(iface);
    HID_RETURN_ON_INVALID_ARG(idle_rate);

    uint8_t tmp[1] = { 0xff };

    const usb_request_struct_t get_request = {
        .bRequest = HID_CLASS_SPECIFIC_REQ_GET_IDLE,
        .wValue = report_id,
        .wIndex = iface->dev_params.iface_num,
        .wLength = 1,
        .data = tmp
    };

    HID_RETURN_ON_ERROR( hid_class_request_get(iface->parent, &get_request, NULL),
                         "HID class request transfer failure");

    *idle_rate = tmp[0];

    return ESP_OK;
}

esp_err_t hid_class_request_get_protocol(hid_host_device_handle_t hid_dev_handle,
        hid_report_protocol_t *protocol)
{
    hid_iface_t *iface = get_iface_by_handle(hid_dev_handle);

    HID_RETURN_ON_INVALID_ARG(iface);
    HID_RETURN_ON_INVALID_ARG(protocol);

    uint8_t tmp[1] = { 0xff };

    const usb_request_struct_t get_request = {
        .bRequest = HID_CLASS_SPECIFIC_REQ_GET_PROTOCOL,
        .wValue = 0,
        .wIndex = iface->dev_params.iface_num,
        .wLength = 1,
        .data = tmp
    };

    HID_RETURN_ON_ERROR( hid_class_request_get(iface->parent, &get_request, NULL),
                         "HID class request failure");

    *protocol = (hid_report_protocol_t) tmp[0];
    return ESP_OK;
}

esp_err_t hid_class_request_set_report(hid_host_device_handle_t hid_dev_handle,
                                       uint8_t report_type,
                                       uint8_t report_id,
                                       uint8_t *report,
                                       size_t report_length)
{
    hid_iface_t *iface = get_iface_by_handle(hid_dev_handle);

    HID_RETURN_ON_INVALID_ARG(iface);

    const usb_request_struct_t set_request = {
        .bRequest = HID_CLASS_SPECIFIC_REQ_SET_REPORT,
        .wValue = (report_type << 8) | report_id,
        .wIndex = iface->dev_params.iface_num,
        .wLength = report_length,
        .data = report
    };

    return hid_class_request_set(iface->parent, &set_request);
}

esp_err_t hid_class_request_set_idle(hid_host_device_handle_t hid_dev_handle,
                                     uint8_t duration,
                                     uint8_t report_id)
{
    hid_iface_t *iface = get_iface_by_handle(hid_dev_handle);

    HID_RETURN_ON_INVALID_ARG(iface);

    const usb_request_struct_t set_request = {
        .bRequest = HID_CLASS_SPECIFIC_REQ_SET_IDLE,
        .wValue = (duration << 8) | report_id,
        .wIndex = iface->dev_params.iface_num,
        .wLength = 0,
        .data = NULL
    };

    return hid_class_request_set(iface->parent, &set_request);
}

esp_err_t hid_class_request_set_protocol(hid_host_device_handle_t hid_dev_handle,
        hid_report_protocol_t protocol)
{
    hid_iface_t *iface = get_iface_by_handle(hid_dev_handle);

    HID_RETURN_ON_INVALID_ARG(iface);

    const usb_request_struct_t set_request = {
        .bRequest = HID_CLASS_SPECIFIC_REQ_SET_PROTOCOL,
        .wValue = protocol,
        .wIndex = iface->dev_params.iface_num,
        .wLength = 0,
        .data = NULL
    };

    return hid_class_request_set(iface->parent, &set_request);
}
