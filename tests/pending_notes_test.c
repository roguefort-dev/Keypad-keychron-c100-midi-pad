// SPDX-License-Identifier: GPL-2.0-or-later
#include <assert.h>
#include "../keyboards/keychron/c100_8k/keymaps/midi_pad/pending_notes.h"
static struct { bool active; uint8_t slot, tone; uint32_t due; } reference[64];
static uint8_t expected_slot[64], expected_tone[64], count, emitted;
static void emit(uint8_t slot, uint8_t tone) {
  assert(emitted < count);
  assert(slot == expected_slot[emitted] && tone == expected_tone[emitted]);
  ++emitted;
}
int main(void) {
  pending_notes_t queue = {0};
  uint32_t now = UINT32_MAX - 200, random = 17;
  for (unsigned step = 0; step < 100000; ++step) {
    random = random * 1664525U + 1013904223U;
    uint8_t slot = (random >> 8) % 21, tone = (random >> 16) % 15;
    if (random % 5 < 3) {
      uint32_t due = now + ((random >> 16) % 800);
      unsigned i = 0;
      while (i < 64 && reference[i].active) ++i;
      assert(pending_notes_add(&queue, slot, tone, due) == (i < 64));
      if (i < 64) {
        reference[i].active = true; reference[i].slot = slot;
        reference[i].tone = tone; reference[i].due = due;
      }
    } else if (random % 5 == 3) {
      pending_notes_cancel(&queue, slot);
      for (unsigned i = 0; i < 64; ++i)
        if (reference[i].slot == slot) reference[i].active = false;
    } else {
      now += 7;
      count = emitted = 0;
      for (unsigned i = 0; i < 64; ++i) {
        if (reference[i].active && (int32_t)(now - reference[i].due) >= 0) {
          reference[i].active = false;
          expected_slot[count] = reference[i].slot;
          expected_tone[count++] = reference[i].tone;
        }
      }
      pending_notes_run(&queue, now, emit);
      assert(emitted == count);
    }
    if (step % 10000 == 9999) {
      pending_notes_clear(&queue);
      memset(reference, 0, sizeof(reference));
    }
  }
  // Full queue, simultaneous deadlines, cancellation, reuse, and timer wrap.
  pending_notes_clear(&queue);
  for (unsigned i = 0; i < 64; ++i)
    assert(pending_notes_add(&queue, i % 21, i % 15, 3));
  assert(!pending_notes_add(&queue, 0, 0, 4));
  count = emitted = 0;
  pending_notes_run(&queue, UINT32_MAX - 1, emit);
  for (unsigned i = 0; i < 64; ++i) {
    expected_slot[i] = i % 21; expected_tone[i] = i % 15;
  }
  count = 64;
  pending_notes_run(&queue, 3, emit);
  assert(emitted == count);
  return 0;
}
