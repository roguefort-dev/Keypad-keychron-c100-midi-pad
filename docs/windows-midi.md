# Windows MIDI discovery

## USB corrections

Keychron's `2025q3` QMK branch includes an Audio Interface Association
Descriptor (IAD) for the MIDI function, grouping Audio Control interface 2
and MIDI Streaming interface 3. Its device descriptor advertises class,
subclass and protocol `00/00/00` unless virtual serial is enabled. The
connected C100's original firmware descriptors confirmed this combination;
Linux nevertheless exposed its MIDI port.

Microsoft requires the device descriptor triplet **`EF/02/01`** for Windows
to recognize IADs and group their interfaces correctly. See Microsoft's
[USB interface association descriptor documentation](https://learn.microsoft.com/en-us/windows-hardware/drivers/usbcon/usb-interface-association-descriptor).
The mismatch is a concrete firmware defect and a likely explanation for
Windows failing to expose MIDI; Windows hardware verification remains needed.

The connected C100 also negotiated **480 Mb/s high speed**, while QMK
advertised **64-byte bulk MIDI endpoints**. USB 2.0 requires high-speed bulk
endpoints to use a 512-byte maximum packet size; full-speed bulk can use
64 bytes. See Microsoft's
[bulk transfer documentation](https://learn.microsoft.com/en-us/windows-hardware/drivers/usbcon/usb-bulk-and-interrupt-transfer).
This is a second concrete compatibility defect; Linux accepting the device
does not establish descriptor compliance.

The userspace correction has two parts:

- `usb_descriptor_midi.c` uses Keychron's existing weak descriptor hook to
  advertise `EF/02/01` and select 512-byte MIDI endpoints at high speed or
  64-byte endpoints at full speed. It also supplies the Device Qualifier and
  Other Speed Configuration descriptors required by a dual-speed USB device.
  Descriptor storage survives the control transfer; the transport still
  handles short requests, including the initial eight-byte probe.
- `usb_midi_transport.c` reserves fixed buffers large enough for 512-byte
  packets and wraps ChibiOS's `usbStart` through the linker. Its event adapter
  selects matching endpoint and queue sizes before QMK handles configuration,
  including after resets where the host reuses cached descriptors. Full-speed
  receive queues use 64-byte payloads so a complete 64-byte packet is delivered
  immediately. Existing callbacks are preserved, and QMK still flushes each
  outgoing MIDI event immediately.

The MIDI interface grouping and endpoint addresses, HID reports, strings,
VID/PID (`3434:042C`) and firmware release number remain QMK's. High-speed
operation and the C100's polling settings are preserved. The external QMK
checkout needs no patch, and virtual serial stays disabled. The fixed buffers
and descriptor copies increase static/reserved RAM by 4,496 bytes compared
with the preceding production build; firmware flash usage grows by 436 bytes
to 74,418. The downloadable `.bin` is 74,436 bytes including two alignment
bytes and a 16-byte DFU suffix.

The firmware release number stays `1.0.1`; incrementing it alone would not
guarantee removal of an existing Windows interface binding. Re-enumeration
steps below address that separately.

## Check the firmware and descriptors

```sh
make test-usb QMK_HOME=../qmk_firmware
```

This builds the C100 firmware and compiles QMK's actual descriptor resolver
and the userspace hook together on the host, using the build's generated
identity and the C100's USB feature flags. It verifies `EF/02/01`, short and
full requests, both-speed configurations, the qualifier, MIDI grouping and
unchanged HID/report/string data. A small HAL adapter checks endpoint and
queue sizes before the original configuration callback, high/full-speed
reset transitions without descriptor requests, same-speed reconfiguration,
driver restarts and callback forwarding. The real ARM build checks integration
with QMK's actual HAL types; the host adapter does not simulate USB hardware
or Windows driver installation. For memory/undefined-behavior checks, run:

```sh
make test-usb HOST_CFLAGS='-O1 -g -Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer'
```

The build helper keeps a stable userspace symlink under
`$XDG_CACHE_HOME/keychron-c100-qmk` (default `~/.cache/keychron-c100-qmk`), or
an owned `/tmp/keychron-c100-qmk-UID` directory if the cache path contains
spaces. This supports repository paths with spaces while keeping QMK's
generated dependency paths valid across consecutive builds.

After installing the resulting firmware, a descriptor viewer should show
device class `EF`, subclass `02`, protocol `01`, and the same two-interface
Audio IAD. MIDI bulk endpoints should show a maximum packet size of 512 at
high speed or 64 at full speed, with the opposite sizes in the Other Speed
Configuration. On Linux, use `lsusb -v -d 3434:042c`, `lsusb -t` and `amidi -l`.

Live validation on **1 November 2026** flashed this build to the connected
C100 and confirmed Linux re-enumeration at 480 Mb/s, device class `EF/02/01`,
512-byte bulk endpoints `OUT 04` and `IN 83`, and a valid 10-byte Device
Qualifier. Both Audio Control and MIDI Streaming interfaces bound to
`snd-usb-audio`, and `amidi -l` exposed the input/output MIDI port. Read-only
USB requests returned the 159-byte Configuration with 512-byte endpoints and
the 159-byte Other Speed Configuration with 64-byte endpoints. Full-speed
operation has host-model coverage but has not been tested on a physical
full-speed connection. Windows discovery still needs verification on Windows.

## Verify on Windows

1. Flash the new C100 firmware, reconnect USB, and reopen the MIDI monitor
   or DAW so it refreshes its available MIDI ports.
2. In Device Manager, check for an audio/MIDI function under **Sound, video
   and game controllers** as well as the keyboard's HID functions. The
   displayed audio device and MIDI port names depend on the Windows driver.
3. Enable the C100 MIDI input in the application and verify matching Note On
   and Note Off messages. If using host MIDI clock, also verify that the
   application can send clock to the C100 output port.

If an earlier stock/HID-only binding persists, reinstall the C100 device:

1. In Device Manager, locate the C100 composite parent using **View → Devices
   by connection**. Confirm **Properties → Details → Hardware Ids** contains
   `USB\VID_3434&PID_042C`.
2. Unplug the C100. Enable **View → Show hidden devices**, then uninstall
   that C100 device instance and any remaining stale C100 child entries.
   Leave the option to delete the driver package unchecked; Windows' shared
   USB, HID and audio driver packages should remain installed.
3. Reconnect the C100, allow Windows to enumerate its interfaces, then
   reopen the MIDI application and repeat the note test.

These steps follow Microsoft's
[Device Manager removal procedure](https://learn.microsoft.com/en-us/windows-hardware/drivers/install/using-device-manager-to-uninstall-devices-and-driver-packages)
while keeping removal scoped to this device. If MIDI still does not appear,
record the Windows version, C100 device/child Hardware Ids, Device Manager
status/error code, and the device/configuration descriptors from a USB
descriptor viewer. Those details distinguish a remaining descriptor issue
from an existing driver binding or application port configuration.
