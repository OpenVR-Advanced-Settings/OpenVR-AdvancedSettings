## Sending keypresses from VR to the desktop

The `Keyboard Shortcut One`, `Keyboard Shortcut Two` and `Keyboard Shortcut Three` actions under the `misc` tab will send the currently configured key sequence to the operating system.

The default keybinding for all three actions is `Ctrl+Shift+M`, which mutes your microphone in Discord (unless you've changed it).
This sequence can be changed to anything you want, as described in the [spec](specs/Keyboard_Manager-Parser_Spec.md).
An easier to read version of the spec can be found below.

The hotkeys can for example be configured to run an AutoHotKey script or mute VOIP applications.

### Changing the key sequences

Currently, the only way to change which sequences are run when the bindings are activated is directly through the settings file.

On Windows the settings file can be found in `%APPDATA%\AdvancedSettings-Team` (copy paste this into Explorer or the Run command).

On Linux the settings file can be found in `~/.config/AdvancedSettings-Team/`.

The `OpenVRAdvancedSettings.ini` file will contain a heading like
```
[keyboardShortcuts]
keyboardOne=^>m
keyboardTwo=^>m
keyboardThree=^>m
```
The character _after_ the `=` will be run when the related binding is activated.
The above key sequence for all bindings is `^>m` (`Ctrl+Shift+M`).
An empty binding (no input will be run) would be `keyboardOne=`.

### Simple Configuration

Simply changing the last key of the default bindings should be enough for most people.
Remember that you will also need to set up the application (Discord, Mumble, TeamSpeak, etc.) to respond to the key sequence.
Single characters _must_ be lower case.
If you enter them as uppercase it will be treated as an error, and only correct keys up to the error point will be sent to the OS.

The default configuration is `^>m` for all bindings. Change the letter (`m`) to the character you want.
For example, if you want Keyboard Shortcut Two to be `Ctrl+Shift+X` you would write `keyboardTwo=^>x`.
If you just want it to be `Ctrl+X` you would remove the `>` symbol and just write `keyboardTwo=^m`.

Notice that all pressed keys are released again when the sequence is over. 
It is not possible to hold down keys.

### Advanced Configuration

Look at the key table [spec](specs/Keyboard_Manager-Parser_Spec.md) and find the keys that you want to press.

Notice that single characters (a, b, c, etc.) are lower case, while buttons are uppercase (ENTER, END, ESC, etc.). 
If you enter single letters as upper case it will not work.
Buttons are pressed and released immediately, modifiers are held until there's a space in the sequence or the sequence ends.

The modifier keys should come before the "real" button inputs.
`^m` (`Ctrl+M` (Ctrl is held down, M is pressed and released, Ctrl is released)) is _not_ the same as `m^` (`M+Ctrl` (M is pressed and released, Ctrl is pressed and released)).

Spaces mean that all currently held modifier keys are released and that a new sequence is ready to be parsed. For example `^a ^a` would press `Ctrl+A` two times.
