#pragma once

#include <openvr.h>
#include "ivrinput_action.h"
#include "ivrinput_manifest.h"
#include "ivrinput_action_set.h"

namespace input
{
class SteamIVRInput
{
public:
    SteamIVRInput();
    void Loop();
    bool nextSong();

    // Destructor. There are no terminating calls for the IVRInput API, so it
    // is left blank.
    ~SteamIVRInput() {}

    // These have been explicitly deleted to make sure there are no attempts at
    // copying the class in weird ways. It is not worth defining what should
    // happen on copy because it simply shouldn't be done.
    // They are declared public because compilers will give wrong error messages
    // if they are private.
    // Copy constructor
    SteamIVRInput( const SteamIVRInput& ) = delete;
    // Copy assignment
    SteamIVRInput& operator=( const SteamIVRInput& ) = delete;
    // Move constructor
    SteamIVRInput( SteamIVRInput&& ) = delete;
    // Move assignment
    SteamIVRInput& operator=( const SteamIVRInput&& ) = delete;

private:
    Manifest m_manifest;
    ActionSet m_mainSet;
    vr::VRActiveActionSet_t m_activeActionSet = {};

    Action m_nextTrack;
};

namespace input_strings
{
    constexpr auto k_actionNextTrack = "/actions/main/in/PlayNextTrack";
    constexpr auto k_setMain = "/actions/main";

} // namespace input_strings

} // namespace input
