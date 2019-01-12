#ifndef KEYBOARDINPUTDUMMY_H
#define KEYBOARDINPUTDUMMY_H

#include "../KeyboardInput.h"

// application namespace
namespace advsettings
{
class KeyboardInputDummy : public KeyboardInput
{
public:
    virtual void sendKeyboardInput( QString input ) override;
    virtual void sendKeyboardEnter() override;
    virtual void sendKeyboardBackspace( const int count ) override;
    virtual void sendKeyboardAltTab() override;
    virtual void sendMediaNextSong() override;
    virtual void sendMediaPreviousSong() override;
    virtual void sendMediaPausePlay() override;
    virtual void sendMediaStopSong() override;
};

} // namespace advsettings

#endif // KEYBOARDINPUTDUMMY_H
