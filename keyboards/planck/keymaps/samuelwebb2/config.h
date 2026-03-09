// config.h — sammy_plank keymap for planck/rev6
#pragma once

// ── Tapping term ─────────────────────────────────────────────────────────────
// Global tapping term (ms). Tap within this window → tap, hold beyond → hold.
#define TAPPING_TERM 200

// Per-key tapping term support — override get_tapping_term() in keymap.c
// if you ever want different timings for specific keys.
#define TAPPING_TERM_PER_KEY

// Prevent accidental mod activation when rolling keys quickly.
#define PERMISSIVE_HOLD

// ── One-shot keys ─────────────────────────────────────────────────────────────
// How long a one-shot modifier stays active after being tapped (ms).
#define ONESHOT_TIMEOUT 3000

// ── Caps Word ─────────────────────────────────────────────────────────────────
#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD
#define CAPS_WORD_INVERT_ON_SHIFT