#ifndef KEYBOARDINPUTWINDOWS_H
#define KEYBOARDINPUTWINDOWS_H

#include "../KeyboardInput.h"

// application namespace
namespace advsettings
{
class KeyboardInputWindows
{
public:
    void sendKeyboardInput( QString input );
    void sendKeyboardEnter();
    void sendKeyboardBackspace( const int count );
    void sendKeyboardAltTab();
    void sendKeyboardAltEnter();
    void sendMediaNextSong();
    void sendMediaPreviousSong();
    void sendMediaPausePlay();
    void sendMediaStopSong();
};

} // namespace advsettings

#endif // KEYBOARDINPUTWINDOWS_H
