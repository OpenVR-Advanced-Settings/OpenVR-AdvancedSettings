
#pragma once

#include <QObject>
#include <QTime>
#include <openvr.h>
#include <memory>
#include "KeyboardInput.h"

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
    Q_PROPERTY(
        bool steamDesktopOverlayAvailable READ steamDesktopOverlayAvailable
            NOTIFY steamDesktopOverlayAvailableChanged )
    Q_PROPERTY(
        float steamDesktopOverlayWidth READ steamDesktopOverlayWidth WRITE
            setSteamDesktopOverlayWidth NOTIFY steamDesktopOverlayWidthChanged )

private:
    void setUpDesktopOverlay( float desktopWidthInMeters );

    OverlayController* m_parent;
    QQuickWindow* m_widget;

    unsigned settingsUpdateCounter = 0;

    bool m_alarmEnabled = false;
    bool m_alarmIsModal = true;
    QTime m_alarmTime;
    QTime m_alarmLastCheckTime;
    bool m_steamDesktopOverlayAvailable = false;
    float m_steamDesktopOverlayWidth = 4.0f;
    vr::VROverlayHandle_t m_batteryOverlayHandles[vr::k_unMaxTrackedDeviceCount]
        = { 0 };
    int m_batteryState[vr::k_unMaxTrackedDeviceCount];
    bool m_batteryVisible[vr::k_unMaxTrackedDeviceCount];

    std::unique_ptr<KeyboardInput> keyboardInput;

public:
    void initStage1();
    void initStage2( OverlayController* var_parent, QQuickWindow* var_widget );

    void eventLoopTick();

    bool alarmEnabled() const;
    bool alarmIsModal() const;
    int alarmTimeHour() const;
    int alarmTimeMinute() const;

    bool steamDesktopOverlayAvailable() const;
    float steamDesktopOverlayWidth() const;

public slots:
    void sendKeyboardInput( QString input );
    void sendKeyboardEnter();
    void sendKeyboardAltTab();
    void sendKeyboardAltEnter();
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

    void setSteamDesktopOverlayWidth( float width,
                                      bool notify = true,
                                      bool notifyOpenVr = true );

signals:
    void alarmEnabledChanged( bool enabled );
    void alarmIsModalChanged( bool modal );
    void alarmTimeHourChanged( int hour );
    void alarmTimeMinuteChanged( int min );

    void steamDesktopOverlayAvailableChanged( bool available );
    void steamDesktopOverlayWidthChanged( float width );
};

} // namespace advsettings
