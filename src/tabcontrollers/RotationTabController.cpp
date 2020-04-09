#include "RotationTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"
#include "../settings/settings.h"
#include "../utils/Matrix.h"
#include "../quaternion/quaternion.h"
#include <cmath>

// application namespace
namespace advsettings
{
void RotationTabController::initStage1()
{ // TODO adjust update rate
  // m_chaperoneSettingsUpdateCounter
  //   = utils::adjustUpdateRate( k_chaperoneSettingsUpdateCounter );
}

void RotationTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
}

void RotationTabController::eventLoopTick(
    vr::TrackedDevicePose_t* devicePoses )
{
    if ( devicePoses )
    {
        m_isHMDActive = false;
        std::lock_guard<std::recursive_mutex> lock(
            parent->chaperoneUtils().mutex() );
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
        // TODO: this doesn't need to run if both are off
        if ( poseHmd.bPoseIsValid && poseHmd.bDeviceIsConnected
             && poseHmd.eTrackingResult == vr::TrackingResult_Running_OK )
        {
            auto chaperoneDistances
                = parent->chaperoneUtils().getDistancesToChaperone(
                    { poseHmd.mDeviceToAbsoluteTracking.m[0][3],
                      poseHmd.mDeviceToAbsoluteTracking.m[1][3],
                      poseHmd.mDeviceToAbsoluteTracking.m[2][3] } );

            // Autoturn mode
            if ( RotationTabController::autoTurnEnabled() )
            {
                doAutoTurn( poseHmd, chaperoneDistances );
            }
            // Vestibular motion. Dependent on autoTurn so the playspace
            // doesn't move when you use the keybind
            if ( RotationTabController::vestibularMotionEnabled() )
            {
                doVestibularMotion( poseHmd, chaperoneDistances );
            }

            m_autoTurnChaperoneDistancesLast = std::move( chaperoneDistances );
        }
    }
}

void RotationTabController::doVestibularMotion(
    const vr::TrackedDevicePose_t& poseHmd,
    const std::vector<utils::ChaperoneQuadData>& chaperoneDistances )
{
    if ( m_isHMDActive && poseHmd.bPoseIsValid && poseHmd.bDeviceIsConnected
         && poseHmd.eTrackingResult == vr::TrackingResult_Running_OK
         && !chaperoneDistances.empty() )
    {
        if ( chaperoneDistances.size()
             != m_autoTurnChaperoneDistancesLast.size() )
        {
            m_autoTurnChaperoneDistancesLast = chaperoneDistances;
            m_autoTurnLastHmdUpdate = poseHmd.mDeviceToAbsoluteTracking;
        }
        // Find the nearest wall we're moving TOWARDS and rotate
        // away from it Rotate dist/(2*pi*r) where r is
        // m_autoTurnVestibularMotionRadius, as if we had walked
        // however many inches along a circle and the world was
        // turning to compensate

        do
        {
            // find nearest wall we're moving towards
            auto nearestWall = chaperoneDistances.end();
            auto itrLast = m_autoTurnChaperoneDistancesLast.begin();
            for ( auto itr = chaperoneDistances.begin();
                  itr != chaperoneDistances.end();
                  itr++ )
            {
                itrLast++;
                if ( itr->distance > itrLast->distance )
                {
                    continue;
                }

                if ( nearestWall == chaperoneDistances.end()
                     || itr->distance < nearestWall->distance )
                {
                    nearestWall = itr;
                }
            }
            if ( nearestWall == chaperoneDistances.end() )
            {
                break;
            }

            // Convert pose matrix to quaternion
            auto hmdQuaternion = quaternion::fromHmdMatrix34(
                poseHmd.mDeviceToAbsoluteTracking );

            // Get HMD raw yaw
            double hmdYaw = quaternion::getYaw( hmdQuaternion );

            // Get angle between HMD position and nearest point on
            // wall
            double hmdPositionToWallYaw = static_cast<double>(
                std::atan2( poseHmd.mDeviceToAbsoluteTracking.m[0][3]
                                - nearestWall->nearestPoint.v[0],
                            poseHmd.mDeviceToAbsoluteTracking.m[2][3]
                                - nearestWall->nearestPoint.v[2] ) );

            // Get angle between HMD and wall
            double hmdToWallYaw
                = reduceAngle<>( hmdYaw - hmdPositionToWallYaw, -M_PI, M_PI );
            bool turnLeft = hmdToWallYaw > 0.0;

            // Get the distance between previous and current
            // position
            double distanceChange = std::sqrt(
                std::pow( poseHmd.mDeviceToAbsoluteTracking.m[0][3]
                              - m_autoTurnLastHmdUpdate.m[0][3],
                          2.0 )
                + std::pow( poseHmd.mDeviceToAbsoluteTracking.m[0][3]
                                - m_autoTurnLastHmdUpdate.m[0][3],
                            2.0 ) );

            double rotationAmount
                = ( distanceChange
                    / ( 2.0 * M_PI
                        * RotationTabController::vestibularMotionRadius() ) )
                  * ( turnLeft ? 1 : -1 );

            double newRotationAngleDeg
                = std::fmod( parent->m_moveCenterTabController.rotation()
                                 + ( rotationAmount * k_radiansToCentidegrees )
                                 + m_autoTurnRoundingError,
                             360000.0 );
            int newRotationAngleInt = static_cast<int>( newRotationAngleDeg );
            m_autoTurnRoundingError = newRotationAngleDeg - newRotationAngleInt;

            parent->m_moveCenterTabController.setRotation(
                newRotationAngleInt );

        } while ( false );
        m_autoTurnLastHmdUpdate = poseHmd.mDeviceToAbsoluteTracking;
    }
}

void RotationTabController::doAutoTurn(
    const vr::TrackedDevicePose_t& poseHmd,
    const std::vector<utils::ChaperoneQuadData>& chaperoneDistances )
{
    if ( m_isHMDActive && poseHmd.bPoseIsValid && poseHmd.bDeviceIsConnected
         && poseHmd.eTrackingResult == vr::TrackingResult_Running_OK
         && !chaperoneDistances.empty() )
    {
        auto currentTime = std::chrono::steady_clock::now();

        if ( m_autoTurnWallActive.size() != chaperoneDistances.size() )
        {
            // Chaperone changed
            m_autoTurnWallActive.clear();
            // Initialize all to 'true' so we don't rotate on startup if
            // the user is outside of the play area.
            m_autoTurnWallActive.resize( chaperoneDistances.size(), true );
            m_autoTurnLastUpdate = currentTime;
        }

        // Apply smooth rotation per-frame: get the time since last frame,
        // multiply by speed
        if ( RotationTabController::autoTurnModeType()
                 == AutoTurnModes::LINEAR_SMOOTH_TURN
             && m_autoTurnLinearSmoothTurnRemaining != 0 )
        {
            // TODO: implement angular acceleration max?
            auto deltaMillis
                = std::chrono::duration_cast<std::chrono::milliseconds>(
                      currentTime - m_autoTurnLastUpdate )
                      .count();
            auto miniDeltaAngle = static_cast<int>(
                std::abs(
                    ( deltaMillis * RotationTabController::autoTurnSpeed() )
                    / 1000 )
                * ( m_autoTurnLinearSmoothTurnRemaining < 0 ? -1 : 1 ) );
            if ( std::abs( m_autoTurnLinearSmoothTurnRemaining )
                 < std::abs( miniDeltaAngle ) )
            {
                miniDeltaAngle = m_autoTurnLinearSmoothTurnRemaining;
            }
            int newRotationAngleDeg = reduceAngle<>(
                static_cast<int>( parent->m_moveCenterTabController.rotation()
                                  + miniDeltaAngle ),
                0,
                36000 );

            parent->m_moveCenterTabController.setRotation(
                newRotationAngleDeg );
            m_autoTurnLinearSmoothTurnRemaining -= miniDeltaAngle;
        }

        for ( size_t i = 0; i < chaperoneDistances.size(); i++ )
        {
            const auto& chaperoneQuad = chaperoneDistances[i];
            if ( chaperoneQuad.distance
                     <= RotationTabController::autoTurnActivationDistance()
                 && !m_autoTurnWallActive[i] )
            {
                // Convert pose matrix to quaternion
                auto hmdQuaternion = quaternion::fromHmdMatrix34(
                    poseHmd.mDeviceToAbsoluteTracking );

                // Get HMD raw yaw
                double hmdYaw = quaternion::getYaw( hmdQuaternion );

                // Get angle between HMD position and nearest point on
                // wall
                double hmdPositionToWallYaw = static_cast<double>(
                    std::atan2( poseHmd.mDeviceToAbsoluteTracking.m[0][3]
                                    - chaperoneQuad.nearestPoint.v[0],
                                poseHmd.mDeviceToAbsoluteTracking.m[2][3]
                                    - chaperoneQuad.nearestPoint.v[2] ) );

                // Get angle between HMD and wall
                double hmdToWallYaw = reduceAngle<>(
                    hmdYaw - hmdPositionToWallYaw, -M_PI, M_PI );

                do
                {
                    // Ignore if the wall we encountered is behind us
                    if ( std::abs( hmdToWallYaw ) >= M_PI / 2 )
                    {
                        LOG( DEBUG ) << "Ignoring turn in opposite "
                                        "direction (angle "
                                     << std::abs( hmdToWallYaw ) << ")";
                        break;
                    }

                    // If the closest corner shares a wall with the last
                    // wall we turned at, turn relative to that corner
                    bool cornerShared
                        = m_autoTurnWallActive[circularIndex(
                              i, true, chaperoneDistances.size() )]
                          || m_autoTurnWallActive[circularIndex(
                              i, false, chaperoneDistances.size() )];

                    bool turnLeft = true;
                    // Turn away from corner
                    if ( cornerShared )
                    {
                        // Turn left or right depending on which corner
                        // it is. If we go based on yaw, we could end up
                        // turning the wrong way if it's large obtuse
                        // angle and we're facing more towards the
                        // previous wall than the left.
                        turnLeft = m_autoTurnWallActive[circularIndex(
                            i, false, chaperoneDistances.size() )];
                        LOG( DEBUG ) << "turning away from shared corner";
                    }
                    // If within m_cordDetanglingAngle degrees of
                    // 'straight at a wall', start in whatever direction
                    // will start untangling your cord
                    else if ( ( std::abs( hmdToWallYaw )
                                <= RotationTabController::cordDetangleAngle() )
                              && ( std::abs( parent->m_moveCenterTabController
                                                 .getHmdYawTotal() )
                                   > RotationTabController::minCordTangle() ) )
                    {
                        turnLeft = ( parent->m_moveCenterTabController
                                         .getHmdYawTotal()
                                     < 0.0 );
                        LOG( DEBUG ) << "turning to detangle cord";
                    }
                    // Turn the closest angle to the wall
                    else
                    {
                        turnLeft = hmdToWallYaw > 0.0;
                        LOG( DEBUG ) << "turning closest angle to wall";
                    }

                    LOG( DEBUG ) << "hmd yaw " << hmdYaw
                                 << ", hmd position to wall angle "
                                 << hmdPositionToWallYaw;
                    LOG( DEBUG ) << "hmd to wall angle " << hmdToWallYaw;
                    // Positive hmd-to-wall is facing left, negative is
                    // facing right (relative to the wall)
                    double delta_degrees
                        = ( ( turnLeft ? M_PI / 2 : -M_PI / 2 ) - hmdToWallYaw )
                          * k_radiansToCentidegrees;
                    if ( RotationTabController::autoTurnUseCornerAngle()
                         && cornerShared )
                    {
                        // Turn the angle of the corner
                        // Three relevant points - the far corner wall
                        // we're currently touching, the far corner on
                        // the wall we've just touched, and the corner
                        // between them
                        const size_t cornerCnt
                            = parent->chaperoneUtils().quadsCount();
                        const size_t middleCornerIdx
                            = turnLeft ? i
                                       : circularIndex( i, true, cornerCnt );
                        const auto& middleCorner
                            = parent->chaperoneUtils().getCorner(
                                middleCornerIdx );
                        const auto& newWallCorner
                            = parent->chaperoneUtils().getCorner( circularIndex(
                                middleCornerIdx, turnLeft, cornerCnt ) );
                        const auto& touchingWallCorner
                            = parent->chaperoneUtils().getCorner( circularIndex(
                                middleCornerIdx, !turnLeft, cornerCnt ) );

                        double newWallAngle = static_cast<double>( std::atan2(
                            middleCorner.v[0] - newWallCorner.v[0],
                            middleCorner.v[2] - newWallCorner.v[2] ) );
                        double touchingWallAngle
                            = static_cast<double>( std::atan2(
                                middleCorner.v[0] - touchingWallCorner.v[0],
                                middleCorner.v[2] - touchingWallCorner.v[2] ) );
                        LOG( DEBUG ) << "twa: " << touchingWallAngle
                                     << ", nwa: " << newWallAngle << ", diff: "
                                     << ( newWallAngle - touchingWallAngle );
                        delta_degrees = ( newWallAngle - touchingWallAngle
                                          + ( turnLeft ? M_PI : -M_PI ) )
                                        * k_radiansToCentidegrees;
                        delta_degrees
                            = reduceAngle<>( delta_degrees, -18000.0, 18000.0 );
                    }
                    LOG( DEBUG ) << "rotating space " << ( delta_degrees / 100 )
                                 << " degrees";
                    switch ( RotationTabController::autoTurnModeType() )
                    {
                    case AutoTurnModes::SNAP:
                        parent->m_moveCenterTabController.setRotation(
                            static_cast<int>(
                                parent->m_moveCenterTabController.rotation()
                                + delta_degrees ) );
                        break;
                    case AutoTurnModes::LINEAR_SMOOTH_TURN:
                        m_autoTurnLinearSmoothTurnRemaining
                            += static_cast<int>( delta_degrees );
                        break;
                    }
                } while ( false );

                m_autoTurnWallActive[i] = true;
            }
            else if ( ( chaperoneQuad.distance
                        > ( RotationTabController::autoTurnActivationDistance()
                            + RotationTabController::
                                autoTurnDeactivationDistance() ) )
                      && m_autoTurnWallActive[i] )
            {
                m_autoTurnWallActive[i] = false;
            }
        }
        m_autoTurnLastUpdate = currentTime;
    }
    else
    {
        m_autoTurnWallActive.clear();
    }
}

// getters

bool RotationTabController::autoTurnEnabled() const
{
    return settings::getSetting(
        settings::BoolSetting::ROTATION_autoturnEnabled );
}

float RotationTabController::autoTurnActivationDistance() const
{
    return static_cast<float>( settings::getSetting(
        settings::DoubleSetting::ROTATION_activationDistance ) );
}

float RotationTabController::autoTurnDeactivationDistance() const
{
    return static_cast<float>( settings::getSetting(
        settings::DoubleSetting::ROTATION_deactivateDistance ) );
}

bool RotationTabController::autoTurnUseCornerAngle() const
{
    return settings::getSetting(
        settings::BoolSetting::ROTATION_autoturnUseCornerAngle );
}

double RotationTabController::cordDetangleAngle() const
{
    return settings::getSetting(
        settings::DoubleSetting::ROTATION_cordDetanglingAngle );
}

double RotationTabController::minCordTangle() const
{
    return settings::getSetting(
        settings::DoubleSetting::ROTATION_autoturnMinCordTangle );
}

int RotationTabController::autoTurnSpeed() const
{
    return settings::getSetting(
        settings::IntSetting::ROTATION_autoturnLinearTurnSpeed );
}

AutoTurnModes RotationTabController::autoTurnModeType() const
{
    return static_cast<AutoTurnModes>(
        settings::getSetting( settings::IntSetting::ROTATION_autoturnMode ) );
}

int RotationTabController::autoTurnMode() const
{
    return settings::getSetting( settings::IntSetting::ROTATION_autoturnMode );
}

bool RotationTabController::vestibularMotionEnabled() const
{
    return settings::getSetting(
        settings::BoolSetting::ROTATION_autoturnVestibularMotionEnabled );
}

double RotationTabController::vestibularMotionRadius() const
{
    return settings::getSetting(
        settings::DoubleSetting::ROTATION_autoturnVestibularMotionRadius );
}

// Setters

void RotationTabController::setAutoTurnEnabled( bool value, bool notify )
{
    settings::setSetting( settings::BoolSetting::ROTATION_autoturnEnabled,
                          value );

    if ( notify )
    {
        emit autoTurnEnabledChanged( value );
    }
}
void RotationTabController::setAutoTurnActivationDistance( float value,
                                                           bool notify )
{
    settings::setSetting( settings::DoubleSetting::ROTATION_activationDistance,
                          static_cast<double>( value ) );
    if ( notify )
    {
        emit autoTurnActivationDistanceChanged( value );
    }
}
void RotationTabController::setAutoTurnDeactivationDistance( float value,
                                                             bool notify )
{
    settings::setSetting( settings::DoubleSetting::ROTATION_deactivateDistance,
                          static_cast<double>( value ) );
    if ( notify )
    {
        emit autoTurnDeactivationDistanceChanged( value );
    }
}

void RotationTabController::setAutoTurnUseCornerAngle( bool value, bool notify )
{
    settings::setSetting(
        settings::BoolSetting::ROTATION_autoturnUseCornerAngle, value );
    if ( notify )
    {
        emit autoTurnUseCornerAngleChanged( value );
    }
}

void RotationTabController::setCordDetangleAngle( double value, bool notify )
{
    settings::setSetting( settings::DoubleSetting::ROTATION_cordDetanglingAngle,
                          value );
    if ( notify )
    {
        emit cordDetangleAngleChanged( value );
    }
}

void RotationTabController::setMinCordTangle( double value, bool notify )
{
    settings::setSetting(
        settings::DoubleSetting::ROTATION_autoturnMinCordTangle, value );
    if ( notify )
    {
        emit minCordTangleChanged( value );
    }
}
void RotationTabController::setAutoTurnSpeed( int value, bool notify )
{
    settings::setSetting(
        settings::IntSetting::ROTATION_autoturnLinearTurnSpeed, value );
    if ( notify )
    {
        emit autoTurnSpeedChanged( value );
    }
}
void RotationTabController::setAutoTurnMode( int value, bool notify )
{
    settings::setSetting( settings::IntSetting::ROTATION_autoturnMode,
                          static_cast<int>( value ) );
    if ( notify )
    {
        emit autoTurnModeChanged( value );
    }
}
void RotationTabController::setVestibularMotionEnabled( bool value,
                                                        bool notify )
{
    settings::setSetting(
        settings::BoolSetting::ROTATION_autoturnVestibularMotionEnabled,
        value );
    if ( notify )
    {
        emit vestibularMotionEnabledChanged( value );
    }
}
void RotationTabController::setVestibularMotionRadius( double value,
                                                       bool notify )
{
    settings::setSetting(
        settings::DoubleSetting::ROTATION_autoturnVestibularMotionRadius,
        value );
    if ( notify )
    {
        emit vestibularMotionRadiusChanged( value );
    }
}

} // namespace advsettings
