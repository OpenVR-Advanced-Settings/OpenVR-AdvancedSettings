#pragma once

#include <openvr.h>
#include "ivrinput_action.h"
#include "ivrinput_manifest.h"
#include "ivrinput_action_set.h"

namespace input
{
/*!
Responsible for controller input.

UpdateStates should be called every frame.

Binary actions should only have one function,
and they should reflect the expected behaviour. I.e. if a button being pressed
and held should only return true the first time, that logic should be in the
function.

An action in the IVRInput API is entered in the actions manifest. This is a
.json file that is included with the final binary.
Currently an Action is created with a string and a type. The type determines
which function should be used for getting information from the API. The type in
the actions manifest must match the type in the source code.

Actions should be added as member objects to SteamIVRInput and initialized in
the constructor. They should then make a small number of accessor functions
available for outside use. The internal structs of the IVRInput API are not
entirely stable, and should not be leaked outside this class.

The name of the actions manifest must be set in ivrinput_manifest.h.
*/
class SteamIVRInput
{
public:
    SteamIVRInput();

    void UpdateStates();
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

/*!
The IVRInput system is _very_ finnicky about strings matching and if they don't
match there are usually no errors logged reflecting that. constexpr auto strings
should be used to refer to all action manifest strings.
*/
namespace input_strings
{
    constexpr auto k_actionNextTrack = "/actions/main/in/PlayNextTrack";
    constexpr auto k_setMain = "/actions/main";

} // namespace input_strings

} // namespace input
