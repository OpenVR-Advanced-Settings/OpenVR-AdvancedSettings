#include "SettingsTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"
#include "../utils/update_rate.h"

// application namespace
namespace advsettings
{
void SettingsTabController::initStage1()
{
    m_autoStartEnabled = vr::VRApplications()->GetApplicationAutoLaunch(
        application_strings::applicationKey );
}

void SettingsTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
    setNativeChaperoneToggle( nativeChaperoneToggle() );
    setOculusSdkToggle( oculusSdkToggle() );
}

void SettingsTabController::dashboardLoopTick()
{
    if ( updateRate.shouldSubjectNotRun(
             UpdateSubject::SettingsTabController ) )
    {
        return;
    }

    setAutoStartEnabled( vr::VRApplications()->GetApplicationAutoLaunch(
        application_strings::applicationKey ) );
}

bool SettingsTabController::autoStartEnabled() const
{
    return m_autoStartEnabled;
}

bool SettingsTabController::nativeChaperoneToggle()
{
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_CollisionBounds_Section,
        vr::k_pch_CollisionBounds_EnableDriverImport,
        "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return !( p.second );
    }
    return false;
}

bool SettingsTabController::oculusSdkToggle()
{
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_BlockOculusSDKOnAllLaunches_Bool,
        "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return ( p.second );
    }
    return false;
}

void SettingsTabController::setNativeChaperoneToggle( bool value, bool notify )
{
    if ( nativeChaperoneToggle() != value )
    {
        ovr_settings_wrapper::setBool(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_EnableDriverImport,
            !value,
            "" );
        // On Error Do Nothing
        if ( notify )
        {
            emit nativeChaperoneToggleChanged( !value );
        }
    }
}

void SettingsTabController::setOculusSdkToggle( bool value, bool notify )
{
    if ( oculusSdkToggle() != value )
    {
        ovr_settings_wrapper::setBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_BlockOculusSDKOnAllLaunches_Bool,
            value,
            "" );
        // On Error Do Nothing
        if ( notify )
        {
            emit oculusSdkToggleChanged( value );
        }
    }
}

void SettingsTabController::setAutoStartEnabled( bool value, bool notify )
{
    if ( m_autoStartEnabled != value )
    {
        m_autoStartEnabled = value;
        auto apperror = vr::VRApplications()->SetApplicationAutoLaunch(
            application_strings::applicationKey, m_autoStartEnabled );
        if ( apperror != vr::VRApplicationError_None )
        {
            LOG( ERROR ) << "Could not set auto start: "
                         << vr::VRApplications()
                                ->GetApplicationsErrorNameFromEnum( apperror );
        }
        if ( notify )
        {
            emit autoStartEnabledChanged( m_autoStartEnabled );
        }
    }
}

} // namespace advsettings
