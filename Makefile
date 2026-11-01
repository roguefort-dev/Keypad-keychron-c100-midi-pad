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

HOST_CC ?= cc
HOST_CFLAGS ?= -Os -Wall -Wextra -Werror
KEYMAP_DIR := keyboards/keychron/c100_8k/keymaps/midi_pad
HOST_TESTS := arp_logic chord_degree_logic note_pool pending_notes

.PHONY: test test-sanitize benchmark

test:
	@mkdir -p .build/tests
	@set -eu; for name in $(HOST_TESTS); do \
		$(HOST_CC) -std=gnu11 $(HOST_CFLAGS) tests/$${name}_test.c -o .build/tests/$$name; \
		.build/tests/$$name; \
	done
	$(HOST_CC) -std=gnu11 $(HOST_CFLAGS) -Itests/host -I$(KEYMAP_DIR) \
		-DQMK_KEYBOARD_H='"qmk_keyboard.h"' tests/performance.c tests/host/runtime.c \
		-o .build/tests/keymap
	.build/tests/keymap --replay > .build/tests/replay.actual
	diff -u tests/keymap-replay.expected .build/tests/replay.actual
	@cat .build/tests/replay.actual

test-sanitize:
	$(MAKE) test HOST_CFLAGS="-O1 -g -Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer"

benchmark:
	./tools/benchmark-local $(BENCH_ARGS)
