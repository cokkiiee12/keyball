/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

enum custom_keycodes {
    GEST_MO = KEYBALL_SAFE_RANGE,
    WIN_PREV,
    WIN_NEXT,
    WIN_LEFT,
    WIN_RIGHT,
    WIN_MAX,
    WIN_MIN,
    WIN_DESK,
    GEST_WM,
    MAC_MOD,
};

#define SCROLL_LAYER      4
#define GESTURE_LAYER     4
#define GESTURE_THRESHOLD 24

static bool    gesture_hold  = false;
static bool    gesture_fired = false;
static int16_t gesture_x     = 0;
static int16_t gesture_y     = 0;
static uint8_t gesture_mode  = 0;
static bool    mac_mod_hold  = false;

enum {
    GESTURE_MODE_NONE = 0,
    GESTURE_MODE_SWITCH,
    GESTURE_MODE_WINDOW,
};

static inline int16_t iabs16(int16_t v) {
    return v < 0 ? -v : v;
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_universal(
    KC_ESC   , KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                                  KC_6     , KC_7     , KC_8     , KC_9     , KC_0     , KC_MINS  ,
    KC_DEL   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                  KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_INT3  ,
    KC_TAB   , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                                  KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  , S(KC_7)  ,
    MO(1)    , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     , KC_RBRC  ,              KC_NUHS, KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_RSFT  ,
    _______  , KC_LCTL  , KC_LALT  , KC_LGUI,LT(1,KC_LNG2),LT(2,KC_SPC),LT(3,KC_LNG1),    KC_BSPC,LT(2,KC_ENT),LT(1,KC_LNG2),KC_RGUI, _______ , KC_RALT  , KC_PSCR
  ),

  [1] = LAYOUT_universal(
    S(KC_ESC), S(KC_1)  , KC_LBRC  , S(KC_3)  , S(KC_4)  , S(KC_5)  ,                                  KC_EQL   , S(KC_6)  ,S(KC_QUOT), S(KC_8)  , S(KC_9)  ,S(KC_INT1),
    S(KC_DEL), S(KC_Q)  , S(KC_W)  , S(KC_E)  , S(KC_R)  , S(KC_T)  ,                                  S(KC_Y)  , S(KC_U)  , S(KC_I)  , S(KC_O)  , S(KC_P)  ,S(KC_INT3),
    S(KC_TAB), S(KC_A)  , S(KC_S)  , S(KC_D)  , S(KC_F)  , S(KC_G)  ,                                  S(KC_H)  , S(KC_J)  , S(KC_K)  , S(KC_L)  , KC_QUOT  , S(KC_2)  ,
    _______  , S(KC_Z)  , S(KC_X)  , S(KC_C)  , S(KC_V)  , S(KC_B)  ,S(KC_RBRC),           S(KC_NUHS), S(KC_N)  , S(KC_M)  ,S(KC_COMM), S(KC_DOT),S(KC_SLSH),S(KC_RSFT),
    _______  ,S(KC_LCTL),S(KC_LALT),S(KC_LGUI), _______  , _______  , _______  ,            _______  , _______  , _______  ,S(KC_RGUI), _______  , S(KC_RALT), _______
  ),

  [2] = LAYOUT_universal(
    SSNP_FRE , KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                                  KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    SSNP_VRT , _______  , KC_7     , KC_8     , KC_9     , _______  ,                                  _______  , KC_LEFT  , KC_UP    , KC_RGHT  , _______  , KC_F12   ,
    SSNP_HOR , _______  , KC_4     , KC_5     , KC_6     ,S(KC_SCLN),                                  KC_PGUP  , KC_BTN1  , KC_DOWN  , KC_BTN2  , KC_BTN3  , _______  ,
    _______  , _______  , KC_1     , KC_2     , KC_3     ,S(KC_MINS), S(KC_8)  ,            S(KC_9)  , KC_PGDN  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , KC_0     , KC_DOT   , _______  , _______  , _______  ,             KC_DEL  , _______  , _______  , _______  , _______  , _______  , _______
  ),

  [3] = LAYOUT_universal(
    SSNP_FRE , KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                                  KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    SSNP_VRT , _______  , KC_7     , KC_8     , KC_9     , _______  ,                                  _______  , KC_LEFT  , KC_UP    , KC_RGHT  , _______  , KC_F12   ,
    SSNP_HOR , _______  , KC_4     , KC_5     , KC_6     ,S(KC_SCLN),                                  KC_PGUP  , KC_BTN1  , KC_DOWN  , KC_BTN2  , KC_BTN3  , _______  ,
    _______  , _______  , KC_1     , KC_2     , KC_3     ,S(KC_MINS), S(KC_8)  ,            S(KC_9)  , KC_PGDN  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , KC_0     , KC_DOT   , _______  , _______  , _______  ,             KC_DEL  , _______  , _______  , _______  , _______  , _______  , _______
  ),

  [4] = LAYOUT_universal(
    RGB_TOG  , AML_TO   , AML_I50  , AML_D50  , _______  , _______  ,                                  RGB_M_P  , RGB_M_B  , RGB_M_R  , RGB_M_SW , RGB_M_SN , RGB_M_K  ,
    RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  , _______  , _______  ,                                  RGB_M_X  , RGB_M_G  , RGB_M_T  , RGB_M_TW , _______  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , _______  , _______  ,                                  CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , KBC_SAVE , KBC_RST  ,
    _______  , _______  , SCRL_DVD , SCRL_DVI , SCRL_MO  , SCRL_TO  , EE_CLR   ,            EE_CLR   , KC_HOME  , KC_PGDN  , KC_PGUP  , KC_END   , _______  , _______  ,
    QK_BOOT  , _______  , KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , _______  ,            _______  , KC_BSPC  , _______  , _______  , _______  , _______  , QK_BOOT
  ),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case GEST_MO:
            if (record->event.pressed) {
                gesture_hold  = true;
                gesture_fired = false;
                gesture_x     = 0;
                gesture_y     = 0;
                gesture_mode  = GESTURE_MODE_SWITCH;
            } else {
                gesture_hold  = false;
                gesture_fired = false;
                gesture_x     = 0;
                gesture_y     = 0;
                gesture_mode  = GESTURE_MODE_NONE;
            }
            return false;
        case GEST_WM:
            if (record->event.pressed) {
                gesture_hold  = true;
                gesture_fired = false;
                gesture_x     = 0;
                gesture_y     = 0;
                gesture_mode  = GESTURE_MODE_WINDOW;
            } else {
                gesture_hold  = false;
                gesture_fired = false;
                gesture_x     = 0;
                gesture_y     = 0;
                gesture_mode  = GESTURE_MODE_NONE;
            }
            return false;
        case MAC_MOD:
            mac_mod_hold = record->event.pressed;
            return false;
        case WIN_PREV:
            if (record->event.pressed) {
                tap_code16(S(A(KC_TAB)));
            }
            return false;
        case WIN_NEXT:
            if (record->event.pressed) {
                tap_code16(A(KC_TAB));
            }
            return false;
        case WIN_LEFT:
            if (record->event.pressed) {
                tap_code16(G(KC_LEFT));
            }
            return false;
        case WIN_RIGHT:
            if (record->event.pressed) {
                tap_code16(G(KC_RGHT));
            }
            return false;
        case WIN_MAX:
            if (record->event.pressed) {
                tap_code16(G(KC_UP));
            }
            return false;
        case WIN_MIN:
            if (record->event.pressed) {
                tap_code16(G(KC_DOWN));
            }
            return false;
        case WIN_DESK:
            if (record->event.pressed) {
                tap_code16(G(KC_D));
            }
            return false;
    }
    return true;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (!(gesture_hold && get_highest_layer(layer_state) == GESTURE_LAYER)) {
        return mouse_report;
    }

    // In scroll mode movement may come as h/v; in cursor mode as x/y.
    gesture_x += mouse_report.x + mouse_report.h;
    gesture_y += mouse_report.y + mouse_report.v;

    // While judging a gesture, suppress normal pointer/scroll output.
    mouse_report.x = 0;
    mouse_report.y = 0;
    mouse_report.h = 0;
    mouse_report.v = 0;

    if (!gesture_fired && (iabs16(gesture_x) >= GESTURE_THRESHOLD || iabs16(gesture_y) >= GESTURE_THRESHOLD)) {
        if (gesture_mode == GESTURE_MODE_WINDOW) {
            if (iabs16(gesture_x) > iabs16(gesture_y)) {
                if (gesture_x > 0) {
                    tap_code16(G(KC_RGHT));
                } else {
                    tap_code16(G(KC_LEFT));
                }
            } else {
                if (gesture_y > 0) {
                    tap_code16(G(KC_DOWN));
                } else {
                    tap_code16(G(KC_UP));
                }
            }
        } else {
            if (iabs16(gesture_x) > iabs16(gesture_y)) {
                if (gesture_x > 0) {
                    if (mac_mod_hold) {
                        tap_code16(C(KC_RGHT));
                    } else {
                        tap_code16(A(KC_TAB));
                    }
                } else {
                    if (mac_mod_hold) {
                        tap_code16(C(KC_LEFT));
                    } else {
                        tap_code16(S(A(KC_TAB)));
                    }
                }
            } else {
                if (gesture_y > 0) {
                    if (mac_mod_hold) {
                        tap_code16(C(KC_DOWN));
                    } else {
                        tap_code16(G(KC_DOWN));
                    }
                } else {
                    if (mac_mod_hold) {
                        tap_code16(C(KC_UP));
                    } else {
                        tap_code16(G(KC_UP));
                    }
                }
            }
        }
        gesture_fired = true;
    }

    return mouse_report;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is layer 4 (0-based).
    keyball_set_scroll_mode(get_highest_layer(state) == SCROLL_LAYER);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif

#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
void pointing_device_init_user(void) {
    set_auto_mouse_enable(true);
}
#endif
