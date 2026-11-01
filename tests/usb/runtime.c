// SPDX-License-Identifier: GPL-2.0-or-later
#include "usb_driver.h"

unsigned host_usb_starts;

// Keep the underlying function in a separate translation unit so the linker
// must resolve calls through --wrap exactly as in the firmware.
msg_t usbStart(USBDriver *usbp, const USBConfig *config) {
    ++host_usb_starts;
    usbp->config = config;
    return 123;
}
