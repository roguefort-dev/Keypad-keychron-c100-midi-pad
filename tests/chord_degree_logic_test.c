// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>

#include "../keyboards/keychron/c100_8k/keymaps/midi_pad/chord_degree_logic.h"

int main(void) {
  assert(chord_degree_action_for_state(true, true, false) ==
         CHORD_DEGREE_EXTEND_PRESET);
  assert(chord_degree_action_for_state(true, false, false) ==
         CHORD_DEGREE_START_CUSTOM);
  assert(chord_degree_action_for_state(false, true, false) ==
         CHORD_DEGREE_TOGGLE_CUSTOM);
  const enum chord_degree_action degree_x =
      chord_degree_action_for_state(true, false, true);
  const enum chord_degree_action degree_y =
      chord_degree_action_for_state(true, false, true);
  assert(degree_x == CHORD_DEGREE_EXTEND_PRESET);
  assert(degree_y == CHORD_DEGREE_EXTEND_PRESET);
  assert(chord_degree_action_for_state(true, false, false) ==
         CHORD_DEGREE_START_CUSTOM);

  uint8_t lifts = 0;
  const int16_t major_seventh[] = {40, 44, 47, 51};
  const int16_t added_ninth =
      chord_note_above(42, major_seventh[3], &lifts);
  assert(major_seventh[0] == 40);
  assert(major_seventh[1] == 44);
  assert(major_seventh[2] == 47);
  assert(major_seventh[3] == 51);
  assert(added_ninth == 54);
  assert(lifts == 1);
  const int16_t added_eleventh = chord_note_above(45, added_ninth, &lifts);
  assert(added_eleventh == 57);
  assert(lifts == 1);
  return 0;
}
