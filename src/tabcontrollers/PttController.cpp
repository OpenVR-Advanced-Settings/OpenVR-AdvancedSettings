#include "PttController.h"
#include <QQuickWindow>
#include <QApplication>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings
{
bool PttController::pttEnabled() const
{
    return m_pttEnabled;
}

bool PttController::pttActive() const
{
    return m_pttActive;
}

bool PttController::pttShowNotification() const
{
    return m_pttShowNotification;
}

void PttController::reloadPttConfig()
{
    std::lock_guard<std::recursive_mutex> lock( eventLoopMutex );
    auto settings = OverlayController::appSettings();
    settings->beginGroup( getSettingsName() );
    setPttEnabled(
        settings->value( "pttEnabled", false ).toBool(), true, false );
    setPttShowNotification(
        settings->value( "pttShowNotification", true ).toBool(), true, false );
    settings->endGroup();
}

void PttController::savePttConfig()
{
    auto settings = OverlayController::appSettings();
    settings->beginGroup( getSettingsName() );
    settings->setValue( "pttEnabled", pttEnabled() );
    settings->setValue( "pttShowNotification", pttShowNotification() );
    settings->endGroup();
}

void PttController::startPtt()
{
    m_pttActive = true;
    onPttStart();
    if ( m_pttShowNotification
         && getNotificationOverlayHandle() != vr::k_ulOverlayHandleInvalid )
    {
        vr::VROverlay()->ShowOverlay( getNotificationOverlayHandle() );
    }
    emit pttActiveChanged( m_pttActive );
}

void PttController::stopPtt()
{
    m_pttActive = false;
    onPttStop();
    if ( m_pttShowNotification
         && getNotificationOverlayHandle() != vr::k_ulOverlayHandleInvalid )
    {
        vr::VROverlay()->HideOverlay( getNotificationOverlayHandle() );
    }
    emit pttActiveChanged( m_pttActive );
}

void PttController::setPttEnabled( bool value, bool notify, bool save )
{
    std::lock_guard<std::recursive_mutex> lock( eventLoopMutex );
    if ( value != m_pttEnabled )
    {
        m_pttEnabled = value;
        if ( value )
        {
            onPttEnabled();
        }
        else
        {
            onPttDisabled();
        }
        if ( notify )
        {
            emit pttEnabledChanged( value );
        }
        if ( save )
        {
            savePttConfig();
        }
    }
}

void PttController::setPttShowNotification( bool value, bool notify, bool save )
{
    std::lock_guard<std::recursive_mutex> lock( eventLoopMutex );
    if ( value != m_pttShowNotification )
    {
        m_pttShowNotification = value;
        if ( notify )
        {
            emit pttShowNotificationChanged( value );
        }
        if ( save )
        {
            savePttConfig();
        }
    }
}

} // namespace advsettings
