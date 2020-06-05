#include <easylogging++.h>
#include "../tabcontrollers/UtilitiesTabController.h"
#include "vr_alarm.h"
#include "../openvr/ovr_overlay_wrapper.h"
#include "../utils/update_rate.h"

namespace alarm_clock
{
using ::settings::BoolSetting;
using ::settings::getSetting;
using ::settings::IntSetting;

VrAlarm::VrAlarm()
    : m_alarm( getSetting( IntSetting::UTILITY_alarmHour ),
               getSetting( IntSetting::UTILITY_alarmMinute ),
               getSetting( IntSetting::UTILITY_alarmSecond ) )
{
    ovr_overlay_wrapper::createOverlay(
        "OVR-AlarmOverlay", "OVR-AlarmOverlay", &m_alarmOverlayHandle );

    ovr_overlay_wrapper::hideOverlay( m_alarmOverlayHandle );

    const auto filename = "/res/img/alarm/alarm_activated.png";
    ovr_overlay_wrapper::setOverlayFromFile( m_alarmOverlayHandle, filename );

    ovr_overlay_wrapper::setOverlayWidthInMeters( m_alarmOverlayHandle, 0.3f );

    vr::HmdMatrix34_t overlayTransform = { { { 1.0f, 0.0f, 0.0f, 0.12f },
                                             { 0.0f, 1.0f, 0.0f, 0.08f },
                                             { 0.0f, 0.0f, 1.0f, -0.3f } } };

    vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(
        m_alarmOverlayHandle,
        vr::k_unTrackedDeviceIndex_Hmd,
        &overlayTransform );
}

void VrAlarm::eventLoopTick()
{
    if ( updateRate.shouldSubjectNotRun(
             UpdateSubject::UtilitiesTabController ) )
    {
        return;
    }

    if ( alarmEnabled() && m_alarm.isValid() )
    {
        checkAlarmStatus();
    }
}

void VrAlarm::showAlarmNotification()
{
    ovr_overlay_wrapper::showOverlay( m_alarmOverlayHandle );
}

bool VrAlarm::alarmEnabled()
{
    return getSetting( BoolSetting::UTILITY_alarmEnabled );
}

void VrAlarm::setAlarmEnabled( const bool enabled )
{
    setSetting( BoolSetting::UTILITY_alarmEnabled, enabled );
    emit alarmEnabledChanged();
}

void VrAlarm::checkAlarmStatus()
{
    const auto timeToAlarmActivation = QTime::currentTime().secsTo( m_alarm );
    // Allow times to be set across the 24:00 hour line.
    // Setting a timer at 0100 when it's 2359 should not trigger
    // alarm immediately.
    const auto secondsOfGracePeriod = 2;
    const auto activateAlarm = timeToAlarmActivation < secondsOfGracePeriod
                               && timeToAlarmActivation > 0;
    if ( activateAlarm )
    {
        LOG( INFO ) << "Alarm fired at " << m_alarm.hour() << ":"
                    << m_alarm.minute() << ":" << m_alarm.second() << ".";
        showAlarmNotification();
        setAlarmEnabled( false );

        std::thread t(
            []( vr::VROverlayHandle_t overlayHandle ) {
                // This will freeze the overlay for 3 seconds
                std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
                ovr_overlay_wrapper::hideOverlay( overlayHandle );
            },
            m_alarmOverlayHandle );
        t.detach();
    }
}

void VrAlarm::setAlarmTimeToCurrentTime()
{
    auto now = QTime::currentTime();
    setAlarmTime( now.hour(), now.minute(), now.second() );
}

void VrAlarm::setAlarmTime( const int hour, const int minute, const int second )
{
    if ( !QTime::isValid( hour, minute, second ) )
    {
        LOG( ERROR ) << "setAlarmTime passed invalid time values: '" << hour
                     << "' hours, '" << minute << "' minutes, '" << second
                     << "' seconds.";
    }

    setSetting( IntSetting::UTILITY_alarmHour, hour );
    setSetting( IntSetting::UTILITY_alarmMinute, minute );
    setSetting( IntSetting::UTILITY_alarmSecond, second );

    m_alarm.setHMS( hour, minute, second );
    emit alarmTimeChanged();
}

void VrAlarm::modAlarmTime( const int hour, const int minute, const int second )
{
    if ( hour > 22 || hour < -22 )
    {
        LOG( ERROR ) << "modAlarmTime called with incorrect hour value: '"
                     << hour << "'. Values not applied.";
        return;
    }
    if ( minute > 58 || minute < -58 )
    {
        LOG( ERROR ) << "modAlarmTime called with incorrect minute value: '"
                     << minute << "'. Values not applied.";
        return;
    }
    if ( second > 58 || minute < -58 )
    {
        LOG( ERROR ) << "modAlarmTime called with incorrect second value: '"
                     << second << "'. Values not applied.";
        return;
    }

    auto resultingHour = getSetting( IntSetting::UTILITY_alarmHour ) + hour;
    auto resultingMinute
        = getSetting( IntSetting::UTILITY_alarmMinute ) + minute;
    auto resultingSecond
        = getSetting( IntSetting::UTILITY_alarmSecond ) + second;

    if ( resultingSecond > 59 )
    {
        resultingSecond -= 60;
        resultingMinute += 1;
    }
    else if ( resultingSecond < 0 )
    {
        resultingSecond += 60;
        resultingMinute -= 1;
    }

    if ( resultingMinute > 59 )
    {
        resultingMinute -= 60;
        resultingHour += 1;
    }
    else if ( resultingMinute < 0 )
    {
        resultingMinute += 60;
        resultingHour -= 1;
    }

    if ( resultingHour > 23 )
    {
        resultingHour = resultingHour - 24;
    }
    else if ( resultingHour < 0 )
    {
        resultingHour = resultingHour + 24;
    }

    setAlarmTime( resultingHour, resultingMinute, resultingSecond );
}

int VrAlarm::getAlarmHour()
{
    return getSetting( IntSetting::UTILITY_alarmHour );
}

int VrAlarm::getAlarmMinute()
{
    return getSetting( IntSetting::UTILITY_alarmMinute );
}

int VrAlarm::getAlarmSecond()
{
    return getSetting( IntSetting::UTILITY_alarmSecond );
}

} // namespace alarm_clock
