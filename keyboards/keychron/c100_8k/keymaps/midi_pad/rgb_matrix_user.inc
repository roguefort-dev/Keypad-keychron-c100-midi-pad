// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later
RGB_MATRIX_EFFECT(MIDI_PAD_CANVAS)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
// The indicator callback owns all 100 pixels. A background animation would
// immediately be overwritten and would dirty otherwise unchanged SPI buffers.
static bool MIDI_PAD_CANVAS(effect_params_t *params) {
  (void)params;
  return false;
}
#endif
