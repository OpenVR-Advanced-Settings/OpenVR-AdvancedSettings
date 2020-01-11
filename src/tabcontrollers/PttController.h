
#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <string>
#include <mutex>
#include <openvr.h>

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;

class PttController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool pttEnabled READ pttEnabled WRITE setPttEnabled NOTIFY
                    pttEnabledChanged )
    Q_PROPERTY( bool pttActive READ pttActive NOTIFY pttActiveChanged )
    Q_PROPERTY( bool pttShowNotification READ pttShowNotification WRITE
                    setPttShowNotification NOTIFY pttShowNotificationChanged )

protected:
    bool m_pttEnabled = false;
    bool m_pttActive = false;
    bool m_pttShowNotification = false;

protected:
    virtual QString getSettingsName() = 0;
    virtual void onPttStart() {}
    virtual void onPttStop() {}
    virtual void onPttEnabled() {}
    virtual void onPttDisabled() {}

    virtual vr::VROverlayHandle_t getNotificationOverlayHandle()
    {
        return vr::k_ulOverlayHandleInvalid;
    }
    std::recursive_mutex eventLoopMutex;

public:
    bool pttEnabled() const;
    bool pttActive() const;
    bool pttShowNotification() const;

    void startPtt();
    void stopPtt();

    void reloadPttConfig();
    void savePttConfig();

public slots:
    void setPttEnabled( bool value, bool notify = true, bool save = true );
    void setPttShowNotification( bool value,
                                 bool notify = true,
                                 bool save = true );

signals:
    void pttEnabledChanged( bool value );
    void pttActiveChanged( bool value );
    void pttShowNotificationChanged( bool value );
};

} // namespace advsettings
