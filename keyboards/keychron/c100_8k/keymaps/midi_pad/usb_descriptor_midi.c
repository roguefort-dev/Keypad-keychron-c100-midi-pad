// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later

#include <string.h>

#include "usb_descriptor.h"
#include "usb_midi_transport.h"

extern USB_Descriptor_Configuration_t ConfigurationDescriptor;
extern const USB_Descriptor_Device_t DeviceDescriptor;

static void set_midi_packet_size(USB_Descriptor_Configuration_t *descriptor,
                                 uint16_t size) {
    descriptor->MIDI_In_Jack_Endpoint.Endpoint.EndpointSize = size;
    descriptor->MIDI_Out_Jack_Endpoint.Endpoint.EndpointSize = size;
}

// Keychron's MIDI configuration includes an Audio Interface Association
// Descriptor, but its device descriptor advertises IAD support only for CDC.
// Windows needs EF/02/01 to group the Audio Control and MIDI Streaming
// interfaces. Preserve QMK's identity and interface layout while also fixing
// bulk packet sizes for the C100's negotiated high/full speed.
void get_usb_descriptor_kb(const uint16_t wValue, const uint16_t wIndex,
                           const uint16_t wLength,
                           const void **const DescriptorAddress,
                           uint16_t *size) {
    (void)wIndex;
    (void)wLength;
    const uint8_t type = wValue >> 8;
    if (type == DTYPE_Configuration || type == DTYPE_Other) {
        static USB_Descriptor_Configuration_t configuration;
        static USB_Descriptor_Configuration_t other_speed;
        USB_Descriptor_Configuration_t *descriptor =
            type == DTYPE_Configuration ? &configuration : &other_speed;
        memcpy(descriptor, &ConfigurationDescriptor, sizeof(*descriptor));
        descriptor->Config.Header.Type = type;
        const uint16_t current_size = midi_usb_packet_size();
        set_midi_packet_size(descriptor,
            type == DTYPE_Configuration ? current_size :
            current_size == MIDI_USB_HIGH_SPEED_SIZE ?
                MIDI_USB_FULL_SPEED_SIZE : MIDI_USB_HIGH_SPEED_SIZE);
        *DescriptorAddress = descriptor;
        *size = sizeof(*descriptor);
    } else if (type == DTYPE_DeviceQualifier) {
        static USB_Descriptor_DeviceQualifier_t qualifier;
        qualifier = (USB_Descriptor_DeviceQualifier_t){
            .Header = {sizeof(qualifier), DTYPE_DeviceQualifier},
            .USBSpecification = DeviceDescriptor.USBSpecification,
            .Class = USB_CSCP_IADDeviceClass,
            .SubClass = USB_CSCP_IADDeviceSubclass,
            .Protocol = USB_CSCP_IADDeviceProtocol,
            .Endpoint0Size = DeviceDescriptor.Endpoint0Size,
            .NumberOfConfigurations = DeviceDescriptor.NumberOfConfigurations,
        };
        *DescriptorAddress = &qualifier;
        *size = sizeof(qualifier);
    } else if (type == DTYPE_Device &&
               *size == sizeof(USB_Descriptor_Device_t) && *DescriptorAddress) {
        // Static storage must outlive the control transfer. QMK/ChibiOS clips
        // responses to wLength, including the initial eight-byte request.
        static USB_Descriptor_Device_t descriptor;
        memcpy(&descriptor, *DescriptorAddress, sizeof(descriptor));
        descriptor.Class = USB_CSCP_IADDeviceClass;
        descriptor.SubClass = USB_CSCP_IADDeviceSubclass;
        descriptor.Protocol = USB_CSCP_IADDeviceProtocol;
        *DescriptorAddress = &descriptor;
    }
}
