# Keypad (Keychron C100 MIDI Pad)

[![Build QMK firmware](https://github.com/roguefort-dev/Keypad-keychron-c100-midi-pad/actions/workflows/build_binaries.yaml/badge.svg)](https://github.com/roguefort-dev/Keypad-keychron-c100-midi-pad/actions/workflows/build_binaries.yaml)

USB MIDI firmware for the **Keychron C100 8K** giant macro pad, built on
Keychron's QMK fork. The default layer is an isomorphic **E natural minor** MIDI
controller with selectable roots and scales, octave controls, and responsive
per-key RGB.

## Scale performance layer

The top-left two keys shift the playable range down or up by one octave. The
top-right key momentarily opens Settings. The other top-row positions are kept
free for future navigation.

| Oct − | Oct + |  |  |  |  |  |  |  | Hold Settings |
|---|---|---|---|---|---|---|---|---|---|

The remaining 90 keys send notes from the selected scale. The lowest note is at
the bottom-left. Moving right advances one scale degree; moving up advances
three degrees. The overlap keeps interval and chord shapes consistent and
deliberately places duplicate notes around the grid.

- Default root and scale: E natural minor (`E F♯ G A B C D`)
- Base note: E2 (MIDI note 40)
- Fixed velocity: 100
- Fixed channel: 1
- Octave range: −2 to +2

Every root note stays visible in warm amber. Pressing a note changes every pad
that represents that exact MIDI note to complementary cyan-blue, including its
duplicates. Releasing the final duplicate restores the scale colors.

## Settings

Hold the top-right Settings key to open this 10×10 layout:

- The two scale rows contain 18 selections per page. Their rightmost keys move
  to the previous or next page and briefly show a 3×6 pixel arrow.
- The lower-left 6×2 block selects the root: `C/C♯`, `D/D♯`, `E/F`, `F♯/G`,
  `G♯/A`, and `A♯/B`.
- A blank column separates the root selector from a 7×6 character display.
- Root notes appear as a 3×6 TomTentacles glyph. Sharps add a cyan 2×2 marker
  after a one-column gutter.
- Scale names start left-aligned, hold for 400 ms, and then scroll. Selecting a
  different scale always restarts the text from the first character.
- Page arrows use the same 3×6 size and left alignment as the font, and remain
  visible for 250 ms or until another control is pressed.

The first page contains:

| Major | Minor | Harmonic | Melodic | Dorian | Phrygian | Lydian | Mixolydian | Locrian |
|---|---|---|---|---|---|---|---|---|
| Major pent | Minor pent | Blues | Whole tone | Chromatic | Half-whole dim | Whole-half dim | Bebop major | Bebop dominant |

The second page currently contains Hungarian minor and Japanese pentatonic,
leaving room for sixteen more scales without another layout change.

## Layer navigation

While Settings is held, press the key immediately to its left to enter the
latched layer-select page. Releasing Settings does not take you back. The first
two keys there select Scale or Chord mode.

Chord mode is intentionally an empty proof-of-concept placeholder. Its
top-right key returns to the layer-select page; this is a direct layer change,
not a hold or toggle.

## Build locally

This repository is a QMK external userspace. It expects the Keychron fork on
branch `2025q3`, which contains C100 8K support.

```sh
git clone --branch 2025q3 --recurse-submodules https://github.com/Keychron/qmk_firmware.git
git clone https://github.com/roguefort-dev/Keypad-keychron-c100-midi-pad.git
cd Keypad-keychron-c100-midi-pad
make build QMK_HOME=../qmk_firmware
```

The resulting file is `keychron_c100_8k_midi_pad.bin` in this repository's
root. GitHub Actions also builds the firmware after each push and publishes the
binary through the repository workflow.

The current build uses 60,634 bytes of the target's verified 128 KiB flash
region (about 46%).

## Test MIDI after flashing

After the pad reconnects, confirm that the operating system exposes a USB MIDI
input. Open a MIDI monitor or DAW and verify that note presses produce matching
note-on and note-off messages. Changing the root, scale, octave, or active layer
releases all tracked notes first to prevent stuck notes.

## Flashing and recovery

Flashing firmware can make the pad temporarily unusable if the wrong image is
selected. Keep Keychron's stock C100 8K v1.0.1 firmware available. Its verified
SHA-256 is:

```text
2f95264cc589bbe8c7e2612fb950503e75464409e64874159ca4da6da845ebc1
```

Enter the bootloader by holding the top-left key while connecting USB, or use
the physical reset button under the spacebar position. Flash only a binary
built for `keychron/c100_8k`.

## License

GPL-2.0-or-later, matching QMK Firmware. See [LICENSE](LICENSE) and
[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
