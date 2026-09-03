// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "midi.h"
#include "qmk_midi.h"

enum layers {
  SCALE_LAYER,
  CHORD_LAYER,
  LAYER_SELECT_LAYER,
  SETTINGS_LAYER,
};

enum scale_mode {
  SCALE_MAJOR,
  SCALE_NATURAL_MINOR,
  SCALE_HARMONIC_MINOR,
  SCALE_MELODIC_MINOR,
  SCALE_DORIAN,
  SCALE_PHRYGIAN,
  SCALE_LYDIAN,
  SCALE_MIXOLYDIAN,
  SCALE_LOCRIAN,
  SCALE_MAJOR_PENTATONIC,
  SCALE_MINOR_PENTATONIC,
  SCALE_BLUES,
  SCALE_WHOLE_TONE,
  SCALE_CHROMATIC,
  SCALE_DIMINISHED_HALF_WHOLE,
  SCALE_DIMINISHED_WHOLE_HALF,
  SCALE_BEBOP_MAJOR,
  SCALE_BEBOP_DOMINANT,
  SCALE_HUNGARIAN_MINOR,
  SCALE_JAPANESE_PENTATONIC,
  SCALE_MODE_COUNT,
};

enum custom_keycodes {
  MD_NOTE = SAFE_RANGE,
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
  MD_SCALE_SLOT_0,
  MD_SCALE_SLOT_1,
  MD_SCALE_SLOT_2,
  MD_SCALE_SLOT_3,
  MD_SCALE_SLOT_4,
  MD_SCALE_SLOT_5,
  MD_SCALE_SLOT_6,
  MD_SCALE_SLOT_7,
  MD_SCALE_SLOT_8,
  MD_SCALE_SLOT_9,
  MD_SCALE_SLOT_10,
  MD_SCALE_SLOT_11,
  MD_SCALE_SLOT_12,
  MD_SCALE_SLOT_13,
  MD_SCALE_SLOT_14,
  MD_SCALE_SLOT_15,
  MD_SCALE_SLOT_16,
  MD_SCALE_SLOT_17,
  MD_SCALE_PAGE_UP,
  MD_SCALE_PAGE_DOWN,
  MD_OCTAVE_DOWN,
  MD_OCTAVE_UP,
};

#define MD_N MD_NOTE
#define MD_SETTINGS MO(SETTINGS_LAYER)
#define MD_LAYERS TO(LAYER_SELECT_LAYER)
#define MD_SCALE TO(SCALE_LAYER)
#define MD_CHORD TO(CHORD_LAYER)
#define XXXXXXX KC_NO
#define _______ KC_TRNS

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [SCALE_LAYER] = LAYOUT_tkl_ansi(
        MD_OCTAVE_DOWN, MD_OCTAVE_UP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MD_SETTINGS,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N,
        MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N, MD_N),

    [CHORD_LAYER] = LAYOUT_tkl_ansi(
        MD_OCTAVE_DOWN, MD_OCTAVE_UP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MD_LAYERS,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),

    [LAYER_SELECT_LAYER] = LAYOUT_tkl_ansi(
        MD_SCALE, MD_CHORD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),

    [SETTINGS_LAYER] = LAYOUT_tkl_ansi(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MD_LAYERS, _______,
        MD_SCALE_SLOT_0, MD_SCALE_SLOT_1, MD_SCALE_SLOT_2, MD_SCALE_SLOT_3, MD_SCALE_SLOT_4, MD_SCALE_SLOT_5, MD_SCALE_SLOT_6, MD_SCALE_SLOT_7, MD_SCALE_SLOT_8, MD_SCALE_PAGE_UP,
        MD_SCALE_SLOT_9, MD_SCALE_SLOT_10, MD_SCALE_SLOT_11, MD_SCALE_SLOT_12, MD_SCALE_SLOT_13, MD_SCALE_SLOT_14, MD_SCALE_SLOT_15, MD_SCALE_SLOT_16, MD_SCALE_SLOT_17, MD_SCALE_PAGE_DOWN,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        MD_ROOT_C, MD_ROOT_CS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        MD_ROOT_D, MD_ROOT_DS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        MD_ROOT_E, MD_ROOT_F, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        MD_ROOT_FS, MD_ROOT_G, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        MD_ROOT_GS, MD_ROOT_A, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        MD_ROOT_AS, MD_ROOT_B, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),
};
// clang-format on

#undef MD_N
#undef MD_SETTINGS
#undef MD_LAYERS
#undef MD_SCALE
#undef MD_CHORD
#undef XXXXXXX
#undef _______

#define NO_ACTIVE_NOTE 0xFF
#define MIDI_MAX_NOTE 127
#define MIDI_PAD_CHANNEL 0
#define PITCH_CLASS_COUNT 12
#define SCALE_PAGE_SIZE 18
#define SCALE_PAGE_COUNT                                                       \
  ((SCALE_MODE_COUNT + SCALE_PAGE_SIZE - 1) / SCALE_PAGE_SIZE)
#define DISPLAY_ROW 4
#define DISPLAY_COL 3
#define DISPLAY_WIDTH 7
#define DISPLAY_HEIGHT 6
#define GLYPH_WIDTH 3
#define GLYPH_ADVANCE 4
#define SCALE_SCROLL_HOLD_MS 400
#define SCALE_SCROLL_STEP_MS 260
#define PAGE_ARROW_MS 250

typedef struct {
  uint8_t note_count;
  uint8_t intervals[PITCH_CLASS_COUNT];
} scale_definition_t;

static const scale_definition_t PROGMEM scale_definitions[SCALE_MODE_COUNT] = {
    [SCALE_MAJOR] = {7, {0, 2, 4, 5, 7, 9, 11}},
    [SCALE_NATURAL_MINOR] = {7, {0, 2, 3, 5, 7, 8, 10}},
    [SCALE_HARMONIC_MINOR] = {7, {0, 2, 3, 5, 7, 8, 11}},
    [SCALE_MELODIC_MINOR] = {7, {0, 2, 3, 5, 7, 9, 11}},
    [SCALE_DORIAN] = {7, {0, 2, 3, 5, 7, 9, 10}},
    [SCALE_PHRYGIAN] = {7, {0, 1, 3, 5, 7, 8, 10}},
    [SCALE_LYDIAN] = {7, {0, 2, 4, 6, 7, 9, 11}},
    [SCALE_MIXOLYDIAN] = {7, {0, 2, 4, 5, 7, 9, 10}},
    [SCALE_LOCRIAN] = {7, {0, 1, 3, 5, 6, 8, 10}},
    [SCALE_MAJOR_PENTATONIC] = {5, {0, 2, 4, 7, 9}},
    [SCALE_MINOR_PENTATONIC] = {5, {0, 3, 5, 7, 10}},
    [SCALE_BLUES] = {6, {0, 3, 5, 6, 7, 10}},
    [SCALE_WHOLE_TONE] = {6, {0, 2, 4, 6, 8, 10}},
    [SCALE_CHROMATIC] = {12, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}},
    [SCALE_DIMINISHED_HALF_WHOLE] = {8, {0, 1, 3, 4, 6, 7, 9, 10}},
    [SCALE_DIMINISHED_WHOLE_HALF] = {8, {0, 2, 3, 5, 6, 8, 9, 11}},
    [SCALE_BEBOP_MAJOR] = {8, {0, 2, 4, 5, 7, 8, 9, 11}},
    [SCALE_BEBOP_DOMINANT] = {8, {0, 2, 4, 5, 7, 9, 10, 11}},
    [SCALE_HUNGARIAN_MINOR] = {7, {0, 2, 3, 6, 7, 8, 11}},
    [SCALE_JAPANESE_PENTATONIC] = {5, {0, 1, 5, 7, 8}},
};

static const char PROGMEM scale_names[SCALE_MODE_COUNT][12] = {
    [SCALE_MAJOR] = "MAJOR",
    [SCALE_NATURAL_MINOR] = "MINOR",
    [SCALE_HARMONIC_MINOR] = "HARMONIC",
    [SCALE_MELODIC_MINOR] = "MELODIC",
    [SCALE_DORIAN] = "DORIAN",
    [SCALE_PHRYGIAN] = "PHRYGIAN",
    [SCALE_LYDIAN] = "LYDIAN",
    [SCALE_MIXOLYDIAN] = "MIXOLYDIAN",
    [SCALE_LOCRIAN] = "LOCRIAN",
    [SCALE_MAJOR_PENTATONIC] = "MAJOR PENT",
    [SCALE_MINOR_PENTATONIC] = "MINOR PENT",
    [SCALE_BLUES] = "BLUES",
    [SCALE_WHOLE_TONE] = "WHOLE TONE",
    [SCALE_CHROMATIC] = "CHROMATIC",
    [SCALE_DIMINISHED_HALF_WHOLE] = "HALF WHOLE",
    [SCALE_DIMINISHED_WHOLE_HALF] = "WHOLE HALF",
    [SCALE_BEBOP_MAJOR] = "BEBOP MAJOR",
    [SCALE_BEBOP_DOMINANT] = "BEBOP DOM",
    [SCALE_HUNGARIAN_MINOR] = "HUNGARIAN",
    [SCALE_JAPANESE_PENTATONIC] = "JAPANESE",
};

// TomTentacles is a fixed-width 3x6 CC0 font. Bits 7..5 are left to right.
// See THIRD_PARTY_NOTICES.md.
static const uint8_t PROGMEM font_3x6[26][DISPLAY_HEIGHT] = {
    ['A' - 'A'] = {0x40, 0xA0, 0xA0, 0xE0, 0xA0, 0xA0},
    ['B' - 'A'] = {0xC0, 0xA0, 0xC0, 0xA0, 0xA0, 0xC0},
    ['C' - 'A'] = {0x60, 0x80, 0x80, 0x80, 0x80, 0x60},
    ['D' - 'A'] = {0xC0, 0xA0, 0xA0, 0xA0, 0xA0, 0xC0},
    ['E' - 'A'] = {0xE0, 0x80, 0xC0, 0x80, 0x80, 0xE0},
    ['F' - 'A'] = {0xE0, 0x80, 0xC0, 0x80, 0x80, 0x80},
    ['G' - 'A'] = {0x60, 0x80, 0x80, 0xE0, 0xA0, 0x60},
    ['H' - 'A'] = {0xA0, 0xA0, 0xA0, 0xE0, 0xA0, 0xA0},
    ['I' - 'A'] = {0xE0, 0x40, 0x40, 0x40, 0x40, 0xE0},
    ['J' - 'A'] = {0x20, 0x20, 0x20, 0x20, 0xA0, 0x40},
    ['K' - 'A'] = {0xA0, 0xA0, 0xC0, 0xC0, 0xA0, 0xA0},
    ['L' - 'A'] = {0x80, 0x80, 0x80, 0x80, 0x80, 0xE0},
    ['M' - 'A'] = {0xA0, 0xE0, 0xE0, 0xA0, 0xA0, 0xA0},
    ['N' - 'A'] = {0xA0, 0xE0, 0xE0, 0xE0, 0xA0, 0xA0},
    ['O' - 'A'] = {0x40, 0xA0, 0xA0, 0xA0, 0xA0, 0x40},
    ['P' - 'A'] = {0xC0, 0xA0, 0xA0, 0xC0, 0x80, 0x80},
    ['Q' - 'A'] = {0x40, 0xA0, 0xA0, 0xA0, 0xE0, 0x60},
    ['R' - 'A'] = {0xC0, 0xA0, 0xA0, 0xC0, 0xA0, 0xA0},
    ['S' - 'A'] = {0x60, 0x80, 0xC0, 0x20, 0x20, 0xC0},
    ['T' - 'A'] = {0xE0, 0x40, 0x40, 0x40, 0x40, 0x40},
    ['U' - 'A'] = {0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0x40},
    ['V' - 'A'] = {0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0x40},
    ['W' - 'A'] = {0xA0, 0xA0, 0xA0, 0xE0, 0xE0, 0xA0},
    ['X' - 'A'] = {0xA0, 0xA0, 0x40, 0x40, 0xA0, 0xA0},
    ['Y' - 'A'] = {0xA0, 0xA0, 0x40, 0x40, 0x40, 0x40},
    ['Z' - 'A'] = {0xE0, 0x20, 0x40, 0x40, 0x80, 0xE0},
};

static const char PROGMEM pitch_class_letters[PITCH_CLASS_COUNT] = {
    'C', 'C', 'D', 'D', 'E', 'F', 'F', 'G', 'G', 'A', 'A', 'B',
};

enum display_mode {
  DISPLAY_ROOT,
  DISPLAY_SCALE,
};

enum page_arrow {
  PAGE_ARROW_NONE,
  PAGE_ARROW_UP,
  PAGE_ARROW_DOWN,
};

static uint8_t active_notes[MATRIX_ROWS][MATRIX_COLS];
static uint8_t active_note_counts[MIDI_MAX_NOTE + 1];
static uint8_t root_pitch_class = 4; // E
static uint8_t selected_scale = SCALE_NATURAL_MINOR;
static uint8_t scale_page = 0;
static uint8_t midi_velocity = MIDI_PAD_DEFAULT_VELOCITY;
static int8_t octave_shift = 0;
static enum display_mode settings_display = DISPLAY_ROOT;
static enum page_arrow transient_arrow = PAGE_ARROW_NONE;
static uint16_t scale_scroll_offset = DISPLAY_WIDTH;
static uint32_t scale_scroll_timer = 0;
static uint32_t page_arrow_timer = 0;
static bool scale_scroll_holding = false;

static uint8_t led_index(uint8_t row, uint8_t col) {
  return (row * MATRIX_COLS) + col;
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

static uint8_t clamp_midi_note(int16_t note) {
  if (note < 0) {
    return 0;
  }
  if (note > MIDI_MAX_NOTE) {
    return MIDI_MAX_NOTE;
  }
  return (uint8_t)note;
}

// Lowest notes are at the bottom-left. Moving right advances one scale degree;
// moving up advances three, giving repeatable shapes and deliberate duplicates.
static uint8_t note_for_position(uint8_t row, uint8_t col) {
  const uint8_t note_count =
      pgm_read_byte(&scale_definitions[selected_scale].note_count);
  const uint8_t scale_degree = col + ((MATRIX_ROWS - 1 - row) * 3);
  const uint8_t interval = pgm_read_byte(
      &scale_definitions[selected_scale].intervals[scale_degree % note_count]);
  const int16_t note = MIDI_PAD_BASE_C_NOTE + root_pitch_class +
                       ((scale_degree / note_count) * 12) + interval +
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

static void clear_page_arrow(void) { transient_arrow = PAGE_ARROW_NONE; }

static void select_root(uint8_t pitch_class) {
  release_tracked_notes();
  clear_page_arrow();
  root_pitch_class = pitch_class;
  settings_display = DISPLAY_ROOT;
}

static void select_scale(uint8_t scale) {
  release_tracked_notes();
  clear_page_arrow();
  selected_scale = scale;
  settings_display = DISPLAY_SCALE;
  scale_scroll_offset = DISPLAY_WIDTH;
  scale_scroll_timer = timer_read32();
  scale_scroll_holding = true;
}

static void change_scale_page(enum page_arrow direction) {
  if (direction == PAGE_ARROW_UP) {
    scale_page = (scale_page + SCALE_PAGE_COUNT - 1) % SCALE_PAGE_COUNT;
  } else {
    scale_page = (scale_page + 1) % SCALE_PAGE_COUNT;
  }
  transient_arrow = direction;
  page_arrow_timer = timer_read32();
}

void keyboard_post_init_user(void) {
  for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      active_notes[row][col] = NO_ACTIVE_NOTE;
    }
  }
}

layer_state_t layer_state_set_user(layer_state_t state) {
  if (get_highest_layer(state) != SCALE_LAYER) {
    release_tracked_notes();
  }
  return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  const uint8_t row = record->event.key.row;
  const uint8_t col = record->event.key.col;

  if (record->event.pressed && transient_arrow != PAGE_ARROW_NONE &&
      keycode != MD_SCALE_PAGE_UP && keycode != MD_SCALE_PAGE_DOWN) {
    clear_page_arrow();
  }

  switch (keycode) {
  case MD_NOTE:
    if (record->event.pressed) {
      press_note(row, col);
    } else {
      release_note(row, col);
    }
    return false;

  case MD_ROOT_C ... MD_ROOT_B:
    if (record->event.pressed) {
      select_root(keycode - MD_ROOT_C);
    }
    return false;

  case MD_SCALE_SLOT_0 ... MD_SCALE_SLOT_17:
    if (record->event.pressed) {
      const uint8_t scale =
          (scale_page * SCALE_PAGE_SIZE) + (keycode - MD_SCALE_SLOT_0);
      if (scale < SCALE_MODE_COUNT) {
        select_scale(scale);
      }
    }
    return false;

  case MD_SCALE_PAGE_UP:
    if (record->event.pressed) {
      change_scale_page(PAGE_ARROW_UP);
    }
    return false;

  case MD_SCALE_PAGE_DOWN:
    if (record->event.pressed) {
      change_scale_page(PAGE_ARROW_DOWN);
    }
    return false;

  case MD_OCTAVE_DOWN:
    if (record->event.pressed && octave_shift > -2) {
      release_tracked_notes();
      --octave_shift;
    }
    return false;

  case MD_OCTAVE_UP:
    if (record->event.pressed && octave_shift < 2) {
      release_tracked_notes();
      ++octave_shift;
    }
    return false;
  }

  return true;
}

static uint8_t scale_name_length(void) {
  uint8_t length = 0;
  while (length < sizeof(scale_names[0]) &&
         pgm_read_byte(&scale_names[selected_scale][length]) != '\0') {
    ++length;
  }
  return length;
}

static uint16_t scale_scroll_cycle_width(void) {
  return DISPLAY_WIDTH + (scale_name_length() * GLYPH_ADVANCE) - 1 +
         DISPLAY_WIDTH;
}

static void update_display_timers(void) {
  if (transient_arrow != PAGE_ARROW_NONE) {
    if (timer_elapsed32(page_arrow_timer) >= PAGE_ARROW_MS) {
      transient_arrow = PAGE_ARROW_NONE;
      scale_scroll_timer = timer_read32();
      scale_scroll_holding = false;
    }
    return;
  }

  if (settings_display != DISPLAY_SCALE) {
    return;
  }

  const uint16_t interval =
      scale_scroll_holding ? SCALE_SCROLL_HOLD_MS : SCALE_SCROLL_STEP_MS;
  if (timer_elapsed32(scale_scroll_timer) >= interval) {
    scale_scroll_offset =
        (scale_scroll_offset + 1) % scale_scroll_cycle_width();
    scale_scroll_timer = timer_read32();
    scale_scroll_holding = false;
  }
}

static bool font_pixel(char character, uint8_t row, uint8_t col) {
  if (character < 'A' || character > 'Z' || row >= DISPLAY_HEIGHT ||
      col >= GLYPH_WIDTH) {
    return false;
  }
  const uint8_t row_bits = pgm_read_byte(&font_3x6[character - 'A'][row]);
  return row_bits & (0x80 >> col);
}

static void draw_font_pixel(uint8_t row, uint8_t col, uint8_t red,
                            uint8_t green, uint8_t blue) {
  rgb_matrix_set_color(led_index(DISPLAY_ROW + row, DISPLAY_COL + col), red,
                       green, blue);
}

static void render_root_preview(void) {
  const char letter = pgm_read_byte(&pitch_class_letters[root_pitch_class]);
  for (uint8_t row = 0; row < DISPLAY_HEIGHT; ++row) {
    for (uint8_t col = 0; col < GLYPH_WIDTH; ++col) {
      if (font_pixel(letter, row, col)) {
        draw_font_pixel(row, col, 145, 52, 0);
      }
    }
  }

  if (is_black_key(root_pitch_class)) {
    // One blank column separates the 3x6 letter from this 2x2 marker.
    rgb_matrix_set_color(led_index(DISPLAY_ROW, DISPLAY_COL + 4), 0, 170, 255);
    rgb_matrix_set_color(led_index(DISPLAY_ROW, DISPLAY_COL + 5), 0, 170, 255);
    rgb_matrix_set_color(led_index(DISPLAY_ROW + 1, DISPLAY_COL + 4), 0, 170,
                         255);
    rgb_matrix_set_color(led_index(DISPLAY_ROW + 1, DISPLAY_COL + 5), 0, 170,
                         255);
  }
}

static void render_scale_preview(void) {
  const uint8_t name_length = scale_name_length();
  const uint16_t text_width = (name_length * GLYPH_ADVANCE) - 1;
  const uint16_t offset = scale_scroll_offset % scale_scroll_cycle_width();

  for (uint8_t view_col = 0; view_col < DISPLAY_WIDTH; ++view_col) {
    const int16_t source_col = (int16_t)offset + view_col - DISPLAY_WIDTH;
    if (source_col < 0 || source_col >= text_width) {
      continue;
    }

    const uint8_t letter_index = source_col / GLYPH_ADVANCE;
    const uint8_t glyph_col = source_col % GLYPH_ADVANCE;
    if (glyph_col >= GLYPH_WIDTH) {
      continue;
    }

    const char letter =
        pgm_read_byte(&scale_names[selected_scale][letter_index]);
    for (uint8_t row = 0; row < DISPLAY_HEIGHT; ++row) {
      if (font_pixel(letter, row, glyph_col)) {
        draw_font_pixel(row, view_col, 0, 150, 190);
      }
    }
  }
}

static void render_page_arrow(void) {
  static const uint8_t PROGMEM arrows[][DISPLAY_HEIGHT] = {
      [PAGE_ARROW_UP - 1] = {0x40, 0xE0, 0x40, 0x40, 0x40, 0x40},
      [PAGE_ARROW_DOWN - 1] = {0x40, 0x40, 0x40, 0x40, 0xE0, 0x40},
  };
  const uint8_t arrow_index = transient_arrow - 1;

  for (uint8_t row = 0; row < DISPLAY_HEIGHT; ++row) {
    const uint8_t row_bits = pgm_read_byte(&arrows[arrow_index][row]);
    for (uint8_t col = 0; col < GLYPH_WIDTH; ++col) {
      if (row_bits & (0x80 >> col)) {
        draw_font_pixel(row, col, 115, 60, 180);
      }
    }
  }
}

static void set_root_selector_led(uint8_t pitch_class) {
  const uint8_t row = DISPLAY_ROW + (pitch_class / 2);
  const uint8_t col = pitch_class % 2;
  const uint8_t led = led_index(row, col);
  if (pitch_class == root_pitch_class) {
    rgb_matrix_set_color(led, 110, 38, 0);
  } else {
    rgb_matrix_set_color(led, 45, 22, 4);
  }
}

static void set_scale_selector_led(uint8_t slot) {
  const uint8_t scale = (scale_page * SCALE_PAGE_SIZE) + slot;
  if (scale >= SCALE_MODE_COUNT) {
    return;
  }

  const uint8_t row = 1 + (slot / 9);
  const uint8_t col = slot % 9;
  if (scale == selected_scale) {
    rgb_matrix_set_color(led_index(row, col), 0, 110, 145);
  } else {
    rgb_matrix_set_color(led_index(row, col), 0, 28, 38);
  }
}

static void clear_all_leds(void) {
  for (uint8_t led = 0; led < RGB_MATRIX_LED_COUNT; ++led) {
    rgb_matrix_set_color(led, 0, 0, 0);
  }
}

static void render_settings_layer(void) {
  clear_all_leds();
  update_display_timers();

  rgb_matrix_set_color(led_index(0, 8), 0, 70, 90); // Layer select
  rgb_matrix_set_color(led_index(0, 9), 75, 0, 90); // Held Settings key

  for (uint8_t slot = 0; slot < SCALE_PAGE_SIZE; ++slot) {
    set_scale_selector_led(slot);
  }
  rgb_matrix_set_color(led_index(1, 9), 70, 35, 105); // Page up
  rgb_matrix_set_color(led_index(2, 9), 70, 35, 105); // Page down

  for (uint8_t pitch_class = 0; pitch_class < PITCH_CLASS_COUNT;
       ++pitch_class) {
    set_root_selector_led(pitch_class);
  }

  if (transient_arrow != PAGE_ARROW_NONE) {
    render_page_arrow();
  } else if (settings_display == DISPLAY_SCALE) {
    render_scale_preview();
  } else {
    render_root_preview();
  }
}

static void render_scale_layer(void) {
  clear_all_leds();
  rgb_matrix_set_color(led_index(0, 0), 0, 25, 80); // Octave down
  rgb_matrix_set_color(led_index(0, 1), 0, 25, 80); // Octave up
  rgb_matrix_set_color(led_index(0, 9), 75, 0, 90); // Settings

  for (uint8_t row = 1; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      const uint8_t note = note_for_position(row, col);
      const uint8_t led = led_index(row, col);
      if (active_note_counts[note] > 0) {
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

static void render_chord_layer(void) {
  clear_all_leds();
  rgb_matrix_set_color(led_index(0, 0), 0, 25, 80); // Octave down
  rgb_matrix_set_color(led_index(0, 1), 0, 25, 80); // Octave up
  rgb_matrix_set_color(led_index(0, 9), 0, 70, 90); // Layer select
}

static void render_layer_select(void) {
  clear_all_leds();
  rgb_matrix_set_color(led_index(0, 0), 0, 105, 50); // Scale
  rgb_matrix_set_color(led_index(0, 1), 35, 45, 65); // Chord placeholder
}

bool rgb_matrix_indicators_user(void) {
  switch (get_highest_layer(layer_state)) {
  case SETTINGS_LAYER:
    render_settings_layer();
    break;
  case LAYER_SELECT_LAYER:
    render_layer_select();
    break;
  case CHORD_LAYER:
    render_chord_layer();
    break;
  default:
    render_scale_layer();
    break;
  }
  return false;
}
