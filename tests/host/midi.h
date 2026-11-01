// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <stdint.h>
typedef struct { uint8_t unused; } MidiDevice;
extern MidiDevice midi_device;
#define MIDI_START 0xFA
#define MIDI_CONTINUE 0xFB
#define MIDI_STOP 0xFC
#define MIDI_CLOCK 0xF8
#define SYSEX_BEGIN 0xF0
#define SYSEX_END 0xF7
void midi_send_noteon(MidiDevice *, uint8_t, uint8_t, uint8_t);
void midi_send_noteoff(MidiDevice *, uint8_t, uint8_t, uint8_t);
void midi_register_realtime_callback(MidiDevice *, void (*)(MidiDevice *, uint8_t));
void midi_register_sysex_callback(MidiDevice *, void (*)(MidiDevice *, uint16_t, uint8_t, uint8_t *));
