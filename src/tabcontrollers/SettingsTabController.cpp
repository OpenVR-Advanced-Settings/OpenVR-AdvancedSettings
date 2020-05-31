#include "SettingsTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings
{
void SettingsTabController::initStage1()
{
    m_autoStartEnabled = vr::VRApplications()->GetApplicationAutoLaunch(
        application_strings::applicationKey );
    m_settingsTabSettingsUpdateCounter
        = utils::adjustUpdateRate( k_settingsTabSettingsUpdateCounter );
}

void SettingsTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
    setNativeChaperoneToggle( nativeChaperoneToggle() );
}

void SettingsTabController::dashboardLoopTick()
{
    if ( settingsUpdateCounter >= m_settingsTabSettingsUpdateCounter )
    {
        setAutoStartEnabled( vr::VRApplications()->GetApplicationAutoLaunch(
            application_strings::applicationKey ) );
        settingsUpdateCounter = 0;
    }
    else
    {
        settingsUpdateCounter++;
    }
}

bool SettingsTabController::autoStartEnabled() const
{
    return m_autoStartEnabled;
}

bool SettingsTabController::nativeChaperoneToggle()
{
    std::pair<ovr_settings_wrapper::SettingsError, bool> p
        = ovr_settings_wrapper::getFloat(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_EnableDriverImport,
            "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        m_nativeChaperoneToggle = !( p.second );
    }
    return m_nativeChaperoneToggle;
}

void SettingsTabController::setNativeChaperoneToggle( bool value, bool notify )
{
    if ( m_nativeChaperoneToggle != value )
    {
        m_nativeChaperoneToggle = !( value );
        ovr_settings_wrapper::setBool(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_EnableDriverImport,
            m_nativeChaperoneToggle,
            "" );
        // On Error Do Nothing
        if ( notify )
        {
            emit nativeChaperoneToggleChanged( m_nativeChaperoneToggle );
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
