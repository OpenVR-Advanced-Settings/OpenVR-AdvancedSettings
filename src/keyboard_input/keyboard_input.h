#pragma once

#include <vector>
#include <QString>

namespace keyboardinput
{
/*!
Represents the state of a keyboard button press. Can be either Up or Down.

Implemented because neither Qt nor Windows.h included a sensible key state enum.
*/
enum class KeyStatus
{
    Up,
    Down,
};

void sendKeyboardInput( QString input );
void sendKeyboardEnter();
void sendKeyboardBackspace( const int count );
void sendKeyboardAltTab();
void sendKeyboardAltEnter();
void sendKeyboardCtrlC();
void sendKeyboardCtrlV();

// the following functions are used for debug commands in VRChat
// I (Kung) wanted them personally because they're very hard to
// hit from in VR, but it's a narrow use case for a specific app
// so for now these will only be enabled in the UI via manually
// adding vrcDebug = true to [utilitiesSettings] in
// OpenVRAdvancedSettings.ini

// These just call a thread
void sendKeyboardRShiftTilde1();
void sendKeyboardRShiftTilde2();
void sendKeyboardRShiftTilde3();
void sendKeyboardRShiftTilde4();
void sendKeyboardRShiftTilde5();
void sendKeyboardRShiftTilde6();
void sendKeyboardRShiftTilde7();
void sendKeyboardRShiftTilde8();
void sendKeyboardRShiftTilde9();
void sendKeyboardRShiftTilde0();

// These should be run after a slight delay
void sendKeyboardRShiftTilde1Delayed();
void sendKeyboardRShiftTilde2Delayed();
void sendKeyboardRShiftTilde3Delayed();
void sendKeyboardRShiftTilde4Delayed();
void sendKeyboardRShiftTilde5Delayed();
void sendKeyboardRShiftTilde6Delayed();
void sendKeyboardRShiftTilde7Delayed();
void sendKeyboardRShiftTilde8Delayed();
void sendKeyboardRShiftTilde9Delayed();
void sendKeyboardRShiftTilde0Delayed();

} // namespace keyboardinput
