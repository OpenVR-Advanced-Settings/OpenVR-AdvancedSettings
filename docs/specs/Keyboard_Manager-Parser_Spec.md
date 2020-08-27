## Specification considerations for sending keys to OS

The currently implemented parser, the same one used in the "Keyboard Input" button on the Utilities page, can handle a sequence of characters with shift key support. It does not support the ctrl, alt or super modifier keys.

The current spec sends a key press and release before sending the next key. The shift key is held down if two or more keys in a row are capitals.

The replacement spec should be easy to use for non-technical end users, while not being too limited for more technical end-users.

Key down (pressed) is defined as `KEY+` and key up (released) is defined as `KEY-`. Pressing and releasing the `a` key would be `a+, a-`.

A hard requirement is being able to trigger the numpad asterisk key (NUM_\*+, NUM_\*-) and a Ctrl + H sequence (Ctrl+, H+, H-, Ctrl-).

Ideally the same algorithm would handle both sequential non-modifier characters and a single modifier enabled key.

The VR keyboard does not have a numpad, which means it's necessary to have specific string values for numpad keys and then parse the string with that in mind.

Supporting all localizations is most likely going to be a nightmare outside of the scope of this project. Only QWERTY support should be expected.

## Suggested spec

### Definitions

A modifier key is either Alt, Ctrl, Super, AltGr or Shift. Only the left variants of modifiers with several versions are supported.

A literal key is either a-z, 0-9, Backspace, Enter, Space, Tab, Esc, insert, delete, end, page down, page up, NUM_/, NUM_*, NUM_-, NUM_+, CAPS, F1-F12, PRNTSCRN, PAUSE/BREAK, SCROLL LOCK, SPACE or the arrow keys.

A modifier symbol is a single representative symbol of a modifier key.

A literal symbol is a single representative lowercase character or an uppercase character sequence for a literal key.

A token is either a modifier symbol, literal symbol, symbol sequence or space character.


### Modifier symbols

| Modifier | Symbol |
|---|---|
| Ctrl | `^` |
| Alt | `*` |
| Shift | `>` |
| Super (Windows) | `#` |

### Literal symbols

| Key | Symbol |
|---|---|
| A to Z | The letter in lowercase. `a`, `b`, `c`, ... |
| Number Row 0-9 | The number. `0`, `1`, `2`, ... |
| Function keys F1-F9 | Capital F followed by number. `F1`, `F2`, `F3`, ... |
| Function keys F10-F12 | Capital G followed by number. `G0` = F10, `G1` = F11, ... |
| Backspace | `BACKSPACE` |
| Space | `SPACE` |
| Tab | `TAB` |
| Escape | `ESC` |
| Insert | `INS` |
| Delete | `DEL` |
| End | `END` |
| Page Down | `PGDN` |
| Page Up | `PGUP` |
| Caps Lock | `CAPS` |
| Print Screen | `PRNSCRN` |
| Pause/Break | `PAUSE` |
| Scroll Lock | `SCRLOCK` |
| Left Arrow | `LEFTARROW` |
| Right Arrow | `RIGHTARROW` |
| Up Arrow | `UPARROW` |
| Down Arrow | `DOWNARROW` |
| Keypad Slash | `KPSLASH` |
| Keypad Asterisk | `KPSTAR` |
| Keypad Minus | `KPMINUS` |
| Keypad Plus | `KPPLUS` |
| Enter | `ENTER` |
| Backslash | `BACKSLASH` |


### Misc symbols
| Action | Symbol |
|---|---|
| Release Held Keys | ` ` (literal space or end of string) |

### Operation

Strings are parsed from left to right. There is no support for right to left scripts.

The entire string is parsed before any keys are sent. If an invalid sequence is encountered all valid tokens are sent as keys, but the invalid output is discarded and parsing will not continue. A log message may be displayed.

When a modifier token is encountered the modifier is held down until either a space token or the end of the string is encountered.

When a literal token is encountered the literal key is pressed and then released with no other actions in between.

When a space token or end of string is encountered all currently held modifier keys are released in the order they were pressed.

### Misc

There is no way to release modifier keys other than a space or end of string. Additional modifier symbols when that modifier is already pressed does nothing.

Two or more spaces in succession has the same effect as a single space.

There is no limit on the length of the string.

If a specific key is not supported on a platform, triggering that key is a no-op. A log message may be displayed.

Only F1 through F9 is supported. This is because there is no way to tell if `F12` is meant to be `F1+, F1-, 2+, 2-` or `F12+, F12-`.

Whether A on AZERTY keyboards triggers A or Q is undefined.

Anything to do with unsupported keyboard locales is undefined.

### Examples

| Sequence | String | Key Representation |
|---|---|---|
| Ctrl + H | `^h` | Ctrl+, h+, h-, Ctrl- |
| Ctrl + H + H | `^hh` | Ctrl+, h+, h-, h+, h-, Ctrl- |
| Ctrl + H twice | `^h ^h` | Ctrl+, h+, h-, Ctrl-, Ctrl+, h+, h-, Ctrl- |
| Ctrl + Alt + Delete | `^*DEL` | Ctrl+, Alt+, DEL+, DEL-, Ctrl-, Alt- |
| Alt + Enter | `*ENTER` | Alt+, ENTER+, ENTER-, Alt- |
| H + E + L + P | `help` | h+, h-, e+, e-, l+, l-, p+, p- |
| E + N + D | `end` | e+, e-, n+, n-, d+, d- |
| END Button | `END` | END+, END- |
| Alt + Shift + ENTER | `*>ENTER` | Alt+, Shift+, ENTER+, ENTER-, Alt-, Shift- |
| Alt + Tab + G + G + ENTER + Alt + Tab | `*TAB gg ENTER *TAB` | Alt+, Tab+, Tab-, Alt-, g+, g-, g+, g-, ENTER+, ENTER-, Alt+, Tab+, Tab-, Alt- |


