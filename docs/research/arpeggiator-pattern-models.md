# Arpeggiator pattern models for the C100 MIDI Pad

Research date: 2026-09-04

## Recommendation

An arpeggiator is realistic on this keyboard. The clean model is a shared MIDI
processor, not a third performance mode:

```text
Scale notes ─┐
             ├─> held-note set ─> octave expansion ─> pattern ─> gate ─> MIDI
Chord tones ─┘
```

Scale and Chord remain the two performance modes. Tapping `ARP` should open its
settings page; holding `ARP` should toggle the processor on or off. The `ARP`
key can use fixed semantic colors outside the selected theme—green when enabled
and red when disabled—because this is status rather than decoration.

For the first version, implement these patterns:

1. Up
2. Down
3. Up/Down (exclusive)
4. Down/Up (exclusive)
5. Thumb Up (low-anchor)
6. Pinky Up (high-anchor)
7. Played Order
8. Random Other (random permutation)
9. Random Once
10. Chord

Outside-In and Inside-Out are worthwhile second-page additions. Inclusive
ping-pong can follow later because its only distinction is whether the turning
notes repeat.

This vocabulary is established rather than invented. Ableton exposes Up, Down,
UpDown, DownUp, Converge, Diverge, Pinky, Thumb, Play Order, Chord Trigger, and
three random modes. Novation exposes Up, Down, two Up/Down conventions, Random,
Played, and Chord. Apple Logic documents Up, Down, Up/Down, Outside-In, Random,
and As Played. [Ableton Live MIDI Effect Reference](https://www.ableton.com/en/live-manual/11/live-midi-effect-reference/),
[Novation SL MkIII Arpeggiator guide](https://userguides.novationmusic.com/hc/en-gb/articles/25626813121298-Using-the-SL-MkIII-s-Arpeggiators-Arp),
[Apple Logic Pro note-order parameters](https://support.apple.com/en-ca/guide/logicpro/lgce129c3fbe/mac)

## Exact pattern definitions

Let the distinct active pitches, sorted low to high, be:

```text
S = [1, 2, ... N]
```

The numbers below are positions in that set, not scale degrees. Therefore every
definition works for any number of held notes and for chromatic chord tones.

| UI name | One cycle for arbitrary `N` | Three-note example |
|---|---|---|
| Up | `1, 2, ... N` | `123` |
| Down | `N, N-1, ... 1` | `321` |
| Up/Down | `1 ... N, N-1 ... 2` | `1232` |
| Down/Up | `N ... 1, 2 ... N-1` | `3212` |
| Up/Down + ends | `1 ... N, N ... 1` | `123321` |
| Down/Up + ends | `N ... 1, 1 ... N` | `321123` |
| Thumb Up | for each `k = 2 ... N`, emit `1, k` | `1213` |
| Pinky Up | for each `k = 1 ... N-1`, emit `k, N` | `1323` |
| Outside-In | `N, 1, N-1, 2, ...` | `312` |
| Played Order | trigger-order IDs, not pitch order | depends on playing |
| Chord | all current notes simultaneously | `[123]` |

This directly maps the five requested examples:

- `123,123...` is **Up**.
- `321,321...` is **Down**.
- `1232,1232...` is endpoint-exclusive **Up/Down**.
- `1213,1213...` is **Thumb Up**, a low-anchor alternation.
- `3212,3212...` is endpoint-exclusive **Down/Up**, not a high-anchor
  pattern. A literal high-anchor pattern must revisit the high note, so the
  established **Pinky Up** pattern is `1323` for three notes.

The exclusive/inclusive distinction is standard hardware behavior. Novation
calls no repeated turning notes “Up/Down 1” and repeated high/low notes
“Up/Down 2”; Arturia calls the same concepts Exclusive and Inclusive.
[Novation SL MkIII Arpeggiator guide](https://userguides.novationmusic.com/hc/en-gb/articles/25626813121298-Using-the-SL-MkIII-s-Arpeggiators-Arp),
[Arturia DX7 V manual, pp. 16–17](https://downloads.arturia.com/products/dx7-v/manual/dx7-v_Manual_1_0_EN.pdf)

Ableton's diagrams establish the Thumb and Pinky families, while Outside-In has
an exact verbal definition in Logic: highest, lowest, second-highest,
second-lowest, and so on. [Ableton Live MIDI Effect Reference](https://www.ableton.com/en/live-manual/11/live-midi-effect-reference/),
[Apple Logic Pro note-order parameters](https://support.apple.com/en-ca/guide/logicpro/lgce129c3fbe/mac)

### Inside-Out

Logic documents two useful center-out variations. For six notes they are
`4,3,5,2,6,1` (high-center first) and `3,4,2,5,1,6` (low-center first).
An arbitrary-`N` implementation uses two cursors around the center and skips a
duplicate center when `N` is odd. This is a clearer UI label than “Diverge,”
whose meaning is not consistent across every product.
[Apple Logic Pro note-order variations](https://support.apple.com/en-ie/guide/logicpro/lgce2a5ad7b7/mac)

### Random modes

Three established behaviors are useful:

- **Random**: independently choose a note every step; immediate repeats are
  possible.
- **Random Other**: shuffle the current set and exhaust the shuffle before
  reshuffling, so every note is heard once per cycle.
- **Random Once**: make one shuffle and repeat it until the input-note set
  changes.

Ableton documents all three semantics. “Random Other” is the best default
because it sounds random without starving a held note. [Ableton Live MIDI Effect Reference](https://www.ableton.com/en/live-manual/11/live-midi-effect-reference/)

## Small-set behavior

Define these cases explicitly rather than relying on modulo arithmetic:

| Note count | Behavior |
|---|---|
| `N = 0` | Send Note Off for the current arp output and wait. Do not advance the pattern. |
| `N = 1` | Every melodic pattern repeats that one note. Chord repeats a one-note block. |
| `N = 2` | Exclusive Up/Down collapses to `12`; exclusive Down/Up to `21`. Thumb Up and Pinky Up also collapse to `12`. |

For inclusive two-note ping-pong, retaining its defining endpoint repeats gives
`1221` or `2112`. This is musically different enough to justify a separate
pattern only if users ask for it. Arturia also specifies that one held note is
repeated and two or more notes alternate. [Arturia DX7 V manual, pp. 16–17](https://downloads.arturia.com/products/dx7-v/manual/dx7-v_Manual_1_0_EN.pdf)

## Notes changing while the arp runs

The stable behavior is **continuous update**, not restarting on every key
change. Logic adds new notes seamlessly, removes released notes, and only stops
when no notes remain unless Latch is active. Novation likewise preserves timing
and direction when an endpoint disappears or the pattern type changes.
[Apple Logic Pro note-order parameters](https://support.apple.com/en-ca/guide/logicpro/lgce129c3fbe/mac),
[Novation SL MkIII Arpeggiator guide](https://userguides.novationmusic.com/hc/en-gb/articles/25626813121298-Using-the-SL-MkIII-s-Arpeggiators-Arp)

Recommended rules:

1. On `0 → 1` active notes, start at the pattern's defined first note on the
   next step boundary.
2. On additions, keep phase and direction. The new pitch becomes eligible on
   the next step.
3. On removal of a note that is currently sounding, send its Note Off
   immediately. Choose the next surviving pitch in the current direction.
4. If a boundary note disappears in a ping-pong pattern, turn around only when
   the remaining set's new boundary is reached.
5. On `1 → 0`, stop and reset the melodic cursor. Transport phase may continue,
   so the next gesture can still enter on-grid.
6. Changing pattern should preserve clock phase but restart the pattern cursor
   at the next step. This makes the audible result deterministic.

For Played Order, assign a small monotonically increasing ID when a distinct
pitch's reference count changes from zero to one. Remove that ID when its count
returns to zero. Logic uses position IDs for the same purpose, while Ableton
defines Play Order as trigger order. [Apple Logic Pro note-order parameters](https://support.apple.com/en-ca/guide/logicpro/lgce129c3fbe/mac),
[Ableton Live MIDI Effect Reference](https://www.ableton.com/en/live-manual/11/live-midi-effect-reference/)

The C100's duplicate scale pads must remain reference-counted: pressing two
pads that represent the same MIDI pitch should create one arp member, and that
member remains until the final duplicate is released.

## Octave expansion

For an octave range of `O`, create candidates at `pitch + 12 × octave` for
`octave = 0 ... O-1`, discard values above MIDI note 127, and de-duplicate the
result. Then apply these conventions:

- Pitch patterns (Up, Down, ping-pong, Thumb/Pinky, Outside/Inside) traverse the
  entire expanded pitch set.
- Played Order emits the played sequence at the original octave, then repeats
  that sequence at each higher octave.
- Random chooses across the entire expanded set.
- Chord emits the base chord as a block, then each higher-octave block.

These match Novation's documented behavior: directional patterns span the full
octave range, Played runs the first octave before later octaves, Random uses the
whole range, and Chord advances one octave per block. Novation also de-duplicates
notes created by octave expansion. [Novation SL MkIII Arpeggiator guide](https://userguides.novationmusic.com/hc/en-gb/articles/25626813121298-Using-the-SL-MkIII-s-Arpeggiators-Arp)

Start with one to three octaves. This is enough to be useful without making the
settings page or the resulting register unpredictable.

## Clock, rate, gate, latch, and transport

### External MIDI clock

MIDI Timing Clock sends 24 pulses per quarter note. MIDI Start, Continue, and
Stop control sequence playback; Song Position Pointer can identify another
timeline position. [MIDI Association, “About MIDI — Part 3: MIDI Messages”](https://midi.org/about-midi-part-3midi-messages)

The useful exact divisions are:

| Rate | Clock pulses per arp step |
|---|---:|
| 1/4 | 24 |
| 1/8 | 12 |
| 1/8 triplet | 8 |
| 1/16 | 6 |
| 1/16 triplet | 4 |
| 1/32 | 3 |
| 1/32 triplet | 2 |

All of those divide 24 exactly. Dotted 1/32 is 4.5 pulses, so it needs a
fractional accumulator; it is sensible to leave dotted rates out of the first
firmware. Novation offers the same straight and triplet family from 1/4 through
1/32, and Logic additionally offers dotted values. [Novation SL MkIII Arpeggiator guide](https://userguides.novationmusic.com/hc/en-gb/articles/25626813121298-Using-the-SL-MkIII-s-Arpeggiators-Arp),
[Apple Logic Pro note-order parameters](https://support.apple.com/en-ca/guide/logicpro/lgce129c3fbe/mac)

Recommended transport behavior:

- **Start**: turn transport running on, clear clock remainder, restart the
  pattern at its first step, and silence any stale arp-owned voice.
- **Continue**: resume without resetting pattern position.
- **Stop**: immediately send Note Off for every arp-owned voice and pause. Keep
  the held/latch set, but do not emit steps until another Start or Continue.
- **Clock without Start**: update the tempo estimate but do not run in strict
  host-sync mode. A later compatibility option can allow clock-only hosts.
- **Lost clock**: after a conservative timeout, silence arp-owned voices and
  show a waiting state; silently switching to an internal clock would create a
  phase jump.

### Gate

Gate is note duration as a percentage of the current step duration. Novation
uses 1–100%; Ableton permits more than 100%, which overlaps notes for legato.
For the first firmware, 10–100% is simpler and guarantees Note Off before the
next Note On. A 75% default is a safe musical starting point.
[Novation SL MkIII Arpeggiator guide](https://userguides.novationmusic.com/hc/en-gb/articles/25626813121298-Using-the-SL-MkIII-s-Arpeggiators-Arp),
[Ableton Live MIDI Effect Reference](https://www.ableton.com/en/live-manual/11/live-midi-effect-reference/)

Estimate step duration from a short rolling average of incoming clock-pulse
intervals, then schedule Note Off at `step_ms × gate / 100`. Pulse-counting
alone makes gate resolution too coarse at fast divisions (a 1/32-triplet step
contains only two MIDI clock pulses).

### Latch

The least surprising initial latch is “replace on next gesture”:

- Latch off: the arp input is the physically held/generated note set.
- Latch on: releasing all keys leaves the set running; the next new scale note
  or chord gesture replaces it atomically.
- Turning Latch or ARP off clears the latched set and sends all required Note
  Off messages.

Ableton Hold similarly continues after release until another key is played.
Logic demonstrates that more elaborate Reset, Transpose, Add, Add Temporarily,
and Through latch modes are possible, but those can wait rather than crowding
the first settings page. [Ableton Live MIDI Effect Reference](https://www.ableton.com/en/live-manual/11/live-midi-effect-reference/),
[Apple Logic Pro Arpeggiator control parameters](https://support.apple.com/en-gb/guide/logicpro/lgce2a327604/mac)

Chord-generated tones must enter the latch as one batch. Treating the first
tone as the replacement gesture and the remaining tones as later gestures
would incorrectly reduce a chord to one note.

## Compact QMK implementation

This can be implemented without dynamic allocation or a large precomputed step
grid.

### State

```c
typedef struct {
    bool enabled;
    bool transport_running;
    bool latch;
    uint8_t pattern;
    uint8_t rate;
    uint8_t gate_percent;
    uint8_t octave_count;
    uint8_t direction;
    uint8_t cursor;
    uint8_t sounding_note;
    uint8_t input_count[128];
    uint8_t sorted_notes[128];
    uint8_t played_order[128];
    uint8_t note_count;
} arp_state_t;
```

The actual struct can use bitsets and smaller configured maxima, but even this
straightforward form is modest for the C100's Cortex-M4. A pattern generator can
calculate the next index from `cursor`, `direction`, and `N`; it does not need
to store an expanded `1232...` sequence. Random Other/Once need only a bounded
shuffle array and a tiny PRNG state.

### Integration seam

Refactor Scale and Chord note production into source-note press/release calls:

```text
arp disabled: source press/release -> existing tracked MIDI output
arp enabled:  source press/release -> arp input reference counts
```

Keep arp-owned output separate from source-note reference counts. On each step:

1. End the previous arp voice if still active.
2. Resolve the next pattern pitch.
3. Send its Note On.
4. Schedule its Note Off from the gate duration.

The current firmware already has a non-blocking event loop in
`matrix_scan_user()` for delayed chord strums, so the same style suits gate
deadlines and pending arp steps.

Keychron's pinned QMK fork already parses incoming real-time MIDI bytes without
disturbing the surrounding parser state and exposes
`midi_register_realtime_callback()`. Its USB MIDI setup initializes the receive
path and `midi_device_process()` drains it during QMK's MIDI task. Therefore no
USB descriptor or host helper application is required; the keymap needs to
register a small callback and consume its counters in the regular scan loop.
[Keychron QMK MIDI parser at the pinned commit](https://github.com/Keychron/qmk_firmware/blob/9ada9b7baecb9591c469b9b068146ac5891a480a/quantum/midi/midi_device.c),
[Keychron QMK MIDI callback API](https://github.com/Keychron/qmk_firmware/blob/9ada9b7baecb9591c469b9b068146ac5891a480a/quantum/midi/midi.c),
[Keychron QMK USB MIDI setup](https://github.com/Keychron/qmk_firmware/blob/9ada9b7baecb9591c469b9b068146ac5891a480a/quantum/midi/qmk_midi.c)

Keep the real-time callback short: record Start/Continue/Stop flags, increment a
saturating pending-clock counter, and timestamp Clock arrival. Perform pattern
generation and MIDI output in the scan loop. This avoids adding variable work
to the receive path and lets the scan loop catch up if several clock messages
arrive together.

### Required tests before flashing

Implement the pattern generator as a pure C module and host-test at least:

- every pattern for `N = 0, 1, 2, 3, 4, 7`;
- exact requested sequences `123`, `321`, `1232`, `3212`, and `1213`;
- inclusive versus exclusive endpoint behavior;
- duplicate physical pads contributing one pitch until the final release;
- removing the current, lowest, and highest pitch mid-cycle;
- adding a pitch mid-cycle without resetting clock phase;
- octave expansion, MIDI 127 clipping, and de-duplication;
- Clock division counts, Start reset, Continue resume, Stop/all-notes-off;
- gate Note Off before retriggering the same pitch;
- Scale and Chord producing identical arp behavior from the same pitch set;
- toggling ARP off while notes are sounding or latched;
- a multi-tone chord entering latch as one atomic gesture.

## Primary sources

- [Ableton Live 11 Reference Manual — Arpeggiator](https://www.ableton.com/en/live-manual/11/live-midi-effect-reference/)
- [Apple Logic Pro — Arpeggiator note-order parameters](https://support.apple.com/en-ca/guide/logicpro/lgce129c3fbe/mac)
- [Apple Logic Pro — Arpeggiator note-order variations](https://support.apple.com/en-ie/guide/logicpro/lgce2a5ad7b7/mac)
- [Apple Logic Pro — Arpeggiator control and latch parameters](https://support.apple.com/en-gb/guide/logicpro/lgce2a327604/mac)
- [Arturia DX7 V User Manual](https://downloads.arturia.com/products/dx7-v/manual/dx7-v_Manual_1_0_EN.pdf)
- [Novation SL MkIII — Using the Arpeggiators](https://userguides.novationmusic.com/hc/en-gb/articles/25626813121298-Using-the-SL-MkIII-s-Arpeggiators-Arp)
- [MIDI Association — About MIDI, Part 3: MIDI Messages](https://midi.org/about-midi-part-3midi-messages)
- [Keychron QMK fork — MIDI parser and callbacks at commit `9ada9b7`](https://github.com/Keychron/qmk_firmware/tree/9ada9b7baecb9591c469b9b068146ac5891a480a/quantum/midi)
