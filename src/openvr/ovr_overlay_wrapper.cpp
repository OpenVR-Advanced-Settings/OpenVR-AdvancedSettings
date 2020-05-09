#include "ovr_overlay_wrapper.h"

namespace ovr_overlay_wrapper
{
OverlayError createOverlay( const std::string pchOverlayKey,
                            const std::string pchOverlayName,
                            vr::VROverlayHandle_t* pOverlayHandle,
                            std::string customErrorMsg )
{
    vr::VROverlayError oError = vr::VROverlay()->CreateOverlay(
        pchOverlayKey.c_str(), pchOverlayName.c_str(), pOverlayHandle );
    if ( oError == vr::VROverlayError_None )
    {
        return OverlayError::NoError;
    }
    else
    {
        LOG( ERROR ) << "Could not create overlay: " << pchOverlayName
                     << "because "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum( oError )
                     << " " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
}

OverlayError setOverlayColor( vr::VROverlayHandle_t ulOverlayHandle,
                              float fRed,
                              float fGreen,
                              float fBlue,
                              std::string customErrorMsg )
{
    vr::VROverlayError oError = vr::VROverlay()->SetOverlayColor(
        ulOverlayHandle, fRed, fGreen, fBlue );
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

OverlayError setOverlayAlpha( vr::VROverlayHandle_t ulOverlayHandle,
                              float alpha,
                              std::string customErrorMsg )
{
    vr::VROverlayError oError
        = vr::VROverlay()->SetOverlayAlpha( ulOverlayHandle, alpha );
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

OverlayError setOverlayWidthInMeters( vr::VROverlayHandle_t ulOverlayHandle,
                                      float fWidthInMeters,
                                      std::string customErrorMsg )
{
    vr::VROverlayError oError = vr::VROverlay()->SetOverlayWidthInMeters(
        ulOverlayHandle, fWidthInMeters );
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

OverlayError setOverlayFromFile( vr::VROverlayHandle_t ulOverlayHandle,
                                 std::string fileName,
                                 std::string customErrorMsg )
{
    {
        const auto overlayPath = paths::binaryDirectoryFindFile( fileName );
        if ( overlayPath.has_value() )
        {
            vr::VROverlayError oError = vr::VROverlay()->SetOverlayFromFile(
                ulOverlayHandle, overlayPath->c_str() );
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

OverlayError showOverlay( vr::VROverlayHandle_t ulOverlayHandle,
                          std::string customErrorMsg )
{
    vr::VROverlayError oError = vr::VROverlay()->ShowOverlay( ulOverlayHandle );
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

OverlayError hideOverlay( vr::VROverlayHandle_t ulOverlayHandle,
                          std::string customErrorMsg )
{
    vr::VROverlayError oError = vr::VROverlay()->HideOverlay( ulOverlayHandle );
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
    vr::VROverlayHandle_t ulOverlayHandle,
    vr::ETrackingUniverseOrigin eTrackingOrigin,
    const vr::HmdMatrix34_t* pmatTrackingOriginToOverlayTransform,
    std::string customErrorMsg )
{
    vr::VROverlayError oError = vr::VROverlay()->SetOverlayTransformAbsolute(
        ulOverlayHandle,
        eTrackingOrigin,
        pmatTrackingOriginToOverlayTransform );
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
