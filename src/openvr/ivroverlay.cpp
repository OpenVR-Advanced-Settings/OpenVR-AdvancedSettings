#include "ivroverlay.h"

namespace ivroverlay
{
overlayError createOverlay( const std::string pchOverlayKey,
                            const std::string pchOverlayName,
                            vr::VROverlayHandle_t* pOverlayHandle,
                            std::string customErrorMsg )
{
    vr::VROverlayError overlayError = vr::VROverlay()->CreateOverlay(
        pchOverlayKey.c_str(), pchOverlayName.c_str(), pOverlayHandle );
    if ( overlayError == vr::VROverlayError_None )
    {
        return overlayError::noErr;
    }
    else
    {
        LOG( ERROR ) << "Could not create overlay: " << pchOverlayName
                     << "because "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                            overlayError )
                     << " " << customErrorMsg;
        return overlayError::undefErr;
    }
}

overlayError setOverlayColor( vr::VROverlayHandle_t ulOverlayHandle,
                              float fRed,
                              float fGreen,
                              float fBlue,
                              std::string customErrorMsg )
{
    vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayColor(
        ulOverlayHandle, fRed, fGreen, fBlue );
    if ( overlayError == vr::VROverlayError_None )
    {
        return overlayError::noErr;
    }
    else
    {
        // TODO get Name and print out error.....
        LOG( ERROR ) << "Error setting Color: " << customErrorMsg;
        return overlayError::undefErr;
    }
}

overlayError setOverlayAlpha( vr::VROverlayHandle_t ulOverlayHandle,
                              float alpha,
                              std::string customErrorMsg )
{
    vr::VROverlayError overlayError
        = vr::VROverlay()->SetOverlayAlpha( ulOverlayHandle, alpha );
    if ( overlayError == vr::VROverlayError_None )
    {
        return overlayError::noErr;
    }
    else
    {
        // TODO get Name and print out error.....
        LOG( ERROR ) << "Error setting Alpha: " << customErrorMsg;
        return overlayError::undefErr;
    }
}

overlayError setOverlayWidthInMeters( vr::VROverlayHandle_t ulOverlayHandle,
                                      float fWidthInMeters,
                                      std::string customErrorMsg )
{
    vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayWidthInMeters(
        ulOverlayHandle, fWidthInMeters );
    if ( overlayError == vr::VROverlayError_None )
    {
        return overlayError::noErr;
    }
    else
    {
        // TODO get Name and print out error.....
        LOG( ERROR ) << "Error setting Width" << customErrorMsg;
        return overlayError::undefErr;
    }
}

overlayError setOverlayFromFile( vr::VROverlayHandle_t ulOverlayHandle,
                                 std::string fileName,
                                 std::string customErrorMsg )
{
    {
        const auto overlayPath = paths::binaryDirectoryFindFile( fileName );
        if ( overlayPath.has_value() )
        {
            vr::VROverlayError overlayError
                = vr::VROverlay()->SetOverlayFromFile( ulOverlayHandle,
                                                       overlayPath->c_str() );
            if ( overlayError == vr::VROverlayError_None )
            {
                return overlayError::noErr;
            }
            else
            {
                // TODO get Name and print out error.....
                LOG( ERROR ) << "Error creating overlay " << customErrorMsg;
                return overlayError::undefErr;
            }
        }
        else
        {
            LOG( ERROR ) << "File not Found: " << fileName << customErrorMsg;
            return overlayError::undefErr;
        }
    }
}

overlayError showOverlay( vr::VROverlayHandle_t ulOverlayHandle,
                          std::string customErrorMsg )
{
    vr::VROverlayError overlayError
        = vr::VROverlay()->ShowOverlay( ulOverlayHandle );
    if ( overlayError == vr::VROverlayError_None )
    {
        return overlayError::noErr;
    }
    else
    {
        // TODO get Name and print out error.....
        LOG( ERROR ) << "Error showing overlay " << customErrorMsg;
        return overlayError::undefErr;
    }
}

overlayError hideOverlay( vr::VROverlayHandle_t ulOverlayHandle,
                          std::string customErrorMsg )
{
    vr::VROverlayError overlayError
        = vr::VROverlay()->HideOverlay( ulOverlayHandle );
    if ( overlayError == vr::VROverlayError_None )
    {
        return overlayError::noErr;
    }
    else
    {
        // TODO get Name and print out error.....
        LOG( ERROR ) << "Error hiding overlay " << customErrorMsg;
        return overlayError::undefErr;
    }
}

} // namespace ivroverlay
