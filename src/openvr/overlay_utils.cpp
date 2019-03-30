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

vr::HmdMatrix34_t getDesktopOverlayPositions()
{
    vr::HmdMatrix34_t currentPosition = {};

    const auto overlayHandle = getOverlayHandle();

    vr::ETrackingUniverseOrigin origin
        = vr::ETrackingUniverseOrigin::TrackingUniverseStanding;

    vr::VROverlay()->GetOverlayTransformAbsolute(
        overlayHandle, &origin, &currentPosition );

    return currentPosition;
}

void setDesktopOverlayPositions( vr::HmdMatrix34_t& positions )
{
    const auto overlayHandle = getOverlayHandle();

    // We need to make sure we get the same Universe Origin as before.
    vr::HmdMatrix34_t currentPosition = {};
    vr::ETrackingUniverseOrigin origin
        = vr::ETrackingUniverseOrigin::TrackingUniverseStanding;
    vr::VROverlay()->GetOverlayTransformAbsolute(
        overlayHandle, &origin, &currentPosition );

    vr::VROverlay()->SetOverlayTransformAbsolute(
        overlayHandle, origin, &positions );
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

    LOG( INFO ) << settings.fileName();
    settings.beginGroup( strings::settingsGroupName );
    m_width = settings.value( strings::widthSettingsName, defaultOverlayWidth )
                  .toDouble();
    m_forwardsMovement
        = settings.value( strings::forwardsSettingsName, defaultMovement )
              .toDouble();
    m_rightMovement
        = settings.value( strings::rightMovementSettingsName, defaultMovement )
              .toDouble();
    m_upMovement
        = settings.value( strings::upMovementSettingsName, defaultMovement )
              .toDouble();
    settings.endGroup();
}

void DesktopOverlay::update()
{
    setDesktopOverlayWidth( m_width );

    auto positionMatrix = getDesktopOverlayPositions();

    auto changedPositions = getChangedMatrix( positionMatrix );

    setDesktopOverlayPositions( changedPositions );
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

void DesktopOverlay::setRightMovement( double distance )
{
    setSettingsToValue( strings::rightMovementSettingsName, distance );
    m_rightMovement = distance;
}

double DesktopOverlay::getCurrentRightMovement() const noexcept
{
    return m_rightMovement;
}

void DesktopOverlay::setForwardsMovement( double distance )
{
    setSettingsToValue( strings::forwardsSettingsName, distance );
    m_forwardsMovement = distance;
}

double DesktopOverlay::getCurrentForwardsMovement() const noexcept
{
    return m_forwardsMovement;
}

void DesktopOverlay::setHeight( double height )
{
    setSettingsToValue( strings::upMovementSettingsName, height );
    m_upMovement = height;
}

double DesktopOverlay::getCurrentHeight() const noexcept
{
    return m_upMovement;
}

vr::HmdMatrix34_t
    DesktopOverlay::getChangedMatrix( vr::HmdMatrix34_t& matrix ) const noexcept
{
    auto position = QVector3D( matrix.m[0][3], matrix.m[1][3], matrix.m[2][3] );

    const auto screenRight
        = QVector3D( matrix.m[0][0], matrix.m[1][0], matrix.m[2][0] );

    const auto screenForwards
        = QVector3D( matrix.m[0][2], matrix.m[1][2], matrix.m[2][2] );

    const auto screenUp
        = QVector3D( matrix.m[0][1], matrix.m[1][1], matrix.m[2][1] );

    position += ( static_cast<float>( m_rightMovement ) * screenRight );

    position += ( static_cast<float>( m_forwardsMovement ) * screenForwards );

    position += ( static_cast<float>( m_upMovement ) * screenUp );

    matrix.m[0][3] = position.x();
    matrix.m[1][3] = position.y();
    matrix.m[2][3] = position.z();

    return matrix;
}

} // namespace overlay
