// SPDX-License-Identifier: GPL-2.0-or-later
#include <assert.h>
#include <string.h>
#include "../keyboards/keychron/c100_8k/keymaps/midi_pad/note_pool.h"

static uint32_t random_state = 42;
static uint32_t next_random(void) {
  random_state = random_state * 1664525U + 1013904223U;
  return random_state;
}
static uint8_t refs[128], order[64], order_count;
static void reference_add(uint8_t note) {
  if (refs[note]++ == 0 && order_count < 64) order[order_count++] = note;
}
static void reference_remove(uint8_t note) {
  if (!refs[note] || --refs[note]) return;
  for (unsigned i = 0; i < order_count; ++i) {
    if (order[i] == note) {
      --order_count;
      memmove(order + i, order + i + 1, order_count - i);
      break;
    }
  }
}
static void expect_view(note_pool_t *pool, bool played, uint8_t octaves) {
  uint8_t expected[64], expected_count = 0;
  // Deliberately simple reference: octave-major append, linear dedup, sort.
  for (unsigned octave = 0; octave <= octaves; ++octave) {
    for (unsigned i = 0; i < (played ? order_count : 128U); ++i) {
      if (!played && !refs[i]) continue;
      unsigned note = (played ? order[i] : i) + octave * 12;
      if (note > 127) continue;
      unsigned j = 0;
      while (j < expected_count && expected[j] != note) ++j;
      if (j == expected_count && expected_count < 64)
        expected[expected_count++] = note;
    }
  }
  if (!played) {
    for (unsigned i = 0; i < expected_count; ++i)
      for (unsigned j = i + 1; j < expected_count; ++j)
        if (expected[j] < expected[i]) {
          uint8_t temp = expected[i]; expected[i] = expected[j]; expected[j] = temp;
        }
  }
  for (unsigned repeat = 0; repeat < 2; ++repeat) {
    uint8_t count;
    const uint8_t *notes = note_pool_view(pool, played, octaves, &count);
    assert(count == expected_count);
    assert(memcmp(notes, expected, count) == 0);
  }
  unsigned sources = 0;
  for (unsigned i = 0; i < 128; ++i) sources += refs[i] != 0;
  assert(pool->source_count == sources);
}

int main(void) {
  note_pool_t pool = {0};
  for (unsigned step = 0; step < 20000; ++step) {
    uint32_t value = next_random();
    uint8_t note = (value >> 8) % 128;
    if ((value & 3) != 0 && refs[note] < 90) {
      note_pool_add(&pool, note); reference_add(note);
    } else {
      note_pool_remove(&pool, note); reference_remove(note);
    }
    expect_view(&pool, (value >> 16) & 1, (value >> 17) % 3);
    if (step % 1000 == 999) {
      note_pool_clear(&pool);
      memset(refs, 0, sizeof(refs));
      order_count = 0;
      expect_view(&pool, false, 0);
    }
  }
  return 0;
}
