
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
    Q_PROPERTY( bool oculusSdkToggle READ oculusSdkToggle WRITE
                    setOculusSdkToggle NOTIFY oculusSdkToggleChanged )

private:
    OverlayController* parent;

    bool m_autoStartEnabled = false;

public:
    void initStage1();
    void initStage2( OverlayController* parent );

    void dashboardLoopTick();

    bool autoStartEnabled() const;
    bool nativeChaperoneToggle();
    bool oculusSdkToggle();

public slots:
    void setAutoStartEnabled( bool value, bool notify = true );
    void setNativeChaperoneToggle( bool value, bool notify = true );
    void setOculusSdkToggle( bool value, bool notify = true );

signals:
    void autoStartEnabledChanged( bool value );
    void nativeChaperoneToggleChanged( bool value );
    void oculusSdkToggleChanged( bool value );
};

} // namespace advsettings
