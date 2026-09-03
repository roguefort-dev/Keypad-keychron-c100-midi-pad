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
  MD_PALETTE_PREV,
  MD_PALETTE_NEXT,
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
        MD_PALETTE_PREV, MD_PALETTE_NEXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MD_LAYERS, _______,
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
#define TEXT_SCROLL_HOLD_MS 400
#define TEXT_SCROLL_STEP_MS 260
#define PAGE_ARROW_MS 250
#define SETTINGS_UNSELECTED_BRIGHTNESS 180
#define PERFORMANCE_PAD_BRIGHTNESS 100
#define PERFORMANCE_NAV_BRIGHTNESS 200
#define MIDI_PAD_CONFIG_VERSION 0xA2

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

enum palette_id {
  PALETTE_NEON,
  PALETTE_CYBERPUNK,
  PALETTE_TERMINAL,
  PALETTE_NAVY,
  PALETTE_MATRIX,
  PALETTE_COUNT,
};

enum palette_color {
  COLOR_PRIMARY,
  COLOR_SECONDARY,
  COLOR_TERTIARY,
  COLOR_QUATERNARY,
  COLOR_QUINARY,
  PALETTE_COLOR_COUNT,
};

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} palette_color_t;

typedef union {
  uint32_t raw;
  struct {
    uint8_t version;
    uint8_t root_pitch_class;
    uint8_t selected_scale;
    uint8_t selected_palette;
  } values;
} midi_pad_config_t;

// Palettes are five ordered source colors. Each view decides how to compose
// them; brightness is applied separately from the palette itself.
static const palette_color_t PROGMEM
    palettes[PALETTE_COUNT][PALETTE_COLOR_COUNT] = {
        [PALETTE_NEON] =
            {
                [COLOR_PRIMARY] = {0x00, 0x2A, 0xFF},
                [COLOR_SECONDARY] = {0x00, 0xFB, 0xFF},
                [COLOR_TERTIARY] = {0xAE, 0x00, 0xFF},
                [COLOR_QUATERNARY] = {0xFF, 0x00, 0xEA},
                [COLOR_QUINARY] = {0xFF, 0x00, 0x33},
            },
        [PALETTE_CYBERPUNK] =
            {
                [COLOR_PRIMARY] = {0xFF, 0x00, 0xAE},
                [COLOR_SECONDARY] = {0xFF, 0x73, 0x00},
                [COLOR_TERTIARY] = {0xB3, 0xFF, 0x00},
                [COLOR_QUATERNARY] = {0xFF, 0xD5, 0x00},
                [COLOR_QUINARY] = {0xFF, 0x00, 0x6F},
            },
        [PALETTE_TERMINAL] =
            {
                [COLOR_PRIMARY] = {0x00, 0xB3, 0xFF},
                [COLOR_SECONDARY] = {0xFF, 0xA2, 0x00},
                [COLOR_TERTIARY] = {0xFF, 0x00, 0x00},
                [COLOR_QUATERNARY] = {0x1F, 0xFF, 0x62},
                [COLOR_QUINARY] = {0xFB, 0xFF, 0x00},
            },
        [PALETTE_NAVY] =
            {
                [COLOR_PRIMARY] = {0x00, 0x9D, 0xFF},
                [COLOR_SECONDARY] = {0xFF, 0x73, 0x00},
                [COLOR_TERTIARY] = {0xFF, 0xFF, 0x00},
                [COLOR_QUATERNARY] = {0xBE, 0x00, 0xFF},
                [COLOR_QUINARY] = {0x00, 0xFF, 0x5A},
            },
        [PALETTE_MATRIX] =
            {
                [COLOR_PRIMARY] = {0x00, 0xFF, 0xCC},
                [COLOR_SECONDARY] = {0xB3, 0xFF, 0x00},
                [COLOR_TERTIARY] = {0xFF, 0x00, 0xC8},
                [COLOR_QUATERNARY] = {0xFF, 0x00, 0x00},
                [COLOR_QUINARY] = {0xFF, 0x95, 0x00},
            },
};

static const char PROGMEM palette_names[PALETTE_COUNT][12] = {
    [PALETTE_NEON] = "NEON",         [PALETTE_CYBERPUNK] = "CYBERPUNK",
    [PALETTE_TERMINAL] = "TERMINAL", [PALETTE_NAVY] = "NAVY",
    [PALETTE_MATRIX] = "MATRIX",
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

enum scroll_source {
  SCROLL_SCALE_NAME,
  SCROLL_PALETTE_NAME,
};

static uint8_t active_notes[MATRIX_ROWS][MATRIX_COLS];
static uint8_t active_note_counts[MIDI_MAX_NOTE + 1];
static uint8_t root_pitch_class = 4; // E
static uint8_t selected_scale = SCALE_NATURAL_MINOR;
static uint8_t scale_page = 0;
static uint8_t selected_palette = PALETTE_TERMINAL;
static uint8_t midi_velocity = MIDI_PAD_DEFAULT_VELOCITY;
static int8_t octave_shift = 0;
static midi_pad_config_t persistent_config;
static enum display_mode settings_display = DISPLAY_ROOT;
static enum page_arrow transient_arrow = PAGE_ARROW_NONE;
static enum scroll_source text_scroll_source = SCROLL_SCALE_NAME;
static uint16_t text_scroll_offset = DISPLAY_WIDTH;
static uint32_t text_scroll_timer = 0;
static uint32_t page_arrow_timer = 0;
static bool text_scroll_holding = false;
static bool palette_announcement = false;

static void set_default_persistent_config(void) {
  persistent_config.raw = 0;
  persistent_config.values.version = MIDI_PAD_CONFIG_VERSION;
  persistent_config.values.root_pitch_class = 4; // E
  persistent_config.values.selected_scale = SCALE_NATURAL_MINOR;
  persistent_config.values.selected_palette = PALETTE_TERMINAL;
}

static bool persistent_config_is_valid(void) {
  return persistent_config.values.version == MIDI_PAD_CONFIG_VERSION &&
         persistent_config.values.root_pitch_class < PITCH_CLASS_COUNT &&
         persistent_config.values.selected_scale < SCALE_MODE_COUNT &&
         persistent_config.values.selected_palette < PALETTE_COUNT;
}

static void save_persistent_selection(void) {
  midi_pad_config_t updated = persistent_config;
  updated.values.version = MIDI_PAD_CONFIG_VERSION;
  updated.values.root_pitch_class = root_pitch_class;
  updated.values.selected_scale = selected_scale;
  updated.values.selected_palette = selected_palette;

  if (updated.raw != persistent_config.raw) {
    persistent_config = updated;
    eeconfig_update_user(persistent_config.raw);
  }
}

static uint8_t led_index(uint8_t row, uint8_t col) {
  return (row * MATRIX_COLS) + col;
}

static uint8_t scale_component(uint8_t component, uint8_t brightness) {
  return ((uint16_t)component * brightness) / 255;
}

static void set_palette_color(uint8_t led, enum palette_color color_index,
                              uint8_t brightness) {
  const palette_color_t *color = &palettes[selected_palette][color_index];
  const uint8_t red = pgm_read_byte(&color->red);
  const uint8_t green = pgm_read_byte(&color->green);
  const uint8_t blue = pgm_read_byte(&color->blue);
  rgb_matrix_set_color(led, scale_component(red, brightness),
                       scale_component(green, brightness),
                       scale_component(blue, brightness));
}

static void set_key_color(uint8_t row, uint8_t col,
                          enum palette_color color_index, uint8_t brightness) {
  set_palette_color(led_index(row, col), color_index, brightness);
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

static void start_text_scroll(enum scroll_source source) {
  text_scroll_source = source;
  text_scroll_offset = DISPLAY_WIDTH;
  text_scroll_timer = timer_read32();
  text_scroll_holding = true;
}

static void cancel_palette_announcement(void) { palette_announcement = false; }

static void select_root(uint8_t pitch_class) {
  release_tracked_notes();
  clear_page_arrow();
  cancel_palette_announcement();
  root_pitch_class = pitch_class;
  save_persistent_selection();
  settings_display = DISPLAY_ROOT;
}

static void select_scale(uint8_t scale) {
  release_tracked_notes();
  clear_page_arrow();
  cancel_palette_announcement();
  selected_scale = scale;
  save_persistent_selection();
  settings_display = DISPLAY_SCALE;
  start_text_scroll(SCROLL_SCALE_NAME);
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

static void change_palette(int8_t amount) {
  clear_page_arrow();
  selected_palette =
      (selected_palette + amount + PALETTE_COUNT) % PALETTE_COUNT;
  save_persistent_selection();
  palette_announcement = true;
  start_text_scroll(SCROLL_PALETTE_NAME);
}

void eeconfig_init_user(void) {
  set_default_persistent_config();
  eeconfig_update_user(persistent_config.raw);
}

void keyboard_post_init_user(void) {
  persistent_config.raw = eeconfig_read_user();
  if (!persistent_config_is_valid()) {
    eeconfig_init_user();
  }

  root_pitch_class = persistent_config.values.root_pitch_class;
  selected_scale = persistent_config.values.selected_scale;
  selected_palette = persistent_config.values.selected_palette;
  scale_page = selected_scale / SCALE_PAGE_SIZE;

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

  case MD_PALETTE_PREV:
    if (record->event.pressed) {
      change_palette(-1);
    }
    return false;

  case MD_PALETTE_NEXT:
    if (record->event.pressed) {
      change_palette(1);
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

static uint8_t text_capacity(enum scroll_source source) {
  return source == SCROLL_PALETTE_NAME ? sizeof(palette_names[0])
                                       : sizeof(scale_names[0]);
}

static char text_character(enum scroll_source source, uint8_t index) {
  if (source == SCROLL_PALETTE_NAME) {
    return pgm_read_byte(&palette_names[selected_palette][index]);
  }
  return pgm_read_byte(&scale_names[selected_scale][index]);
}

static uint8_t text_length(enum scroll_source source) {
  uint8_t length = 0;
  while (length < text_capacity(source) &&
         text_character(source, length) != '\0') {
    ++length;
  }
  return length;
}

static uint16_t text_width(enum scroll_source source) {
  return (text_length(source) * GLYPH_ADVANCE) - 1;
}

static uint16_t text_scroll_cycle_width(enum scroll_source source) {
  return DISPLAY_WIDTH + text_width(source) + DISPLAY_WIDTH;
}

static bool text_scroll_is_visible(void) {
  return palette_announcement || settings_display == DISPLAY_SCALE;
}

static void update_display_timers(void) {
  if (transient_arrow != PAGE_ARROW_NONE) {
    if (timer_elapsed32(page_arrow_timer) >= PAGE_ARROW_MS) {
      transient_arrow = PAGE_ARROW_NONE;
      text_scroll_timer = timer_read32();
    }
    return;
  }

  if (!text_scroll_is_visible()) {
    return;
  }

  const uint16_t interval =
      text_scroll_holding ? TEXT_SCROLL_HOLD_MS : TEXT_SCROLL_STEP_MS;
  if (timer_elapsed32(text_scroll_timer) < interval) {
    return;
  }

  ++text_scroll_offset;
  text_scroll_timer = timer_read32();
  text_scroll_holding = false;

  // A palette announcement ends after its final column leaves the display.
  if (palette_announcement && text_scroll_source == SCROLL_PALETTE_NAME &&
      text_scroll_offset >= DISPLAY_WIDTH + text_width(text_scroll_source)) {
    palette_announcement = false;
    if (settings_display == DISPLAY_SCALE) {
      start_text_scroll(SCROLL_SCALE_NAME);
    }
    return;
  }

  text_scroll_offset %= text_scroll_cycle_width(text_scroll_source);
}

static bool font_pixel(char character, uint8_t row, uint8_t col) {
  if (character < 'A' || character > 'Z' || row >= DISPLAY_HEIGHT ||
      col >= GLYPH_WIDTH) {
    return false;
  }
  const uint8_t row_bits = pgm_read_byte(&font_3x6[character - 'A'][row]);
  return row_bits & (0x80 >> col);
}

static void draw_font_pixel(uint8_t row, uint8_t col,
                            enum palette_color color_index,
                            uint8_t brightness) {
  set_key_color(DISPLAY_ROW + row, DISPLAY_COL + col, color_index, brightness);
}

static void render_root_preview(void) {
  const char letter = pgm_read_byte(&pitch_class_letters[root_pitch_class]);
  for (uint8_t row = 0; row < DISPLAY_HEIGHT; ++row) {
    for (uint8_t col = 0; col < GLYPH_WIDTH; ++col) {
      if (font_pixel(letter, row, col)) {
        draw_font_pixel(row, col, COLOR_TERTIARY, 255);
      }
    }
  }

  if (is_black_key(root_pitch_class)) {
    // One blank column separates the 3x6 letter from this 2x2 marker.
    set_key_color(DISPLAY_ROW, DISPLAY_COL + 4, COLOR_SECONDARY, 255);
    set_key_color(DISPLAY_ROW, DISPLAY_COL + 5, COLOR_SECONDARY, 255);
    set_key_color(DISPLAY_ROW + 1, DISPLAY_COL + 4, COLOR_SECONDARY, 255);
    set_key_color(DISPLAY_ROW + 1, DISPLAY_COL + 5, COLOR_SECONDARY, 255);
  }
}

static void render_scrolling_text(void) {
  const uint16_t width = text_width(text_scroll_source);
  const uint16_t offset =
      text_scroll_offset % text_scroll_cycle_width(text_scroll_source);

  for (uint8_t view_col = 0; view_col < DISPLAY_WIDTH; ++view_col) {
    const int16_t source_col = (int16_t)offset + view_col - DISPLAY_WIDTH;
    if (source_col < 0 || source_col >= width) {
      continue;
    }

    const uint8_t letter_index = source_col / GLYPH_ADVANCE;
    const uint8_t glyph_col = source_col % GLYPH_ADVANCE;
    if (glyph_col >= GLYPH_WIDTH) {
      continue;
    }

    const char letter = text_character(text_scroll_source, letter_index);
    for (uint8_t row = 0; row < DISPLAY_HEIGHT; ++row) {
      if (font_pixel(letter, row, glyph_col)) {
        draw_font_pixel(row, view_col, COLOR_TERTIARY, 255);
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
        draw_font_pixel(row, col, COLOR_QUATERNARY, 255);
      }
    }
  }
}

static void set_root_selector_led(uint8_t pitch_class) {
  const uint8_t row = DISPLAY_ROW + (pitch_class / 2);
  const uint8_t col = pitch_class % 2;
  const uint8_t led = led_index(row, col);
  if (pitch_class == root_pitch_class) {
    set_palette_color(led, COLOR_TERTIARY, 255);
  } else {
    set_palette_color(led, COLOR_PRIMARY, SETTINGS_UNSELECTED_BRIGHTNESS);
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
    set_key_color(row, col, COLOR_SECONDARY, 255);
  } else {
    set_key_color(row, col, COLOR_PRIMARY, SETTINGS_UNSELECTED_BRIGHTNESS);
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

  set_key_color(0, 0, COLOR_QUATERNARY, 255); // Previous palette
  set_key_color(0, 1, COLOR_QUINARY, 255);    // Next palette

  // Five passive swatches make the palette ordering visible in Settings.
  for (uint8_t color_index = 0; color_index < PALETTE_COLOR_COUNT;
       ++color_index) {
    set_key_color(0, 2 + color_index, color_index, 255);
  }

  set_key_color(0, 8, COLOR_QUINARY, 255);    // Layer select
  set_key_color(0, 9, COLOR_QUATERNARY, 255); // Held Settings key

  for (uint8_t slot = 0; slot < SCALE_PAGE_SIZE; ++slot) {
    set_scale_selector_led(slot);
  }
  set_key_color(1, 9, COLOR_QUATERNARY, 255); // Page up
  set_key_color(2, 9, COLOR_QUINARY, 255);    // Page down

  for (uint8_t pitch_class = 0; pitch_class < PITCH_CLASS_COUNT;
       ++pitch_class) {
    set_root_selector_led(pitch_class);
  }

  if (transient_arrow != PAGE_ARROW_NONE) {
    render_page_arrow();
  } else if (text_scroll_is_visible()) {
    render_scrolling_text();
  } else {
    render_root_preview();
  }
}

static void render_scale_layer(void) {
  clear_all_leds();
  set_key_color(0, 0, COLOR_QUATERNARY,
                PERFORMANCE_NAV_BRIGHTNESS); // Octave down
  set_key_color(0, 1, COLOR_QUINARY,
                PERFORMANCE_NAV_BRIGHTNESS); // Octave up
  set_key_color(0, 9, COLOR_QUATERNARY,
                PERFORMANCE_NAV_BRIGHTNESS); // Settings

  for (uint8_t row = 1; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      const uint8_t note = note_for_position(row, col);
      const uint8_t led = led_index(row, col);
      if (active_note_counts[note] > 0) {
        set_palette_color(led, COLOR_SECONDARY, 255);
      } else if (note % PITCH_CLASS_COUNT == root_pitch_class) {
        set_palette_color(led, COLOR_TERTIARY, 255);
      } else {
        set_palette_color(led, COLOR_PRIMARY, PERFORMANCE_PAD_BRIGHTNESS);
      }
    }
  }
}

static void render_chord_layer(void) {
  clear_all_leds();
  set_key_color(0, 0, COLOR_QUATERNARY,
                PERFORMANCE_NAV_BRIGHTNESS); // Octave down
  set_key_color(0, 1, COLOR_QUINARY,
                PERFORMANCE_NAV_BRIGHTNESS);  // Octave up
  set_key_color(0, 9, COLOR_QUATERNARY, 255); // Layer select
}

static void render_layer_select(void) {
  clear_all_leds();
  set_key_color(0, 0, COLOR_PRIMARY, 255);   // Scale
  set_key_color(0, 1, COLOR_SECONDARY, 255); // Chord placeholder
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
