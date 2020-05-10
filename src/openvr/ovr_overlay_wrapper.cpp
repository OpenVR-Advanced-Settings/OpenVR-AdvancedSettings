#include "ovr_overlay_wrapper.h"

namespace ovr_overlay_wrapper
{
OverlayError createOverlay( const std::string overlayKey,
                            const std::string overlayName,
                            vr::VROverlayHandle_t* overlayHandle,
                            std::string customErrorMsg )
{
    vr::VROverlayError oError = vr::VROverlay()->CreateOverlay(
        overlayKey.c_str(), overlayName.c_str(), overlayHandle );
    if ( oError == vr::VROverlayError_None )
    {
        return OverlayError::NoError;
    }
    else
    {
        LOG( ERROR ) << "Could not create overlay: " << overlayName
                     << "because "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum( oError )
                     << " " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
}

OverlayError setOverlayColor( vr::VROverlayHandle_t overlayHandle,
                              float red,
                              float green,
                              float blue,
                              std::string customErrorMsg )
{
    vr::VROverlayError oError
        = vr::VROverlay()->SetOverlayColor( overlayHandle, red, green, blue );
    if ( oError == vr::VROverlayError_None )
    {
        return OverlayError::NoError;
    }
    else
    {
        // TODO get Name and print out error.....
        LOG( ERROR ) << "Error setting Color: " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
}

OverlayError setOverlayAlpha( vr::VROverlayHandle_t overlayHandle,
                              float alpha,
                              std::string customErrorMsg )
{
    vr::VROverlayError oError
        = vr::VROverlay()->SetOverlayAlpha( overlayHandle, alpha );
    if ( oError == vr::VROverlayError_None )
    {
        return OverlayError::NoError;
    }
    else
    {
        // TODO get Name and print out error.....
        LOG( ERROR ) << "Error setting Alpha: " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
}

OverlayError setOverlayWidthInMeters( vr::VROverlayHandle_t overlayHandle,
                                      float widthInMeters,
                                      std::string customErrorMsg )
{
    vr::VROverlayError oError = vr::VROverlay()->SetOverlayWidthInMeters(
        overlayHandle, widthInMeters );
    if ( oError == vr::VROverlayError_None )
    {
        return OverlayError::NoError;
    }
    else
    {
        // TODO get Name and print out error.....
        LOG( ERROR ) << "Error setting Width" << customErrorMsg;
        return OverlayError::UndefinedError;
    }
}

OverlayError setOverlayFromFile( vr::VROverlayHandle_t overlayHandle,
                                 std::string fileName,
                                 std::string customErrorMsg )
{
    {
        const auto overlayPath = paths::binaryDirectoryFindFile( fileName );
        if ( overlayPath.has_value() )
        {
            vr::VROverlayError oError = vr::VROverlay()->SetOverlayFromFile(
                overlayHandle, overlayPath->c_str() );
            if ( oError == vr::VROverlayError_None )
            {
                return OverlayError::NoError;
            }
            else
            {
                // TODO get Name and print out error.....
                LOG( ERROR ) << "Error creating overlay " << customErrorMsg;
                return OverlayError::UndefinedError;
            }
        }
        else
        {
            LOG( ERROR ) << "File not Found: " << fileName << customErrorMsg;
            return OverlayError::UndefinedError;
        }
    }
}

OverlayError showOverlay( vr::VROverlayHandle_t overlayHandle,
                          std::string customErrorMsg )
{
    vr::VROverlayError oError = vr::VROverlay()->ShowOverlay( overlayHandle );
    if ( oError == vr::VROverlayError_None )
    {
        return OverlayError::NoError;
    }
    else
    {
        // TODO get Name and print out error.....
        LOG( ERROR ) << "Error showing overlay " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
}

OverlayError hideOverlay( vr::VROverlayHandle_t overlayHandle,
                          std::string customErrorMsg )
{
    vr::VROverlayError oError = vr::VROverlay()->HideOverlay( overlayHandle );
    if ( oError == vr::VROverlayError_None )
    {
        return OverlayError::NoError;
    }
    else
    {
        // TODO get Name and print out error.....
        LOG( ERROR ) << "Error hiding overlay " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
}

OverlayError setOverlayTransformAbsolute(
    vr::VROverlayHandle_t overlayHandle,
    vr::ETrackingUniverseOrigin trackingOrigin,
    const vr::HmdMatrix34_t* trackingOriginToOverlayTransform,
    std::string customErrorMsg )
{
    vr::VROverlayError oError = vr::VROverlay()->SetOverlayTransformAbsolute(
        overlayHandle, trackingOrigin, trackingOriginToOverlayTransform );
    if ( oError == vr::VROverlayError_None )
    {
        return OverlayError::NoError;
    }
    else
    {
        // TODO get handle name
        LOG( ERROR ) << "Error setting Overlay Position "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum( oError )
                     << " " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
}

} // namespace ovr_overlay_wrapper
