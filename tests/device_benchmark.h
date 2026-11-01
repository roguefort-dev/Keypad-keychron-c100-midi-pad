// SPDX-License-Identifier: GPL-2.0-or-later
// Injected only into disposable benchmark builds by tools/benchmark-device.
// Never included by the production keymap.
static volatile uint32_t device_bench_sink;
static void device_bench_scan(void) { matrix_scan_user(); }
static void device_bench_rgb(void) { rgb_matrix_indicators_user(); }
static void device_bench_rgb_flush(void) {
  rgb_matrix_indicators_user();
  rgb_matrix_driver.flush();
}
static void device_bench_voice(void) {
  uint8_t notes[CHORD_MAX_TONES];
  uint16_t mask;
  uint8_t count = build_chord_notes(52, notes, &mask);
  device_bench_sink += count + mask + notes[0];
}
static void device_bench_list(void) {
#ifdef NOTE_POOL_CAPACITY
  uint8_t count;
  const uint8_t *notes = note_pool_view(&arp_sources,
      arp_selected_pattern == ARP_PATTERN_PLAYED, arp_extra_octaves, &count);
#else
  uint8_t notes[ARP_MAX_SOURCE_NOTES];
  uint8_t count = arp_build_note_list(notes);
#endif
  device_bench_sink += count + notes[0];
}
static void device_bench_measure(uint8_t scenario, void (*run)(void),
                                  uint16_t iterations) {
  for (uint8_t i = 0; i < 10; ++i) run();
  for (uint8_t sample = 0; sample < 5; ++sample) {
    // Keep held-note scans away from the internal clock and strum deadline.
    arp_last_clock_timer = timer_read32();
    uint32_t start = DWT->CYCCNT;
    for (uint16_t i = 0; i < iterations; ++i) run();
    uint32_t elapsed = DWT->CYCCNT - start;
    // 7-bit SysEx payload: scenario, sample, iterations (2), cycles (5).
    // Two padding bytes accommodate QMK midi_send_array's final 3-byte read.
    uint8_t reply[18] = {0xF0, 0x7D, 'C', '1', '0', '0', 'P',
                         scenario, sample,
                         iterations & 127, (iterations >> 7) & 127,
                         elapsed & 127, (elapsed >> 7) & 127,
                         (elapsed >> 14) & 127, (elapsed >> 21) & 127,
                         (elapsed >> 28) & 127, 0xF7, 0};
    midi_send_array(&midi_device, 17, reply);
  }
}
static void device_bench_run(void) {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  release_tracked_notes();
  arp_enabled = false;
  arp_key_pressed = false;
  root_pitch_class = 4;
  selected_scale = SCALE_NATURAL_MINOR;
  selected_palette = PALETTE_TERMINAL;
  octave_shift = 0;
  layer_transition_active = false;
  layer_state = 1U << SCALE_LAYER;
  device_bench_measure(0, device_bench_scan, 1000);
  arp_enabled = true;
  device_bench_measure(1, device_bench_scan, 1000);
  arp_source_add(100);
  arp_host_clock = true;
  arp_host_clock_seen = true;
  device_bench_measure(2, device_bench_scan, 1000);
  arp_enabled = false;
  arp_clear_source();
  device_bench_measure(3, device_bench_rgb, 100);
  layer_state = 1U << CHORD_LAYER;
  device_bench_measure(4, device_bench_rgb, 100);
  selected_chord_shape = 34;
  // Render held voicings without sending notes to attached software.
  for (uint8_t col = 7; col < 10; ++col) {
    active_chord_t *chord = &active_chords[chord_root_slot(9, col)];
    chord->active = true;
    chord->root_note = chord_root_note_for_position(9, col);
    chord->note_count = build_chord_notes(chord->root_note, chord->notes,
                                         &chord->alternate_mask);
    previous_chord_note_count = chord->note_count;
    memcpy(previous_chord_notes, chord->notes, chord->note_count);
  }
#ifdef NOTE_POOL_CAPACITY
  chord_indicators_dirty = true;
#endif
  device_bench_measure(5, device_bench_rgb, 100);
  chord_auto = true;
  chord_open = 2;
  chord_drop = 3;
  device_bench_measure(6, device_bench_voice, 100);
  queue_chord_tone(0, 0, timer_read32() + 50000);
  device_bench_measure(7, device_bench_scan, 1000);
  release_tracked_notes();
  arp_extra_octaves = 2;
  arp_selected_pattern = ARP_PATTERN_UP;
  for (uint8_t note = 36; note < 60; note += 3) arp_source_add(note);
  device_bench_measure(8, device_bench_list, 1000);
  for (uint8_t note = 0; note < 128; ++note) arp_source_add(note);
  device_bench_measure(9, device_bench_list, 1000);
  release_tracked_notes();
  arp_enabled = false;
  layer_state = 1U << SCALE_LAYER;
  device_bench_measure(10, device_bench_rgb_flush, 100);
  // The caller restores production firmware after diagnostics. No EEPROM writes.
  uint8_t done[10] = {0xF0, 0x7D, 'C', '1', '0', '0', 'P', 127, 0xF7, 0};
  midi_send_array(&midi_device, 9, done);
}
