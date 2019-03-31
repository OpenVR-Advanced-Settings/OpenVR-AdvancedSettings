#include "KeyboardInputDummy.h"

// Used to get the compiler to shut up about C4100: unreferenced formal
// parameter. The cast is to get GCC to shut up about it.
#define UNREFERENCED_PARAMETER( P ) static_cast<void>( ( P ) )

namespace keyboardinput
{
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
