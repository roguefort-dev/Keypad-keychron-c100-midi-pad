// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later

#include <hal.h>

#include "usb_driver.h"
#include "usb_endpoints.h"
#include "usb_midi_transport.h"

extern usb_endpoint_in_t usb_endpoints_in[USB_ENDPOINT_IN_COUNT];
extern usb_endpoint_out_t usb_endpoints_out[USB_ENDPOINT_OUT_COUNT];

// Allocate enough for high speed before QMK initializes its queues. Queue
// payloads still match the negotiated packet size, including 64-byte full
// speed OUT transfers that must complete without waiting for another packet.
static _Alignas(4) uint8_t midi_in_buffer[
    BQ_BUFFER_SIZE(MIDI_STREAM_IN_CAPACITY, MIDI_USB_HIGH_SPEED_SIZE)];
static _Alignas(4) uint8_t midi_out_buffer[
    BQ_BUFFER_SIZE(MIDI_STREAM_OUT_CAPACITY, MIDI_USB_HIGH_SPEED_SIZE)];

void keyboard_pre_init_user(void) {
    usb_endpoints_in[USB_ENDPOINT_IN_MIDI].config.buffer = midi_in_buffer;
    usb_endpoints_out[USB_ENDPOINT_OUT_MIDI].config.buffer = midi_out_buffer;
}

uint16_t midi_usb_packet_size(void) {
    return (USB_DRIVER.otg->DSTS & DSTS_ENUMSPD_MASK) == DSTS_ENUMSPD_HS_480
               ? MIDI_USB_HIGH_SPEED_SIZE
               : MIDI_USB_FULL_SPEED_SIZE;
}

static USBConfig midi_usb_config;
static usbeventcb_t original_event_cb;

static void midi_usb_event(USBDriver *usbp, usbevent_t event) {
    if (event == USB_EVENT_CONFIGURED) {
        usb_endpoint_in_t *in = &usb_endpoints_in[USB_ENDPOINT_IN_MIDI];
        usb_endpoint_out_t *out = &usb_endpoints_out[USB_ENDPOINT_OUT_MIDI];
        const uint16_t size = midi_usb_packet_size();

        // QMK resets/suspends queues on reset or unconfiguration before this
        // callback. Reinitialize only when speed changes, before its original
        // callback configures endpoint hardware and resumes the queues.
        osalSysLockFromISR();
        in->ep_config.in_maxsize = size;
        out->ep_config.out_maxsize = size;
        if (in->config.buffer_size != size) {
            in->config.buffer_size = size;
            usb_endpoint_in_init(in);
        }
        if (out->config.buffer_size != size) {
            out->config.buffer_size = size;
            usb_endpoint_out_init(out);
        }
        osalSysUnlockFromISR();
    }
    if (original_event_cb) original_event_cb(usbp, event);
}

// Wrap the public start function so the event adapter is installed before USB
// connects, and again on QMK driver restarts. Preserve all existing callbacks.
// No descriptor request is required before a host reuses a configuration.
msg_t __real_usbStart(USBDriver *usbp, const USBConfig *config);
msg_t __wrap_usbStart(USBDriver *usbp, const USBConfig *config) {
    if (usbp != &USB_DRIVER) return __real_usbStart(usbp, config);
    if (config != &midi_usb_config) {
        midi_usb_config = *config;
        original_event_cb = config->event_cb;
        midi_usb_config.event_cb = midi_usb_event;
    }
    return __real_usbStart(usbp, &midi_usb_config);
}
