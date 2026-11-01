// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// Small host adapter for the QMK/ChibiOS lifecycle. Production compilation
// uses QMK's real types; descriptor structures and AT32 registers remain real
// in this test. Queue initialization records the payload size used by QMK.
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "usb_descriptor.h"
#include "at32_otg.h"

#define BQ_BUFFER_SIZE(n, size) (((size_t)(size) + sizeof(size_t)) * (n))
typedef int msg_t;
typedef enum {
    USB_EVENT_RESET, USB_EVENT_ADDRESS, USB_EVENT_CONFIGURED,
    USB_EVENT_UNCONFIGURED, USB_EVENT_SUSPEND, USB_EVENT_WAKEUP, USB_EVENT_STALLED
} usbevent_t;
typedef struct USBDriver USBDriver;
typedef void (*usbeventcb_t)(USBDriver *, usbevent_t);
typedef struct {
    usbeventcb_t event_cb;
    void (*get_descriptor_cb)(void);
    void (*requests_hook_cb)(void);
    void (*sof_cb)(void);
} USBConfig;
struct USBDriver {
    at32_otg_t *otg;
    const USBConfig *config;
};
extern USBDriver host_usb;
#define USB_DRIVER host_usb

typedef struct {
    size_t buffer_size;
    size_t buffer_capacity;
    uint8_t *buffer;
} usb_endpoint_config_t;
typedef struct {
    size_t payload_size;
} host_queue_t;
typedef struct {
    size_t in_maxsize;
    size_t out_maxsize;
} host_ep_config_t;
typedef struct {
    usb_endpoint_config_t config;
    host_ep_config_t ep_config;
    host_queue_t obqueue;
} usb_endpoint_in_t;
typedef struct {
    usb_endpoint_config_t config;
    host_ep_config_t ep_config;
    host_queue_t ibqueue;
} usb_endpoint_out_t;

void osalSysLockFromISR(void);
void osalSysUnlockFromISR(void);
void usb_endpoint_in_init(usb_endpoint_in_t *endpoint);
void usb_endpoint_out_init(usb_endpoint_out_t *endpoint);
msg_t usbStart(USBDriver *usbp, const USBConfig *config);
