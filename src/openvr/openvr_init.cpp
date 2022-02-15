#include <string>
#include <openvr.h>
#include <QMessageBox>
#include "openvr_init.h"
#include <easylogging++.h>
#include "../openvr/ivrinput_manifest.h"

namespace openvr_init
{
void initializeProperly( const OpenVrInitializationType initType )
{
    auto initializationType = vr::EVRApplicationType::VRApplication_Other;
    if ( initType == OpenVrInitializationType::Overlay )
    {
        initializationType = vr::EVRApplicationType::VRApplication_Overlay;
    }
    else if ( initType == OpenVrInitializationType::Utility )
    {
        initializationType = vr::EVRApplicationType::VRApplication_Utility;
    }

    auto initError = vr::VRInitError_None;
    vr::VR_Init( &initError, initializationType );
    if ( initError != vr::VRInitError_None )
    {
        if ( initError == vr::VRInitError_Init_HmdNotFound
             || initError == vr::VRInitError_Init_HmdNotFoundPresenceFailed )
        {
            QMessageBox::critical( nullptr,
                                   "OpenVR Advanced Settings Overlay",
                                   "Could not find HMD!" );
        }
        LOG( ERROR ) << "Failed to initialize OpenVR: "
                            + std::string(
                                vr::VR_GetVRInitErrorAsEnglishDescription(
                                    initError ) );
        exit( EXIT_FAILURE );
    }
    else
    {
        LOG( INFO ) << "OpenVR initialized successfully.";
    }
}

void initializeOpenVR( const OpenVrInitializationType initType )
{
    initializeProperly( initType );

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

} // namespace openvr_init
