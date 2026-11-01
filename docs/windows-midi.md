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
The mismatch is a concrete firmware defect. A separate Windows catalog
permission issue also prevented discovery on the tested Windows machine;
its diagnosis and verified repair are described below.

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
full-speed connection. Windows discovery was subsequently verified after the
host repair below, including a user-confirmed test in Bitwig Studio.

## Verify on Windows

Check the actual WinMM input list that Windows DAWs can use:

```powershell
python tools/check_windows_midi.py
```

This read-only command prints inputs and outputs and returns success only if
a `Keychron C100` input was enumerated successfully. `flash_c100.py --check`
checks flashing availability and can succeed through its direct-driver
fallback even when this DAW discovery check fails.

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

## Verified Windows MIDI service repair

On **1 November 2026**, Windows 11 build 26300 exposed the C100's kernel MIDI
stream but listed **zero WinMM inputs**. Re-enumerating the device and
restarting MIDI Services did not fix discovery. Both the older USB Audio
driver and the installed Microsoft `USBMidi2-ACX` driver loaded without a
device error. The latter remains selected on the verified machine.

A short ETW trace of `Microsoft.Windows.Midi2.MidiSrv` showed
`CMidiDeviceManager::Initialize` rejecting every transport with
**`0x800B0100` (`TRUST_E_NOSIGNATURE`)**. No transport DLLs loaded into the
service. The MIDI files passed targeted `sfc /verifyfile` checks. Their
original Microsoft catalog existed, contained the installed DLL hashes, and
had a valid Microsoft Windows signature when checked as administrator.

The failure was its **protected catalog ACL**: only SYSTEM and Administrators
had access. `MidiSrv` runs as LocalService with its own service SID, so it
could not read the catalog needed to verify its transport libraries.

The affected file on this machine was:

```text
C:\Windows\System32\CatRoot\{F750E6C3-38EE-11D1-85E5-00C04FC295EE}\Microsoft-Windows-Client-Desktop-Required-Package~31bf3856ad364e35~amd64~~10.0.26100.9711.cat
```

After explicit approval, the repair added **Read** for **`NT SERVICE\MidiSrv`**
to this one catalog and preserved its original permissions in a local backup.
The catalog's SHA-256 stayed
`5dbf9b2492ac417b7a017785a594fb10484e9f8946320e3138b3a53b1898d88a`.
The original Microsoft signature remained valid; signing policy and trust
stores were unchanged.

After restarting `midisrv`, all registered transport DLLs loaded. An ordinary
Windows user could enumerate `Keychron C100 8K` as both input and output.
**The user confirmed that it works in Bitwig Studio.** No further Windows
steps are required on that repaired installation.

### Reproduce or repair the same permission fault

Apply this procedure when the service trace shows the same signing failure
and the catalog that actually signs the installed MIDI components has the
SYSTEM/Administrators-only ACL. The catalog version above is specific to the
verified installation; identify the correct installed catalog on another PC.
The helper deliberately requires its exact path and never selects one by a
wildcard or changes a whole directory.

Close MIDI applications first. In an **administrator PowerShell**, inspect
the diagnosed catalog, then apply the repair after reviewing that scope:

```powershell
$catalog = '<full path to the verified catalog for the installed MIDI components>'
.\tools\repair-windows-midi-catalog.ps1 -CatalogPath $catalog
.\tools\repair-windows-midi-catalog.ps1 -CatalogPath $catalog -Apply
Restart-Service midisrv
python tools/check_windows_midi.py
```

Without `-Apply`, the helper only inspects. It checks the catalog's location
and Microsoft signature, recognizes the specific protected ACL, backs up its
SDDL under `.cache/windows-midi-catalog/`, and grants only the service's Read
permission. An existing service Read grant is left as-is. The helper never
restarts Windows or services itself. Reopen the DAW and select the C100 MIDI
input after the check passes.

To reverse an application of this helper, restore the DACL from its backup
in an administrator PowerShell, then restart MIDI Services:

```powershell
$backup = Get-Content '<backup JSON path printed by the helper>' -Raw | ConvertFrom-Json
$acl = Get-Acl -LiteralPath $backup.CatalogPath
$acl.SetSecurityDescriptorSddlForm($backup.OriginalSddl, [Security.AccessControl.AccessControlSections]::Access)
Set-Acl -LiteralPath $backup.CatalogPath -AclObject $acl
Restart-Service midisrv
```

This host permission repair is separate from choosing a USB class driver.
Microsoft documents [switching between the installed MIDI class drivers](https://microsoft.github.io/MIDI/kb/how-to-switch-between-class-drivers/)
and [the service's signature checks](https://microsoft.github.io/MIDI/kb/service-plugin-not-loaded/).
The file checks used [SFC's per-file verification](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/sfc).

### Focused tool tests

```powershell
python -m unittest discover -s tests -p windows_midi_discovery_test.py
powershell -NoProfile -ExecutionPolicy Bypass -File tests/windows_midi_catalog_test.ps1
```

These tests inject WinMM and filesystem/signature operations. They exercise
missing inputs, native enumeration failures, backup-before-write ordering,
the service-only Read grant, idempotence, and rejection of unrelated paths,
unsigned catalogs and other ACL layouts without changing Windows permissions.
