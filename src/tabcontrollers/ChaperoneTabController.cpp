#include "ChaperoneTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"
#include "../settings/settings.h"
#include "../utils/Matrix.h"
#include "../quaternion/quaternion.h"
#include <cmath>

// application namespace
namespace advsettings
{
void ChaperoneTabController::initStage1()
{
    if ( disableChaperone() )
    {
        setFadeDistance( 0.0f, true );
    }

    reloadChaperoneProfiles();
    m_chaperoneSettingsUpdateCounter
        = utils::adjustUpdateRate( k_chaperoneSettingsUpdateCounter );
    eventLoopTick( nullptr );
}

void ChaperoneTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
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
            vr::EVRSettingsError vrSettingsError;
            m_chaperoneSwitchToBeginnerLastStyle = vr::VRSettings()->GetInt32(
                vr::k_pch_CollisionBounds_Section,
                vr::k_pch_CollisionBounds_Style_Int32,
                &vrSettingsError );
            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( WARNING )
                    << "Could not read \""
                    << vr::k_pch_CollisionBounds_Style_Int32 << "\" setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
            }
            else
            {
                vr::VRSettings()->SetInt32(
                    vr::k_pch_CollisionBounds_Section,
                    vr::k_pch_CollisionBounds_Style_Int32,
                    vr::COLLISION_BOUNDS_STYLE_BEGINNER,
                    &vrSettingsError );
                if ( vrSettingsError != vr::VRSettingsError_None )
                {
                    LOG( WARNING )
                        << "Could not set \""
                        << vr::k_pch_CollisionBounds_Style_Int32
                        << "\" setting: "
                        << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                               vrSettingsError );
                }
                else
                {
                    m_chaperoneSwitchToBeginnerActive = true;
                }
            }
        }
        else if ( ( distance > activationDistance || !m_isHMDActive )
                  && m_chaperoneSwitchToBeginnerActive )
        {
            vr::EVRSettingsError vrSettingsError;
            vr::VRSettings()->SetInt32( vr::k_pch_CollisionBounds_Section,
                                        vr::k_pch_CollisionBounds_Style_Int32,
                                        m_chaperoneSwitchToBeginnerLastStyle,
                                        &vrSettingsError );
            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( WARNING )
                    << "Could not set \""
                    << vr::k_pch_CollisionBounds_Style_Int32 << "\" setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
            }
            else
            {
                m_chaperoneSwitchToBeginnerActive = false;
            }
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
                    [&]( ChaperoneTabController* _this ) {
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
    vr::TrackedDevicePose_t* devicePoses )
{
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
            // attempts to reload chaperone data once per ~5 seconds.
            m_updateTicksChaperoneReload++;
            if ( m_updateTicksChaperoneReload >= 500 )
            {
                m_updateTicksChaperoneReload = 0;
                LOG( WARNING ) << "Attempting to reload chaperone data";
                parent->chaperoneUtils().loadChaperoneData();
            }
        }

        // Infinite-walk mode
	// TODO: if (infinite_walk_enabled) 	
	if ( poseHmd.bPoseIsValid && poseHmd.bDeviceIsConnected
             && poseHmd.eTrackingResult == vr::TrackingResult_Running_OK )
        {
            auto chaperoneDistances = parent->chaperoneUtils().getDistancesToChaperone(
                { poseHmd.mDeviceToAbsoluteTracking.m[0][3],
                  poseHmd.mDeviceToAbsoluteTracking.m[1][3],
                  poseHmd.mDeviceToAbsoluteTracking.m[2][3] });
            if(m_chaperoneSnapTurnActive.size() != chaperoneDistances.size())
            {
                // Chaperone changed
                m_chaperoneSnapTurnActive.clear();
                m_chaperoneSnapTurnActive.resize(chaperoneDistances.size(), true);
            }
            float activationDistance = chaperoneSwitchToBeginnerDistance();
    	    float deactivateDistance = 0.2f;
            
            for(size_t i = 0; i < chaperoneDistances.size(); i++)
            {
                const auto& chaperoneQuad = chaperoneDistances[i];
                if ( chaperoneQuad.distance <= activationDistance && m_isHMDActive
                        && !m_chaperoneSnapTurnActive[i] )
                {
		    // ------------------
		    // Set up (un)rotation matrix
		    vr::HmdMatrix34_t hmdMatrixRotMat;
		    vr::HmdMatrix34_t hmdMatrixAbsolute;
		    utils::initRotationMatrix(
		        hmdMatrixRotMat, 1, 0.0f );

		    // Get hmdMatrixAbsolute in un-rotated coordinates.
		    utils::matMul33( hmdMatrixAbsolute, hmdMatrixRotMat, poseHmd.mDeviceToAbsoluteTracking );

		    // Convert pose matrix to quaternion
		    auto hmdQuaternion = quaternion::fromHmdMatrix34( hmdMatrixAbsolute );

		    // Get HMD raw yaw
	        double hmdYaw = quaternion::getYaw( hmdQuaternion );

		    // Get HMD to wall yaw
		    double hmdToWallYaw = static_cast<double>( std::atan2(
			poseHmd.mDeviceToAbsoluteTracking.m[0][3] - chaperoneQuad.nearestPoint.v[0],
			poseHmd.mDeviceToAbsoluteTracking.m[2][3] - chaperoneQuad.nearestPoint.v[2]
		    ) );

            // TODO: ignore if the wall we encountered is behind us?

		    // Get the closest corner on this wall
            const auto& closestCorner = chaperoneQuad.closestCorner({
                    poseHmd.mDeviceToAbsoluteTracking.m[0][3],
                    poseHmd.mDeviceToAbsoluteTracking.m[1][3],
                    poseHmd.mDeviceToAbsoluteTracking.m[2][3]
            });
		    
		    // Get HMD to closest corner yaw
		    double hmdToOpposingWallYaw = static_cast<double>( std::atan2(
			poseHmd.mDeviceToAbsoluteTracking.m[0][3] - closestCorner.v[0],
			poseHmd.mDeviceToAbsoluteTracking.m[2][3] - closestCorner.v[2]
		    ) );

		    LOG( INFO ) << "hmd yaw " << hmdYaw << ", hmd to wall yaw " << hmdToWallYaw;
		    LOG( INFO ) << "hmd to wall angle " << (hmdYaw - hmdToWallYaw) << ", to opposing wall angle " << (hmdYaw - hmdToOpposingWallYaw);
		    // Positive hmd-to-wall is facing left, negative is facing right (relative to the wall)
		    double delta_degrees = ( - (hmdYaw - hmdToWallYaw) - (((hmdYaw - hmdToOpposingWallYaw) > 0.0) ? - M_PI/2 : M_PI/2)) * k_radiansToCentidegrees;
		    //double delta_degrees = ( - (hmdYaw - hmdToWallYaw) - (- M_PI/2)) * k_radiansToCentidegrees;
		    LOG( INFO ) << "rotating space " << (delta_degrees/100) << " degrees";
		    //LOG( INFO ) << "total rotation " << parent->m_moveCenterTabController.getHmdYawTotal();
		    // TODO: Account for overall yaw, have an upper limit to rotation
		    int newRotationAngleDeg = static_cast<int>(parent->m_moveCenterTabController.rotation() + delta_degrees) % 36000;
		    parent->m_moveCenterTabController.setRotation(newRotationAngleDeg);
		    

		    // -----------------

        	    m_chaperoneSnapTurnActive[i] = true;
        	}
        	else if ((chaperoneQuad.distance > (activationDistance + deactivateDistance) || !m_isHMDActive)
                     && m_chaperoneSnapTurnActive[i] )
        	{
        	    m_chaperoneSnapTurnActive[i] = false;
        	}
             }

        }    
    }

    if ( settingsUpdateCounter >= m_chaperoneSettingsUpdateCounter )
    {
        if ( parent->isDashboardVisible() )
        {
            updateChaperoneSettings();
        }
        settingsUpdateCounter = 0;
    }
    else
    {
        settingsUpdateCounter++;
    }
}

void ChaperoneTabController::updateChaperoneSettings()
{
    vr::EVRSettingsError vrSettingsError;
    float vis = static_cast<float>(
        vr::VRSettings()->GetInt32( vr::k_pch_CollisionBounds_Section,
                                    vr::k_pch_CollisionBounds_ColorGammaA_Int32,
                                    &vrSettingsError ) );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not read \""
                       << vr::k_pch_CollisionBounds_ColorGammaA_Int32
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }
    setBoundsVisibility( vis / 255.0f );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not read \""
                       << vr::k_pch_CollisionBounds_FadeDistance_Float
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    auto cm = vr::VRSettings()->GetBool(
        vr::k_pch_CollisionBounds_Section,
        vr::k_pch_CollisionBounds_CenterMarkerOn_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not read \""
                       << vr::k_pch_CollisionBounds_CenterMarkerOn_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }
    setCenterMarker( cm );
    auto ps
        = vr::VRSettings()->GetBool( vr::k_pch_CollisionBounds_Section,
                                     vr::k_pch_CollisionBounds_PlaySpaceOn_Bool,
                                     &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not read \""
                       << vr::k_pch_CollisionBounds_PlaySpaceOn_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }
    setPlaySpaceMarker( ps );
}

float ChaperoneTabController::boundsVisibility() const
{
    return m_visibility;
}

void ChaperoneTabController::setBoundsVisibility( float value, bool notify )
{
    if ( fabs( static_cast<double>( m_visibility - value ) ) > 0.005 )
    {
        if ( value <= 0.3f )
        {
            m_visibility = 0.3f;
        }
        else
        {
            m_visibility = value;
        }
        vr::VRSettings()->SetInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaA_Int32,
            static_cast<int32_t>( 255 * m_visibility ) );

        if ( notify )
        {
            emit boundsVisibilityChanged( m_visibility );
        }
    }
}

float ChaperoneTabController::fadeDistance() const
{
    return m_fadeDistance;
}

void ChaperoneTabController::setFadeDistance( float value, bool notify )
{
    if ( fabs( static_cast<double>( m_fadeDistance - value ) ) > 0.005 )
    {
        m_fadeDistance = value;
        vr::VRSettings()->SetFloat(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_FadeDistance_Float,
            m_fadeDistance );
        if ( notify )
        {
            emit fadeDistanceChanged( m_fadeDistance );
        }
    }
}

float ChaperoneTabController::height() const
{
    return m_height;
}

void ChaperoneTabController::setHeight( float value, bool notify )
{
    if ( fabs( static_cast<double>( m_height - value ) ) > 0.005 )
    {
        m_height = value;
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

bool ChaperoneTabController::centerMarker() const
{
    return m_centerMarker;
}

void ChaperoneTabController::setCenterMarker( bool value, bool notify )
{
    if ( m_centerMarker != value )
    {
        m_centerMarker = value;
        vr::VRSettings()->SetBool(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_CenterMarkerOn_Bool,
            m_centerMarker );
        if ( notify )
        {
            emit centerMarkerChanged( m_centerMarker );
        }
    }
}

bool ChaperoneTabController::playSpaceMarker() const
{
    return m_playSpaceMarker;
}

void ChaperoneTabController::setPlaySpaceMarker( bool value, bool notify )
{
    if ( m_playSpaceMarker != value )
    {
        m_playSpaceMarker = value;
        vr::VRSettings()->SetBool( vr::k_pch_CollisionBounds_Section,
                                   vr::k_pch_CollisionBounds_PlaySpaceOn_Bool,
                                   m_playSpaceMarker );
        if ( notify )
        {
            emit playSpaceMarkerChanged( m_playSpaceMarker );
        }
    }
}

bool ChaperoneTabController::forceBounds() const
{
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

void ChaperoneTabController::setChaperoneSwitchToBeginnerEnabled( bool value,
                                                                  bool notify )
{
    if ( isChaperoneSwitchToBeginnerEnabled() != value )
    {
        if ( !value && m_chaperoneSwitchToBeginnerActive )
        {
            vr::EVRSettingsError vrSettingsError;
            vr::VRSettings()->SetInt32( vr::k_pch_CollisionBounds_Section,
                                        vr::k_pch_CollisionBounds_Style_Int32,
                                        m_chaperoneSwitchToBeginnerLastStyle,
                                        &vrSettingsError );
            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( WARNING )
                    << "Could not set \""
                    << vr::k_pch_CollisionBounds_Style_Int32 << "\" setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
            }
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
                static_cast<double>( value ) );

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

    parent->RotateUniverseCenter( vr::TrackingUniverseStanding,
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
    vr::EVRSettingsError vrSettingsError;
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
        profile->visibility = m_visibility;
    }
    profile->includesFadeDistance = includeFadeDistance;
    if ( includeFadeDistance )
    {
        profile->fadeDistance = m_fadeDistance;
    }
    profile->includesCenterMarker = includeCenterMarker;
    if ( includeCenterMarker )
    {
        profile->centerMarker = m_centerMarker;
    }
    profile->includesPlaySpaceMarker = includePlaySpaceMarker;
    if ( includePlaySpaceMarker )
    {
        profile->playSpaceMarker = m_playSpaceMarker;
    }
    profile->includesFloorBoundsMarker = includeFloorBounds;
    if ( includeFloorBounds )
    {
        profile->floorBoundsMarker = vr::VRSettings()->GetBool(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_GroundPerimeterOn_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_CollisionBounds_GroundPerimeterOn_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
    }
    profile->includesBoundsColor = includeBoundsColor;
    if ( includeBoundsColor )
    {
        profile->boundsColor[0] = vr::VRSettings()->GetInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaR_Int32,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_CollisionBounds_ColorGammaR_Int32
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        profile->boundsColor[1] = vr::VRSettings()->GetInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaG_Int32,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_CollisionBounds_ColorGammaG_Int32
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        profile->boundsColor[2] = vr::VRSettings()->GetInt32(
            vr::k_pch_CollisionBounds_Section,
            vr::k_pch_CollisionBounds_ColorGammaB_Int32,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_CollisionBounds_ColorGammaB_Int32
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
    }
    profile->includesChaperoneStyle = includeChaperoneStyle;
    if ( includeChaperoneStyle )
    {
        profile->chaperoneStyle
            = vr::VRSettings()->GetInt32( vr::k_pch_CollisionBounds_Section,
                                          vr::k_pch_CollisionBounds_Style_Int32,
                                          &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_CollisionBounds_Style_Int32
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
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
        }
        if ( profile.includesCenterMarker )
        {
            setCenterMarker( profile.centerMarker );
        }
        if ( profile.includesPlaySpaceMarker )
        {
            setPlaySpaceMarker( profile.playSpaceMarker );
        }
        if ( profile.includesFloorBoundsMarker )
        {
            vr::VRSettings()->SetBool(
                vr::k_pch_CollisionBounds_Section,
                vr::k_pch_CollisionBounds_GroundPerimeterOn_Bool,
                profile.floorBoundsMarker );
        }
        if ( profile.includesBoundsColor )
        {
            vr::VRSettings()->SetInt32(
                vr::k_pch_CollisionBounds_Section,
                vr::k_pch_CollisionBounds_ColorGammaR_Int32,
                profile.boundsColor[0] );
            vr::VRSettings()->SetInt32(
                vr::k_pch_CollisionBounds_Section,
                vr::k_pch_CollisionBounds_ColorGammaG_Int32,
                profile.boundsColor[1] );
            vr::VRSettings()->SetInt32(
                vr::k_pch_CollisionBounds_Section,
                vr::k_pch_CollisionBounds_ColorGammaB_Int32,
                profile.boundsColor[2] );
        }
        if ( profile.includesChaperoneStyle )
        {
            vr::VRSettings()->SetInt32( vr::k_pch_CollisionBounds_Section,
                                        vr::k_pch_CollisionBounds_Style_Int32,
                                        profile.chaperoneStyle );
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
    vr::EVRSettingsError vrSettingsError;

    vr::VRSettings()->RemoveKeyInSection(
        vr::k_pch_CollisionBounds_Section,
        vr::k_pch_CollisionBounds_ColorGammaA_Int32,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vr::k_pch_CollisionBounds_ColorGammaA_Int32
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        vr::k_pch_CollisionBounds_Section,
        vr::k_pch_CollisionBounds_FadeDistance_Float,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vr::k_pch_CollisionBounds_FadeDistance_Float
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        vr::k_pch_CollisionBounds_Section,
        vr::k_pch_CollisionBounds_CenterMarkerOn_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vr::k_pch_CollisionBounds_CenterMarkerOn_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        vr::k_pch_CollisionBounds_Section,
        vr::k_pch_CollisionBounds_PlaySpaceOn_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vr::k_pch_CollisionBounds_PlaySpaceOn_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

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
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetInt32( vr::k_pch_CollisionBounds_Section,
                                    vr::k_pch_CollisionBounds_Style_Int32,
                                    m_chaperoneSwitchToBeginnerLastStyle,
                                    &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \""
                           << vr::k_pch_CollisionBounds_Style_Int32
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
    }
}

} // namespace advsettings
