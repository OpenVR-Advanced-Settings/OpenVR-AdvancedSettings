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
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "applicationSettings" );
    auto value = settings->value( "forceRevivePage", m_forceRevivePage );
    settings->endGroup();
    if ( value.isValid() && !value.isNull() )
    {
        m_forceRevivePage = value.toBool();
    }
}

void SettingsTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
}

void SettingsTabController::eventLoopTick()
{
    if ( settingsUpdateCounter >= k_settingsTabSettingsUpdateCounter )
    {
        if ( parent->isDashboardVisible() )
        {
            setAutoStartEnabled( vr::VRApplications()->GetApplicationAutoLaunch(
                application_strings::applicationKey ) );
        }
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

bool SettingsTabController::forceRevivePage() const
{
    return m_forceRevivePage;
}

void SettingsTabController::setForceRevivePage( bool value, bool notify )
{
    if ( m_forceRevivePage != value )
    {
        m_forceRevivePage = value;
        auto settings = OverlayController::appSettings();
        settings->beginGroup( "applicationSettings" );
        settings->setValue( "forceRevivePage", m_forceRevivePage );
        settings->endGroup();
        settings->sync();
        if ( notify )
        {
            emit forceRevivePageChanged( m_forceRevivePage );
        }
    }
}

} // namespace advsettings
