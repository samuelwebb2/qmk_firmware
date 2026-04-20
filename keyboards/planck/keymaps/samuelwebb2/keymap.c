// keymap.c — sammy_plank keymap for planck/rev6
// Author: Sammy Webb
// Target OS layout: British English (Windows / Linux / macOS)
//
// Layers:
//   0 — QWERTY        (default)
//   1 — Colemak-DH    (alternate default, switch via DF(1) / DF(0) in Adjust)
//   2 — Nav / Fn      (hold LT2 keys on bottom row)
//   3 — Symbols / Num (hold LT3 keys on bottom row)
//   4 — Adjust        (hold both LT3 and MO4 simultaneously)
//
// macOS notes:
//   OS detection fires automatically on USB connect and sets is_mac.
//   GUI/Ctrl are swapped in RAM (no EEPROM write) so Cmd shortcuts work on Mac
//   without touching CG_SWAP/CG_NORM in Adjust.
//
//   macOS "British" layout swaps KC_NUBS and KC_NUHS relative to Windows/Linux:
//     PC British:   KC_NUBS = \ |     KC_NUHS = # ~
//     Mac British:  KC_NUBS = § ±     KC_NUHS = \ |
//   UK_BSLS_AUTO / UK_PIPE_AUTO pick the right code per OS automatically.
//
//   If you use "British - PC" on macOS (System Settings → Keyboard → Input Sources)
//   the keycodes match Windows/Linux exactly and the Mac-specific overrides below
//   are not needed — but they are harmless (they'd never fire since detected_host_os
//   would only return OS_MACOS when on macOS, which uses "British - PC" identically).
//
//   ~ on macOS "British": Shift+Option+§ = LSA(KC_GRAVE).
//   If this doesn't produce ~ on your setup, try S(KC_NUBS) or LALT(KC_GRAVE).

#include QMK_KEYBOARD_H
#include "keymap_uk.h"
#include "os_detection.h"

#ifdef AUDIO_ENABLE
#    include "muse.h"
#endif

// ── OS detection ──────────────────────────────────────────────────────────────
static bool is_mac = false;

bool process_detected_host_os_user(os_variant_t os) {
    is_mac = (os == OS_MACOS || os == OS_IOS);
    // Auto-swap Ctrl↔GUI in RAM so Mac shortcuts (Cmd+C etc.) work without
    // manually pressing CG_SWAP.  No EEPROM write — resets on every reconnect,
    // so manual CG_NORM/CG_SWAP in Adjust still work as a session override.
    keymap_config.swap_lctl_lgui = is_mac;
    keymap_config.swap_rctl_rgui = is_mac;
    return true;
}

// ── Custom keycodes ───────────────────────────────────────────────────────────
// UK_GESC: UK-aware reimplementation of QK_GESC with full per-modifier logic.
//
//   Modifier held   Key registered              PC output    Mac output
//   ─────────────   ──────────────              ─────────    ──────────
//   none            KC_ESCAPE                   Escape       Escape
//   Shift           KC_NUHS (PC) / LSA(GRAVE)   ~            ~
//   GUI             KC_GRAVE                    `            `
//   Ctrl            KC_GRAVE (w/ ctrl)          Ctrl+`       Ctrl+`
//   Alt             KC_ESCAPE (w/ alt)          Alt+Esc      Alt+Esc
//
//   On Mac "British" layout KC_NUBS/KC_NUHS are swapped vs PC:
//     PC:  S(KC_NUHS) = ~
//     Mac: S(KC_NUHS) = |  — so we use LSA(KC_GRAVE) instead.
//          LSA = Shift+Option+§ = ~ on macOS "British" keyboard layout.
//          If ~ still isn't produced, try replacing KC_GRAVE with KC_NUBS.
//
// UK_BSLS_AUTO: OS-aware \ / | key (replaces UK_BSLS in the base layers).
//   PC:  KC_NUBS  → \ (unshifted),  | (shifted)
//   Mac: KC_NUHS  → \ (unshifted),  | (shifted)
//
// UK_PIPE_AUTO: OS-aware explicit-| key (replaces UK_PIPE in the SYM layer).
//   PC:  S(KC_NUBS) → |
//   Mac: S(KC_NUHS) → |
enum custom_keycodes {
    UK_GESC = SAFE_RANGE,
    UK_BSLS_AUTO,
    UK_PIPE_AUTO,
};

// Tracks which key was registered on press for correct release handling.
typedef enum { GESC_ESC, GESC_GRAVE, GESC_TILD, GESC_TILD_MAC } uk_gesc_state_t;
static uk_gesc_state_t uk_gesc_pressed = GESC_ESC;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case UK_GESC: {
        const uint8_t mods = get_mods();

        if (record->event.pressed) {
            if (mods & MOD_MASK_SHIFT) {
                if (is_mac) {
                    // Mac "British": ~ = Shift+Option+§ (LSA = Shift+Alt).
                    // Shift is already in the report; we add Alt and KC_GRAVE.
                    uk_gesc_pressed = GESC_TILD_MAC;
                    add_mods(MOD_LALT);
                    add_key(KC_GRAVE);
                } else {
                    // PC UK: ~ = Shift+KC_NUHS (shift already in the report)
                    uk_gesc_pressed = GESC_TILD;
                    add_key(KC_NUHS);
                }
            } else if (mods & (MOD_MASK_GUI | MOD_MASK_CTRL)) {
                // GUI or Ctrl held → ` (KC_GRAVE; modifier stays in the report)
                uk_gesc_pressed = GESC_GRAVE;
                add_key(KC_GRAVE);
            } else {
                // No relevant modifier (includes Alt+Esc) → Escape
                uk_gesc_pressed = GESC_ESC;
                add_key(KC_ESCAPE);
            }
        } else {
            switch (uk_gesc_pressed) {
                case GESC_TILD_MAC: del_key(KC_GRAVE); del_mods(MOD_LALT); break;
                case GESC_TILD:     del_key(KC_NUHS);                      break;
                case GESC_GRAVE:    del_key(KC_GRAVE);                      break;
                case GESC_ESC:      del_key(KC_ESCAPE);                     break;
            }
        }

        send_keyboard_report();
        return false;
    }

    case UK_BSLS_AUTO:
        // PC UK: KC_NUBS = \ | ;  Mac British: KC_NUHS = \ |
        if (record->event.pressed) {
            register_code(is_mac ? KC_NUHS : KC_NUBS);
        } else {
            unregister_code(is_mac ? KC_NUHS : KC_NUBS);
        }
        return false;

    case UK_PIPE_AUTO:
        // PC UK: S(KC_NUBS) = | ;  Mac British: S(KC_NUHS) = |
        if (record->event.pressed) {
            register_code16(is_mac ? S(KC_NUHS) : S(KC_NUBS));
        } else {
            unregister_code16(is_mac ? S(KC_NUHS) : S(KC_NUBS));
        }
        return false;

    default:
        break;
    }
    return true;
}

// ── Layer indices ─────────────────────────────────────────────────────────────
enum layers {
    _QWERTY = 0,
    _COLEMAK,
    _NAV,
    _SYM,
    _ADJUST,
};

#define QWERTY PDF(_QWERTY)
#define COLEMAK PDF(_COLEMAK)

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
     * | REPT | Copy | Pste | LGUI |LT3/↵ |LT2/⌫ |LT2/SP|LT3/↵ | RALT | Vol- | Vol+ |LT4/⏵|
     * `-----------------------------------------------------------------------------------'
     *
     * UK_BSLS_AUTO = \ / | key, uses KC_NUBS on PC and KC_NUHS on Mac.
     * UK_GESC = Esc (plain) | ~ (Shift) | ` (GUI) | Ctrl+` (Ctrl) | Alt+Esc (Alt)
     */
    [_QWERTY] = LAYOUT_ortho_4x12(
        KC_TAB,  KC_Q,          KC_W,          KC_E,          KC_R,          KC_T,
        KC_Y,    KC_U,          KC_I,          KC_O,          KC_P,          UK_BSLS_AUTO,

        UK_GESC, LCTL_T(KC_A),  LALT_T(KC_S),  LGUI_T(KC_D),  LSFT_T(KC_F),  KC_G,
        KC_H,    RSFT_T(KC_J),  RGUI_T(KC_K),  LALT_T(KC_L),  RCTL_T(KC_SCLN), UK_QUOT,

        KC_LBRC, KC_Z,          KC_X,          KC_C,          KC_V,          KC_B,
        KC_N,    KC_M,          KC_COMM,       KC_DOT,        KC_SLSH,       KC_RBRC,

        QK_REP,  KC_COPY,       KC_PSTE,       KC_LGUI,       LT(_SYM,KC_ENT),  LT(_NAV,KC_BSPC),
        LT(_NAV,KC_SPC), LT(_SYM,KC_ENT), KC_RALT, KC_VOLD, KC_VOLU, LT(_ADJUST,KC_MPLY)
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
        KC_M,    RSFT_T(KC_N),  RGUI_T(KC_E),  LALT_T(KC_I),  RCTL_T(KC_O),  KC_TRNS,

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
     * | Tab  |  !   |  "   |  #   |  $   |  %   |  ^   |  &   |  *   |  (   |  )   |  |  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Del  |1/CTL |2/ALT |3/GUI |4/SFT |  5   |  6   |7/SFT |8/GUI |9/ALT |0/CTL |  =  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |  {   |  `   |  €   |  £   |  _   |  :   |  +   |  -   |  ,   |  .   |  /   |  }  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |(trns)| MO4  |(trns)|(trns)| MO4  |(trns)|      |      |      |
     * `-----------------------------------------------------------------------------------'
     *
     * All symbols use UK_ aliases from keymap_uk.h so the correct character
     * is produced regardless of which UK-layout key physically maps where.
     *
     * UK_AT   = @  (Shift+2 on UK = ", so UK_AT compensates)
     * UK_HASH = #  (the non-US hash key)
     * UK_PIPE_AUTO = |  (correct | on both PC and Mac UK layout)
     * UK_LCBR = {  (correct { on UK layout)
     * UK_RCBR = }  (correct } on UK layout)
     * UK_GRV  = `  (correct ` on UK layout; ~ via Shift+` naturally)
     * UK_PND  = £  (aligned with the 3 key, matching the physical UK keyboard)
     * UK_EURO = €  (aligned with the 2 key)
     * ~ is not needed here — use Shift+UK_GESC or Shift+` from this layer.
     */
    [_SYM] = LAYOUT_ortho_4x12(
        KC_TAB,  KC_EXLM,       UK_DQUO,         UK_HASH,       KC_DLR,        KC_PERC,
        KC_CIRC, KC_AMPR,       KC_ASTR,       KC_LPRN,       KC_RPRN,       UK_PIPE_AUTO,

        KC_DEL,  LCTL_T(KC_1),  LALT_T(KC_2),  LGUI_T(KC_3),  LSFT_T(KC_4),  KC_5,
        KC_6,    RSFT_T(KC_7),  RGUI_T(KC_8),  LALT_T(KC_9),  RCTL_T(KC_0),  KC_EQL,

        UK_LCBR, UK_GRV,        UK_PND,        UK_EURO,       KC_UNDS,       UK_COLN,
        KC_PLUS, KC_MINS,       KC_COMM,       KC_DOT,        KC_SLSH,       UK_RCBR,

        KC_NO,   KC_NO,         KC_NO,         KC_TRNS,       MO(_ADJUST),   KC_TRNS,
        KC_TRNS, MO(_ADJUST),   KC_TRNS,       KC_NO,         KC_NO,         KC_NO
    ),

    /* Layer 4 — Adjust  (reached by holding both LT3 and MO4 at once or by holding play)
     * ,-----------------------------------------------------------------------------------.
     * | MAKE | BOOT |DBTOG |      |      |      |      |      |      |      |      | Del  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |      |      |CGNORM|CGSWAP|QWERTY|COLEMA|     |      |      |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |      |      |      |      |      |      |      |      |      |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |      |      |      |      |      |      | Prev | Next | Play |
     * `-----------------------------------------------------------------------------------'
     */
    [_ADJUST] = LAYOUT_ortho_4x12(
        QK_MAKE, QK_BOOT, DB_TOGG, KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_DEL,

        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   CG_NORM,
        CG_SWAP, QWERTY,  COLEMAK, KC_NO,   KC_NO,   KC_NO,

        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,

        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_MPRV, KC_MNXT, KC_MPLY
    ),
};
