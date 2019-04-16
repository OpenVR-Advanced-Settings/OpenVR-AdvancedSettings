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
constexpr auto noModifier = 0;
void sendKeyPress( const KeySym keySymbol, const KeySym modifierSymbol )
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
    sendKeyPress( XK_Return, noModifier );
}

void sendKeyboardBackspace( const int count )
{
    for ( int i = 0; i < count; ++i )
    {
        sendKeyPress( XK_BackSpace, noModifier );
    }
}

void sendKeyboardAltTab()
{
    sendKeyPress( XK_Tab, XK_Alt_L );
}

void sendKeyboardAltEnter()
{
    sendKeyPress( XK_Return, XK_Alt_L );
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
