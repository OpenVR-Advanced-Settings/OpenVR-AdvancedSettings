
#pragma once

#include <QObject>
#include "../utils/FrameRateUtils.h"

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;

class SteamVRTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool systemButton READ systemButton WRITE setSystemButton NOTIFY
                    systemButtonChanged )
    Q_PROPERTY( bool multipleDriver READ multipleDriver WRITE setMultipleDriver
                    NOTIFY multipleDriverChanged )

    Q_PROPERTY( bool noFadeToGrid READ noFadeToGrid WRITE setNoFadeToGrid NOTIFY
                    noFadeToGridChanged )

    Q_PROPERTY( bool performanceGraph READ performanceGraph WRITE
                    setPerformanceGraph NOTIFY performanceGraphChanged )
    Q_PROPERTY( bool dnd READ dnd WRITE setDND NOTIFY dNDChanged )

    Q_PROPERTY( bool cameraActive READ cameraActive WRITE setCameraActive NOTIFY
                    cameraActiveChanged )
    Q_PROPERTY( bool cameraBounds READ cameraBounds WRITE setCameraBounds NOTIFY
                    cameraBoundsChanged )
    Q_PROPERTY( bool cameraRoom READ cameraRoom WRITE setCameraRoom NOTIFY
                    cameraRoomChanged )
    Q_PROPERTY( bool cameraDashboard READ cameraDashboard WRITE
                    setCameraDashboard NOTIFY cameraDashboardChanged )

private:
    OverlayController* parent;

    bool m_performanceGraphToggle = false;
    bool m_systemButtonToggle = false;
    bool m_noFadeToGridToggle = false;
    bool m_multipleDriverToggle = false;
    bool m_dnd = false;

    bool m_cameraActive = false;
    bool m_cameraBounds = false;
    bool m_cameraRoom = false;
    bool m_cameraDashboard = false;

    unsigned settingsUpdateCounter = 0;

    unsigned int m_steamVrSettingsUpdateCounter = 97;

public:
    void initStage1();
    void initStage2( OverlayController* parent );

    void eventLoopTick();
    void dashboardLoopTick();

    bool performanceGraph() const;
    bool noFadeToGrid() const;
    bool multipleDriver() const;
    bool systemButton() const;
    bool dnd() const;

    bool cameraActive() const;
    bool cameraBounds() const;
    bool cameraRoom() const;
    bool cameraDashboard() const;

    Q_INVOKABLE void launchBindingUI();

public slots:

    void setPerformanceGraph( bool value, bool notify = true );
    void setSystemButton( bool value, bool notify = true );
    void setNoFadeToGrid( bool value, bool notify = true );
    void setMultipleDriver( bool value, bool notify = true );
    void setDND( bool value, bool notify = true );

    void setCameraActive( bool value, bool notify = true );
    void setCameraBounds( bool value, bool notify = true );
    void setCameraRoom( bool value, bool notify = true );
    void setCameraDashboard( bool value, bool notify = true );

    void restartSteamVR();

signals:

    void performanceGraphChanged( bool value );
    void multipleDriverChanged( bool value );
    void systemButtonChanged( bool value );
    void noFadeToGridChanged( bool value );
    void dNDChanged( bool value );

    void cameraActiveChanged( bool value );
    void cameraBoundsChanged( bool value );
    void cameraRoomChanged( bool value );
    void cameraDashboardChanged( bool value );
};

} // namespace advsettings
