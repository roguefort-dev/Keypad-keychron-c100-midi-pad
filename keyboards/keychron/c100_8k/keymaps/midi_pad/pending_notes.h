// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define PENDING_NOTES_CAPACITY 64

// Structure-of-arrays avoids padding every event to eight bytes. Bitmaps keep
// free-slot allocation and iteration bounded by live events, with no heap.
typedef struct {
  uint32_t due[PENDING_NOTES_CAPACITY];
  uint8_t slot[PENDING_NOTES_CAPACITY];
  uint8_t tone[PENDING_NOTES_CAPACITY];
  uint32_t used[2];
  uint32_t next_due;
} pending_notes_t;

static inline void pending_notes_clear(pending_notes_t *queue) {
  queue->used[0] = queue->used[1] = 0;
}

static inline bool pending_notes_add(pending_notes_t *queue, uint8_t slot,
                                     uint8_t tone, uint32_t due) {
  for (uint8_t word = 0; word < 2; ++word) {
    uint32_t free_bits = ~queue->used[word];
    if (!free_bits) continue;
    uint8_t bit = __builtin_ctz(free_bits);
    uint8_t index = word * 32 + bit;
    if (!(queue->used[0] | queue->used[1]) ||
        (int32_t)(due - queue->next_due) < 0)
      queue->next_due = due;
    queue->due[index] = due;
    queue->slot[index] = slot;
    queue->tone[index] = tone;
    queue->used[word] |= UINT32_C(1) << bit;
    return true;
  }
  return false;
}

static inline void pending_notes_cancel(pending_notes_t *queue, uint8_t slot) {
  for (uint8_t word = 0; word < 2; ++word) {
    uint32_t bits = queue->used[word];
    while (bits) {
      uint8_t bit = __builtin_ctz(bits);
      if (queue->slot[word * 32 + bit] == slot)
        queue->used[word] &= ~(UINT32_C(1) << bit);
      bits &= bits - 1;
    }
  }
  // A cancelled earliest deadline may cause one early scan, never a late note.
}

// Deadlines must be less than 2^31 ms ahead, as with QMK timer comparisons.
// Emit in event-index order to preserve simultaneous Note On ordering.
// The emit callback must not mutate this queue.
static inline void pending_notes_run(pending_notes_t *queue, uint32_t now,
                                     void (*emit)(uint8_t, uint8_t)) {
  if (!(queue->used[0] | queue->used[1]) ||
      (int32_t)(now - queue->next_due) < 0) return;
  bool have_next = false;
  for (uint8_t word = 0; word < 2; ++word) {
    uint32_t bits = queue->used[word];
    while (bits) {
      uint8_t bit = __builtin_ctz(bits);
      uint8_t index = word * 32 + bit;
      if ((int32_t)(now - queue->due[index]) >= 0) {
        queue->used[word] &= ~(UINT32_C(1) << bit);
        emit(queue->slot[index], queue->tone[index]);
      } else if (!have_next ||
                 (int32_t)(queue->due[index] - queue->next_due) < 0) {
        queue->next_due = queue->due[index];
        have_next = true;
      }
      bits &= bits - 1;
    }
  }
}
