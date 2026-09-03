# Keypad (Keychron C100 MIDI Pad)

[![Build QMK firmware](https://github.com/roguefort-dev/Keypad-keychron-c100-midi-pad/actions/workflows/build_binaries.yaml/badge.svg)](https://github.com/roguefort-dev/Keypad-keychron-c100-midi-pad/actions/workflows/build_binaries.yaml)

USB MIDI firmware for the **Keychron C100 8K** giant macro pad, built on
Keychron's QMK fork. The proof of concept turns the 10×10 pad into an
isomorphic **E natural minor** MIDI controller.

## Performance layer

The top row is reserved for switching performance layers. In this proof of
concept, the far-right key momentarily opens the Settings layer; the other nine
positions are reserved for chord, drum, macro, and additional scale layers.

| Reserved | Reserved | Reserved | Reserved | Reserved | Reserved | Reserved | Reserved | Reserved | Settings |
|---|---|---|---|---|---|---|---|---|---|

The remaining 90 keys send notes from E natural minor (`E F♯ G A B C D`). The
lowest note is on the bottom-left. Moving right advances one scale degree;
moving up advances three degrees, a fourth. The overlap makes interval and
chord shapes repeat consistently across the pad.

- Base note: E2 (MIDI note 40)
- Default velocity: 100
- Fixed channel: 1
- Octave range: −2 to +2
- Panic releases tracked notes and sends sustain-off, all-sound-off, and
  all-notes-off on every channel

The responsive RGB overlay keeps every root note visible in warm amber. Black
notes are dim blue and the other scale notes are dim white. Pressing any note
changes every duplicate of that pitch to the complementary cyan-blue, then
restores their scale colors after the last duplicate is released.

## Settings layer

Hold the top-right Settings key while choosing an option:

| C | C♯ | D | D♯ | E | F | F♯ | G | G♯ | A |
|---|---|---|---|---|---|---|---|---|---|
| A♯ | B | Major | Natural minor | Oct − | Oct + | Vel − | Vel + | Sustain toggle | Panic |

E and Natural minor are selected at startup. Root and scale changes release
all sounding notes first, preventing stuck notes. The selected root and scale
are highlighted while Settings is held. The lower seven rows show the selected
root as a persistent 5×7 pixel-art letter; sharp roots add a cyan 3×7 `#`
beside the amber letter without obscuring the controls.

## Build locally

This repository is a QMK external userspace. It expects the Keychron fork on
branch `2025q3`, which is the branch containing C100 8K support.

```sh
git clone --branch 2025q3 --recurse-submodules https://github.com/Keychron/qmk_firmware.git
git clone https://github.com/roguefort-dev/Keypad-keychron-c100-midi-pad.git
cd Keypad-keychron-c100-midi-pad
make build QMK_HOME=../qmk_firmware
```

The resulting file is `keychron_c100_8k_midi_pad.bin` in this repository's
root. GitHub Actions also builds the firmware after each push and publishes the
result through the repository workflow.

The initial proof-of-concept build uses 58,890 bytes of the target's verified
128 KiB flash region (about 45%).

## Test MIDI before flashing

After flashing and reconnecting the pad, confirm that your operating system
shows a new USB MIDI input. Open a MIDI monitor or DAW, press a note pad, and
verify that it receives a matching note-on and note-off pair. Test the Panic
key before a longer playing session.

## Flashing and recovery

Flashing firmware can make the pad temporarily unusable if the wrong image is
selected. Keep Keychron's stock C100 8K v1.0.1 firmware available before
flashing. Its verified SHA-256 is:

```text
2f95264cc589bbe8c7e2612fb950503e75464409e64874159ca4da6da845ebc1
```

Enter the bootloader by holding the top-left key while connecting USB, or by
using the physical reset button under the spacebar position. Flash only a
binary built for `keychron/c100_8k`.

## Roadmap

- Musical scale layouts
- Chord pads and inversions
- Multiple performance/macro layers
- Persistent MIDI settings
- Launcher/VIA-friendly configuration where practical

## License

GPL-2.0-or-later, matching QMK Firmware. See [LICENSE](LICENSE).
