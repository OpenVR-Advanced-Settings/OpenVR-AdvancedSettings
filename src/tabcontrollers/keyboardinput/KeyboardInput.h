#pragma once

#include <vector>
#include <QString>

namespace keyboardinput
{
void sendKeyboardInput( QString input );
void sendKeyboardEnter();
void sendKeyboardBackspace( const int count );
void sendKeyboardAltTab();
void sendKeyboardAltEnter();
void sendMediaNextSong();
void sendMediaPreviousSong();
void sendMediaPausePlay();
void sendMediaStopSong();

} // namespace keyboardinput
