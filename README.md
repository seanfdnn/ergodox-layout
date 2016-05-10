algernon's layout
=======================

This is an unconventional layout for the ErgoDox EZ: first of all, it is based
on the Dvorak layout, but moves a few keys elsewhere for an improved coding
experience. Second, there is no `DEL` key, nor a `Caps Lock`. Third, the
`SHIFT`, `Alt`, and `Control` keys are all sticky: when you press and release
them, they still register pressed, until you toggle them off.

But lets get on with the base layer, before I dive into a deeper explanation!

## Base layer

![Base layer](images/base-layer.png)

As you can see, this appears to be a somewhat unremarkable Dvorak layout, with
some very minor changes:

* The `Backslash/Pipe` key moved a row up, because it is easier to reach it on
  the 4th row than on the 5th.
* Consequently `Minus/Underscore` key was moved to the left half.
* There are dedicated `bracket` and `parens` keys.

Apart from these changes, as far as the alphanumerics are considered, this is
just plain old Dvorak. The fun comes with the modifiers, and their behaviour!

Originally, I had the `Shift` keys at their more conventional position, on the
sides of the keyboard, where `Minus` and `Backslash` are right now. The `Alt`,
`Control`, `GUI`, and `Apps` keys were on the bottom row, and `ESC` up top. But
I press modifiers very often, so placing them on the bottom row was incredibly
inconvenient, and my fingers struggled reaching them, let alone holding them. On
the other hand, I use the navigational keys less often, and when I do, I will
likely stay in navigation mode for some time.

For these reasons, I rearranged the keys to their current position: modifiers,
layer switchers, `Backspace`, `Return`, `Space` and `ESC` are all on the thumb
cluster, because my strongest digit has no problem hitting them often. It is
easy to reach them, and them being toggles, I don't even have no hold them.

The navigation keys are somewhat experimental at the moment, I'm not entirely
happy with how they turned out. Yet, I am playing with the idea of learning to
switch to the media layer for navigational needs. We'll see how that works in
the next few weeks.

There are two more interesting keys: the red ones on the far sides. The left one
simply sends `M-m`, for the rare case when my Spacemacs is in `holy-mode`. The
right one is the Unicode compose key: it sends a sequence that tells my OS that
we are doing unicode composition.

As for the coloring of the keys: the guiding idea is that keys pressed by the
same finger have the same color. But that is just a guide, because the thumb
cluster has many colors, all pressed by the same digit. Functionality is the
other guide, for most special keys.

## Symbols layer

![Symbols layer](images/symbol-layer.png)

The symbol layer is a bit more conventional, and retains most of the special
keys in the same position as in the base layer. As an addition, function keys
appear on the sides, and the two other layer toggles turn into a `λ` and a
shruggie sequence, for various reasons.

The key colors were done the same way as in the base layer.

## Media layer

![Media layer](images/media-layer.png)

For all mouse-y, navigational and media needs!

The coloring here also follows a different pattern: blue are mouse keys, red is
cursor navigation, light orange is media control, pink are copy/paste related
functions, and the rest are as usual.

The copy/paste stuff needs some explanation: they only work in Spacemacs. The
topmost pink button toggles visual mode on and off, by sending `V` first, and
`ESC` the second time. The button below it sends `X` for cutting when tapped,
`D` for deleting when held. The pink button on the thumb cluster sends `Y` for
copy when tapped, `P` for paste when held.

## Things to experiment with:

* Use `OSL(HUN)` - a one-shot layer switch - for the Hungarian layer. May have
  to move the button for that?
* Blinking LEDs for the layers. Blink when layer's active, on when modifier is.
* `DEBUG` and `RESET` buttons.
* Improve the `Paste` key on the media layer: add a way to paste above or below
  the current line.
