#!/usr/bin/env python3
"""Check the WinMM input ports visible to Windows DAWs, without a USB fallback."""
import argparse
import ctypes as ct
import json
import os

if __package__:
    from .flash_c100 import WindowsMidi
else:
    from flash_c100 import WindowsMidi


class InputCaps(ct.Structure):
    _pack_ = 1
    _fields_ = WindowsMidi.Caps._fields_[:4] + [("support", ct.c_uint32)]


def enumerate_ports(api=None):
    """Accept an injected WinMM API for tests; never open or reboot a device."""
    api = api if api is not None else ct.WinDLL("winmm")
    result = {}
    for direction, caps_type in (("In", InputCaps), ("Out", WindowsMidi.Caps)):
        count = getattr(api, f"midi{direction}GetNumDevs")
        count.argtypes, count.restype = [], ct.c_uint
        capabilities = getattr(api, f"midi{direction}GetDevCapsW")
        capabilities.argtypes = [ct.c_size_t, ct.POINTER(caps_type), ct.c_uint]
        capabilities.restype = ct.c_uint
        ports = []
        for index in range(count()):
            info = caps_type()
            status = capabilities(index, ct.byref(info), ct.sizeof(info))
            ports.append({"index": index, "name": info.name, "status": status})
        result[direction] = ports
    return result


def has_input(ports, name):
    return any(name.casefold() in port["name"].casefold() and port["status"] == 0
               for port in ports["In"])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--name", default="Keychron C100", help="part of the required MIDI input name")
    args = parser.parse_args()
    if os.name != "nt":
        parser.error("This check requires Windows.")
    ports = enumerate_ports()
    print(json.dumps(ports, indent=2))
    found = has_input(ports, args.name)
    print(f"{args.name} WinMM input: {'PASS' if found else 'FAIL'}")
    return 0 if found else 1


if __name__ == "__main__":
    raise SystemExit(main())
