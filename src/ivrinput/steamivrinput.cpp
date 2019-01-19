#include "steamivrinput.h"
#include <openvr.h>
#include <iostream>
#include <QStandardPaths>
#include <easylogging++.h>

namespace input
{
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

bool isDigitalActionActivatedOnce( Action& action )
{
    const auto handleData = getDigitalActionData( action );

    return handleData.bState && handleData.bChanged;
}

bool isDigitalActionActivatedConstant( Action& action )
{
    const auto handleData = getDigitalActionData( action );

    return handleData.bState;
}

SteamIVRInput::SteamIVRInput()
    : m_manifest(), m_mainSet( input_strings::k_setMain ),
      m_nextTrack( input_strings::k_actionNextTrack, ActionType::Digital )
{
    m_activeActionSet.ulActionSet = m_mainSet.handle();
    m_activeActionSet.ulRestrictedToDevice = vr::k_ulInvalidInputValueHandle;
    m_activeActionSet.nPriority = 0;
}

bool SteamIVRInput::nextSong()
{
    return isDigitalActionActivatedOnce( m_nextTrack );
}

void SteamIVRInput::Loop()
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
