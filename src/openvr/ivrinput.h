#pragma once

#include <openvr.h>
#include "ivrinput_action.h"
#include "ivrinput_manifest.h"
#include "ivrinput_action_set.h"
#include "ivrinput_input_source.h"

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
    bool leftHandSpaceTurn();
    bool rightHandSpaceTurn();
    bool leftHandSpaceDrag();
    bool rightHandSpaceDrag();
    bool optionalOverrideLeftHandSpaceTurn();
    bool optionalOverrideRightHandSpaceTurn();
    bool optionalOverrideLeftHandSpaceDrag();
    bool optionalOverrideRightHandSpaceDrag();
    bool swapSpaceDragToLeftHandOverride();
    bool swapSpaceDragToRightHandOverride();
    bool gravityToggle();
    bool gravityReverse();
    bool heightToggle();
    bool resetOffsets();
    bool snapTurnLeft();
    bool snapTurnRight();
    bool xAxisLockToggle();
    bool yAxisLockToggle();
    bool zAxisLockToggle();

    bool pushToTalk();

    bool proxIsActive();
    vr::VRActionHandle_t leftHapticActionHandle();
    vr::VRActionHandle_t rightHapticActionHandle();

    vr::VRInputValueHandle_t rightInputHandle();
    vr::VRInputValueHandle_t leftInputHandle();

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
    DigitalAction m_nextTrack;
    DigitalAction m_previousTrack;
    DigitalAction m_pausePlayTrack;
    DigitalAction m_stopTrack;

    // Space bindings
    DigitalAction m_leftHandSpaceTurn;
    DigitalAction m_rightHandSpaceTurn;
    DigitalAction m_leftHandSpaceDrag;
    DigitalAction m_rightHandSpaceDrag;
    DigitalAction m_optionalOverrideLeftHandSpaceTurn;
    DigitalAction m_optionalOverrideRightHandSpaceTurn;
    DigitalAction m_optionalOverrideLeftHandSpaceDrag;
    DigitalAction m_optionalOverrideRightHandSpaceDrag;
    DigitalAction m_swapSpaceDragToLeftHandOverride;
    DigitalAction m_swapSpaceDragToRightHandOverride;
    DigitalAction m_gravityToggle;
    DigitalAction m_gravityReverse;
    DigitalAction m_heightToggle;
    DigitalAction m_resetOffsets;
    DigitalAction m_snapTurnLeft;
    DigitalAction m_snapTurnRight;
    DigitalAction m_xAxisLockToggle;
    DigitalAction m_yAxisLockToggle;
    DigitalAction m_zAxisLockToggle;

    // Push To Talk
    DigitalAction m_pushToTalk;

    // haptic bindings
    DigitalAction m_leftHaptic;
    DigitalAction m_rightHaptic;

    // input sources
    InputSource m_leftHand;
    InputSource m_rightHand;
};

/*!
The IVRInput system is _very_ finnicky about strings matching and if they don't
match there are usually no errors logged reflecting that. constexpr auto strings
should be used to refer to all action manifest strings.
*/
namespace action_keys
{
    constexpr auto k_actionNextTrack = "/actions/main/in/NextTrack";
    constexpr auto k_actionPreviousTrack = "/actions/main/in/PreviousTrack";
    constexpr auto k_actionPausePlayTrack = "/actions/main/in/PausePlayTrack";
    constexpr auto k_actionStopTrack = "/actions/main/in/StopTrack";

    constexpr auto k_actionLeftHandSpaceTurn
        = "/actions/main/in/LeftHandSpaceTurn";
    constexpr auto k_actionRightHandSpaceTurn
        = "/actions/main/in/RightHandSpaceTurn";
    constexpr auto k_actionLeftHandSpaceDrag
        = "/actions/main/in/LeftHandSpaceDrag";
    constexpr auto k_actionRightHandSpaceDrag
        = "/actions/main/in/RightHandSpaceDrag";
    constexpr auto k_actionOptionalOverrideLeftHandSpaceTurn
        = "/actions/main/in/OptionalOverrideLeftHandSpaceTurn";
    constexpr auto k_actionOptionalOverrideRightHandSpaceTurn
        = "/actions/main/in/OptionalOverrideRightHandSpaceTurn";
    constexpr auto k_actionOptionalOverrideLeftHandSpaceDrag
        = "/actions/main/in/OptionalOverrideLeftHandSpaceDrag";
    constexpr auto k_actionOptionalOverrideRightHandSpaceDrag
        = "/actions/main/in/OptionalOverrideRightHandSpaceDrag";
    constexpr auto k_actionSwapSpaceDragToLeftHandOverride
        = "/actions/main/in/SwapSpaceDragToLeftHandOverride";
    constexpr auto k_actionSwapSpaceDragToRightHandOverride
        = "/actions/main/in/SwapSpaceDragToRightHandOverride";
    constexpr auto k_actionGravityToggle = "/actions/main/in/GravityToggle";
    constexpr auto k_actionGravityReverse = "/actions/main/in/GravityReverse";
    constexpr auto k_actionHeightToggle = "/actions/main/in/HeightToggle";
    constexpr auto k_actionResetOffsets = "/actions/main/in/ResetOffsets";
    constexpr auto k_actionSnapTurnLeft = "/actions/main/in/SnapTurnLeft";
    constexpr auto k_actionSnapTurnRight = "/actions/main/in/SnapTurnRight";
    constexpr auto k_actionXAxisLockToggle = "/actions/main/in/XAxisLockToggle";
    constexpr auto k_actionYAxisLockToggle = "/actions/main/in/YAxisLockToggle";
    constexpr auto k_actionZAxisLockToggle = "/actions/main/in/ZAxisLockToggle";

    constexpr auto k_actionPushToTalk = "/actions/main/in/PushToTalk";

    constexpr auto k_actionHapticsLeft = "/actions/main/out/HapticsLeft";
    constexpr auto k_actionHapticsRight = "/actions/main/out/HapticsRight";

    constexpr auto k_actionProxSensor = "/actions/main/in/ProxSensor";

} // namespace action_keys

/*!
Keys to get input source handles (things like hmd controllers etc.)
*/
namespace input_keys
{
    constexpr auto k_inputSourceLeft = "/user/hand/left";
    constexpr auto k_inputSourceRight = "/user/hand/right";
} // namespace input_keys

/*!
Keys for different action sets
*/
namespace action_sets
{
    constexpr auto main = "/actions/main";
} // namespace action_sets

} // namespace input
