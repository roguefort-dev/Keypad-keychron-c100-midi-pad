// Copyright 2026 roguefort-dev
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// The MIDI engine tracks note/channel pairs itself so notes cannot become
// stuck when a setting changes while a pad is held.
#define MIDI_PAD_BASE_C_NOTE 36
#define MIDI_PAD_DEFAULT_VELOCITY 100
