QMK_HOME ?= ../qmk_firmware
USERSPACE_PATH ?= $(CURDIR)

.PHONY: build clean

build:
	$(MAKE) -C "$(QMK_HOME)" keychron/c100_8k:midi_pad QMK_USERSPACE="$(USERSPACE_PATH)"

clean:
	$(MAKE) -C "$(QMK_HOME)" clean QMK_USERSPACE="$(USERSPACE_PATH)"
