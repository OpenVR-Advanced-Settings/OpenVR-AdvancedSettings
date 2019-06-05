#include "media_keys.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtDBus/QtDBus>

namespace keyboardinput
{
void callMethodForAllMediaPlayerAvailableServices( const QString& method )
{
    if ( !QDBusConnection::sessionBus().isConnected() )
    {
        LOG( ERROR ) << "Media Keys: Unable to connect to DBUS session bus.";
        return;
    }

    QDBusReply<QStringList> names
        = QDBusConnection::sessionBus().interface()->registeredServiceNames();
    if ( !names.isValid() )
    {
        LOG( ERROR )
            << "Media Keys: Error getting DBUS registered service names:";
        LOG( ERROR ) << names.error().message();
    }

    foreach ( QString name, names.value() )
    {
        if ( name.contains( "org.mpris.MediaPlayer2" ) )
        {
            QDBusInterface iface( name,
                                  "/org/mpris/MediaPlayer2",
                                  "org.mpris.MediaPlayer2.Player",
                                  QDBusConnection::sessionBus() );
            iface.call( method );
        }
    }
}

void sendMediaNextSong()
{
    callMethodForAllMediaPlayerAvailableServices( "Next" );
}

void sendMediaPreviousSong()
{
    callMethodForAllMediaPlayerAvailableServices( "Previous" );
}

void sendMediaPausePlay()
{
    callMethodForAllMediaPlayerAvailableServices( "PlayPause" );
}

void sendMediaStopSong()
{
    callMethodForAllMediaPlayerAvailableServices( "Stop" );
}

} // namespace keyboardinput
