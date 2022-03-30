
#pragma once

#include <QObject>
#include "../utils/FrameRateUtils.h"
#include "../openvr/ovr_settings_wrapper.h"
#include "../openvr/lh_console_util.h"
#include <QStringListModel>
#include <QTableWidget>
#include "../openvr/lh_console_util.h"
#include <QDir>
#include "../openvr/ovr_system_wrapper.h"
#include <regex>
#include <QtWebSockets/QWebSocket>
#include <QUrl>
#include <QNetworkRequest>

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;

struct DeviceInfo
{
    int index = -1;
    std::string conDongle = "";
    std::string txName = "";
    std::string deviceName = "";
    std::string hand = "";
    std::string dongleType = "n/a";
    std::string serialNumber = "";
    bool hasName = false;
};

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
    bool m_pathRXTXInit = false;
    int m_dongleCountCur = 0;
    int m_dongleCountMax = 0;
    QString m_unparsedDongleString = "";
    QString m_last_pair_sn = "";

    std::vector<DeviceInfo> m_deviceList;

    void GatherDeviceInfo( DeviceInfo& device );
    void AddUnPairedDevice( DeviceInfo& device, std::string donSN );
    void synchSteamVR();
    std::vector<QString> getDongleSerialList( std::string deviceString );
    bool isSteamVRTracked( QString sn );

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
    QNetworkProxy m_proxy;
    QWebSocket m_webSocket;

    Q_INVOKABLE void searchRXTX();

    Q_INVOKABLE void launchBindingUI();

    Q_INVOKABLE unsigned getRXTXCount();
    Q_INVOKABLE QString getTXList( int i );
    Q_INVOKABLE QString getRXList( int i );
    Q_INVOKABLE QString getDeviceName( int i );
    Q_INVOKABLE QString getDongleType( int i );
    Q_INVOKABLE QString getDongleUsage();
    Q_INVOKABLE void pairDevice( QString sn );

public slots:
    void onConnected();
    void onDisconnect();
    void onMsgRec( QString Msg );
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
