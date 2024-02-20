#include "ChaperoneTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"
#include "../settings/settings.h"
#include "../utils/Matrix.h"
#include "../quaternion/quaternion.h"
#include "../utils/update_rate.h"
#include <cmath>

// application namespace
namespace advsettings
{
void ChaperoneTabController::initStage1()
{
    m_trackingUniverse = vr::VRCompositor()->GetTrackingSpace();
    if ( disableChaperone() )
    {
        setFadeDistance( 0.0f, true );
    }

    reloadChaperoneProfiles();
    initCenterMarkerOverlay();
    eventLoopTick( m_trackingUniverse, nullptr );
}

void ChaperoneTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;

    updateChaperoneSettings();

    if ( m_centerMarkerOverlayIsInit )
    {
        vr::HmdMatrix34_t temp = { { { 1.0f, 0.0f, 0.0f, 0.0f },
                                     { 0.0f, 0.0f, 1.0f, 0.0f },
                                     { 0.0f, -1.0f, 0.0f, 0.0f } } };
        updateCenterMarkerOverlay( &( temp ) );
    }
}

void ChaperoneTabController::dashboardLoopTick()
{
    if ( updateRate.shouldSubjectNotRun(
             UpdateSubject::ChaperoneTabController ) )
    {
        return;
    }

    updateChaperoneSettings();
}

ChaperoneTabController::~ChaperoneTabController()
{
    m_chaperoneHapticFeedbackActive = false;
    if ( m_chaperoneHapticFeedbackThread.joinable() )
    {
        m_chaperoneHapticFeedbackThread.join();
    }
}

void ChaperoneTabController::reloadChaperoneProfiles()
{
    settings::loadAllObjects( chaperoneProfiles );
}

void ChaperoneTabController::saveChaperoneProfiles()
{
    settings::saveAllObjects( chaperoneProfiles );
}

void ChaperoneTabController::handleChaperoneWarnings( float distance )
{
    vr::VRControllerState_t hmdState;
    vr::VRSystem()->GetControllerState( vr::k_unTrackedDeviceIndex_Hmd,
                                        &hmdState,
                                        sizeof( vr::VRControllerState_t ) );
    // If proximity Sensor is True (only can be true if exists)
    // Then use prox sensor else use hmd activity level.
    // Needed as detection since the "has prox sensor" property is unreliable.
    // Note m_isProxActive should only ever be set to true if there is a prox
    // sensor AND its currently on.

    bool proxSensorOverrideState = false;
    m_HMDHasProx ? ( proxSensorOverrideState = m_isProxActive )
                 : ( proxSensorOverrideState = m_isHMDActive );

    // Switch to Beginner Mode
    if ( isChaperoneSwitchToBeginnerEnabled() )
    {
        float activationDistance = chaperoneSwitchToBeginnerDistance();

        if ( distance <= activationDistance && m_isHMDActive
             && !m_chaperoneSwitchToBeginnerActive )
        {
            // Instead of backing out at every stage, this will log errors, but
            // proceed forward.
            m_chaperoneSwitchToBeginnerLastStyle = collisionBoundStyle();
            // IMPORTANT DO NOT NOTIFY IF WE SWITCH BECAUSE WARNINGS
            setCollisionBoundStyle(
                static_cast<int>( vr::COLLISION_BOUNDS_STYLE_BEGINNER ),
                false,
                true );
            m_chaperoneSwitchToBeginnerActive = true;
        }
        else if ( ( distance > activationDistance || !m_isHMDActive )
                  && m_chaperoneSwitchToBeginnerActive )
        {
            setCollisionBoundStyle( m_chaperoneSwitchToBeginnerLastStyle,
                                    true );
            m_chaperoneSwitchToBeginnerActive = false;
        }
    }

    // Haptic Feedback

    if ( isChaperoneHapticFeedbackEnabled() )
    {
        float activationDistance = chaperoneHapticFeedbackDistance();

        if ( distance <= activationDistance && proxSensorOverrideState )
        {
            if ( !m_chaperoneHapticFeedbackActive )
            {
                if ( m_chaperoneHapticFeedbackThread.joinable() )
                {
                    m_chaperoneHapticFeedbackActive = false;
                    m_chaperoneHapticFeedbackThread.join();
                }
                m_chaperoneHapticFeedbackActive = true;
                m_chaperoneHapticFeedbackThread = std::thread(
                    [&]( ChaperoneTabController* _this )
                    {
                        auto leftIndex
                            = vr::VRSystem()
                                  ->GetTrackedDeviceIndexForControllerRole(
                                      vr::TrackedControllerRole_LeftHand );
                        auto rightIndex
                            = vr::VRSystem()
                                  ->GetTrackedDeviceIndexForControllerRole(
                                      vr::TrackedControllerRole_RightHand );
                        while ( _this->m_chaperoneHapticFeedbackActive )
                        {
                            // AS it stands both controllers will vibrate
                            // regardless of which is closer to boundary
                            // haptic Frequency is 0-320Hz
                            if ( leftIndex
                                 != vr::k_unTrackedDeviceIndexInvalid )
                            {
                                vr::VRInput()->TriggerHapticVibrationAction(
                                    m_leftActionHandle,
                                    0.0f,
                                    0.2f,
                                    120.0f,
                                    0.5f,
                                    m_leftInputHandle );
                            }
                            if ( rightIndex
                                 != vr::k_unTrackedDeviceIndexInvalid )
                            {
                                vr::VRInput()->TriggerHapticVibrationAction(
                                    m_rightActionHandle,
                                    0.0f,
                                    0.2f,
                                    120.0f,
                                    0.5f,
                                    m_rightInputHandle );
                            }
                            std::this_thread::sleep_for(
                                std::chrono::milliseconds( 5 ) );
                        }
                    },
                    this );
            }
        }
        else if ( ( distance > activationDistance || !proxSensorOverrideState )
                  && m_chaperoneHapticFeedbackActive )
        {
            m_chaperoneHapticFeedbackActive = false;
        }
    }

    // Alarm Sound
    if ( isChaperoneAlarmSoundEnabled() )
    {
        // LOG(WARNING) << "In alarm";
        float activationDistance = chaperoneAlarmSoundDistance();

        if ( distance <= activationDistance && proxSensorOverrideState )
        {
            if ( !m_chaperoneAlarmSoundActive )
            {
                parent->playAlarm01Sound( isChaperoneAlarmSoundLooping() );
                m_chaperoneAlarmSoundActive = true;
            }
            if ( isChaperoneAlarmSoundLooping()
                 && isChaperoneAlarmSoundAdjustVolume() )
            {
                float vol = 1.1f - distance / activationDistance;
                if ( vol > 1.0f )
                {
                    vol = 1.0f;
                }
                parent->setAlarm01SoundVolume( vol );
            }
            else
            {
                parent->setAlarm01SoundVolume( 1.0f );
            }
        }
        else if ( ( distance > activationDistance || !proxSensorOverrideState )
                  && m_chaperoneAlarmSoundActive )
        {
            parent->cancelAlarm01Sound();
            m_chaperoneAlarmSoundActive = false;
        }
    }

    // Show Dashboard
    if ( isChaperoneShowDashboardEnabled() )
    {
        float activationDistance = chaperoneShowDashboardDistance();
        if ( distance <= activationDistance && !m_chaperoneShowDashboardActive )
        {
            if ( !vr::VROverlay()->IsDashboardVisible() )
            {
                vr::VROverlay()->ShowDashboard(
                    application_strings::applicationKey );
            }
            m_chaperoneShowDashboardActive = true;
        }
        else if ( distance > activationDistance
                  && m_chaperoneShowDashboardActive )
        {
            m_chaperoneShowDashboardActive = false;
        }
    }
}

void ChaperoneTabController::eventLoopTick(
    vr::ETrackingUniverseOrigin universe,
    vr::TrackedDevicePose_t* devicePoses )
{
    m_trackingUniverse = universe;

    if ( centerMarkerNew() )
    {
        // This Runs Independently of Updates to orientation and position of
        // center Marker This also only runs Every ~.5 seconds
        checkCenterMarkerOverlayRotationCount();
    }

    if ( m_dimmingActive && m_dimNotificationTimestamp )
    {
        auto count = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::steady_clock::now()
                         - *m_dimNotificationTimestamp )
                         .count();
        if ( count > 10000 )
        {
            m_dimNotificationTimestamp.reset();
            setFadeDistance( 0.0f, true );
        }
        else
        {
            float pct = std::min(
                1.0f, 2.0f - static_cast<float>( count ) / 5000.0f );
            // Originally this used opacity, but steam will override that it
            // seems
            setFadeDistance( pct * 0.4f, true );
        }
    }

    if ( devicePoses )
    {
        m_isHMDActive = false;
        std::lock_guard<std::recursive_mutex> lock(
            parent->chaperoneUtils().mutex() );
        auto minDistance = NAN;
        auto& poseHmd = devicePoses[vr::k_unTrackedDeviceIndex_Hmd];

        // m_isHMDActive is true when prox sensor OR HMD is moving (~10 seconds
        // to update from OVR)
        // THIS IS A WORK-AROUND Until proper binding support/calls are made
        // availble for prox sensor
        if ( vr::VRSystem()->GetTrackedDeviceActivityLevel(
                 vr::k_unTrackedDeviceIndex_Hmd )
             == vr::k_EDeviceActivityLevel_UserInteraction )
        {
            m_isHMDActive = true;
        }
        if ( poseHmd.bPoseIsValid && poseHmd.bDeviceIsConnected
             && poseHmd.eTrackingResult == vr::TrackingResult_Running_OK )
        {
            auto distanceHmd = parent->chaperoneUtils().getDistanceToChaperone(
                { poseHmd.mDeviceToAbsoluteTracking.m[0][3],
                  poseHmd.mDeviceToAbsoluteTracking.m[1][3],
                  poseHmd.mDeviceToAbsoluteTracking.m[2][3] } );
            if ( !std::isnan( distanceHmd.distance ) )
            {
                minDistance = distanceHmd.distance;
            }
            if ( chaperoneDimHeight() > 0.0f )
            {
                // Both of these only activate on state changes (e.g. when first
                // going above/below chaperoneDimHeight())
                if ( !m_dimmingActive
                     && poseHmd.mDeviceToAbsoluteTracking.m[1][3]
                            < chaperoneDimHeight() )
                {
                    m_dimmingActive = true;
                    m_dimNotificationTimestamp.emplace(
                        std::chrono::steady_clock::now() );
                }
                else if ( m_dimmingActive
                          && poseHmd.mDeviceToAbsoluteTracking.m[1][3]
                                 >= chaperoneDimHeight() )
                {
                    m_dimmingActive = false;
                    setFadeDistance( 0.4f, true );
                }
            }
        }
        auto leftIndex = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
            vr::TrackedControllerRole_LeftHand );
        if ( leftIndex != vr::k_unTrackedDeviceIndexInvalid )
        {
            auto& poseLeft = devicePoses[leftIndex];
            if ( poseLeft.bPoseIsValid && poseLeft.bDeviceIsConnected
                 && poseLeft.eTrackingResult == vr::TrackingResult_Running_OK )
            {
                auto distanceLeft
                    = parent->chaperoneUtils().getDistanceToChaperone(
                        { poseLeft.mDeviceToAbsoluteTracking.m[0][3],
                          poseLeft.mDeviceToAbsoluteTracking.m[1][3],
                          poseLeft.mDeviceToAbsoluteTracking.m[2][3] } );
                if ( !std::isnan( distanceLeft.distance )
                     && ( std::isnan( minDistance )
                          || distanceLeft.distance < minDistance ) )
                {
                    minDistance = distanceLeft.distance;
                }
            }
        }
        auto rightIndex
            = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
                vr::TrackedControllerRole_RightHand );
        if ( rightIndex != vr::k_unTrackedDeviceIndexInvalid )
        {
            auto& poseRight = devicePoses[rightIndex];
            if ( poseRight.bPoseIsValid && poseRight.bDeviceIsConnected
                 && poseRight.eTrackingResult == vr::TrackingResult_Running_OK )
            {
                auto distanceRight
                    = parent->chaperoneUtils().getDistanceToChaperone(
                        { poseRight.mDeviceToAbsoluteTracking.m[0][3],
                          poseRight.mDeviceToAbsoluteTracking.m[1][3],
                          poseRight.mDeviceToAbsoluteTracking.m[2][3] } );
                if ( !std::isnan( distanceRight.distance )
                     && ( std::isnan( minDistance )
                          || distanceRight.distance < minDistance ) )
                {
                    minDistance = distanceRight.distance;
                }
            }
        }
        if ( !std::isnan( minDistance ) )
        {
            handleChaperoneWarnings( minDistance );
        }
        else
        {
            // attempts to reload chaperone data once per ~10 seconds.
            m_updateTicksChaperoneReload++;
            if ( m_updateTicksChaperoneReload >= 1000 )
            {
                m_updateTicksChaperoneReload = 0;
                parent->chaperoneUtils().loadChaperoneData();
            }
        }
    }
}

void ChaperoneTabController::updateChaperoneSettings()
{
    setBoundsVisibility( boundsVisibility() );
    setFadeDistance( fadeDistance(), true );
    setCenterMarker( centerMarker() );
    setPlaySpaceMarker( playSpaceMarker() );
    setChaperoneColorR( chaperoneColorR() );
    setChaperoneColorG( chaperoneColorG() );
    setChaperoneColorB( chaperoneColorB() );
    setChaperoneFloorToggle( chaperoneFloorToggle() );
    setCollisionBoundStyle( collisionBoundStyle() );
    setHeight( height() );
}

void ChaperoneTabController::setBoundsVisibility( float value, bool notify )
{
    setChaperoneColorA(
        static_cast<int>( static_cast<float>( value ) * 255.0f ), notify );
}

float ChaperoneTabController::fadeDistance()
{
    std::pair<ovr_settings_wrapper::SettingsError, float> p
        = ovr_settings_wrapper::getFloat(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_FadeDistance_Float,
            "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
    }
    return m_fadeDistance;
}

void ChaperoneTabController::setFadeDistance( float value, bool notify )
{
    if ( fabs( static_cast<double>( m_fadeDistance - value ) ) > 0.005 )
    {
        m_fadeDistance = value;
        ovr_settings_wrapper::setFloat(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_FadeDistance_Float,
            m_fadeDistance,
            "" );
        // On failure do Nothing
        if ( notify )
        {
            emit fadeDistanceChanged( m_fadeDistance );
        }
    }
}

float ChaperoneTabController::height()
{
    std::pair<ovr_settings_wrapper::SettingsError, float> p
        = ovr_settings_wrapper::getFloat(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_WallHeight_Float,
            "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        m_height = p.second;
        return p.second;
    }

    return m_height;
}

void ChaperoneTabController::setHeight( float value, bool notify )
{
    if ( fabs( static_cast<double>( m_height - value ) ) > 0.005 )
    {
        m_height = value;
        ovr_settings_wrapper::setFloat(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_WallHeight_Float,
            m_height,
            "" );
        parent->m_moveCenterTabController.setBoundsBasisHeight( m_height );
        if ( notify )
        {
            emit heightChanged( m_height );
        }
    }
}

void ChaperoneTabController::updateHeight( float value, bool notify )
{
    if ( fabs( static_cast<double>( m_height - value ) ) > 0.005 )
    {
        m_height = value;
        if ( notify )
        {
            emit heightChanged( m_height );
        }
    }
}

bool ChaperoneTabController::centerMarker()
{
    std::pair<ovr_settings_wrapper::SettingsError, bool> p
        = ovr_settings_wrapper::getBool(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_CenterMarkerOn_Bool,
            "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
    }

    return m_centerMarker;
}

void ChaperoneTabController::setCenterMarker( bool value, bool notify )
{
    if ( m_centerMarker != value )
    {
        m_centerMarker = value;
        ovr_settings_wrapper::setBool(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_CenterMarkerOn_Bool,
            m_centerMarker,
            "" );
        if ( notify )
        {
            emit centerMarkerChanged( m_centerMarker );
        }
    }
}

bool ChaperoneTabController::playSpaceMarker()
{
    std::pair<ovr_settings_wrapper::SettingsError, bool> p
        = ovr_settings_wrapper::getBool(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_PlaySpaceOn_Bool,
            "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
    }

    return m_playSpaceMarker;
}

void ChaperoneTabController::setPlaySpaceMarker( bool value, bool notify )
{
    if ( m_playSpaceMarker != value )
    {
        m_playSpaceMarker = value;
        ovr_settings_wrapper::setBool(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_PlaySpaceOn_Bool,
            m_playSpaceMarker,
            "" );
        // On Error Do Nothing
        if ( notify )
        {
            emit playSpaceMarkerChanged( m_playSpaceMarker );
        }
    }
}

bool ChaperoneTabController::forceBounds() const
{
    // Currently th ere is no way to get from OVR if force bounds is on/off as
    // of OVR 1.12.5
    return m_forceBounds;
}

bool ChaperoneTabController::isChaperoneSwitchToBeginnerEnabled() const
{
    return settings::getSetting(
        settings::BoolSetting::CHAPERONE_chaperoneSwitchToBeginnerEnabled );
}

float ChaperoneTabController::chaperoneSwitchToBeginnerDistance() const
{
    return static_cast<float>( settings::getSetting(
        settings::DoubleSetting::CHAPERONE_switchToBeginnerDistance ) );
}

bool ChaperoneTabController::isChaperoneHapticFeedbackEnabled() const
{
    return settings::getSetting(
        settings::BoolSetting::CHAPERONE_chaperoneHapticFeedbackEnabled );
}

float ChaperoneTabController::chaperoneHapticFeedbackDistance() const
{
    return static_cast<float>( settings::getSetting(
        settings::DoubleSetting::CHAPERONE_hapticFeedbackDistance ) );
}

bool ChaperoneTabController::isChaperoneAlarmSoundEnabled() const
{
    return settings::getSetting(
        settings::BoolSetting::CHAPERONE_chaperoneAlarmSoundEnabled );
}

bool ChaperoneTabController::isChaperoneAlarmSoundLooping() const
{
    return settings::getSetting(
        settings::BoolSetting::CHAPERONE_chaperoneAlarmSoundLooping );
}

bool ChaperoneTabController::isChaperoneAlarmSoundAdjustVolume() const
{
    return settings::getSetting(
        settings::BoolSetting::CHAPERONE_chaperoneAlarmSoundAdjustVolume );
}

bool ChaperoneTabController::disableChaperone() const
{
    return settings::getSetting(
        settings::BoolSetting::CHAPERONE_disableChaperone );
}

float ChaperoneTabController::chaperoneDimHeight() const
{
    return static_cast<float>(
        settings::getSetting( settings::DoubleSetting::CHAPERONE_dimHeight ) );
}

float ChaperoneTabController::chaperoneAlarmSoundDistance() const
{
    return static_cast<float>( settings::getSetting(
        settings::DoubleSetting::CHAPERONE_alarmSoundDistance ) );
}

bool ChaperoneTabController::isChaperoneShowDashboardEnabled() const
{
    return settings::getSetting(
        settings::BoolSetting::CHAPERONE_chaperoneShowDashboardEnabled );
}

float ChaperoneTabController::chaperoneShowDashboardDistance() const
{
    return static_cast<float>( settings::getSetting(
        settings::DoubleSetting::CHAPERONE_showDashboardDistance ) );
}

int ChaperoneTabController::chaperoneColorR()
{
    std::pair<ovr_settings_wrapper::SettingsError, int> p
        = ovr_settings_wrapper::getInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaR_Int32,
            "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
    }
    return m_chaperoneColorR;
}
int ChaperoneTabController::chaperoneColorG()
{
    std::pair<ovr_settings_wrapper::SettingsError, int> p
        = ovr_settings_wrapper::getInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaG_Int32,
            "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
    }
    return m_chaperoneColorG;
}
int ChaperoneTabController::chaperoneColorB()
{
    std::pair<ovr_settings_wrapper::SettingsError, int> p
        = ovr_settings_wrapper::getInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaB_Int32,
            "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
    }
    return m_chaperoneColorB;
}
// aka transparancy
int ChaperoneTabController::chaperoneColorA()
{
    std::pair<ovr_settings_wrapper::SettingsError, int> p
        = ovr_settings_wrapper::getInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaA_Int32,
            "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return m_chaperoneColorA;
    }
    return p.second;
}

float ChaperoneTabController::boundsVisibility()
{
    std::pair<ovr_settings_wrapper::SettingsError, int> p
        = ovr_settings_wrapper::getInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaA_Int32,
            "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return static_cast<float>( static_cast<float>( m_chaperoneColorA )
                                   / 255.0f );
    }
    return static_cast<float>( static_cast<float>( p.second ) / 255.0f );
}

bool ChaperoneTabController::chaperoneFloorToggle()
{
    std::pair<ovr_settings_wrapper::SettingsError, int> p
        = ovr_settings_wrapper::getBool(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_GroundPerimeterOn_Bool,
            "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
    }
    return m_chaperoneFloorToggle;
}

int ChaperoneTabController::collisionBoundStyle()
{
    std::pair<ovr_settings_wrapper::SettingsError, int> p
        = ovr_settings_wrapper::getInt32( vr::k_pch_CollisionBounds_Section,
                                          vr::k_pch_CollisionBounds_Style_Int32,
                                          "" );
    if ( p.first == ovr_settings_wrapper::SettingsError::NoError )
    {
        return p.second;
    }
    return m_collisionBoundStyle;
}

bool ChaperoneTabController::centerMarkerNew()
{
    bool temp = settings::getSetting(
        settings::BoolSetting::CHAPERONE_centerMarkerNew );
    return temp;
}

Q_INVOKABLE unsigned ChaperoneTabController::getChaperoneProfileCount()
{
    return static_cast<unsigned int>( chaperoneProfiles.size() );
}

Q_INVOKABLE QString
    ChaperoneTabController::getChaperoneProfileName( unsigned index )
{
    if ( index >= chaperoneProfiles.size() )
    {
        return QString();
    }
    else
    {
        return QString::fromStdString( chaperoneProfiles[index].profileName );
    }
}

void ChaperoneTabController::setForceBounds( bool value, bool notify )
{
    if ( m_forceBounds != value )
    {
        m_forceBounds = value;
        vr::VRChaperone()->ForceBoundsVisible( m_forceBounds );
        if ( notify )
        {
            emit forceBoundsChanged( m_forceBounds );
        }
    }
}

void ChaperoneTabController::setChaperoneColorR( int value, bool notify )
{
    if ( value != m_chaperoneColorR )
    {
        if ( value > 255 )
        {
            value = 255;
            LOG( WARNING ) << "Red Channel larger than 255, setting to 255";
        }
        else if ( value < 0 )
        {
            value = 0;
            LOG( WARNING ) << "Red Channel smaller than 0, setting to 0";
        }
        m_chaperoneColorR = value;
        ovr_settings_wrapper::setInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaR_Int32,
            m_chaperoneColorR,
            "" );
        updateCenterMarkerOverlayColor();
        if ( notify )
        {
            emit chaperoneColorRChanged( value );
        }
    }
}

void ChaperoneTabController::setChaperoneColorG( int value, bool notify )
{
    if ( value != m_chaperoneColorG )
    {
        if ( value > 255 )
        {
            value = 255;
            LOG( WARNING ) << "Green Channel larger than 255, setting to 255";
        }
        else if ( value < 0 )
        {
            value = 0;
            LOG( WARNING ) << "Green Channel smaller than 0, setting to 0";
        }
        m_chaperoneColorG = value;
        ovr_settings_wrapper::setInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaG_Int32,
            m_chaperoneColorG,
            "" );
        updateCenterMarkerOverlayColor();
        if ( notify )
        {
            emit chaperoneColorGChanged( value );
        }
    }
}

void ChaperoneTabController::setChaperoneColorB( int value, bool notify )
{
    if ( value != m_chaperoneColorB )
    {
        if ( value > 255 )
        {
            value = 255;
            LOG( WARNING ) << "Blue Channel larger than 255, setting to 255";
        }
        else if ( value < 0 )
        {
            value = 0;
            LOG( WARNING ) << "Blue Channel smaller than 0, setting to 0";
        }
        m_chaperoneColorB = value;
        ovr_settings_wrapper::setInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaB_Int32,
            m_chaperoneColorB,
            "" );
        updateCenterMarkerOverlayColor();
        if ( notify )
        {
            emit chaperoneColorBChanged( value );
        }
    }
}
void ChaperoneTabController::updateCenterMarkerOverlayColor()
{
    float chapColorR = static_cast<float>( m_chaperoneColorR ) / 255.0f;
    float chapColorG = static_cast<float>( m_chaperoneColorG ) / 255.0f;
    float chapColorB = static_cast<float>( m_chaperoneColorB ) / 255.0f;
    ovr_overlay_wrapper::setOverlayColor(
        m_chaperoneFloorOverlayHandle, chapColorR, chapColorG, chapColorB, "" );
}

void ChaperoneTabController::setChaperoneColorA( int value, bool notify )
{
    if ( value != m_chaperoneColorA )
    {
        if ( value > 255 )
        {
            value = 255;
            LOG( WARNING ) << "Alpha Channel larger than 255, setting to 255";
        }
        else if ( value < 0 )
        {
            value = 0;
            LOG( WARNING ) << "Alpha Channel smaller than 0, setting to 0";
        }
        m_chaperoneColorA = value;
        ovr_settings_wrapper::setInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaA_Int32,
            value,
            "" );
        ovr_overlay_wrapper::setOverlayAlpha( m_chaperoneFloorOverlayHandle,
                                              static_cast<float>( value )
                                                  / 255.0f,
                                              "" );
        if ( notify )
        {
            emit chaperoneColorAChanged( value );
        }
    }
} // namespace advsettings

void ChaperoneTabController::setCenterMarkerNew( bool value, bool notify )
{
    settings::setSetting( settings::BoolSetting::CHAPERONE_centerMarkerNew,
                          value );
    if ( value )
    {
        ovr_overlay_wrapper::showOverlay( m_chaperoneFloorOverlayHandle );
        m_centerMarkerOverlayNeedsUpdate = true;
    }
    else
    {
        ovr_overlay_wrapper::hideOverlay( m_chaperoneFloorOverlayHandle );
        m_centerMarkerOverlayNeedsUpdate = false;
    }

    if ( notify )
    {
        emit centerMarkerNewChanged( value );
    }
}

void ChaperoneTabController::setChaperoneFloorToggle( bool value, bool notify )
{
    if ( value != m_chaperoneFloorToggle )
    {
        m_chaperoneFloorToggle = value;
        ovr_settings_wrapper::setInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_GroundPerimeterOn_Bool,
            m_chaperoneFloorToggle,
            "" );
        if ( notify )
        {
            emit chaperoneFloorToggleChanged( value );
        }
    }
}

void ChaperoneTabController::setCollisionBoundStyle( int value,
                                                     bool notify,
                                                     bool isTemp )
{
    if ( value != m_collisionBoundStyle )
    {
        if ( vr::COLLISION_BOUNDS_STYLE_COUNT < value )
        {
            value = 0;
            LOG( WARNING )
                << "Invalid Collision Bound Value (>count), set to beginner";
        }
        else if ( value < 0 )
        {
            value = 0;
            LOG( WARNING )
                << "Invalid Collision Bound Value (<0), set to beginner";
        }
        if ( !isTemp )
        {
            if ( m_chaperoneSwitchToBeginnerActive )
            {
                m_chaperoneSwitchToBeginnerLastStyle
                    = static_cast<int32_t>( value );
            }
            else
            {
                m_collisionBoundStyle = value;
            }
        }

        ovr_settings_wrapper::setInt32( vr::k_pch_CollisionBounds_Section,
                                        vr::k_pch_CollisionBounds_Style_Int32,
                                        m_collisionBoundStyle,
                                        "" );
        if ( notify )
        {
            emit collisionBoundStyleChanged( value );
        }
    }
}

void ChaperoneTabController::setChaperoneSwitchToBeginnerEnabled( bool value,
                                                                  bool notify )
{
    if ( isChaperoneSwitchToBeginnerEnabled() != value )
    {
        if ( !value && m_chaperoneSwitchToBeginnerActive )
        {
            ovr_settings_wrapper::setInt32(
                vr::k_pch_CollisionBounds_Section,
                vr::k_pch_CollisionBounds_Style_Int32,
                m_chaperoneSwitchToBeginnerLastStyle,
                "" );
        }

        m_chaperoneSwitchToBeginnerActive = false;

        settings::setSetting(
            settings::BoolSetting::CHAPERONE_chaperoneSwitchToBeginnerEnabled,
            value );
        if ( notify )
        {
            emit chaperoneSwitchToBeginnerEnabledChanged( value );
        }
    }
}

void ChaperoneTabController::setChaperoneSwitchToBeginnerDistance( float value,
                                                                   bool notify )
{
    if ( fabs( static_cast<double>( chaperoneSwitchToBeginnerDistance()
                                    - value ) )
         > 0.005 )
    {
        settings::setSetting(
            settings::DoubleSetting::CHAPERONE_switchToBeginnerDistance,
            static_cast<double>( value ) );

        if ( notify )
        {
            emit chaperoneSwitchToBeginnerDistanceChanged( value );
        }
    }
}

void ChaperoneTabController::setChaperoneHapticFeedbackEnabled( bool value,
                                                                bool notify )
{
    if ( isChaperoneHapticFeedbackEnabled() != value )
    {
        m_chaperoneHapticFeedbackActive = false;
        if ( m_chaperoneHapticFeedbackThread.joinable() )
        {
            m_chaperoneHapticFeedbackThread.join();
        }

        settings::setSetting(
            settings::BoolSetting::CHAPERONE_chaperoneHapticFeedbackEnabled,
            value );

        if ( notify )
        {
            emit chaperoneHapticFeedbackEnabledChanged( value );
        }
    }
}

void ChaperoneTabController::setChaperoneHapticFeedbackDistance( float value,
                                                                 bool notify )
{
    if ( fabs(
             static_cast<double>( chaperoneHapticFeedbackDistance() - value ) )
         > 0.005 )
    {
        settings::setSetting(
            settings::DoubleSetting::CHAPERONE_hapticFeedbackDistance,
            static_cast<double>( value ) );

        if ( notify )
        {
            emit chaperoneHapticFeedbackDistanceChanged( value );
        }
    }
}

void ChaperoneTabController::setChaperoneAlarmSoundEnabled( bool value,
                                                            bool notify )
{
    if ( isChaperoneAlarmSoundEnabled() != value )
    {
        if ( !value && m_chaperoneAlarmSoundActive )
        {
            parent->cancelAlarm01Sound();
        }
        m_chaperoneAlarmSoundActive = false;

        settings::setSetting(
            settings::BoolSetting::CHAPERONE_chaperoneAlarmSoundEnabled,
            value );

        if ( notify )
        {
            emit chaperoneAlarmSoundEnabledChanged( value );
        }
    }
}

void ChaperoneTabController::setChaperoneAlarmSoundLooping( bool value,
                                                            bool notify )
{
    if ( isChaperoneAlarmSoundLooping() != value )
    {
        if ( isChaperoneAlarmSoundEnabled() && m_chaperoneAlarmSoundActive )
        {
            if ( isChaperoneAlarmSoundLooping() )
            {
                parent->playAlarm01Sound( isChaperoneAlarmSoundLooping() );
            }
            else
            {
                parent->cancelAlarm01Sound();
            }
        }

        settings::setSetting(
            settings::BoolSetting::CHAPERONE_chaperoneAlarmSoundLooping,
            value );

        if ( notify )
        {
            emit chaperoneAlarmSoundLoopingChanged( value );
        }
    }
}

void ChaperoneTabController::setChaperoneAlarmSoundAdjustVolume( bool value,
                                                                 bool notify )
{
    if ( isChaperoneAlarmSoundAdjustVolume() != value )
    {
        settings::setSetting(
            settings::BoolSetting::CHAPERONE_chaperoneAlarmSoundAdjustVolume,
            value );

        if ( notify )
        {
            emit chaperoneAlarmSoundAdjustVolumeChanged( value );
        }
    }
}

void ChaperoneTabController::setChaperoneDimHeight( float value, bool notify )
{
    settings::setSetting( settings::DoubleSetting::CHAPERONE_dimHeight,
                          static_cast<double>( value ) );

    if ( notify )
    {
        emit chaperoneDimHeightChanged( value );
    }
}

void ChaperoneTabController::setChaperoneAlarmSoundDistance( float value,
                                                             bool notify )
{
    if ( fabs( static_cast<double>( chaperoneAlarmSoundDistance() - value ) )
         > 0.005 )
    {
        settings::setSetting(
            settings::DoubleSetting::CHAPERONE_alarmSoundDistance,
            static_cast<double>( value ) );

        if ( notify )
        {
            emit chaperoneAlarmSoundDistanceChanged( value );
        }
    }
}

void ChaperoneTabController::setChaperoneShowDashboardEnabled( bool value,
                                                               bool notify )
{
    if ( isChaperoneShowDashboardEnabled() != value )
    {
        m_chaperoneShowDashboardActive = false;

        settings::setSetting(
            settings::BoolSetting::CHAPERONE_chaperoneShowDashboardEnabled,
            value );

        if ( notify )
        {
            emit chaperoneShowDashboardEnabledChanged( value );
        }
    }
}

void ChaperoneTabController::setChaperoneShowDashboardDistance( float value,
                                                                bool notify )
{
    if ( fabs( static_cast<double>( chaperoneShowDashboardDistance() - value ) )
         > 0.005 )
    {
        settings::setSetting(
            settings::DoubleSetting::CHAPERONE_showDashboardDistance,
            static_cast<double>( value ) );

        if ( notify )
        {
            emit chaperoneShowDashboardDistanceChanged( value );
        }
    }
}

void ChaperoneTabController::setDisableChaperone( bool value, bool notify )
{
    if ( disableChaperone() != value )
    {
        if ( value )
        {
            settings::setSetting(
                settings::DoubleSetting::CHAPERONE_fadeDistanceRemembered,
                static_cast<double>( m_fadeDistance ) );

            setFadeDistance( 0.0f, true );
        }
        else
        {
            setFadeDistance( static_cast<float>( settings::getSetting(
                                 settings::DoubleSetting::
                                     CHAPERONE_fadeDistanceRemembered ) ),
                             true );
        }

        settings::setSetting( settings::BoolSetting::CHAPERONE_disableChaperone,
                              value );

        if ( notify )
        {
            emit disableChaperoneChanged( value );
        }
    }
}

void ChaperoneTabController::flipOrientation( double degrees )
{
    parent->m_moveCenterTabController.reset();
    double rad = 0;
    rad = degrees * ( M_PI / 180.0 );

    parent->RotateUniverseCenter( m_trackingUniverse,
                                  static_cast<float>( rad ) );
    parent->m_moveCenterTabController.zeroOffsets();
}

void ChaperoneTabController::reloadFromDisk()
{
    parent->m_moveCenterTabController.reset();
    vr::VRChaperoneSetup()->ReloadFromDisk( vr::EChaperoneConfigFile_Live );
    vr::VRChaperoneSetup()->CommitWorkingCopy( vr::EChaperoneConfigFile_Live );
    parent->m_moveCenterTabController.zeroOffsets();
}

void ChaperoneTabController::addChaperoneProfile(
    QString name,
    bool includeGeometry,
    bool includeVisbility,
    bool includeFadeDistance,
    bool includeCenterMarker,
    bool includePlaySpaceMarker,
    bool includeFloorBounds,
    bool includeBoundsColor,
    bool includeChaperoneStyle,
    bool includeForceBounds,
    bool includesProximityWarningSettings )
{
    ChaperoneProfile* profile = nullptr;
    for ( auto& p : chaperoneProfiles )
    {
        if ( p.profileName.compare( name.toStdString() ) == 0 )
        {
            profile = &p;
            break;
        }
    }
    if ( !profile )
    {
        auto i = chaperoneProfiles.size();
        chaperoneProfiles.emplace_back();
        profile = &chaperoneProfiles[i];
    }
    profile->profileName = name.toStdString();
    profile->includesChaperoneGeometry = includeGeometry;
    if ( includeGeometry )
    {
        vr::VRChaperoneSetup()->HideWorkingSetPreview();
        vr::VRChaperoneSetup()->RevertWorkingCopy();
        uint32_t quadCount = 0;
        vr::VRChaperoneSetup()->GetLiveCollisionBoundsInfo( nullptr,
                                                            &quadCount );
        profile->chaperoneGeometryQuadCount = quadCount;
        for ( int i = 0; i < static_cast<int>( quadCount ); ++i )
        {
            profile->chaperoneGeometryQuads.emplace_back();
        }

        vr::VRChaperoneSetup()->GetLiveCollisionBoundsInfo(
            profile->chaperoneGeometryQuads.data(), &quadCount );
        vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(
            &profile->standingCenter );
        vr::VRChaperoneSetup()->GetWorkingPlayAreaSize(
            &profile->playSpaceAreaX, &profile->playSpaceAreaZ );
    }
    profile->includesVisibility = includeVisbility;
    if ( includeVisbility )
    {
        profile->visibility = boundsVisibility();
    }
    profile->includesFadeDistance = includeFadeDistance;
    if ( includeFadeDistance )
    {
        profile->fadeDistance = m_fadeDistance;
        profile->chaperoneDimHeight = chaperoneDimHeight();
    }
    profile->includesCenterMarker = includeCenterMarker;
    if ( includeCenterMarker )
    {
        profile->centerMarker = m_centerMarker;
        profile->centerMarkerNew = centerMarkerNew();
    }
    profile->includesPlaySpaceMarker = includePlaySpaceMarker;
    if ( includePlaySpaceMarker )
    {
        profile->playSpaceMarker = m_playSpaceMarker;
    }
    profile->includesFloorBoundsMarker = includeFloorBounds;
    if ( includeFloorBounds )
    {
        profile->floorBoundsMarker = m_chaperoneFloorToggle;
    }
    profile->includesBoundsColor = includeBoundsColor;
    if ( includeBoundsColor )
    {
        profile->boundsColor[0] = chaperoneColorR();
        profile->boundsColor[1] = chaperoneColorG();
        profile->boundsColor[2] = chaperoneColorB();
    }
    profile->includesChaperoneStyle = includeChaperoneStyle;
    if ( includeChaperoneStyle )
    {
        profile->chaperoneStyle

            = m_collisionBoundStyle;
    }
    profile->includesForceBounds = includeForceBounds;
    if ( includeForceBounds )
    {
        profile->forceBounds = m_forceBounds;
    }
    profile->includesProximityWarningSettings
        = includesProximityWarningSettings;
    if ( includesProximityWarningSettings )
    {
        profile->enableChaperoneSwitchToBeginner
            = isChaperoneSwitchToBeginnerEnabled();
        profile->chaperoneSwitchToBeginnerDistance
            = chaperoneSwitchToBeginnerDistance();
        profile->enableChaperoneHapticFeedback
            = isChaperoneHapticFeedbackEnabled();
        profile->chaperoneHapticFeedbackDistance
            = chaperoneHapticFeedbackDistance();
        profile->enableChaperoneAlarmSound = isChaperoneAlarmSoundEnabled();
        profile->chaperoneAlarmSoundLooping = isChaperoneAlarmSoundLooping();
        profile->chaperoneAlarmSoundAdjustVolume
            = isChaperoneAlarmSoundAdjustVolume();
        profile->chaperoneAlarmSoundDistance = chaperoneAlarmSoundDistance();
        profile->enableChaperoneShowDashboard
            = isChaperoneShowDashboardEnabled();
        profile->chaperoneShowDashboardDistance
            = chaperoneShowDashboardDistance();
    }

    saveChaperoneProfiles();
    emit chaperoneProfilesUpdated();
}

void ChaperoneTabController::applyChaperoneProfile( unsigned index )
{
    if ( index < chaperoneProfiles.size() )
    {
        auto& profile = chaperoneProfiles[index];
        if ( profile.includesChaperoneGeometry )
        {
            parent->m_moveCenterTabController.reset();
            vr::VRChaperoneSetup()->HideWorkingSetPreview();
            vr::VRChaperoneSetup()->RevertWorkingCopy();
            vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(
                profile.chaperoneGeometryQuads.data(),
                profile.chaperoneGeometryQuadCount );
            vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(
                &profile.standingCenter );
            vr::VRChaperoneSetup()->SetWorkingPlayAreaSize(
                profile.playSpaceAreaX, profile.playSpaceAreaZ );
            vr::VRChaperoneSetup()->CommitWorkingCopy(
                vr::EChaperoneConfigFile_Live );
            parent->m_moveCenterTabController.zeroOffsets();
        }
        if ( profile.includesVisibility )
        {
            setBoundsVisibility( profile.visibility );
        }
        if ( profile.includesFadeDistance )
        {
            setFadeDistance( profile.fadeDistance );
            setChaperoneDimHeight( profile.chaperoneDimHeight );
        }
        if ( profile.includesCenterMarker )
        {
            setCenterMarker( profile.centerMarker );
            setCenterMarkerNew( profile.centerMarkerNew );
        }
        if ( profile.includesPlaySpaceMarker )
        {
            setPlaySpaceMarker( profile.playSpaceMarker );
        }
        if ( profile.includesFloorBoundsMarker )
        {
            setChaperoneFloorToggle( profile.floorBoundsMarker );
        }
        if ( profile.includesBoundsColor )
        {
            setChaperoneColorR( profile.boundsColor[0] );
            setChaperoneColorG( profile.boundsColor[1] );
            setChaperoneColorB( profile.boundsColor[2] );
        }
        if ( profile.includesChaperoneStyle )
        {
            setCollisionBoundStyle( profile.chaperoneStyle, true );
        }
        if ( profile.includesForceBounds )
        {
            setForceBounds( profile.forceBounds );
        }
        if ( profile.includesProximityWarningSettings )
        {
            setChaperoneSwitchToBeginnerDistance(
                profile.chaperoneSwitchToBeginnerDistance );
            setChaperoneSwitchToBeginnerEnabled(
                profile.enableChaperoneSwitchToBeginner );
            setChaperoneHapticFeedbackDistance(
                profile.chaperoneHapticFeedbackDistance );
            setChaperoneHapticFeedbackEnabled(
                profile.enableChaperoneHapticFeedback );
            setChaperoneAlarmSoundLooping( profile.chaperoneAlarmSoundLooping );
            setChaperoneAlarmSoundAdjustVolume(
                profile.chaperoneAlarmSoundAdjustVolume );
            setChaperoneAlarmSoundDistance(
                profile.chaperoneAlarmSoundDistance );
            setChaperoneAlarmSoundEnabled( profile.enableChaperoneAlarmSound );
            setChaperoneShowDashboardDistance(
                profile.chaperoneShowDashboardDistance );
            setChaperoneShowDashboardEnabled(
                profile.enableChaperoneShowDashboard );
        }
    }
}

void ChaperoneTabController::deleteChaperoneProfile( unsigned index )
{
    if ( index < chaperoneProfiles.size() )
    {
        auto pos = chaperoneProfiles.begin() + index;
        chaperoneProfiles.erase( pos );
        saveChaperoneProfiles();
        emit chaperoneProfilesUpdated();
    }
}

std::pair<bool, unsigned>
    ChaperoneTabController::getChaperoneProfileIndexFromName( std::string name )
{
    std::pair<bool, unsigned> result = { false, 0 };
    unsigned profileIndex = 0;
    for ( auto& p : chaperoneProfiles )
    {
        if ( name == p.profileName )
        {
            result.first = true;
            result.second = profileIndex;
            break;
        }
        profileIndex++;
    }
    return result;
}

void ChaperoneTabController::createNewAutosaveProfile()
{
    // update settings to live chaperone
    updateChaperoneSettings();

    // lookup the index for old autosave and delete it
    std::pair<bool, unsigned> previousAutosaveIndexLookup
        = getChaperoneProfileIndexFromName( "«Autosaved Profile (previous)»" );
    if ( previousAutosaveIndexLookup.first )
    {
        deleteChaperoneProfile( previousAutosaveIndexLookup.second );
    }
    else
    {
        LOG( INFO )
            << "[Chaperone Autosave] «Autosaved Profile (previous)» not found";
    }

    // lookup the index for current autosave and rename to old
    std::pair<bool, unsigned> currentAutosaveIndexLookup
        = getChaperoneProfileIndexFromName( "«Autosaved Profile»" );
    if ( currentAutosaveIndexLookup.first )
    {
        chaperoneProfiles[currentAutosaveIndexLookup.second].profileName
            = "«Autosaved Profile (previous)»";
        saveChaperoneProfiles();
        emit chaperoneProfilesUpdated();
    }
    else
    {
        LOG( INFO ) << "[Chaperone Autosave] «Autosaved Profile» not found";
    }

    // create a new autosave from current chaperone (all options set true)
    addChaperoneProfile( "«Autosaved Profile»",
                         true,
                         true,
                         true,
                         true,
                         true,
                         true,
                         true,
                         true,
                         true,
                         true );
}

void ChaperoneTabController::applyAutosavedProfile()
{
    // lookup index of autosave
    std::pair<bool, unsigned> autosaveIndexLookup
        = getChaperoneProfileIndexFromName( "«Autosaved Profile»" );

    // check if it exists
    if ( autosaveIndexLookup.first )
    {
        // apply if it exists
        applyChaperoneProfile( autosaveIndexLookup.second );
    }
    else
    {
        LOG( WARNING ) << "Failed to apply chaperone Autosaved Profile "
                          "(autosave doesn't exist)";
    }
}

void ChaperoneTabController::reset()
{
    ovr_settings_wrapper::removeSection( vr::k_pch_CollisionBounds_Section,
                                         "Reseting Collision Section" );

    setForceBounds( false );

    settingsUpdateCounter = 999; // Easiest way to get default values
}

void ChaperoneTabController::setRightHapticActionHandle(
    vr::VRActionHandle_t handle )
{
    m_rightActionHandle = handle;
}
void ChaperoneTabController::setLeftHapticActionHandle(
    vr::VRActionHandle_t handle )
{
    m_leftActionHandle = handle;
}
void ChaperoneTabController::setRightInputHandle(
    vr::VRInputValueHandle_t handle )
{
    m_rightInputHandle = handle;
}
void ChaperoneTabController::setLeftInputHandle(
    vr::VRInputValueHandle_t handle )
{
    m_leftInputHandle = handle;
}

void ChaperoneTabController::addLeftHapticClick( bool leftHapticClickPressed )
{
    // detect new press
    if ( leftHapticClickPressed && !m_leftHapticClickActivated )
    {
        // play activation haptic sequence
        vr::VRInput()->TriggerHapticVibrationAction(
            m_leftActionHandle, 0.0f, 0.08f, 300.0f, 0.7f, m_leftInputHandle );
        m_leftHapticClickActivated = true;
        return;
    }

    // detect new release
    if ( !leftHapticClickPressed && m_leftHapticClickActivated )
    {
        // play deactivation haptic sequence
        vr::VRInput()->TriggerHapticVibrationAction(
            m_leftActionHandle, 0.0f, 0.08f, 120.0f, 0.7f, m_leftInputHandle );

        m_leftHapticClickActivated = false;
        return;
    }
}

void ChaperoneTabController::addRightHapticClick( bool rightHapticClickPressed )
{
    // detect new press
    if ( rightHapticClickPressed && !m_rightHapticClickActivated )
    {
        // play activation haptic sequence
        vr::VRInput()->TriggerHapticVibrationAction( m_rightActionHandle,
                                                     0.0f,
                                                     0.08f,
                                                     300.0f,
                                                     0.7f,
                                                     m_rightInputHandle );
        m_rightHapticClickActivated = true;
        return;
    }

    // detect new release
    if ( !rightHapticClickPressed && m_rightHapticClickActivated )
    {
        // play deactivation haptic sequence
        vr::VRInput()->TriggerHapticVibrationAction( m_rightActionHandle,
                                                     0.0f,
                                                     0.08f,
                                                     120.0f,
                                                     0.7f,
                                                     m_rightInputHandle );

        m_rightHapticClickActivated = false;
        return;
    }
}

void ChaperoneTabController::initCenterMarkerOverlay()
{
    std::string overlayFloorMarkerKey
        = std::string( application_strings::applicationKey ) + ".floormarker";
    ovr_overlay_wrapper::OverlayError overlayError
        = ovr_overlay_wrapper::createOverlay( overlayFloorMarkerKey,
                                              overlayFloorMarkerKey,
                                              &m_chaperoneFloorOverlayHandle,
                                              "" );
    if ( overlayError == ovr_overlay_wrapper::OverlayError::NoError )
    {
        ovr_overlay_wrapper::setOverlayFromFile(
            m_chaperoneFloorOverlayHandle, m_floorMarkerFN, "" );
        ovr_overlay_wrapper::setOverlayWidthInMeters(
            m_chaperoneFloorOverlayHandle, 0.5f );
        updateCenterMarkerOverlayColor();
        ovr_overlay_wrapper::setOverlayAlpha(
            m_chaperoneFloorOverlayHandle, boundsVisibility(), "" );
        m_centerMarkerOverlayIsInit = true;
    }
    else
    {
        LOG( ERROR ) << "overlay Not initialized";
        // TODO Set Failure variable.
    }
}

// This Function Is called And Handled In MoveCenterTabController to reduce
// un-necessary overhead, as well as consistancy in movement operations
// It will be updated Every frame that movement is updated
void ChaperoneTabController::updateCenterMarkerOverlay(
    vr::HmdMatrix34_t* centerPlaySpaceMatrix )
{
    if ( m_trackingUniverse != vr::TrackingUniverseRawAndUncalibrated )
    {
        ovr_overlay_wrapper::setOverlayTransformAbsolute(
            m_chaperoneFloorOverlayHandle,
            m_trackingUniverse,
            ( centerPlaySpaceMatrix ),
            "" );
    }
}

void ChaperoneTabController::checkCenterMarkerOverlayRotationCount()
{
    // can only turn so quickly soooo roughly .5 secondish update should be fine
    if ( m_rotationUpdateCounter > 45 )
    {
        m_rotationUpdateCounter = 0;
        float rotation = parent->m_statisticsTabController.hmdRotations();
        int rotationNext;
        if ( rotation > 0 )
        {
            rotation = rotation + 0.5f;
            int fullRotation = static_cast<int>( rotation );
            switch ( fullRotation )
            {
            case 0:
                m_floorMarkerFN = "/res/img/chaperone/centermark.png";
                rotationNext = 0;
                break;
            case 1:
                m_floorMarkerFN = "/res/img/chaperone/centermarkr1.png";
                rotationNext = 1;
                break;
            case 2:
                m_floorMarkerFN = "/res/img/chaperone/centermarkr2.png";
                rotationNext = 2;
                break;

            default:

                m_floorMarkerFN = "/res/img/chaperone/centermarkr3.png";
                rotationNext = 3;
            }
        }
        else
        {
            rotation = rotation + 0.5f;
            int fullRotation = static_cast<int>( std::floor( rotation ) );
            switch ( fullRotation )
            {
            case 0:
                m_floorMarkerFN = "/res/img/chaperone/centermark.png";
                rotationNext = 0;
                break;
            case -1:
                m_floorMarkerFN = "/res/img/chaperone/centermarkl1.png";
                rotationNext = -1;
                break;
            case -2:
                m_floorMarkerFN = "/res/img/chaperone/centermarkl2.png";
                rotationNext = -2;
                break;
            default:
                m_floorMarkerFN = "/res/img/chaperone/centermarkl3.png";
                rotationNext = -3;
            }
        }

        if ( rotationNext != m_rotationCurrent && m_centerMarkerOverlayIsInit )
        {
            m_rotationCurrent = rotationNext;
            ovr_overlay_wrapper::setOverlayFromFile(
                m_chaperoneFloorOverlayHandle, m_floorMarkerFN, "" );
        }
    }
    else
    {
        m_rotationUpdateCounter++;
    }
}

void ChaperoneTabController::setProxState( bool value )
{
    if ( value )
    {
        m_HMDHasProx = true;
    }

    m_isProxActive = value;
}

void ChaperoneTabController::shutdown()
{
    if ( isChaperoneSwitchToBeginnerEnabled()
         && m_chaperoneSwitchToBeginnerActive )
    {
        setCollisionBoundStyle( m_chaperoneSwitchToBeginnerLastStyle, false );
    }
}

} // namespace advsettings
