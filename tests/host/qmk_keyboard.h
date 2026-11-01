// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#define PROGMEM
#define pgm_read_byte(p) (*(const uint8_t *)(p))
#define MATRIX_ROWS 10
#define MATRIX_COLS 10
#define RGB_MATRIX_LED_COUNT 100
#define SAFE_RANGE 0x6000
#define KC_NO 0
#define KC_TRNS 1
#define MO(n) (0x100 + (n))
#define TO(n) (0x200 + (n))
// Layout wiring belongs to QMK; host tests invoke the keymap's event handlers.
#define LAYOUT_tkl_ansi(...) {{0}}
#include "config.h"
typedef uint32_t layer_state_t;
typedef struct { struct { struct { uint8_t row, col; } key; bool pressed; } event; } keyrecord_t;
extern layer_state_t layer_state;
extern volatile uint32_t host_now;
extern uint8_t host_leds[100][3];
extern bool host_keys[10][10];
extern uint64_t host_led_writes;
uint32_t timer_read32(void);
uint32_t timer_elapsed32(uint32_t since);
uint8_t get_highest_layer(layer_state_t state);
bool matrix_is_on(uint8_t row, uint8_t col);
void rgb_matrix_set_color(int index, uint8_t red, uint8_t green, uint8_t blue);
uint32_t eeconfig_read_user(void);
void eeconfig_update_user(uint32_t value);
void reset_keyboard(void);
void layer_move(uint8_t layer);

#define RGB_MATRIX_CUSTOM_MIDI_PAD_CANVAS 1
void rgb_matrix_mode_noeeprom(uint8_t mode);
