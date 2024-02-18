#include "SteamVRTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"
#include "../utils/update_rate.h"
#include <QDesktopServices>

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
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_Perf_Section, vr::k_pch_Perf_PerfGraphInHMD_Bool );

    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
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
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_RequireHmd_String );

    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
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
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_ActivateMultipleDrivers_Bool );

    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
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
    auto p = ovr_settings_wrapper::getBool( vr::k_pch_SteamVR_Section,
                                            vr::k_pch_SteamVR_DoNotFadeToGrid );

    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
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
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_Power_Section,
        vr::k_pch_Power_AutoLaunchSteamVROnButtonPress );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
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
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_SendSystemButtonToAllApps_Bool );

    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
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
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_Notifications_Section,
        vr::k_pch_Notifications_DoNotDisturb_Bool );

    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
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
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_Camera_Section, vr::k_pch_Camera_EnableCamera_Bool );

    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
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
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_Camera_Section,
        vr::k_pch_Camera_EnableCameraForCollisionBounds_Bool );

    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
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
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_Camera_Section, vr::k_pch_Camera_ShowOnController_Bool );

    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
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
        m_deviceList.push_back( DeviceInfo{ device } );
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
        for ( auto c : dongleList )
        {
            if ( c == ';' )
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
                m_deviceList.push_back( DeviceInfo{} );
                AddUnPairedDevice( m_deviceList.back(), dSN.toStdString() );
                // TODO create
            }
        }
    }
    return;
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
    std::string cd = ovr_system_wrapper::getStringTrackedProperty(
                         device.index, vr::Prop_ConnectedWirelessDongle_String )
                         .second;
    if ( cd.empty() || cd == " " )
    {
        cd = "n/a";
        device.dongleType = "n/a";
    }
    else
    {
        m_dongleCountCur++;
        if ( cd.find( "-RYB" ) != std::string::npos
             || cd.find( "-LYM" ) != std::string::npos )
        {
            device.dongleType = "Headset";
        }
        // TODO 1yx SN
        else if ( std::regex_match( cd, std::regex( "(.*)(-[0-9]YX)" ) ) )
        {
            device.dongleType = "Tundra Dongle";
        }
        else
        {
            device.dongleType = "Standard Dongle";
        }
    }
    device.conDongle = cd;

    std::string dd = ovr_system_wrapper::getStringTrackedProperty(
                         device.index, vr::Prop_SerialNumber_String )
                         .second;
    if ( dd.empty() || dd == " " )
    {
        dd = "n/a";
    }
    device.txName = dd;

    auto devClass = ovr_system_wrapper::getDeviceClass( device.index );
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
    int role = ovr_system_wrapper::getInt32TrackedProperty(
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
    vr::VRActionSetHandle_t actionHandle = 0;
    vr::VRInputValueHandle_t inputHandle = 0;

    auto error2 = vr::VRInput()->GetInputSourceHandle( "/user/hand/right",
                                                       &inputHandle );
    if ( error2 != vr::VRInputError_None )
    {
        LOG( ERROR )
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
        LOG( ERROR ) << "Input Error: " << error;
    }
}
void SteamVRTabController::restartSteamVR()
{
    QString cmd = QString( "cmd.exe /C restartvrserver.bat \"" )
                  + parent->getVRRuntimePathUrl().toLocalFile()
                  + QString( "\"" );
    LOG( INFO ) << "SteamVR Restart Command: " << cmd;
    QProcess::startDetached( cmd );
}

Q_INVOKABLE unsigned SteamVRTabController::getRXTXCount()
{
    return static_cast<unsigned>( m_deviceList.size() );
}

Q_INVOKABLE QString SteamVRTabController::getTXList( int i )
{
    return QString::fromStdString( m_deviceList[i].txName );
}

Q_INVOKABLE QString SteamVRTabController::getDeviceName( int i )
{
    return QString::fromStdString( m_deviceList[i].deviceName );
}

Q_INVOKABLE QString SteamVRTabController::getRXList( int i )
{
    return QString::fromStdString( m_deviceList[i].conDongle );
}
Q_INVOKABLE QString SteamVRTabController::getDongleType( int i )
{
    return QString::fromStdString( m_deviceList[i].dongleType );
}
Q_INVOKABLE QString SteamVRTabController::getDongleUsage()
{
    return QString::fromStdString( std::to_string( m_dongleCountCur ) + "/"
                                   + std::to_string( m_dongleCountMax ) );
}
Q_INVOKABLE void SteamVRTabController::pairDevice( QString sn )
{
    if ( !isSteamVRTracked( sn ) )
    {
        LOG( WARNING ) << sn.toStdString()
                       << " Is Not a SteamVR Dongle, skipping Pair";
        return;
    }
    m_last_pair_sn = sn;
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
    return;
}
bool SteamVRTabController::isSteamVRTracked( QString sn )
{
    return m_unparsedDongleString.contains( sn );
}
void SteamVRTabController::onConnected()
{
    m_webSocket.sendTextMessage( QStringLiteral( "mailbox_open OVRAS_pair" ) );
    QString messageout = "mailbox_send lighthouse_pairing "
                         "{\"type\":\"start_pairing\", "
                         "\"returnAddress\":\"OVRAS_pair\", "
                         "\"serial\":\"";
    messageout.append( m_last_pair_sn );
    messageout.append( QString::fromStdString( "\", \"timeoutSeconds\":15}" ) );
    emit pairStatusChanged( QString( "Pairing..." ) );
    if ( m_last_pair_sn == "" )
    {
        LOG( ERROR ) << "No Last SN to pair, this shouldn't happen";
    }
    m_webSocket.sendTextMessage( messageout );
    // m_webSocket.close();
}
void SteamVRTabController::onDisconnect()
{
    LOG( INFO ) << "Pair WS disconnect";
    m_webSocket.close();
}
void SteamVRTabController::onMsgRec( QString Msg )
{
    if ( Msg.contains( "success" ) )
    {
        LOG( INFO ) << "Pair Success";
        emit pairStatusChanged( QString( "Success" ) );
    }
    if ( Msg.contains( "timeout" ) )
    {
        LOG( INFO ) << "Pair Timeout";
        emit pairStatusChanged( QString( "Timeout" ) );
    }
    m_webSocket.close();
}
std::vector<QString>
    SteamVRTabController::getDongleSerialList( std::string deviceString )
{
    std::vector<QString> dongleList;
    size_t pos = 0;

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
        else
        {
            pos = deviceString.find( ',' );
            if ( pos == std::string::npos )
            {
                break;
            }
            dongleList.push_back(
                QString::fromStdString( deviceString.substr( 0, pos ) ) );
            break;
        }
        break;
    }
    return dongleList;
}

// Binding Functions
void SteamVRTabController::getBindingUrlReq( std::string appID )
{
    m_lastAppID = appID;
    std::string urls
        = "http://localhost:27062/input/getactions.json?app_key=" + appID;
    QUrl url = QUrl( urls.c_str() );
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
    return;
}
void SteamVRTabController::onGetBindingUrlResponse( QNetworkReply* reply )
{
    QString data = QString::fromUtf8( reply->readAll() );
    if ( data.size() < 2 )
    {
        return;
    }
    std::string controllerName = ovr_system_wrapper::getControllerName();
    if ( controllerName == "" )
    {
        LOG( WARNING ) << "No Controller Detected Skipping Bindings";
        return;
    }
    json jsonfull = json::parse( data.toStdString() );
    //    LOG( INFO ) << "URL RESPOSNE XXXXXXX";
    //    LOG( INFO ) << jsonfull.dump().c_str();
    std::string filepath
        = jsonfull["current_binding_url"][controllerName].get<std::string>();
    LOG( INFO ) << "binding url at " << filepath;
    // TODO perhaps some form of error checking if packet wrong?
    reply->close();
    getBindingDataReq( filepath, m_lastAppID, controllerName );
    return;
}

void SteamVRTabController::getBindingDataReq( std::string steamURL,
                                              std::string appID,
                                              std::string ctrlType )
{
    std::string urls = "http://localhost:27062/input/"
                       "loadbindingfromurl.json?binding_url="
                       + steamURL + "&controller_type=" + ctrlType
                       + "&app_key=" + appID;
    QUrl url = QUrl( urls.c_str() );
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
    return;
}
void SteamVRTabController::onGetBindingDataResponse( QNetworkReply* reply )
{
    json output = "";
    QString data = QString::fromUtf8( reply->readAll() );
    if ( data.size() < 2 )
    {
        return;
    }
    //    LOG( WARNING ) << "DATA RESPONSE";
    //    LOG( WARNING ) << data.toStdString();
    json jsonfull = json::parse( data.toStdString() );
    if ( !jsonfull.contains( "success" ) )
    {
        LOG( ERROR ) << "Binding Data Packet Mal-Formed?";
    }
    if ( !jsonfull["success"].get<bool>() )
    {
        // TODO better error handling?
        LOG( ERROR ) << "Binding Data Request Failed";
    }
    reply->close();
    output = jsonfull["binding_config"];
    std::string sceneAppID = ovr_application_wrapper::getSceneAppID();
    if ( sceneAppID == "error" )
    {
        LOG( ERROR ) << "aborting bind save, could not find scene app ID";
        return;
    }
    std::string ctrl = ovr_system_wrapper::getControllerName();
    saveBind( m_lastAppID, sceneAppID, ctrl, output, m_setDefault );
    return;
}

bool SteamVRTabController::saveBind( std::string appID,
                                     std::string sceneAppID,
                                     std::string ctrlType,
                                     json binds,
                                     bool def )
{
    m_setDefault = false;
    QFileInfo fi(
        QString::fromStdString( settings::initializeAndGetSettingsPath() ) );
    QDir directory = fi.absolutePath();
    QString Fn;
    if ( def )
    {
        Fn = QString::fromStdString( "defovl" + appID + "ctrl" + ctrlType
                                     + ".json" );
    }
    else
    {
        Fn = QString::fromStdString( "ovl" + appID + "scene" + sceneAppID
                                     + "ctrl" + ctrlType + ".json" );
    }
    QString absPath = directory.absolutePath() + "/" + Fn;

    QFile bindFile( absPath );
    bindFile.open( QIODevice::ReadWrite | QIODevice::Truncate
                   | QIODevice::Text );
    QByteArray qba = binds.dump().c_str();
    bindFile.write( qba );
    bindFile.flush();
    bindFile.close();
    if ( bindFile.exists() )
    {
        LOG( INFO ) << ( def ? "Default " : ( sceneAppID + " " ) )
                           + "Binding File saved at:"
                    << absPath.toStdString();
        std::string jsonstring = binds.dump().c_str();
        LOG( INFO ) << jsonstring;
        return true;
    }
    return false;
}

bool SteamVRTabController::customBindExists( std::string appID,
                                             std::string sceneAppID,
                                             std::string ctrl )
{
    if ( appID == "" )
    {
        appID = m_lastAppID;
    }
    if ( sceneAppID == "" )
    {
        sceneAppID = ovr_application_wrapper::getSceneAppID();
    }
    if ( ctrl == "" )
    {
        ctrl = ovr_system_wrapper::getControllerName();
    }
    QFileInfo fi( QString::fromStdString(
        settings::initializeAndGetSettingsPath() + "ovl" + appID + "scene"
        + sceneAppID + "ctrl" + ctrl + ".json" ) );
    if ( fi.exists() )
    {
        return true;
    }
    return false;
}

bool SteamVRTabController::defBindExists( std::string appID, std::string ctrl )
{
    if ( appID == "" )
    {
        appID = m_lastAppID;
    }
    if ( ctrl == "" )
    {
        ctrl = ovr_system_wrapper::getControllerName();
    }
    QFileInfo fi( QString::fromStdString(
        settings::initializeAndGetSettingsPath() + "defovl" + appID + "ctrl"
        + ctrl + ".json" ) );
    if ( fi.exists() )
    {
        return true;
    }
    return false;
}

void SteamVRTabController::applyBindingReq( std::string appID )
{
    std::string ctrlType = ovr_system_wrapper::getControllerName();
    QFileInfo fi(
        QString::fromStdString( settings::initializeAndGetSettingsPath() ) );
    QDir directory = fi.absolutePath();
    QString Fn;
    std::string sceneAppID = ovr_application_wrapper::getSceneAppID();
    if ( sceneAppID == "" )
    {
        LOG( ERROR ) << "NO Scene App Detected unable to apply bindings";
        return;
    }

    // TODO check forward slash compatibility with linux
    Fn = QString::fromStdString( "ovl" + appID + "scene" + sceneAppID + "ctrl"
                                 + ctrlType + ".json" );
    if ( !QFileInfo::exists( fi.absolutePath() + QString( "/" ) + Fn ) )
    {
        LOG( INFO ) << "No Specific Binding Detected for: " + appID
                           + "for Scene: " + sceneAppID + " Checking Default";
        Fn = QString::fromStdString( "defovl" + appID + "ctrl" + ctrlType
                                     + ".json" );
        if ( !QFileInfo::exists( fi.absolutePath() + QString( "/" ) + Fn ) )
        {
            LOG( INFO ) << "No Def Binding Detected for: " + appID
                               + " Not Adjusting Bindings";
            return;
        }
    }
    // TODO possible linux compatibility issue
    QString absPath = directory.absolutePath() + "/" + Fn;
    QUrl urlized = QUrl::fromLocalFile( absPath );
    // std::string filePath = "file:///" + absPath.toStdString();
    std::string url = "http://localhost:27062/input/selectconfig.action";
    // LOG( INFO ) << urlized.toEncoded().toStdString();
    QUrl urls = QUrl( url.c_str() );
    QNetworkRequest request;
    request.setUrl( urls );
    LOG( INFO ) << "Attempting to Apply Binding at: "
                << urlized
                       .toEncoded( QUrl::EncodeSpaces | QUrl::EncodeReserved )
                       .toStdString();
    // This is Important as otherwise Valve's VRWebServerWillIgnore the Request
    // If referrer is wrong
    request.setHeader( QNetworkRequest::ContentTypeHeader,
                       "application/x-www-form-urlencoded" );
    request.setRawHeader(
        QByteArray( "Referer" ),
        QByteArray(
            "http://localhost:27062/dashboard/controllerbinding.html" ) );
    QByteArray data
        = ( "{\"app_key\":\"" + appID + "\",\"controller_type\":\"" + ctrlType
            + "\",\"url\":\""
            + urlized.toEncoded( QUrl::EncodeSpaces | QUrl::EncodeReserved )
                  .toStdString()
            + "\"}" )
              .c_str();
    //    LOG( INFO ) << "Sending Binding Set Request";
    //    LOG( INFO ) << data.toStdString();
    connect( &m_networkManagerApply,
             SIGNAL( finished( QNetworkReply* ) ),
             this,
             SLOT( onApplyBindingResponse( QNetworkReply* ) ) );
    m_networkManagerApply.post( request, data );
}

void SteamVRTabController::onApplyBindingResponse( QNetworkReply* reply )
{
    json output = "";
    QString data = QString::fromUtf8( reply->readAll() );
    if ( data.size() < 2 )
    {
        return;
    }
    //    LOG( INFO ) << "APPLY RESPONSE";
    //    LOG( INFO ) << data.toStdString();
    reply->close();
    json jsonfull = json::parse( data.toStdString() );
    if ( !jsonfull.contains( "success" ) )
    {
        LOG( ERROR ) << "Apply Binding Packet Mal-Formed?";
        return;
    }
    if ( !jsonfull["success"].get<bool>() )
    {
        // TODO better error handling?
        LOG( ERROR ) << "Binding Failed To Apply";
    }
    LOG( INFO ) << "New Binding Applied";
    return;
}

void SteamVRTabController::setBindingQMLWrapper( QString appID, bool def )
{
    m_setDefault = def;
    std::string aID = appID.toStdString();
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
    QFileInfo fi(
        QString::fromStdString( settings::initializeAndGetSettingsPath() ) );
    QDir directory = fi.absolutePath();
    QStringList bindings = directory.entryList( QStringList() << "*.json"
                                                              << "*.JSON",
                                                QDir::Files );
    std::set<std::string> appIDs;
    std::regex r1( "ovl(.*)scene" );
    std::regex r2( "defovl(.*)ctrl" );
    foreach ( QString filename, bindings )
    {
        std::smatch m;
        std::string s = filename.toStdString();

        if ( regex_search( s, m, r1 ) )
        {
            if ( m.size() == 2 )
            {
                appIDs.insert( m[1].str() );
                continue;
            }
        }
        if ( regex_search( s, m, r2 ) )
        {
            if ( m.size() == 2 )
            {
                appIDs.insert( m[1].str() );
                continue;
            }
        }
    }
    foreach ( std::string appID, appIDs )
    {
        applyBindingReq( appID );
    }
}

} // namespace advsettings
