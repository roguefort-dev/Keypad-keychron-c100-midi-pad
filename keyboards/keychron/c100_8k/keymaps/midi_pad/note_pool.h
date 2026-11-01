// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define NOTE_POOL_CAPACITY 64
#define NOTE_POOL_PITCHES 128

// Fixed storage for reference-counted sources and their octave-expanded view.
// Only membership/order changes invalidate the view; repeated notes do not.
// Callers supply MIDI pitches 0..127, at most 255 references per pitch and
// octave expansion 0..2. The returned view lasts until the next mutation/view.
typedef struct {
  uint32_t membership[4];
  uint8_t references[NOTE_POOL_PITCHES];
  uint8_t order[NOTE_POOL_CAPACITY];
  uint8_t notes[NOTE_POOL_CAPACITY];
  uint8_t source_count;
  uint8_t order_count;
  uint8_t note_count;
  uint8_t cached_octaves;
  bool cached_played;
  bool valid;
} note_pool_t;

static inline void note_pool_clear(note_pool_t *pool) {
  memset(pool, 0, sizeof(*pool));
}

static inline void note_pool_add(note_pool_t *pool, uint8_t note) {
  if (pool->references[note]++ != 0) return;
  ++pool->source_count;
  pool->membership[note / 32] |= UINT32_C(1) << (note % 32);
  pool->valid = false;
  if (pool->order_count < NOTE_POOL_CAPACITY)
    pool->order[pool->order_count++] = note;
}

static inline void note_pool_remove(note_pool_t *pool, uint8_t note) {
  if (pool->references[note] == 0 || --pool->references[note] != 0) return;
  --pool->source_count;
  pool->membership[note / 32] &= ~(UINT32_C(1) << (note % 32));
  pool->valid = false;
  for (uint8_t i = 0; i < pool->order_count; ++i) {
    if (pool->order[i] != note) continue;
    --pool->order_count;
    memmove(&pool->order[i], &pool->order[i + 1], pool->order_count - i);
    break;
  }
}

static inline bool note_pool_append(note_pool_t *pool, uint32_t *seen,
                                     uint16_t note) {
  if (note >= NOTE_POOL_PITCHES) return false;
  const uint32_t bit = UINT32_C(1) << (note % 32);
  if (seen[note / 32] & bit) return false;
  seen[note / 32] |= bit;
  pool->notes[pool->note_count++] = note;
  return pool->note_count == NOTE_POOL_CAPACITY;
}

// Preserve octave-major admission order even when the 64-note view fills.
// Sorting before truncating would change which notes play in dense chords.
static inline const uint8_t *note_pool_view(note_pool_t *pool, bool played,
                                           uint8_t octaves, uint8_t *count) {
  if (!pool->valid || pool->cached_played != played ||
      pool->cached_octaves != octaves) {
    uint32_t seen[4] = {0};
    pool->note_count = 0;
    for (uint8_t octave = 0; octave <= octaves; ++octave) {
      if (played) {
        for (uint8_t i = 0; i < pool->order_count; ++i)
          if (note_pool_append(pool, seen, pool->order[i] + octave * 12))
            goto filled;
      } else {
        for (uint8_t word = 0; word < 4; ++word) {
          uint32_t bits = pool->membership[word];
          while (bits) {
            uint8_t source = word * 32 + __builtin_ctz(bits);
            if (note_pool_append(pool, seen, source + octave * 12))
              goto filled;
            bits &= bits - 1;
          }
        }
      }
    }
  filled:
    if (!played) {
      // The bitset is already ordered by MIDI pitch: no insertion sort.
      uint8_t index = 0;
      for (uint8_t word = 0; word < 4; ++word) {
        uint32_t bits = seen[word];
        while (bits) {
          pool->notes[index++] = word * 32 + __builtin_ctz(bits);
          bits &= bits - 1;
        }
      }
    }
    pool->cached_played = played;
    pool->cached_octaves = octaves;
    pool->valid = true;
  }
  *count = pool->note_count;
  return pool->notes;
}
