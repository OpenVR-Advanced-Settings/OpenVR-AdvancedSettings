
#pragma once

#include "../../third-party/nlhomann/json.hpp"
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QStringListModel>
#include <QTableWidget>
#include <QUrl>
#include <QtWebSockets/QWebSocket>

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration

class OverlayController;

struct DeviceInfo
{
    int index = -1;
    std::string conDongle;
    std::string txName;
    std::string deviceName;
    std::string hand;
    std::string dongleType = "n/a";
    std::string serialNumber;
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
    Q_PROPERTY( bool cameraCont READ cameraCont WRITE setCameraCont NOTIFY
                    cameraContChanged )
    Q_PROPERTY( bool perAppBindEnabled READ perAppBindEnabled WRITE
                    setPerAppBindEnabled NOTIFY perAppBindEnabledChanged )
    Q_PROPERTY( bool controllerPower READ controllerPower WRITE
                    setControllerPower NOTIFY controllerPowerChanged )
    Q_PROPERTY( bool noHMD READ noHMD WRITE setNoHMD NOTIFY noHMDChanged )

private:
    OverlayController* parent;

    bool m_performanceGraphToggle = false;
    bool m_systemButtonToggle = false;
    bool m_noFadeToGridToggle = false;
    bool m_multipleDriverToggle = false;
    bool m_dnd = false;
    bool m_controllerPower = false;
    bool m_noHMD = false;
    QNetworkAccessManager m_networkManagerApply;
    QNetworkAccessManager m_networkManagerUrl;
    QNetworkAccessManager m_networkManagerBind;
    QNetworkRequest m_networkRequest;
    // QNetworkReply* m_networkReply;

    bool m_cameraActive = false;
    bool m_cameraBounds = false;
    bool m_cameraCont = false;
    bool m_pathRXTXInit = false;
    int m_dongleCountCur = 0;
    int m_dongleCountMax = 0;
    QString m_unparsedDongleString = "";
    QString m_last_pair_sn = "";
    std::string m_lastAppID = "steam.overlay.1009850";
    bool m_setDefault = false;

    std::vector<DeviceInfo> m_deviceList;

    void GatherDeviceInfo( DeviceInfo& device );
    void AddUnPairedDevice( DeviceInfo& device, std::string donSN );
    void synchSteamVR();
    std::vector<QString> getDongleSerialList( std::string deviceString );
    bool isSteamVRTracked( QString serialnum );
    void applyBindingReq( std::string appID );

    QNetworkProxy m_proxy;
    QWebSocket m_webSocket;

public:
    void initStage1();
    void initStage2( OverlayController* parent );

    void eventLoopTick();
    void dashboardLoopTick();

    [[nodiscard]] bool performanceGraph() const;
    [[nodiscard]] bool noFadeToGrid() const;
    [[nodiscard]] bool multipleDriver() const;
    [[nodiscard]] bool systemButton() const;
    [[nodiscard]] bool dnd() const;
    [[nodiscard]] bool controllerPower() const;
    [[nodiscard]] bool noHMD() const;

    [[nodiscard]] bool cameraActive() const;
    [[nodiscard]] bool cameraBounds() const;
    [[nodiscard]] bool cameraCont() const;

    void getBindingUrlReq( std::string appID );
    void getBindingDataReq( std::string steamURL,
                            std::string appID,
                            std::string ctrlType );
    bool customBindExists( std::string appID = "",
                           std::string sceneAppID = "",
                           std::string ctrl = "" );
    bool defBindExists( std::string appID = "", std::string ctrl = "" );
    //    void setCustomBind( std::string appID,
    //                        std::string sceneAppID = "",
    //                        std::string ctrl = "" );
    bool saveBind( std::string appID,
                   std::string sceneAppID,
                   std::string ctrlType,
                   nlohmann::json binds,
                   bool def = false );

    [[nodiscard]] bool perAppBindEnabled() const;
    void applyAllCustomBindings();

    Q_INVOKABLE void searchRXTX();

    Q_INVOKABLE void launchBindingUI();

    Q_INVOKABLE unsigned getRXTXCount();
    Q_INVOKABLE QString getTXList( int index );
    Q_INVOKABLE QString getRXList( int index );
    Q_INVOKABLE QString getDeviceName( int index );
    Q_INVOKABLE QString getDongleType( int index );
    Q_INVOKABLE QString getDongleUsage();
    Q_INVOKABLE void pairDevice( QString serialnum );
    Q_INVOKABLE void updateRXTXList();
    Q_INVOKABLE void setBindingQMLWrapper( QString appID, bool def = false );

    void onConnected();
    void onDisconnect();
    void onMsgRec( QString Msg );
    void setPerformanceGraph( bool value, bool notify = true );
    void setSystemButton( bool value, bool notify = true );
    void setNoFadeToGrid( bool value, bool notify = true );
    void setMultipleDriver( bool value, bool notify = true );
    void setDND( bool value, bool notify = true );
    void setControllerPower( bool value, bool notify = true );
    void setNoHMD( bool value, bool notify = true );

    void setCameraActive( bool value, bool notify = true );
    void setCameraBounds( bool value, bool notify = true );
    void setCameraCont( bool value, bool notify = true );

    void setPerAppBindEnabled( bool value, bool notify = true );

    void restartSteamVR();
    void onGetBindingUrlResponse( QNetworkReply* reply );
    void onApplyBindingResponse( QNetworkReply* reply );
    void onGetBindingDataResponse( QNetworkReply* reply );

signals:

    void performanceGraphChanged( bool value );
    void multipleDriverChanged( bool value );
    void systemButtonChanged( bool value );
    void noFadeToGridChanged( bool value );
    void dNDChanged( bool value );
    void controllerPowerChanged( bool value );
    void noHMDChanged( bool value );

    void cameraActiveChanged( bool value );
    void cameraBoundsChanged( bool value );
    void cameraContChanged( bool value );

    void pairStatusChanged( QString value );
    void updateRXTX( bool value );

    void perAppBindEnabledChanged( bool value );
};

} // namespace advsettings
