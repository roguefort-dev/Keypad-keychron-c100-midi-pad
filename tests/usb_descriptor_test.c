// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "usb_descriptor.h"
#include "usb_driver.h"
#include "usb_endpoints.h"
#include "usb_midi_transport.h"

extern const USB_Descriptor_Device_t DeviceDescriptor;
extern USB_Descriptor_Configuration_t ConfigurationDescriptor;
extern const USB_Descriptor_String_t LanguageString;
extern const USB_Descriptor_String_t ManufacturerString;
extern const USB_Descriptor_String_t ProductString;
extern const USB_Descriptor_HIDReport_Datatype_t KeyboardReport[];
extern const USB_Descriptor_HIDReport_Datatype_t SharedReport[];

static at32_otg_t otg;
USBDriver host_usb = {.otg = &otg};
usb_endpoint_in_t usb_endpoints_in[USB_ENDPOINT_IN_COUNT];
usb_endpoint_out_t usb_endpoints_out[USB_ENDPOINT_OUT_COUNT];
static unsigned locks, in_inits, out_inits, forwarded_events;
extern unsigned host_usb_starts;
static uint16_t expected_packet_size;

void osalSysLockFromISR(void) { assert(locks++ == 0); }
void osalSysUnlockFromISR(void) { assert(--locks == 0); }
void usb_endpoint_in_init(usb_endpoint_in_t *endpoint) {
    ++in_inits;
    endpoint->obqueue.payload_size = endpoint->config.buffer_size;
}
void usb_endpoint_out_init(usb_endpoint_out_t *endpoint) {
    ++out_inits;
    endpoint->ibqueue.payload_size = endpoint->config.buffer_size;
}
static void original_event(USBDriver *usbp, usbevent_t event) {
    assert(usbp == &host_usb && locks == 0);
    ++forwarded_events;
    if (event != USB_EVENT_CONFIGURED) return;
    // QMK's original event callback initializes hardware and starts receiving
    // immediately. Both endpoint and queue sizes must already be correct.
    assert(usb_endpoints_in[USB_ENDPOINT_IN_MIDI].ep_config.in_maxsize == expected_packet_size);
    assert(usb_endpoints_out[USB_ENDPOINT_OUT_MIDI].ep_config.out_maxsize == expected_packet_size);
    assert(usb_endpoints_in[USB_ENDPOINT_IN_MIDI].obqueue.payload_size == expected_packet_size);
    assert(usb_endpoints_out[USB_ENDPOINT_OUT_MIDI].ibqueue.payload_size == expected_packet_size);
}
static void unchanged_callback(void) {}

static void expect_configuration(bool other, uint16_t packet_size) {
    const void *address = NULL;
    USB_Descriptor_Configuration_t expected = ConfigurationDescriptor;
    expected.Config.Header.Type = other ? 7 : 2;
    expected.MIDI_In_Jack_Endpoint.Endpoint.EndpointSize = packet_size;
    expected.MIDI_Out_Jack_Endpoint.Endpoint.EndpointSize = packet_size;
    assert(get_usb_descriptor(other ? 0x0700 : 0x0200, 0, 9, &address) == sizeof(expected));
    assert(memcmp(address, &expected, sizeof(expected)) == 0);
    const void *full = NULL;
    assert(get_usb_descriptor(other ? 0x0700 : 0x0200, 0, UINT16_MAX, &full) == sizeof(expected));
    assert(full == address);
    assert(memcmp(full, &expected, sizeof(expected)) == 0);
}

void keyboard_pre_init_user(void);
static void test_transport(void) {
    usb_endpoint_in_t *in = &usb_endpoints_in[USB_ENDPOINT_IN_MIDI];
    usb_endpoint_out_t *out = &usb_endpoints_out[USB_ENDPOINT_OUT_MIDI];
    in->config.buffer_size = out->config.buffer_size = 64;
    in->config.buffer_capacity = MIDI_STREAM_IN_CAPACITY;
    out->config.buffer_capacity = MIDI_STREAM_OUT_CAPACITY;
    keyboard_pre_init_user();
    assert(in->config.buffer && out->config.buffer);
    assert((uintptr_t)in->config.buffer % 4 == 0);
    assert((uintptr_t)out->config.buffer % 4 == 0);
    // Exercise the full backing allocation under ASan, including queue headers.
    memset(in->config.buffer, 0xA5, BQ_BUFFER_SIZE(MIDI_STREAM_IN_CAPACITY, 512));
    memset(out->config.buffer, 0x5A, BQ_BUFFER_SIZE(MIDI_STREAM_OUT_CAPACITY, 512));
    usb_endpoint_in_init(in);
    usb_endpoint_out_init(out);
    USBConfig config = {original_event, unchanged_callback,
                        unchanged_callback, unchanged_callback};
    assert(usbStart(&host_usb, &config) == 123);
    assert(host_usb.config != &config);
    assert(host_usb.config->get_descriptor_cb == config.get_descriptor_cb);
    assert(host_usb.config->requests_hook_cb == config.requests_hook_cb);
    assert(host_usb.config->sof_cb == config.sof_cb);
    const uint32_t speeds[] = {DSTS_ENUMSPD_HS_480, DSTS_ENUMSPD_HS_480,
                              DSTS_ENUMSPD_FS_30_60, DSTS_ENUMSPD_FS_48,
                              DSTS_ENUMSPD_HS_480};
    for (unsigned i = 0; i < sizeof(speeds) / sizeof(speeds[0]); ++i) {
        host_usb.config->event_cb(&host_usb, USB_EVENT_RESET);
        otg.DSTS = speeds[i];
        expected_packet_size = speeds[i] == DSTS_ENUMSPD_HS_480 ? 512 : 64;
        unsigned before = in_inits;
        size_t previous = in->config.buffer_size;
        // Deliberately configure before fetching any descriptors: a host may
        // reuse a cached configuration after reset or a driver restart.
        host_usb.config->event_cb(&host_usb, USB_EVENT_CONFIGURED);
        assert(in_inits == before + (previous != expected_packet_size));
        assert(in_inits == out_inits);
        expect_configuration(false, expected_packet_size);
        expect_configuration(true, expected_packet_size == 512 ? 64 : 512);
    }
    host_usb.config->event_cb(&host_usb, USB_EVENT_SUSPEND);
    host_usb.config->event_cb(&host_usb, USB_EVENT_WAKEUP);
    host_usb.config->event_cb(&host_usb, USB_EVENT_UNCONFIGURED);
    assert(forwarded_events == 13);
    // A wrapped config is safe to reuse, and starting with QMK's original
    // config again also preserves the original callback without recursion.
    assert(usbStart(&host_usb, host_usb.config) == 123);
    assert(usbStart(&host_usb, &config) == 123);
    host_usb.config->event_cb(&host_usb, USB_EVENT_RESET);
    assert(forwarded_events == 14 && host_usb_starts == 3);
    USBDriver other = {0};
    assert(usbStart(&other, &config) == 123);
    assert(other.config == &config);
}

static void expect_descriptor(uint16_t value, uint16_t index, uint16_t length,
                              const void *expected, uint16_t expected_size) {
    const void *address = NULL;
    assert(get_usb_descriptor(value, index, length, &address) == expected_size);
    assert(address == expected);
}

int main(void) {
    test_transport();
    _Static_assert(sizeof(USB_Descriptor_Device_t) == 18, "USB device layout");
    USB_Descriptor_Device_t original = DeviceDescriptor;
    USB_Descriptor_Device_t expected = original;
    expected.Class = 0xEF;
    expected.SubClass = 0x02;
    expected.Protocol = 0x01;

    // Exercise QMK's real resolver and strong-hook linkage, including the
    // initial short probe. The USB transport, rather than the hook, truncates
    // the full returned descriptor to the setup packet's requested length.
    const uint16_t lengths[] = {0, 1, 8, 18, 64, UINT16_MAX};
    const void *device = NULL;
    for (unsigned i = 0; i < sizeof(lengths) / sizeof(lengths[0]); ++i) {
        const void *address = NULL;
        assert(get_usb_descriptor(0x0100, 0, lengths[i], &address) == 18);
        assert(address != &DeviceDescriptor);
        assert(memcmp(address, &expected, sizeof(expected)) == 0);
        if (device) assert(device == address);
        device = address;
    }
    assert(memcmp(&DeviceDescriptor, &original, sizeof(original)) == 0);

    USB_Descriptor_Configuration_t configuration = ConfigurationDescriptor;
    expect_configuration(false, 512);
    expect_configuration(true, 64);
    const void *qualifier_address = NULL;
    assert(get_usb_descriptor(0x0600, 0, 10, &qualifier_address) == 10);
    const USB_Descriptor_DeviceQualifier_t *qualifier = qualifier_address;
    assert(qualifier->Header.Size == 10 && qualifier->Header.Type == 6);
    assert(qualifier->Class == 0xEF && qualifier->SubClass == 2 && qualifier->Protocol == 1);
    assert(qualifier->USBSpecification == original.USBSpecification);
    assert(qualifier->Endpoint0Size == original.Endpoint0Size);
    assert(qualifier->NumberOfConfigurations == original.NumberOfConfigurations);
    assert(qualifier->Reserved == 0);

    // The advertised IAD must still describe the contiguous AC/AS interfaces,
    // and HID, strings, endpoints and all class-specific MIDI data stay QMK's.
    const USB_Descriptor_Interface_Association_t *iad =
        &ConfigurationDescriptor.Audio_Interface_Association;
    assert(iad->Header.Type == 0x0B);
    assert(iad->TotalInterfaces == 2);
    assert(iad->FirstInterfaceIndex == AC_INTERFACE);
    assert(iad->Class == 1 && iad->SubClass == 1 && iad->Protocol == 0);
    assert(ConfigurationDescriptor.Audio_ControlInterface.InterfaceNumber == AC_INTERFACE);
    assert(ConfigurationDescriptor.Audio_StreamInterface.InterfaceNumber == AS_INTERFACE);
    assert(AS_INTERFACE == AC_INTERFACE + 1);
    assert(ConfigurationDescriptor.Audio_StreamInterface.TotalEndpoints == 2);

    expect_descriptor(0x0300, 0, 2, &LanguageString, LanguageString.Header.Size);
    expect_descriptor(0x0301, 0x0409, UINT16_MAX, &ManufacturerString,
                      ManufacturerString.Header.Size);
    expect_descriptor(0x0302, 0x0409, UINT16_MAX, &ProductString,
                      ProductString.Header.Size);
    expect_descriptor(0x2100, KEYBOARD_INTERFACE, UINT16_MAX,
                      &ConfigurationDescriptor.Keyboard_HID,
                      sizeof(ConfigurationDescriptor.Keyboard_HID));
    expect_descriptor(0x2200, KEYBOARD_INTERFACE, UINT16_MAX, KeyboardReport,
                      ConfigurationDescriptor.Keyboard_HID.HIDReportLength);
    expect_descriptor(0x2100, SHARED_INTERFACE, UINT16_MAX,
                      &ConfigurationDescriptor.Shared_HID,
                      sizeof(ConfigurationDescriptor.Shared_HID));
    expect_descriptor(0x2200, SHARED_INTERFACE, UINT16_MAX, SharedReport,
                      ConfigurationDescriptor.Shared_HID.HIDReportLength);
    expect_descriptor(0xFFFF, 0, UINT16_MAX, NULL, NO_DESCRIPTOR);
    assert(memcmp(&ConfigurationDescriptor, &configuration, sizeof(configuration)) == 0);
    assert(memcmp(device, &expected, sizeof(expected)) == 0);
    puts("USB: IAD class, HS/FS descriptors and queues, resets/restarts, callback forwarding and HID/string passthrough passed");
}
