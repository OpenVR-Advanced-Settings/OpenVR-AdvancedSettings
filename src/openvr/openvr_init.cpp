#include <string>
#include <openvr.h>
#include <QMessageBox>
#include "openvr_init.h"
#include <easylogging++.h>
#include "../openvr/ivrinput_manifest.h"

namespace openvr_init
{
bool initializeProperly( const OpenVrInitializationType initType )
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
            // In particular in some setups these errors are thrown while
            // nothing is wrong with their setup presumably this is some sort of
            // race condition
            LOG( WARNING ) << "HMD not Found During Startup";
            LOG( WARNING ) << "steamvr error: "
                                  + std::string(
                                      vr::VR_GetVRInitErrorAsEnglishDescription(
                                          initError ) );
            return false;
        }
        LOG( ERROR ) << "Failed to initialize OpenVR: "
                            + std::string(
                                vr::VR_GetVRInitErrorAsEnglishDescription(
                                    initError ) );
        // Going to stop Exiting App, This may lead to crashes if OpenVR
        // actually fails to start, HOWEVER based on the HMD errors we are
        // probably pre-maturely killing ourselves from some sort of OpenVR race
        // condition
        // exit( EXIT_FAILURE );
        return false;
    }

    LOG( INFO ) << "OpenVR initialized successfully.";
    return true;
}

void initializeOpenVR( const OpenVrInitializationType initType, int count )
{
    bool success = initializeProperly( initType );

    // The function call and error message was the same for all version checks.
    // Specific error messages are unlikely to be necessary since both the type
    // and version are in the string and will be output.
    auto reportVersionError
        = []( const char* const interfaceAndVersion, const int trynumber )
    {
        // as of 5.8.1 Based on some information from valve, if ANY interface
        // fails to load we should have issues. we will re-initialize a few
        // times if that is the case and hope that fixes things
        LOG( WARNING ) << "OpenVR version is invalid: Interface version "
                       << interfaceAndVersion << " not found. attempt number: "
                       << std::to_string( trynumber );
    };

    // Check whether OpenVR is too outdated

    // Sleep duration is an attempt to hopefully alleviate a possible race
    // condition in steamvr
    if ( !vr::VR_IsInterfaceVersionValid( vr::IVRSystem_Version ) )
    {
        reportVersionError( vr::IVRSystem_Version, count );
        success = false;
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRSettings_Version ) )
    {
        reportVersionError( vr::IVRSettings_Version, count );
        success = false;
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVROverlay_Version ) )
    {
        reportVersionError( vr::IVROverlay_Version, count );
        success = false;
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRApplications_Version ) )
    {
        reportVersionError( vr::IVRApplications_Version, count );
        success = false;
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRChaperone_Version ) )
    {
        reportVersionError( vr::IVRChaperone_Version, count );
        success = false;
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRChaperoneSetup_Version ) )
    {
        reportVersionError( vr::IVRChaperoneSetup_Version, count );
        success = false;
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRCompositor_Version ) )
    {
        reportVersionError( vr::IVRCompositor_Version, count );
        success = false;
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRNotifications_Version ) )
    {
        reportVersionError( vr::IVRNotifications_Version, count );
        success = false;
    }
    else if ( !vr::VR_IsInterfaceVersionValid( vr::IVRInput_Version ) )
    {
        reportVersionError( vr::IVRInput_Version, count );
        success = false;
    }
    if ( !success && count < 3 )
    {
        LOG( WARNING ) << "An error occured on initialization of "
                          "openvr/steamvr attempting again "
                       << std::to_string( count + 1 ) << " of 3 Attempts";
        std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
        // 5.8.1 Unknown if VR shutdown needs to be callled if vr init fails
        initializeOpenVR( initType, ( count + 1 ) );
    }
    if ( count >= 3 )
    {
        LOG( ERROR ) << "initialization errors persist proceeding anyways";
    }
}

} // namespace openvr_init
