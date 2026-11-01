"""C100-only kernel MIDI fallback for Windows installations missing WinMM ports."""
import ctypes as ct
from contextlib import contextmanager
import struct
import uuid

PIN_PROPERTIES = uuid.UUID("8c134960-51ad-11cf-878a-94f801c10000").bytes_le
CONNECTION_PROPERTIES = uuid.UUID("1d58c920-ac9b-11cf-a5d6-28db04c10000").bytes_le
MUSIC = uuid.UUID("e725d360-62cc-11cf-a5d6-28db04c10000").bytes_le
MIDI = uuid.UUID("1d262760-e957-11cf-a5d6-28db04c10000").bytes_le
NONE = uuid.UUID("0f6417d6-c318-11d0-a43f-00a0c9223196").bytes_le
IOCTL_PROPERTY = 0x2F0003
IOCTL_WRITE_STREAM = 0x2F8013


class Identifier(ct.Structure):
    _fields_ = [("guid", ct.c_ubyte * 16), ("id", ct.c_uint32), ("flags", ct.c_uint32)]


class Connect(ct.Structure):
    _fields_ = [("interface", Identifier), ("medium", Identifier), ("pin", ct.c_uint32),
                ("handle", ct.c_void_p), ("priority", ct.c_uint32), ("subclass", ct.c_uint32)]


class Time(ct.Structure):
    _fields_ = [("time", ct.c_int64), ("numerator", ct.c_uint32), ("denominator", ct.c_uint32)]


class StreamHeader(ct.Structure):
    _fields_ = [("size", ct.c_uint32), ("type_flags", ct.c_uint32), ("time", Time),
                ("duration", ct.c_int64), ("extent", ct.c_uint32), ("used", ct.c_uint32),
                ("data", ct.c_void_p), ("flags", ct.c_uint32)]
    if ct.sizeof(ct.c_void_p) == 8:
        _fields_.append(("reserved", ct.c_uint32))


class Overlapped(ct.Structure):
    _fields_ = [("internal", ct.c_size_t), ("internal_high", ct.c_size_t),
                ("offset", ct.c_uint32), ("offset_high", ct.c_uint32), ("event", ct.c_void_p)]


def midi_range(data):
    """Validate the variable-length KSMULTIPLE_ITEM/KSDATARANGE response."""
    if len(data) < 8:
        return False
    size, count = struct.unpack_from("<II", data)
    if not 8 <= size <= len(data):
        return False
    offset = 8
    found = False
    for _ in range(count):
        if offset + 64 > size:
            return False
        length = struct.unpack_from("<I", data, offset)[0]
        if length < 64 or offset + length > size:
            return False
        if data[offset + 16:offset + 64] == MUSIC + MIDI + NONE:
            found = True
        offset += length
    return found


class KernelMidi:
    def __init__(self):
        self.kernel = ct.WinDLL("kernel32", use_last_error=True)
        self.config = ct.WinDLL("cfgmgr32")
        self.ks = ct.WinDLL("ksuser")
        signatures = {
            "CreateFileW": ([ct.c_wchar_p, ct.c_uint, ct.c_uint, ct.c_void_p, ct.c_uint, ct.c_uint, ct.c_void_p], ct.c_void_p),
            "CloseHandle": ([ct.c_void_p], ct.c_int),
            "CreateEventW": ([ct.c_void_p, ct.c_int, ct.c_int, ct.c_wchar_p], ct.c_void_p),
            "WaitForSingleObject": ([ct.c_void_p, ct.c_uint], ct.c_uint),
            "CancelIoEx": ([ct.c_void_p, ct.POINTER(Overlapped)], ct.c_int),
            "GetOverlappedResult": ([ct.c_void_p, ct.POINTER(Overlapped), ct.POINTER(ct.c_uint), ct.c_int], ct.c_int),
            "DeviceIoControl": ([ct.c_void_p, ct.c_uint, ct.c_void_p, ct.c_uint, ct.c_void_p,
                                 ct.c_uint, ct.POINTER(ct.c_uint), ct.POINTER(Overlapped)], ct.c_int),
        }
        for name, (arguments, result) in signatures.items():
            function = getattr(self.kernel, name)
            function.argtypes, function.restype = arguments, result
        self.ks.KsCreatePin.argtypes = [ct.c_void_p, ct.c_void_p, ct.c_uint, ct.POINTER(ct.c_void_p)]
        self.ks.KsCreatePin.restype = ct.c_uint
        self.config.CM_Get_Device_Interface_List_SizeW.argtypes = [ct.POINTER(ct.c_uint), ct.c_void_p, ct.c_wchar_p, ct.c_uint]
        self.config.CM_Get_Device_Interface_ListW.argtypes = [ct.c_void_p, ct.c_wchar_p, ct.c_wchar_p, ct.c_uint, ct.c_uint]

    @contextmanager
    def open_filter(self, path):
        handle = self.kernel.CreateFileW(path, 0xC0000000, 3, None, 3, 0, None)
        if handle == ct.c_void_p(-1).value:
            raise ct.WinError(ct.get_last_error())
        try:
            yield handle
        finally:
            self.kernel.CloseHandle(handle)

    def ioctl(self, handle, code, request=b"", output=None, asynchronous=False):
        if output is None:
            output = ct.create_string_buffer(4096)
        received = ct.c_uint()
        pending = Overlapped()
        if asynchronous:
            pending.event = self.kernel.CreateEventW(None, True, False, None)
            if not pending.event:
                raise ct.WinError(ct.get_last_error())
        try:
            ok = self.kernel.DeviceIoControl(handle, code, request, len(request), ct.byref(output),
                                             ct.sizeof(output), ct.byref(received),
                                             ct.byref(pending) if asynchronous else None)
            if not ok and asynchronous and ct.get_last_error() == 997:  # ERROR_IO_PENDING
                if self.kernel.WaitForSingleObject(pending.event, 5000) != 0:
                    self.kernel.CancelIoEx(handle, ct.byref(pending))
                    # Drain cancellation before allowing any buffers to be freed.
                    self.kernel.GetOverlappedResult(handle, ct.byref(pending), ct.byref(received), True)
                    raise RuntimeError("C100 kernel MIDI request timed out.")
                ok = self.kernel.GetOverlappedResult(handle, ct.byref(pending), ct.byref(received), False)
            if not ok:
                raise ct.WinError(ct.get_last_error())
            return bytes(output)[:received.value]
        finally:
            if pending.event:
                self.kernel.CloseHandle(pending.event)

    def paths(self):
        category = uuid.UUID("65e8773e-8f56-11d0-a3b9-00a0c9223196").bytes_le  # KSCATEGORY_RENDER
        length = ct.c_uint()
        # Retry only the documented buffer-small race during device arrival.
        for _ in range(3):
            if self.config.CM_Get_Device_Interface_List_SizeW(ct.byref(length), category, None, 0):
                raise RuntimeError("Could not enumerate Windows MIDI driver interfaces.")
            buffer = ct.create_unicode_buffer(length.value)
            result = self.config.CM_Get_Device_Interface_ListW(category, None, buffer, length.value, 0)
            if result == 0:
                return [path for path in buffer[:].split("\0")
                        if "usb#vid_3434&pid_042c&mi_02#" in path.lower()]
            if result != 0x1A:  # CR_BUFFER_SMALL
                break
        raise RuntimeError("Windows MIDI driver interface enumeration failed.")

    def ports(self):
        ports = []
        for path in self.paths():
            with self.open_filter(path) as handle:
                request = PIN_PROPERTIES + struct.pack("<II", 1, 1)  # KSPROPERTY_PIN_CTYPES / GET
                count = struct.unpack("<I", self.ioctl(handle, IOCTL_PROPERTY, request))[0]
                for pin in range(count):
                    def prop(identifier):
                        return self.ioctl(handle, IOCTL_PROPERTY,
                                          PIN_PROPERTIES + struct.pack("<IIII", identifier, 1, pin, 0))
                    if (prop(2) == struct.pack("<I", 1)  # KSPIN_DATAFLOW_IN
                            and prop(7) in (struct.pack("<I", 1), struct.pack("<I", 3))
                            and midi_range(prop(3))):
                        ports.append(((path, pin), "Keychron C100 8K (kernel MIDI)"))
        return ports

    def send(self, port, message):
        path, pin_id = port[0]
        interface = Identifier.from_buffer_copy(uuid.UUID("1a8766a0-62ce-11cf-a5d6-28db04c10000").bytes_le + b"\0" * 8)
        medium = Identifier.from_buffer_copy(uuid.UUID("4747b320-62ce-11cf-a5d6-28db04c10000").bytes_le + b"\0" * 8)
        connect = Connect(interface=interface, medium=medium, pin=pin_id, priority=0x40000000, subclass=1)
        data_format = struct.pack("<IIII", 64, 0, 0, 0) + MUSIC + MIDI + NONE
        connection = ct.create_string_buffer(bytes(connect) + data_format)
        with self.open_filter(path) as handle:
            pin = ct.c_void_p()
            result = self.ks.KsCreatePin(handle, connection, 0xC0000000, ct.byref(pin))
            if result:
                raise OSError(result, "Could not open the C100 kernel MIDI output.")
            try:
                for state in (1, 2, 3):  # ACQUIRE, PAUSE, RUN
                    self.ioctl(pin, IOCTL_PROPERTY, CONNECTION_PROPERTIES + struct.pack("<II", 0, 2),
                               ct.c_uint32(state), asynchronous=True)
                payload = struct.pack("<II", 0, len(message)) + message
                payload += b"\0" * (-len(payload) % 4)
                buffer = ct.create_string_buffer(payload)
                header = StreamHeader(size=ct.sizeof(StreamHeader), extent=len(payload), used=len(payload),
                                      data=ct.addressof(buffer))
                header.time.numerator = header.time.denominator = 1
                self.ioctl(pin, IOCTL_WRITE_STREAM, output=header, asynchronous=True)
            finally:
                self.kernel.CloseHandle(pin)
