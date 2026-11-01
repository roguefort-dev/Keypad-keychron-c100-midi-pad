"""Inject host responses to exercise flashing without touching USB hardware."""
import contextlib
import ctypes as ct
import importlib.util
import io
from pathlib import Path
import struct
import subprocess
import tempfile
import unittest
from unittest.mock import Mock, patch
import zlib

spec = importlib.util.spec_from_file_location("flash_c100", Path(__file__).resolve().parents[1] / "tools/flash_c100.py")
flash = importlib.util.module_from_spec(spec)
spec.loader.exec_module(flash)


def firmware_bytes():
    data = struct.pack("<II", 0x20000400, 0x08000009) + b"\0" * 24
    data += struct.pack("<HHHH3sB", 0xFFFF, 0xDF11, 0x2E3C, 0x0100, b"UFD", 16)
    return data + struct.pack("<I", ~zlib.crc32(data) & 0xFFFFFFFF)


class FlashTest(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.image = Path(self.temporary.name) / flash.FIRMWARE_NAME
        self.image.write_bytes(firmware_bytes())
        self.host = Mock()
        self.host.ports.return_value = [(1, "Keychron C100 8K")]
        self.host.download.return_value = subprocess.CompletedProcess([], 0, "File downloaded successfully\n", "")

    def run_flash(self):
        with contextlib.redirect_stdout(io.StringIO()):
            flash.flash(self.host, self.image, wait=lambda probe: probe())

    def test_running_and_already_in_dfu(self):
        for already_dfu in (False, True):
            with self.subTest(already_dfu=already_dfu):
                self.host.reset_mock()
                self.host.dfu.side_effect = [already_dfu, True]
                self.run_flash()
                self.assertEqual(self.host.reboot.call_count, int(not already_dfu))
                self.host.download.assert_called_once_with(self.image)

    def test_invalid_image_never_touches_device(self):
        good = firmware_bytes()
        for name, data in (("truncated", good[:12]), ("corrupt", good[:-1] + bytes([good[-1] ^ 1])),
                           ("oversize", b"\0" * (128 * 1024 + 17))):
            with self.subTest(name=name):
                self.image.write_bytes(data)
                with self.assertRaises(RuntimeError):
                    self.run_flash()
                self.assertFalse(self.host.mock_calls)

    def test_wrong_target_and_vectors_rejected_even_with_valid_crc(self):
        for position, value in ((36, 0x00), (0, 0x01), (4, 0x08)):
            with self.subTest(position=position):
                data = bytearray(firmware_bytes())
                data[position] = value
                data[-4:] = struct.pack("<I", ~zlib.crc32(data[:-4]) & 0xFFFFFFFF)
                self.image.write_bytes(data)
                with self.assertRaises(RuntimeError):
                    self.run_flash()
                self.assertFalse(self.host.mock_calls)

    def test_missing_or_ambiguous_midi_never_reboots(self):
        for ports in ([], [(1, "C100"), (2, "C100")]):
            with self.subTest(ports=ports):
                self.host.dfu.return_value = False
                self.host.ports.return_value = ports
                with self.assertRaisesRegex(RuntimeError, "exactly one"):
                    self.run_flash()
                self.host.reboot.assert_not_called()
                self.host.download.assert_not_called()

    def test_dfu_transition_timeout_does_not_download(self):
        self.host.dfu.return_value = False
        with self.assertRaisesRegex(RuntimeError, "did not enter DFU"):
            self.run_flash()
        self.host.download.assert_not_called()

    def test_failure_cannot_be_hidden_by_reconnection(self):
        for status, output, succeeds in ((0, "", True), (74, "File downloaded successfully", True),
                                         (74, "Error during download", False), (1, "", False)):
            with self.subTest(status=status, output=output):
                self.host.dfu.return_value = True
                self.host.download.return_value = subprocess.CompletedProcess([], status, output, "")
                if succeeds:
                    self.run_flash()
                else:
                    with self.assertRaisesRegex(RuntimeError, "not confirmed"):
                        self.run_flash()

    def test_no_reconnection_is_failure_even_after_successful_download(self):
        self.host.dfu.return_value = True
        self.host.ports.return_value = []
        with self.assertRaisesRegex(RuntimeError, "did not reconnect"):
            self.run_flash()

    @patch.object(flash.subprocess, "run")
    def test_dfu_probe_detects_permissions_and_multiple_devices(self, run):
        host = object.__new__(flash.Host)
        host.dfu_util = "dfu-util"
        line = 'Found DFU: [2e3c:df11] ver=0200, alt=0, name="Internal Flash", serial="123"\n'
        for output, status, expected in (("", 0, False), (line, 0, True), (line * 2, 0, None),
                                          ("Cannot open DFU device 2e3c:df11", 74, None), ("probe failed", 1, None)):
            with self.subTest(output=output):
                run.return_value = subprocess.CompletedProcess([], status, output, "")
                if expected is None:
                    with self.assertRaises(RuntimeError):
                        host.dfu()
                else:
                    self.assertEqual(host.dfu(), expected)

    def test_native_midi_layout_and_sysex_buffer_lifetime(self):
        # The WinSDK declares MIDIHDR with one-byte packing on both x86/x64.
        self.assertEqual(ct.sizeof(flash.WindowsMidi.Header), 64 if ct.sizeof(ct.c_void_p) == 4 else 112)
        midi = object.__new__(flash.WindowsMidi)
        midi.mm = Mock()
        for name in ("midiOutOpen", "midiOutPrepareHeader", "midiOutUnprepareHeader", "midiOutReset", "midiOutClose"):
            getattr(midi.mm, name).return_value = 0

        def send(handle, pointer, size):
            header = pointer._obj
            self.assertEqual(ct.string_at(header.data, header.length), flash.DFU_COMMAND)
            header.flags |= 1
            return 0

        midi.mm.midiOutLongMsg.side_effect = send
        midi.reboot((1, "C100"))
        names = [call[0] for call in midi.mm.mock_calls]
        self.assertLess(names.index("midiOutReset"), names.index("midiOutUnprepareHeader"))
        midi.mm.midiOutClose.assert_called_once()


if __name__ == "__main__":
    unittest.main()
