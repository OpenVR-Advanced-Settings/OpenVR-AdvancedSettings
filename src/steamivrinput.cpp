#include "steamivrinput.h"
#include <openvr.h>
#include <iostream>
#include <QStandardPaths>
#include <easylogging++.h>

void setActionManifestPath( const QString actionManifestPath )
{
    auto error = vr::VRInput()->SetActionManifestPath(
        actionManifestPath.toStdString().c_str() );
    if ( error != vr::EVRInputError::VRInputError_None )
    {
        LOG( ERROR ) << "Error setting action manifest path: "
                     << actionManifestPath << ". OpenVR Error: " << error;
    }
}

vr::VRActionHandle_t getActionHandle( const char* const action )
{
    vr::VRActionHandle_t actionHandle = 0;

    auto error = vr::VRInput()->GetActionHandle( action, &actionHandle );

    if ( error != vr::EVRInputError::VRInputError_None )
    {
        LOG( ERROR ) << "Error getting handle for '" << action
                     << "'. OpenVR Error: " << error;
    }

    return actionHandle;
}

vr::VRActionSetHandle_t getActionSetHandle( const char* const set )
{
    vr::VRActionSetHandle_t setHandle = 0;

    auto error = vr::VRInput()->GetActionSetHandle( set, &setHandle );
    if ( error != vr::EVRInputError::VRInputError_None )
    {
        LOG( ERROR ) << "Error getting handle for '" << set
                     << "'. OpenVR Error: " << error;
    }

    return setHandle;
}

vr::InputDigitalActionData_t getDigitalActionData( vr::VRActionHandle_t handle )
{
    vr::InputDigitalActionData_t handleData = {};

    auto error = vr::VRInput()->GetDigitalActionData(
        handle,
        &handleData,
        sizeof( handleData ),
        vr::k_ulInvalidInputValueHandle );

    if ( error != vr::EVRInputError::VRInputError_None )
    {
        LOG( ERROR ) << "Error getting Digital Action Data for handle "
                     << handle << ". SteamVR Error: " << error;
    }

    return handleData;
}

bool isActionActivatedOnce( vr::VRActionHandle_t handle )
{
    auto handleData = getDigitalActionData( handle );

    return handleData.bState && handleData.bChanged;
}

void SteamIVRInput::Init()
{
    QString actionManifestPath
        = QStandardPaths::locate( QStandardPaths::AppDataLocation,
                                  QStringLiteral( "action_manifest.json" ) );
    setActionManifestPath( actionManifestPath );

    m_nextSongHandler = getActionHandle( input_strings::k_actionNextTrack );

    m_mainSetHandler = getActionSetHandle( input_strings::k_setMain );

    m_activeActionSet.ulActionSet = m_mainSetHandler;
    m_activeActionSet.ulRestrictedToDevice = vr::k_ulInvalidInputValueHandle;
    m_activeActionSet.nPriority = 0;
}

bool SteamIVRInput::nextSongSet()
{
    return isActionActivatedOnce( m_nextSongHandler );
}

void SteamIVRInput::Loop()
{
    auto error = vr::VRInput()->UpdateActionState(
        &m_activeActionSet, sizeof( m_activeActionSet ), 1 );

    if ( error != vr::EVRInputError::VRInputError_None )
    {
        LOG( ERROR ) << "Loop error.";
    }
}
