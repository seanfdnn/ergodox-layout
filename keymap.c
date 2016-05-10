#include "ergodox_ez.h"
#include "led.h"
#include "debug.h"
#include "action_layer.h"
#include "action_util.h"
#include "mousekey.h"
#include "timer.h"

#define BASE     0 // default layer
#define HUN_LK   1 // Hungarian base layer
#define SYMB_LK  2 // symbols lock layer
#define MDIA_LK  3 // media lock layer

#define HU_AA M(10)
#define HU_OO M(11)
#define HU_EE M(12)
#define HU_UU M(13)
#define HU_II M(14)
#define HU_OE M(15)
#define HU_UE M(16)
#define HU_OEE M(17)
#define HU_UEE M(18)

#define C_AA  10
#define C_OO  11
#define C_EE  12
#define C_UU  13
#define C_II  14
#define C_OE  15
#define C_UE  16
#define C_OEE 17
#define C_UEE 18

#define AM_LSFT M(5)
#define AM_RSFT M(6)
#define AM_CAPS M(5)

#define AM_LALT M(7)
#define AM_RALT M(7)

#define AM_LCTRL M(8)
#define AM_RCTRL M(8)

#define MUL 20
#define MUR 21
#define MDL 22
#define MDR 23

#define MUNICODE 24
#define MLAMBDA 25
#define MSHRUGGIE 26

#define MVISUAL 27
#define MCUTDEL 28
#define MCOPYPASTE 29
#define MCPYP MCOPYPASTE

/*
 * algernon's ErgoDox EZ layout.
 */

uint8_t shift_state = 0;
uint8_t alt_state = 0;
uint8_t ctrl_state = 0;

uint8_t m_visual_state = 0;
static uint16_t m_cutdel_timer;
static uint16_t m_copypaste_timer;

#define PASTE_DELAY 150

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Keymap 0: Base Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |        `~ |   1  |   2  |   3  |   4  |   5  | Apps |           | Apps |   6  |   7  |   8  |   9  |   0  | =         |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |       Tab |   '  |   ,  |   .  |   P  |   Y  |   [  |           |  ]   |   F  |   G  |   C  |   R  |  L   | /         |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |         - |   A  |   O  |   E  |   U  |   I  |------|           |------|   D  |   H  |   T  |   N  |  S   | \         |
 * |-----------+------+------+------+------+------|   (  |           |  )   |------+------+------+------+------+-----------|
 * | Play/Pause|   ;  |   Q  |   J  |   K  |   X  |      |           |      |   B  |   M  |   W  |   V  |  Z   |      Stop |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     |  M-m  | Home | PgUp | PgDn | End  |                                       | Left |  Up  | Down | Right|Unicode|
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | LAlt | GUI  |           | Media| SYM  |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | Ctrl |           |  HUN |      |      |
 *                                  |Backsp|LShift|------|           |------| Enter| Space|
 *                                  |      |      | ESC  |           |  ESC |      |      |
 *                                  `--------------------'           `--------------------'
 */
[BASE] = KEYMAP(
// left hand
 KC_GRV             ,KC_1        ,KC_2        ,KC_3   ,KC_4  ,KC_5  ,KC_APP
,KC_TAB             ,KC_QUOT     ,KC_COMM     ,KC_DOT ,KC_P  ,KC_Y  ,KC_LBRC
,KC_MINS            ,KC_A        ,KC_O        ,KC_E   ,KC_U  ,KC_I
,KC_MPLY            ,KC_SCLN     ,KC_Q        ,KC_J   ,KC_K  ,KC_X  ,KC_LPRN
,M(1)               ,KC_HOME     ,KC_PGUP     ,KC_PGDN,KC_END

                                                            ,AM_LALT,KC_LGUI
                                                                    ,AM_LCTRL
                                                    ,KC_BSPC,AM_LSFT,KC_ESC

                                                                // right hand
                                                               ,KC_APP  ,KC_6   ,KC_7   ,KC_8   ,KC_9        ,KC_0        ,KC_EQL
                                                               ,KC_RBRC ,KC_F   ,KC_G   ,KC_C   ,KC_R        ,KC_L        ,KC_SLSH
                                                                        ,KC_D   ,KC_H   ,KC_T   ,KC_N        ,KC_S        ,KC_BSLS
                                                               ,KC_RPRN ,KC_B   ,KC_M   ,KC_W   ,KC_V        ,KC_Z        ,KC_MSTP
                                                                                ,KC_LEFT,KC_UP  ,KC_DOWN     ,KC_RGHT     ,M(MUNICODE)

                                                               ,KC_FN3  ,KC_FN2
                                                               ,OSL(HUN_LK)
                                                               ,KC_ESC  ,KC_ENT ,KC_SPC
    ),

/* Keymap 1: Hungarian Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |  Ő   |      |  Ű   |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |   Á  |  Ó   |  É   |  Ú   |  Í   |------|           |------|      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |  Ö   |      |  Ü   |      |      |           |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |UNLOCK|      |      |
 *                                  |      |      |------|           |------|      |      |
 *                                  |      |      |      |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */

[HUN_LK] = KEYMAP(
// left hand
 KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_TRNS
,KC_NO   ,KC_NO   ,HU_OEE  ,KC_NO   ,HU_UEE  ,KC_NO   ,KC_NO
,KC_NO   ,HU_AA   ,HU_OO   ,HU_EE   ,HU_UU   ,HU_II
,KC_TRNS ,KC_NO   ,HU_OE   ,KC_NO   ,HU_UE   ,KC_NO   ,KC_NO
,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                             ,KC_TRNS ,KC_TRNS
                                                      ,KC_TRNS
                                    ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                               ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                                        ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                               ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                                                 ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                               ,KC_NO   ,KC_NO
                                                               ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS  ,KC_TRNS
    ),

/* Keymap 2: Symbol Lock Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |     F1    |   1  |   2  |   3  |   4  |   5  |      |           |      |   6  |   7  |   8  |   9  |   0  |    F6     |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |     F2    |   !  |   @  |   {  |   }  |   &  |  <   |           |  >   |   |  |   4  |   5  |   6  |   *  |    F7     |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |     F3    |   #  |   $  |   (  |   )  |   `  |------|           |------|   /  |   1  |   2  |   3  |   -  |    F8     |
 * |-----------+------+------+------+------+------| Tab  |           |  Tab |------+------+------+------+------+-----------|
 * |     F4    |   %  |   ^  |   [  |   ]  |   ~  |      |           |      |   \  |   0  |   .  |   =  |   +  |    F9     |
 * `-----------+------+------+------+------+-------------'           `------------+------+------+------+------+------------'
 *      |  F5  |      |      |      |      |                                       |      |      |      |      |  F10 |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |  λ   |UNLOCK|
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           | SHRUG|      |      |
 *                                  |      |      |------|           |------|      |      |
 *                                  |      |      |      |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */
[SYMB_LK] = KEYMAP(
// left hand
 KC_F1       ,KC_1    ,KC_2    ,KC_3    ,KC_4    ,KC_5    ,KC_TRNS
,KC_F2       ,KC_EXLM ,KC_AT   ,KC_LCBR ,KC_RCBR ,KC_AMPR ,LSFT(KC_COMM)
,KC_F3       ,KC_HASH ,KC_DLR  ,KC_LPRN ,KC_RPRN ,KC_GRV
,KC_F4       ,KC_PERC ,KC_CIRC ,KC_LBRC ,KC_RBRC ,KC_TILD ,KC_TAB
,KC_F5       ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                 ,KC_TRNS ,KC_TRNS
                                                          ,KC_TRNS
                                        ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                    // right hand
                                                                    ,KC_TRNS     ,KC_6    ,KC_7   ,KC_8   ,KC_9   ,KC_0    ,KC_F6
                                                                    ,LSFT(KC_DOT),KC_PIPE ,KC_4   ,KC_5   ,KC_6   ,KC_ASTR ,KC_F7
                                                                                 ,KC_SLSH ,KC_1   ,KC_2   ,KC_3   ,KC_MINS ,KC_F8
                                                                    ,KC_TAB      ,KC_BSLS ,KC_0   ,KC_DOT ,KC_EQL ,KC_PLUS ,KC_F9
                                                                                          ,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS ,KC_F10
                                                                    ,M(MLAMBDA)  ,KC_FN1
                                                                    ,M(MSHRUGGIE)
                                                                    ,KC_TRNS ,KC_TRNS ,KC_TRNS
    ),

/* Keymap 3: Media Lock Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |PrintScreen| F11  | F12  | F13  | F14  | F15  |ScrLCK|           |ScrLCK| F16  | F17  | F18  | F19  | F20  |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      | Home |  Up  | PgUp |      |Visual|           |Scroll|Vol Up|MsUpL | MsUp |MsUpR |      |           |
 * |-----------+------+------+------+------+------| Mode |           |  Up  |------+------+------+------+------+-----------|
 * |           |      | Left | Down | Right|      |------|           |------|Vol Dn|MsLeft| MsDn |MsRght|      |           |
 * |-----------+------+------+------+------+------| Cut  |           |Scroll|------+------+------+------+------+-----------|
 * | Play/Pause|      | End  | Down | PgDn |      |Delete|           | Down | Mute |MsDnL | MsDn |MsDnR |      |      Stop |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |  Alt | GUI  |           |UNLOCK| MClk |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |Paste |      | Ctrl |           | Prev |Left  |Right |
 *                                  |      |LShift|------|           |------| Click| Click|
 *                                  | Copy |      | ESC  |           | Next |      |      |
 *                                  `--------------------'           `--------------------'
 */
[MDIA_LK] = KEYMAP(
// left hand
 KC_PSCR    ,KC_F11      ,KC_F12  ,KC_F13  ,KC_F14  ,KC_F15  ,LGUI(KC_L)
,KC_NO      ,KC_NO       ,KC_HOME ,KC_UP   ,KC_PGUP ,KC_NO   ,M(MVISUAL)
,KC_NO      ,KC_NO       ,KC_LEFT ,KC_DOWN ,KC_RIGHT,KC_NO
,KC_MPLY    ,KC_NO       ,KC_END  ,KC_DOWN ,KC_PGDN ,KC_NO   ,M(MCUTDEL)
,KC_NO      ,KC_NO       ,KC_NO   ,KC_NO   ,KC_NO
                                                    ,KC_TRNS ,KC_TRNS
                                                             ,KC_TRNS
                                           ,M(MCPYP),KC_TRNS ,KC_TRNS
                                                                     // right hand
                                                                     ,LGUI(KC_L),KC_F16  ,KC_F17 ,KC_F18  ,KC_F19  ,KC_F20  ,KC_NO
                                                                     ,KC_WH_U   ,KC_VOLU ,M(MUL) ,KC_MS_U ,M(MUR)  ,KC_NO   ,KC_NO
                                                                                ,KC_VOLD ,KC_MS_L,KC_MS_D ,KC_MS_R ,KC_NO   ,KC_NO
                                                                     ,KC_WH_D   ,KC_MUTE ,M(MDL) ,KC_MS_D ,M(MDR)  ,KC_NO   ,KC_MSTP
                                                                                         ,KC_NO  ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                                     ,KC_FN1    ,KC_MS_BTN3
                                                                     ,KC_MPRV
                                                                     ,KC_MNXT   ,KC_BTN1 ,KC_BTN2
    )

};

const uint16_t PROGMEM fn_actions[] = {
     [1] = ACTION_LAYER_CLEAR(ON_PRESS)           // FN1 - clear to base layer
    ,[2] = ACTION_LAYER_INVERT(SYMB_LK, ON_PRESS) // FN2 - toggle to Symbols on press
    ,[3] = ACTION_LAYER_INVERT(MDIA_LK, ON_PRESS) // FN3 - toggle to Media on press
    ,[4] = ACTION_LAYER_INVERT(HUN_LK, ON_PRESS)  // FN4 - toggle to Hungarian on press
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
        if (record->event.pressed) {
          register_code(KC_RSFT);
        } else {
          unregister_code(KC_RSFT);
        }
        break;

      case 1:
        if (record->event.pressed) {
          return MACRO (D(LALT), T(M), U(LALT), END);
        }
        break;

      case 5:
      case 6:
        if (record->event.pressed) {
          if (shift_state == 0) {
            register_code(KC_RSFT);
            shift_state = 1;
            ergodox_right_led_1_set(LED_BRIGHTNESS_HI);
            ergodox_right_led_1_on();
          } else {
            unregister_code(KC_RSFT);
            unregister_code(KC_CAPS);
            ergodox_right_led_1_set(LED_BRIGHTNESS_LO);
            ergodox_right_led_1_off();
            shift_state = 0;
          }
        }
        break;

      case 7:
        if (record->event.pressed) {
          if (alt_state == 0) {
            register_code (KC_LALT);
            alt_state = 1;
            ergodox_right_led_2_set(LED_BRIGHTNESS_HI);
            ergodox_right_led_2_on();
          } else {
            unregister_code (KC_LALT);
            ergodox_right_led_2_set(LED_BRIGHTNESS_LO);
            ergodox_right_led_2_off();
            alt_state = 0;
          }
        }
        break;

      case 8:
        if (record->event.pressed) {
          if (ctrl_state == 0) {
            register_code (KC_LCTRL);
            ctrl_state = 1;
            ergodox_right_led_3_set(LED_BRIGHTNESS_HI);
            ergodox_right_led_3_on();
          } else {
            unregister_code (KC_LCTRL);
            ergodox_right_led_3_set(LED_BRIGHTNESS_LO);
            ergodox_right_led_3_off();
            ctrl_state = 0;
          }
        }
        break;

      case C_AA:
        if (record->event.pressed) {
          if (shift_state == 0) {
            unregister_code(KC_RSFT);
            return MACRO (U(RSFT), D(RALT), U(RALT), T(QUOT), T(A), END);
          }
          else
            {
              if (keyboard_report->mods & MOD_BIT(KC_RSFT))
                unregister_code (KC_RSFT);
              return MACRO (U(RSFT), D(RALT), U(RALT), T(QUOT), D(RSFT), T(A), END);
            }
        }
        break;
      case C_OO:
        if (record->event.pressed) {
          if (shift_state == 0) {
            unregister_code(KC_RSFT);
            return MACRO (U(RSFT), D(RALT), U(RALT), T(QUOT), T(O), END);
          }
          else
            {
              if (keyboard_report->mods & MOD_BIT(KC_RSFT))
                unregister_code (KC_RSFT);
              return MACRO (U(RSFT), D(RALT), U(RALT), T(QUOT), D(RSFT), T(O), END);
            }
        }
        break;
      case C_EE:
        if (record->event.pressed) {
          if (shift_state == 0) {
            unregister_code(KC_RSFT);
            return MACRO (U(RSFT), D(RALT), U(RALT), T(QUOT), T(E), END);
          }
          else
            {
              if (keyboard_report->mods & MOD_BIT(KC_RSFT))
                unregister_code (KC_RSFT);
              return MACRO (U(RSFT), D(RALT), U(RALT), T(QUOT), D(RSFT), T(E), END);
            }
        }
        break;
      case C_UU:
        if (record->event.pressed) {
          if (shift_state == 0) {
            unregister_code(KC_RSFT);
            return MACRO (U(RSFT), D(RALT), U(RALT), T(QUOT), T(U), END);
          }
          else
            {
              if (keyboard_report->mods & MOD_BIT(KC_RSFT))
                unregister_code (KC_RSFT);
              return MACRO (U(RSFT), D(RALT), U(RALT), T(QUOT), D(RSFT), T(U), END);
            }
        }
        break;
      case C_II:
        if (record->event.pressed) {
          if (shift_state == 0) {
            unregister_code(KC_RSFT);
            return MACRO (U(RSFT), D(RALT), U(RALT), T(QUOT), T(I), END);
          }
          else
            {
              if (keyboard_report->mods & MOD_BIT(KC_RSFT))
                unregister_code (KC_RSFT);
              return MACRO (U(RSFT), D(RALT), U(RALT), T(QUOT), D(RSFT), T(I), END);
            }
        }
        break;
      case C_OE:
        if (record->event.pressed) {
          if (shift_state == 0) {
            unregister_code(KC_RSFT);
            return MACRO (U(RSFT), D(RALT), U(RALT), D(LSFT), T(QUOT), U(LSFT), T(O), END);
          }
          else
            {
              return MACRO (D(RALT), U(RALT), T(QUOT), T(O), END);
            }
        }
        break;
      case C_UE:
        if (record->event.pressed) {
          if (shift_state == 0) {
            unregister_code(KC_RSFT);
            return MACRO (U(RSFT), D(RALT), U(RALT), D(LSFT), T(QUOT), U(LSFT), T(U), END);
          }
          else
            {
              return MACRO (D(RALT), U(RALT), T(QUOT), T(U), END);
            }
        }
        break;
      case C_OEE:
        if (record->event.pressed) {
          if (shift_state == 0) {
            unregister_code(KC_RSFT);
            return MACRO (U(RSFT), D(RALT), U(RALT), T(EQL), T(O), END);
          }
          else
            {
              if (keyboard_report->mods & MOD_BIT(KC_RSFT))
                unregister_code (KC_RSFT);
              return MACRO (U(RSFT), D(RALT), U(RALT), T(EQL), D(RSFT), T(O), END);
            }
        }
        break;
      case C_UEE:
        if (record->event.pressed) {
          if (shift_state == 0) {
            unregister_code(KC_RSFT);
            return MACRO (U(RSFT), D(RALT), U(RALT), T(EQL), T(U), END);
          }
          else
            {
              if (keyboard_report->mods & MOD_BIT(KC_RSFT))
                unregister_code (KC_RSFT);
              return MACRO (U(RSFT), D(RALT), U(RALT), T(EQL), D(RSFT), T(U), END);
            }
        }
        break;

      case MUL: // mouse up left
        if (record->event.pressed) {
          mousekey_on(KC_MS_UP);
          mousekey_on(KC_MS_LEFT);
          mousekey_send();
        } else {
          mousekey_off(KC_MS_UP);
          mousekey_off(KC_MS_LEFT);
          mousekey_send();
        }
        break;

      case MUR: // mouse up right
        if (record->event.pressed) {
          mousekey_on(KC_MS_UP);
          mousekey_on(KC_MS_RIGHT);
          mousekey_send();
        } else {
          mousekey_off(KC_MS_UP);
          mousekey_off(KC_MS_RIGHT);
          mousekey_send();
        }
        break;

      case MDL: // mouse down left
        if (record->event.pressed) {
          mousekey_on(KC_MS_DOWN);
          mousekey_on(KC_MS_LEFT);
          mousekey_send();
        } else {
          mousekey_off(KC_MS_DOWN);
          mousekey_off(KC_MS_LEFT);
          mousekey_send();
        }
        break;

      case MDR: // mouse down right
        if (record->event.pressed) {
          mousekey_on(KC_MS_DOWN);
          mousekey_on(KC_MS_RIGHT);
          mousekey_send();
        } else {
          mousekey_off(KC_MS_DOWN);
          mousekey_off(KC_MS_RIGHT);
          mousekey_send();
        }
        break;

      case MUNICODE:
        if (record->event.pressed) {
          return MACRO(D(RCTL), D(RSFT), T(U), U(RSFT), U(RCTL), END);
        }
        break;

      case MSHRUGGIE:
        // ¯\_(ツ)_/¯
        if (record->event.pressed) {
          return MACRO(D(RCTL), D(RSFT), T(U), U(RSFT), U(RCTL), T(A), T(F), T(SPC),
                       T(BSLS),
                       D(RSFT), T(MINS), T(9), U(RSFT),
                       D(RCTL), D(RSFT), T(U), U(RSFT), U(RCTL), T(3), T(0), T(C), T(4), T(SPC),
                       D(RSFT), T(0), T(MINS), U(RSFT),
                       T(SLSH),
                       D(RCTL), D(RSFT), T(U), U(RSFT), U(RCTL), T(A), T(F), T(SPC),
                       END);
        }
        break;

      case MLAMBDA:
        if (record->event.pressed) {
          return MACRO(D(RCTL), D(RSFT), T(U), U(RSFT), U(RCTL), T(0), T(3), T(B), T(B), T(SPC), END);
        }
        break;

      case MVISUAL:
        if (record->event.pressed) {
          return MACRO(T(V), END);
        }
        break;

      case MCOPYPASTE:
        if (record->event.pressed) {
          m_copypaste_timer = timer_read ();
        } else {
          if (timer_elapsed (m_copypaste_timer) > PASTE_DELAY) {
            return MACRO(T(P), END);
          } else {
            return MACRO(T(Y), END);
          }
        }
        break;

      case MCUTDEL:
        if (record->event.pressed) {
          m_cutdel_timer = timer_read ();
        } else {
          if (timer_elapsed (m_cutdel_timer) > PASTE_DELAY) {
            return MACRO(T(D), END);
          } else {
            return MACRO(T(X), END);
          }
        }
      }
      return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  ergodox_led_all_set (LED_BRIGHTNESS_LO);
  ergodox_led_all_on();
  _delay_ms(1000);
  ergodox_led_all_off();
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
    uint8_t layer = biton32(layer_state);

    if ((shift_state == 1) && !(keyboard_report->mods & MOD_BIT(KC_RSFT)))
      register_code (KC_RSFT);

    if (layer == HUN_LK) {
      ergodox_right_led_2_on();
      ergodox_right_led_3_on();
    } else if (layer == SYMB_LK) {
      ergodox_right_led_1_on();
      ergodox_right_led_3_on();
    } else if (layer == MDIA_LK) {
      ergodox_right_led_1_on();
      ergodox_right_led_2_on();
    } else {
      if (shift_state == 0)
        ergodox_right_led_1_off();
      if (alt_state == 0)
        ergodox_right_led_2_off();
      if (ctrl_state == 0)
        ergodox_right_led_3_off();
    }
};
