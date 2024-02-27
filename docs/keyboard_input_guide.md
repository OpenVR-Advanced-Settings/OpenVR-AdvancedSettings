# Keyboard Input Guide

## Capabilities
 We provide two distinct types of functionality, `Keyboard Shortcut`'s [macros] and `Key Press`'s [individual key press]

### Limitations
We are not designed to provide a whole bunch of functionality here, if you need a whole bunch of keys instead of 1 or 2 you may want to use another utility such as [OpenVR2Key](https://github.com/BOLL7708/OpenVR2Key) by Boll7708 on Github (no affiliation)

### Keyboard Shortcuts
 
- Sends One or Many keys to the OS like a macro.
- Cannot `hold` keys down
  - Except in the case of Modifiers (alt, ctrl, etc.)

### Key Press

- Sends One and only One key to the OS.
 - Extra Keys Will be Ignored
- Mirrors button state on controller
  - If you hold down `a` on a controller the `key` will be held down
  - If you release `a` on a controller the `key` will be released
- `Key Press System`
  - This Action will ALWAYS be use-able while OVRAS is running
- `Key Press Misc.`
  - This Action MAY not always be use-able depending on other Settings/Binds*
    - Pending a future update.

## How To Set

- Go to the Settings File
  - Windows: `%Appdata%\AdvancedSettings-Team` (`C:\Users\<username>\AppData\Roaming\AdvancedSettings-Team`)
  - Linux: `~/.config/AdvancedSettings-Team/`
- open `OVR Advanced Settings.ini` with a text editor
- Go to `[keyboardShortcuts]`
- Add your sequence right after the `=` for the action you want.
  - By Default Keyboard's are set to ctrl+shift+m `^>m`
  - By Default Key Press's are set to F9 `F9`
  - Example for ctrl+alt+delete for keyboardOne Action
    - `keyboardOne=^*DEL`
  - Example for Key Press System for a`
    - `keyPressSystem=a`
	
## Supported Keys

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
| Function keys F10-F19 | Capital G followed by number. `G0` = F10, `G1` = F11, ... |
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


## Examples

### Keyboard Examples

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


### Key Press Examples

| Sequence | String | Key Rep On Press | Key Rep on Release
|---|---|---|---|
| Ctrl | `^` | Ctrl+ | Ctrl- |
| H | `h` | h+ | h- |
| Ctrl + H | `^h` | Ctrl+ | Ctrl- |


## Common Issues

### Case Sensative
- basic characters are all LOWER case, while keys such as ENTER are all UPPER case.

### Custom Keys Not Saving.
- We only load keybinds from file on start-up, and save based on what is in memory (not in file)
  - **ONLY** edit the settings file while Advanced Settings is **not** running.

### Focus Issues
- we deliver the keypress to the OS, depending on your set-up and program you may have to have the window "in-focus"

### Multiple Key Events
- you must separate multiple events with a space (` `)
 - alt + tab, alt + F4 = `*TAB *F4`

## Parser Spec
[Parser Spec](https://github.com/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings/blob/master/docs/specs/Keyboard_Manager-Parser_Spec.md)
