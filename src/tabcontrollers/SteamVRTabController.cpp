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

} // namespace advsettings
