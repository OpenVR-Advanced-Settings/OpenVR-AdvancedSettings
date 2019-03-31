#ifndef KEYBOARDINPUTDUMMY_H
#define KEYBOARDINPUTDUMMY_H

#include "../KeyboardInput.h"

// application namespace
namespace advsettings
{
class KeyboardInputDummy
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

#endif // KEYBOARDINPUTDUMMY_H
