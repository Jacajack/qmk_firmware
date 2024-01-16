/* Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// clang-format off

enum jmw_key
{
    // Tap/hold keys
    JMW_MENU = LT(0, KC_F15),
    JMW_HUE  = LT(0, KC_F16),
    JMW_SS   = LT(0, KC_PSCR),

    // Shape keys
    JMW_CIR = KC_MPRV,
    JMW_TRI = KC_MPLY,
    JMW_SQR = KC_MNXT,
    JMW_CRS = KC_MUTE,

    // Application shortcuts
    JMW_AP1 = LAG(KC_F13),
    JMW_AP2 = LAG(KC_F14),
    JMW_AP3 = LAG(KC_F15),
    JMW_AP4 = LAG(KC_F16),
    JMW_AP5 = LAG(KC_F17),

    // Mousekeys aliases
    JMW_ML = KC_MS_LEFT,
    JMW_MR = KC_MS_RIGHT,
    JMW_MD = KC_MS_DOWN,
    JMW_MU = KC_MS_UP,
    JMW_MB1 = KC_MS_BTN1,
    JMW_MB2 = KC_MS_BTN2,

    // Misc aliases
    JMW_BOO = QK_BOOTLOADER,
};

enum layers
{
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
    LAY_CAPS,
};

const uint16_t PROGMEM shapes_xxxx[] = {JMW_CIR, JMW_TRI, JMW_SQR, JMW_CRS, COMBO_END};

const uint16_t PROGMEM shapes_xxxo[] = {JMW_CIR, JMW_TRI, JMW_SQR, COMBO_END};
const uint16_t PROGMEM shapes_oxxx[] = {JMW_TRI, JMW_SQR, JMW_CRS, COMBO_END};
const uint16_t PROGMEM shapes_xoxx[] = {JMW_CIR, JMW_SQR, JMW_CRS, COMBO_END};
const uint16_t PROGMEM shapes_xxox[] = {JMW_CIR, JMW_TRI, JMW_CRS, COMBO_END};

const uint16_t PROGMEM shapes_xxoo[] = {JMW_CIR, JMW_TRI, COMBO_END};
const uint16_t PROGMEM shapes_ooxx[] = {JMW_SQR, JMW_CRS, COMBO_END};
const uint16_t PROGMEM shapes_xoox[] = {JMW_CIR, JMW_CRS, COMBO_END};
const uint16_t PROGMEM shapes_oxxo[] = {JMW_TRI, JMW_SQR, COMBO_END};
const uint16_t PROGMEM shapes_xoxo[] = {JMW_CIR, JMW_SQR, COMBO_END};
const uint16_t PROGMEM shapes_oxox[] = {JMW_TRI, JMW_CRS, COMBO_END};

combo_t key_combos[] = {
    COMBO(shapes_xxxo, KC_F13),
    COMBO(shapes_oxxx, KC_F14),
    COMBO(shapes_xoxx, KC_F15), // Hard to press
    COMBO(shapes_xxox, KC_F16), // Hard to press

    COMBO(shapes_xxoo, KC_F17),
    COMBO(shapes_oxxo, KC_F18),
    COMBO(shapes_ooxx, KC_F19),
    COMBO(shapes_xoxo, KC_F20),
    COMBO(shapes_oxox, KC_F21),
    COMBO(shapes_oxxo, KC_F22),
    COMBO(shapes_xoox, KC_F23),

    COMBO(shapes_xxxx, KC_F24),
};
const uint16_t COMBO_LEN = (sizeof(key_combos) / sizeof(key_combos[0]));

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_109_ansi(
        KC_ESC,        KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,    KC_MUTE,    JMW_SS,   JMW_MENU, JMW_HUE,  JMW_CIR,  JMW_TRI,  JMW_SQR,  JMW_CRS,
        KC_GRV,        KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,    KC_INS,   KC_HOME,  KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,        KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,    KC_DEL,   KC_END,   KC_PGDN,  KC_P7,    KC_P8,    KC_P9,    KC_PPLS,
        MO(LAY_CAPS),  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,                                   KC_P4,    KC_P5,    KC_P6,
        KC_LSFT,                 KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,              KC_UP,              KC_P1,    KC_P2,    KC_P3,    KC_PENT,
        KC_LCTL,       KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, KC_ROPTN, MO(MAC_FN), KC_RCTL,    KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT         ),
    [MAC_FN] = LAYOUT_109_ansi(
        _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,     RGB_TOG,    _______,  _______,  RGB_TOG,  _______,  _______,  _______,  _______,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,                                  _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,              _______,            _______,  _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,            _______         ),
    [WIN_BASE] = LAYOUT_109_ansi(
        KC_ESC,        KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,     KC_MUTE,    JMW_SS,   JMW_MENU, JMW_HUE,  JMW_CIR,  JMW_TRI,  JMW_SQR,  JMW_CRS,
        KC_GRV,        KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,    KC_INS,   KC_HOME,  KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,        KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,    KC_DEL,   KC_END,   KC_PGDN,  KC_P7,    KC_P8,    KC_P9,    KC_PPLS,
        MO(LAY_CAPS),  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,                                   KC_P4,    KC_P5,    KC_P6,
        KC_LSFT,                 KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,              KC_UP,              KC_P1,    KC_P2,    KC_P3,    KC_PENT,
        KC_LCTL,       KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RWIN,  MO(WIN_FN), KC_RCTL,    KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT         ),
    [WIN_FN] = LAYOUT_109_ansi(
        _______,       KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,    RGB_TOG,    _______,  _______,  RGB_TOG,  _______,  _______,  _______,  _______,
        _______,       BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        RGB_TOG,       RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,       RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,                                  _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,              _______,            _______,  _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,            _______         ),
    [LAY_CAPS] = LAYOUT_109_ansi(
        _______,       _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    JMW_BOO,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,       JMW_AP1,  JMW_AP2,  JMW_AP3,  JMW_AP4,  JMW_AP5,  _______,  _______,  _______,  _______,  _______,  KC_VOLD,  KC_VOLU,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,       JMW_MB2,  KC_WH_U,  JMW_MB1,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,       _______,  KC_WH_D,  _______,  _______,  _______,  JMW_ML,   JMW_MD,   JMW_MU,   JMW_MR,   _______,  _______,              _______,                                  _______,  _______,  _______,
        _______,                 _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,              _______,            _______,  _______,  _______,  _______,
        _______,       _______,  _______,                                KC_CAPS,                                _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,            _______         ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][1][2] = {
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [LAY_CAPS] = {ENCODER_CCW_CW(KC_WH_U, KC_WH_D) },
};
#endif // ENCODER_MAP_ENABLE

// Emits kc_tap when the key was tapped and kc_hold when the key is pressed
bool process_tap_hold(const keyrecord_t *record, uint16_t kc_tap, uint16_t kc_hold)
{
    if (record->tap.count && record->event.pressed)
        tap_code16(kc_tap);
    else if (record->event.pressed)
        tap_code16(kc_hold);

    return false;
}

bool process_combos(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode)
    {
        #define TAP_HOLD(kc, kc_tap, kc_hold) case (kc): return process_tap_hold(record, (kc_tap), (kc_hold));
        TAP_HOLD(JMW_MENU, KC_F15,   S(KC_F15));
        TAP_HOLD(JMW_HUE,  KC_F16,   S(KC_F16));
        TAP_HOLD(JMW_SS,   KC_PSCR,  S(KC_PSCR));
        #undef TAP_HOLD
        default: break;
    }

    return true;
}

#if 0
static bool process_jk_scroll(uint16_t keycode, keyrecord_t *record)
{
    uint8_t mod_state = get_mods();
    if (mod_state != MOD_BIT(KC_LEFT_GUI))
        return true;

    if (keycode == KC_J || keycode == KC_K)
    {
        unregister_mods(MOD_MASK_GUI);
        tap_code16(keycode == KC_J ? KC_WH_D : KC_WH_U);
        set_mods(mod_state);
        return false;
    }

    return true;
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    return process_combos(keycode, record);
}

bool encoder_update_user(uint8_t index, bool clockwise)
{
    (void) index;
    uint8_t mod_state = get_mods();

    if (mod_state == MOD_BIT(KC_LSFT))
    {
        unregister_mods(MOD_MASK_SHIFT);
        tap_code16(clockwise ? KC_WH_D : KC_WH_U);
        set_mods(mod_state);
    }
    else
    {
        tap_code16(clockwise ? KC_VOLU : KC_VOLD);
    }

    return false;
}

bool rgb_matrix_indicators_user(void)
{
	if (host_keyboard_led_state().caps_lock)
		rgb_matrix_set_color(CAPS_LOCK_INDEX, 255, 255, 255);

	if (!host_keyboard_led_state().num_lock)
		rgb_matrix_set_color(NUM_LOCK_INDEX, 0, 0, 0);

	return false;
}

void keyboard_post_init_user(void)
{
    rgb_matrix_enable();
    rgb_matrix_mode(RGB_MATRIX_CUSTOM_jmw_glimmer);
}
