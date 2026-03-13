/*
This is the c configuration file for the keymap

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

#pragma once

#ifdef RGBLIGHT_ENABLE
//#    define RGBLIGHT_EFFECT_BREATHING
//#    define RGBLIGHT_EFFECT_RAINBOW_MOOD
//#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
//#    define RGBLIGHT_EFFECT_SNAKE
//#    define RGBLIGHT_EFFECT_KNIGHT
//#    define RGBLIGHT_EFFECT_CHRISTMAS
//#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
//#    define RGBLIGHT_EFFECT_RGB_TEST
//#    define RGBLIGHT_EFFECT_ALTERNATING
//#    define RGBLIGHT_EFFECT_TWINKLE
#endif

#define TAP_CODE_DELAY 5
#define DYNAMIC_KEYMAP_LAYER_COUNT 7

// Lower scroll thresholds to reduce "dead zone" feeling while KC_MY_SCR is held.
#define TAKASHI_SCROLL_V_THRESHOLD 2
#define TAKASHI_SCROLL_H_THRESHOLD 2

// Runtime tuning for KC_MY_SCR scroll threshold via KB(16)/KB(17).
#define TAKASHI_SCROLL_THRESHOLD_STEP 1
#define TAKASHI_SCROLL_THRESHOLD_MIN 1
#define TAKASHI_SCROLL_THRESHOLD_MAX 50

// Scroll output divisor: threshold crossings needed to emit 1 scroll unit.
// Increase to slow down scroll output while keeping threshold sensitivity.
#define TAKASHI_SCROLL_OUTPUT_DIVISOR 5

// Gesture: minimum trackball movement (accumulated x or y) to register a direction.
#define GESTURE_THRESHOLD 30
