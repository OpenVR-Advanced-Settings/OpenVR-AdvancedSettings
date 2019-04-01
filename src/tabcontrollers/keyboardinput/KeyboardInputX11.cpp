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
static void sendKeyPressAndRelease( const KeySym keySymbol,
                                    const KeySym modifierSymbol )
{
    Display* const display = XOpenDisplay( nullptr );

    const KeyCode keycode = XKeysymToKeycode( display, keySymbol );
    if ( keycode == 0 )
    {
        return;
    }

    XTestGrabControl( display, True );

    KeyCode modcode = 0;
    if ( modifierSymbol != 0 )
    {
        modcode = XKeysymToKeycode( display, modifierSymbol );
        XTestFakeKeyEvent( display, modcode, True, 0 );
    }

    XTestFakeKeyEvent( display, keycode, True, 0 );
    XTestFakeKeyEvent( display, keycode, False, 0 );

    if ( modifierSymbol != 0 )
    {
        XTestFakeKeyEvent( display, modcode, False, 0 );
    }

    XSync( display, False );
    XTestGrabControl( display, False );
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
    sendKeyPressAndRelease( XK_Tab, XK_Alt_L );
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
