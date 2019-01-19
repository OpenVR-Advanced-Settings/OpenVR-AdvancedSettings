#pragma once

#include <openvr.h>
#include "ivrinput/ivrinput_action.h"

class SteamIVRInput
{
public:
    // SteamIVRInput();
    void Init();
    void Loop();
    bool nextSongSet();

    // These have been explicitly deleted to make sure there are no attempts at
    // copying the class in weird ways. It is not worth defining what should
    // happen on copy because it simply shouldn't be done.
    // Destructor. There are not terminating calls for the IVRInput API, so it
    // is unneeded.
    //~SteamIVRInput() = delete;
    //// Copy constructor
    // SteamIVRInput( const SteamIVRInput& ) = delete;
    //// Copy assignment
    // SteamIVRInput& operator=( const SteamIVRInput& ) = delete;
    //// Move constructor
    // SteamIVRInput( SteamIVRInput&& ) = delete;
    //// Move assignment
    // SteamIVRInput& operator=( const SteamIVRInput&& ) = delete;

private:
    vr::VRActionHandle_t m_nextSongHandler = {};
    vr::VRActionSetHandle_t m_mainSetHandler = {};
    vr::VRActiveActionSet_t m_activeActionSet = {};
    vr::InputDigitalActionData_t m_nextSongData = {};
};

namespace input_strings
{
constexpr auto k_actionNextTrack = "/actions/main/in/PlayNextTrack";
constexpr auto k_setMain = "/actions/main";

} // namespace input_strings
