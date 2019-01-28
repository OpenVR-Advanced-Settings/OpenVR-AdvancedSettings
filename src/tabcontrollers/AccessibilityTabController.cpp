#include "AccessibilityTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings
{
void AccessibilityTabController::initStage1()
{
    setTrackingUniverse( vr::VRCompositor()->GetTrackingSpace() );
    auto settings = OverlayController::appSettings();
    settings->beginGroup( getSettingsName() );
    auto value = settings->value( "heightOffset", m_heightOffset );
    settings->endGroup();
    if ( value.isValid() && !value.isNull() )
    {
        m_heightOffset = value.toFloat();
        emit heightOffsetChanged( m_heightOffset );
    }

    reloadPttConfig();
}

void AccessibilityTabController::initStage2( OverlayController* var_parent,
                                             QQuickWindow* var_widget )
{
    this->parent = var_parent;
    this->widget = var_widget;
}

void AccessibilityTabController::setTrackingUniverse( int value )
{
    if ( m_trackingUniverse != value )
    {
        reset();
        m_trackingUniverse = value;
    }
}

float AccessibilityTabController::heightOffset() const
{
    return m_heightOffset;
}

void AccessibilityTabController::setHeightOffset( float value, bool notify )
{
    if ( std::abs( m_heightOffset - value ) > 0.001 )
    {
        modHeightOffset( value - m_heightOffset, notify );
    }
}

void AccessibilityTabController::modHeightOffset( float value, bool notify )
{
    parent->AddOffsetToUniverseCenter(
        ( vr::TrackingUniverseOrigin ) m_trackingUniverse, 1, -value, false );
    m_heightOffset += value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( getSettingsName() );
    settings->setValue( "heightOffset", m_heightOffset );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit heightOffsetChanged( m_heightOffset );
    }
}

void AccessibilityTabController::onPttStart()
{
    m_toggleHeightOffset = m_heightOffset;
    parent->AddOffsetToUniverseCenter(
        ( vr::TrackingUniverseOrigin ) m_trackingUniverse,
        1,
        m_toggleHeightOffset,
        false );
}

void AccessibilityTabController::onPttStop()
{
    parent->AddOffsetToUniverseCenter(
        ( vr::TrackingUniverseOrigin ) m_trackingUniverse,
        1,
        -m_toggleHeightOffset,
        false );
}

void AccessibilityTabController::onPttDisabled()
{
    if ( pttActive() )
    {
        stopPtt();
    }
    m_toggleHeightOffset = 0;
}

void AccessibilityTabController::reset()
{
    std::lock_guard<std::recursive_mutex> lock( eventLoopMutex );
    vr::VRChaperoneSetup()->RevertWorkingCopy();
    parent->AddOffsetToUniverseCenter(
        ( vr::TrackingUniverseOrigin ) m_trackingUniverse,
        1,
        m_heightOffset,
        false,
        false );
    vr::VRChaperoneSetup()->CommitWorkingCopy( vr::EChaperoneConfigFile_Live );
    m_heightOffset = 0.0f;
    emit heightOffsetChanged( m_heightOffset );
}

void AccessibilityTabController::eventLoopTick(
    vr::ETrackingUniverseOrigin universe )
{
    if ( !eventLoopMutex.try_lock() )
    {
        return;
    }

    // 151 was chosen because it's a prime number (to reduce overlap of
    // simultaneous settings updates).
    // Actual rate of updates is 151 * vsync (~11ms)
    // Values chosen based on update speed priority
    // Values in other tabs are as follows (avoid using same values):
    // AudioTabController: 89
    // ChaperoneTabController: 101
    // MoveCenterTabController: 149
    // ReviveTabController: 139
    // SettingsTabController: 157
    // SteamVRTabController: 97
    // UtilitiesTabController: 19
    if ( settingsUpdateCounter >= 151 )
    {
        if ( parent->isDashboardVisible() )
        {
            setTrackingUniverse( ( int ) universe );
        }
        settingsUpdateCounter = 0;
    }
    else
    {
        settingsUpdateCounter++;
    }
    checkPttStatus();
    eventLoopMutex.unlock();
}

} // namespace advsettings
