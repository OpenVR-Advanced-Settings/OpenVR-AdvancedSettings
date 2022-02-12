#include "SteamVRTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"
#include "../utils/update_rate.h"

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
    initLHCUtil();
    synchSteamVR();
}

void SteamVRTabController::initLHCUtil()
{
    vr::EVRApplicationError error;
    const uint32_t bufferMax = 4096;
    char cStringOut[bufferMax];
    vr::VRApplications()->GetApplicationPropertyString(
        "steam.overlay.250820",
        vr::VRApplicationProperty_BinaryPath_String,
        cStringOut,
        bufferMax,
        &error );
    std::string FileInProg( cStringOut );
    std::filesystem::path overlayPath( FileInProg );
    overlayPath.remove_filename();
    auto lhPath = overlayPath / ".." / ".." / "tools" / "lighthouse" / "bin"
                  / "win64" / "lighthouse_Console.exe";
    if ( error != vr::VRApplicationError_None )
    {
        LOG( WARNING ) << "Could Not Find Path To Lighthouse Console";
        LOG( WARNING )
            << vr::VRApplications()->GetApplicationsErrorNameFromEnum( error );
        m_pathRXTXInit = false;
    }
    else
    {
        m_pathRXTXInit = true;
    }
    auto path
        = QDir::toNativeSeparators( QString::fromStdString( lhPath.string() ) );
    auto qdir = QDir( path );
    m_LHUtil = new lh_con_util::LHCUtil( qdir.path() );
    LOG( WARNING ) << "path is" << qdir.path().toStdString();
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
    setCameraRoom( cameraRoom() );
    setCameraDashboard( cameraDashboard() );
    setCameraBounds( cameraBounds() );
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

bool SteamVRTabController::cameraRoom() const
{
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_Camera_Section,
        vr::k_pch_Camera_EnableCameraForRoomView_Bool );

    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
    }
    return m_cameraRoom;
}

void SteamVRTabController::setCameraRoom( const bool value, const bool notify )
{
    if ( m_cameraRoom != value )
    {
        m_cameraRoom = value;
        ovr_settings_wrapper::setBool(
            vr::k_pch_Camera_Section,
            vr::k_pch_Camera_EnableCameraForRoomView_Bool,
            m_cameraRoom );
        if ( notify )
        {
            emit cameraRoomChanged( m_cameraRoom );
        }
    }
}

bool SteamVRTabController::cameraDashboard() const
{
    auto p = ovr_settings_wrapper::getBool(
        vr::k_pch_Camera_Section,
        vr::k_pch_Camera_EnableCameraInDashboard_Bool );

    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
    }
    return m_cameraDashboard;
}

void SteamVRTabController::clearTXRXUI() {}

void SteamVRTabController::createTXRXListUI( std::vector<std::string> txlist,
                                             std::vector<std::string> rxlist )
{
}

bool SteamVRTabController::searchRXTX()
{
    if ( !m_pathRXTXInit )
    {
        initLHCUtil();
    }
    m_controllerList.clear();
    bool output = m_LHUtil->FindAll();
    for ( auto& txrxItem : m_LHUtil->RXTX_Pairs_ )
    {
        std::string TX = txrxItem.TX_Serial.toStdString();
        if ( TX.empty() || TX == " " )
        {
            txrxItem.TX_Serial = QString( "N/A" );
            m_controllerList.push_back( QString( "Not Connected" ) );
        }
    }
    return output;
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
    auto error = vr::VRInput()->OpenBindingUI(
        application_strings::applicationKey, actionHandle, inputHandle, false );
    if ( error != vr::VRInputError_None )
    {
        LOG( ERROR ) << "Input Error: " << error;
    }
}

void SteamVRTabController::setCameraDashboard( const bool value,
                                               const bool notify )
{
    if ( m_cameraDashboard != value )
    {
        m_cameraDashboard = value;
        ovr_settings_wrapper::setBool(
            vr::k_pch_Camera_Section,
            vr::k_pch_Camera_EnableCameraInDashboard_Bool,
            m_cameraDashboard );
        if ( notify )
        {
            emit cameraDashboardChanged( m_cameraDashboard );
        }
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
    LOG( WARNING ) << m_LHUtil->RXTX_Pairs_.size() << "size expected";
    return static_cast<unsigned>( m_LHUtil->RXTX_Pairs_.size() );
}

Q_INVOKABLE QString SteamVRTabController::getTXList( int i )
{
    LOG( WARNING ) << m_LHUtil->RXTX_Pairs_[i].TX_Serial.toStdString()
                   << " TX expected";
    return m_LHUtil->RXTX_Pairs_[i].TX_Serial;
}

Q_INVOKABLE QString SteamVRTabController::getRXList( int i )
{
    LOG( WARNING ) << m_LHUtil->RXTX_Pairs_[i].RX_Serial.toStdString()
                   << " RX expected";
    return m_LHUtil->RXTX_Pairs_[i].RX_Serial;
}
} // namespace advsettings
