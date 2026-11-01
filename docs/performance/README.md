# C100 performance pass — 2026-09-06

All 13 local benchmark scenarios improved. Measurements on the connected C100 also confirmed improvements in all 11 device scenarios. These are timings of individual firmware operations, not a claim that the entire keyboard or physical-key-to-DAW latency improved by the same factor.

## Scope and architecture

The pass covered the custom keymap, scale/chord logic, arpeggiator, scheduling, every display/animation layer, persistence, build/test setup and flashing helper. The target-specific QMK scan/debounce, RGB scheduler, SNLED27351 SPI driver, USB MIDI transport and memory map were inspected to identify integration costs. The vendored QMK tree remains unchanged.

- **Arpeggiator note pool:** `note_pool.h` owns source reference counts, played order, active-pitch bitmaps and a cached octave-expanded view. Membership/order changes invalidate the view; pattern ordering and octave count are checked on access. Expansion visits active pitches, deduplicates with 128 bits and preserves the original first-64 admission policy before sorting. A view avoids copying a 64-byte list onto the step stack.
- **Pending notes:** `pending_notes.h` owns 64 fixed events in arrays of deadlines, chord slots and tone indexes. Two 32-bit occupancy words replace per-event flags/padding; the earliest deadline makes scans constant-time when nothing is due. Due events retain event-index order, cancellation and overflow behavior. Deadline comparisons handle timer wrap.
- **Musical lookups:** one cache keyed by scale, root and octave stores the 37 distinct grid degrees, pitch membership and degree indexes. Chord rendering uses two 128-bit maps, invalidated by chord changes, musical lookup changes or quantization changes, instead of repeatedly searching all chords for each LED.
- **Chord voicing:** inversion moves only the raised first tone through the sorted array. Auto advances from the previous candidate inversion instead of rebuilding all preceding inversions each time.
- **RGB ownership:** the keymap composes layers, transitions and press overlays in a fixed 300-byte frame, then writes each LED once. The dedicated QMK canvas effect does no background drawing. This prevents an invisible stock effect from dirtying driver buffers. Unchanged final pixels let the existing driver skip SPI transfers; suspend/resume still repaints through QMK.
- **No heap allocation:** both new modules and the custom keymap use fixed storage and bounded stack arrays. QMK still has its own allocator, including debounce initialization; this pass does not remove allocation from the entire framework.

## Local measurements

Baseline: `a6e47e80b9085789ef512d2784a2b5433572bb24`. QMK: `9ada9b7baecb9591c469b9b068146ac5891a480a` (Keychron `2025q3`). Host: AMD Ryzen 9 9955HX, x86-64. Both builds use `-Os`, matching the firmware optimization level. Seven samples alternate before/after execution, pinned to the same CPU, with warmup for every scenario. Values are medians in ns per operation. Compiler, ranges and metadata: [local-results.json](local-results.json).

The harness compiles the actual `keymap.c` against a small QMK adapter with a controllable clock, matrix, RGB writes and MIDI event collector. The adapter is a separate translation unit and observable sinks prevent dead-code removal. No USB or SPI hardware cost is simulated. The layout macro is stubbed: physical matrix wiring is validated by the real firmware build, not this harness.

| Scenario | Before (ns) | After (ns) | Speedup |
|---|---:|---:|---:|
| scan_idle | 31.0 | 4.1 | 7.62× |
| scan_arp_empty | 84.1 | 4.4 | 18.94× |
| scan_arp_held | 139.7 | 17.0 | 8.21× |
| rgb_scale | 1,862.6 | 1,388.3 | 1.34× |
| rgb_chord_idle | 1,940.3 | 1,368.1 | 1.42× |
| rgb_chord_held | 3,067.7 | 1,354.4 | 2.26× |
| voice_auto | 966.4 | 713.8 | 1.35× |
| scan_strum_pending | 32.6 | 4.8 | 6.80× |
| arp_step_8_sources | 374.3 | 28.7 | 13.03× |
| arp_source_edit_and_2_steps | 826.1 | 232.3 | 3.56× |
| arp_step_saturated | 8,291.9 | 28.8 | 288.02× |
| rgb_settings | 1,216.0 | 1,025.9 | 1.19× |
| rgb_animation_transition | 2,530.3 | 2,184.6 | 1.16× |

`arp_source_edit_and_2_steps` deliberately rebuilds the view after both an add and a remove. It caught a regression in the first draft; active-pitch iteration fixed it. The saturated case is a stress test, not typical playing. Steady frames drop from 160–310 LED setter calls to exactly 100.

## Device measurements

The connected AT32F405 C100 ran temporary baseline and optimized builds with the same diagnostic harness. Its Cortex-M4 DWT counter measured CPU cycles with interrupts enabled. Each scenario has five samples after warmup; measurements include function-call/loop overhead and interrupt activity. Results were returned over USB MIDI SysEx. Production firmware excludes all diagnostic hooks.

| Scenario | Before (cycles) | After (cycles) | Speedup |
|---|---:|---:|---:|
| scan_idle | 1,144.2 | 355.1 | 3.22× |
| scan_arp_empty | 2,574.8 | 366.1 | 7.03× |
| scan_arp_held | 2,591.1 | 684.1 | 3.79× |
| rgb_scale | 52,111.7 | 39,967.9 | 1.30× |
| rgb_chord_idle | 57,126.8 | 35,995.8 | 1.59× |
| rgb_chord_held | 101,743.0 | 35,517.1 | 2.86× |
| voice_auto | 20,885.2 | 16,258.5 | 1.28× |
| scan_strum_pending | 1,143.0 | 364.4 | 3.14× |
| arp_list_8_sources | 8,794.9 | 123.3 | 71.35× |
| arp_list_saturated | 190,675.5 | 123.2 | 1547.25× |
| rgb_scale_with_driver_flush | 266,109.8 | 40,038.0 | 6.65× |

Device arpeggiator rows measure note-list lookup only; local arpeggiator-step rows also handle gate/output state and collect MIDI events. Do not compare those ratios directly. The large saturated-list ratio comes from reusing a cached result instead of repeating deduplication and sorting.

`rgb_scale_with_driver_flush` includes the real LED-driver flush after rendering an unchanged scale frame. Previously the intermediate clear/redraw dirtied SPI buffers on every frame. The optimized version leaves those buffers unchanged, eliminating that transfer. Transfers still occur when LEDs change. Ordinary RGB rows time rendering only.

Raw cycle samples: [device-results.json](device-results.json). Original USB MIDI captures: [baseline.syx](baseline.syx), [optimized.syx](optimized.syx).

## Memory and firmware

| Item | Before | After | Change |
|---|---:|---:|---:|
| Production binary | 73,214 B | 73,982 B | +768 B |
| Static/reserved RAM before heap | 14,960 B | 15,312 B | +352 B |
| Pending-event storage | 512 B | 396 B | −116 B (22.7%) |

The small RAM increase buys the caches and RGB frame. RAM figures use linker heap-start addresses (`0x20003a70` → `0x20003bd0`) rather than summing `size` output: ChibiOS reserves remaining RAM for its heap, which makes raw BSS totals misleading. Existing stack reservations are unchanged. The production image occupies 56.4% of the verified 128 KiB flash region.

Production SHA-256: `e29dff4221dc5989ee000c09d372c07d1a41c671addbc46d3260daa1a00724fc`.

## Correctness and reproduction

- The before/after replay matches **1,349,732 MIDI events**, including timestamps and byte order, and LED frames after every sampled interaction. Coverage includes all 20 scales, 12 roots, five octave positions and 35 presets; custom/rootless chords, extensions, Auto/drop/open/bass/inversion/strum, latch, duplicate sources, saturation, arpeggiator patterns/octaves/gates, host-clock fallback, displays, transitions and timer wrap.
- Dedicated tests compare note-pool behavior to a simple reference model across 20,000 edits and pending-note behavior across 100,000 operations, plus full-queue and wrap cases. Existing arpeggiator and degree-logic tests still pass.
- AddressSanitizer and UndefinedBehaviorSanitizer pass. The standard test target also compares the complete replay against a checked-in baseline fingerprint.
- The ARM firmware builds successfully. Temporary diagnostics are replaced by the production image, and USB MIDI re-enumeration is checked after flashing.

```sh
make test
make test-sanitize
make benchmark BENCH_ARGS='--baseline-ref a6e47e80b9085789ef512d2784a2b5433572bb24'
make build
```

The benchmark runner writes individual CSVs, a frozen baseline source directory, executables and JSON under `.build/performance/local/`. `--optimization O2` is available for a separate desktop comparison.

To reproduce device measurements, prepare a fresh disposable userspace, build it and explicitly flash it:

```sh
./tools/benchmark-device .build/performance/local/baseline .build/performance/device-before-new
make -C .build/performance/device-before-new build QMK_HOME="$PWD/../qmk_firmware"
./tools/flash-c100 .build/performance/device-before-new/keychron_c100_8k_midi_pad.bin
amidi -l
# Substitute the current C100 port if it differs:
amidi -p hw:2,0,0 -r .build/performance/before.syx -S 'F0 7D 43 31 30 30 42 45 4E 43 48 F7' -t 3
./tools/decode-device-benchmark .build/performance/before.syx
```

Repeat with `keyboards/keychron/c100_8k/keymaps/midi_pad` as the source and a fresh destination for the optimized build. The benchmark temporarily changes in-memory performance state and should run while the pad is idle. It does not write selection settings to EEPROM. Restore normal operation with `make flash` afterward.

Host timings establish algorithmic improvements; device cycles validate the hardware direction and magnitude for the measured operations. Neither benchmark measures physical switch behavior, USB host scheduling, DAW buffering or audible latency.
