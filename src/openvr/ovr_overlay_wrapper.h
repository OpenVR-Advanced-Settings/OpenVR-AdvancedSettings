#pragma once

#include <openvr.h>
#include <iostream>
#include <string>
#include <easylogging++.h>
#include <utility>
#include <string>
#include "../utils/paths.h"

/* Wrapper For OpenVR's IVR settings class, allows us to do our error logging
 * while also minimizing code
 *
 */
namespace ovr_overlay_wrapper
{
// might make sense to mirror ovr's errors exactly, but for now this is more
// than sufficient
enum class OverlayError
{
    NoError,
    UndefinedError,

};

OverlayError createOverlay( const std::string overlayKey,
                            const std::string overlayName,
                            vr::VROverlayHandle_t* overlayHandle,
                            std::string customErrorMsg = "" );

OverlayError setOverlayColor( vr::VROverlayHandle_t overlayHandle,
                              float red,
                              float green,
                              float blue,
                              std::string customErrorMsg = "" );
OverlayError setOverlayAlpha( vr::VROverlayHandle_t overlayHandle,
                              float alpha,
                              std::string customErrorMsg = "" );

OverlayError setOverlayWidthInMeters( vr::VROverlayHandle_t overlayHandle,
                                      float widthInMeters,
                                      std::string customErrorMsg = "" );
OverlayError setOverlayFromFile( vr::VROverlayHandle_t overlayHandle,
                                 std::string fileName,
                                 std::string customErrorMsg = "" );
OverlayError showOverlay( vr::VROverlayHandle_t overlayHandle,
                          std::string customErrorMsg = "" );
OverlayError hideOverlay( vr::VROverlayHandle_t overlayHandle,
                          std::string customErrorMsg = "" );
OverlayError setOverlayTransformAbsolute(
    vr::VROverlayHandle_t overlayHandle,
    vr::ETrackingUniverseOrigin trackingOrigin,
    const vr::HmdMatrix34_t* trackingOriginToOverlayTransform,
    std::string customErrorMsg = "" );

std::string getOverlayKey( vr::VROverlayHandle_t overlayHandle );

} // namespace ovr_overlay_wrapper
