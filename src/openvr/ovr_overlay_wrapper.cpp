#include "ovr_overlay_wrapper.h"
#include "utils/paths.h"
#include <QtLogging>
#include <QtDebug>
#include <QImage>

namespace ovr_overlay_wrapper
{
OverlayError createOverlay( const std::string overlayKey,
                            const std::string overlayName,
                            vr::VROverlayHandle_t* overlayHandle,
                            std::string customErrorMsg )
{
    vr::VROverlayError const oError = vr::VROverlay()->CreateOverlay(
        overlayKey.c_str(), overlayName.c_str(), overlayHandle );
    if ( oError != vr::VROverlayError_None )
    {
        qCritical() << "Could not create overlay: " << overlayName << "because "
                    << vr::VROverlay()->GetOverlayErrorNameFromEnum( oError )
                    << " " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
    return OverlayError::NoError;
}

OverlayError setOverlayColor( vr::VROverlayHandle_t overlayHandle,
                              float red,
                              float green,
                              float blue,
                              std::string customErrorMsg )
{
    vr::VROverlayError const oError
        = vr::VROverlay()->SetOverlayColor( overlayHandle, red, green, blue );
    if ( oError != vr::VROverlayError_None )
    {
        qCritical() << "Error setting Overlay Color For "
                    << getOverlayKey( overlayHandle ) << " with error: "
                    << vr::VROverlay()->GetOverlayErrorNameFromEnum( oError )
                    << " " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
    return OverlayError::NoError;
}

OverlayError setOverlayAlpha( vr::VROverlayHandle_t overlayHandle,
                              float alpha,
                              std::string customErrorMsg )
{
    vr::VROverlayError const oError
        = vr::VROverlay()->SetOverlayAlpha( overlayHandle, alpha );
    if ( oError != vr::VROverlayError_None )
    {
        qCritical() << "Error setting Overlay Alpha For "
                    << getOverlayKey( overlayHandle ) << " with error: "
                    << vr::VROverlay()->GetOverlayErrorNameFromEnum( oError )
                    << " " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
    return OverlayError::NoError;
}

OverlayError setOverlayWidthInMeters( vr::VROverlayHandle_t overlayHandle,
                                      float widthInMeters,
                                      std::string customErrorMsg )
{
    vr::VROverlayError const oError = vr::VROverlay()->SetOverlayWidthInMeters(
        overlayHandle, widthInMeters );
    if ( oError != vr::VROverlayError_None )
    {
        qCritical() << "Error setting overlay width For "
                    << getOverlayKey( overlayHandle ) << " with error: "
                    << vr::VROverlay()->GetOverlayErrorNameFromEnum( oError )
                    << " " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
    return OverlayError::NoError;
}

OverlayError setOverlayFromFile( vr::VROverlayHandle_t overlayHandle,
                                 std::string fileName,
                                 std::string customErrorMsg )
{
    {
        const auto overlayPath = paths::binaryDirectoryFindFile( fileName );
        if ( !overlayPath.has_value() )
        {
            qCritical() << "File not Found: " << fileName << customErrorMsg;
            return OverlayError::UndefinedError;
        }
        vr::VROverlayError const oError = vr::VROverlay()->SetOverlayFromFile(
            overlayHandle, overlayPath->c_str() );
        if ( oError != vr::VROverlayError_None )
        {
            qCritical() << "Error setting Overlay from file For "
                        << getOverlayKey( overlayHandle ) << " with error: "
                        << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                               oError )
                        << " " << customErrorMsg;
            return OverlayError::UndefinedError;
        }
        return OverlayError::NoError;
    }
}

OverlayError setOverlayFromQImage( vr::VROverlayHandle_t overlayHandle,
                                   QImage& image,
                                   std::string customErrorMsg )
{
    image.convertTo( QImage::Format_RGBA8888 );
    vr::VROverlayError const oError = vr::VROverlay()->SetOverlayRaw(
        overlayHandle, image.bits(), image.width(), image.height(), 4 );
    if ( oError != vr::VROverlayError_None )
    {
        qCritical() << "Error setting Overlay from" << image << "For"
                    << getOverlayKey( overlayHandle ) << "with error:"
                    << vr::VROverlay()->GetOverlayErrorNameFromEnum( oError )
                    << customErrorMsg;
        return OverlayError::UndefinedError;
    }
    return OverlayError::NoError;
}

OverlayError showOverlay( vr::VROverlayHandle_t overlayHandle,
                          std::string customErrorMsg )
{
    vr::VROverlayError const oError
        = vr::VROverlay()->ShowOverlay( overlayHandle );
    if ( oError != vr::VROverlayError_None )
    {
        qCritical() << "Error showing overlay For "
                    << getOverlayKey( overlayHandle ) << " with error: "
                    << vr::VROverlay()->GetOverlayErrorNameFromEnum( oError )
                    << " " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
    return OverlayError::NoError;
}

OverlayError hideOverlay( vr::VROverlayHandle_t overlayHandle,
                          std::string customErrorMsg )
{
    vr::VROverlayError const oError
        = vr::VROverlay()->HideOverlay( overlayHandle );
    if ( oError != vr::VROverlayError_None )
    {
        qCritical() << "Error hiding overlay For "
                    << getOverlayKey( overlayHandle ) << " with error: "
                    << vr::VROverlay()->GetOverlayErrorNameFromEnum( oError )
                    << " " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
    return OverlayError::NoError;
}

OverlayError setOverlayTransformAbsolute(
    vr::VROverlayHandle_t overlayHandle,
    vr::ETrackingUniverseOrigin trackingOrigin,
    const vr::HmdMatrix34_t* trackingOriginToOverlayTransform,
    std::string customErrorMsg )
{
    vr::VROverlayError const oError
        = vr::VROverlay()->SetOverlayTransformAbsolute(
            overlayHandle, trackingOrigin, trackingOriginToOverlayTransform );
    if ( oError != vr::VROverlayError_None )
    {
        qCritical() << "Error setting Overlay Position For "
                    << getOverlayKey( overlayHandle ) << " with error: "
                    << vr::VROverlay()->GetOverlayErrorNameFromEnum( oError )
                    << " " << customErrorMsg;
        return OverlayError::UndefinedError;
    }
    return OverlayError::NoError;
}

std::string getOverlayKey( vr::VROverlayHandle_t overlayHandle )
{
    // max size as of ovr 1.11.11 128 bytes
    const uint32_t bufferMax = vr::k_unVROverlayMaxKeyLength;
    std::array<char, bufferMax> cStringOut;
    vr::EVROverlayError oErrorOut = {};
    vr::VROverlay()->GetOverlayKey(
        overlayHandle, cStringOut.data(), bufferMax, &oErrorOut );
    std::string output = cStringOut.data();
    if ( oErrorOut != vr::VROverlayError_None )
    {
        std::string const errorName
            = vr::VROverlay()->GetOverlayErrorNameFromEnum( oErrorOut );
        return ( "can't get name:" + errorName );
    }
    return output;
}

} // namespace ovr_overlay_wrapper
