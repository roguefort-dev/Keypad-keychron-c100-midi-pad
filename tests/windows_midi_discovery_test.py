"""Exercise DAW discovery using injected WinMM responses, without USB access."""
import unittest
from unittest.mock import Mock

from tools.check_windows_midi import enumerate_ports, has_input


def winmm(inputs, outputs):
    api = Mock()
    for direction, records in (("In", inputs), ("Out", outputs)):
        getattr(api, f"midi{direction}GetNumDevs").return_value = len(records)

        def capabilities(index, pointer, size, records=records):
            name, status = records[index]
            pointer._obj.name = name
            return status

        getattr(api, f"midi{direction}GetDevCapsW").side_effect = capabilities
    return api


class DiscoveryTest(unittest.TestCase):
    def test_reports_inputs_and_outputs_without_treating_output_as_input(self):
        ports = enumerate_ports(winmm([], [("Example MIDI device", 0)]))
        self.assertEqual(ports["Out"][0]["name"], "Example MIDI device")
        self.assertFalse(has_input(ports, "Example"))

    def test_selects_matching_input_among_other_ports(self):
        ports = enumerate_ports(winmm([("Other device", 0), ("Example MIDI device", 0)], []))
        self.assertTrue(has_input(ports, "example midi"))
        self.assertFalse(has_input(ports, "Missing device"))
        self.assertEqual(ports["In"][1]["index"], 1)

    def test_capability_error_never_reports_success(self):
        for status in (2, 5, 6):
            with self.subTest(status=status):
                ports = enumerate_ports(winmm([("Example MIDI device", status)], []))
                self.assertFalse(has_input(ports, "Example"))
                self.assertEqual(ports["In"][0]["status"], status)


if __name__ == "__main__":
    unittest.main()
