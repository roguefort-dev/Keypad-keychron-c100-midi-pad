# Hardware chord-mode interaction patterns

Research date: 2026-09-03. Sources are first-party manufacturer manuals, user guides, and support documents. “Design takeaways” are synthesis for a 10×10 square-key mechanical MIDI pad, not claims made by the manufacturers.

## Executive takeaways

- Separate **what plays** from **how it plays**. A stable chord area plus a smaller modifier area for inversion, spread, strum, split, or arpeggiation keeps the primary chord locations learnable.
- Offer two complementary chord sources: a scale-derived/generated bank for discovery and a small user bank for rehearsed progressions. Make saving deliberate and protect saved chords with a lock or edit state.
- Make voicing transforms momentary by default, with an explicit latch for one-handed use. Latch must have an unmistakable LED state and a single action that clears active modifiers.
- Treat root/key as persistent global context. Show it continuously, distinguish roots from other scale degrees by color, and make semitone and octave transposition available without leaving performance mode.
- Preserve spatial memory. A “root follows corner” layout helps beginners; a fixed-position option makes modulation less disruptive for practiced users.
- RGB feedback should answer four questions without a display: which keys are playable, which are roots, which chord/modifier is selected, and which notes are sounding. Chord names and exact notes are useful secondary display information, not a substitute for pad feedback.

## Product patterns

### Novation Launchpad Pro MK3

Launchpad Pro uses the grid for three jobs at once. Scale-relative triads occupy orange pads in ascending degree order; two white columns hold user-recorded chords; and five columns form a note/voicing area. In that note area, each row starts on a scale degree and proceeds through root, fifth, compound third, seventh, and compound fifth, so a player can build open, scale-valid voicings as a repeated spatial pattern. [Novation’s Chord Mode Guide](https://support.novationmusic.com/hc/en-gb/articles/360011206299-Launchpad-Pro-MK3-Chord-Mode-Guide)

Saving is direct: hold a white slot and play notes; Clear plus the slot removes it. A dedicated Chord Lock pad prevents accidental additions to saved slots, while the adjacent pink pad is held for sustain. This is a particularly compact combination of user favorites, edit protection, and momentary performance control. [Novation’s Chord Mode Guide](https://support.novationmusic.com/hc/en-gb/articles/360011206299-Launchpad-Pro-MK3-Chord-Mode-Guide)

Color carries harmonic meaning: in most seven-note scales, blue indicates major, purple minor, green diminished or augmented, and lighter top/bottom rows identify the root degree. The scale itself is selected through Shift + Note. [Novation’s Chord Mode Guide](https://support.novationmusic.com/hc/en-gb/articles/360011206299-Launchpad-Pro-MK3-Chord-Mode-Guide)

**Tradeoff:** the layout exposes theory and invites voicing construction, but five interval columns plus preset and user areas consume most of an 8×8 grid. It favors exploration over a large bank of immediately triggerable chords.

### Novation Launchkey MK4 / FLkey 2 Chord Map

Chord Map makes a strong distinction between selection and transformation. Eight blue pads select scale-related chords; six orange performance pads temporarily apply manual arpeggio, inversion, or split behaviors. Encoders control Adventure, Explore, Spread, and Roll: the first two navigate among chord sets/complexity, Spread distributes notes across octaves, and Roll staggers the notes. The chosen chord or modifier is named on the screen. [Novation’s Chord Map Guide](https://support.novationmusic.com/hc/en-gb/articles/20346898821778-Chord-Map-Guide)

The broader Launchkey guide describes 40 banks of eight chords per scale-linked chord set. Adventure and Explore navigate the bank space, while Spread and Roll alter the output rather than changing pad identity. [Novation’s Launchkey built-in features guide](https://userguides.novationmusic.com/hc/en-gb/articles/27612205912210-Using-the-Launchkey-s-built-in-features)

Performance modifiers are hold-to-use by default, and only one can be active. A separate Scene Launch action enables latch; the modifier then toggles, the latch button lights white, and disabling latch also clears the active modifier. Inversion Up cycles by moving successive low notes up an octave; Inversion Down moves successive high notes down. Changing chord or releasing the momentary modifier resets the inversion cycle. [Novation’s Launchkey built-in features guide](https://userguides.novationmusic.com/hc/en-gb/articles/27612205912210-Using-the-Launchkey-s-built-in-features)

User Chord mode turns pads into a favorites bank. Holding an empty pad while entering up to six notes assigns a chord; saved pads are blue and turn white while played. Pad Up/Down transposes the whole user bank by semitone, Shift + Pad Up/Down moves by octave, and Function + chord pad deletes. Fixed Chord instead learns one interval structure from the keyboard and transposes it from any played key; the last learned chord remains available when the mode is toggled off and on. [Novation’s Launchkey built-in features guide](https://userguides.novationmusic.com/hc/en-gb/articles/27612205912210-Using-the-Launchkey-s-built-in-features)

**Tradeoff:** generative breadth is hidden behind two abstract bank axes, so the screen matters for discovery. The momentary/latch pairing is excellent for performance, but inversion-as-a-cycle makes repeated presses stateful and requires a clear reset rule.

### Arturia KeyStep Pro

KeyStep Pro learns an ordered interval structure rather than offering a chord catalog. The player holds a chord, invokes Shift + Tie/Rest/Chord, and can continue adding notes while holding either control. Once learned, any single key reconstructs and transposes the chord; the first note entered is the transposition center, keys above transpose up, and keys below transpose down. Input order affects the stored intervals/voicing, and the current scale can constrain/adapt the result. [Arturia KeyStep Pro manual 2.0, “Memorizing a Chord”](https://downloads.arturia.net/products/keystep-pro/manual/keystep-pro_Manual_2_0_EN.pdf)

The same manual documents chords of up to 16 notes, one chord memory per track, a once-per-second button flash while Chord mode is active, Shift + Tie/Rest/Chord to leave or re-enter with the learned chord intact, and loss of chord memory at power-off. [Arturia KeyStep Pro manual 2.0, “Memorizing a Chord”](https://downloads.arturia.net/products/keystep-pro/manual/keystep-pro_Manual_2_0_EN.pdf)

Firmware 2.0 revised entry so notes may be added as long as one entered note remains held, and it allowed Chord Mode output to be captured during real-time sequence recording. [Arturia KeyStep Pro manual 2.0, firmware changes](https://downloads.arturia.net/products/keystep-pro/manual/keystep-pro_Manual_2_0_EN.pdf)

For sequenced chords, step LEDs reveal constituent notes; individual chord tones can then be selected and edited for pitch, velocity, gate, time shift, and randomness. Time-shifting individual tones supports strum-like playback. [Arturia KeyStep Pro manual 1.3, “Advanced editing”](https://downloads.arturia.net/products/keystep-pro/manual/keystep-pro_Manual_1_3_EN.pdf)

**Tradeoff:** learning any interval set is flexible and instantly transposable, but one volatile memory per track is not a favorites workflow. The flashing mode indicator is compact, though it communicates presence rather than chord identity or voicing.

### Native Instruments Komplete Kontrol

Komplete Kontrol separates chord generation into Harmonizer and Chord Set. Harmonizer adds scale-relative intervals such as 1-3-5 or 1-3-5-7 to each played note. Chord Set instead selects from major/minor chord collections tied to the global root; in that mode Scale Type and Key Mode are unavailable. Root Note, chord Mode, Type, and Position are separate parameters. [Native Instruments Komplete Kontrol manual, Chord Parameters](https://docs.native-instruments.com/ni-tech-manuals/komplete-kontrol-manual/en/using-scales-and-the-arpeggiator.html#chord_parameters)

Position is a dedicated voice-leading control. Root always plays root position; negative values move the highest tone down an octave; positive values move the lowest tone up; Auto selects an inversion intended to make transitions between consecutive chords more musical. [Native Instruments Komplete Kontrol manual, Chord Position](https://docs.native-instruments.com/ni-tech-manuals/komplete-kontrol-manual/en/using-scales-and-the-arpeggiator.html#chordposition)

On Kontrol S-Series MK3 hardware, the Light Guide makes root notes bright, other scale notes dim, and inactive keys unlit; when a one-key chord sounds, every generated tone is shown by a bright LED. This gives generated notes distinct feedback from the available trigger keys. [Native Instruments Kontrol S-Series MK3 manual, Scale](https://docs.native-instruments.com/ni-tech-manuals/kontrol-s-mk3-manual/en/scale)

**Tradeoff:** explicit Mode/Type/Position parameters are legible and powerful, especially Auto voice leading, but the experience depends on encoders and labels. It maps less naturally to a pad-only surface unless common positions get dedicated keys.

### Akai MPC

MPC’s Pad Perform window offers explicit tabs for Chromatic, Notes, Chords, Chromatic Chords, Progressions, and Custom. Chords maps each pad to a chord whose root ascends by scale degree and keeps available chords in key; Chromatic Chords also ascends roots by scale degree but allows chord types outside the selected key. Progressions maps each pad to a chord from a chosen progression and allows pads to be played in any order. The touchscreen shows the current bank’s note or chord mapping. [Akai MPC Standalone OS 3.7 User Guide, “Pad Perform”](https://cdn.inmusicbrands.com/Software/37/MPC%20Standalone%20OS%20-%20User%20Guide%20-%20v3.7.pdf)

The MPC software guide adds a useful bank-navigation choice: Continuous makes the next bank begin one scale degree after the previous bank’s final pad, while Start on Root makes pad 1 of every bank begin on the scale root. The root pad is lit differently and highlighted on screen; Root Note and Octave are independent controls. [Akai MPC Software User Guide 2.14, “Pad Perform”](https://cdn.inmusicbrands.com/akai/214SMPCSTEMS/MPC%20Software%20-%20User%20Guide%20-%20v2.14.pdf)

User progression files can be created in MPC software, copied to a `Progressions` folder on external storage, and then appear under Pad Perform → Progressions → Other in standalone hardware. [Akai’s standalone user-progressions support article](https://support.akaipro.com/en/support/solutions/articles/69000824380-mpc-x-live-and-one-using-your-progressions-in-standalone)

**Tradeoff:** named tabs and an on-screen 4×4 map make the mode model explicit and easy to browse, but progression import is off-device and the documented Pad Perform controls emphasize chord type/root/scale rather than quick inversion or spread performance.

### Ableton Push 3

Push documents chord playing as reusable shapes on a scale-aware note grid rather than as one-pad chord presets. In the default 64 Notes layout, horizontal movement advances through scale notes and vertical movement is a fourth; the manual shows a triad shape that can be moved around the grid. Scale mode selects key and scale, while the left encoder chooses 4ths, 3rds, or Sequential layout. [Ableton Push 3 manual, “Playing and Programming Melodies and Harmonies”](https://www.ableton.com/en/push/manual/#playing-and-programming-melodies-and-harmonies)

In Key exposes only notes in the selected key; Chromatic retains every pitch but leaves out-of-key pads unlit. Fixed On preserves physical note positions across key changes, while Fixed Off moves the selected root to the bottom-left pad. Root pads use the track color, other in-scale notes are white, sounding notes are green, and scale choices persist with the Set. [Ableton Push 3 manual, “Playing in Other Keys”](https://www.ableton.com/en/push/manual/#playing-in-other-keys)

In Melodic Sequencer + 32 Notes, the lower half selects/plays pitches and a top-half step receives all selected pitches as a chord. Select + pad selects silently; holding a populated step reveals its tones below, where individual notes can be removed. Octave buttons move by octave, while Shift + Octave moves by one scale degree. [Ableton Push 3 manual, “Melodic Sequencer + 32 Notes”](https://www.ableton.com/en/push/manual/#melodic-sequencer--32-notes)

**Tradeoff:** a shape-based isomorphic grid preserves learned fingering and exposes every voicing, but demands multi-finger chord skill and provides no fast favorites bank. It is a valuable alternate mode, not a replacement for one-pad chords.

## Recommended 10×10 interaction model

### 1. Use a stable 8×8 performance core and the outer keys for context

A workable starting allocation is:

- **8×8 center:** chord bank, user bank, or isomorphic note grid.
- **Top edge:** root/key selection, bank/page, and mode tabs.
- **Right edge:** inversion down/up, spread down/up, strum/roll, split, sustain, latch.
- **Bottom edge:** octave/semitone transpose, learn, clear, lock, and an all-notes-off escape.
- **Corners:** Chord mode, Back/Home, display/context toggle, and panic.

This borrows Launchkey’s selection-versus-modification split without sacrificing Launchpad/Push-style spatial play. Keep chord locations invariant when modifiers change.

### 2. Provide three chord pages, not one overloaded page

1. **Scale Chords:** rows are scale degrees; columns are chord families or extensions. Keep the tonic in a predictable corner and color chord quality.
2. **User Chords:** direct favorite slots. Hold Learn + slot, enter notes, then release to save. Clear requires Clear + slot; Lock disables editing while leaving performance available.
3. **Shape Grid:** an isomorphic 4ths or 3rds layout for players who want manual voicings. Offer In Key/Chromatic and Root-at-corner/Fixed toggles.

The first page optimizes discovery, the second performance recall, and the third expressive construction. Mode keys should remain in the same physical positions on every page.

### 3. Make transforms composable but bounded

Treat inversion, spread, strum, and split as output transforms applied after chord selection. Default them to hold-to-use. A dedicated Latch key converts the next transform press into a toggle; its LED should stay bright while latched. Unlike Launchkey’s one-modifier limit, inversion plus modest spread or strum can be useful, but cap combinations to those with deterministic note ordering and clear feedback.

Prefer direct inversion choices (`root`, `1st`, `2nd`, `3rd`, `auto`) over an invisible press-cycle. If key count is tight, cycle only while a modifier is held and reset on chord change, mirroring Launchkey’s explicit reset behavior. Auto voice leading is worth a dedicated choice because it reduces jumps without asking the player to reason about inversions mid-performance.

### 4. Make transposition semantics explicit

Maintain two separate concepts:

- **Key/root change:** regenerates scale-derived chord content and moves the highlighted tonic.
- **Bank transpose:** shifts every stored user chord by semitone or octave without rewriting its saved intervals.

For learned fixed chords, store intervals relative to an explicit anchor, not silently to whichever MIDI note sorts lowest. Show the anchor and sounding note names when a display is available. Offer both “root follows corner” and “fixed physical positions” so modulation does not destroy spatial memory.

### 5. Use a small, consistent LED grammar

- Dim track color: available chord/note.
- Distinct tonic color or brighter shade: root degree.
- White: selected/latched chord or modifier.
- Green: currently sounding.
- Red: destructive/edit target or clipping/error, never a normal chord quality.
- Unlit: unavailable or out of scale.
- Slow pulse: learned but not yet committed; steady light: saved.

If chord quality colors are used, keep them optional because major/minor/diminished distinctions compete with selection, playback, and edit state. State feedback should win over theory coloring.

### 6. Define note ownership for mechanical switches

Chord pads should be momentary: key-down sends all chord Note Ons and key-up sends the corresponding Note Offs. A sustain/latch transform must take ownership of those notes so releasing the source key does not prematurely stop them. Changing pages, disabling Chord mode, changing MIDI channel, or pressing Panic must release every owned note. This is especially important when multiple chord pads share pitches: use per-note reference counts so releasing one chord does not silence a shared tone still held by another.

## Highest-value prototype questions

1. Can players understand an 8×8 chord core plus edge modifiers without labels after ten minutes?
2. Is direct inversion selection faster than a Launchkey-style repeated-press cycle on mechanical keys?
3. Should generated banks use theory labels (degree/quality) or an Explore/Adventure continuum?
4. Does Auto voice leading need to be global, latched, or recalculated independently for each chord press?
5. How many user slots are enough before paging becomes harder than remembering locations?
6. Which transform combinations remain predictable enough to support simultaneously?
