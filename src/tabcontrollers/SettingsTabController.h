
#pragma once

#include <QObject>
#include "../utils/FrameRateUtils.h"
#include "../openvr/ovr_settings_wrapper.h"

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;

class SettingsTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool autoStartEnabled READ autoStartEnabled WRITE
                    setAutoStartEnabled NOTIFY autoStartEnabledChanged )
    Q_PROPERTY(
        bool nativeChaperoneToggle READ nativeChaperoneToggle WRITE
            setNativeChaperoneToggle NOTIFY nativeChaperoneToggleChanged )

private:
    OverlayController* parent;

    unsigned settingsUpdateCounter = 0;

    bool m_autoStartEnabled = false;
    bool m_nativeChaperoneToggle = false;

    unsigned int m_settingsTabSettingsUpdateCounter = 157;

public:
    void initStage1();
    void initStage2( OverlayController* parent );

    void dashboardLoopTick();

    bool autoStartEnabled() const;
    bool nativeChaperoneToggle();

public slots:
    void setAutoStartEnabled( bool value, bool notify = true );
    void setNativeChaperoneToggle( bool value, bool notify = true );

signals:
    void autoStartEnabledChanged( bool value );
    void nativeChaperoneToggleChanged( bool value );
};

} // namespace advsettings
