
#pragma once

#include <QObject>
#include <QTime>
#include <openvr.h>
#include <memory>
#include "src/keyboard_input/keyboard_input.h"
#include "src/media_keys/media_keys.h"

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;

class UtilitiesTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool alarmEnabled READ alarmEnabled WRITE setAlarmEnabled NOTIFY
                    alarmEnabledChanged )
    Q_PROPERTY( bool alarmIsModal READ alarmIsModal WRITE setAlarmIsModal NOTIFY
                    alarmIsModalChanged )
    Q_PROPERTY( int alarmTimeHour READ alarmTimeHour WRITE setAlarmTimeHour
                    NOTIFY alarmTimeHourChanged )
    Q_PROPERTY( int alarmTimeMinute READ alarmTimeMinute WRITE
                    setAlarmTimeMinute NOTIFY alarmTimeMinuteChanged )

private:
    OverlayController* m_parent;

    unsigned settingsUpdateCounter = 0;

    bool m_alarmEnabled = false;
    bool m_alarmIsModal = true;
    QTime m_alarmTime;
    QTime m_alarmLastCheckTime;

    vr::VROverlayHandle_t m_batteryOverlayHandles[vr::k_unMaxTrackedDeviceCount]
        = { 0 };
    int m_batteryState[vr::k_unMaxTrackedDeviceCount];
    bool m_batteryVisible[vr::k_unMaxTrackedDeviceCount];

public:
    void initStage1();
    void initStage2( OverlayController* var_parent );

    void eventLoopTick();

    bool alarmEnabled() const;
    bool alarmIsModal() const;
    int alarmTimeHour() const;
    int alarmTimeMinute() const;

public slots:
    void sendKeyboardInput( QString input );
    void sendKeyboardEnter();
    void sendKeyboardAltTab();
    void sendKeyboardAltEnter();
    void sendKeyboardCtrlC();
    void sendKeyboardCtrlV();
    void sendKeyboardBackspace( const int count );
    void sendMediaNextSong();
    void sendMediaPreviousSong();
    void sendMediaPausePlay();
    void sendMediaStopSong();

    void setAlarmEnabled( bool enabled, bool notify = true );
    void setAlarmIsModal( bool modal, bool notify = true );
    void setAlarmTimeHour( int hour, bool notify = true );
    void setAlarmTimeMinute( int min, bool notify = true );
    void setAlarmTimeToCurrentTime();
    void modAlarmTimeHour( int value, bool notify = true );
    void modAlarmTimeMinute( int value, bool notify = true );

signals:
    void alarmEnabledChanged( bool enabled );
    void alarmIsModalChanged( bool modal );
    void alarmTimeHourChanged( int hour );
    void alarmTimeMinuteChanged( int min );
};

} // namespace advsettings
