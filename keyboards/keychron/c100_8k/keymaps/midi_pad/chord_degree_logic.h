// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdbool.h>
#include <stdint.h>

enum chord_degree_action {
  CHORD_DEGREE_TOGGLE_CUSTOM,
  CHORD_DEGREE_START_CUSTOM,
  CHORD_DEGREE_EXTEND_PRESET,
};

static inline enum chord_degree_action
chord_degree_action_for_state(bool preset_selected, bool chord_active) {
  if (!preset_selected) {
    return CHORD_DEGREE_TOGGLE_CUSTOM;
  }
  return chord_active ? CHORD_DEGREE_EXTEND_PRESET
                      : CHORD_DEGREE_START_CUSTOM;
}

static inline int16_t chord_note_above(int16_t note, int16_t highest,
                                       uint8_t *octave_lifts) {
  *octave_lifts = 0;
  while (note <= highest) {
    note += 12;
    ++*octave_lifts;
  }
  return note;
}
