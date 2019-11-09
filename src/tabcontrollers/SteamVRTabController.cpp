#include "SteamVRTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings
{
void SteamVRTabController::initStage1()
{
    m_k_steamVrSettingsUpdateCounter
        = utils::adjustUpdateRate( k_steamVrSettingsUpdateCounter );
    dashboardLoopTick();
}

void SteamVRTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
}

void SteamVRTabController::dashboardLoopTick()
{
    if ( settingsUpdateCounter >= m_k_steamVrSettingsUpdateCounter )
    {
        vr::EVRSettingsError vrSettingsError;

        // Checks and synchs performance graph
        auto pg = vr::VRSettings()->GetBool( vr::k_pch_Perf_Section,
                                             vr::k_pch_Perf_PerfGraphInHMD_Bool,
                                             &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_Perf_PerfGraphInHMD_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setPerformanceGraph( pg );

        // synch systembutton
        auto sb = vr::VRSettings()->GetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_SendSystemButtonToAllApps_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_SendSystemButtonToAllApps_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setSystemButton( sb );

        // synch nofadetogrid
        auto nf = vr::VRSettings()->GetBool( vr::k_pch_SteamVR_Section,
                                             vr::k_pch_SteamVR_DoNotFadeToGrid,
                                             &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_DoNotFadeToGrid
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setNoFadeToGrid( nf );

        // synch multipleDriver
        auto md = vr::VRSettings()->GetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_ActivateMultipleDrivers_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_ActivateMultipleDrivers_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setMultipleDriver( md );

        // synch dnd
        auto donotd = vr::VRSettings()->GetBool(
            vr::k_pch_Notifications_Section,
            vr::k_pch_Notifications_DoNotDisturb_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_Notifications_DoNotDisturb_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setDND( donotd );

        // synch camera
        auto ca = vr::VRSettings()->GetBool( vr::k_pch_Camera_Section,
                                             vr::k_pch_Camera_EnableCamera_Bool,
                                             &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_Camera_EnableCamera_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setCameraActive( ca );

        // synch camera room
        auto cr = vr::VRSettings()->GetBool(
            vr::k_pch_Camera_Section,
            vr::k_pch_Camera_EnableCameraForRoomView_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_Camera_EnableCameraForRoomView_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setCameraRoom( cr );

        // synch camera dashboard
        auto cd = vr::VRSettings()->GetBool(
            vr::k_pch_Camera_Section,
            vr::k_pch_Camera_EnableCameraInDashboard_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_Camera_EnableCameraInDashboard_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setCameraDashboard( cd );

        // synch camera bounds
        auto cb = vr::VRSettings()->GetBool(
            vr::k_pch_Camera_Section,
            vr::k_pch_Camera_EnableCameraForCollisionBounds_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING )
                << "Could not read \""
                << vr::k_pch_Camera_EnableCameraForCollisionBounds_Bool
                << "\" setting: "
                << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                       vrSettingsError );
        }
        setCameraBounds( cb );

        settingsUpdateCounter = 0;
    }
    else
    {
        settingsUpdateCounter++;
    }
}

bool SteamVRTabController::performanceGraph() const
{
    return m_performanceGraphToggle;
}

void SteamVRTabController::setPerformanceGraph( const bool value,
                                                const bool notify )
{
    if ( m_performanceGraphToggle != value )
    {
        m_performanceGraphToggle = value;
        vr::VRSettings()->SetBool( vr::k_pch_Perf_Section,
                                   vr::k_pch_Perf_PerfGraphInHMD_Bool,
                                   m_performanceGraphToggle );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit performanceGraphChanged( m_performanceGraphToggle );
        }
    }
}

bool SteamVRTabController::multipleDriver() const
{
    return m_multipleDriverToggle;
}

void SteamVRTabController::setMultipleDriver( const bool value,
                                              const bool notify )
{
    if ( m_multipleDriverToggle != value )
    {
        m_multipleDriverToggle = value;
        vr::VRSettings()->SetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_ActivateMultipleDrivers_Bool,
            m_multipleDriverToggle );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit multipleDriverChanged( m_multipleDriverToggle );
        }
    }
}

bool SteamVRTabController::noFadeToGrid() const
{
    return m_noFadeToGridToggle;
}

void SteamVRTabController::setNoFadeToGrid( const bool value,
                                            const bool notify )
{
    if ( m_noFadeToGridToggle != value )
    {
        m_noFadeToGridToggle = value;
        vr::VRSettings()->SetBool( vr::k_pch_SteamVR_Section,
                                   vr::k_pch_SteamVR_DoNotFadeToGrid,
                                   m_noFadeToGridToggle );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit noFadeToGridChanged( m_noFadeToGridToggle );
        }
    }
}

bool SteamVRTabController::systemButton() const
{
    return m_systemButtonToggle;
}

void SteamVRTabController::setSystemButton( const bool value,
                                            const bool notify )
{
    if ( m_systemButtonToggle != value )
    {
        m_systemButtonToggle = value;
        vr::VRSettings()->SetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_SendSystemButtonToAllApps_Bool,
            m_systemButtonToggle );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit systemButtonChanged( m_systemButtonToggle );
        }
    }
}

bool SteamVRTabController::dnd() const
{
    return m_dnd;
}

void SteamVRTabController::setDND( const bool value, const bool notify )
{
    if ( m_dnd != value )
    {
        m_dnd = value;
        vr::VRSettings()->SetBool( vr::k_pch_Notifications_Section,
                                   vr::k_pch_Notifications_DoNotDisturb_Bool,
                                   m_dnd );
        vr::VRSettings()->Sync();
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
    return m_cameraActive;
}

void SteamVRTabController::setCameraActive( const bool value,
                                            const bool notify )
{
    if ( m_cameraActive != value )
    {
        m_cameraActive = value;
        vr::VRSettings()->SetBool( vr::k_pch_Camera_Section,
                                   vr::k_pch_Camera_EnableCamera_Bool,
                                   m_cameraActive );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit cameraActiveChanged( m_cameraActive );
        }
    }
}

bool SteamVRTabController::cameraBounds() const
{
    return m_cameraBounds;
}

void SteamVRTabController::setCameraBounds( const bool value,
                                            const bool notify )
{
    if ( m_cameraBounds != value )
    {
        m_cameraBounds = value;
        vr::VRSettings()->SetBool(
            vr::k_pch_Camera_Section,
            vr::k_pch_Camera_EnableCameraForCollisionBounds_Bool,
            m_cameraBounds );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit cameraBoundsChanged( m_cameraBounds );
        }
    }
}

bool SteamVRTabController::cameraRoom() const
{
    return m_cameraRoom;
}

void SteamVRTabController::setCameraRoom( const bool value, const bool notify )
{
    if ( m_cameraRoom != value )
    {
        m_cameraRoom = value;
        vr::VRSettings()->SetBool(
            vr::k_pch_Camera_Section,
            vr::k_pch_Camera_EnableCameraForRoomView_Bool,
            m_cameraRoom );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit cameraRoomChanged( m_cameraRoom );
        }
    }
}

bool SteamVRTabController::cameraDashboard() const
{
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
        vr::VRSettings()->SetBool(
            vr::k_pch_Camera_Section,
            vr::k_pch_Camera_EnableCameraInDashboard_Bool,
            m_cameraDashboard );
        vr::VRSettings()->Sync();
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
