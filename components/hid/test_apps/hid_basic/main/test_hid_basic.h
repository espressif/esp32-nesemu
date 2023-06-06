/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "usb/hid_host.h"

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------ HID Test -------------------------------------------

void test_hid_setup(hid_host_driver_event_cb_t device_callback);

void test_hid_teardown(void);

#ifdef __cplusplus
}
#endif //__cplusplus
