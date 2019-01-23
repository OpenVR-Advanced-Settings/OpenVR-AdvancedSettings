#pragma once

#include <vector>
#include <QString>

// application namespace
namespace advsettings
{
class AudioTabController;

class KeyboardInput
{
public:
    virtual ~KeyboardInput(){};

    virtual void sendKeyboardInput( QString input ) = 0;
    virtual void sendKeyboardEnter() = 0;
    virtual void sendKeyboardBackspace( const int count ) = 0;
    virtual void sendKeyboardAltTab() = 0;
    virtual void sendKeyboardAltEnter() = 0;
    virtual void sendMediaNextSong() = 0;
    virtual void sendMediaPreviousSong() = 0;
    virtual void sendMediaPausePlay() = 0;
    virtual void sendMediaStopSong() = 0;
};

} // namespace advsettings
