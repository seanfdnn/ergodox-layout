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
#define APPSEL  1 // application select layer
#define HUN     2 // Hungarian layer
#define EMACS   3 // (Spac)Emacs layer
#define OHLFT   4 // One-handed, left side
#define OHRGT   5 // One-handed, right side

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
#define AE_INS    32 // Insert mode
#define AE_OVR    33 // Overwrite mode
#define AE_APPND  34 // Append

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

#define APP_SLK   27 // Slack
#define APP_EMCS  28 // Emacs
#define APP_TERM  29 // Terminal
#define APP_CHRM  30 // Chrome
#define APP_MSIC  31 // Music

#define KF_1      35 // 1, F1
#define KF_2      36 // 2, F2
#define KF_3      37 // ...
#define KF_4      38
#define KF_5      39
#define KF_6      40
#define KF_7      41
#define KF_8      42
#define KF_9      43
#define KF_10     44
#define KF_11     45 // =, F11

#define OH_BSSPC  46
#define OH_ENTSFT 47
#define OH_BASE   48
#define OH_LEFT   49
#define OH_RIGHT  50

/* Fn keys */
#define F_BSE     0
#define F_ECS     1
#define F_HUN     2
#define F_GUI     3

/* States & timers */

uint8_t shift_state = 0;
uint8_t alt_state = 0;
uint8_t ctrl_state = 0;

uint8_t m_visual_state = 0;
static uint16_t m_cutdel_timer;
static uint16_t m_copypaste_timer;

uint16_t gui_timer = 0;

uint16_t kf_timers[12];

uint16_t oh_base_timer = 0;
uint16_t oh_bsspc_timer = 0;
uint16_t oh_entsft_timer = 0;

#define OH_BLINK_INTERVAL 500

uint8_t oh_left_blink = 0;
uint16_t oh_left_blink_timer = 0;
uint8_t oh_right_blink = 0;
uint16_t oh_right_blink_timer = 0;

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
 * |        `~ | 1 F1 | 2 F2 | 3 F3 | 4 F4 | 5 F5 | Apps |           | Apps | 6 F6 | 7 F7 | 8 F8 | 9 F9 | 0 F10| =     F11 |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |       Tab |   '  |   ,  |   .  |   P  |   Y  |   [  |           |  ]   |   F  |   G  |   C  |   R  |  L   | /         |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |         - |   A  |   O  |   E  |   U  |   I  |------|           |------|   D  |   H  |   T  |   N  |  S   | \         |
 * |-----------+------+------+------+------+------|   (  |           |  )   |------+------+------+------+------+-----------|
 * | Play/Pause|   ;  |   Q  |   J  |   K  |   X  |      |           |      |   B  |   M  |   W  |   V  |  Z   |      Stop |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     |  Home | PgUp | PgDn | End  |   :  |                                       | Left |  Up  | Down | Right|Unicode|
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | LAlt | GUI  |           |EMACS | 1HND |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | Ctrl |           | HUN  |      |      |
 *                                  |Backsp|LShift|------|           |------| Enter| Space|
 *                                  |      |      | ESC  |           | ESC  |      |      |
 *                                  `--------------------'           `--------------------'
 */
[BASE] = KEYMAP(
// left hand
 KC_GRV             ,M(KF_1)     ,M(KF_2)     ,M(KF_3),M(KF_4),M(KF_5),KC_APP
,KC_TAB             ,KC_QUOT     ,KC_COMM     ,KC_DOT ,KC_P   ,KC_Y   ,KC_LBRC
,KC_MINS            ,KC_A        ,KC_O        ,KC_E   ,KC_U   ,KC_I
,KC_MPLY            ,KC_SCLN     ,KC_Q        ,KC_J   ,KC_K   ,KC_X   ,KC_LPRN
,KC_HOME            ,KC_PGUP     ,KC_PGDN     ,KC_END ,KC_COLN

                                                            ,M(A_ALT),F(F_GUI)
                                                                     ,M(A_CTRL)
                                                    ,KC_BSPC,M(A_SFT),KC_ESC

                                                                // right hand
                                                               ,KC_APP  ,M(KF_6),M(KF_7),M(KF_8),M(KF_9)     ,M(KF_10)    ,M(KF_11)
                                                               ,KC_RBRC ,KC_F   ,KC_G   ,KC_C   ,KC_R        ,KC_L        ,KC_SLSH
                                                                        ,KC_D   ,KC_H   ,KC_T   ,KC_N        ,KC_S        ,KC_BSLS
                                                               ,KC_RPRN ,KC_B   ,KC_M   ,KC_W   ,KC_V        ,KC_Z        ,KC_MSTP
                                                                                ,KC_LEFT,KC_UP  ,KC_DOWN     ,KC_RGHT     ,M(A_UNIC)

                                                               ,F(F_ECS),M(OH_LEFT)
                                                               ,F(F_HUN)
                                                               ,KC_ESC  ,KC_ENT ,KC_SPC
    ),

/* Keymap 1: Application select layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |Music |Slack |Emacs |Term  |Chrome|      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |------|           |------|      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |      |      |
 *                                  |      |      |------|           |------|      |      |
 *                                  |      |      |      |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */

[APPSEL] = KEYMAP(
// left hand
 KC_TRNS ,M(APP_MSIC),M(APP_SLK),M(APP_EMCS),M(APP_TERM),M(APP_CHRM),KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS

                                             ,KC_TRNS ,KC_TRNS
                                                      ,KC_TRNS
                                    ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                        ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                                 ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                               ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS  ,KC_TRNS
    ),


/* Keymap 2: Hungarian Layer
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
 *                                  |      |      |      |           | BASE |      |      |
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
                                                               ,F(F_BSE)
                                                               ,KC_TRNS ,KC_TRNS  ,KC_TRNS
    ),

/* Keymap 3: Spacemacs layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | MS Slow   | 1 F1 | 2 F2 | 3 F3 | 4 F4 | 5 F5 |ScrLCK|           |ScrLCK| 6 F6 | 7 F7 | 8 F8 | 9 F9 | 0 F10|    F11    |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * | MS Normal |      | Home |  Up  | End  |      |Visual|           |Scroll|  $   |MsUpL | MsUp |MsUpR |  R   |PrintScreen|
 * |-----------+------+------+------+------+------| Mode |           |  Up  |------+------+------+------+------+-----------|
 * | MS Fast   |APPEND| Left | Down | Right| INS  |------|           |------|  D   |MsLeft| MsDn |MsRght|      |           |
 * |-----------+------+------+------+------+------| Cut  |           |Scroll|------+------+------+------+------+-----------|
 * | Play/Pause|      | PgUp | Down | PgDn |  X   | Copy |           | Down |      |MsDnL |  W   |MsDnR |      |      Stop |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |EmacsM|TermM |OtherM|      |      |                                       |Vol Up|Vol Dn| Mute |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |  Alt | GUI  |           | BASE | MClk |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |Delete|      | Ctrl |           | Prev |Left  |Right |
 *                                  |      |LShift|------|           |------| Click| Click|
 *                                  |Paste |      | ESC  |           | Next |      |      |
 *                                  `--------------------'           `--------------------'
 */
[EMACS] = KEYMAP(
// left hand
 KC_ACL0    ,M(KF_1)     ,M(KF_2)    ,M(KF_3) ,M(KF_4) ,M(KF_5) ,LGUI(KC_L)
,KC_ACL1    ,KC_NO       ,KC_HOME    ,KC_UP   ,KC_END  ,KC_NO   ,M(AE_VIS)
,KC_ACL2    ,M(AE_APPND) ,KC_LEFT    ,KC_DOWN ,KC_RIGHT,M(AE_INS)
,KC_MPLY    ,KC_NO       ,KC_PGUP    ,KC_DOWN ,KC_PGDN ,KC_X    ,M(AE_CPYC)
,M(AE_EMACS),M(AE_TERM)  ,M(AE_OTHER),KC_NO   ,KC_NO
                                                        ,KC_TRNS ,KC_TRNS
                                                                 ,KC_TRNS
                                           ,M(AE_PSTDEL),KC_TRNS ,KC_TRNS

                                                                     // right hand
                                                                     ,LGUI(KC_L),M(KF_6) ,M(KF_7) ,M(KF_8) ,M(KF_9) ,M(KF_10),M(KF_11)
                                                                     ,KC_WH_U   ,KC_DLR  ,M(A_MUL),KC_MS_U ,M(A_MUR),KC_R    ,KC_PSCR
                                                                                ,KC_D    ,KC_MS_L ,KC_MS_D ,KC_MS_R ,KC_NO   ,KC_NO
                                                                     ,KC_WH_D   ,KC_NO   ,M(A_MDL),KC_W    ,M(A_MDR),KC_NO   ,KC_MSTP
                                                                                         ,KC_VOLU ,KC_VOLD ,KC_MUTE ,KC_NO   ,KC_NO
                                                                     ,F(F_BSE)  ,KC_MS_BTN3
                                                                     ,KC_MPRV
                                                                     ,KC_MNXT   ,KC_BTN1 ,KC_BTN2
    ),

/* Keymap 5: One-handed, left side
 *
 * ,-----------------------------------------------------.
 * |        `~ | 1 F1 | 2 F2 | 3 F3 | 4 F4 | 5 F5 |A  BSE|
 * |-----------+------+------+------+------+-------------|
 * |       Tab |   '  |   ,  |   .  |   P  |   Y  |   [  |
 * |-----------+------+------+------+------+------|      |
 * |         - |   A  |   O  |   E  |   U  |   I  |------|
 * |-----------+------+------+------+------+------|   (  |
 * | Play/Pause|   ;  |   Q  |   J  |   K  |   X  |      |
 * `-----------+------+------+------+------+-------------'
 *     |  ESC  | Home | PgUp | PgDn | End  |
 *     `-----------------------------------'
 *                                         ,-------------.
 *                                         | LAlt | GUI  |
 *                                  ,------|------|------|
 *                                  |BackSp|LShift| Ctrl |
 *                                  |      |      |------|
 *                                  |Space |Enter |OTHER |
 *                                  `--------------------'
 */
[OHLFT] = KEYMAP(
// left hand
 KC_GRV     ,M(KF_1)     ,M(KF_2)     ,M(KF_3)   ,M(KF_4)    ,M(KF_5) ,M(OH_BASE)
,KC_TAB     ,KC_QUOT     ,KC_COMM     ,KC_DOT    ,KC_P       ,KC_Y    ,KC_LBRC
,KC_MINS    ,KC_A        ,KC_O        ,KC_E      ,KC_U       ,KC_I
,KC_MPLY    ,KC_SCLN     ,KC_Q        ,KC_J      ,KC_K       ,KC_X    ,KC_LPRN
,KC_ESC     ,KC_HOME     ,KC_PGUP     ,KC_PGDN   ,KC_END

                                                                ,M(A_ALT)    ,F(F_GUI)
                                                                             ,M(A_CTRL)
                                                    ,M(OH_BSSPC),M(OH_ENTSFT),M(OH_RIGHT)

                                                                // right hand
                                                               ,KC_NO   ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                               ,KC_NO   ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                                        ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                               ,KC_NO   ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                                                ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO

                                                               ,KC_NO   ,KC_NO
                                                               ,KC_NO
                                                               ,KC_NO   ,KC_NO  ,KC_NO
    ),

/* Keymap 6: One-handed, right side
 *
 * ,-----------------------------------------------------.
 * | =     F11 | 0 F10| 9 F9 | 8 F8 | 7 F7 | 6 F6 |A  BSE|
 * |-----------+------+------+------+------+-------------|
 * |         / |   L  |   R  |   C  |   G  |   F  |   ]  |
 * |-----------+------+------+------+------+------|      |
 * |         \ |   S  |   N  |   T  |   H  |   D  |------|
 * |-----------+------+------+------+------+------|   )  |
 * |      Stop |   Z  |   V  |   W  |   M  |   B  |      |
 * `-----------+------+------+------+------+-------------'
 *     |  ESC  | Left |  Up  | Down | Rght |
 *     `-----------------------------------'
 *                                         ,-------------.
 *                                         | LAlt | GUI  |
 *                                  ,------|------|------|
 *                                  |BackSp|LShift| Ctrl |
 *                                  |      |      |------|
 *                                  |Space |Enter |OTHER |
 *                                  `--------------------'
 */
[OHRGT] = KEYMAP(
// left hand
 M(KF_11)   ,M(KF_10)    ,M(KF_9)     ,M(KF_8)   ,M(KF_7)    ,M(KF_6) ,M(OH_BASE)
,KC_SLSH    ,KC_L        ,KC_R        ,KC_C      ,KC_G       ,KC_F    ,KC_RBRC
,KC_BSLS    ,KC_S        ,KC_N        ,KC_T      ,KC_H       ,KC_D
,KC_MSTP    ,KC_Z        ,KC_V        ,KC_W      ,KC_M       ,KC_B    ,KC_RPRN
,KC_ESC     ,KC_LEFT     ,KC_UP       ,KC_DOWN   ,KC_RIGHT

                                                                ,M(A_ALT)    ,F(F_GUI)
                                                                             ,M(A_CTRL)
                                                    ,M(OH_BSSPC),M(OH_ENTSFT),M(OH_LEFT)

                                                                // right hand
                                                               ,KC_NO   ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                               ,KC_NO   ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                                        ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                               ,KC_NO   ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                                                ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO

                                                               ,KC_NO   ,KC_NO
                                                               ,KC_NO
                                                               ,KC_NO   ,KC_NO  ,KC_NO
    ),


};

const uint16_t PROGMEM fn_actions[] = {
   [F_BSE] = ACTION_LAYER_CLEAR(ON_PRESS)
  ,[F_ECS] = ACTION_LAYER_INVERT(EMACS, ON_PRESS)
  ,[F_HUN] = ACTION_LAYER_INVERT(HUN, ON_PRESS)
  ,[F_GUI] = ACTION_MACRO_TAP(A_GUI)
};

void ang_handle_kf (keyrecord_t *record, uint8_t id)
{
  uint8_t code = id - KF_1;

  if (record->event.pressed) {
    kf_timers[code] = timer_read ();
  } else {
    uint8_t kc;

    if (timer_elapsed (kf_timers[code]) > TAPPING_TERM) {
      // Long press
      kc = KC_F1 + code;
    } else {
      if (id == KF_11)
        kc = KC_EQL;
      else
        kc = KC_1 + code;
    }

    register_code (kc);
    unregister_code (kc);
  }
}

void ang_shift_toggle (void)
{
  if (shift_state == 0) {
    register_code(KC_RSFT);
    shift_state = 1;
    ergodox_right_led_1_set(LED_BRIGHTNESS_HI);
    ergodox_right_led_1_on();
  } else {
    unregister_code(KC_RSFT);
    ergodox_right_led_1_set(LED_BRIGHTNESS_LO);
    ergodox_right_led_1_off();
    shift_state = 0;
  }
}


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
          ang_shift_toggle ();
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
          layer_off(HUN);
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
          layer_off(HUN);
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
          layer_off(HUN);
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
          layer_off(HUN);
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
          layer_off(HUN);
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
          layer_off(HUN);
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
          layer_off(HUN);
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
          layer_off(HUN);
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
          layer_off(HUN);
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
              if (shift_state == 1)
                {
                  unregister_code (KC_RSFT);
                  return MACRO(U(RSFT), T(RBRC), T(P), D(RSFT), END);
                }
              else
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
        break;

      case AE_INS:
        if (record->event.pressed) {
          return MACRO(T(I), END);
        } else {
          layer_clear();
        }
        break;

      case AE_OVR:
        if (record->event.pressed) {
          return MACRO(T(R), END);
        } else {
          layer_clear();
        }
        break;

      case AE_APPND:
        if (record->event.pressed) {
          return MACRO(T(A), END);
        } else {
          layer_clear();
        }
        break;

      case A_GUI:
        if (record->event.pressed) {
          register_code (KC_LGUI);
          if (record->tap.count && !record->tap.interrupted) {
            if (record->tap.count >= 2) {
              register_code (KC_W);
              layer_on (APPSEL);
              set_oneshot_layer (APPSEL, ONESHOT_START);
            }
          } else {
            record->tap.count = 0;
          }
          gui_timer = 0;
        } else {
          if (record->tap.count >= 2)
            {
              unregister_code (KC_W);
              clear_oneshot_layer_state (ONESHOT_PRESSED);
            }
          gui_timer = timer_read ();
        }
        break;

      case APP_SLK:
        if (record->event.pressed)
          return MACRO(T(S), T(C), T(U), T(D), T(C), T(L), T(O), T(U), T(D), T(ENT), END);
        break;

      case APP_EMCS:
        if (record->event.pressed)
          return MACRO(T(E), T(M), T(A), T(C), T(S), T(ENT), END);
        break;

      case APP_TERM:
        if (record->event.pressed)
          return MACRO(T(T), T(E), T(R), T(M), T(ENT), END);
        break;

      case APP_CHRM:
        if (record->event.pressed)
          return MACRO(T(C), T(H), T(R), T(O), T(M), T(ENT), END);
        break;

      case APP_MSIC:
        if (record->event.pressed)
          return MACRO(T(R), T(H), T(Y), T(T), T(H), T(M), T(B), T(O), T(X), T(ENT), END);
        break;

      case KF_1 ... KF_11:
        ang_handle_kf (record, id);
        break;

      case OH_BASE:
        if (record->event.pressed) {
          oh_base_timer = timer_read ();
        } else {
          if (timer_elapsed (oh_base_timer) > TAPPING_TERM) {
            layer_clear ();
          } else {
            return MACRO (T(APP), END);
          }
        }
        break;

      case OH_BSSPC:
        if (record->event.pressed) {
          oh_bsspc_timer = timer_read ();
        } else {
          if (timer_elapsed (oh_bsspc_timer) > TAPPING_TERM) {
            return MACRO (T(BSPC), END);
          } else {
            return MACRO (T(SPC), END);
          }
        }
        break;

      case OH_ENTSFT:
        if (record->event.pressed) {
          oh_entsft_timer = timer_read ();
        } else {
          if (timer_elapsed (oh_entsft_timer) > TAPPING_TERM) {
            ang_shift_toggle ();
          } else {
            return MACRO (T(ENT), END);
          }
        }
        break;

      case OH_LEFT:
        if (record->event.pressed) {
          layer_move (OHLFT);
          oh_left_blink = 1;
          oh_left_blink_timer = timer_read ();
          ergodox_right_led_1_on ();
        }
        break;

      case OH_RIGHT:
        if (record->event.pressed) {
          layer_move (OHRGT);
          oh_right_blink = 1;
          oh_right_blink_timer = timer_read ();
          ergodox_right_led_3_on ();
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

  if (gui_timer && timer_elapsed (gui_timer) > TAPPING_TERM)
    unregister_code (KC_LGUI);

  if (layer != OHLFT)
    oh_left_blink = 0;
  if (layer != OHRGT)
    oh_right_blink = 0;

  if (layer == HUN) {
    ergodox_right_led_2_on();
    ergodox_right_led_3_on();
  } else if (layer == EMACS) {
    ergodox_right_led_1_on();
    ergodox_right_led_2_on();
  } else {
    if (shift_state == 0 && oh_left_blink == 0)
      ergodox_right_led_1_off();
    if (alt_state == 0 && !(layer == OHLFT || layer == OHRGT))
      ergodox_right_led_2_off();
    if (ctrl_state == 0 && oh_right_blink == 0)
      ergodox_right_led_3_off();
  }

  if (layer == OHLFT || layer == OHRGT) {
    ergodox_right_led_2_on();

    if (oh_left_blink) {
      if (timer_elapsed (oh_left_blink_timer) > OH_BLINK_INTERVAL) {
        if (shift_state == 0)
          ergodox_right_led_1_off ();
      }
      if (timer_elapsed (oh_left_blink_timer) > OH_BLINK_INTERVAL * 2) {
        ergodox_right_led_1_on ();
        oh_left_blink_timer = timer_read ();
      }
    }

    if (oh_right_blink) {
      if (timer_elapsed (oh_right_blink_timer) > OH_BLINK_INTERVAL) {
        if (ctrl_state == 0)
          ergodox_right_led_3_off ();
      }
      if (timer_elapsed (oh_right_blink_timer) > OH_BLINK_INTERVAL * 2) {
        ergodox_right_led_3_on ();
        oh_right_blink_timer = timer_read ();
      }
    }
  }
};
