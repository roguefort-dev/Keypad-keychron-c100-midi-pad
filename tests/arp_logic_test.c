// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../keyboards/keychron/c100_8k/keymaps/midi_pad/arp_logic.h"

static void expect_pattern(enum arp_pattern pattern, uint8_t count,
                           const uint8_t *expected, size_t length) {
  assert(arp_pattern_length(pattern, count) == length);
  for (size_t step = 0; step < length * 3; ++step) {
    assert(arp_pattern_note_index(pattern, step, count) ==
           expected[step % length]);
  }
}

int main(void) {
  const uint8_t up[] = {0, 1, 2};
  const uint8_t down[] = {2, 1, 0};
  const uint8_t up_down[] = {0, 1, 2, 1};
  const uint8_t down_up[] = {2, 1, 0, 1};
  const uint8_t thumb[] = {0, 1, 0, 2};
  const uint8_t pinky[] = {0, 2, 1, 2};
  const uint8_t up_down_four[] = {0, 1, 2, 3, 2, 1};
  const uint8_t down_up_four[] = {3, 2, 1, 0, 1, 2};
  const uint8_t thumb_four[] = {0, 1, 0, 2, 0, 3};
  const uint8_t pinky_four[] = {0, 3, 1, 3, 2, 3};
  expect_pattern(ARP_PATTERN_UP, 3, up, 3);
  expect_pattern(ARP_PATTERN_DOWN, 3, down, 3);
  expect_pattern(ARP_PATTERN_UP_DOWN, 3, up_down, 4);
  expect_pattern(ARP_PATTERN_DOWN_UP, 3, down_up, 4);
  expect_pattern(ARP_PATTERN_THUMB, 3, thumb, 4);
  expect_pattern(ARP_PATTERN_PINKY, 3, pinky, 4);
  expect_pattern(ARP_PATTERN_PLAYED, 3, up, 3);
  expect_pattern(ARP_PATTERN_UP_DOWN, 4, up_down_four, 6);
  expect_pattern(ARP_PATTERN_DOWN_UP, 4, down_up_four, 6);
  expect_pattern(ARP_PATTERN_THUMB, 4, thumb_four, 6);
  expect_pattern(ARP_PATTERN_PINKY, 4, pinky_four, 6);

  assert(arp_pattern_length(ARP_PATTERN_UP_DOWN, 1) == 1);
  assert(arp_pattern_length(ARP_PATTERN_UP_DOWN, 0) == 0);
  assert(arp_pattern_note_index(ARP_PATTERN_UP, 12, 0) == 0);
  for (uint8_t pattern = 0; pattern < ARP_PATTERN_COUNT; ++pattern) {
    for (uint8_t count = 1; count <= 32; ++count) {
      const uint8_t length = arp_pattern_length(pattern, count);
      assert(length > 0);
      for (uint8_t step = 0; step < length; ++step) {
        assert(arp_pattern_note_index(pattern, step, count) < count);
      }
    }
  }

  assert(arp_step_clock_units(4, ARP_MODIFIER_OFF) == 144);
  assert(arp_step_clock_units(5, ARP_MODIFIER_OFF) == 72);
  assert(arp_step_clock_units(6, ARP_MODIFIER_OFF) == 36);
  assert(arp_step_clock_units(6, ARP_MODIFIER_DOTTED) == 54);
  assert(arp_step_clock_units(6, ARP_MODIFIER_TRIPLET) == 24);
  assert(arp_step_milliseconds(4, ARP_MODIFIER_OFF, 120) == 125);
  assert(arp_step_milliseconds(4, ARP_MODIFIER_DOTTED, 120) == 187);
  assert(arp_step_milliseconds(4, ARP_MODIFIER_TRIPLET, 120) == 83);
  assert(arp_gate_milliseconds(125, 750) == 93);
  assert(arp_gate_milliseconds(125, 1001) == 126);
  assert(!arp_gate_is_legato(1000));
  assert(arp_gate_is_legato(1001));

  // Host sync must not make performance modes silent before a clock arrives,
  // or after a previously connected clock source stops.
  assert(!arp_host_clock_is_active(true, false, 0, 750));
  assert(arp_host_clock_is_active(true, true, 20, 750));
  assert(!arp_host_clock_is_active(true, true, 751, 750));
  assert(!arp_host_clock_is_active(false, true, 20, 750));

  // MIDI transport must not disable live performance. A stopped DAW either
  // keeps driving us with clock or lets the internal scheduler take over.
  assert(arp_clock_driver_for_state(true, true, false) ==
         ARP_CLOCK_DRIVER_HOST);
  assert(arp_clock_driver_for_state(true, false, false) ==
         ARP_CLOCK_DRIVER_INTERNAL);

  // A 60 BPM host emits MIDI Clock roughly every 42 ms. The preview must use
  // that measured period instead of remaining at the internal 120 BPM speed.
  assert(arp_preview_step_milliseconds(4, ARP_MODIFIER_OFF, true, 42, 120) ==
         252);
  assert(arp_preview_step_milliseconds(4, ARP_MODIFIER_OFF, false, 42, 120) ==
         125);
  return 0;
}
