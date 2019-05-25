## Sending keypresses from VR to the desktop

The `Keyboard Shortcut One`, `Keyboard Shortcut Two` and `Keyboard Shortcut Three` actions under the `misc` tab will send the currently configured key sequence to the operating system.

The default keybinding for all three actions is `Ctrl+Shift+M`, which mutes your microphone in Discord (unless you've changed it).
This sequence can be changed to anything you want, as described in the [spec](specs/Keyboard_Manager-Parser_Spec.txt). An easier to read version of the spec can be found below.

The hotkeys can for example be configured to run an AutoHotKey script or mute VOIP applications.

### Changing the key sequences

Currently, the only way to change which sequences are run when the bindings are activated is directly through the settings file.

On Windows the settings file can be found in `%APPDATA%\AdvancedSettings-Team` (copy paste this into Explorer or the Run command).

On Linux the settings file can be found in `/home/myname/.local/share/AdvancedSettings-Team/OpenVRAdvancedSettings/`.

The `OpenVRAdvancedSettings.ini` file will contain a heading like
```
[keyboardShortcuts]
keyboardOne=^>m
keyboardTwo=^>m
keyboardThree=^>m
```
The character the _after_ the `=` will be run when the related binding is activated. The above key sequence for all bindings is `^>m` (`Ctrl+Shift+M`).
An empty binding (no input will be run) would be `keyboardOne=`.

### Simple Configuration

Simply changing the last key of the default bindings should be enough for most people.
Remember that you will also need to set up the application (Discord, Mumble, TeamSpeak, etc.) to respond to the key sequence.
Single characters _must_ be lower case.
If you enter them as uppercase it will be treated as an error, and only correct keys up to the error point will be sent to the OS.

The default configuration is `^>m` for all bindings. Change the letter (`m`) to the character you want.
For example, if you want Keyboard Shortcut Two to be `Ctrl+Shift+X` you would write `keyboardTwo=^>x`.
If you just want it to be `Ctrl+X` you would remove the `>` symbol and just write `keyboardTwo=^m`.

### Advanced Configuration

Look at the key table [spec](specs/Keyboard_Manager-Parser_Spec.txt) and find the keys that you want to press.

Notice that single characters (a, b, c, etc.) are lower case, while buttons are uppercase (ENTER, END, ESC, etc.). 
If you enter single letters as lower case it will not work.

The modifier keys should come before the "real" (letters, numbers, ENTER, etc.).
`^m` (`Ctrl+M` (Ctrl is held down, M is pressed and released, Ctrl is released)) is _not_ the same as `m^` (`M+Ctrl` (M is pressed and released, Ctrl is pressed and released)).

Spaces will mean that all currently held modifier keys are released and that a new sequence is ready to be parsed. For example `^a ^a` would press `Ctrl+A` two times.
