# Unattended C100 flashing on Windows

The running MIDI pad firmware accepts the SysEx message
`F0 7D 43 31 30 30 44 46 55 F7` and calls QMK's `reset_keyboard()` from the
scan loop. Firmware transfer then uses the AT32 USB DFU bootloader
(`2E3C:DF11`). The normal C100 device is `3434:042C`.

## One-time setup

Install Python 3, keep one C100 connected, then run from this repository:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tools/setup-c100-windows.ps1
```

This downloads QMK Toolbox 0.3.4 utilities from its pinned source commit and
verifies their SHA-256 hashes. Windows asks for administrator approval once to
stage a WinUSB driver scoped to `2E3C:DF11`. If MIDI is missing, setup removes
only the C100 composite device instance and rescans it to refresh cached USB
descriptors. Shared HID/audio driver packages remain installed. Setup writes
its result to `.cache/c100-windows/setup.log` and fails if no usable MIDI or
DFU interface is available.

Windows driver installation requires administrator rights. The setup cannot
approve Windows' UAC prompt for you. Future firmware updates run as an ordinary
user after the DFU driver is installed.

If the installed firmware predates the MIDI reboot command, or neither WinMM
nor the C100's kernel MIDI stream is available, one initial manual DFU
entry is necessary: hold the top-left key while connecting USB, or press the
physical reset button. Flash the current MIDI pad firmware once using the
command below. Subsequent updates can request DFU automatically over MIDI.

## Flash

Build `keychron/c100_8k:midi_pad` using the documented QMK environment, or
download the `Firmware` artifact from a successful build of the intended
commit. For example, with GitHub CLI authenticated:

```powershell
gh run list --workflow build_binaries.yaml --commit (git rev-parse HEAD) --status success
gh run download <run-id> --name Firmware --dir .build/firmware
python tools/flash_c100.py .build/firmware/keychron_c100_8k_midi_pad.bin
```

The command validates the filename, flash size, ARM vectors, DFU target and
checksum before requesting a reboot. It requires exactly one C100 MIDI output
or an already accessible DFU bootloader, waits for DFU, downloads the image,
and waits for the MIDI output to reconnect. A failed transfer or reconnect
returns an error. The AT32's disconnect-related exit code 74 is accepted only
when dfu-util reports a completed download and MIDI subsequently reconnects.
This confirms transfer and availability, not a read-back comparison of flash.

To inspect availability without changing the device:

```powershell
python tools/flash_c100.py --check
```

Windows normally uses WinMM. If WinMM lists no C100 output, the command finds
the C100's active kernel streaming interface and a MIDI-compatible render pin,
then sends the same SysEx through the existing Windows audio driver. This
fallback is scoped to `3434:042C`, interface 2, and does not replace the runtime
driver or change system MIDI settings. It lets flashing work even when a DAW
using WinMM still cannot discover the device. Windows is allowed time to bind
WinUSB after the device enters DFU.

Close applications that exclusively hold the C100 MIDI output before flashing.
An inaccessible DFU device means the WinUSB driver needs installation; do not
replace the C100's normal HID or MIDI drivers with WinUSB.

## Focused regression checks

```powershell
python -m unittest discover -s tests -p flash_c100_test.py
```

The tests inject host responses and never reboot or flash hardware. Linux's
`make flash` uses the same transfer orchestration, with ALSA `amidi` in place
of Windows WinMM. Its device-access rule remains necessary.

## Verified on this Windows machine

On 1 November 2026, setup installed the DFU WinUSB driver and the current
`8fa188c` CI image was flashed. The live USB descriptor reports `EF/02/01`,
with 512-byte bulk MIDI endpoints. Windows' audio driver exposes the MIDI
stream, although WinMM still lists no C100 port on this installation.

A complete subsequent update succeeded through the kernel MIDI fallback as
an ordinary, non-administrator Windows user: the running pad received SysEx,
entered DFU, accepted the image, and reconnected with its MIDI driver stream
available. No physical reset or UAC approval was needed for that update. The
tested image was 74,196 bytes including its DFU suffix, SHA-256
`e1f4a12c3b200977198773ce066abca9fe794a87dab02966b4295259cfbe41c1`.
The focused tests also cover native packet layout, output pointers, handle
cleanup, cancellation and the transient WinUSB binding delay.

Sources: [WinMM SysEx API](https://learn.microsoft.com/en-us/windows/win32/api/mmeapi/nf-mmeapi-midioutlongmsg),
[dfu-util Windows driver requirements](https://dfu-util.sourceforge.net/),
[QMK driver installer](https://github.com/qmk/qmk_driver_installer),
[QMK Toolbox 0.3.4](https://github.com/qmk/qmk_toolbox/releases/tag/0.3.4),
[Windows kernel MIDI transport](https://microsoft.github.io/MIDI/kb/kernel-streaming/),
[kernel MIDI packet format](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ksmedia/ns-ksmedia-ksmusicformat).
