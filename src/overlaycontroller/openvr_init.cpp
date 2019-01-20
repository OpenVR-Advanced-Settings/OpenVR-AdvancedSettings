#include <string>
#include <openvr.h>
#include <QMessageBox>
#include "openvr_init.h"
#include <easylogging++.h>
#include "../ivrinput/ivrinput_manifest.h"

namespace openvr_init
{
void initializeProperly()
{
    auto initError = vr::VRInitError_None;
    vr::VR_Init( &initError, vr::VRApplication_Overlay );
    if ( initError != vr::VRInitError_None )
    {
        if ( initError == vr::VRInitError_Init_HmdNotFound
             || initError == vr::VRInitError_Init_HmdNotFoundPresenceFailed )
        {
            QMessageBox::critical( nullptr,
                                   "OpenVR Advanced Settings Overlay",
                                   "Could not find HMD!" );
        }
        throw std::runtime_error(
            std::string( "Failed to initialize OpenVR: " )
            + std::string(
                  vr::VR_GetVRInitErrorAsEnglishDescription( initError ) ) );
    }
    else
    {
        LOG( INFO ) << "OpenVR initialized successfully.";
    }
}

/*!
Initializes OpenVR to be in the bindings menu and then exits.
This is needed because all applications initialized with OpenVR that aren't of
type vr::VRApplication_Scene do not show up the in bindings menu.

This is obviously an ugly hack and should be done away with as soon as Valve
fixes this issue.

Official Issues for this:
https://github.com/ValveSoftware/openvr/issues/914
https://github.com/ValveSoftware/openvr/issues/926
https://github.com/ValveSoftware/openvr/issues/879

You can also make it appear by changing the
C:\Program Files (x86)\Steam\config\vrappconfig\*.vrappconfig file for your
project to have a "last_launch_time" of something greater than zero.
From experimentation it seems that there's a cutoff time around 2 weeks before
the current time. Setting it to the day before seems to be a good bet. (In unix
time).
*/
[[noreturn]] void openBindingsMenu()
{
    // Prefix the logs with this to visibly document that this isn't how things
    // are supposed to be. Hopefully someone reads the logs and comes here after
    // Valve has fixed it.
    constexpr auto logPrefix = "[HACK]::openBindingsMenu ";

    auto initError = vr::VRInitError_None;
    LOG( INFO ) << logPrefix << "Initializing application as scene.";

    vr::VR_Init( &initError, vr::VRApplication_Scene );

    if ( initError != vr::VRInitError_None )
    {
        LOG( ERROR ) << logPrefix << "Error initializing application as scene.";
        exit( 2 );
    }
    else
    {
        LOG( INFO ) << logPrefix << "Application initialized as scene.";
    }

    // This needs to be initialized, otherwise the bindings won't show up in the
    // menu.
    input::Manifest manifest;

    // SteamVR just needs a good enough time to start up comfortably and for the
    // user to get into the bindings menu.
    while ( true )
    {
    }
}

OpenVR_Init::OpenVR_Init()
{
    initializeProperly();

    // The function call and error message was the same for all version checks.
    // Specific error messages are unlikely to be necessary since both the type
    // and version are in the string and will be output.
    auto reportVersionError = []( const char* const interfaceAndVersion ) {
        QMessageBox::critical(
            nullptr,
            "OpenVR Advanced Settings Overlay",
            "OpenVR version is too outdated. Please update OpenVR." );
        throw std::runtime_error(
            std::string( "OpenVR version is too outdated: Interface version " )
            + std::string( interfaceAndVersion )
            + std::string( " not found." ) );
    };

    // Check whether OpenVR is too outdated
    if ( !vr::VR_IsInterfaceVersionValid( vr::IVRSystem_Version ) )
    {
        reportVersionError( vr::IVRSystem_Version );
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRSettings_Version ) )
    {
        reportVersionError( vr::IVRSettings_Version );
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVROverlay_Version ) )
    {
        reportVersionError( vr::IVROverlay_Version );
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRApplications_Version ) )
    {
        reportVersionError( vr::IVRApplications_Version );
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRChaperone_Version ) )
    {
        reportVersionError( vr::IVRChaperone_Version );
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRChaperoneSetup_Version ) )
    {
        reportVersionError( vr::IVRChaperoneSetup_Version );
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRCompositor_Version ) )
    {
        reportVersionError( vr::IVRCompositor_Version );
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRNotifications_Version ) )
    {
        reportVersionError( vr::IVRNotifications_Version );
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRInput_Version ) )
    {
        reportVersionError( vr::IVRInput_Version );
    }
}

OpenVR_Init::~OpenVR_Init()
{
    vr::VR_Shutdown();
}
} // namespace openvr_init
