
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

enum TouchPadArea
{
    PAD_AREA_LEFT = ( 1 << 0 ),
    PAD_AREA_TOP = ( 1 << 1 ),
    PAD_AREA_RIGHT = ( 1 << 2 ),
    PAD_AREA_BOTTOM = ( 1 << 3 ),
};

struct PttControllerConfig
{
    QVariantList digitalButtons;
    uint64_t digitalButtonMask = 0;
    unsigned triggerMode = 0; // 0 .. disabled, 1 .. enabled
    unsigned touchpadMode
        = 0; // 0 .. disabled, 1 .. only touch, 2 .. only press, 3 .. both
    unsigned touchpadAreas = 0;
};

struct PttProfile
{
    std::string profileName;

    bool showNotification = false;
    bool leftControllerEnabled = false;
    bool rightControllerEnabled = false;
    PttControllerConfig controllerConfigs[2];
};

class PttController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool pttEnabled READ pttEnabled WRITE setPttEnabled NOTIFY
                    pttEnabledChanged )
    Q_PROPERTY( bool pttActive READ pttActive NOTIFY pttActiveChanged )
    Q_PROPERTY( bool pttShowNotification READ pttShowNotification WRITE
                    setPttShowNotification NOTIFY pttShowNotificationChanged )
    Q_PROPERTY(
        bool pttLeftControllerEnabled READ pttLeftControllerEnabled WRITE
            setPttLeftControllerEnabled NOTIFY pttLeftControllerEnabledChanged )
    Q_PROPERTY( bool pttRightControllerEnabled READ pttRightControllerEnabled
                    WRITE setPttRightControllerEnabled NOTIFY
                        pttRightControllerEnabledChanged )

protected:
    bool m_pttEnabled = false;
    bool m_pttActive = false;
    bool m_pttShowNotification = false;
    bool m_pttLeftControllerEnabled = false;
    bool m_pttRightControllerEnabled = false;
    PttControllerConfig m_pttControllerConfigs[2];

    std::vector<PttProfile> pttProfiles;

protected:
    void checkPttStatus();
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
    bool pttLeftControllerEnabled() const;
    bool pttRightControllerEnabled() const;

    void startPtt();
    void stopPtt();

    void reloadPttProfiles();
    void reloadPttConfig();
    void savePttProfiles();
    void savePttConfig();

    virtual bool pttChangeValid()
    {
        return true;
    }

    Q_INVOKABLE QVariantList pttDigitalButtons( unsigned controller );
    Q_INVOKABLE unsigned long pttDigitalButtonMask( unsigned controller );
    Q_INVOKABLE unsigned pttTouchpadMode( unsigned controller );
    Q_INVOKABLE unsigned pttTriggerMode( unsigned controller );
    Q_INVOKABLE unsigned pttTouchpadArea( unsigned controller );

    Q_INVOKABLE unsigned getPttProfileCount();
    Q_INVOKABLE QString getPttProfileName( unsigned index );

public slots:
    void setPttEnabled( bool value, bool notify = true, bool save = true );
    void setPttShowNotification( bool value,
                                 bool notify = true,
                                 bool save = true );
    void setPttLeftControllerEnabled( bool value,
                                      bool notify = true,
                                      bool save = true );
    void setPttRightControllerEnabled( bool value,
                                       bool notify = true,
                                       bool save = true );

    void setPttControllerConfig( unsigned controller,
                                 QVariantList buttons,
                                 unsigned triggerMode,
                                 unsigned padMode,
                                 unsigned padAreas );

    void addPttProfile( QString name );
    void applyPttProfile( unsigned index );
    void deletePttProfile( unsigned index );

signals:
    void pttEnabledChanged( bool value );
    void pttActiveChanged( bool value );
    void pttShowNotificationChanged( bool value );
    void pttLeftControllerEnabledChanged( bool value );
    void pttRightControllerEnabledChanged( bool value );

    void pttProfilesUpdated();
    void pttProfileAdded();
};

} // namespace advsettings
