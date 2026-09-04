QMK_HOME ?= ../qmk_firmware
USERSPACE_PATH ?= $(CURDIR)

.PHONY: build clean flash

build:
	@set -eu; \
	temp_dir=$$(mktemp -d); \
	ln -s "$(USERSPACE_PATH)" "$$temp_dir/userspace"; \
	trap 'rm -f "$$temp_dir/userspace"; rmdir "$$temp_dir"' EXIT; \
	$(MAKE) -C "$(QMK_HOME)" keychron/c100_8k:midi_pad QMK_USERSPACE="$$temp_dir/userspace"

clean:
	@set -eu; \
	temp_dir=$$(mktemp -d); \
	ln -s "$(USERSPACE_PATH)" "$$temp_dir/userspace"; \
	trap 'rm -f "$$temp_dir/userspace"; rmdir "$$temp_dir"' EXIT; \
	$(MAKE) -C "$(QMK_HOME)" clean QMK_USERSPACE="$$temp_dir/userspace"

flash: build
	@./tools/flash-c100
