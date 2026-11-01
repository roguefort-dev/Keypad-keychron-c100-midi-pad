// SPDX-License-Identifier: GPL-2.0-or-later
#include "qmk_keyboard.h"
#include "midi.h"
#include <assert.h>
layer_state_t layer_state = 1;
volatile uint32_t host_now = 1000;
uint8_t host_leds[100][3];
bool host_keys[10][10];
uint64_t host_led_writes;
uint64_t host_midi_hash = 1469598103934665603ULL;
uint64_t host_midi_events;
MidiDevice midi_device;
static uint32_t config;
uint32_t timer_read32(void) { return host_now; }
uint32_t timer_elapsed32(uint32_t since) { return host_now - since; }
uint8_t get_highest_layer(layer_state_t state) {
  return state ? 31 - __builtin_clz(state) : 0;
}
bool matrix_is_on(uint8_t row, uint8_t col) { return host_keys[row][col]; }
void rgb_matrix_set_color(int index, uint8_t r, uint8_t g, uint8_t b) {
  assert(index >= 0 && index < 100);
  host_leds[index][0] = r;
  host_leds[index][1] = g;
  host_leds[index][2] = b;
  ++host_led_writes;
}
uint32_t eeconfig_read_user(void) { return config; }
void eeconfig_update_user(uint32_t value) { config = value; }
void reset_keyboard(void) {}
extern layer_state_t layer_state_set_user(layer_state_t);
void layer_move(uint8_t layer) {
  layer_state = layer_state_set_user((layer_state_t)1 << layer);
}
static void midi_event(uint8_t kind, uint8_t channel, uint8_t note, uint8_t velocity) {
  assert(note <= 127);
  uint8_t bytes[] = {kind, channel, note, velocity};
  for (unsigned i = 0; i < sizeof(bytes); ++i)
    host_midi_hash = (host_midi_hash ^ bytes[i]) * 1099511628211ULL;
  uint32_t now = host_now;
  for (unsigned i = 0; i < 4; ++i)
    host_midi_hash = (host_midi_hash ^ ((now >> (8 * i)) & 255)) * 1099511628211ULL;
  ++host_midi_events;
}
void midi_send_noteon(MidiDevice *d, uint8_t c, uint8_t n, uint8_t v) {
  (void)d; midi_event(0x90, c, n, v);
}
void midi_send_noteoff(MidiDevice *d, uint8_t c, uint8_t n, uint8_t v) {
  (void)d; midi_event(0x80, c, n, v);
}
void midi_register_realtime_callback(MidiDevice *d, void (*cb)(MidiDevice *, uint8_t)) { (void)d; (void)cb; }
void midi_register_sysex_callback(MidiDevice *d, void (*cb)(MidiDevice *, uint16_t, uint8_t, uint8_t *)) { (void)d; (void)cb; }

void rgb_matrix_mode_noeeprom(uint8_t mode) { (void)mode; }
