
#pragma once

#include <QObject>

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

private:
    OverlayController* parent;

    unsigned settingsUpdateCounter = 0;

    bool m_autoStartEnabled = false;

public:
    void initStage1();
    void initStage2( OverlayController* parent );

    void dashboardLoopTick();

    bool autoStartEnabled() const;

public slots:
    void setAutoStartEnabled( bool value, bool notify = true );

signals:
    void autoStartEnabledChanged( bool value );
};

} // namespace advsettings
