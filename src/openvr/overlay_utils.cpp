#include "overlay_utils.h"
#include "../overlaycontroller.h"

namespace overlay
{
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

void setSettingsToValue( const std::string setting, const double value )
{
    QSettings settings( QSettings::Format::IniFormat,
                        QSettings::Scope::UserScope,
                        application_strings::applicationOrganizationName,
                        application_strings::applicationName );
    settings.beginGroup( strings::settingsGroupName );
    settings.setValue( setting.c_str(), value );
    settings.endGroup();
}

overlay::DesktopOverlay::DesktopOverlay()
{
    QSettings settings( QSettings::Format::IniFormat,
                        QSettings::Scope::UserScope,
                        application_strings::applicationOrganizationName,
                        application_strings::applicationName );

    settings.beginGroup( strings::settingsGroupName );
    m_width = settings.value( strings::widthSettingsName, defaultOverlayWidth )
                  .toDouble();
    settings.endGroup();
}

void DesktopOverlay::update()
{
    setDesktopOverlayWidth( m_width );
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

    setSettingsToValue( strings::widthSettingsName, width );
    m_width = width;
}

double DesktopOverlay::getCurrentWidth() const noexcept
{
    return m_width;
}

} // namespace overlay
