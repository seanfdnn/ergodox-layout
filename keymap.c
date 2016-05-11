/*
 * algernon's ErgoDox EZ layout, please see the README.md file!
 */

#include "ergodox_ez.h"
#include "led.h"
#include "debug.h"
#include "action_layer.h"
#include "action_util.h"
#include "mousekey.h"
#include "timer.h"

/* Layers */

#define BASE    0 // default layer
#define HUN     1 // Hungarian layer
#define SYMB    2 // symbols layer
#define NAV     3 // navigation layer

/* Macros */

#define A_SFT      0 // shift toggle
#define A_ALT      1 // alt toggle
#define A_CTRL     2 // control toggle
#define A_GUI      3 // GUI magic

#define A_MUL      4 // mouse up-left
#define A_MUR      5 // mouse up-right
#define A_MDL      6 // mouse down-left
#define A_MDR      7 // mouse down-right

#define A_UNIC     8 // Unicode input key
#define AU_LMBD    9 // λ
#define AU_SHRG   10 // ¯\_(ツ)_/¯

#define AE_VIS    11 // Visual mode
#define AE_PSTDEL 12 // Paste/Delete
#define AE_CPYC   13 // Copy/Cut
#define AE_EMACS  14 // Emacs copy & paste mode
#define AE_TERM   15 // Terminal copy & paste mode
#define AE_OTHER  16 // Other copy & paste mode

#define HU_AA     17 // Á
#define HU_OO     18 // Ó
#define HU_EE     19 // É
#define HU_UU     20 // Ú
#define HU_II     21 // Í
#define HU_OE     22 // Ö
#define HU_UE     23 // Ü
#define HU_OEE    24 // Ő
#define HU_UEE    25 // Ű

#define ASE_META  26 // M-m

/* States & timers */

uint8_t shift_state = 0;
uint8_t alt_state = 0;
uint8_t ctrl_state = 0;

uint8_t m_visual_state = 0;
static uint16_t m_cutdel_timer;
static uint16_t m_copypaste_timer;

uint16_t gui_timer = 0;

enum {
  CP_EMACS = 0,
  CP_TERM  = 1,
  CP_OTHER = 2,
};

uint8_t cp_mode = CP_EMACS;

/* The Keymap */

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
,M(ASE_META)        ,KC_HOME     ,KC_PGUP     ,KC_PGDN,KC_END

                                                            ,M(A_ALT),M(A_GUI)
                                                                     ,M(A_CTRL)
                                                    ,KC_BSPC,M(A_SFT),KC_ESC

                                                                // right hand
                                                               ,KC_APP  ,KC_6   ,KC_7   ,KC_8   ,KC_9        ,KC_0        ,KC_EQL
                                                               ,KC_RBRC ,KC_F   ,KC_G   ,KC_C   ,KC_R        ,KC_L        ,KC_SLSH
                                                                        ,KC_D   ,KC_H   ,KC_T   ,KC_N        ,KC_S        ,KC_BSLS
                                                               ,KC_RPRN ,KC_B   ,KC_M   ,KC_W   ,KC_V        ,KC_Z        ,KC_MSTP
                                                                                ,KC_LEFT,KC_UP  ,KC_DOWN     ,KC_RGHT     ,M(A_UNIC)

                                                               ,KC_FN3  ,KC_FN2
                                                               ,KC_FN4
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

[HUN] = KEYMAP(
// left hand
 KC_NO   ,KC_NO   ,KC_NO    ,KC_NO   ,KC_NO    ,KC_NO   ,KC_TRNS
,KC_NO   ,KC_NO   ,M(HU_OEE),KC_NO   ,M(HU_UEE),KC_NO   ,KC_NO
,KC_NO   ,M(HU_AA),M(HU_OO) ,M(HU_EE),M(HU_UU) ,M(HU_II)
,KC_TRNS ,KC_NO   ,M(HU_OE) ,KC_NO   ,M(HU_UE) ,KC_NO   ,KC_NO
,KC_TRNS ,KC_TRNS ,KC_TRNS  ,KC_TRNS ,KC_TRNS

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
                                                               ,KC_FN1
                                                               ,KC_TRNS ,KC_TRNS  ,KC_TRNS
    ),

/* Keymap 2: Symbol Layer
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
[SYMB] = KEYMAP(
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
                                                                    ,M(AU_LMBD)  ,KC_FN1
                                                                    ,M(AU_SHRG)
                                                                    ,KC_TRNS ,KC_TRNS ,KC_TRNS
    ),

/* Keymap 3: Navigation layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | MS Slow   | F11  | F12  | F13  | F14  | F15  |ScrLCK|           |ScrLCK| F16  | F17  | F18  | F19  | F20  |PrintScreen|
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * | MS Normal |      | Home |  Up  | PgUp |      |Visual|           |Scroll|Vol Up|MsUpL | MsUp |MsUpR |      |           |
 * |-----------+------+------+------+------+------| Mode |           |  Up  |------+------+------+------+------+-----------|
 * | MS Fast   |      | Left | Down | Right|      |------|           |------|Vol Dn|MsLeft| MsDn |MsRght|      |           |
 * |-----------+------+------+------+------+------|Delete|           |Scroll|------+------+------+------+------+-----------|
 * | Play/Pause|      | End  | Down | PgDn |      |Paste |           | Down | Mute |MsDnL | MsDn |MsDnR |      |      Stop |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |EmacsM|TermM |OtherM|      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |  Alt | GUI  |           |UNLOCK| MClk |
 *                                  ,------|------|------|           |------+------+------.
 *                                  | Cut  |      | Ctrl |           | Prev |Left  |Right |
 *                                  |      |LShift|------|           |------| Click| Click|
 *                                  | Copy |      | ESC  |           | Next |      |      |
 *                                  `--------------------'           `--------------------'
 */
[NAV] = KEYMAP(
// left hand
 KC_ACL0    ,KC_F11      ,KC_F12     ,KC_F13  ,KC_F14  ,KC_F15  ,LGUI(KC_L)
,KC_ACL1    ,KC_NO       ,KC_HOME    ,KC_UP   ,KC_PGUP ,KC_NO   ,M(AE_VIS)
,KC_ACL2    ,KC_NO       ,KC_LEFT    ,KC_DOWN ,KC_RIGHT,KC_NO
,KC_MPLY    ,KC_NO       ,KC_END     ,KC_DOWN ,KC_PGDN ,KC_NO   ,M(AE_PSTDEL)
,M(AE_EMACS),M(AE_TERM)  ,M(AE_OTHER),KC_NO   ,KC_NO
                                                      ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS
                                           ,M(AE_CPYC),KC_TRNS ,KC_TRNS
                                                                     // right hand
                                                                     ,LGUI(KC_L),KC_F16  ,KC_F17  ,KC_F18  ,KC_F19  ,KC_F20  ,KC_PSCR
                                                                     ,KC_WH_U   ,KC_VOLU ,M(A_MUL),KC_MS_U ,M(A_MUR),KC_NO   ,KC_NO
                                                                                ,KC_VOLD ,KC_MS_L ,KC_MS_D ,KC_MS_R ,KC_NO   ,KC_NO
                                                                     ,KC_WH_D   ,KC_MUTE ,M(A_MDL),KC_MS_D ,M(A_MDR),KC_NO   ,KC_MSTP
                                                                                         ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                                     ,KC_FN1    ,KC_MS_BTN3
                                                                     ,KC_MPRV
                                                                     ,KC_MNXT   ,KC_BTN1 ,KC_BTN2
    )

};

const uint16_t PROGMEM fn_actions[] = {
     [1] = ACTION_LAYER_CLEAR(ON_PRESS)           // FN1 - clear to base layer
    ,[2] = ACTION_LAYER_INVERT(SYMB, ON_PRESS) // FN2 - toggle to Symbols on press
    ,[3] = ACTION_LAYER_INVERT(NAV, ON_PRESS) // FN3 - toggle to Media on press
    ,[4] = ACTION_LAYER_INVERT(HUN, ON_PRESS)  // FN4 - toggle to Hungarian on press
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
      switch(id) {
      case ASE_META:
        if (record->event.pressed) {
          return MACRO (D(LALT), T(M), U(LALT), END);
        }
        break;

      case A_SFT:
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

      case A_ALT:
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

      case A_CTRL:
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

      case HU_AA:
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
      case HU_OO:
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
      case HU_EE:
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
      case HU_UU:
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
      case HU_II:
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
      case HU_OE:
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
      case HU_UE:
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
      case HU_OEE:
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
      case HU_UEE:
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

      case A_MUL:
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

      case A_MUR:
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

      case A_MDL:
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

      case A_MDR:
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

      case A_UNIC:
        if (record->event.pressed) {
          return MACRO(D(RCTL), D(RSFT), T(U), U(RSFT), U(RCTL), END);
        }
        break;

      case AU_SHRG:
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

      case AU_LMBD:
        if (record->event.pressed) {
          return MACRO(D(RCTL), D(RSFT), T(U), U(RSFT), U(RCTL), T(0), T(3), T(B), T(B), T(SPC), END);
        }
        break;

      case AE_EMACS:
        if (record->event.pressed) {
          cp_mode = CP_EMACS;
        }
        break;

      case AE_TERM:
        if (record->event.pressed) {
          cp_mode = CP_TERM;
        }
        break;

      case AE_OTHER:
        if (record->event.pressed) {
          cp_mode = CP_OTHER;
        }
        break;

      case AE_VIS:
        if (cp_mode == CP_EMACS && record->event.pressed) {
          return MACRO(T(V), END);
        }
        break;

      case AE_CPYC:
        if (record->event.pressed) {
          m_copypaste_timer = timer_read ();
        } else {
          if (timer_elapsed (m_copypaste_timer) > TAPPING_TERM) {
            // Long press: Cut
            switch (cp_mode) {
            case CP_EMACS:
              return MACRO(T(X), END);
              break;
            case CP_TERM:
              return MACRO(D(RCTRL), D(RSFT), T(X), U(RSFT), U(RCTRL), END);
              break;
            case CP_OTHER:
              return MACRO(D(RCTRL), T(X), U(RCTRL), END);
              break;
            }
          } else {
            // Short press: Copy
            switch (cp_mode) {
            case CP_EMACS:
              return MACRO(T(Y), END);
              break;
            case CP_TERM:
              return MACRO(D(RCTRL), D(RSFT), T(C), U(RSFT), U(RCTRL), END);
              break;
            case CP_OTHER:
              return MACRO(D(RCTRL), T(C), U(RCTRL), END);
              break;
            }
          }
        }
        break;

      case AE_PSTDEL:
        if (record->event.pressed) {
          m_cutdel_timer = timer_read ();
        } else {
          if (timer_elapsed (m_cutdel_timer) > TAPPING_TERM) {
            // Long press: Delete
            switch (cp_mode) {
            case CP_EMACS:
              return MACRO(T(D), END);
              break;
            case CP_TERM:
            case CP_OTHER:
              return MACRO(T(DEL), END);
              break;
            }
          } else {
            // Short press: Paste
            switch (cp_mode) {
            case CP_EMACS:
              return MACRO(T(P), END);
              break;
            case CP_TERM:
              return MACRO(D(RCTRL), D(RSFT), T(V), U(RSFT), U(RCTRL), END);
              break;
            case CP_OTHER:
              return MACRO(D(RCTRL), T(V), U(RCTRL), END);
              break;
            }
          }
        }

      case A_GUI:
        if (record->event.pressed) {
          register_mods(MOD_BIT(KC_LGUI));
          gui_timer = timer_read();
        } else {
          if (timer_elapsed (gui_timer) > TAPPING_TERM) {
            unregister_mods(MOD_BIT(KC_LGUI));
          } else {
            return MACRO(T(W), U(LGUI), END);
          }
        }
        break;
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

    if (layer == HUN) {
      ergodox_right_led_2_on();
      ergodox_right_led_3_on();
    } else if (layer == SYMB) {
      ergodox_right_led_1_on();
      ergodox_right_led_3_on();
    } else if (layer == NAV) {
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
