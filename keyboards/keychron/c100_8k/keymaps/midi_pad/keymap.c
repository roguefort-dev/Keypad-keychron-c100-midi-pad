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
  CH_INV,
  CH_BASS,
  CH_DROP,
  CH_AUTO,
  CH_OPEN,
  CH_STRUM,
  CH_LATCH,
  CH_QUANTIZE,
  CH_DEGREE_1,
  CH_DEGREE_2,
  CH_DEGREE_3,
  CH_DEGREE_4,
  CH_DEGREE_5,
  CH_DEGREE_6,
  CH_DEGREE_7,
  CH_DEGREE_8,
  CH_DEGREE_9,
  CH_DEGREE_10,
  CH_DEGREE_11,
  CH_DEGREE_12,
  CH_DEGREE_13,
  CH_DEGREE_14,
  CH_SHAPE_0,
  CH_SHAPE_1,
  CH_SHAPE_2,
  CH_SHAPE_3,
  CH_SHAPE_4,
  CH_SHAPE_5,
  CH_SHAPE_6,
  CH_SHAPE_7,
  CH_SHAPE_8,
  CH_SHAPE_9,
  CH_SHAPE_10,
  CH_SHAPE_11,
  CH_SHAPE_12,
  CH_SHAPE_13,
  CH_SHAPE_14,
  CH_SHAPE_15,
  CH_SHAPE_16,
  CH_SHAPE_17,
  CH_SHAPE_18,
  CH_SHAPE_19,
  CH_SHAPE_20,
  CH_SHAPE_21,
  CH_SHAPE_22,
  CH_SHAPE_23,
  CH_SHAPE_24,
  CH_SHAPE_25,
  CH_SHAPE_26,
  CH_SHAPE_27,
  CH_SHAPE_28,
  CH_SHAPE_29,
  CH_SHAPE_30,
  CH_SHAPE_31,
  CH_SHAPE_32,
  CH_SHAPE_33,
  CH_SHAPE_34,
  CH_ROOT,
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
        MD_OCTAVE_DOWN, MD_OCTAVE_UP, XXXXXXX, MD_SCALE, MD_CHORD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MD_SETTINGS,
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
        MD_OCTAVE_DOWN, MD_OCTAVE_UP, XXXXXXX, MD_SCALE, MD_CHORD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MD_SETTINGS,
        CH_AUTO, CH_LATCH, CH_QUANTIZE, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        CH_INV, CH_BASS, CH_DROP, CH_OPEN, CH_STRUM, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        CH_DEGREE_1, CH_DEGREE_2, CH_DEGREE_3, CH_DEGREE_4, CH_DEGREE_5, CH_DEGREE_6, CH_DEGREE_7, CH_ROOT, CH_ROOT, CH_ROOT,
        CH_DEGREE_8, CH_DEGREE_9, CH_DEGREE_10, CH_DEGREE_11, CH_DEGREE_12, CH_DEGREE_13, CH_DEGREE_14, CH_ROOT, CH_ROOT, CH_ROOT,
        CH_SHAPE_0, CH_SHAPE_1, CH_SHAPE_2, CH_SHAPE_3, CH_SHAPE_4, CH_SHAPE_5, CH_SHAPE_6, CH_ROOT, CH_ROOT, CH_ROOT,
        CH_SHAPE_7, CH_SHAPE_8, CH_SHAPE_9, CH_SHAPE_10, CH_SHAPE_11, CH_SHAPE_12, CH_SHAPE_13, CH_ROOT, CH_ROOT, CH_ROOT,
        CH_SHAPE_14, CH_SHAPE_15, CH_SHAPE_16, CH_SHAPE_17, CH_SHAPE_18, CH_SHAPE_19, CH_SHAPE_20, CH_ROOT, CH_ROOT, CH_ROOT,
        CH_SHAPE_21, CH_SHAPE_22, CH_SHAPE_23, CH_SHAPE_24, CH_SHAPE_25, CH_SHAPE_26, CH_SHAPE_27, CH_ROOT, CH_ROOT, CH_ROOT,
        CH_SHAPE_28, CH_SHAPE_29, CH_SHAPE_30, CH_SHAPE_31, CH_SHAPE_32, CH_SHAPE_33, CH_SHAPE_34, CH_ROOT, CH_ROOT, CH_ROOT),

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
#define DEFAULT_PAD_BRIGHTNESS 180
#define ROOT_NOTE_BRIGHTNESS 225
#define PRESSED_PAD_BRIGHTNESS 255
#define SETTINGS_UNSELECTED_BRIGHTNESS DEFAULT_PAD_BRIGHTNESS
#define PERFORMANCE_PAD_BRIGHTNESS DEFAULT_PAD_BRIGHTNESS
#define PERFORMANCE_NAV_BRIGHTNESS DEFAULT_PAD_BRIGHTNESS
#define MIDI_PAD_CONFIG_VERSION 0xA2
#define CHORD_ROOT_FIRST_ROW 3
#define CHORD_ROOT_FIRST_COL 7
#define CHORD_ROOT_ROW_COUNT 7
#define CHORD_ROOT_COL_COUNT 3
#define CHORD_ROOT_SLOT_COUNT (CHORD_ROOT_ROW_COUNT * CHORD_ROOT_COL_COUNT)
#define CHORD_DEGREE_COUNT 14
#define CHORD_PRESET_COUNT 35
#define CHORD_MAX_TONES 15
#define CHORD_PENDING_EVENT_COUNT 64
#define CHORD_CONTROL_HOLD_MS 1000
#define CHORD_DISPLAY_MS 450
#define CHORD_RESET_DISPLAY_MS 350
#define CHORD_RESET_FLASH_MS 300
#define CHORD_RESET_FLASH_STEP_MS 50
#define CHORD_CONTROL_BRIGHTNESS DEFAULT_PAD_BRIGHTNESS
#define CHORD_IDLE_BRIGHTNESS DEFAULT_PAD_BRIGHTNESS

enum chord_control {
  CHORD_CONTROL_INVERSION,
  CHORD_CONTROL_BASS,
  CHORD_CONTROL_DROP,
  CHORD_CONTROL_OPEN,
  CHORD_CONTROL_STRUM,
  CHORD_CONTROL_QUANTIZE,
  CHORD_CONTROL_COUNT,
};

enum chord_quantize_mode {
  CHORD_QUANTIZE_OFF,
  CHORD_QUANTIZE_UP,
  CHORD_QUANTIZE_DOWN,
  CHORD_QUANTIZE_COUNT,
};

typedef struct {
  uint8_t note_count;
  int8_t intervals[7];
} chord_definition_t;

typedef struct {
  bool active;
  uint8_t root_note;
  uint8_t note_count;
  uint8_t notes[CHORD_MAX_TONES];
  uint16_t sent_mask;
  uint16_t alternate_mask;
} active_chord_t;

typedef struct {
  bool active;
  uint8_t slot;
  uint8_t tone_index;
  uint32_t due_at;
} pending_chord_note_t;

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

// Presets are ordered as a two-dimensional complexity map: simple chords begin
// at the bottom-left of the physical board, then grow more complex to the right
// and toward the upper rows.
static const chord_definition_t PROGMEM
    chord_definitions[CHORD_PRESET_COUNT] = {
        // Advanced / jazz
        {4, {0, 5, 10, 15}},
        {4, {0, 4, 8, 11}},
        {5, {0, 4, 7, 11, 18}},
        {5, {0, 3, 7, 11, 14}},
        {6, {0, 4, 7, 10, 13, 21}},
        {6, {0, 4, 7, 10, 18, 21}},
        {6, {0, 4, 8, 10, 13, 15}},
        // Diminished / altered
        {3, {0, 3, 6}},
        {4, {0, 3, 6, 9}},
        {4, {0, 3, 6, 10}},
        {3, {0, 4, 8}},
        {4, {0, 4, 6, 10}},
        {4, {0, 4, 8, 10}},
        {4, {0, 3, 6, 11}},
        // Dominant
        {4, {0, 4, 7, 10}},
        {5, {0, 4, 7, 10, 14}},
        {6, {0, 4, 7, 10, 14, 17}},
        {6, {0, 4, 7, 10, 14, 21}},
        {5, {0, 4, 7, 10, 13}},
        {5, {0, 4, 7, 10, 15}},
        {5, {0, 4, 7, 10, 18}},
        // Minor
        {3, {0, 3, 7}},
        {4, {0, 3, 7, 9}},
        {4, {0, 3, 7, 10}},
        {5, {0, 3, 7, 10, 14}},
        {6, {0, 3, 7, 10, 14, 17}},
        {7, {0, 3, 7, 10, 14, 17, 21}},
        {4, {0, 3, 7, 11}},
        // Major
        {3, {0, 4, 7}},
        {4, {0, 4, 7, 9}},
        {4, {0, 4, 7, 11}},
        {5, {0, 4, 7, 11, 14}},
        {6, {0, 4, 7, 11, 14, 17}},
        {7, {0, 4, 7, 11, 14, 17, 21}},
        {7, {0, 4, 7, 11, 14, 18, 21}},
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

// Multi-press controls deliberately use an independent full-spectrum state
// palette so their level remains readable under every five-color theme.
static const palette_color_t PROGMEM chord_state_colors[12] = {
    {0xFF, 0x00, 0x00}, {0xFF, 0x7A, 0x00}, {0xFF, 0xD5, 0x00},
    {0x9D, 0xFF, 0x00}, {0x00, 0xFF, 0x66}, {0x00, 0xFF, 0xD5},
    {0x00, 0xB3, 0xFF}, {0x00, 0x55, 0xFF}, {0x7A, 0x00, 0xFF},
    {0xC8, 0x00, 0xFF}, {0xFF, 0x00, 0xAE}, {0xFF, 0x00, 0x55},
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
static active_chord_t active_chords[CHORD_ROOT_SLOT_COUNT];
static pending_chord_note_t pending_chord_notes[CHORD_PENDING_EVENT_COUNT];
static uint8_t previous_chord_notes[CHORD_MAX_TONES];
static uint8_t previous_chord_note_count = 0;
static uint16_t custom_degree_mask = (1U << 0) | (1U << 2) | (1U << 4);
static uint8_t custom_degree_octave_lifts[CHORD_DEGREE_COUNT];
static int8_t selected_chord_shape = -1;
static uint8_t chord_inversion = 0;
static uint8_t chord_bass = 0;
static uint8_t chord_drop = 0;
static uint8_t chord_open = 0;
static uint8_t chord_strum = 0;
static uint8_t chord_quantize = CHORD_QUANTIZE_OFF;
static bool chord_auto = false;
static bool chord_latch = false;
static uint32_t chord_control_press_timer[CHORD_CONTROL_COUNT];
static bool chord_control_pressed[CHORD_CONTROL_COUNT];
static bool chord_display_active = false;
static char chord_display_glyph = 'I';
static uint8_t chord_display_level = 0;
static bool chord_display_dots = true;
static uint32_t chord_display_timer = 0;
static uint16_t chord_display_duration = CHORD_DISPLAY_MS;
static int8_t chord_reset_flash_control = -1;
static uint32_t chord_reset_flash_timer = 0;

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

static void set_chord_state_color(uint8_t row, uint8_t col, uint8_t level,
                                  uint8_t brightness) {
  if (level == 0) {
    set_key_color(row, col, COLOR_PRIMARY, brightness);
    return;
  }
  const palette_color_t *color = &chord_state_colors[(level - 1) % 12];
  const uint8_t red = pgm_read_byte(&color->red);
  const uint8_t green = pgm_read_byte(&color->green);
  const uint8_t blue = pgm_read_byte(&color->blue);
  rgb_matrix_set_color(led_index(row, col), scale_component(red, brightness),
                       scale_component(green, brightness),
                       scale_component(blue, brightness));
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

static void tracked_note_on(uint8_t note) {
  if (active_note_counts[note]++ == 0) {
    midi_send_noteon(&midi_device, MIDI_PAD_CHANNEL, note, midi_velocity);
  }
}

static void tracked_note_off(uint8_t note) {
  if (active_note_counts[note] > 0 && --active_note_counts[note] == 0) {
    midi_send_noteoff(&midi_device, MIDI_PAD_CHANNEL, note, 0);
  }
}

static void press_note(uint8_t row, uint8_t col) {
  if (active_notes[row][col] != NO_ACTIVE_NOTE) {
    return;
  }

  const uint8_t note = note_for_position(row, col);
  active_notes[row][col] = note;
  tracked_note_on(note);
}

static void release_note(uint8_t row, uint8_t col) {
  const uint8_t note = active_notes[row][col];
  if (note == NO_ACTIVE_NOTE) {
    return;
  }

  active_notes[row][col] = NO_ACTIVE_NOTE;
  tracked_note_off(note);
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

  for (uint8_t slot = 0; slot < CHORD_ROOT_SLOT_COUNT; ++slot) {
    active_chords[slot].active = false;
    active_chords[slot].note_count = 0;
    active_chords[slot].sent_mask = 0;
    active_chords[slot].alternate_mask = 0;
  }
  for (uint8_t event = 0; event < CHORD_PENDING_EVENT_COUNT; ++event) {
    pending_chord_notes[event].active = false;
  }
  previous_chord_note_count = 0;
}

static uint8_t chord_root_slot(uint8_t row, uint8_t col) {
  return ((row - CHORD_ROOT_FIRST_ROW) * CHORD_ROOT_COL_COUNT) +
         (col - CHORD_ROOT_FIRST_COL);
}

static uint8_t chord_root_note_for_position(uint8_t row, uint8_t col) {
  const uint8_t note_count =
      pgm_read_byte(&scale_definitions[selected_scale].note_count);
  const uint8_t scale_degree = (MATRIX_ROWS - 1) - row;
  const uint8_t interval = pgm_read_byte(
      &scale_definitions[selected_scale].intervals[scale_degree % note_count]);
  const int16_t note = MIDI_PAD_BASE_C_NOTE + root_pitch_class + interval +
                       ((scale_degree / note_count) * 12) +
                       ((col - CHORD_ROOT_FIRST_COL) * 12) +
                       (octave_shift * 12);
  return clamp_midi_note(note);
}

static uint8_t chord_scale_degree_for_note(uint8_t note) {
  const uint8_t note_count =
      pgm_read_byte(&scale_definitions[selected_scale].note_count);
  const uint8_t relative_pitch =
      (note + PITCH_CLASS_COUNT - root_pitch_class) % PITCH_CLASS_COUNT;
  for (uint8_t degree = 0; degree < note_count; ++degree) {
    if (pgm_read_byte(&scale_definitions[selected_scale].intervals[degree]) ==
        relative_pitch) {
      return degree;
    }
  }
  return 0;
}

static bool note_is_in_selected_scale(uint8_t note) {
  const uint8_t note_count =
      pgm_read_byte(&scale_definitions[selected_scale].note_count);
  const uint8_t relative_pitch =
      (note + PITCH_CLASS_COUNT - root_pitch_class) % PITCH_CLASS_COUNT;
  for (uint8_t degree = 0; degree < note_count; ++degree) {
    if (pgm_read_byte(&scale_definitions[selected_scale].intervals[degree]) ==
        relative_pitch) {
      return true;
    }
  }
  return false;
}

static void nearest_scale_notes(uint8_t note, uint8_t *lower,
                                uint8_t *upper, uint8_t *lower_distance,
                                uint8_t *upper_distance) {
  *lower = note;
  *upper = note;
  *lower_distance = UINT8_MAX;
  *upper_distance = UINT8_MAX;
  for (uint8_t distance = 1; distance <= PITCH_CLASS_COUNT; ++distance) {
    if (*lower_distance == UINT8_MAX && note >= distance &&
        note_is_in_selected_scale(note - distance)) {
      *lower = note - distance;
      *lower_distance = distance;
    }
    if (*upper_distance == UINT8_MAX && note <= MIDI_MAX_NOTE - distance &&
        note_is_in_selected_scale(note + distance)) {
      *upper = note + distance;
      *upper_distance = distance;
    }
    if (*lower_distance != UINT8_MAX && *upper_distance != UINT8_MAX) {
      return;
    }
  }
}

static uint8_t quantize_chord_note(uint8_t note) {
  if (chord_quantize == CHORD_QUANTIZE_OFF ||
      note_is_in_selected_scale(note)) {
    return note;
  }
  uint8_t lower;
  uint8_t upper;
  uint8_t lower_distance;
  uint8_t upper_distance;
  nearest_scale_notes(note, &lower, &upper, &lower_distance, &upper_distance);
  if (lower_distance < upper_distance) {
    return lower;
  }
  if (upper_distance < lower_distance) {
    return upper;
  }
  return chord_quantize == CHORD_QUANTIZE_UP ? upper : lower;
}

static int16_t chord_note_for_degree(uint8_t root_note,
                                     uint8_t degree_offset) {
  const uint8_t note_count =
      pgm_read_byte(&scale_definitions[selected_scale].note_count);
  const uint8_t root_degree = chord_scale_degree_for_note(root_note);
  const uint8_t absolute_degree = root_degree + degree_offset;
  const int8_t root_interval = pgm_read_byte(
      &scale_definitions[selected_scale].intervals[root_degree]);
  const int8_t target_interval = pgm_read_byte(
      &scale_definitions[selected_scale].intervals[absolute_degree %
                                                   note_count]);
  return root_note + target_interval - root_interval +
         ((absolute_degree / note_count) * 12);
}

static void sort_chord_tones(int16_t *tones, uint8_t count) {
  for (uint8_t index = 1; index < count; ++index) {
    const int16_t value = tones[index];
    int8_t position = index - 1;
    while (position >= 0 && tones[position] > value) {
      tones[position + 1] = tones[position];
      --position;
    }
    tones[position + 1] = value;
  }
}

static void apply_chord_inversion(int16_t *tones, uint8_t count,
                                  uint8_t inversion) {
  if (count < 2) {
    return;
  }
  sort_chord_tones(tones, count);
  for (uint8_t step = 0; step < inversion; ++step) {
    tones[0] += 12;
    sort_chord_tones(tones, count);
  }
}

static void apply_chord_drop(int16_t *tones, uint8_t count) {
  sort_chord_tones(tones, count);
  if ((chord_drop == 1 || chord_drop == 3) && count >= 2) {
    tones[count - 2] -= 12;
  }
  if (chord_drop == 2 && count >= 3) {
    tones[count - 3] -= 12;
  }
  if (chord_drop == 3 && count >= 4) {
    tones[count - 4] -= 12;
  }
  sort_chord_tones(tones, count);
}

static void apply_chord_open(int16_t *tones, uint8_t count) {
  sort_chord_tones(tones, count);
  for (uint8_t index = 0; index < count; ++index) {
    if (chord_open == 1 && (index & 1U)) {
      tones[index] += 12;
    } else if (chord_open == 2) {
      tones[index] += ((index + 1) / 2) * 12;
    } else if (chord_open == 3) {
      tones[index] += index * 12;
    }
  }
  sort_chord_tones(tones, count);
}

static uint8_t append_chord_bass(int16_t *tones, uint8_t count,
                                 uint8_t root_note) {
  if (chord_bass == 0 || count >= CHORD_MAX_TONES) {
    return count;
  }

  const int16_t bass_note = root_note - (chord_bass * 12);
  if (bass_note < 0 || bass_note > MIDI_MAX_NOTE) {
    return count;
  }
  for (uint8_t index = 0; index < count; ++index) {
    if (tones[index] == bass_note) {
      return count;
    }
  }

  tones[count++] = bass_note;
  sort_chord_tones(tones, count);
  return count;
}

static uint16_t chord_distance(uint8_t left, uint8_t right) {
  return left > right ? left - right : right - left;
}

static uint16_t score_chord_voice(const int16_t *tones, uint8_t count) {
  if (previous_chord_note_count == 0) {
    return 0;
  }
  const uint8_t shared_count =
      count < previous_chord_note_count ? count : previous_chord_note_count;
  uint16_t score = chord_distance(count, previous_chord_note_count) * 12;
  for (uint8_t index = 0; index < shared_count; ++index) {
    score += chord_distance(clamp_midi_note(tones[index]),
                            previous_chord_notes[index]);
  }

  // Extra voices retain octave-sensitive nearest-neighbor scoring while the
  // shared voices remain paired low-to-high.
  for (uint8_t index = shared_count; index < count; ++index) {
    uint16_t nearest = UINT16_MAX;
    for (uint8_t previous = 0; previous < previous_chord_note_count;
         ++previous) {
      const uint16_t distance =
          chord_distance(clamp_midi_note(tones[index]),
                         previous_chord_notes[previous]);
      if (distance < nearest) {
        nearest = distance;
      }
    }
    score += nearest;
  }
  return score;
}

static uint8_t current_chord_tone_count(void) {
  if (selected_chord_shape >= 0) {
    return pgm_read_byte(
        &chord_definitions[selected_chord_shape].note_count);
  }
  uint8_t count = 0;
  for (uint8_t degree = 0; degree < CHORD_DEGREE_COUNT; ++degree) {
    if (custom_degree_mask & (1U << degree)) {
      ++count;
    }
  }
  return count;
}

static uint8_t build_chord_notes(uint8_t root_note, uint8_t *output,
                                 uint16_t *alternate_mask) {
  int16_t base[CHORD_MAX_TONES];
  uint8_t altered_notes[CHORD_MAX_TONES];
  uint8_t altered_count = 0;
  uint8_t count = 0;

  if (selected_chord_shape >= 0) {
    count = pgm_read_byte(
        &chord_definitions[selected_chord_shape].note_count);
    for (uint8_t index = 0; index < count; ++index) {
      base[index] = root_note + pgm_read_byte(
                                    &chord_definitions[selected_chord_shape]
                                         .intervals[index]);
    }
  } else {
    for (uint8_t degree = 0; degree < CHORD_DEGREE_COUNT; ++degree) {
      if (custom_degree_mask & (1U << degree)) {
        base[count++] = chord_note_for_degree(root_note, degree) +
                        (custom_degree_octave_lifts[degree] * 12);
      }
    }
  }

  if (chord_auto && previous_chord_note_count > 0 && count > 0) {
    uint16_t best_score = UINT16_MAX;
    int16_t best[CHORD_MAX_TONES];
    uint8_t best_count = 0;
    for (uint8_t inversion = 0; inversion < count; ++inversion) {
      int16_t candidate[CHORD_MAX_TONES];
      for (uint8_t index = 0; index < count; ++index) {
        candidate[index] = base[index];
      }
      apply_chord_inversion(candidate, count, inversion);
      apply_chord_drop(candidate, count);
      apply_chord_open(candidate, count);
      const uint8_t candidate_count =
          append_chord_bass(candidate, count, root_note);

      // The selected root pad is a hard register anchor. Auto chooses an
      // inversion but never silently moves the entire voicing to another
      // octave; the three root columns remain meaningful and predictable.
      bool valid = true;
      for (uint8_t index = 0; index < candidate_count; ++index) {
        if (candidate[index] < 0 || candidate[index] > MIDI_MAX_NOTE) {
          valid = false;
        }
      }
      if (!valid) {
        continue;
      }
      const uint16_t score = score_chord_voice(candidate, candidate_count);
      if (score < best_score) {
        best_score = score;
        best_count = candidate_count;
        for (uint8_t index = 0; index < candidate_count; ++index) {
          best[index] = candidate[index];
        }
      }
    }
    if (best_score != UINT16_MAX) {
      count = best_count;
      for (uint8_t index = 0; index < count; ++index) {
        base[index] = best[index];
      }
    }
  } else {
    apply_chord_inversion(base, count,
                          count == 0 ? 0 : chord_inversion % count);
    apply_chord_drop(base, count);
    apply_chord_open(base, count);
    count = append_chord_bass(base, count, root_note);
  }

  for (uint8_t index = 0; index < count; ++index) {
    const uint8_t source_note = clamp_midi_note(base[index]);
    if (!note_is_in_selected_scale(source_note)) {
      const uint8_t quantized_note = quantize_chord_note(source_note);
      base[index] = quantized_note;
      altered_notes[altered_count++] = quantized_note;
    }
  }

  sort_chord_tones(base, count);
  *alternate_mask = 0;
  uint8_t unique_count = 0;
  for (uint8_t index = 0; index < count; ++index) {
    const uint8_t note = clamp_midi_note(base[index]);
    if (unique_count == 0 || output[unique_count - 1] != note) {
      output[unique_count] = note;
      for (uint8_t altered = 0; altered < altered_count; ++altered) {
        if (altered_notes[altered] == note) {
          *alternate_mask |= (1U << unique_count);
          break;
        }
      }
      ++unique_count;
    }
  }
  return unique_count;
}

static void cancel_pending_chord_notes(uint8_t slot) {
  for (uint8_t event = 0; event < CHORD_PENDING_EVENT_COUNT; ++event) {
    if (pending_chord_notes[event].active &&
        pending_chord_notes[event].slot == slot) {
      pending_chord_notes[event].active = false;
    }
  }
}

static void stop_chord_slot(uint8_t slot) {
  active_chord_t *chord = &active_chords[slot];
  cancel_pending_chord_notes(slot);
  for (uint8_t index = 0; index < chord->note_count; ++index) {
    if (chord->sent_mask & (1U << index)) {
      tracked_note_off(chord->notes[index]);
    }
  }
  chord->active = false;
  chord->note_count = 0;
  chord->sent_mask = 0;
  chord->alternate_mask = 0;
}

static void stop_all_chord_slots(void) {
  for (uint8_t slot = 0; slot < CHORD_ROOT_SLOT_COUNT; ++slot) {
    stop_chord_slot(slot);
  }
}

static void send_chord_tone(uint8_t slot, uint8_t tone_index) {
  active_chord_t *chord = &active_chords[slot];
  if (!chord->active || tone_index >= chord->note_count ||
      (chord->sent_mask & (1U << tone_index))) {
    return;
  }
  tracked_note_on(chord->notes[tone_index]);
  chord->sent_mask |= (1U << tone_index);
}

static void queue_chord_tone(uint8_t slot, uint8_t tone_index,
                             uint32_t due_at) {
  for (uint8_t event = 0; event < CHORD_PENDING_EVENT_COUNT; ++event) {
    if (!pending_chord_notes[event].active) {
      pending_chord_notes[event].active = true;
      pending_chord_notes[event].slot = slot;
      pending_chord_notes[event].tone_index = tone_index;
      pending_chord_notes[event].due_at = due_at;
      return;
    }
  }
  send_chord_tone(slot, tone_index);
}

static uint8_t chord_strum_delay(void) {
  if (chord_strum == 0) {
    return 0;
  }
  return chord_strum <= 3 ? chord_strum * 20 : (chord_strum - 3) * 20;
}

static void start_chord_slot(uint8_t slot, uint8_t root_note) {
  active_chord_t *chord = &active_chords[slot];
  stop_chord_slot(slot);
  chord->active = true;
  chord->root_note = root_note;
  chord->note_count =
      build_chord_notes(root_note, chord->notes, &chord->alternate_mask);
  chord->sent_mask = 0;

  const uint8_t delay = chord_strum_delay();
  const bool descending = chord_strum >= 4;
  const uint32_t now = timer_read32();
  for (uint8_t order = 0; order < chord->note_count; ++order) {
    const uint8_t tone_index =
        descending ? chord->note_count - 1 - order : order;
    if (delay == 0 || order == 0) {
      send_chord_tone(slot, tone_index);
    } else {
      queue_chord_tone(slot, tone_index, now + (order * delay));
    }
  }

  previous_chord_note_count = chord->note_count;
  for (uint8_t index = 0; index < chord->note_count; ++index) {
    previous_chord_notes[index] = chord->notes[index];
  }
}

static void revoice_active_chords(void) {
  uint8_t roots[CHORD_ROOT_SLOT_COUNT];
  bool active[CHORD_ROOT_SLOT_COUNT];
  for (uint8_t slot = 0; slot < CHORD_ROOT_SLOT_COUNT; ++slot) {
    active[slot] = active_chords[slot].active;
    roots[slot] = active_chords[slot].root_note;
    if (active[slot]) {
      stop_chord_slot(slot);
    }
  }
  for (uint8_t slot = 0; slot < CHORD_ROOT_SLOT_COUNT; ++slot) {
    if (active[slot]) {
      start_chord_slot(slot, roots[slot]);
    }
  }
}

static int8_t first_active_chord_slot(void) {
  for (uint8_t slot = 0; slot < CHORD_ROOT_SLOT_COUNT; ++slot) {
    if (active_chords[slot].active) {
      return slot;
    }
  }
  return -1;
}

static void cancel_chord_parameter_display(void) {
  chord_display_active = false;
  chord_reset_flash_control = -1;
}

static void press_chord_root(uint8_t row, uint8_t col) {
  cancel_chord_parameter_display();
  const uint8_t slot = chord_root_slot(row, col);
  if (chord_latch) {
    stop_all_chord_slots();
  }
  start_chord_slot(slot, chord_root_note_for_position(row, col));
}

static void release_chord_root(uint8_t row, uint8_t col) {
  if (!chord_latch) {
    stop_chord_slot(chord_root_slot(row, col));
  }
}

static void normalize_chord_controls(void) {
  const uint8_t tone_count = current_chord_tone_count();
  if (tone_count == 0 || chord_inversion >= tone_count) {
    chord_inversion = 0;
  }
  const uint8_t drop_count = tone_count < 4 ? tone_count : 4;
  if (drop_count == 0 || chord_drop >= drop_count) {
    chord_drop = 0;
  }
}

static void select_chord_shape(uint8_t shape) {
  cancel_chord_parameter_display();
  selected_chord_shape = shape;
  normalize_chord_controls();
  revoice_active_chords();
}

static void toggle_chord_degree(uint8_t degree) {
  cancel_chord_parameter_display();
  const bool entering_custom = selected_chord_shape >= 0;
  const bool was_enabled = custom_degree_mask & (1U << degree);

  if (entering_custom) {
    custom_degree_mask = (1U << 0) | (1U << degree);
    for (uint8_t index = 0; index < CHORD_DEGREE_COUNT; ++index) {
      custom_degree_octave_lifts[index] = 0;
    }
  } else if (was_enabled) {
    const uint16_t updated = custom_degree_mask & ~(1U << degree);
    if (updated == 0) {
      return;
    }
    custom_degree_mask = updated;
    custom_degree_octave_lifts[degree] = 0;
  } else {
    const int8_t active_slot = first_active_chord_slot();
    custom_degree_mask |= (1U << degree);
    custom_degree_octave_lifts[degree] = 0;
    if (active_slot >= 0) {
      const active_chord_t *chord = &active_chords[active_slot];
      int16_t new_note = chord_note_for_degree(chord->root_note, degree);
      const uint8_t highest = chord->notes[chord->note_count - 1];
      while (new_note <= highest) {
        new_note += 12;
        ++custom_degree_octave_lifts[degree];
      }
    }
  }

  selected_chord_shape = -1;
  normalize_chord_controls();
  revoice_active_chords();
}

static void start_chord_parameter_display(char glyph, uint8_t level,
                                          bool dots, uint16_t duration) {
  chord_display_active = true;
  chord_display_glyph = glyph;
  chord_display_level = level;
  chord_display_dots = dots;
  chord_display_duration = duration;
  chord_display_timer = timer_read32();
}

static void start_chord_reset_flash(enum chord_control control, char glyph) {
  start_chord_parameter_display(glyph, 0, true, CHORD_RESET_DISPLAY_MS);
  chord_reset_flash_control = control;
  chord_reset_flash_timer = timer_read32();
}

static char chord_control_glyph(enum chord_control control) {
  static const char glyphs[CHORD_CONTROL_COUNT] = {'I', 'B', 'D',
                                                   'O', 'S', 'Q'};
  return glyphs[control];
}

static void cycle_chord_control(enum chord_control control) {
  uint8_t level = 0;
  bool looped = false;
  switch (control) {
  case CHORD_CONTROL_INVERSION: {
    const uint8_t count = current_chord_tone_count();
    chord_auto = false;
    chord_inversion = count == 0 ? 0 : (chord_inversion + 1) % count;
    level = chord_inversion;
    looped = level == 0;
    break;
  }
  case CHORD_CONTROL_BASS:
    chord_bass = (chord_bass + 1) % 3;
    level = chord_bass;
    looped = level == 0;
    break;
  case CHORD_CONTROL_DROP: {
    const uint8_t tones = current_chord_tone_count();
    const uint8_t count = tones < 4 ? tones : 4;
    chord_drop = count == 0 ? 0 : (chord_drop + 1) % count;
    level = chord_drop;
    looped = level == 0;
    break;
  }
  case CHORD_CONTROL_OPEN:
    chord_open = (chord_open + 1) % 4;
    level = chord_open;
    looped = level == 0;
    break;
  case CHORD_CONTROL_STRUM:
    chord_strum = (chord_strum + 1) % 7;
    level = chord_strum;
    looped = level == 0;
    break;
  case CHORD_CONTROL_QUANTIZE:
    chord_quantize = (chord_quantize + 1) % CHORD_QUANTIZE_COUNT;
    level = chord_quantize;
    looped = level == 0;
    break;
  default:
    break;
  }

  revoice_active_chords();
  if (looped) {
    start_chord_reset_flash(control, chord_control_glyph(control));
  } else {
    start_chord_parameter_display(chord_control_glyph(control), level, true,
                                  CHORD_DISPLAY_MS);
  }
}

static void reset_chord_control(enum chord_control control) {
  switch (control) {
  case CHORD_CONTROL_INVERSION:
    chord_auto = false;
    chord_inversion = 0;
    break;
  case CHORD_CONTROL_BASS:
    chord_bass = 0;
    break;
  case CHORD_CONTROL_DROP:
    chord_drop = 0;
    break;
  case CHORD_CONTROL_OPEN:
    chord_open = 0;
    break;
  case CHORD_CONTROL_STRUM:
    chord_strum = 0;
    break;
  case CHORD_CONTROL_QUANTIZE:
    chord_quantize = CHORD_QUANTIZE_OFF;
    break;
  default:
    break;
  }
  revoice_active_chords();
  start_chord_reset_flash(control, chord_control_glyph(control));
}

static void process_chord_control(enum chord_control control,
                                  bool pressed) {
  if (pressed) {
    chord_control_pressed[control] = true;
    chord_control_press_timer[control] = timer_read32();
    return;
  }
  if (!chord_control_pressed[control]) {
    return;
  }
  chord_control_pressed[control] = false;
  if (timer_elapsed32(chord_control_press_timer[control]) >=
      CHORD_CONTROL_HOLD_MS) {
    reset_chord_control(control);
  } else {
    cycle_chord_control(control);
  }
}

static void toggle_chord_auto(void) {
  chord_auto = !chord_auto;
  revoice_active_chords();
  start_chord_parameter_display('A', chord_auto ? 1 : 0, false,
                                CHORD_DISPLAY_MS);
}

static void toggle_chord_latch(void) {
  chord_latch = !chord_latch;
  if (!chord_latch) {
    stop_all_chord_slots();
  }
  start_chord_parameter_display('L', chord_latch ? 1 : 0, false,
                                CHORD_DISPLAY_MS);
}

void matrix_scan_user(void) {
  const uint32_t now = timer_read32();
  for (uint8_t event = 0; event < CHORD_PENDING_EVENT_COUNT; ++event) {
    pending_chord_note_t *pending = &pending_chord_notes[event];
    if (pending->active && (int32_t)(now - pending->due_at) >= 0) {
      pending->active = false;
      send_chord_tone(pending->slot, pending->tone_index);
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

  case CH_ROOT:
    if (record->event.pressed) {
      press_chord_root(row, col);
    } else {
      release_chord_root(row, col);
    }
    return false;

  case CH_DEGREE_1 ... CH_DEGREE_14:
    if (record->event.pressed) {
      toggle_chord_degree(keycode - CH_DEGREE_1);
    }
    return false;

  case CH_SHAPE_0 ... CH_SHAPE_34:
    if (record->event.pressed) {
      select_chord_shape(keycode - CH_SHAPE_0);
    }
    return false;

  case CH_INV:
    process_chord_control(CHORD_CONTROL_INVERSION, record->event.pressed);
    return false;

  case CH_BASS:
    process_chord_control(CHORD_CONTROL_BASS, record->event.pressed);
    return false;

  case CH_DROP:
    process_chord_control(CHORD_CONTROL_DROP, record->event.pressed);
    return false;

  case CH_OPEN:
    process_chord_control(CHORD_CONTROL_OPEN, record->event.pressed);
    return false;

  case CH_STRUM:
    process_chord_control(CHORD_CONTROL_STRUM, record->event.pressed);
    return false;

  case CH_QUANTIZE:
    process_chord_control(CHORD_CONTROL_QUANTIZE, record->event.pressed);
    return false;

  case CH_AUTO:
    if (record->event.pressed) {
      toggle_chord_auto();
    }
    return false;

  case CH_LATCH:
    if (record->event.pressed) {
      toggle_chord_latch();
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
        draw_font_pixel(row, col, COLOR_TERTIARY, ROOT_NOTE_BRIGHTNESS);
      }
    }
  }

  if (is_black_key(root_pitch_class)) {
    // One blank column separates the 3x6 letter from this 2x2 marker.
    set_key_color(DISPLAY_ROW, DISPLAY_COL + 4, COLOR_SECONDARY,
                  DEFAULT_PAD_BRIGHTNESS);
    set_key_color(DISPLAY_ROW, DISPLAY_COL + 5, COLOR_SECONDARY,
                  DEFAULT_PAD_BRIGHTNESS);
    set_key_color(DISPLAY_ROW + 1, DISPLAY_COL + 4, COLOR_SECONDARY,
                  DEFAULT_PAD_BRIGHTNESS);
    set_key_color(DISPLAY_ROW + 1, DISPLAY_COL + 5, COLOR_SECONDARY,
                  DEFAULT_PAD_BRIGHTNESS);
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
        draw_font_pixel(row, view_col, COLOR_TERTIARY,
                        DEFAULT_PAD_BRIGHTNESS);
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
        draw_font_pixel(row, col, COLOR_QUATERNARY,
                        DEFAULT_PAD_BRIGHTNESS);
      }
    }
  }
}

static void set_root_selector_led(uint8_t pitch_class) {
  const uint8_t row = DISPLAY_ROW + (pitch_class / 2);
  const uint8_t col = pitch_class % 2;
  const uint8_t led = led_index(row, col);
  if (pitch_class == root_pitch_class) {
    set_palette_color(led, COLOR_TERTIARY, ROOT_NOTE_BRIGHTNESS);
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
    set_key_color(row, col, COLOR_SECONDARY, DEFAULT_PAD_BRIGHTNESS);
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

  set_key_color(0, 0, COLOR_QUATERNARY,
                DEFAULT_PAD_BRIGHTNESS); // Previous palette
  set_key_color(0, 1, COLOR_QUINARY,
                DEFAULT_PAD_BRIGHTNESS); // Next palette
  set_key_color(0, 8, COLOR_QUINARY,
                DEFAULT_PAD_BRIGHTNESS); // Layer select
  set_key_color(0, 9, COLOR_QUATERNARY,
                DEFAULT_PAD_BRIGHTNESS); // Settings

  for (uint8_t slot = 0; slot < SCALE_PAGE_SIZE; ++slot) {
    set_scale_selector_led(slot);
  }
  set_key_color(1, 9, COLOR_QUATERNARY,
                DEFAULT_PAD_BRIGHTNESS); // Page up
  set_key_color(2, 9, COLOR_QUINARY,
                DEFAULT_PAD_BRIGHTNESS); // Page down

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

static void render_performance_top_row(enum layers active_layer) {
  set_key_color(0, 0, COLOR_QUATERNARY,
                PERFORMANCE_NAV_BRIGHTNESS); // Octave down
  set_key_color(0, 1, COLOR_QUINARY,
                PERFORMANCE_NAV_BRIGHTNESS); // Octave up
  set_key_color(0, 3,
                active_layer == SCALE_LAYER ? COLOR_TERTIARY : COLOR_PRIMARY,
                PERFORMANCE_NAV_BRIGHTNESS);
  set_key_color(0, 4,
                active_layer == CHORD_LAYER ? COLOR_TERTIARY : COLOR_PRIMARY,
                PERFORMANCE_NAV_BRIGHTNESS);
  set_key_color(0, 9, COLOR_QUATERNARY,
                PERFORMANCE_NAV_BRIGHTNESS); // Settings
}

static void render_scale_layer(void) {
  clear_all_leds();
  render_performance_top_row(SCALE_LAYER);

  for (uint8_t row = 1; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      const uint8_t note = note_for_position(row, col);
      const uint8_t led = led_index(row, col);
      if (active_note_counts[note] > 0) {
        set_palette_color(led, COLOR_SECONDARY, PRESSED_PAD_BRIGHTNESS);
      } else if (note % PITCH_CLASS_COUNT == root_pitch_class) {
        set_palette_color(led, COLOR_TERTIARY, ROOT_NOTE_BRIGHTNESS);
      } else {
        set_palette_color(led, COLOR_PRIMARY, PERFORMANCE_PAD_BRIGHTNESS);
      }
    }
  }
}

static uint8_t chord_glyph_row(char glyph, uint8_t row) {
  static const uint8_t PROGMEM glyphs[8][5] = {
      // I, B, D, A, O, S, L, Q. Bits 7..5 are left to right.
      {0xE0, 0x40, 0x40, 0x40, 0xE0},
      {0xC0, 0xA0, 0xC0, 0xA0, 0xC0},
      {0xC0, 0xA0, 0xA0, 0xA0, 0xC0},
      {0x40, 0xA0, 0xE0, 0xA0, 0xA0},
      {0xE0, 0xA0, 0xA0, 0xA0, 0xE0},
      {0xE0, 0x80, 0xE0, 0x20, 0xE0},
      {0x80, 0x80, 0x80, 0x80, 0xE0},
      {0x40, 0xA0, 0xA0, 0xE0, 0x60},
  };
  uint8_t glyph_index = 0;
  switch (glyph) {
  case 'B':
    glyph_index = 1;
    break;
  case 'D':
    glyph_index = 2;
    break;
  case 'A':
    glyph_index = 3;
    break;
  case 'O':
    glyph_index = 4;
    break;
  case 'S':
    glyph_index = 5;
    break;
  case 'L':
    glyph_index = 6;
    break;
  case 'Q':
    glyph_index = 7;
    break;
  default:
    break;
  }
  return pgm_read_byte(&glyphs[glyph_index][row]);
}

static uint8_t chord_control_level(enum chord_control control) {
  switch (control) {
  case CHORD_CONTROL_INVERSION:
    return chord_inversion;
  case CHORD_CONTROL_BASS:
    return chord_bass;
  case CHORD_CONTROL_DROP:
    return chord_drop;
  case CHORD_CONTROL_OPEN:
    return chord_open;
  case CHORD_CONTROL_STRUM:
    return chord_strum;
  case CHORD_CONTROL_QUANTIZE:
    return chord_quantize;
  default:
    return 0;
  }
}

static uint8_t chord_control_row(enum chord_control control) {
  return control == CHORD_CONTROL_QUANTIZE ? 1 : 2;
}

static uint8_t chord_control_col(enum chord_control control) {
  switch (control) {
  case CHORD_CONTROL_INVERSION:
    return 0;
  case CHORD_CONTROL_BASS:
    return 1;
  case CHORD_CONTROL_DROP:
    return 2;
  case CHORD_CONTROL_OPEN:
    return 3;
  case CHORD_CONTROL_STRUM:
    return 4;
  case CHORD_CONTROL_QUANTIZE:
    return 2;
  default:
    return 0;
  }
}

static bool chord_reset_flash_is_on(void) {
  if (chord_reset_flash_control < 0) {
    return false;
  }
  const uint32_t elapsed = timer_elapsed32(chord_reset_flash_timer);
  return elapsed < CHORD_RESET_FLASH_MS &&
         ((elapsed / CHORD_RESET_FLASH_STEP_MS) % 2 == 0);
}

static void update_chord_display_timers(void) {
  if (chord_display_active &&
      timer_elapsed32(chord_display_timer) >= chord_display_duration) {
    chord_display_active = false;
  }
  if (chord_reset_flash_control >= 0 &&
      timer_elapsed32(chord_reset_flash_timer) >= CHORD_RESET_FLASH_MS) {
    chord_reset_flash_control = -1;
  }
}

static void render_chord_controls(void) {
  for (uint8_t control = 0; control < CHORD_CONTROL_COUNT; ++control) {
    const uint8_t row = chord_control_row(control);
    const uint8_t col = chord_control_col(control);
    if (chord_reset_flash_control == control) {
      if (chord_reset_flash_is_on()) {
        set_key_color(row, col, COLOR_QUINARY, 255);
      }
      continue;
    }
    const uint8_t level = chord_control_level(control);
    if (level > 0) {
      if (control == CHORD_CONTROL_QUANTIZE) {
        set_key_color(row, col, COLOR_SECONDARY, CHORD_CONTROL_BRIGHTNESS);
      } else {
        set_chord_state_color(row, col, level, CHORD_CONTROL_BRIGHTNESS);
      }
    } else {
      set_key_color(row, col, COLOR_PRIMARY, CHORD_CONTROL_BRIGHTNESS);
    }
  }

  set_key_color(1, 0, chord_auto ? COLOR_SECONDARY : COLOR_PRIMARY,
                CHORD_CONTROL_BRIGHTNESS);
  set_key_color(1, 1, chord_latch ? COLOR_SECONDARY : COLOR_PRIMARY,
                CHORD_CONTROL_BRIGHTNESS);
}

static void render_chord_degrees(void) {
  for (uint8_t degree = 0; degree < CHORD_DEGREE_COUNT; ++degree) {
    const uint8_t row = CHORD_ROOT_FIRST_ROW + (degree / 7);
    const uint8_t col = degree % 7;
    if (selected_chord_shape < 0 &&
        (custom_degree_mask & (1U << degree))) {
      set_key_color(row, col, COLOR_QUINARY, DEFAULT_PAD_BRIGHTNESS);
    } else {
      set_key_color(row, col, COLOR_QUATERNARY, CHORD_IDLE_BRIGHTNESS);
    }
  }
}

static void render_chord_shapes(void) {
  for (uint8_t shape = 0; shape < CHORD_PRESET_COUNT; ++shape) {
    const uint8_t row = 5 + (shape / 7);
    const uint8_t col = shape % 7;
    set_key_color(row, col,
                  selected_chord_shape == shape ? COLOR_QUINARY
                                                : COLOR_PRIMARY,
                  CHORD_IDLE_BRIGHTNESS);
  }
}

static bool note_is_an_active_chord_tone(uint8_t note) {
  for (uint8_t slot = 0; slot < CHORD_ROOT_SLOT_COUNT; ++slot) {
    const active_chord_t *chord = &active_chords[slot];
    if (!chord->active) {
      continue;
    }
    for (uint8_t index = 0; index < chord->note_count; ++index) {
      if (chord->notes[index] == note) {
        return true;
      }
    }
  }
  return false;
}

static bool note_is_an_alternate_indicator(uint8_t note) {
  if (chord_quantize != CHORD_QUANTIZE_OFF) {
    return false;
  }
  for (uint8_t slot = 0; slot < CHORD_ROOT_SLOT_COUNT; ++slot) {
    const active_chord_t *chord = &active_chords[slot];
    if (!chord->active) {
      continue;
    }
    for (uint8_t index = 0; index < chord->note_count; ++index) {
      if (!(chord->alternate_mask & (1U << index))) {
        continue;
      }
      const uint8_t altered_note = chord->notes[index];
      uint8_t lower;
      uint8_t upper;
      uint8_t lower_distance;
      uint8_t upper_distance;
      nearest_scale_notes(altered_note, &lower, &upper, &lower_distance,
                          &upper_distance);
      const uint8_t nearest_distance =
          lower_distance < upper_distance ? lower_distance : upper_distance;
      if ((note == lower && lower_distance == nearest_distance) ||
          (note == upper && upper_distance == nearest_distance)) {
        return true;
      }
    }
  }
  return false;
}

static void render_chord_root_grid(void) {
  for (uint8_t row = CHORD_ROOT_FIRST_ROW; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = CHORD_ROOT_FIRST_COL; col < MATRIX_COLS; ++col) {
      const uint8_t note = chord_root_note_for_position(row, col);
      const uint8_t slot = chord_root_slot(row, col);
      if (active_chords[slot].active) {
        set_key_color(row, col, COLOR_SECONDARY, PRESSED_PAD_BRIGHTNESS);
      } else if (note_is_an_alternate_indicator(note)) {
        set_key_color(row, col, COLOR_QUATERNARY, DEFAULT_PAD_BRIGHTNESS);
      } else if (note_is_an_active_chord_tone(note)) {
        set_key_color(row, col, COLOR_QUINARY, DEFAULT_PAD_BRIGHTNESS);
      } else if (note % PITCH_CLASS_COUNT == root_pitch_class) {
        set_key_color(row, col, COLOR_TERTIARY, ROOT_NOTE_BRIGHTNESS);
      } else {
        set_key_color(row, col, COLOR_PRIMARY, CHORD_IDLE_BRIGHTNESS);
      }
    }
  }
}

static void render_chord_parameter_display(void) {
  const bool reset_flashing = chord_reset_flash_control >= 0;
  const bool reset_on = chord_reset_flash_is_on();

  if (chord_display_dots && chord_display_level > 0) {
    const uint8_t completed_laps = (chord_display_level - 1) / 6;
    const uint8_t dots_in_lap = ((chord_display_level - 1) % 6) + 1;
    for (uint8_t dot = 0; dot < 6; ++dot) {
      int8_t lap = -1;
      if (dot < dots_in_lap) {
        lap = completed_laps;
      } else if (completed_laps > 0) {
        lap = completed_laps - 1;
      }
      if (lap >= 0) {
        set_chord_state_color(CHORD_ROOT_FIRST_ROW + (dot / 3),
                              CHORD_ROOT_FIRST_COL + (dot % 3), lap + 1,
                              DEFAULT_PAD_BRIGHTNESS);
      }
    }
  }

  const bool binary = chord_display_glyph == 'A' || chord_display_glyph == 'L';
  for (uint8_t glyph_row = 0; glyph_row < 5; ++glyph_row) {
    const uint8_t row_bits = chord_glyph_row(chord_display_glyph, glyph_row);
    for (uint8_t col = 0; col < 3; ++col) {
      if (!(row_bits & (0x80 >> col))) {
        continue;
      }
      const uint8_t row = CHORD_ROOT_FIRST_ROW + 2 + glyph_row;
      const uint8_t physical_col = CHORD_ROOT_FIRST_COL + col;
      if (reset_flashing) {
        if (reset_on) {
          set_key_color(row, physical_col, COLOR_QUINARY, 255);
        }
      } else if (binary) {
        set_key_color(row, physical_col,
                      chord_display_level ? COLOR_SECONDARY : COLOR_PRIMARY,
                      DEFAULT_PAD_BRIGHTNESS);
      } else if (chord_display_level > 0) {
        set_chord_state_color(row, physical_col, chord_display_level,
                              DEFAULT_PAD_BRIGHTNESS);
      } else {
        set_key_color(row, physical_col, COLOR_PRIMARY,
                      DEFAULT_PAD_BRIGHTNESS);
      }
    }
  }
}

static void render_chord_layer(void) {
  clear_all_leds();
  update_chord_display_timers();
  render_performance_top_row(CHORD_LAYER);
  render_chord_controls();
  render_chord_degrees();
  render_chord_shapes();
  if (chord_display_active) {
    render_chord_parameter_display();
  } else {
    render_chord_root_grid();
  }
}

static void render_layer_select(void) {
  clear_all_leds();
  set_key_color(0, 0, COLOR_PRIMARY, DEFAULT_PAD_BRIGHTNESS); // Scale
  set_key_color(0, 1, COLOR_SECONDARY,
                DEFAULT_PAD_BRIGHTNESS); // Chord placeholder
}

static void render_physically_pressed_keys(void) {
  for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      if (matrix_is_on(row, col)) {
        set_key_color(row, col, COLOR_SECONDARY, PRESSED_PAD_BRIGHTNESS);
      }
    }
  }
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
  render_physically_pressed_keys();
  return false;
}
