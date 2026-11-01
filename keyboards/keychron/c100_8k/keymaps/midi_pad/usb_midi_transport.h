// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stdint.h>

#define MIDI_USB_FULL_SPEED_SIZE 64
#define MIDI_USB_HIGH_SPEED_SIZE 512

uint16_t midi_usb_packet_size(void);
