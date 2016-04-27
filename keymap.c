#include "ergodox_ez.h"
#include "led.h"
#include "debug.h"
#include "action_layer.h"
#include "action_util.h"

#define BASE     0 // default layer
#define LOCK     1 // shift-lock layer
#define HUN_LK   2 // Hungarian base layer
#define SYMB_SH  3 // symbols shift layer
#define SYMB_LK  4 // symbols lock layer
#define SYMB_UN  5 // symbols unlock layer
#define MDIA_SH  6 // media shift layer
#define MDIA_LK  7 // media lock layer
#define MDIA_UN  8 // media unlock layer
#define UNLOCK   9 // clear to base layer

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
/*
 * algernon's ErgoDox EZ layout.
 *  Based on Ordinary, with a bit of Dvorak, Hungarian and other changes.
 */

uint8_t shift_state = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Keymap 0: Base Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | LOCK   `~ |   1  |   2  |   3  |   4  |   5  | ESC  |           |  -   |   6  |   7  |   8  |   9  |   0  | =    LOCK |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * | Media Tab |   '  |   ,  |   .  |   P  |   Y  |   [  |           |  ]   |   F  |   G  |   C  |   R  |  L   | /   Media |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * | Symbol    |   A  |   O  |   E  |   U  |   I  |------|           |------|   D  |   H  |   T  |   N  |  S   | \  Symbol |
 * |-----------+------+------+------+------+------|   (  |           |  )   |------+------+------+------+------+-----------|
 * | LShift    |   ;  |   Q  |   J  |   K  |   X  |      |           |      |   B  |   M  |   W  |   V  |  Z   |    RShift |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     |  M-x  | LCtrl| Apps | LAlt | LGui |                                       | RGui | RAlt | Apps | RCtrl|  HUN  |
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | Home | End  |           | Left | Right|
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | PgUp |           |  Up  |      |      |
 *                                  |Backsp| Del  |------|           |------| Enter| Space|
 *                                  |      |      | PgDn |           | Down |      |      |
 *                                  `--------------------'           `--------------------'
 */
[BASE] = KEYMAP(
// left hand
 LT(LOCK, KC_GRV)   ,KC_1        ,KC_2        ,KC_3   ,KC_4  ,KC_5  ,KC_ESC 
,LT(MDIA_SH, KC_TAB),KC_QUOT     ,KC_COMM     ,KC_DOT ,KC_P  ,KC_Y  ,KC_LBRC
,MO(SYMB_SH)        ,KC_A        ,KC_O        ,KC_E   ,KC_U  ,KC_I
,AM_LSFT            ,KC_SCLN     ,KC_Q        ,KC_J   ,KC_K  ,KC_X  ,KC_LPRN     
,M(1)               ,KC_LCTL     ,KC_APP      ,KC_LALT,KC_LGUI

                                                            ,KC_HOME,KC_END
                                                                    ,KC_PGUP
                                                    ,KC_BSPC,KC_DEL ,KC_PGDN

                                                                // right hand
                                                               ,KC_MINS ,KC_6   ,KC_7   ,KC_8   ,KC_9        ,KC_0        ,LT(LOCK, KC_EQL)
                                                               ,KC_RBRC ,KC_F   ,KC_G   ,KC_C   ,KC_R        ,KC_L        ,LT(MDIA_SH, KC_SLSH)
                                                                        ,KC_D   ,KC_H   ,KC_T   ,KC_N        ,KC_S        ,LT(SYMB_SH, KC_BSLS)
                                                               ,KC_RPRN ,KC_B   ,KC_M   ,KC_W   ,KC_V        ,KC_Z        ,AM_RSFT
                                                                                ,KC_RGUI,KC_RALT,KC_APP      ,KC_RCTL     ,KC_FN4 

                                                               ,KC_LEFT ,KC_RGHT
                                                               ,KC_UP
                                                               ,KC_DOWN ,KC_ENT ,KC_SPC
    ),

/* Keymap 1: Layer Lock Keys
 *
 * ,-------------------------------------------------------.           ,-------------------------------------------------------.
 * |             |  Esc |      |      |      |      |      |           |      |      |      |      |      | Bspc |             |
 * |-------------+------+------+------+------+-------------|           |------+------+------+------+------+------+-------------|
 * | Media  LOCK |      |      |      |      |      |      |           |      |      |      |      |      |      | Media  LOCK |
 * |-------------+------+------+------+------+------|      |           |      |------+------+------+------+------+-------------|
 * | Symbol LOCK |      |      |      |      |      |------|           |------|      |      |      |      |      | Symbol LOCK |
 * |-------------+------+------+------+------+------|      |           |      |------+------+------+------+------+-------------|
 * | Caps   LOCK |      |      |      |      |      |      |           |      |      |      |      |      |      | Caps   LOCK |
 * `-------------+------+------+------+------+-------------'           `-------------+------+------+------+------+-------------'
 *      |        |      |      |      |      |                                       |      |      |      |      | HUN LK |
 *      `------------------------------------'                                       `------------------------------------'
 *                                           ,-------------.           ,-------------.
 *                                           |      |      |           |      |      |
 *                                    ,------|------|------|           |------+------+------.
 *                                    |      |      |      |           |      |      |      |
 *                                    |      |      |------|           |------|      |      |
 *                                    |      |      |      |           |      |      |      |
 *                                    `--------------------'           `--------------------'
 */
[LOCK] = KEYMAP(
// left hand
 KC_TRNS ,KC_ESC  ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,KC_FN3  ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,KC_FN2  ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,AM_CAPS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                             ,KC_TRNS ,KC_TRNS
                                                      ,KC_TRNS
                                     ,KC_TRNS,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_BSPC ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_FN3
                                                                        ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_FN2
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,AM_CAPS
                                                                                 ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_FN4 

                                                               ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS
    ),

/* Keymap 2: Hungarian Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | UNLOCK    |      |      |      |      |      |      |           |      |      |      |      |      |      |    UNLOCK |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |  Ő   |      |  Ű   |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |   Á  |  Ó   |  É   |  Ú   |  Í   |------|           |------|      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * | Caps LOCK |      |  Ö   |      |  Ü   |      |      |           |      |      |      |      |      |      | Caps LOCK |
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

[HUN_LK] = KEYMAP(
// left hand
 KC_FN1  ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_TRNS
,KC_NO   ,KC_NO   ,HU_OEE  ,KC_NO   ,HU_UEE  ,KC_NO   ,KC_NO  
,KC_NO   ,HU_AA   ,HU_OO   ,HU_EE   ,HU_UU   ,HU_II
,KC_CAPS ,KC_NO   ,HU_OE   ,KC_NO   ,HU_UE   ,KC_NO   ,KC_NO  
,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                             ,KC_TRNS ,KC_TRNS
                                                      ,KC_TRNS
                                    ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_FN1
                                                               ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO  
                                                                        ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO  
                                                               ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_CAPS
                                                                                 ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                               ,KC_TRNS,KC_TRNS
                                                               ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS  ,KC_TRNS
    ),

/* Keymap 3: Symbol Shift Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | LOCK      |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |  -   |  F6  |  F7  |  F8  |  F9  |  F10 |      LOCK |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * | Media     |   !  |   @  |   {  |   }  |   &  |  <   |           |  >   |   |  |   7  |   8  |   9  |   /  |     Media |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * | Symbol    |   #  |   $  |   (  |   )  |   `  |------|           |------|   /  |   4  |   5  |   6  |   *  |    Symbol |
 * |-----------+------+------+------+------+------| Tab  |           |  Alt |------+------+------+------+------+-----------|
 * | LShift    |   %  |   ^  |   [  |   ]  |   ~  |      |           |  -Tab|   \  |   1  |   2  |   3  |   -  |    RShift |
 * `-----------+------+------+------+------+-------------'           `------------+------+------+------+------+------------'
 *      |      |      |      |      |      |                                       |   0  |    . |   =  |   +  | Entr |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | Left | Right|           | Home | End  |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |  Up  |           | PgUp |      |      |
 *                                  |Space |Enter |------|           |------|BackSp| Del  |
 *                                  |      |      | Down |           | PgDn |      |      |
 *                                  `--------------------'           `--------------------'
 */
[SYMB_SH] = KEYMAP(
// left hand
 KC_FN2        ,KC_F1   ,KC_F2   ,KC_F3   ,KC_F4   ,KC_F5   ,KC_TRNS
,MO(MDIA_SH)   ,KC_EXLM ,KC_AT   ,KC_LCBR ,KC_RCBR ,KC_AMPR ,LSFT(KC_COMM)
,MO(SYMB_SH)   ,KC_HASH ,KC_DLR  ,KC_LPRN ,KC_RPRN ,KC_GRV 
,AM_LSFT       ,KC_PERC ,KC_CIRC ,KC_LBRC ,KC_RBRC ,KC_TILD ,KC_TAB
,KC_TRNS       ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                   ,KC_LEFT ,KC_RGHT
                                                            ,KC_UP
                                          ,KC_SPC  ,KC_ENT  ,KC_DOWN
                                                                    // right hand
                                                                    ,KC_MINS     ,KC_F6   ,KC_F7 ,KC_F8  ,KC_F9 ,KC_F10  ,KC_FN2
                                                                    ,LSFT(KC_DOT),KC_PIPE ,KC_7  ,KC_8   ,KC_9  ,KC_SLSH ,MO(MDIA_SH)
                                                                                 ,KC_SLSH ,KC_4  ,KC_5   ,KC_6  ,KC_ASTR ,MO(SYMB_SH)
                                                                    ,LALT(KC_TAB),KC_BSLS ,KC_1  ,KC_2   ,KC_3  ,KC_MINS ,AM_RSFT
                                                                                          ,GUI_T(KC_0),ALT_T(KC_DOT),ALL_T(KC_EQL),MEH_T(KC_PLUS),CTL_T(KC_ENT)
                                                                    ,KC_HOME     ,KC_END
                                                                    ,KC_PGUP
                                                                    ,KC_PGDN     ,KC_BSPC ,KC_DEL
),

/* Keymap 4: Symbol Lock Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | LOCK      |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |  -   |  F6  |  F7  |  F8  |  F9  |  F10 |      LOCK |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * | Media     |   !  |   @  |   {  |   }  |   &  |  <   |           |  >   |   |  |   7  |   8  |   9  |   /  |     Media |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * | Symbol    |   #  |   $  |   (  |   )  |   `  |------|           |------|   /  |   4  |   5  |   6  |   *  |    Symbol |
 * |-----------+------+------+------+------+------| Tab  |           |  Alt |------+------+------+------+------+-----------|
 * | LShift    |   %  |   ^  |   [  |   ]  |   ~  |      |           |  -Tab|   \  |   1  |   2  |   3  |   -  |    RShift |
 * `-----------+------+------+------+------+-------------'           `------------+------+------+------+------+------------'
 *      |      |      |      |      |      |                                       |   0  |    . |   =  |   +  | Entr |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | Left | Right|           | Home | End  |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |  Up  |           | PgUp |      |      |
 *                                  |Space |Enter |------|           |------|BackSp| Del  |
 *                                  |      |      | Down |           | PgDn |      |      |
 *                                  `--------------------'           `--------------------'
 */
[SYMB_LK] = KEYMAP(
// left hand
 MO(SYMB_UN)  ,KC_F1   ,KC_F2   ,KC_F3   ,KC_F4   ,KC_F5   ,KC_TRNS
,MO(MDIA_SH) ,KC_EXLM ,KC_AT   ,KC_LCBR ,KC_RCBR ,KC_AMPR ,LSFT(KC_COMM)
,MO(UNLOCK)  ,KC_HASH ,KC_DLR  ,KC_LPRN ,KC_RPRN ,KC_GRV
,AM_LSFT     ,KC_PERC ,KC_CIRC ,KC_LBRC ,KC_RBRC ,KC_TILD ,KC_TAB
,KC_TRNS     ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                 ,KC_LEFT ,KC_RGHT
                                                          ,KC_UP
                                        ,KC_SPC  ,KC_ENT  ,KC_DOWN
                                                                    // right hand
                                                                    ,KC_MINS     ,KC_F6   ,KC_F7 ,KC_F8  ,KC_F9 ,KC_F10  ,MO(SYMB_UN)
                                                                    ,LSFT(KC_DOT),KC_PIPE ,KC_7  ,KC_8   ,KC_9  ,KC_SLSH ,MO(MDIA_SH)
                                                                                 ,KC_SLSH ,KC_4  ,KC_5   ,KC_6  ,KC_ASTR ,MO(UNLOCK)
                                                                    ,LALT(KC_TAB),KC_BSLS ,KC_1  ,KC_2   ,KC_3  ,KC_MINS ,AM_RSFT
                                                                                          ,GUI_T(KC_0),ALT_T(KC_DOT),ALL_T(KC_EQL),MEH_T(KC_PLUS),CTL_T(KC_ENT)
                                                                    ,KC_HOME     ,KC_END
                                                                    ,KC_PGUP
                                                                    ,KC_PGDN     ,KC_BSPC ,KC_DEL
    ),

/* Keymap 5: Symbol Unlock Layer
 *
 * ,------------------------------------------------------.           ,------------------------------------------------------.
 * |            | Esc  |      |      |      |      |      |           |      |      |      |      |      | Bspc |            |
 * |------------+------+------+------+------+-------------|           |------+------+------+------+------+------+------------|
 * | Media LOCK |      |      |      |      |      |      |           |      |      |      |      |      |      | Media LOCK |
 * |------------+------+------+------+------+------|      |           |      |------+------+------+------+------+------------|
 * |     UNLOCK |      |      |      |      |      |------|           |------|      |      |      |      |      |     UNLOCK |
 * |------------+------+------+------+------+------|      |           |      |------+------+------+------+------+------------|
 * | Caps  LOCK |      |      |      |      |      |      |           |      |      |      |      |      |      | Caps  LOCK |
 * `------------+------+------+------+------+-------------'           `-------------+------+------+------+------+------------'
 *       |      |      |      |      |      |                                       |      |      |      |      |      |
 *       `----------------------------------'                                       `----------------------------------'
 *                                          ,-------------.           ,-------------.
 *                                          |      |      |           |      |      |
 *                                   ,------|------|------|           |------+------+------.
 *                                   |      |      |      |           |      |      |      |
 *                                   |      |      |------|           |------|      |      |
 *                                   |      |      |      |           |      |      |      |
 *                                   `--------------------'           `--------------------'
 */
[SYMB_UN] = KEYMAP(
// left hand
 KC_TRNS     ,KC_ESC  ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,TG(MDIA_LK) ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,KC_FN1      ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,AM_CAPS     ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,KC_TRNS     ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                ,KC_TRNS  ,KC_TRNS
                                                          ,KC_TRNS
                                        ,KC_TRNS,KC_TRNS  ,KC_TRNS
                                                                     // right hand
                                                                     ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_BSPC ,KC_TRNS
                                                                     ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,TG(MDIA_LK)
                                                                              ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_FN1
                                                                     ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,AM_CAPS
                                                                     ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                     ,KC_TRNS ,KC_TRNS
                                                                     ,KC_TRNS
                                                                     ,KC_TRNS ,KC_TRNS ,KC_TRNS
),

/* Keymap 6: Media Shift Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | LOCK      | F11  | F12  | F13  | F14  | F15  |ScrLCK|           |      | F16  | F17  | F18  | F19  | F20  |      LOCK |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * | Media     |      |LClick| MsUp |RClick|Vol Up|ScrlUp|           |ScrlUp|PrtScr| Home |  Up  | PgUp | Mail |     Media |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * | Symbol    |      |MsLeft|MsDown|MsRght|Vol Dn|------|           |------|NumLok| Left | Down | Right|MyComp|    Symbol |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * | LShift    |      |      |MsDown|      | Mute |ScrlDn|           |ScrlDn|      | End  | Down | PgDn |      |    RShift |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |LCtrl |Hyper | LAlt | LGui |                                       |Insert|Delete| Hyper| LAlt | LGui |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | Stop |Refrsh|           | Prev | Next |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |Left  |Right |Search|           |VolUp |      |      |
 *                                  | Click| Click|------|           |------| Stop | Play-|
 *                                  |      |      | Home |           |VolDn |      | Pause|
 *                                  `--------------------'           `--------------------'
 */
[MDIA_SH] = KEYMAP(
// left hand
 KC_FN3        ,KC_F11      ,KC_F12  ,KC_F13  ,KC_F14  ,KC_F15  ,LGUI(KC_L)
,MO(MDIA_SH)   ,KC_NO       ,KC_BTN1 ,KC_MS_U ,KC_BTN2 ,KC_VOLU ,KC_WH_U
,MO(SYMB_SH)   ,KC_NO       ,KC_MS_L ,KC_MS_D ,KC_MS_R ,KC_VOLD
,AM_LSFT       ,KC_NO       ,KC_NO   ,KC_MS_D ,KC_NO   ,KC_MUTE ,KC_WH_D
,KC_NO         ,KC_LCTL     ,ALL_T(KC_NO),KC_LALT,KC_LGUI
                                                       ,KC_MS_BTN3 ,KC_WREF
                                                                ,KC_WSCH
                                              ,KC_BTN1 ,KC_BTN2 ,KC_WHOM
                                                                     // right hand
                                                                     ,KC_NO    ,KC_F16  ,KC_F17 ,KC_F18  ,KC_F19  ,KC_F20  ,KC_FN3
                                                                     ,KC_WH_U  ,KC_PSCR ,KC_HOME,KC_UP   ,KC_PGUP ,KC_MAIL ,MO(MDIA_SH)
                                                                               ,KC_NLCK ,KC_LEFT,KC_DOWN ,KC_RIGHT,KC_MYCM ,MO(SYMB_SH)
                                                                     ,KC_WH_D  ,KC_NO   ,KC_END ,KC_DOWN ,KC_PGDN ,KC_NO   ,AM_RSFT
                                                                                        ,GUI_T(KC_INS),ALT_T(KC_DEL),ALL_T(KC_NO),MEH_T(KC_NO),KC_RCTL
                                                                     ,KC_MPRV  ,KC_MNXT
                                                                     ,KC_VOLU
                                                                     ,KC_VOLD  ,KC_MSTP ,KC_MPLY
),

/* Keymap 7: Media Lock Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | LOCK      | F11  | F12  | F13  | F14  | F15  |ScrLCK|           |      | F16  | F17  | F18  | F19  | F20  |      LOCK |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * | Media     |      |LClick| MsUp |RClick|Vol Up|ScrlUp|           |ScrlUp|PrtScr| Home |  Up  | PgUp | Mail |     Media |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |MsLeft|MsDown|MsRght|Vol Dn|------|           |------|NumLok| Left | Down | Right|MyComp|           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * | LShift    |      |      |MsDown|      | Mute |ScrlDn|           |ScrlDn|      | End  | Down | PgDn |      |    RShift |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |LCtrl |Hyper | LAlt | LGui |                                       |Insert|Delete| Hyper| LAlt | LGui |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | Stop |Refrsh|           | Prev | Next |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |Left  |Right |Search|           |VolUp |      |      |
 *                                  | Click| Click|------|           |------| Stop | Play-|
 *                                  |      |      | Home |           |VolDn |      | Pause|
 *                                  `--------------------'           `--------------------'
 */
[MDIA_LK] = KEYMAP(
// left hand
 MO(MDIA_UN),KC_F11      ,KC_F12  ,KC_F13  ,KC_F14  ,KC_F15  ,LGUI(KC_L)
,MO(UNLOCK) ,KC_NO       ,KC_BTN1 ,KC_MS_U ,KC_BTN2 ,KC_VOLU ,KC_WH_U
,KC_NO      ,KC_NO       ,KC_MS_L ,KC_MS_D ,KC_MS_R ,KC_VOLD  
,AM_LSFT    ,KC_NO       ,KC_NO   ,KC_MS_D ,KC_NO   ,KC_MUTE ,KC_WH_D
,KC_NO      ,KC_LCTL     ,ALL_T(KC_NO),KC_LALT,KC_LGUI
                                                    ,KC_MS_BTN3 ,KC_WREF
                                                             ,KC_WSCH
                                           ,KC_BTN1 ,KC_BTN2 ,KC_WHOM
                                                                     // right hand
                                                                     ,KC_NO    ,KC_F16  ,KC_F17 ,KC_F18  ,KC_F19  ,KC_F20  ,MO(MDIA_UN)
                                                                     ,KC_WH_U  ,KC_PSCR ,KC_HOME,KC_UP   ,KC_PGUP ,KC_MAIL ,MO(UNLOCK)
                                                                               ,KC_NLCK ,KC_LEFT,KC_DOWN ,KC_RIGHT,KC_MYCM ,KC_NO
                                                                     ,KC_WH_D  ,KC_NO   ,KC_END ,KC_DOWN ,KC_PGDN ,KC_NO   ,AM_RSFT
                                                                                        ,GUI_T(KC_INS),ALT_T(KC_DEL),ALL_T(KC_NO),MEH_T(KC_NO),KC_RCTL
                                                                     ,KC_MPRV  ,KC_MNXT
                                                                     ,KC_VOLU
                                                                     ,KC_VOLD  ,KC_MSTP ,KC_MPLY
    ),

/* Keymap 8: Media Unlock Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           | Esc  |      |      |      |      |      |           |      |      |      |      |      | Bspc |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |    UNLOCK |      |      |      |      |      |      |           |      |      |      |      |      |      |    UNLOCK |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |------|           |------|      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * | Caps LOCK |      |      |      |      |      |      |           |      |      |      |      |      |      | Caps LOCK |
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
[MDIA_UN] = KEYMAP(
// left hand
 KC_TRNS ,KC_ESC  ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,KC_FN1  ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,AM_CAPS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                             ,KC_TRNS ,KC_TRNS
                                                      ,KC_TRNS
                                    ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_BSPC ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_FN1
                                                                        ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,AM_CAPS
                                                                                 ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                               ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS
    ),
/* Keymap 9: Unlock Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | UNLOCK    |      |      |      |      |      |      |           |      |      |      |      |      |      |    UNLOCK |
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
[UNLOCK] = KEYMAP(
// left hand
 KC_FN1  ,KC_ESC  ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                             ,KC_TRNS ,KC_TRNS
                                                      ,KC_TRNS
                                    ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_FN1
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                        ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                                 ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                               ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS  ,KC_TRNS
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
          return MACRO (D(LALT), T(X), U(LALT), END);
        }
        break;

      case 5:
      case 6:
        if (record->event.pressed) {
          if (shift_state == 0) {
            register_code(KC_RSFT);
            shift_state = 1;
            ergodox_right_led_1_on();
          } else {
            unregister_code(KC_RSFT);
            unregister_code(KC_CAPS);
            ergodox_right_led_1_off();
            shift_state = 0;
          }
        }
        break;

      case C_AA:
        if (record->event.pressed)
          return MACRO (D(RALT), U(RALT), T(QUOT), T(A), END);
        break;
      case C_OO:
        if (record->event.pressed)
          return MACRO (D(RALT), U(RALT), T(QUOT), T(O), END);
        break;
      case C_EE:
        if (record->event.pressed)
          return MACRO (D(RALT), U(RALT), T(QUOT), T(E), END);
        break;
      case C_UU:
        if (record->event.pressed)
          return MACRO (D(RALT), U(RALT), T(QUOT), T(U), END);
        break;
      case C_II:
        if (record->event.pressed)
          return MACRO (D(RALT), U(RALT), T(QUOT), T(I), END);
        break;
      case C_OE:
        if (record->event.pressed)
          return MACRO (D(RALT), U(RALT), D(LSHIFT), T(QUOT), U(LSHIFT), T(O), END);
        break;
      case C_UE:
        if (record->event.pressed)
          return MACRO (D(RALT), U(RALT), D(LSHIFT), T(QUOT), U(LSHIFT), T(U), END);
        break;
      case C_OEE:
        if (record->event.pressed)
          return MACRO (D(RALT), U(RALT), T(EQL), T(O), END);
        break;
      case C_UEE:
        if (record->event.pressed)
          return MACRO (D(RALT), U(RALT), T(EQL), T(U), END);
        break;
      }
    return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
    uint8_t layer = biton32(layer_state);

    // do not change lights while locking and unlocking
    if(layer == LOCK || layer == SYMB_UN || layer == MDIA_UN || layer == UNLOCK) {
        return;
    }

    if (layer == HUN_LK) {
      ergodox_right_led_2_on();
      ergodox_right_led_3_on();
    } else {
      // symbol turns on green light
      if(layer == SYMB_SH || layer == SYMB_LK) {
        ergodox_right_led_2_on();
      } else {
        ergodox_right_led_2_off();
      }

      // media turns on blue light
      if(layer == MDIA_SH || layer == MDIA_LK) {
        ergodox_right_led_3_on();
      } else {
        ergodox_right_led_3_off();
      }
    }
};

