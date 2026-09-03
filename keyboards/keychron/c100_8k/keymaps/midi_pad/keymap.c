// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "midi.h"
#include "qmk_midi.h"

enum layers {
  MIDI_LAYER,
  SETTINGS_LAYER,
};

enum scale_mode {
  SCALE_MAJOR,
  SCALE_NATURAL_MINOR,
};

enum custom_keycodes {
  MD_NOTE = SAFE_RANGE,
  MD_PANIC,
  MD_ROOT_C,
  MD_ROOT_CS,
  MD_ROOT_D,
  MD_ROOT_DS,
  MD_ROOT_E,
  MD_ROOT_F,
  MD_ROOT_FS,
  MD_ROOT_G,
  MD_ROOT_GS,
  MD_ROOT_A,
  MD_ROOT_AS,
  MD_ROOT_B,
  MD_SCALE_MAJOR,
  MD_SCALE_MINOR,
  MD_OCTAVE_DOWN,
  MD_OCTAVE_UP,
  MD_VELOCITY_DOWN,
  MD_VELOCITY_UP,
  MD_SUSTAIN,
};

#define MD_N MD_NOTE
#define MD_SETTINGS MO(SETTINGS_LAYER)

// The top row is reserved for layer navigation. Settings currently occupies
// the far-right slot; the other slots remain available for future modes.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MIDI_LAYER] = LAYOUT_tkl_ansi(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        MD_SETTINGS, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N),

    [SETTINGS_LAYER] = LAYOUT_tkl_ansi(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, MD_ROOT_C, MD_ROOT_CS, MD_ROOT_D, MD_ROOT_DS,
        MD_ROOT_E, MD_ROOT_F, MD_ROOT_FS, MD_ROOT_G, MD_ROOT_GS, MD_ROOT_A,
        MD_ROOT_AS, MD_ROOT_B, MD_SCALE_MAJOR, MD_SCALE_MINOR, MD_OCTAVE_DOWN,
        MD_OCTAVE_UP, MD_VELOCITY_DOWN, MD_VELOCITY_UP, MD_SUSTAIN, MD_PANIC,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
};

#undef MD_N
#undef MD_SETTINGS

#define NO_ACTIVE_NOTE 0xFF
#define MIDI_CHANNEL_COUNT 16
#define MIDI_MAX_NOTE 127
#define MIDI_PAD_CHANNEL 0
#define SCALE_NOTE_COUNT 7
#define PITCH_CLASS_COUNT 12

static const uint8_t scale_intervals[][SCALE_NOTE_COUNT] = {
    [SCALE_MAJOR] = {0, 2, 4, 5, 7, 9, 11},
    [SCALE_NATURAL_MINOR] = {0, 2, 3, 5, 7, 8, 10},
};

static uint8_t active_notes[MATRIX_ROWS][MATRIX_COLS];
static uint8_t active_note_counts[MIDI_MAX_NOTE + 1];
static uint8_t root_pitch_class = 4; // E
static uint8_t selected_scale = SCALE_NATURAL_MINOR;
static uint8_t midi_velocity = MIDI_PAD_DEFAULT_VELOCITY;
static int8_t octave_shift = 0;
static bool sustain_active = false;

static uint8_t clamp_midi_note(int16_t note) {
  if (note < 0) {
    return 0;
  }
  if (note > MIDI_MAX_NOTE) {
    return MIDI_MAX_NOTE;
  }
  return (uint8_t)note;
}

// Lowest notes are at the bottom-left. Moving right advances one scale degree.
// Moving up advances three scale degrees (a fourth), producing repeatable
// interval and chord shapes with deliberately duplicated pitches.
static uint8_t note_for_position(uint8_t row, uint8_t col) {
  const uint8_t scale_degree = col + ((MATRIX_ROWS - 1 - row) * 3);
  const int16_t note =
      MIDI_PAD_BASE_C_NOTE + root_pitch_class +
      ((scale_degree / SCALE_NOTE_COUNT) * 12) +
      scale_intervals[selected_scale][scale_degree % SCALE_NOTE_COUNT] +
      (octave_shift * 12);
  return clamp_midi_note(note);
}

static void press_note(uint8_t row, uint8_t col) {
  if (active_notes[row][col] != NO_ACTIVE_NOTE) {
    return;
  }

  const uint8_t note = note_for_position(row, col);
  active_notes[row][col] = note;
  if (active_note_counts[note]++ == 0) {
    midi_send_noteon(&midi_device, MIDI_PAD_CHANNEL, note, midi_velocity);
  }
}

static void release_note(uint8_t row, uint8_t col) {
  const uint8_t note = active_notes[row][col];
  if (note == NO_ACTIVE_NOTE) {
    return;
  }

  active_notes[row][col] = NO_ACTIVE_NOTE;
  if (active_note_counts[note] > 0 && --active_note_counts[note] == 0) {
    midi_send_noteoff(&midi_device, MIDI_PAD_CHANNEL, note, 0);
  }
}

static void release_tracked_notes(void) {
  for (uint16_t note = 0; note <= MIDI_MAX_NOTE; ++note) {
    if (active_note_counts[note] > 0) {
      midi_send_noteoff(&midi_device, MIDI_PAD_CHANNEL, note, 0);
      active_note_counts[note] = 0;
    }
  }

  for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      active_notes[row][col] = NO_ACTIVE_NOTE;
    }
  }
}

static void midi_panic(void) {
  release_tracked_notes();
  for (uint8_t channel = 0; channel < MIDI_CHANNEL_COUNT; ++channel) {
    midi_send_cc(&midi_device, channel, 64, 0);  // Sustain off
    midi_send_cc(&midi_device, channel, 120, 0); // All sound off
    midi_send_cc(&midi_device, channel, 123, 0); // All notes off
  }
  sustain_active = false;
}

static void prepare_layout_change(void) { midi_panic(); }

void keyboard_post_init_user(void) {
  for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      active_notes[row][col] = NO_ACTIVE_NOTE;
    }
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  const uint8_t row = record->event.key.row;
  const uint8_t col = record->event.key.col;

  switch (keycode) {
  case MD_NOTE:
    if (record->event.pressed) {
      press_note(row, col);
    } else {
      release_note(row, col);
    }
    return false;

  case MD_PANIC:
    if (record->event.pressed) {
      midi_panic();
    }
    return false;

  case MD_ROOT_C ... MD_ROOT_B:
    if (record->event.pressed) {
      prepare_layout_change();
      root_pitch_class = keycode - MD_ROOT_C;
    }
    return false;

  case MD_SCALE_MAJOR:
    if (record->event.pressed) {
      prepare_layout_change();
      selected_scale = SCALE_MAJOR;
    }
    return false;

  case MD_SCALE_MINOR:
    if (record->event.pressed) {
      prepare_layout_change();
      selected_scale = SCALE_NATURAL_MINOR;
    }
    return false;

  case MD_OCTAVE_DOWN:
    if (record->event.pressed && octave_shift > -2) {
      prepare_layout_change();
      --octave_shift;
    }
    return false;

  case MD_OCTAVE_UP:
    if (record->event.pressed && octave_shift < 2) {
      prepare_layout_change();
      ++octave_shift;
    }
    return false;

  case MD_VELOCITY_DOWN:
    if (record->event.pressed) {
      midi_velocity = midi_velocity > 16 ? midi_velocity - 16 : 1;
    }
    return false;

  case MD_VELOCITY_UP:
    if (record->event.pressed) {
      midi_velocity = midi_velocity < 112 ? midi_velocity + 16 : 127;
    }
    return false;

  case MD_SUSTAIN:
    if (record->event.pressed) {
      sustain_active = !sustain_active;
      midi_send_cc(&midi_device, MIDI_PAD_CHANNEL, 64,
                   sustain_active ? 127 : 0);
    }
    return false;
  }

  return true;
}

static bool is_black_key(uint8_t pitch_class) {
  switch (pitch_class % PITCH_CLASS_COUNT) {
  case 1:
  case 3:
  case 6:
  case 8:
  case 10:
    return true;
  default:
    return false;
  }
}

enum note_glyph {
  GLYPH_A,
  GLYPH_B,
  GLYPH_C,
  GLYPH_D,
  GLYPH_E,
  GLYPH_F,
  GLYPH_G,
};

// Five columns by seven rows. The low five bits describe one LED row.
static const uint8_t PROGMEM note_glyphs[][7] = {
    [GLYPH_A] = {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11},
    [GLYPH_B] = {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E},
    [GLYPH_C] = {0x0F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0F},
    [GLYPH_D] = {0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E},
    [GLYPH_E] = {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F},
    [GLYPH_F] = {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10},
    [GLYPH_G] = {0x0F, 0x10, 0x10, 0x17, 0x11, 0x11, 0x0F},
};

static const uint8_t PROGMEM sharp_glyph[7] = {
    0x05, 0x05, 0x07, 0x05, 0x07, 0x05, 0x05,
};

static const uint8_t PROGMEM pitch_class_glyphs[PITCH_CLASS_COUNT] = {
    GLYPH_C, GLYPH_C, GLYPH_D, GLYPH_D, GLYPH_E, GLYPH_F,
    GLYPH_F, GLYPH_G, GLYPH_G, GLYPH_A, GLYPH_A, GLYPH_B,
};

static bool pitch_class_is_sharp(uint8_t pitch_class) {
  return is_black_key(pitch_class);
}

static void render_root_preview(void) {
  const bool is_sharp = pitch_class_is_sharp(root_pitch_class);
  const uint8_t letter_start = is_sharp ? 0 : 2;
  const uint8_t glyph = pgm_read_byte(&pitch_class_glyphs[root_pitch_class]);

  // Rows 0-2 remain available for navigation and settings controls. A natural
  // note is centered; a sharp note uses five columns, a gap, and three columns.
  for (uint8_t glyph_row = 0; glyph_row < 7; ++glyph_row) {
    const uint8_t letter_bits = pgm_read_byte(&note_glyphs[glyph][glyph_row]);
    for (uint8_t glyph_col = 0; glyph_col < 5; ++glyph_col) {
      if (letter_bits & (1 << (4 - glyph_col))) {
        const uint8_t led =
            (glyph_row + 3) * MATRIX_COLS + letter_start + glyph_col;
        rgb_matrix_set_color(led, 145, 52, 0);
      }
    }

    if (is_sharp) {
      const uint8_t sharp_bits = pgm_read_byte(&sharp_glyph[glyph_row]);
      for (uint8_t glyph_col = 0; glyph_col < 3; ++glyph_col) {
        if (sharp_bits & (1 << (2 - glyph_col))) {
          const uint8_t led = (glyph_row + 3) * MATRIX_COLS + 6 + glyph_col;
          rgb_matrix_set_color(led, 0, 170, 255);
        }
      }
    }
  }
}

static void set_root_selector_led(uint8_t led, uint8_t pitch_class) {
  if (pitch_class == root_pitch_class) {
    rgb_matrix_set_color(led, 110, 38, 0);
  } else if (is_black_key(pitch_class)) {
    rgb_matrix_set_color(led, 8, 12, 35);
  } else {
    rgb_matrix_set_color(led, 45, 45, 38);
  }
}

static void render_settings_layer(void) {
  for (uint8_t led = 0; led < RGB_MATRIX_LED_COUNT; ++led) {
    rgb_matrix_set_color(led, 0, 0, 0);
  }

  // Root selectors: C through A on row 2, then A# and B on row 3.
  for (uint8_t pitch_class = 0; pitch_class < 10; ++pitch_class) {
    set_root_selector_led(10 + pitch_class, pitch_class);
  }
  set_root_selector_led(20, 10);
  set_root_selector_led(21, 11);

  rgb_matrix_set_color(22, selected_scale == SCALE_MAJOR ? 0 : 30,
                       selected_scale == SCALE_MAJOR ? 100 : 0, 45);
  rgb_matrix_set_color(23, selected_scale == SCALE_NATURAL_MINOR ? 0 : 30,
                       selected_scale == SCALE_NATURAL_MINOR ? 100 : 0, 45);
  rgb_matrix_set_color(24, 0, 0, 70);  // Octave down
  rgb_matrix_set_color(25, 0, 0, 70);  // Octave up
  rgb_matrix_set_color(26, 70, 35, 0); // Velocity down
  rgb_matrix_set_color(27, 70, 35, 0); // Velocity up
  rgb_matrix_set_color(28, 0, sustain_active ? 255 : 70,
                       sustain_active ? 100 : 45);
  rgb_matrix_set_color(29, 90, 0, 0); // Panic

  // Keep the held Settings key visibly distinct.
  rgb_matrix_set_color(9, 75, 0, 90);
  render_root_preview();
}

static void render_midi_layer(void) {
  for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
    rgb_matrix_set_color(col, 0, 0, 0);
  }
  rgb_matrix_set_color(9, 35, 0, 45); // Settings

  for (uint8_t row = 1; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      const uint8_t led = row * MATRIX_COLS + col;
      const uint8_t note = note_for_position(row, col);

      if (active_note_counts[note] > 0) {
        // All duplicate pads for the sounding pitch light together.
        // Cyan-blue complements the warm amber root-note marker.
        rgb_matrix_set_color(led, 0, 170, 255);
      } else if (note % PITCH_CLASS_COUNT == root_pitch_class) {
        rgb_matrix_set_color(led, 110, 38, 0);
      } else if (is_black_key(note)) {
        rgb_matrix_set_color(led, 8, 12, 35);
      } else {
        rgb_matrix_set_color(led, 45, 45, 38);
      }
    }
  }
}

bool rgb_matrix_indicators_user(void) {
  if (get_highest_layer(layer_state) == SETTINGS_LAYER) {
    render_settings_layer();
  } else {
    render_midi_layer();
  }
  return false;
}
