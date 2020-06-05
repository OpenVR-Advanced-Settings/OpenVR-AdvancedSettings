#pragma once
#include <QObject>
#include <QTime>
#include <openvr.h>
#include <chrono>
#include "../settings/settings.h"
#include "../openvr/ovr_overlay_wrapper.h"

namespace alarm_clock
{
class VrAlarm : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool alarmEnabled READ alarmEnabled WRITE setAlarmEnabled NOTIFY
                    alarmEnabledChanged )

public:
    VrAlarm();

    void eventLoopTick();

    void showAlarmNotification();

    bool alarmEnabled();
    void setAlarmEnabled( const bool enabled );
    void checkAlarmStatus();

    Q_INVOKABLE void setAlarmTimeToCurrentTime();
    Q_INVOKABLE void
        setAlarmTime( const int hour, const int minute, const int second );
    Q_INVOKABLE
    void modAlarmTime( const int hour, const int minute, const int second );
    Q_INVOKABLE int getAlarmHour();
    Q_INVOKABLE int getAlarmMinute();
    Q_INVOKABLE int getAlarmSecond();

    /*
       Stopwatch
            isEnabled
            enable
            Start
            Pause
            Stop
            Restart

       Timer
            isEnabled
            enable
            Start
            Pause
            Stop
            Restart
    */
signals:
    void alarmEnabledChanged();
    void alarmTimeChanged();
    void alarmFired();

private:
    QTime m_alarm;
    vr::VROverlayHandle_t m_alarmOverlayHandle = 0;
};

} // namespace alarm_clock
