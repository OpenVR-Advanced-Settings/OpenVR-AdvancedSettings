
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
    Q_PROPERTY( bool forceRevivePage READ forceRevivePage WRITE
                    setForceRevivePage NOTIFY forceRevivePageChanged )

private:
    OverlayController* parent;

    unsigned settingsUpdateCounter = 0;

    bool m_autoStartEnabled = false;
    bool m_forceRevivePage = false;

public:
    void initStage1();
    void initStage2( OverlayController* parent );

    void eventLoopTick();

    bool autoStartEnabled() const;
    bool forceRevivePage() const;

public slots:
    void setAutoStartEnabled( bool value, bool notify = true );
    void setForceRevivePage( bool value, bool notify = true );

signals:
    void autoStartEnabledChanged( bool value );
    void forceRevivePageChanged( bool value );
};

} // namespace advsettings
