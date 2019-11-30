#include "overlay_utils.h"
#include "../overlaycontroller.h"
#include "../settings/settings.h"

namespace overlay
{
namespace strings
{
    constexpr auto overlaykey = "valve.steam.desktop";
} // namespace strings

vr::VROverlayHandle_t getOverlayHandle()
{
    vr::VROverlayHandle_t pOverlayHandle;
    vr::VROverlay()->FindOverlay( strings::overlaykey, &pOverlayHandle );
    return pOverlayHandle;
}

void setDesktopOverlayWidth( double width )
{
    const auto overlayHandle = getOverlayHandle();
    auto error = vr::VROverlay()->SetOverlayWidthInMeters(
        overlayHandle, static_cast<float>( width ) );
    if ( error != vr::VROverlayError_None )
    {
        LOG( ERROR ) << "Could not modify overlay width of \""
                     << strings::overlaykey << "\": "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum( error );
    }
}

void DesktopOverlay::update()
{
    setDesktopOverlayWidth(
        settings::getSetting( settings::DoubleSetting::UTILITY_desktopWidth ) );
}

bool DesktopOverlay::isAvailable() const
{
    vr::VROverlayHandle_t pOverlayHandle = 0;
    auto error
        = vr::VROverlay()->FindOverlay( strings::overlaykey, &pOverlayHandle );
    if ( error != vr::VROverlayError_None )
    {
        LOG( INFO ) << "Could not find overlay \"" << strings::overlaykey
                    << "\": "
                    << vr::VROverlay()->GetOverlayErrorNameFromEnum( error );
        return false;
    }
    return true;
}

void overlay::DesktopOverlay::setWidth( double width )
{
    if ( width < 0.01 )
    {
        width = 0.01;
    }

    settings::setSetting( settings::DoubleSetting::UTILITY_desktopWidth,
                          width );
}

double DesktopOverlay::getCurrentWidth() const noexcept
{
    return settings::getSetting(
        settings::DoubleSetting::UTILITY_desktopWidth );
}

} // namespace overlay
