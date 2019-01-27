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
    bool previousSong();
    bool pausePlaySong();
    bool stopSong();
    bool leftHandPlayspaceRotate();
    bool rightHandPlayspaceRotate();
    bool leftHandPlayspaceMove();
    bool rightHandPlayspaceMove();
    bool optionalOverrideLeftHandPlayspaceRotate();
    bool optionalOverrideRightHandPlayspaceRotate();
    bool optionalOverrideLeftHandPlayspaceMove();
    bool optionalOverrideRightHandPlayspaceMove();

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

    // Music player bindings
    Action m_nextTrack;
    Action m_previousTrack;
    Action m_pausePlayTrack;
    Action m_stopTrack;

    // Playspace bindings
    Action m_leftHandPlayspaceRotate;
    Action m_rightHandPlayspaceRotate;
    Action m_leftHandPlayspaceMove;
    Action m_rightHandPlayspaceMove;
    Action m_optionalOverrideLeftHandPlayspaceRotate;
    Action m_optionalOverrideRightHandPlayspaceRotate;
    Action m_optionalOverrideLeftHandPlayspaceMove;
    Action m_optionalOverrideRightHandPlayspaceMove;
};

/*!
The IVRInput system is _very_ finnicky about strings matching and if they don't
match there are usually no errors logged reflecting that. constexpr auto strings
should be used to refer to all action manifest strings.
*/
namespace input_strings
{
    constexpr auto k_actionNextTrack = "/actions/main/in/NextTrack";
    constexpr auto k_actionPreviousTrack = "/actions/main/in/PreviousTrack";
    constexpr auto k_actionPausePlayTrack = "/actions/main/in/PausePlayTrack";
    constexpr auto k_actionStopTrack = "/actions/main/in/StopTrack";

    constexpr auto k_actionLeftHandPlayspaceRotate
        = "/actions/main/in/LeftHandPlayspaceRotate";
    constexpr auto k_actionRightHandPlayspaceRotate
        = "/actions/main/in/RightHandPlayspaceRotate";
    constexpr auto k_actionLeftHandPlayspaceMove
        = "/actions/main/in/LeftHandPlayspaceMove";
    constexpr auto k_actionRightHandPlayspaceMove
        = "/actions/main/in/RightHandPlayspaceMove";
    constexpr auto k_actionOptionalOverrideLeftHandPlayspaceRotate
        = "/actions/main/in/OptionalOverrideLeftHandPlayspaceRotate";
    constexpr auto k_actionOptionalOverrideRightHandPlayspaceRotate
        = "/actions/main/in/OptionalOverrideRightHandPlayspaceRotate";
    constexpr auto k_actionOptionalOverrideLeftHandPlayspaceMove
        = "/actions/main/in/OptionalOverrideLeftHandPlayspaceMove";
    constexpr auto k_actionOptionalOverrideRightHandPlayspaceMove
        = "/actions/main/in/OptionalOverrideRightHandPlayspaceMove";

    constexpr auto k_setMain = "/actions/main";

} // namespace input_strings

} // namespace input
