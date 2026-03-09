// keymap.c — sammy_plank keymap for planck/rev6
// Author: Sammy Webb
// Generated from QMK Configurator export.
// Target OS layout: British English (Windows / Linux — tested on Fedora)
//
// Layers:
//   0 — QWERTY        (default)
//   1 — Colemak-DH    (alternate default, switch via DF(1) / DF(0) in Adjust)
//   2 — Nav / Fn      (hold LT2 keys on bottom row)
//   3 — Symbols / Num (hold LT3 keys on bottom row)
//   4 — Adjust        (hold both LT3 and MO4 simultaneously)

#include QMK_KEYBOARD_H

#include "sendstring_uk.h"

// ── Layer indices ─────────────────────────────────────────────────────────────
// Note: OS_HYPR is a built-in QMK keycode (defined in quantum_keycodes.h)
// and is used directly in the keymap below — no custom definition needed.
enum layers {
    _QWERTY = 0,
    _COLEMAK,
    _NAV,
    _SYM,
    _ADJUST,
};

// ── Keymap ────────────────────────────────────────────────────────────────────
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Layer 0 — QWERTY
     * ,-----------------------------------------------------------------------------------.
     * | Tab  |  Q   |  W   |  E   |  R   |  T   |  Y   |  U   |  I   |  O   |  P   |  \  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |Esc/` |A/CTL |S/ALT |D/GUI |F/SFT |  G   |  H   |J/SFT |K/GUI |L/ALT |;/CTL |  '  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |  [   |  Z   |  X   |  C   |  V   |  B   |  N   |  M   |  ,   |  .   |  /   |  ]  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |OSHYPR| Copy | Pste | LGUI |LT3/↵ |LT2/⌫ |LT2/SP|LT3/↵ | LALT | Vol- | Vol+ |LT4/⏵|
     * `-----------------------------------------------------------------------------------'
     */
    [_QWERTY] = LAYOUT_ortho_4x12(
        KC_TAB,  KC_Q,          KC_W,          KC_E,          KC_R,          KC_T,
        KC_Y,    KC_U,          KC_I,          KC_O,          KC_P,          KC_BSLS,

        QK_GESC, LCTL_T(KC_A),  LALT_T(KC_S),  LGUI_T(KC_D),  LSFT_T(KC_F),  KC_G,
        KC_H,    RSFT_T(KC_J),  RGUI_T(KC_K),  RALT_T(KC_L),  RCTL_T(KC_SCLN), KC_QUOT,

        KC_LBRC, KC_Z,          KC_X,          KC_C,          KC_V,          KC_B,
        KC_N,    KC_M,          KC_COMM,       KC_DOT,        KC_SLSH,       KC_RBRC,

        OS_HYPR, KC_COPY,       KC_PSTE,       KC_LGUI,       LT(_SYM,KC_ENT),  LT(_NAV,KC_BSPC),
        LT(_NAV,KC_SPC), LT(_SYM,KC_ENT), KC_LALT, KC_VOLD, KC_VOLU, LT(_ADJUST,KC_MPLY)
    ),

    /* Layer 1 — Colemak-DH
     * ,-----------------------------------------------------------------------------------.
     * |(trns)|  Q   |  W   |  F   |  P   |  B   |  J   |  L   |  U   |  Y   |  ;   |(trns)|
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |(trns)|A/CTL |R/ALT |S/GUI |T/SFT |  G   |  M   |N/SFT |E/GUI |I/ALT |O/CTL |(trns)|
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |(trns)|  Z   |  X   |  C   |  D   |  V   |  K   |  H   |  ,   |  .   |  /   |(trns)|
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |(trns)|(trns)|(trns)|(trns)|(trns)|(trns)|(trns)|(trns)|(trns)|(trns)|(trns)|(trns)|
     * `-----------------------------------------------------------------------------------'
     */
    [_COLEMAK] = LAYOUT_ortho_4x12(
        KC_TRNS, KC_Q,          KC_W,          KC_F,          KC_P,          KC_B,
        KC_J,    KC_L,          KC_U,          KC_Y,          KC_SCLN,       KC_TRNS,

        KC_TRNS, LCTL_T(KC_A),  LALT_T(KC_R),  LGUI_T(KC_S),  LSFT_T(KC_T),  KC_G,
        KC_M,    RSFT_T(KC_N),  RGUI_T(KC_E),  RALT_T(KC_I),  RCTL_T(KC_O),  KC_TRNS,

        KC_TRNS, KC_Z,          KC_X,          KC_C,          KC_D,          KC_V,
        KC_K,    KC_H,          KC_COMM,       KC_DOT,        KC_SLSH,       KC_TRNS,

        KC_TRNS, KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS,
        KC_TRNS, KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS
    ),

    /* Layer 2 — Nav / Fn
     * ,-----------------------------------------------------------------------------------.
     * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  | F10  | F11  | F12  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      | LCTL | LALT | LGUI | LSFT |CW_TOG|  ←   |  ↓   |  ↑   |  →   | Del  |      |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |      | Ins  | Caps | Home | PgDn | PgUp | End  |      |      |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |(trns)|(trns)|(trns)|(trns)|(trns)|(trns)|      |      |      |
     * `-----------------------------------------------------------------------------------'
     */
    [_NAV] = LAYOUT_ortho_4x12(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,

        KC_NO,   KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, CW_TOGG,
        KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_DEL,  KC_NO,

        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_INS,  KC_CAPS,
        KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_NO,   KC_NO,

        KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,   KC_NO,   KC_NO
    ),

    /* Layer 3 — Symbols / Numbers  (British English layout)
     * ,-----------------------------------------------------------------------------------.
     * | Tab  |  !   |  @   |  #   |  $   |  %   |  ^   |  &   |  *   |  (   |  )   |  |  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Del  |1/CTL |2/ALT |3/GUI |4/SFT |  5   |  6   |7/SFT |8/GUI |9/ALT |0/CTL |  =  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |  {   |  `   |  £   |  €   |  _   |  ~   |  +   |  -   |  ,   |  .   |  /   |  }  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |(trns)| MO4  |(trns)|(trns)| MO4  |(trns)|      |      |      |
     * `-----------------------------------------------------------------------------------'
     *
     * UK layout notes:
     *   KC_NUHS  = the physical #/~ key on a UK board (next to Enter)
     *   On a UK layout, Shift+3 = £, so KC_HASH here actually sends £.
     *   Row 1 KC_HASH is intentionally kept — on a UK OS layout it outputs £.
     *   KC_NUHS  on row 1 position 4 outputs # (the non-US hash key).
     *   £  = LSFT(KC_3)  on Windows/Linux UK layout.
     *   €  = RALT(KC_4)  on Windows/Linux UK layout (AltGr+4).
     */
    [_SYM] = LAYOUT_ortho_4x12(
        KC_TAB,  KC_EXLM,       KC_AT,         KC_NUHS,       KC_DLR,        KC_PERC,
        KC_CIRC, KC_AMPR,       KC_ASTR,       KC_LPRN,       KC_RPRN,       KC_PIPE,

        KC_DEL,  LCTL_T(KC_1),  LALT_T(KC_2),  LGUI_T(KC_3),  LSFT_T(KC_4),  KC_5,
        KC_6,    RSFT_T(KC_7),  RGUI_T(KC_8),  RALT_T(KC_9),  RCTL_T(KC_0),  KC_EQL,

        KC_LCBR, KC_GRV,        LSFT(KC_3),    RALT(KC_4),    KC_UNDS,       KC_TILD,
        KC_PLUS, KC_MINS,       KC_COMM,       KC_DOT,        KC_SLSH,       KC_RCBR,

        KC_NO,   KC_NO,         KC_NO,         KC_TRNS,       MO(_ADJUST),   KC_TRNS,
        KC_TRNS, MO(_ADJUST),   KC_TRNS,       KC_NO,         KC_NO,         KC_NO
    ),

    /* Layer 4 — Adjust  (reached by holding both LT3 and MO4 at once)
     * ,-----------------------------------------------------------------------------------.
     * |      | BOOT |DBTOG |      |      |      |      |      |      |      |      | Del  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |      |      |AGNORM|AGSWAP| DF(0)| DF(1)|      |      |      |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |      |      |      |      |      |      |      |      |      |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |      |      |      |      |      |      | Prev | Next | Play |
     * `-----------------------------------------------------------------------------------'
     */
    [_ADJUST] = LAYOUT_ortho_4x12(
        KC_NO,   QK_BOOT, DB_TOGG, KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_DEL,

        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   AG_NORM,
        AG_SWAP, DF(0),   DF(1),   KC_NO,   KC_NO,   KC_NO,

        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,

        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_MPRV, KC_MNXT, KC_MPLY
    ),
};
