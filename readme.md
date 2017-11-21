<!-- -*- mode: markdown; fill-column: 8192 -*- -->

seanfdnn's layout
=======================

This is a fork of Algernon's ErgoDox EZ layout for my personal use.
 [ez]: https://ergodox-ez.com/
 [blog-ergodox]: https://asylum.madhouse-project.org/blog/tags/ergodox/

Some modifications from Algernon's design:
* Removal of Steno layer
* Removal of Hungarian layer
* Removal of Unicode Symbol Input
* Modifications to optimize for vim and i3wm
* Removal of Adore layer
* Removal of Lead Key and special unicode characters

Roadmap and some undecided changes:
* Collapsing media and/or arrow keys
* Re-purposing TMux keys
* Incorporating ADORE layer parts into base layer

## Table of Contents

* [Layouts](#layouts)
    - [Base layer](#base-layer)
    - [ADORE layer](#adore-layer)
    - [LED states](#led-states)
* [Tools](#tools)
    - [Heatmap](#heatmap)
    - [Layer notification](#layer-notification)
* [Building](#building)
    - [Using on Windows](#using-on-windows)
* [License](#license)

# Layouts

## Base layer

[![Base layer](https://github.com/algernon/ergodox-layout/raw/master/images/base-layer.png)](http://www.keyboard-layout-editor.com/#/gists/28f7eb305fdbff943613e1dc7aa9e82b)

At its core, this is a Dvorak layout, with some minor changes. The more interesting parts are how certain keys behave:

* The number row is the same as in the [ADORE](#adore-layer) layer. The function keys are on the **Media** layer.
* The `Shift`, `Alt`, and `Control` modifiers are one-shot. When tapped, they are considered active for the next key press only. When double tapped, they toggle on, until a third, single tap sometime later. When held, they act as expected. My usual pattern is that I use these for the next keypress only, so this behaviour is perfect. If I need them held, I'll just double-tap.
* The `GUI` key is special, because while a single tap works as usual, when double-tapped, it turns the number row into an application selector, and when triple tapped, it runs an application selector program on the host.
* The `ESC` key also doubles as a one-shot cancel key: if tapped while any of the one-shot modifiers are in-flight (as in, single-tapped, and not expired yet), it cancels all one-shot modifiers. It also cancels the **Hun** layer, if active. Otherwise it sends the usual keycode.
* The **Media** layer keys are one-shot.
* The **Fx** key is one-shot, and activates the **Media** layer, along with a one-shot `Alt`.
* When holding the `Tab`/**Arrow** key, the arrow layer activates while the key is held. Tapping the key produces the normal, `Tab` key. Double-tapping it toggles the **Arrow** layer on until a third tap.
* Tapping the `:` key once yields `:`, tapping it twice yields `;`.
* Tapping the `[{(`/`)}]` keys once yields `[` (or `{` when shifted), tapping them twice yields `(`.

The symbols on the front in the image above have the same color as the key that activates them, with the exception of the **Arrow** layer, which is just black on the front.

## ADORE layer

[![ADORE layer](https://github.com/algernon/ergodox-layout/raw/master/images/adore-layer.png)](http://www.keyboard-layout-editor.com/#/gists/45681a17453d235925b6028dd83bf12a)

My experimental layout, that I keep tweaking. No full description here, because things are very much in flux.

## LED states

The primary purpose of the LEDs is to show the modifier status, a secondary, to show which layer is active. Each modifier, `Shift`, `Alt` and `Control` each have their designated LEDs: the *red*, *green* and *blue*, respectively. When a modifier is in a one-shot state, the respective LED will turn on with a dimmer light. If the modifier is toggled on, the brightness of the LED turns full.

For the layers, the following rules apply:

* When the [ADORE layer](#adore-layer) is toggled on, LEDs will light up from left to right in a sequence, then turn off. When the layer is toggled off, the LEDs light up and turn off in the other direction. No LEDs are on while the layer is active.
* When the **Media** layer is active, the *red* and *green* ones are on.
* When the **ARROW** layer is active, the *red* and *blue* ones are on.

Unless noted otherwise, the layers use a dim light for the LEDs, while modifiers use a stronger one, and modifiers override any layer preferences. For example, when on the one-handed layer, with the left side active (*red* light blinking), if `Shift` is on, the *red* light will be constantly on.

# Special features

## Unicode Symbol Input

Once in the Unicode Symbol Input mode, one is able to type in symbol names, press `Enter` or `Space`, and get the Unicode symbol itself back. When in the mode, a capital `U` is printed first. Once the sequence is finished, all of it is erased by sending enough `Backspace` taps, and the firmware starts the OS-specific unicode input sequence. Then, it looks up the symbol name, and enters the associated code. If it is not found, it will just replay the pressed keycodes.

For the list of supported symbols, please see the source.

This is an experimental feature, and may or may not work reliably.

# Tools

## Heatmap

When the keypress logging functionality is enabled (by `LEAD d`), the keyboard will output a line every time a key is pressed, containing the position of the key in the matrix. This allows one to collect this information, and build analytics over it, such as a heat map, including dead keys too.

Included with the firmware is a small tool that can parse these logs, and create a heatmap that one can import into [KLE][kle]. To use it, either pipe the output of `hid_listen` into it, or pipe it an already saved log, and it will save the results into files in an output directory (given on the command-line). See the output of `tools/log-to-heatmap.py --help` for more information.

 [kle]: http://www.keyboard-layout-editor.com/

The generated heatmap looks somewhat like this:

 ![Heatmap](https://github.com/algernon/ergodox-layout/raw/master/images/heatmap.png)

## Layer notification

There is a very small tool in `tools/layer-notify`, that listens to the HID console, looking for layer change events, and pops up a notification for every detected change. It is a very simple tool, mainly serving as an example.

# Building

To make my workflow easier, this layout is maintained in [its own repository][algernon:ez-layout]. To build it, you will need the [QMK][qmk] firmware checked out, and this repo either checked out to something like `layouts/community/seanfdnn`, or symlinked there. One way to achieve that is this:

 [qmk]: https://github.com/qmk/qmk_firmware

```
$ git clone https://github.com/qmk/qmk_firmware.git
$ cd qmk_firmware
$ git clone https://github.com/seanfdnn/ergodox-layout.git \
            layouts/community/ergodox/seanfdnn
$ make ergodox_ez:seanfdnn
```

From time to time, updates may be submitted back to the QMK repository. If you are reading it there, you can build the firmware like any other firmware included with it (assuming you are in the root directory of the firmware):

```
$ make ergodox_ez:seanfdnn:
```

## Using on Windows

The keymap default to forcing NKRO, which seems to upset Windows, and except the modifiers, none of them work. If you experience this problem, recompile the firmware with `FORCE_NKRO=no` added to the `make` command line.

# License

The layout, being a derivative of the original TMK firmware which is under the GPL-2+, this layout is under the GPL as well, but GPL-3+, rather than the older version.
