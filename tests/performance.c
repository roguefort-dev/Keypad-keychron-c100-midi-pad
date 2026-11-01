// SPDX-License-Identifier: GPL-2.0-or-later
#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "keymap.c"

#ifdef NOTE_POOL_CAPACITY
static uint8_t arp_build_note_list(uint8_t *notes) {
  uint8_t count;
  const uint8_t *view = note_pool_view(&arp_sources,
      arp_selected_pattern == ARP_PATTERN_PLAYED, arp_extra_octaves, &count);
  memcpy(notes, view, count);
  return count;
}
#endif
extern uint64_t host_midi_hash, host_midi_events;
static volatile uint64_t sink;

static uint64_t nanos(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

static void key(uint16_t code, uint8_t row, uint8_t col, bool pressed) {
  keyrecord_t record = {.event = {.key = {row, col}, .pressed = pressed}};
  host_keys[row][col] = pressed;
  process_record_user(code, &record);
}

static uint64_t hash_bytes(uint64_t hash, const void *data, size_t size) {
  const uint8_t *bytes = data;
  while (size--) hash = (hash ^ *bytes++) * 1099511628211ULL;
  return hash;
}

static void frame(uint64_t *hash) {
  rgb_matrix_indicators_user();
  *hash = hash_bytes(*hash, host_leds, sizeof(host_leds));
}

// Deterministic replay of real key events, scans, clocks and final LED frames.
// The same source runs against the baseline and optimized keymaps.
static void replay(void) {
  uint64_t hash = 1469598103934665603ULL;
  for (uint8_t scale = 0; scale < SCALE_MODE_COUNT; ++scale) {
    select_scale(scale);
    for (uint8_t root = 0; root < 12; ++root) {
      select_root(root);
      for (int octave = -2; octave <= 2; ++octave) {
        octave_shift = octave;
        layer_move(SCALE_LAYER);
        host_now += 500;
        for (uint8_t row = 1; row < 10; ++row) {
          key(MD_NOTE, row, row, true);
          key(MD_NOTE, row, (row + 3) % 10, true);
          frame(&hash);
          key(MD_NOTE, row, row, false);
          key(MD_NOTE, row, (row + 3) % 10, false);
        }
        layer_move(CHORD_LAYER);
        host_now += 500;
        for (uint8_t shape = 0; shape < CHORD_PRESET_COUNT; ++shape) {
          select_chord_shape(shape);
          chord_quantize = shape % 3;
          chord_auto = shape % 2;
          chord_drop = shape % 4;
          chord_open = (shape / 4) % 4;
          chord_bass = shape % 3;
          chord_inversion = shape % 5;
          chord_strum = shape % 7;
          for (uint8_t col = 7; col < 10; ++col) {
            press_chord_root(9, col);
            host_now += 23;
            matrix_scan_user();
            frame(&hash);
          }
          host_now += 1000;
          matrix_scan_user();
          frame(&hash);
          stop_all_chord_slots();
        }
      }
    }
  }
  // Custom/rootless chords, preset extensions, latch and live revoicing.
  release_tracked_notes();
  layer_move(CHORD_LAYER);
  for (unsigned i = 0; i < 2000; ++i) {
    if (i % 20 == 0) {
      select_scale((i / 20) % SCALE_MODE_COUNT);
      select_root((i / 40) % 12);
      selected_chord_shape = -1;
      custom_degree_mask = (i * 37) & ((1U << CHORD_DEGREE_COUNT) - 1);
      memset(custom_degree_octave_lifts, 0, sizeof(custom_degree_octave_lifts));
    }
    press_chord_root(3 + i % 7, 7 + i % 3);
    toggle_chord_degree(i % CHORD_DEGREE_COUNT);
    if (i % 3 == 0) {
      select_chord_shape(i % CHORD_PRESET_COUNT);
      toggle_chord_degree((i + 3) % CHORD_DEGREE_COUNT);
    }
    if (i % 5 == 0) toggle_chord_latch();
    host_now += 100;
    matrix_scan_user();
    frame(&hash);
    release_chord_root(3 + i % 7, 7 + i % 3);
    if (i % 17 == 0) release_tracked_notes();
  }
  // Arp order, duplicate sources, clipping, capacity, and cache invalidation.
  for (uint8_t pattern = 0; pattern < ARP_PATTERN_COUNT; ++pattern) {
    arp_set_enabled(true);
    arp_selected_pattern = pattern;
    for (uint8_t oct = 0; oct < 3; ++oct) {
      arp_extra_octaves = oct;
      arp_clear_source();
      for (unsigned i = 0; i < 128; ++i) {
        uint8_t note = (i * 37) % 128;
        arp_source_add(note);
        if (i % 3 == 0) arp_source_add(note);
        uint8_t notes[64];
        uint8_t count = arp_build_note_list(notes);
        hash = hash_bytes(hash, notes, count);
        arp_play_step();
      }
      for (unsigned i = 0; i < 128; ++i) {
        uint8_t note = (i * 29) % 128;
        arp_source_remove(note);
        if (i % 3 == 0) arp_source_remove(note);
        arp_play_step();
      }
      arp_clear_source();
    }
  }
  // Host clock, fallback, gates, duplicate pad presses and live controls.
  release_tracked_notes();
  layer_move(SCALE_LAYER);
  arp_set_enabled(true);
  for (unsigned i = 0; i < 20000; ++i) {
    if (i % 100 == 0) key(MD_NOTE, 9, (i / 100) % 10, true);
    if (i % 100 == 75) key(MD_NOTE, 9, (i / 100) % 10, false);
    if (i % 1000 == 0) arp_cycle_length();
    if (i % 700 == 0) arp_cycle_octaves();
    if (i % 900 == 0) arp_change_pattern(1);
    if (i % 300 == 0) arp_change_division(1);
    if (i % 800 == 0) arp_cycle_modifier();
    if (i % 2000 < 1000 && i % 3 == 0)
      arp_realtime_callback(&midi_device, MIDI_CLOCK);
    if (i % 2100 == 0) arp_realtime_callback(&midi_device, MIDI_STOP);
    if (i % 2200 == 0) arp_realtime_callback(&midi_device, MIDI_START);
    host_now += 7;
    matrix_scan_user();
    frame(&hash);
  }
  arp_set_enabled(false);
  // Text, arrows, previews, all animations, transitions, presses, timer wrap.
  for (uint8_t layer = 0; layer <= SETTINGS_LAYER; ++layer) {
    layer_move(layer);
    for (unsigned i = 0; i < 1000; ++i) {
      if (i % 100 == 0) {
        change_palette(1);
        change_scale_page(PAGE_ARROW_DOWN);
        key(ANIMATION_TRIGGER, (i / 100) % 10, i % 10, true);
        arp_change_pattern(1);
      }
      host_now += 17;
      if (i == 500) host_now = UINT32_MAX - 30;
      matrix_scan_user();
      frame(&hash);
    }
  }
  release_tracked_notes();
  hash = hash_bytes(hash, active_note_counts, sizeof(active_note_counts));
  printf("replay frames=%016llx midi=%016llx events=%llu\n",
         (unsigned long long)hash, (unsigned long long)host_midi_hash,
         (unsigned long long)host_midi_events);
}

static void bench_scan(void) { matrix_scan_user(); }
static void bench_frame(void) { rgb_matrix_indicators_user(); }
static void bench_arp(void) { arp_play_step(); }
static void bench_arp_edit(void) {
  arp_source_add(43);
  arp_play_step();
  arp_source_remove(43);
  arp_play_step();
}
static void bench_voice(void) {
  uint8_t notes[CHORD_MAX_TONES];
  uint16_t mask;
  uint8_t count = build_chord_notes(52, notes, &mask);
  sink += count + mask + notes[0];
}
static void measure(const char *name, void (*run)(void), unsigned iterations) {
  for (unsigned i = 0; i < 1000; ++i) run();
  uint64_t writes = host_led_writes;
  uint64_t start = nanos();
  for (unsigned i = 0; i < iterations; ++i) run();
  uint64_t elapsed = nanos() - start;
  sink += host_leds[99][0] + host_midi_hash;
  printf("%s,%.3f,%.2f\n", name, (double)elapsed / iterations,
         (double)(host_led_writes - writes) / iterations);
}

int main(int argc, char **argv) {
  keyboard_post_init_user();
  if (argc > 1 && strcmp(argv[1], "--replay") == 0) { replay(); return 0; }
  puts("scenario,ns_per_operation,led_writes_per_operation");
  measure("scan_idle", bench_scan, 2000000);
  arp_set_enabled(true);
  measure("scan_arp_empty", bench_scan, 2000000);
  arp_source_add(100);
  measure("scan_arp_held", bench_scan, 2000000);
  arp_set_enabled(false);
  measure("rgb_scale", bench_frame, 100000);
  layer_move(CHORD_LAYER);
  host_now += 1000;
  measure("rgb_chord_idle", bench_frame, 100000);
  selected_chord_shape = 34;
  for (uint8_t col = 7; col < 10; ++col) press_chord_root(9, col);
  measure("rgb_chord_held", bench_frame, 100000);
  chord_auto = true; chord_open = 2; chord_drop = 3;
  measure("voice_auto", bench_voice, 50000);
  chord_strum = 3;
  start_chord_slot(0, 40);
  measure("scan_strum_pending", bench_scan, 2000000);
  release_tracked_notes();
  arp_set_enabled(true);
  arp_extra_octaves = 2;
  for (uint8_t note = 36; note < 60; note += 3) arp_source_add(note);
  measure("arp_step_8_sources", bench_arp, 200000);
  measure("arp_source_edit_and_2_steps", bench_arp_edit, 100000);
  for (uint8_t note = 0; note < 128; ++note) arp_source_add(note);
  measure("arp_step_saturated", bench_arp, 100000);
  arp_set_enabled(false);
  layer_move(SETTINGS_LAYER);
  host_now += 1000;
  start_text_scroll(SCROLL_SCALE_NAME);
  settings_display = DISPLAY_SCALE;
  measure("rgb_settings", bench_frame, 100000);
  layer_move(ANIMATION_PROTOTYPE_LAYER);
  animation_prototype = 7;
  animation_prototype_active = true;
  animation_prototype_timer = host_now;
  host_now += 200;
  measure("rgb_animation_transition", bench_frame, 100000);
  return 0;
}
