#!/usr/bin/env python3
"""Flash the C100 through its MIDI bootloader command on Windows or Linux."""
import argparse
import ctypes as ct
import hashlib
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import time
import urllib.request
import zlib

ROOT = Path(__file__).resolve().parent.parent
FIRMWARE_NAME = "keychron_c100_8k_midi_pad.bin"
CACHE = ROOT / ".cache" / "c100-windows"
DFU_COMMAND = bytes.fromhex("F0 7D 43 31 30 30 44 46 55 F7")
TOOLBOX_COMMIT = "bec1ad084785d47623fab0d172b622dc59396f54"  # QMK Toolbox 0.3.4
TOOLS = {
    "dfu-util.exe": "322fe732c47293b8b0fdeca510f2188f9d7a5136ef1a7900770deb747fd9dd70",
    "libusb-1.0.dll": "d439b7b6bf3bcf1defd3651e6d062513886cf13d25a2a4731e7654eb44419f35",
    "libwinpthread-1.dll": "c944ee510721a1d30d42227cc3061dfdcbc144c952381afcfe4f6e82c5435ffc",
    "qmk_driver_installer.exe": "ccd78a40e3db74a189f42e3eab93463ef6c214c4f928b586992d70863b094431",
}


def setup_windows():
    CACHE.mkdir(parents=True, exist_ok=True)
    base = (f"https://raw.githubusercontent.com/qmk/qmk_toolbox/{TOOLBOX_COMMIT}"
            "/windows/QMK%20Toolbox/Resources")
    for name, expected in TOOLS.items():
        path = CACHE / name
        if not path.exists() or hashlib.sha256(path.read_bytes()).hexdigest() != expected:
            print(f"Downloading {name} from QMK Toolbox 0.3.4...", flush=True)
            with urllib.request.urlopen(f"{base}/{name}", timeout=60) as response:
                data = response.read()
            if hashlib.sha256(data).hexdigest() != expected:
                raise RuntimeError(f"Checksum mismatch for {name}; nothing installed.")
            path.write_bytes(data)
    print(f"Verified Windows tools in {CACHE}")


def validate_firmware(path):
    """Reject a wrong target, corrupt suffix, or invalid MCU image before reboot."""
    data = path.read_bytes()
    if path.name != FIRMWARE_NAME or not 24 <= len(data) <= 128 * 1024 + 16:
        raise RuntimeError(f"Expected a C100 build named {FIRMWARE_NAME}, at most 128 KiB.")
    _, pid, vid, version, signature, length, crc = struct.unpack("<HHHH3sBI", data[-16:])
    if (vid, pid, version, signature, length) != (0x2E3C, 0xDF11, 0x0100, b"UFD", 16):
        raise RuntimeError("Firmware has no matching AT32 DFU suffix.")
    if crc != (~zlib.crc32(data[:-4]) & 0xFFFFFFFF):
        raise RuntimeError("Firmware DFU checksum is invalid.")
    stack, reset = struct.unpack_from("<II", data)
    if not (0x20000000 < stack <= 0x20010000 and stack % 4 == 0
            and reset & 1 and 0x08000000 <= (reset & ~1) < 0x08000000 + len(data) - 16):
        raise RuntimeError("Firmware has an invalid C100 ARM vector table.")
    return hashlib.sha256(data).hexdigest()


class WindowsMidi:
    """WinMM SysEx output, with explicit pointer sizes and buffer lifetime."""
    class Caps(ct.Structure):
        _pack_ = 1  # WinSDK mmsyscom.h uses pshpack1.h.
        _fields_ = [("mid", ct.c_uint16), ("pid", ct.c_uint16), ("version", ct.c_uint32),
                    ("name", ct.c_wchar * 32), ("technology", ct.c_uint16),
                    ("voices", ct.c_uint16), ("notes", ct.c_uint16),
                    ("channels", ct.c_uint16), ("support", ct.c_uint32)]

    class Header(ct.Structure):
        _pack_ = 1
        _fields_ = [("data", ct.c_void_p), ("length", ct.c_uint32),
                    ("recorded", ct.c_uint32), ("user", ct.c_size_t),
                    ("flags", ct.c_uint32), ("next", ct.c_void_p),
                    ("reserved", ct.c_size_t), ("offset", ct.c_uint32),
                    ("reserved_array", ct.c_size_t * 8)]

    def __init__(self):
        self.mm = ct.WinDLL("winmm")
        signatures = {
            "midiOutGetNumDevs": [],
            "midiOutGetDevCapsW": [ct.c_size_t, ct.POINTER(self.Caps), ct.c_uint],
            "midiOutOpen": [ct.POINTER(ct.c_void_p), ct.c_uint, ct.c_size_t, ct.c_size_t, ct.c_uint],
            "midiOutClose": [ct.c_void_p],
            "midiOutReset": [ct.c_void_p],
            "midiOutPrepareHeader": [ct.c_void_p, ct.POINTER(self.Header), ct.c_uint],
            "midiOutLongMsg": [ct.c_void_p, ct.POINTER(self.Header), ct.c_uint],
            "midiOutUnprepareHeader": [ct.c_void_p, ct.POINTER(self.Header), ct.c_uint],
        }
        for name, arguments in signatures.items():
            function = getattr(self.mm, name)
            function.argtypes, function.restype = arguments, ct.c_uint

    @staticmethod
    def check(result, operation):
        if result:
            raise RuntimeError(f"{operation} failed (WinMM {result}). Close applications using the C100.")

    def ports(self):
        result = []
        for index in range(self.mm.midiOutGetNumDevs()):
            caps = self.Caps()
            self.check(self.mm.midiOutGetDevCapsW(index, ct.byref(caps), ct.sizeof(caps)), "MIDI enumeration")
            if "Keychron C100 8K" in caps.name:
                result.append((index, caps.name))
        return result

    def reboot(self, port):
        handle = ct.c_void_p()
        self.check(self.mm.midiOutOpen(ct.byref(handle), port[0], 0, 0, 0), "Opening MIDI output")
        data = ct.create_string_buffer(DFU_COMMAND)
        header = self.Header(data=ct.addressof(data), length=len(DFU_COMMAND))
        prepared = False
        try:
            self.check(self.mm.midiOutPrepareHeader(handle, ct.byref(header), ct.sizeof(header)), "Preparing SysEx")
            prepared = True
            self.check(self.mm.midiOutLongMsg(handle, ct.byref(header), ct.sizeof(header)), "Sending SysEx")
            if not wait_for(lambda: header.flags & 1, timeout=5):  # MHDR_DONE
                raise RuntimeError("MIDI bootloader request timed out.")
        finally:
            # Reset returns queued buffers before unprepare/free, including on disconnect.
            self.mm.midiOutReset(handle)
            try:
                if prepared:
                    self.check(self.mm.midiOutUnprepareHeader(handle, ct.byref(header), ct.sizeof(header)), "Releasing SysEx")
            finally:
                self.mm.midiOutClose(handle)


class Host:
    def __init__(self, dfu_util=None):
        self.windows = os.name == "nt"
        self.midi = WindowsMidi() if self.windows else None
        self.dfu_util = dfu_util or (str(CACHE / "dfu-util.exe") if self.windows else shutil.which("dfu-util"))
        if not self.dfu_util or not (Path(self.dfu_util).is_file() or shutil.which(self.dfu_util)):
            raise RuntimeError("dfu-util is missing. On Windows run: python tools/flash_c100.py --setup")
        if not self.windows:
            for name in ("amidi",):
                if not shutil.which(name):
                    raise RuntimeError(f"Required command is missing: {name}")

    def dfu(self):
        result = subprocess.run([self.dfu_util, "-d", "2e3c:df11", "-l"],
                                capture_output=True, text=True, timeout=5)
        output = result.stdout + result.stderr
        matches = re.findall(r'Found DFU:.*alt=0,.*serial="([^"]*)"', output)
        if len(matches) > 1:
            raise RuntimeError("Multiple AT32 bootloaders connected; leave only the C100 connected.")
        if "Cannot open DFU device" in output:
            raise RuntimeError("C100 DFU is present but inaccessible. Run the Windows setup or install the Linux udev rule.")
        if result.returncode:
            raise RuntimeError(f"dfu-util probe failed ({result.returncode}): {output.strip()}")
        return bool(matches)

    def ports(self):
        if self.windows:
            return self.midi.ports()
        result = subprocess.run(["amidi", "-l"], capture_output=True, text=True, check=True, timeout=5)
        return [(line.split()[1], line) for line in result.stdout.splitlines()
                if "Keychron C100 8K MIDI" in line]

    def reboot(self, port):
        if self.windows:
            self.midi.reboot(port)
        else:
            subprocess.run(["amidi", "-p", port[0], "-S", DFU_COMMAND.hex(" ")], check=True, timeout=10)

    def download(self, firmware):
        return subprocess.run([self.dfu_util, "-d", "2e3c:df11", "-a", "0",
                               "-s", "0x08000000:leave", "-D", str(firmware)],
                              capture_output=True, text=True, timeout=120)


def wait_for(probe, timeout=15, interval=0.25):
    deadline = time.monotonic() + timeout
    while True:
        value = probe()
        if value:
            return value
        if time.monotonic() >= deadline:
            return None
        time.sleep(interval)


def flash(host, firmware, wait=wait_for):
    digest = validate_firmware(firmware)
    print(f"Firmware: {firmware}\nSHA-256: {digest}", flush=True)
    if not host.dfu():
        ports = host.ports()
        if len(ports) != 1:
            raise RuntimeError("Expected exactly one C100 MIDI output. Run Windows setup; older firmware may need one manual DFU entry.")
        print(f"Requesting DFU through {ports[0][1]}...", flush=True)
        host.reboot(ports[0])
        if not wait(host.dfu):
            raise RuntimeError("C100 did not enter DFU. Firmware may predate the host-triggered command.")
    result = host.download(firmware)
    output = result.stdout + result.stderr
    print(output, end="", flush=True)
    # AT32 can disconnect during the final status request after a complete write.
    if result.returncode != 0 and not (result.returncode == 74 and "File downloaded successfully" in output):
        raise RuntimeError(f"dfu-util failed ({result.returncode}); flash was not confirmed.")
    if not wait(lambda: len(host.ports()) == 1):
        raise RuntimeError(f"C100 MIDI did not reconnect (dfu-util exit {result.returncode}); flash was not confirmed.")
    print("Flash complete; C100 MIDI output reconnected.")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("firmware", nargs="?", type=Path, default=ROOT / FIRMWARE_NAME)
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--setup", action="store_true", help="download and verify Windows utilities; no driver changes")
    mode.add_argument("--check", action="store_true", help="check MIDI/DFU availability without rebooting or flashing")
    parser.add_argument("--dfu-util", help="use an existing dfu-util executable")
    args = parser.parse_args()
    try:
        if args.setup:
            setup_windows()
            return 0
        host = Host(args.dfu_util)
        if args.check:
            dfu, ports = host.dfu(), host.ports()
            print(f"Accessible C100 DFU: {dfu}\nC100 MIDI outputs: {ports}")
            return 0 if dfu or len(ports) == 1 else 1
        flash(host, args.firmware.resolve())
        return 0
    except (OSError, RuntimeError, subprocess.SubprocessError) as error:
        print(f"Error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
