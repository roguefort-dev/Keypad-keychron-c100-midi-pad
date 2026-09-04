// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdbool.h>
#include <stdint.h>

enum arp_pattern {
  ARP_PATTERN_UP,
  ARP_PATTERN_DOWN,
  ARP_PATTERN_UP_DOWN,
  ARP_PATTERN_DOWN_UP,
  ARP_PATTERN_THUMB,
  ARP_PATTERN_PINKY,
  ARP_PATTERN_PLAYED,
  ARP_PATTERN_CHORD,
  ARP_PATTERN_COUNT,
};

enum arp_modifier {
  ARP_MODIFIER_OFF,
  ARP_MODIFIER_DOTTED,
  ARP_MODIFIER_TRIPLET,
  ARP_MODIFIER_COUNT,
};

enum arp_clock_driver {
  ARP_CLOCK_DRIVER_STOPPED,
  ARP_CLOCK_DRIVER_INTERNAL,
  ARP_CLOCK_DRIVER_HOST,
};

static inline uint8_t arp_pattern_length(enum arp_pattern pattern,
                                         uint8_t note_count) {
  if (note_count < 2 || pattern == ARP_PATTERN_CHORD ||
      pattern == ARP_PATTERN_UP || pattern == ARP_PATTERN_DOWN ||
      pattern == ARP_PATTERN_PLAYED) {
    return note_count;
  }
  return (note_count - 1) * 2;
}

static inline uint8_t arp_pattern_note_index(enum arp_pattern pattern,
                                             uint8_t step,
                                             uint8_t note_count) {
  if (note_count == 0) {
    return 0;
  }
  if (note_count == 1 || pattern == ARP_PATTERN_UP ||
      pattern == ARP_PATTERN_PLAYED || pattern == ARP_PATTERN_CHORD) {
    return step % note_count;
  }
  if (pattern == ARP_PATTERN_DOWN) {
    return note_count - 1 - (step % note_count);
  }

  const uint8_t pair_step = step % ((note_count - 1) * 2);
  switch (pattern) {
  case ARP_PATTERN_UP_DOWN:
    return pair_step < note_count ? pair_step : (note_count * 2) - 2 - pair_step;
  case ARP_PATTERN_DOWN_UP:
    return pair_step < note_count ? note_count - 1 - pair_step
                                  : pair_step - note_count + 1;
  case ARP_PATTERN_THUMB:
    return (pair_step & 1) ? (pair_step / 2) + 1 : 0;
  case ARP_PATTERN_PINKY:
    return (pair_step & 1) ? note_count - 1 : pair_step / 2;
  default:
    return 0;
  }
}

// MIDI clock is 24 pulses per quarter note. Twenty-four units per clock keep
// straight, dotted, and triplet timing exact through a 1/64 division.
static inline uint16_t arp_step_clock_units(uint8_t division,
                                            enum arp_modifier modifier) {
  const uint8_t safe_division = division < 7 ? division : 6;
  const uint16_t base = 2304U >> safe_division;
  if (modifier == ARP_MODIFIER_DOTTED) {
    return (base * 3) / 2;
  }
  if (modifier == ARP_MODIFIER_TRIPLET) {
    return (base * 2) / 3;
  }
  return base;
}

static inline uint32_t arp_step_milliseconds(uint8_t division,
                                             enum arp_modifier modifier,
                                             uint16_t bpm) {
  const uint8_t safe_division = division < 7 ? division : 6;
  const uint16_t denominator = 1U << safe_division;
  uint32_t duration = 240000UL /
                      ((bpm == 0 ? 120UL : (uint32_t)bpm) * denominator);
  if (modifier == ARP_MODIFIER_DOTTED) {
    duration = (duration * 3) / 2;
  } else if (modifier == ARP_MODIFIER_TRIPLET) {
    duration = (duration * 2) / 3;
  }
  return duration == 0 ? 1 : duration;
}

static inline bool arp_gate_is_legato(uint16_t length_per_mille) {
  return length_per_mille > 1000U;
}

static inline uint32_t arp_gate_milliseconds(uint32_t step_ms,
                                             uint16_t length_per_mille) {
  const uint32_t numerator = step_ms * length_per_mille;
  uint32_t gate = numerator / 1000U;
  if (arp_gate_is_legato(length_per_mille) && numerator % 1000U != 0) {
    ++gate;
  }
  return gate == 0 ? 1 : gate;
}

static inline bool arp_host_clock_is_active(bool host_clock_enabled,
                                            bool host_clock_seen,
                                            uint32_t elapsed_since_clock,
                                            uint32_t timeout_ms) {
  return host_clock_enabled && host_clock_seen &&
         elapsed_since_clock <= timeout_ms;
}

static inline enum arp_clock_driver arp_clock_driver_for_state(
    bool host_clock_enabled, bool host_clock_active, bool transport_running) {
  (void)transport_running;
  return host_clock_enabled && host_clock_active ? ARP_CLOCK_DRIVER_HOST
                                                 : ARP_CLOCK_DRIVER_INTERNAL;
}

static inline uint32_t arp_preview_step_milliseconds(
    uint8_t division, enum arp_modifier modifier, bool host_clock_active,
    uint16_t host_clock_period_ms, uint16_t internal_bpm) {
  if (host_clock_active) {
    return ((uint32_t)arp_step_clock_units(division, modifier) *
            host_clock_period_ms) /
           24;
  }
  return arp_step_milliseconds(division, modifier, internal_bpm);
}
