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

OverlayError createOverlay( const std::string pchOverlayKey,
                            const std::string pchOverlayName,
                            vr::VROverlayHandle_t* pOverlayHandle,
                            std::string customErrorMsg = "" );

OverlayError setOverlayColor( vr::VROverlayHandle_t ulOverlayHandle,
                              float fRed,
                              float fGreen,
                              float fBlue,
                              std::string customErrorMsg = "" );
OverlayError setOverlayAlpha( vr::VROverlayHandle_t ulOverlayHandle,
                              float alpha,
                              std::string customErrorMsg = "" );

OverlayError setOverlayWidthInMeters( vr::VROverlayHandle_t ulOverlayHandle,
                                      float fWidthInMeters,
                                      std::string customErrorMsg = "" );
OverlayError setOverlayFromFile( vr::VROverlayHandle_t ulOverlayHandle,
                                 std::string fileName,
                                 std::string customErrorMsg = "" );
OverlayError showOverlay( vr::VROverlayHandle_t ulOverlayHandle,
                          std::string customErrorMsg = "" );
OverlayError hideOverlay( vr::VROverlayHandle_t ulOverlayHandle,
                          std::string customErrorMsg = "" );
OverlayError setOverlayTransformAbsolute(
    vr::VROverlayHandle_t ulOverlayHandle,
    vr::ETrackingUniverseOrigin eTrackingOrigin,
    const vr::HmdMatrix34_t* pmatTrackingOriginToOverlayTransform,
    std::string customErrorMsg = "" );

} // namespace ovr_overlay_wrapper
