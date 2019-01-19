#include <string>
#include <openvr.h>
#include <QMessageBox>
#include "openvr_init.h"

OpenVR_Init::OpenVR_Init()
{
    // Loading the OpenVR Runtime
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
}

OpenVR_Init::~OpenVR_Init()
{
    vr::VR_Shutdown();
}
