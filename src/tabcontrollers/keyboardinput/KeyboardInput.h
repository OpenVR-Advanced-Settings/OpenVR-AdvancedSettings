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
void sendMediaNextSong();
void sendMediaPreviousSong();
void sendMediaPausePlay();
void sendMediaStopSong();

} // namespace keyboardinput
