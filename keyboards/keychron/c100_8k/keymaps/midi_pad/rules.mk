MIDI_ENABLE = yes
VIA_ENABLE = no
SRC += usb_descriptor_midi.c usb_midi_transport.c
LDFLAGS += -Wl,--wrap=usbStart

# Keychron's stock Pixel Rain effect has an unused state variable under GCC 16.
# Keep upstream warnings strict otherwise while allowing that known warning.
CFLAGS += -Wno-error=unused-but-set-variable

RGB_MATRIX_CUSTOM_USER = yes
