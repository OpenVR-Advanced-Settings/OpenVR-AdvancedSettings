#include "ivrinput.h"
#include "ivrinput_action.h"
#include <openvr.h>
#include <iostream>
#include <easylogging++.h>

namespace input
{
/*!
Wrapper around the IVRInput GetDigitalActionData with error handling.

The struct is created and zero initalized in this function instead of being
passed as reference since the size is currently only 28 bytes, and currently
only two bools are used from the entire struct.
MSVC on /O2 will likely completely optimize the function call away but even if
it isn't the struct will still need to be created somewhere, and with move
semantics it almost doesn't matter if the struct is created in the calling
function and passed as reference, or created in this function.
*/
vr::InputDigitalActionData_t getDigitalActionData( DigitalAction& action )
{
    vr::InputDigitalActionData_t handleData = {};

    const auto error = vr::VRInput()->GetDigitalActionData(
        action.handle(),
        &handleData,
        sizeof( handleData ),
        vr::k_ulInvalidInputValueHandle );

    if ( error != vr::EVRInputError::VRInputError_None )
    {
        LOG( ERROR ) << "Error getting IVRInput Digital Action Data for handle "
                     << action.name() << ". SteamVR Error: " << error;
    }

    return handleData;
}

/*!
Wrapper around the IVRInput GetAnalogActionData with error handling.

The struct is created and zero initalized in this function instead of being
passed as reference since the size is currently only 28 bytes, and currently
only two bools are used from the entire struct.
MSVC on /O2 will likely completely optimize the function call away but even if
it isn't the struct will still need to be created somewhere, and with move
semantics it almost doesn't matter if the struct is created in the calling
function and passed as reference, or created in this function.
*/
vr::InputAnalogActionData_t getAnalogActionData( AnalogAction& action )
{
    vr::InputAnalogActionData_t handleData = {};

    const auto error
        = vr::VRInput()->GetAnalogActionData( action.handle(),
                                              &handleData,
                                              sizeof( handleData ),
                                              vr::k_ulInvalidInputValueHandle );

    if ( error != vr::EVRInputError::VRInputError_None )
    {
        LOG( ERROR ) << "Error getting IVRInput Digital Action Data for handle "
                     << action.name() << ". SteamVR Error: " << error;
    }

    return handleData;
}

/*!
Gets the action state of an Action.
This will only return true the first time the button is released. It is
necessary to release the button and the push it again in order for this function
to return true again.
*/
bool isDigitalActionActivatedOnce( DigitalAction& action )
{
    const auto handleData = getDigitalActionData( action );

    return handleData.bState && handleData.bChanged;
}

/*!
Gets the action state of an Action.
This will continually return true while the button is held down.
*/
bool isDigitalActionActivatedConstant( DigitalAction& action )
{
    const auto handleData = getDigitalActionData( action );

    return handleData.bState;
}

/*!
Sets up the input systems.
Actions should be added to the initializer list before the body of the
constructor. Actions should be certain that the action name strings are
correctly input, and that the correct type is provided.
Wrong action names may not result in direct error messages, but just features
mystically not working.
*/
SteamIVRInput::SteamIVRInput()
    : m_manifest(), m_mainSet( action_sets::main ),
      m_nextTrack( action_keys::k_actionNextTrack ),
      m_previousTrack( action_keys::k_actionPreviousTrack ),
      m_pausePlayTrack( action_keys::k_actionPausePlayTrack ),
      m_stopTrack( action_keys::k_actionStopTrack ),
      m_leftHandSpaceTurn( action_keys::k_actionLeftHandSpaceTurn ),
      m_rightHandSpaceTurn( action_keys::k_actionRightHandSpaceTurn ),
      m_leftHandSpaceDrag( action_keys::k_actionLeftHandSpaceDrag ),
      m_rightHandSpaceDrag( action_keys::k_actionRightHandSpaceDrag ),
      m_optionalOverrideLeftHandSpaceTurn(
          action_keys::k_actionOptionalOverrideLeftHandSpaceTurn ),
      m_optionalOverrideRightHandSpaceTurn(
          action_keys::k_actionOptionalOverrideRightHandSpaceTurn ),
      m_optionalOverrideLeftHandSpaceDrag(
          action_keys::k_actionOptionalOverrideLeftHandSpaceDrag ),
      m_optionalOverrideRightHandSpaceDrag(
          action_keys::k_actionOptionalOverrideRightHandSpaceDrag ),
      m_swapSpaceDragToLeftHandOverride(
          action_keys::k_actionSwapSpaceDragToLeftHandOverride ),
      m_swapSpaceDragToRightHandOverride(
          action_keys::k_actionSwapSpaceDragToRightHandOverride ),
      m_gravityToggle( action_keys::k_actionGravityToggle ),
      m_gravityReverse( action_keys::k_actionGravityReverse ),
      m_heightToggle( action_keys::k_actionHeightToggle ),
      m_resetOffsets( action_keys::k_actionResetOffsets ),
      m_snapTurnLeft( action_keys::k_actionSnapTurnLeft ),
      m_snapTurnRight( action_keys::k_actionSnapTurnRight ),
      m_xAxisLockToggle( action_keys::k_actionXAxisLockToggle ),
      m_yAxisLockToggle( action_keys::k_actionYAxisLockToggle ),
      m_zAxisLockToggle( action_keys::k_actionZAxisLockToggle ),
      m_pushToTalk( action_keys::k_actionPushToTalk ),
      m_leftHaptic( action_keys::k_actionHapticsLeft ),
      m_rightHaptic( action_keys::k_actionHapticsRight ),
      m_leftHand( input_keys::leftHand ), m_rightHand( input_keys::rightHand )
{
    m_activeActionSet.ulActionSet = m_mainSet.handle();
    m_activeActionSet.ulRestrictedToDevice = vr::k_ulInvalidInputValueHandle;
    m_activeActionSet.nPriority = 0;
}
/*!
Returns true if the next media track should be played.

Will only return true the first time that the button is pressed. Holding the
button down will result in false until it has been released and pushed again.
*/
bool SteamIVRInput::nextSong()
{
    return isDigitalActionActivatedOnce( m_nextTrack );
}

/*!
Returns true if the previous media track should be played.

Will only return true the first time that the button is pressed. Holding the
button down will result in false until it has been released and pushed again.
*/
bool SteamIVRInput::previousSong()
{
    return isDigitalActionActivatedOnce( m_previousTrack );
}

/*!
Returns true if media playback should be paused/played.

Will only return true the first time that the button is pressed. Holding the
button down will result in false until it has been released and pushed again.
*/
bool SteamIVRInput::pausePlaySong()
{
    return isDigitalActionActivatedOnce( m_pausePlayTrack );
}

/*!
Returns true if media playback should be stopped.

Will only return true the first time that the button is pressed. Holding the
button down will result in false until it has been released and pushed again.
*/
bool SteamIVRInput::stopSong()
{
    return isDigitalActionActivatedOnce( m_stopTrack );
}

bool SteamIVRInput::leftHandSpaceTurn()
{
    return isDigitalActionActivatedConstant( m_leftHandSpaceTurn );
}

bool SteamIVRInput::rightHandSpaceTurn()
{
    return isDigitalActionActivatedConstant( m_rightHandSpaceTurn );
}

bool SteamIVRInput::leftHandSpaceDrag()
{
    return isDigitalActionActivatedConstant( m_leftHandSpaceDrag );
}

bool SteamIVRInput::rightHandSpaceDrag()
{
    return isDigitalActionActivatedConstant( m_rightHandSpaceDrag );
}
bool SteamIVRInput::optionalOverrideLeftHandSpaceTurn()
{
    return isDigitalActionActivatedConstant(
        m_optionalOverrideLeftHandSpaceTurn );
}

bool SteamIVRInput::optionalOverrideRightHandSpaceTurn()
{
    return isDigitalActionActivatedConstant(
        m_optionalOverrideRightHandSpaceTurn );
}

bool SteamIVRInput::optionalOverrideLeftHandSpaceDrag()
{
    return isDigitalActionActivatedConstant(
        m_optionalOverrideLeftHandSpaceDrag );
}

bool SteamIVRInput::optionalOverrideRightHandSpaceDrag()
{
    return isDigitalActionActivatedConstant(
        m_optionalOverrideRightHandSpaceDrag );
}

bool SteamIVRInput::swapSpaceDragToLeftHandOverride()
{
    return isDigitalActionActivatedConstant(
        m_swapSpaceDragToLeftHandOverride );
}

bool SteamIVRInput::swapSpaceDragToRightHandOverride()
{
    return isDigitalActionActivatedConstant(
        m_swapSpaceDragToRightHandOverride );
}

bool SteamIVRInput::gravityToggle()
{
    return isDigitalActionActivatedOnce( m_gravityToggle );
}

bool SteamIVRInput::gravityReverse()
{
    return isDigitalActionActivatedConstant( m_gravityReverse );
}

bool SteamIVRInput::heightToggle()
{
    return isDigitalActionActivatedOnce( m_heightToggle );
}

bool SteamIVRInput::resetOffsets()
{
    return isDigitalActionActivatedOnce( m_resetOffsets );
}

bool SteamIVRInput::snapTurnLeft()
{
    return isDigitalActionActivatedOnce( m_snapTurnLeft );
}

bool SteamIVRInput::snapTurnRight()
{
    return isDigitalActionActivatedOnce( m_snapTurnRight );
}

bool SteamIVRInput::xAxisLockToggle()
{
    return isDigitalActionActivatedOnce( m_xAxisLockToggle );
}

bool SteamIVRInput::yAxisLockToggle()
{
    return isDigitalActionActivatedOnce( m_yAxisLockToggle );
}

bool SteamIVRInput::zAxisLockToggle()
{
    return isDigitalActionActivatedOnce( m_zAxisLockToggle );
}

bool SteamIVRInput::pushToTalk()
{
    return isDigitalActionActivatedConstant( m_pushToTalk );
}

/*!
Returns the action handle of the Left Haptic Action
*/

vr::VRActionHandle_t SteamIVRInput::leftHapticActionHandle()
{
    return m_leftHaptic.handle();
}

/*!
Returns the action handle of the Right Haptic Action
*/
vr::VRActionHandle_t SteamIVRInput::rightHapticActionHandle()
{
    return m_rightHaptic.handle();
}

/*!
Returns the input handle of the Right hand
*/
vr::VRInputValueHandle_t SteamIVRInput::rightInputHandle()
{
    return m_rightHand.handle();
}

/*!
Returns the input handle of the Left hand
*/
vr::VRInputValueHandle_t SteamIVRInput::leftInputHandle()
{
    return m_leftHand.handle();
}

/*!
Updates the active action set(s).
Should be called every frame, or however often you want the input system to
update state.
*/
void SteamIVRInput::UpdateStates()
{
    constexpr auto numberOfSets = 1;

    const auto error = vr::VRInput()->UpdateActionState(
        &m_activeActionSet, sizeof( m_activeActionSet ), numberOfSets );

    if ( error != vr::EVRInputError::VRInputError_None )
    {
        LOG( ERROR )
            << "Error during IVRInput action state update. OpenVR Error: "
            << error;
    }
}

} // namespace input
