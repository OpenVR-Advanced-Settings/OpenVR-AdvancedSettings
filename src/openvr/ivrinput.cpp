#include "ivrinput.h"
#include <openvr.h>
#include <iostream>
#include <QStandardPaths>
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
vr::InputDigitalActionData_t getDigitalActionData( Action& action )
{
    if ( action.type() != ActionType::Digital )
    {
        LOG( ERROR )
            << "Action was passed to IVRInput getDigitalActionData without "
               "being a digital type. Action: "
            << action.name();

        throw std::runtime_error(
            "Action was passed to IVRInput getDigitalActionData without being "
            "a digital type. See log for details." );
    }

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
vr::InputAnalogActionData_t getAnalogActionData( Action& action )
{
    if ( action.type() != ActionType::Analog )
    {
        LOG( ERROR )
            << "Action was passed to IVRInput getAnalogActionData without "
               "being an analog type. Action: "
            << action.name();

        throw std::runtime_error(
            "Action was passed to IVRInput getAnalogActionData without being "
            "an analog type. See log for details." );
    }

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
bool isDigitalActionActivatedOnce( Action& action )
{
    const auto handleData = getDigitalActionData( action );

    return handleData.bState && handleData.bChanged;
}

/*!
Gets the action state of an Action.
This will continually return true while the button is held down.
*/
bool isDigitalActionActivatedConstant( Action& action )
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
    : m_manifest(), m_mainSet( action_sets::k_setMain ),
      m_nextTrack( action_keys::k_actionNextTrack, ActionType::Digital ),
      m_previousTrack( action_keys::k_actionPreviousTrack,
                       ActionType::Digital ),
      m_pausePlayTrack( action_keys::k_actionPausePlayTrack,
                        ActionType::Digital ),
      m_stopTrack( action_keys::k_actionStopTrack, ActionType::Digital ),
      m_leftHaptic( action_keys::k_actionHapticsLeft, ActionType::Haptic ),
      m_rightHaptic( action_keys::k_actionHapticsRight, ActionType::Haptic ),
      m_rightHand( input_keys::k_inputSourceRight ),
      m_leftHand( input_keys::k_inputSourceLeft ),
      m_leftHandSpaceTurn( action_keys::k_actionLeftHandSpaceTurn,
                           ActionType::Digital ),
      m_rightHandSpaceTurn( action_keys::k_actionRightHandSpaceTurn,
                            ActionType::Digital ),
      m_leftHandSpaceDrag( action_keys::k_actionLeftHandSpaceDrag,
                           ActionType::Digital ),
      m_rightHandSpaceDrag( action_keys::k_actionRightHandSpaceDrag,
                            ActionType::Digital ),
      m_optionalOverrideLeftHandSpaceTurn(
          action_keys::k_actionOptionalOverrideLeftHandSpaceTurn,
          ActionType::Digital ),
      m_optionalOverrideRightHandSpaceTurn(
          action_keys::k_actionOptionalOverrideRightHandSpaceTurn,
          ActionType::Digital ),
      m_optionalOverrideLeftHandSpaceDrag(
          action_keys::k_actionOptionalOverrideLeftHandSpaceDrag,
          ActionType::Digital ),
      m_optionalOverrideRightHandSpaceDrag(
          action_keys::k_actionOptionalOverrideRightHandSpaceDrag,
          ActionType::Digital ),
      m_swapSpaceDragToLeftHandOverride(
          action_keys::k_actionSwapSpaceDragToLeftHandOverride,
          ActionType::Digital ),
      m_swapSpaceDragToRightHandOverride(
          action_keys::k_actionSwapSpaceDragToRightHandOverride,
          ActionType::Digital ),
      m_gravityToggle( action_keys::k_actionGravityToggle,
                       ActionType::Digital ),
      m_heightToggle( action_keys::k_actionHeightToggle,
                      ActionType::Digital ),
      m_resetOffsets( action_keys::k_actionResetOffsets,
                      ActionType::Digital ),
      m_snapTurnLeft( action_keys::k_actionSnapTurnLeft,
                      ActionType::Digital ),
      m_snapTurnRight( action_keys::k_actionSnapTurnRight,
                       ActionType::Digital ),
      m_xAxisLockToggle( action_keys::k_actionXAxisLockToggle,
                         ActionType::Digital ),
      m_yAxisLockToggle( action_keys::k_actionYAxisLockToggle,
                         ActionType::Digital ),
      m_zAxisLockToggle( action_keys::k_actionZAxisLockToggle,
                         ActionType::Digital ),
      m_pushToTalk( action_keys::k_actionPushToTalk, ActionType::Digital )
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
