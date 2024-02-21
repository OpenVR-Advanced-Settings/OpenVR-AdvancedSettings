
#pragma once

#include <QObject>
#include <QTime>
#include <openvr.h>
#include <memory>
#include "src/keyboard_input/keyboard_input.h"
#include "src/media_keys/media_keys.h"
#include "../utils/FrameRateUtils.h"

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;

class UtilitiesTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(
        bool vrcDebug READ vrcDebug WRITE setVrcDebug NOTIFY vrcDebugChanged )
    Q_PROPERTY( bool trackerOvlEnabled READ trackerOvlEnabled WRITE
                    setTrackerOvlEnabled NOTIFY trackerOvlEnabledChanged )

private:
    OverlayController* m_parent;

    unsigned settingsUpdateCounter = 0;

    vr::VROverlayHandle_t m_batteryOverlayHandles[vr::k_unMaxTrackedDeviceCount]
        = { 0 };
    int m_batteryState[vr::k_unMaxTrackedDeviceCount];
    bool m_batteryVisible[vr::k_unMaxTrackedDeviceCount];
    void handleTrackerBatOvl();
    vr::VROverlayHandle_t createBatteryOverlay( vr::TrackedDeviceIndex_t index,
                                                unsigned style = 0 );
    void destroyBatteryOverlays();

public:
    void initStage2( OverlayController* var_parent );

    void eventLoopTick();

    bool vrcDebug() const;
    bool trackerOvlEnabled() const;

public slots:
    void sendKeyboardInput( QString input );
    void sendKeyboardEnter();
    void sendKeyboardAltTab();
    void sendKeyboardAltEnter();
    void sendKeyboardCtrlC();
    void sendKeyboardCtrlV();
    void sendKeyboardBackspace( const int count );
    void sendKeyboardRShiftTilde1();
    void sendKeyboardRShiftTilde2();
    void sendKeyboardRShiftTilde3();
    void sendKeyboardRShiftTilde4();
    void sendKeyboardRShiftTilde5();
    void sendKeyboardRShiftTilde6();
    void sendKeyboardRShiftTilde7();
    void sendKeyboardRShiftTilde8();
    void sendKeyboardRShiftTilde9();
    void sendKeyboardRShiftTilde0();
    void sendKeyboardRShiftTilde1Delayed();
    void sendKeyboardRShiftTilde2Delayed();
    void sendKeyboardRShiftTilde3Delayed();
    void sendKeyboardRShiftTilde4Delayed();
    void sendKeyboardRShiftTilde5Delayed();
    void sendKeyboardRShiftTilde6Delayed();
    void sendKeyboardRShiftTilde7Delayed();
    void sendKeyboardRShiftTilde8Delayed();
    void sendKeyboardRShiftTilde9Delayed();
    void sendKeyboardRShiftTilde0Delayed();
    void sendMediaNextSong();
    void sendMediaPreviousSong();
    void sendMediaPausePlay();
    void sendMediaStopSong();
    Q_INVOKABLE void sendKeyboardOne();
    Q_INVOKABLE void sendKeyboardTwo();
    Q_INVOKABLE void sendKeyboardThree();

    void setVrcDebug( bool value, bool notify = true );
    void setTrackerOvlEnabled( bool value, bool notify = true );

signals:
    void vrcDebugChanged( bool value );
    void trackerOvlEnabledChanged( bool value );
};

} // namespace advsettings
