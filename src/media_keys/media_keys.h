#pragma once
#include <easylogging++.h>
#include <vector>
#include <QString>

namespace keyboardinput
{
/*!
Represents the state of a keyboard button press. Can be either Up or Down.

Implemented because neither Qt nor Windows.h included a sensible key state enum.
*/
void sendMediaNextSong();
void sendMediaPreviousSong();
void sendMediaPausePlay();
void sendMediaStopSong();

} // namespace keyboardinput
