#include "steamivrinput.h"
#include <openvr.h>
#include <iostream>

void SteamIVRInput::Init( const bool init )
{
    if ( init )
    {
        auto initError = vr::VRInitError_None;
        vr::VR_Init( &initError, vr::VRApplication_Overlay );
        if ( initError != vr::VRInitError_None )
        {
            std::cout << "SteamVR init error.";
        }
    }

    auto error
        = vr::VRInput()->SetActionManifestPath( m_actionManifestPath.c_str() );
    if ( error != vr::EVRInputError::VRInputError_None )
    {
        std::cout << "Action manifest error\n";
    }

    // Get action handle
    error = vr::VRInput()->GetActionHandle( "/actions/main/in/PlayNextTrack",
                                            &m_nextSongHandler );
    if ( error != vr::EVRInputError::VRInputError_None )
    {
        std::cout << "Handle error.\n";
    }

    error = vr::VRInput()->GetActionSetHandle( "/actions/main",
                                               &m_mainSetHandler );
    if ( error != vr::EVRInputError::VRInputError_None )
    {
        std::cout << "Handle error.\n";
    }

    m_activeActionSet.ulActionSet = m_mainSetHandler;
    m_activeActionSet.ulRestrictedToDevice = vr::k_ulInvalidInputValueHandle;
    m_activeActionSet.nPriority = 0;
}

bool SteamIVRInput::nextSongSet()
{
    auto e = vr::VRInput()->GetDigitalActionData(
        m_nextSongHandler,
        &m_nextSongData,
        sizeof( m_nextSongData ),
        vr::k_ulInvalidInputValueHandle );

    if ( e != vr::EVRInputError::VRInputError_None )
    {
        std::cout << e;
        std::cout << "GetDigitalAction error.\n";
    }

    if ( m_nextSongData.bActive )
    {
        std::cout << "Action Set Active!\n";
    }

    return m_nextSongData.bState;
}

void SteamIVRInput::Loop()
{
    auto error = vr::VRInput()->UpdateActionState(
        &m_activeActionSet, sizeof( m_activeActionSet ), 1 );

    if ( error != vr::EVRInputError::VRInputError_None )
    {
        std::cout << "Loop error.\n";
    }
}
