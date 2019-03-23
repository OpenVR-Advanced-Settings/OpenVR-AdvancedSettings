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

}

void SettingsTabController::initStage2( OverlayController* var_parent,
                                        QQuickWindow* var_widget )
{
    this->parent = var_parent;
    this->widget = var_widget;
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

} // namespace advsettings
