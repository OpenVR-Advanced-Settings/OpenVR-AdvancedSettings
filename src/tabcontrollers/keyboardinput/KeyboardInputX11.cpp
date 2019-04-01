#include "KeyboardInput.h"
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>

// Used to get the compiler to shut up about C4100: unreferenced formal
// parameter. The cast is to get GCC to shut up about it.
#define UNREFERENCED_PARAMETER( P ) static_cast<void>( ( P ) )

namespace keyboardinput
{
static void sendKeyPressAndRelease (const KeySym keySymbol, const KeySym modifierSymbol){
    Display *disp = XOpenDisplay (NULL);

    KeyCode keycode = 0;
    keycode = XKeysymToKeycode (disp, keySymbol);

    KeyCode modcode = 0;

    if (keycode == 0) {
        return;
    }

    XTestGrabControl (disp, True);

    /* Generate modkey press */
    if (modifierSymbol != 0) {
        modcode = XKeysymToKeycode(disp, modsym);
        XTestFakeKeyEvent (disp, modcode, True, 0);
    }
    /* Generate regular key press and release */
    XTestFakeKeyEvent (disp, keycode, True, 0);
    XTestFakeKeyEvent (disp, keycode, False, 0);

    /* Generate modkey release */
    if (modifierSymbol != 0) {
        XTestFakeKeyEvent (disp, modcode, False, 0);
    }

    XSync (disp, False);
    XTestGrabControl (disp, False);

    XCloseDisplay(disp);
}

void sendKeyboardInput( QString input )
{
    // noop
    UNREFERENCED_PARAMETER( input );
}

void sendKeyboardEnter()
{
    // noop
}

void sendKeyboardBackspace( const int count )
{
    // noop
    UNREFERENCED_PARAMETER( count );
}

void sendKeyboardAltTab()
{
    // noop
}

void sendKeyboardAltEnter()
{
    // noop
}

void sendMediaNextSong()
{
    // noop
}

void sendMediaPreviousSong()
{
    // noop
}

void sendMediaPausePlay()
{
    // noop
}

void sendMediaStopSong()
{
    // noop
}

} // namespace keyboardinput
