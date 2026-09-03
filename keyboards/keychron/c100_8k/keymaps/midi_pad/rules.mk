MIDI_ENABLE = yes
VIA_ENABLE = no

# Keychron's stock Pixel Rain effect has an unused state variable under GCC 16.
# Keep upstream warnings strict otherwise while allowing that known warning.
CFLAGS += -Wno-error=unused-but-set-variable
