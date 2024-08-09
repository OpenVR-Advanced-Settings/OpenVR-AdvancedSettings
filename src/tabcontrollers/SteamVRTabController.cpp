#include "SteamVRTabController.h"
#include <QQuickWindow>
#include <QtLogging>
#include <QtDebug>
#include "../overlaycontroller.h"
#include "../utils/update_rate.h"
#include "openvr.h"
#include "openvr/ovr_application_wrapper.h"
#include "openvr/ovr_settings_wrapper.h"
#include "openvr/ovr_system_wrapper.h"
#include "settings/settings.h"
#include <QDesktopServices>
#include <regex>
#include <set>

QT_USE_NAMESPACE

// application namespace
namespace advsettings
{
void SteamVRTabController::initStage1()
{
    dashboardLoopTick();
}

void SteamVRTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
    synchSteamVR();
}

void SteamVRTabController::dashboardLoopTick()
{
    if ( updateRate.shouldSubjectNotRun( UpdateSubject::SteamVrTabController ) )
    {
        return;
    }
    synchSteamVR();
}

void SteamVRTabController::synchSteamVR()
{
    // Un-comment these if other Apps make heavy use OR ADDED to STEAMVR
    // officially
    setPerformanceGraph( performanceGraph() );
    // setSystemButton(systemButton());
    setMultipleDriver( multipleDriver() );
    setDND( dnd() );
    setNoFadeToGrid( noFadeToGrid() );
    setCameraActive( cameraActive() );
    setCameraCont( cameraCont() );
    setCameraBounds( cameraBounds() );
    setControllerPower( controllerPower() );
    setNoHMD( noHMD() );
}

bool SteamVRTabController::performanceGraph() const
{
    auto pair = ovr_settings_wrapper::getBool(
        vr::k_pch_Perf_Section, vr::k_pch_Perf_PerfGraphInHMD_Bool );

    if ( pair.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return pair.second;
    }
    return m_performanceGraphToggle;
}

void SteamVRTabController::setPerformanceGraph( const bool value,
                                                const bool notify )
{
    if ( m_performanceGraphToggle != value )
    {
        m_performanceGraphToggle = value;
        ovr_settings_wrapper::setBool( vr::k_pch_Perf_Section,
                                       vr::k_pch_Perf_PerfGraphInHMD_Bool,
                                       m_performanceGraphToggle );
        if ( notify )
        {
            emit performanceGraphChanged( m_performanceGraphToggle );
        }
    }
}

bool SteamVRTabController::noHMD() const
{
    auto pair = ovr_settings_wrapper::getBool(
        vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_RequireHmd_String );

    if ( pair.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return pair.second;
    }
    return m_noHMD;
}

void SteamVRTabController::setNoHMD( const bool value, const bool notify )
{
    if ( m_noHMD != value )
    {
        m_noHMD = value;
        ovr_settings_wrapper::setBool( vr::k_pch_SteamVR_Section,
                                       vr::k_pch_SteamVR_RequireHmd_String,
                                       m_noHMD );
        if ( notify )
        {
            emit noHMDChanged( m_noHMD );
        }
    }
}

bool SteamVRTabController::multipleDriver() const
{
    auto pair = ovr_settings_wrapper::getBool(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_ActivateMultipleDrivers_Bool );

    if ( pair.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return pair.second;
    }
    return m_multipleDriverToggle;
}

void SteamVRTabController::setMultipleDriver( const bool value,
                                              const bool notify )
{
    if ( m_multipleDriverToggle != value )
    {
        m_multipleDriverToggle = value;
        ovr_settings_wrapper::setBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_ActivateMultipleDrivers_Bool,
            m_multipleDriverToggle );
        if ( notify )
        {
            emit multipleDriverChanged( m_multipleDriverToggle );
        }
    }
}

bool SteamVRTabController::noFadeToGrid() const
{
    auto pair = ovr_settings_wrapper::getBool(
        vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_DoNotFadeToGrid );

    if ( pair.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return pair.second;
    }
    return m_noFadeToGridToggle;
}

void SteamVRTabController::setNoFadeToGrid( const bool value,
                                            const bool notify )
{
    if ( m_noFadeToGridToggle != value )
    {
        m_noFadeToGridToggle = value;
        ovr_settings_wrapper::setBool( vr::k_pch_SteamVR_Section,
                                       vr::k_pch_SteamVR_DoNotFadeToGrid,
                                       m_noFadeToGridToggle );
        if ( notify )
        {
            emit noFadeToGridChanged( m_noFadeToGridToggle );
        }
    }
}

bool SteamVRTabController::controllerPower() const
{
    auto pair = ovr_settings_wrapper::getBool(
        vr::k_pch_Power_Section,
        vr::k_pch_Power_AutoLaunchSteamVROnButtonPress );
    if ( pair.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return pair.second;
    }
    return m_controllerPower;
}

void SteamVRTabController::setControllerPower( const bool value,
                                               const bool notify )
{
    if ( m_controllerPower != value )
    {
        m_controllerPower = value;
        ovr_settings_wrapper::setBool(
            vr::k_pch_Power_Section,
            vr::k_pch_Power_AutoLaunchSteamVROnButtonPress,
            m_controllerPower );
        if ( notify )
        {
            emit controllerPowerChanged( m_controllerPower );
        }
    }
}

bool SteamVRTabController::systemButton() const
{
    auto pair = ovr_settings_wrapper::getBool(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_SendSystemButtonToAllApps_Bool );

    if ( pair.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return pair.second;
    }
    return m_systemButtonToggle;
}

void SteamVRTabController::setSystemButton( const bool value,
                                            const bool notify )
{
    if ( m_systemButtonToggle != value )
    {
        m_systemButtonToggle = value;
        ovr_settings_wrapper::setBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_SendSystemButtonToAllApps_Bool,
            m_systemButtonToggle );
        if ( notify )
        {
            emit systemButtonChanged( m_systemButtonToggle );
        }
    }
}

bool SteamVRTabController::dnd() const
{
    auto pair = ovr_settings_wrapper::getBool(
        vr::k_pch_Notifications_Section,
        vr::k_pch_Notifications_DoNotDisturb_Bool );

    if ( pair.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return pair.second;
    }
    return m_dnd;
}

void SteamVRTabController::setDND( const bool value, const bool notify )
{
    if ( m_dnd != value )
    {
        m_dnd = value;
        ovr_settings_wrapper::setBool(
            vr::k_pch_Notifications_Section,
            vr::k_pch_Notifications_DoNotDisturb_Bool,
            m_dnd );
        if ( notify )
        {
            emit dNDChanged( m_dnd );
        }
    }
}

/*------------------------------------------*/
/* -----------Camera Setting----------------*/

bool SteamVRTabController::cameraActive() const
{
    auto pair = ovr_settings_wrapper::getBool(
        vr::k_pch_Camera_Section, vr::k_pch_Camera_EnableCamera_Bool );

    if ( pair.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return pair.second;
    }
    return m_cameraActive;
}

void SteamVRTabController::setCameraActive( const bool value,
                                            const bool notify )
{
    if ( m_cameraActive != value )
    {
        m_cameraActive = value;
        ovr_settings_wrapper::setBool( vr::k_pch_Camera_Section,
                                       vr::k_pch_Camera_EnableCamera_Bool,
                                       m_cameraActive );
        if ( notify )
        {
            emit cameraActiveChanged( m_cameraActive );
        }
    }
}

bool SteamVRTabController::cameraBounds() const
{
    auto pair = ovr_settings_wrapper::getBool(
        vr::k_pch_Camera_Section,
        vr::k_pch_Camera_EnableCameraForCollisionBounds_Bool );

    if ( pair.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return pair.second;
    }
    return m_cameraBounds;
}

void SteamVRTabController::setCameraBounds( const bool value,
                                            const bool notify )
{
    if ( m_cameraBounds != value )
    {
        m_cameraBounds = value;
        ovr_settings_wrapper::setBool(
            vr::k_pch_Camera_Section,
            vr::k_pch_Camera_EnableCameraForCollisionBounds_Bool,
            m_cameraBounds );
        if ( notify )
        {
            emit cameraBoundsChanged( m_cameraBounds );
        }
    }
}

bool SteamVRTabController::cameraCont() const
{
    auto pair = ovr_settings_wrapper::getBool(
        vr::k_pch_Camera_Section, vr::k_pch_Camera_ShowOnController_Bool );

    if ( pair.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return pair.second;
    }
    return m_cameraCont;
}

void SteamVRTabController::setCameraCont( const bool value, const bool notify )
{
    if ( m_cameraCont != value )
    {
        m_cameraCont = value;
        ovr_settings_wrapper::setBool( vr::k_pch_Camera_Section,
                                       vr::k_pch_Camera_ShowOnController_Bool,
                                       m_cameraCont );
        if ( notify )
        {
            emit cameraContChanged( m_cameraCont );
        }
    }
}

void SteamVRTabController::updateRXTXList()
{
    emit updateRXTX( true );
}

void SteamVRTabController::searchRXTX()
{
    m_deviceList.clear();
    int hmdIndex = -1;
    auto indexList = ovr_system_wrapper::getAllConnectedDevices( true );
    m_dongleCountCur = 0;
    for ( auto device : indexList )
    {
        if ( vr::TrackedDeviceClass_HMD
             == ovr_system_wrapper::getDeviceClass( device ) )
        {
            hmdIndex = device;
            continue;
        }
        m_deviceList.emplace_back();
        GatherDeviceInfo( m_deviceList.back() );
    }

    if ( hmdIndex != -1 )
    {
        auto dongleList
            = ovr_system_wrapper::getStringTrackedProperty(
                  hmdIndex, vr::Prop_AllWirelessDongleDescriptions_String )
                  .second;
        m_unparsedDongleString = QString::fromStdString( dongleList );
        int count = 0;
        if ( !dongleList.empty() )
        {
            count++;
        }
        for ( auto cha : dongleList )
        {
            if ( cha == ';' )
            {
                count++;
            }
        }
        m_dongleCountMax = count;
        if ( static_cast<int>( m_deviceList.size() ) < m_dongleCountMax )
        {
            auto DSNFullList = getDongleSerialList( dongleList );
            for ( auto dSN : DSNFullList )
            {
                bool isPaired = false;
                for ( auto pairedDev : m_deviceList )
                {
                    if ( QString::fromStdString( pairedDev.conDongle ) == dSN )
                    {
                        isPaired = true;
                        break;
                    }
                }
                if ( isPaired )
                {
                    continue;
                }
                m_deviceList.emplace_back();
                AddUnPairedDevice( m_deviceList.back(), dSN.toStdString() );
                // TODO create
            }
        }
    }
}

void SteamVRTabController::AddUnPairedDevice( DeviceInfo& device,
                                              std::string donSN )
{
    device.conDongle = donSN;
    if ( donSN.find( "-RYB" ) != std::string::npos
         || donSN.find( "-LYM" ) != std::string::npos )
    {
        device.dongleType = "Headset";
    }
    // TODO 1yx SN
    else if ( std::regex_match( donSN, std::regex( "(.*)(-[0-9]YX)" ) ) )
    {
        device.dongleType = "Tundra Dongle";
    }
    else
    {
        device.dongleType = "Standard Dongle";
    }
    device.index = -1;
    device.txName = "No Connection";
    device.serialNumber = "n/a";
    device.deviceName = "No Connection";
}

void SteamVRTabController::GatherDeviceInfo( DeviceInfo& device )
{
    std::string condev
        = ovr_system_wrapper::getStringTrackedProperty(
              device.index, vr::Prop_ConnectedWirelessDongle_String )
              .second;
    if ( condev.empty() || condev == " " )
    {
        condev = "n/a";
        device.dongleType = "n/a";
    }
    else
    {
        m_dongleCountCur++;
        if ( condev.find( "-RYB" ) != std::string::npos
             || condev.find( "-LYM" ) != std::string::npos )
        {
            device.dongleType = "Headset";
        }
        // TODO 1yx SN
        else if ( std::regex_match( condev, std::regex( "(.*)(-[0-9]YX)" ) ) )
        {
            device.dongleType = "Tundra Dongle";
        }
        else
        {
            device.dongleType = "Standard Dongle";
        }
    }
    device.conDongle = condev;

    std::string devicenumber = ovr_system_wrapper::getStringTrackedProperty(
                                   device.index, vr::Prop_SerialNumber_String )
                                   .second;
    if ( devicenumber.empty() || devicenumber == " " )
    {
        devicenumber = "n/a";
    }
    device.txName = devicenumber;

    vr::TrackedDeviceClass const devClass
        = ovr_system_wrapper::getDeviceClass( device.index );
    if ( devClass == vr::TrackedDeviceClass_HMD
         || devClass == vr::TrackedDeviceClass_Controller
         || devClass == vr::TrackedDeviceClass_GenericTracker )
    {
        device.hasName = true;
        device.deviceName = ovr_system_wrapper::getDeviceName( device.index );
    }
    else
    {
        if ( device.txName.find( "LHB-" ) != std::string::npos )
        {
            device.deviceName = "Lighthouse";
        }
    }
    int const role = ovr_system_wrapper::getInt32TrackedProperty(
                         device.index, vr::Prop_ControllerRoleHint_Int32 )
                         .second;
    if ( role == 1 )
    {
        device.deviceName += " (L)";
    }
    else if ( role == 2 )
    {
        device.deviceName += " (R)";
    }
}

void SteamVRTabController::launchBindingUI()
{
    vr::VRActionSetHandle_t const actionHandle = 0;
    vr::VRInputValueHandle_t inputHandle = 0;

    auto error2 = vr::VRInput()->GetInputSourceHandle( "/user/hand/right",
                                                       &inputHandle );
    if ( error2 != vr::VRInputError_None )
    {
        qCritical()
            << "failed to get input handle? is your right controller on?";
    }
    if ( parent->isDesktopMode() )
    {
        QDesktopServices::openUrl(
            QUrl( "http://127.0.0.1:27062/dashboard/controllerbinding.html" ) );
        return;
    }
    auto error = vr::VRInput()->OpenBindingUI(
        application_strings::applicationKey, actionHandle, inputHandle, false );
    if ( error != vr::VRInputError_None )
    {
        qCritical() << "Input Error: " << error;
    }
}
void SteamVRTabController::restartSteamVR()
{
    QString const cmd = QString( "cmd.exe /C restartvrserver.bat \"" )
                        + parent->getVRRuntimePathUrl().toLocalFile()
                        + QString( "\"" );
    qInfo() << "SteamVR Restart Command: " << cmd;
    QProcess::startDetached( cmd );
}

Q_INVOKABLE unsigned SteamVRTabController::getRXTXCount()
{
    return static_cast<unsigned>( m_deviceList.size() );
}

Q_INVOKABLE QString SteamVRTabController::getTXList( int index )
{
    return QString::fromStdString( m_deviceList[index].txName );
}

Q_INVOKABLE QString SteamVRTabController::getDeviceName( int index )
{
    return QString::fromStdString( m_deviceList[index].deviceName );
}

Q_INVOKABLE QString SteamVRTabController::getRXList( int index )
{
    return QString::fromStdString( m_deviceList[index].conDongle );
}
Q_INVOKABLE QString SteamVRTabController::getDongleType( int index )
{
    return QString::fromStdString( m_deviceList[index].dongleType );
}
Q_INVOKABLE QString SteamVRTabController::getDongleUsage()
{
    return QString::fromStdString( std::to_string( m_dongleCountCur ) + "/"
                                   + std::to_string( m_dongleCountMax ) );
}
Q_INVOKABLE void SteamVRTabController::pairDevice( QString serialNumber )
{
    if ( !isSteamVRTracked( serialNumber ) )
    {
        qWarning() << serialNumber.toStdString()
                   << " Is Not a SteamVR Dongle, skipping Pair";
        return;
    }
    m_last_pair_sn = serialNumber;
    auto req = QNetworkRequest();
    req.setUrl( QUrl( "ws://127.0.0.1:27062" ) );
    req.setRawHeader(
        QByteArray( "Referer" ),
        QByteArray(
            "http://localhost:27062/lighthouse/webinterface/pairing.html" ) );
    m_webSocket.open( req );
    connect( &m_webSocket,
             &QWebSocket::connected,
             this,
             &SteamVRTabController::onConnected );
    connect( &m_webSocket,
             &QWebSocket::disconnected,
             this,
             &SteamVRTabController::onDisconnect );
    connect( &m_webSocket,
             &QWebSocket::textMessageReceived,
             this,
             &SteamVRTabController::onMsgRec );
}
bool SteamVRTabController::isSteamVRTracked( QString serialNumber )
{
    return m_unparsedDongleString.contains( serialNumber );
}
void SteamVRTabController::onConnected()
{
    m_webSocket.sendTextMessage( QStringLiteral( "mailbox_open OVRAS_pair" ) );
    QString messageout = "mailbox_send lighthouse_pairing "
                         "{\"type\":\"start_pairing\", "
                         "\"returnAddress\":\"OVRAS_pair\", "
                         "\"serial\":\"";
    messageout.append( m_last_pair_sn );
    messageout.append( QString::fromStdString( R"(", "timeoutSeconds":15})" ) );
    emit pairStatusChanged( QString( "Pairing..." ) );
    if ( m_last_pair_sn == "" )
    {
        qCritical() << "No Last SN to pair, this shouldn't happen";
    }
    m_webSocket.sendTextMessage( messageout );
    // m_webSocket.close();
}
void SteamVRTabController::onDisconnect()
{
    qInfo() << "Pair WS disconnect";
    m_webSocket.close();
}
void SteamVRTabController::onMsgRec( QString Msg )
{
    if ( Msg.contains( "success" ) )
    {
        qInfo() << "Pair Success";
        emit pairStatusChanged( QString( "Success" ) );
    }
    if ( Msg.contains( "timeout" ) )
    {
        qInfo() << "Pair Timeout";
        emit pairStatusChanged( QString( "Timeout" ) );
    }
    m_webSocket.close();
}
std::vector<QString>
    SteamVRTabController::getDongleSerialList( std::string deviceString )
{
    std::vector<QString> dongleList;
    // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
    size_t pos = 0;

    // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
    while ( ( pos = deviceString.find( ',' ) ) != std::string::npos )
    {
        dongleList.push_back(
            QString::fromStdString( deviceString.substr( 0, pos ) ) );
        pos = deviceString.find( ';' );
        if ( pos != std::string::npos )
        {
            deviceString.erase( 0, pos + 1 );
            continue;
        }

        pos = deviceString.find( ',' );
        if ( pos == std::string::npos )
        {
            break;
        }
        dongleList.push_back(
            QString::fromStdString( deviceString.substr( 0, pos ) ) );
        break;

        break;
    }
    return dongleList;
}

// Binding Functions
void SteamVRTabController::getBindingUrlReq( std::string appID )
{
    m_lastAppID = appID;
    std::string const urls
        = "http://localhost:27062/input/getactions.json?app_key=" + appID;
    QUrl const url = QUrl( urls.c_str() );
    QNetworkRequest request;
    request.setUrl( url );
    // This is Important as otherwise Valve's VRWebServerWillIgnore the Request
    request.setRawHeader(
        QByteArray( "Referer" ),
        QByteArray(
            "http://localhost:27062/dashboard/controllerbinding.html" ) );
    // TODO start Timeout timer?
    connect( &m_networkManagerUrl,
             SIGNAL( finished( QNetworkReply* ) ),
             this,
             SLOT( onGetBindingUrlResponse( QNetworkReply* ) ) );
    m_networkManagerUrl.get( request );
}
void SteamVRTabController::onGetBindingUrlResponse( QNetworkReply* reply )
{
    QString const data = QString::fromUtf8( reply->readAll() );
    if ( data.size() < 2 )
    {
        return;
    }
    std::string const controllerName = ovr_system_wrapper::getControllerName();
    if ( controllerName.empty() )
    {
        qWarning() << "No Controller Detected Skipping Bindings";
        return;
    }
    nlohmann::json jsonfull = nlohmann::json::parse( data.toStdString() );
    //     qInfo () << "URL RESPOSNE XXXXXXX";
    //     qInfo () << jsonfull.dump().c_str();
    std::string const filepath
        = jsonfull["current_binding_url"][controllerName].get<std::string>();
    qInfo() << "binding url at " << filepath;
    // TODO perhaps some form of error checking if packet wrong?
    reply->close();
    getBindingDataReq( filepath, m_lastAppID, controllerName );
}

void SteamVRTabController::getBindingDataReq( std::string steamURL,
                                              std::string appID,
                                              std::string ctrlType )
{
    std::string const urls = "http://localhost:27062/input/"
                             "loadbindingfromurl.json?binding_url="
                             + steamURL + "&controller_type=" + ctrlType
                             + "&app_key=" + appID;
    QUrl const url = QUrl( urls.c_str() );
    QNetworkRequest request;
    request.setUrl( url );
    // This is Important as otherwise Valve's VRWebServerWillIgnore the Request
    request.setRawHeader(
        QByteArray( "Referer" ),
        QByteArray(
            "http://localhost:27062/dashboard/controllerbinding.html" ) );
    // TODO start Timeout timer?
    connect( &m_networkManagerBind,
             SIGNAL( finished( QNetworkReply* ) ),
             this,
             SLOT( onGetBindingDataResponse( QNetworkReply* ) ) );
    m_networkManagerBind.get( request );
}
void SteamVRTabController::onGetBindingDataResponse( QNetworkReply* reply )
{
    nlohmann::json output = "";
    QString const data = QString::fromUtf8( reply->readAll() );
    if ( data.size() < 2 )
    {
        return;
    }
    //     qWarning () << "DATA RESPONSE";
    //     qWarning () << data.toStdString();
    nlohmann::json jsonfull = nlohmann::json::parse( data.toStdString() );
    if ( !jsonfull.contains( "success" ) )
    {
        qCritical() << "Binding Data Packet Mal-Formed?";
    }
    if ( !jsonfull["success"].get<bool>() )
    {
        // TODO better error handling?
        qCritical() << "Binding Data Request Failed";
    }
    reply->close();
    output = jsonfull["binding_config"];
    std::string const sceneAppID = ovr_application_wrapper::getSceneAppID();
    if ( sceneAppID == "error" )
    {
        qCritical() << "aborting bind save, could not find scene app ID";
        return;
    }
    std::string const ctrl = ovr_system_wrapper::getControllerName();
    saveBind( m_lastAppID, sceneAppID, ctrl, output, m_setDefault );
}

bool SteamVRTabController::saveBind( std::string appID,
                                     std::string sceneAppID,
                                     std::string ctrlType,
                                     nlohmann::json binds,
                                     bool def )
{
    m_setDefault = false;
    QFileInfo const fileInfo(
        QString::fromStdString( settings::initializeAndGetSettingsPath() ) );
    QDir const directory = fileInfo.absolutePath();
    QString fileName;
    if ( def )
    {
        fileName = QString::fromStdString( "defovl" + appID + "ctrl" + ctrlType
                                           + ".json" );
    }
    else
    {
        fileName = QString::fromStdString( "ovl" + appID + "scene" + sceneAppID
                                           + "ctrl" + ctrlType + ".json" );
    }
    QString const absPath = directory.absolutePath() + "/" + fileName;

    QFile bindFile( absPath );
    bindFile.open( QIODevice::ReadWrite | QIODevice::Truncate
                   | QIODevice::Text );
    QByteArray const qba = binds.dump().c_str();
    bindFile.write( qba );
    bindFile.flush();
    bindFile.close();
    if ( bindFile.exists() )
    {
        qInfo() << ( def ? "Default " : ( sceneAppID + " " ) )
                       + "Binding File saved at:"
                << absPath.toStdString();
        std::string const jsonstring = binds.dump();
        qInfo() << jsonstring;
        return true;
    }
    return false;
}

bool SteamVRTabController::customBindExists( std::string appID,
                                             std::string sceneAppID,
                                             std::string ctrl )
{
    if ( appID.empty() )
    {
        appID = m_lastAppID;
    }
    if ( sceneAppID.empty() )
    {
        sceneAppID = ovr_application_wrapper::getSceneAppID();
    }
    if ( ctrl.empty() )
    {
        ctrl = ovr_system_wrapper::getControllerName();
    }
    QFileInfo const fileInfo( QString::fromStdString(
        settings::initializeAndGetSettingsPath() + "ovl" + appID + "scene"
        + sceneAppID + "ctrl" + ctrl + ".json" ) );
    return fileInfo.exists();
}

bool SteamVRTabController::defBindExists( std::string appID, std::string ctrl )
{
    if ( appID.empty() )
    {
        appID = m_lastAppID;
    }
    if ( ctrl.empty() )
    {
        ctrl = ovr_system_wrapper::getControllerName();
    }
    QFileInfo const fileInfo( QString::fromStdString(
        settings::initializeAndGetSettingsPath() + "defovl" + appID + "ctrl"
        + ctrl + ".json" ) );
    return fileInfo.exists();
}

void SteamVRTabController::applyBindingReq( std::string appID )
{
    std::string const ctrlType = ovr_system_wrapper::getControllerName();
    QFileInfo const fileInfo(
        QString::fromStdString( settings::initializeAndGetSettingsPath() ) );
    QDir const directory = fileInfo.absolutePath();
    QString fileName;
    std::string const sceneAppID = ovr_application_wrapper::getSceneAppID();
    if ( sceneAppID.empty() )
    {
        qCritical() << "NO Scene App Detected unable to apply bindings";
        return;
    }

    // TODO check forward slash compatibility with linux
    fileName = QString::fromStdString( "ovl" + appID + "scene" + sceneAppID
                                       + "ctrl" + ctrlType + ".json" );
    if ( !QFileInfo::exists( fileInfo.absolutePath() + QString( "/" )
                             + fileName ) )
    {
        qInfo() << "No Specific Binding Detected for: " + appID
                       + "for Scene: " + sceneAppID + " Checking Default";
        fileName = QString::fromStdString( "defovl" + appID + "ctrl" + ctrlType
                                           + ".json" );
        if ( !QFileInfo::exists( fileInfo.absolutePath() + QString( "/" )
                                 + fileName ) )
        {
            qInfo() << "No Def Binding Detected for: " + appID
                           + " Not Adjusting Bindings";
            return;
        }
    }
    // TODO possible linux compatibility issue
    QString const absPath = directory.absolutePath() + "/" + fileName;
    QUrl const urlized = QUrl::fromLocalFile( absPath );
    // std::string filePath = "file:///" + absPath.toStdString();
    std::string const url = "http://localhost:27062/input/selectconfig.action";
    //  qInfo () << urlized.toEncoded().toStdString();
    QUrl const urls = QUrl( url.c_str() );
    QNetworkRequest request;
    request.setUrl( urls );
    qInfo() << "Attempting to Apply Binding at: "
            << urlized.toEncoded( QUrl::EncodeSpaces | QUrl::EncodeReserved )
                   .toStdString();
    // This is Important as otherwise Valve's VRWebServerWillIgnore the Request
    // If referrer is wrong
    request.setHeader( QNetworkRequest::ContentTypeHeader,
                       "application/x-www-form-urlencoded" );
    request.setRawHeader(
        QByteArray( "Referer" ),
        QByteArray(
            "http://localhost:27062/dashboard/controllerbinding.html" ) );
    QByteArray const data
        = ( R"({"app_key":")" + appID + R"(","controller_type":")" + ctrlType
            + R"(","url":")"
            + urlized.toEncoded( QUrl::EncodeSpaces | QUrl::EncodeReserved )
                  .toStdString()
            + "\"}" )
              .c_str();
    //     qInfo () << "Sending Binding Set Request";
    //     qInfo () << data.toStdString();
    connect( &m_networkManagerApply,
             SIGNAL( finished( QNetworkReply* ) ),
             this,
             SLOT( onApplyBindingResponse( QNetworkReply* ) ) );
    m_networkManagerApply.post( request, data );
}

void SteamVRTabController::onApplyBindingResponse( QNetworkReply* reply )
{
    nlohmann::json const output = "";
    QString const data = QString::fromUtf8( reply->readAll() );
    if ( data.size() < 2 )
    {
        return;
    }
    //     qInfo () << "APPLY RESPONSE";
    //     qInfo () << data.toStdString();
    reply->close();
    nlohmann::json jsonfull = nlohmann::json::parse( data.toStdString() );
    if ( !jsonfull.contains( "success" ) )
    {
        qCritical() << "Apply Binding Packet Mal-Formed?";
        return;
    }
    if ( !jsonfull["success"].get<bool>() )
    {
        // TODO better error handling?
        qCritical() << "Binding Failed To Apply";
    }
    qInfo() << "New Binding Applied";
}

void SteamVRTabController::setBindingQMLWrapper( QString appID, bool def )
{
    m_setDefault = def;
    std::string const aID = appID.toStdString();
    getBindingUrlReq( aID );
}

bool SteamVRTabController::perAppBindEnabled() const
{
    return settings::getSetting(
        settings::BoolSetting::STEAMVR_perappBindEnabled );
}
void SteamVRTabController::setPerAppBindEnabled( bool value, bool notify )
{
    settings::setSetting( settings::BoolSetting::STEAMVR_perappBindEnabled,
                          value );

    if ( notify )
    {
        emit perAppBindEnabledChanged( value );
    }
}

void SteamVRTabController::applyAllCustomBindings()
{
    QFileInfo const fileInfo(
        QString::fromStdString( settings::initializeAndGetSettingsPath() ) );
    QDir const directory = fileInfo.absolutePath();
    QStringList const bindings = directory.entryList( QStringList() << "*.json"
                                                                    << "*.JSON",
                                                      QDir::Files );
    std::set<std::string> appIDs;
    std::regex const reg1( "ovl(.*)scene" );
    std::regex const reg2( "defovl(.*)ctrl" );
    foreach ( QString const filename, bindings )
    {
        std::smatch mat;
        std::string const sfilename = filename.toStdString();

        if ( regex_search( sfilename, mat, reg1 ) )
        {
            if ( mat.size() == 2 )
            {
                appIDs.insert( mat[1].str() );
                continue;
            }
        }
        if ( regex_search( sfilename, mat, reg2 ) )
        {
            if ( mat.size() == 2 )
            {
                appIDs.insert( mat[1].str() );
                continue;
            }
        }
    }
    foreach ( std::string const appID, appIDs )
    {
        applyBindingReq( appID );
    }
}

} // namespace advsettings
