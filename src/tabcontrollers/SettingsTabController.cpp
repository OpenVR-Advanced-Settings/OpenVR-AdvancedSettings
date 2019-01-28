#include "SettingsTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings
{
void SettingsTabController::initStage1()
{
    m_autoStartEnabled = vr::VRApplications()->GetApplicationAutoLaunch(
        OverlayController::applicationKey );
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "applicationSettings" );
    auto value = settings->value( "forceRevivePage", m_forceRevivePage );
    settings->endGroup();
    if ( value.isValid() && !value.isNull() )
    {
        m_forceRevivePage = value.toBool();
    }
}

void SettingsTabController::initStage2( OverlayController* var_parent,
                                        QQuickWindow* var_widget )
{
    this->parent = var_parent;
    this->widget = var_widget;
}

void SettingsTabController::eventLoopTick()
{
    // 157 was chosen because it's a prime number (to reduce overlap of
    // simultaneous settings updates).
    // Actual rate of updates is 157 * vsync (~11ms)
    // Values chosen based on update speed priority
    // Values in other tabs are as follows (avoid using same values):
    // AccessibiltyTabController: 151
    // AudioTabController: 89
    // ChaperoneTabController: 101
    // MoveCenterTabController: 149
    // ReviveTabController: 139
    // SteamVRTabController: 97
    // UtilitiesTabController: 19
    if ( settingsUpdateCounter >= 157 )
    {
        if ( parent->isDashboardVisible() )
        {
            setAutoStartEnabled( vr::VRApplications()->GetApplicationAutoLaunch(
                OverlayController::applicationKey ) );
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
            OverlayController::applicationKey, m_autoStartEnabled );
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
