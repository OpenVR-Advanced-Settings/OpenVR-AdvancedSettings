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
namespace ivroverlay
{
// might make sense to mirror ovr's errors exactly, but for now this is more
// than sufficient
enum class overlayError
{
    noErr,
    undefErr,

};

overlayError createOverlay( const std::string pchOverlayKey,
                            const std::string pchOverlayName,
                            vr::VROverlayHandle_t* pOverlayHandle,
                            std::string customErrorMsg = "" );

overlayError setOverlayColor( vr::VROverlayHandle_t ulOverlayHandle,
                              float fRed,
                              float fGreen,
                              float fBlue,
                              std::string customErrorMsg = "" );
overlayError setOverlayAlpha( vr::VROverlayHandle_t ulOverlayHandle,
                              float alpha,
                              std::string customErrorMsg = "" );

overlayError setOverlayWidthInMeters( vr::VROverlayHandle_t ulOverlayHandle,
                                      float fWidthInMeters,
                                      std::string customErrorMsg = "" );
overlayError setOverlayFromFile( vr::VROverlayHandle_t ulOverlayHandle,
                                 std::string fileName,
                                 std::string customErrorMsg = "" );

} // namespace ivroverlay
