/*
 * seanfdnn's ErgoDox EZ layout, forked from Algernon's
 * please see the readme.md file!
 */

#include <stdarg.h>
#include QMK_KEYBOARD_H
#include "led.h"
#include "debug.h"
#include "action_layer.h"
#include "action_util.h"
#include "timer.h"
#include "eeconfig.h"
#include "wait.h"
#include "version.h"

/* Layers */

enum {
  BASE = 0,
  ARRW,
  NMDIA,
};

/* Macros */

enum {
  NONE = 0,
  // Buttons that do extra stuff
  A_MPN,

  // number/symbol keys
  A_1, // 1
  A_2, // 2
  A_3, // ...
  A_4,
  A_5,
  A_6,
  A_7,
  A_8,
  A_9,
  A_0,

  // Fx
  Fx,
};

/* Fn keys */

enum {
  F_BSE = 0,
  F_SFT,
  F_ALT,
  F_CTRL
};

/* Custom keycodes */

enum {
  CT_CLN = 0,
  CT_TA,
  CT_LBP,
  CT_RBP,
  CT_TMUX,
  CT_TPS,
  CT_SR,
};

/* States & timers */


#if KEYLOGGER_ENABLE
# ifdef AUTOLOG_ENABLE
bool log_enable = true;
# else
bool log_enable = false;
# endif
#endif

bool time_travel = false;
bool skip_leds = false;


/* The Keymap */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Keymap 0: Base Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | ~         | 9    | 7  @ | 5  * | 3  ^ | 1  $ | F11  |           |  Fx  | 0  % | 2  ! | 4  # | 6  & | 8    |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * | TAB       |   '  |   ,  |   .  |   P  |   Y  |   (  |           |  )   |   F  |   G  |   C  |   R  |  L   | /         |
 * |-----------+------+------+------+------+------|   [  |           |  ]   |------+------+------+------+------+-----------|
 * | Esc/ARROW |   A  |   O  |   E  |   U  |   I  |------|           |------|   D  |   H  |   T  |   N  |  S   | = / Arrow |
 * |-----------+------+------+------+------+------| tmux |           | tmux |------+------+------+------+------+-----------|
 * | Ctrl      |   \  |   Q  |   J  |   K  |   X  |      |           | Pane |   B  |   M  |   W  |   V  |  Z   | Stop/Reset|
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     |       |      |      |      |   :  |                                       |   -  |      |      |      |       |
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | LAlt |      |           | MDIA | Del  |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | Ctrl |           |      |      |      |
 *                                  |LShift|Bkspc |------|           |------| Enter| Space|
 *                                  |      |      | GUI  |           | MDIA |      |      |
 *                                  `--------------------'           `--------------------'
 */
[BASE] = LAYOUT_ergodox(
// left hand
 KC_GRV             ,M(A_9)      ,M(A_7)      ,M(A_5)  ,M(A_3)  ,M(A_1)  ,KC_F11
,KC_TAB             ,KC_QUOT     ,KC_COMM     ,KC_DOT  ,KC_P    ,KC_Y    ,TD(CT_LBP)
,LT(ARRW,KC_ESC)    ,KC_A        ,KC_O        ,KC_E    ,KC_U    ,KC_I
,F(F_CTRL)          ,KC_BSLS     ,KC_Q        ,KC_J    ,KC_K    ,KC_X    ,TD(CT_TMUX)
,KC_NO              ,KC_NO       ,KC_NO       ,KC_NO   ,TD(CT_CLN)

                                                            ,F(F_ALT), KC_NO
                                                                     ,F(F_CTRL)
                                                    ,F(F_SFT),KC_BSPC,KC_LGUI
                                                                // right hand
                                                               ,M(Fx)     ,M(A_0)  ,M(A_2)    ,M(A_4)  ,M(A_6)  ,M(A_8)   , KC_NO
                                                               ,TD(CT_RBP),KC_F    ,KC_G      ,KC_C    ,KC_R    ,KC_L     ,KC_SLSH
                                                                          ,KC_D    ,KC_H      ,KC_T    ,KC_N    ,KC_S     ,KC_EQL
                                                               ,TD(CT_TPS),KC_B    ,KC_M      ,KC_W    ,KC_V    ,KC_Z     ,TD(CT_SR)
                                                                                   ,KC_MINS   ,KC_NO   ,KC_NO   ,KC_NO    ,KC_NO

                                                               ,OSL(NMDIA),KC_DEL
                                                               , KC_NO
                                                               , LT(NMDIA,KC_ESC)  ,KC_ENT,KC_SPC
    ),


/* Keymap 2: Arrow layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      | Home |  Up  |  End |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |------|           |------|      | Left | Down | Rght |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |      |      |
 *                                  | Enter|      |------|           |------| PgUp | PgDn |
 *                                  |      |      |      |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */

[ARRW] = LAYOUT_ergodox(
// left hand
 KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS

                                             ,KC_TRNS ,KC_TRNS
                                                      ,KC_TRNS
                                    ,KC_ENT  ,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_HOME ,KC_UP   ,KC_END  ,KC_TRNS ,KC_TRNS
                                                                        ,KC_TRNS ,KC_LEFT ,KC_DOWN ,KC_RGHT ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                                 ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                               ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS
                                                               ,KC_TRNS ,KC_PGUP ,KC_PGDN
    ),


/* Keymap 5: Navigation & Media layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |  F9  |  F7  |  F5  |  F3  |  F1  |ScrLCK|           |      | F10  |  F2  |  F4  |  F6  |  F8  |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |  7   |  8   |  9   |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |------|           |------|      |  2   |  5   |  6   |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |  1   |  2   |  3   |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |  0   |  .   |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | Mute | VlUp |           | BASE |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | VlDn |           |      |      |      |
 *                                  |      |      |------|           |------|      |      |
 *                                  |      |      |      |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */
[NMDIA] = LAYOUT_ergodox(
// left hand
 KC_NO      ,KC_F9       ,KC_F7      ,KC_F5   ,KC_F3   ,KC_F1   ,KC_NO
,KC_NO      ,KC_NO       ,KC_NO      ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
,KC_NO      ,KC_NO       ,KC_NO      ,KC_NO   ,KC_NO   ,KC_NO
,KC_NO      ,KC_NO       ,KC_NO      ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
,KC_NO      ,KC_NO       ,KC_NO      ,KC_NO   ,KC_NO
                                                        ,KC_MUTE ,KC_VOLU
                                                                 ,KC_VOLD
                                                 ,KC_NO ,KC_NO   ,KC_TRNS

                                                                     // right hand
                                                                     ,KC_TRNS   ,KC_F10  ,KC_F2   ,KC_F4   ,KC_F6   ,KC_F8    ,KC_NO
                                                                     ,KC_NO     ,KC_NO   ,KC_7    ,KC_8    ,KC_9    ,KC_NO    ,KC_NO
                                                                                ,KC_NO   ,KC_4    ,KC_5    ,KC_6    ,KC_NO    ,KC_NO
                                                                     ,KC_NO     ,KC_NO   ,KC_1    ,KC_2    ,KC_3    ,KC_NO    ,KC_NO
                                                                                         ,KC_0    ,KC_DOT  ,KC_NO   ,KC_NO    ,KC_NO

                                                                     ,KC_TRNS   ,KC_NO
                                                                     ,KC_NO
                                                                     ,KC_NO     ,KC_NO   ,KC_NO
    ),


};

const uint16_t PROGMEM fn_actions[] = {
   [F_BSE]  = ACTION_LAYER_CLEAR(ON_PRESS)
  ,[F_SFT]  = ACTION_MODS_ONESHOT (MOD_LSFT)
  ,[F_ALT]  = ACTION_MODS_ONESHOT (MOD_LALT)
  ,[F_CTRL] = ACTION_MODS_ONESHOT (MOD_LCTL)
};


static void ang_handle_num_row(uint8_t id, keyrecord_t *record) {
  uint8_t idx = id - A_1;
  uint8_t kc;
  static bool shifted[10];

  if (keyboard_report->mods & MOD_BIT(KC_LSFT) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
    if (record->event.pressed)
      shifted[idx] = true;
  }

  if (!shifted[idx]) {
    kc = idx + KC_1;
  } else {
    switch (id) {
    case A_8:
    case A_9:
      shifted[idx] = false;
      return;

    case A_7:
      kc = KC_2;
      break;
    case A_5:
      kc = KC_8;
      break;
    case A_3:
      kc = KC_4;
      break;
    case A_1:
      kc = KC_6;
      break;

    case A_0:
      kc = KC_5;
      break;
    case A_2:
      kc = KC_1;
      break;
    case A_4:
      kc = KC_3;
      break;
    case A_6:
      kc = KC_7;
      break;
    }
  }

  if (record->event.pressed) {
    register_code (kc);
  } else {
    unregister_code (kc);
    shifted[idx] = false;
  }
}

const qk_ucis_symbol_t ucis_symbol_table[] = UCIS_TABLE
(
 UCIS_SYM("poop", 0x1f4a9)
 );

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
      switch(id) {
      case A_MPN:
        if (record->event.pressed) {
          if (keyboard_report->mods & MOD_BIT(KC_LSFT) ||
              ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
            int oneshot = ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out());

            if (oneshot)
              clear_oneshot_mods ();
            unregister_code (KC_LSFT);

            register_code (KC_MPRV);
            unregister_code (KC_MPRV);

            if (!oneshot)
              register_code (KC_LSFT);
          } else {
            return MACRO (T(MNXT), END);
          }
        }
        break;

        /* Fx */
      case Fx:
        if (record->event.pressed) {
          set_oneshot_mods (MOD_LALT);
          layer_on (NMDIA);
          set_oneshot_layer (NMDIA, ONESHOT_START);
        } else {
          clear_oneshot_layer_state (ONESHOT_PRESSED);
        }
        break;

        // number row and symbols
      case A_1 ... A_0:
        ang_handle_num_row(id, record);
        break;
      }

      return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

  set_unicode_input_mode(UC_LNX);

  ergodox_led_all_on();
  for (int i = LED_BRIGHTNESS_HI; i > LED_BRIGHTNESS_LO; i--) {
    ergodox_led_all_set (i);
    wait_ms (5);
  }
  wait_ms(1000);
  for (int i = LED_BRIGHTNESS_LO; i > 0; i--) {
    ergodox_led_all_set (i);
    wait_ms (10);
  }
  ergodox_led_all_off();

  if (!eeconfig_is_enabled())
    eeconfig_init();
};

LEADER_EXTERNS();

static void ang_tap (uint16_t code, ...) {
  uint16_t kc = code;
  va_list ap;

  va_start(ap, code);

  do {
    register_code16(kc);
    unregister_code16(kc);
    wait_ms(50);
    kc = va_arg(ap, int);
  } while (kc != 0);
  va_end(ap);
}

#define TAP_ONCE(code)  \
  register_code (code); \
  unregister_code (code)

typedef struct {
  bool layer_toggle;
  bool sticky;
} td_ta_state_t;

static void ang_tap_dance_ta_finished (qk_tap_dance_state_t *state, void *user_data) {
  td_ta_state_t *td_ta = (td_ta_state_t *) user_data;

  if (td_ta->sticky) {
    td_ta->sticky = false;
    td_ta->layer_toggle = false;
    layer_off (ARRW);
    return;
  }

  if (state->count == 1 && !state->pressed) {
    register_code (KC_ESC);
    td_ta->sticky = false;
    td_ta->layer_toggle = false;
  } else {
    td_ta->layer_toggle = true;
    layer_on (ARRW);
    td_ta->sticky = (state->count == 2);
  }
}

static void ang_tap_dance_ta_reset (qk_tap_dance_state_t *state, void *user_data) {
  td_ta_state_t *td_ta = (td_ta_state_t *) user_data;

  if (!td_ta->layer_toggle)
    unregister_code (KC_ESC);
  if (!td_ta->sticky)
    layer_off (ARRW);
}

static void ang_tap_dance_tmux_finished (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code(KC_LALT);
    register_code(KC_SPC);
    unregister_code(KC_SPC);
    unregister_code(KC_LALT);
  } else {
    register_code(KC_LCTL);
    register_code(KC_A);
    unregister_code(KC_A);
    unregister_code(KC_LCTL);
  }
}

static void ang_tap_dance_tmux_pane_select (qk_tap_dance_state_t *state, void *user_data) {
  uint8_t kc = KC_P;

  if (state->count >= 2) {
    kc = KC_Z;
  }

  register_code(KC_LALT);
  register_code(KC_SPC);
  unregister_code(KC_SPC);
  unregister_code(KC_LALT);

  register_code(kc);
  unregister_code(kc);
}

static void
_td_sr_each (qk_tap_dance_state_t *state, void *user_data) {
  skip_leds = true;

  switch (state->count) {
  case 1:
    ergodox_right_led_3_on ();
    break;
  case 2:
    ergodox_right_led_2_on ();
    break;
  case 3:
    ergodox_right_led_1_on ();
    break;
  case 4:
    ergodox_right_led_3_off ();
    wait_ms (50);
    ergodox_right_led_2_off ();
    wait_ms (50);
    ergodox_right_led_1_off ();
    break;
  }
}

static void
_td_sr_finished (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code (KC_MSTP);
  }
  if (state->count >= 4) {
    uprintf("CMD:reflash\n");
    wait_ms (1000);
    reset_keyboard ();
    reset_tap_dance (state);
  }
}

static void
_td_sr_reset (qk_tap_dance_state_t *state, void *user_data) {
  ergodox_right_led_1_off ();
  wait_ms (50);
  ergodox_right_led_2_off ();
  wait_ms (50);
  ergodox_right_led_3_off ();

  if (state->count == 1) {
    unregister_code (KC_MSTP);
  }
}

static void
_td_brackets_finished (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    if (state->keycode == TD(CT_LBP))
      register_code16 (KC_LBRC);
    else
      register_code16 (KC_RBRC);
  } else if (state->count == 2) {
    if (state->keycode == TD(CT_LBP))
      register_code16 (KC_LPRN);
    else
      register_code16 (KC_RPRN);
  } else if (state->count == 3) {
    unicode_input_start();

    if (state->keycode == TD(CT_LBP))
      register_hex (0x300c);
    else
      register_hex (0x300d);

    unicode_input_finish();
  }
}

static void
_td_brackets_reset (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    if (state->keycode == TD(CT_LBP))
      unregister_code16 (KC_LBRC);
    else
      unregister_code16 (KC_RBRC);
  } else if (state->count == 2) {
    if (state->keycode == TD(CT_LBP))
      unregister_code16 (KC_LPRN);
    else
      unregister_code16 (KC_RPRN);
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
   [CT_CLN] = ACTION_TAP_DANCE_DOUBLE (KC_COLN, KC_SCLN)
  ,[CT_TA]  = {
     .fn = { NULL, ang_tap_dance_ta_finished, ang_tap_dance_ta_reset },
     .user_data = (void *)&((td_ta_state_t) { false, false })
   }
  ,[CT_LBP] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, _td_brackets_finished, _td_brackets_reset)
  ,[CT_RBP] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, _td_brackets_finished, _td_brackets_reset)
  ,[CT_TMUX]= ACTION_TAP_DANCE_FN (ang_tap_dance_tmux_finished)
  ,[CT_TPS] = ACTION_TAP_DANCE_FN (ang_tap_dance_tmux_pane_select)
  ,[CT_SR]  = ACTION_TAP_DANCE_FN_ADVANCED (_td_sr_each, _td_sr_finished, _td_sr_reset)
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  uint8_t layer = biton32(layer_state);
  bool is_arrow = false;

  if (!skip_leds) {
    if (layer == NMDIA) {
      ergodox_right_led_1_on();
      ergodox_right_led_2_on();
  }

  if (layer_state & (1UL << ARRW)) {
    if (!skip_leds) {
      ergodox_right_led_1_on ();
      ergodox_right_led_3_on ();
    }
    is_arrow = true;
  }

  if (!skip_leds) {
    if (keyboard_report->mods & MOD_BIT(KC_LSFT) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
      ergodox_right_led_1_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_1_on ();
    } else {
      ergodox_right_led_1_set (LED_BRIGHTNESS_LO);
      if (layer != NMDIA && !is_arrow)
        ergodox_right_led_1_off ();
    }

    if (keyboard_report->mods & MOD_BIT(KC_LALT) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LALT)) && !has_oneshot_mods_timed_out())) {
      ergodox_right_led_2_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_2_on ();
    } else {
      ergodox_right_led_2_set (LED_BRIGHTNESS_LO);
      if (layer != NMDIA)
        ergodox_right_led_2_off ();
    }

    if (keyboard_report->mods & MOD_BIT(KC_LCTRL) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LCTRL)) && !has_oneshot_mods_timed_out())) {
      ergodox_right_led_3_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_3_on ();
    } else {
      ergodox_right_led_3_set (LED_BRIGHTNESS_LO);
      if (!is_arrow)
        ergodox_right_led_3_off ();
    }
  }
}
}
static uint16_t last4[4];

bool process_record_user (uint16_t keycode, keyrecord_t *record) {
#if KEYLOGGER_ENABLE
  if (log_enable) {
    uint8_t layer = biton32(layer_state);

    if (layer == BASE)
      uprintf ("KL: col=%02d, row=%02d, pressed=%d, layer=%s\n", record->event.key.col,
               record->event.key.row, record->event.pressed, "Dvorak");
  }
#endif

  if (keycode == KC_ESC && record->event.pressed) {
    bool queue = true;

    if ((get_oneshot_mods ()) && !has_oneshot_mods_timed_out ()) {
      clear_oneshot_mods ();
      queue = false;
    }
    return queue;
  }

  if (time_travel && !record->event.pressed) {
    uint8_t p;

    // shift cache one to the left
    for (p = 0; p < 3; p++) {
      last4[p] = last4[p + 1];
    }
    last4[3] = keycode;

    if (last4[0] == KC_D && last4[1] == KC_A && last4[2] == KC_T && last4[3] == KC_E) {
      ang_tap (KC_E, KC_SPC, KC_MINS, KC_D, KC_SPC, KC_QUOT, 0);
      register_code (KC_RSFT);
      register_code (KC_EQL);
      unregister_code (KC_EQL);
      unregister_code (KC_RSFT);

      ang_tap (KC_4, KC_SPC, KC_D, KC_A, KC_Y, KC_S, KC_QUOT, 0);

      return false;
    }
  }

  return true;
}

