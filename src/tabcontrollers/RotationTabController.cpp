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
    eventLoopTick( nullptr );
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

        // Autoturn mode
        if ( RotationTabController::autoTurnEnabled() )
        {
            doAutoTurn( poseHmd, minDistance );
        }
    }
}

void RotationTabController::doAutoTurn( vr::TrackedDevicePose_t poseHmd,
                                        float minDistance )
{
    if ( m_isHMDActive && poseHmd.bPoseIsValid && poseHmd.bDeviceIsConnected
         && poseHmd.eTrackingResult == vr::TrackingResult_Running_OK
         && !std::isnan( minDistance ) )
    {
        auto currentTime = std::chrono::steady_clock::now();

        auto chaperoneDistances
            = parent->chaperoneUtils().getDistancesToChaperone(
                { poseHmd.mDeviceToAbsoluteTracking.m[0][3],
                  poseHmd.mDeviceToAbsoluteTracking.m[1][3],
                  poseHmd.mDeviceToAbsoluteTracking.m[2][3] } );
        if ( m_autoturnWallActive.size() != chaperoneDistances.size() )
        {
            // Chaperone changed
            m_autoturnWallActive.clear();
            // Initialize all to 'true' so we don't rotate on startup if
            // the user is outside of the play area.
            m_autoturnWallActive.resize( chaperoneDistances.size(), true );
            m_autoturnLastUpdate = currentTime;
        }

        // Vestibular motion (true infinite-walk)
        if ( RotationTabController::vestibularMotionEnabled() )
        {
            if ( chaperoneDistances.size()
                 != m_autoturnChaperoneDistancesLast.size() )
            {
                m_autoturnChaperoneDistancesLast = chaperoneDistances;
                m_autoturnLastHmdUpdate = poseHmd.mDeviceToAbsoluteTracking;
            }
            // Find the nearest wall we're moving TOWARDS and rotate
            // away from it Rotate dist/(2*pi*r) where r is
            // m_autoturnVestibularMotionRadius, as if we had walked
            // however many inches along a circle and the world was
            // turning to compensate

            do
            {
                // find nearest wall we're moving towards
                auto nearestWall = chaperoneDistances.end();
                auto itrLast = m_autoturnChaperoneDistancesLast.begin();
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
                double hmdToWallYaw = hmdYaw - hmdPositionToWallYaw;
                if ( hmdToWallYaw >= M_PI )
                {
                    hmdToWallYaw -= 2 * M_PI;
                }
                else if ( hmdToWallYaw <= -M_PI )
                {
                    hmdToWallYaw += 2 * M_PI;
                }
                bool turnLeft = hmdToWallYaw > 0.0;

                // Get the distance between previous and current
                // position
                double distanceChange = std::sqrt(
                    std::pow( poseHmd.mDeviceToAbsoluteTracking.m[0][3]
                                  - m_autoturnLastHmdUpdate.m[0][3],
                              2.0 )
                    + std::pow( poseHmd.mDeviceToAbsoluteTracking.m[0][3]
                                    - m_autoturnLastHmdUpdate.m[0][3],
                                2.0 ) );

                double rotationAmount = ( distanceChange
                                          / ( 2.0 * M_PI
                                              * RotationTabController::
                                                  vestibularMotionRadius() ) )
                                        * ( turnLeft ? 1 : -1 );

                double newRotationAngleDeg = std::fmod(
                    parent->m_moveCenterTabController.rotation()
                        + ( rotationAmount * k_radiansToCentidegrees )
                        + m_autoturnRoundingError,
                    360000.0 );
                int newRotationAngleInt
                    = static_cast<int>( newRotationAngleDeg );
                m_autoturnRoundingError
                    = newRotationAngleDeg - newRotationAngleInt;

                parent->m_moveCenterTabController.setRotation(
                    newRotationAngleInt );

            } while ( false );
            m_autoturnLastHmdUpdate = poseHmd.mDeviceToAbsoluteTracking;
        }

        if ( RotationTabController::autoTurnMode()
                 == AutoturnModes::LINEAR_SMOOTH_TURN
             && m_autoturnLinearSmoothTurnRemaining != 0 )
        {
            // TODO: implement angular acceleration max?
            auto deltaMillis
                = std::chrono::duration_cast<std::chrono::milliseconds>(
                      currentTime - m_autoturnLastUpdate )
                      .count();
            auto miniDeltaAngle = static_cast<int>(
                std::abs(
                    ( deltaMillis * RotationTabController::autoTurnSpeed() )
                    / 1000 )
                * ( m_autoturnLinearSmoothTurnRemaining < 0 ? -1 : 1 ) );
            if ( std::abs( m_autoturnLinearSmoothTurnRemaining )
                 < std::abs( miniDeltaAngle ) )
            {
                miniDeltaAngle = m_autoturnLinearSmoothTurnRemaining;
            }
            int newRotationAngleDeg
                = static_cast<int>( parent->m_moveCenterTabController.rotation()
                                    + miniDeltaAngle )
                  % 360000;

            parent->m_moveCenterTabController.setRotation(
                newRotationAngleDeg );
            m_autoturnLinearSmoothTurnRemaining -= miniDeltaAngle;
        }

        for ( size_t i = 0; i < chaperoneDistances.size(); i++ )
        {
            const auto& chaperoneQuad = chaperoneDistances[i];
            if ( chaperoneQuad.distance
                     <= RotationTabController::autoTurnActivationDistance()
                 && !m_autoturnWallActive[i] )
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
                double hmdToWallYaw = hmdYaw - hmdPositionToWallYaw;
                if ( hmdToWallYaw >= M_PI )
                {
                    hmdToWallYaw -= 2 * M_PI;
                }
                else if ( hmdToWallYaw <= -M_PI )
                {
                    hmdToWallYaw += 2 * M_PI;
                }

                do
                {
                    // Ignore if the wall we encountered is behind us
                    if ( std::abs( hmdToWallYaw ) >= M_PI / 2 )
                    {
                        LOG( INFO ) << "Ignoring turn in opposite "
                                       "direction (angle "
                                    << std::abs( hmdToWallYaw ) << ")";
                        break;
                    }

                    // If the closest corner shares a wall with the last
                    // wall we turned at, turn relative to that corner
                    bool cornerShared
                        = ( m_autoturnWallActive[( i + 1 )
                                                 % chaperoneDistances.size()] )
                          || ( m_autoturnWallActive
                                   [( i == 0 )
                                        ? ( chaperoneDistances.size() - 1 )
                                        : ( i - 1 )] );

                    bool turnLeft = true;
                    // Turn away from corner
                    if ( cornerShared )
                    {
                        // Turn left or right depending on which corner
                        // it is. If we go based on yaw, we could end up
                        // turning the wrong way if it's large obtuse
                        // angle and we're facing more towards the
                        // previous wall than the left.
                        turnLeft = !m_autoturnWallActive
                                       [( i + 1 ) % chaperoneDistances.size()];
                        LOG( INFO ) << "turning away from shared corner";
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
                        LOG( INFO ) << "turning to detangle cord";
                    }
                    // Turn the closest angle to the wall
                    else
                    {
                        turnLeft = hmdToWallYaw > 0.0;
                        LOG( INFO ) << "turning closest angle to wall";
                    }

                    // Limit maximum overall turns to 2
                    // const double max_turns = 2 * ( 2 * M_PI );
                    /*
                    // TODO: Turn change wall color if you turn too far?
                    bool exceededTurning
                        = (
                    parent->m_moveCenterTabController.getHmdYawTotal()
                                >= max_turns
                            && !turnLeft )
                          || ( parent->m_moveCenterTabController
                                       .getHmdYawTotal()
                                   <= -max_turns
                               && turnLeft );
                    */
                    LOG( INFO ) << "hmd yaw " << hmdYaw
                                << ", hmd position to wall angle "
                                << hmdPositionToWallYaw;
                    LOG( INFO ) << "hmd to wall angle " << hmdToWallYaw;
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
                        const auto wrapAround
                            = []( const size_t& idx,
                                  const bool& increment,
                                  const size_t& modulus ) -> size_t {
                            if ( increment )
                            {
                                return ( idx + 1 ) % modulus;
                            }
                            else
                            {
                                return ( idx == 0 ) ? ( modulus - 1 )
                                                    : ( idx - 1 );
                            }
                        };
                        const size_t cornerCnt
                            = parent->chaperoneUtils().quadsCount();
                        const size_t middleCornerIdx
                            = turnLeft ? i : wrapAround( i, true, cornerCnt );
                        const auto& middleCorner
                            = parent->chaperoneUtils().getCorner(
                                middleCornerIdx );
                        const auto& newWallCorner
                            = parent->chaperoneUtils().getCorner( wrapAround(
                                middleCornerIdx, turnLeft, cornerCnt ) );
                        const auto& touchingWallCorner
                            = parent->chaperoneUtils().getCorner( wrapAround(
                                middleCornerIdx, !turnLeft, cornerCnt ) );

                        double newWallAngle = static_cast<double>( std::atan2(
                            middleCorner.v[0] - newWallCorner.v[0],
                            middleCorner.v[2] - newWallCorner.v[2] ) );
                        double touchingWallAngle
                            = static_cast<double>( std::atan2(
                                middleCorner.v[0] - touchingWallCorner.v[0],
                                middleCorner.v[2] - touchingWallCorner.v[2] ) );
                        LOG( INFO ) << "twa: " << touchingWallAngle
                                    << ", nwa: " << newWallAngle << ", diff: "
                                    << ( newWallAngle - touchingWallAngle );
                        delta_degrees = ( newWallAngle - touchingWallAngle
                                          + ( turnLeft ? M_PI : -M_PI ) )
                                        * k_radiansToCentidegrees;
                        while ( delta_degrees >= 18000 )
                        {
                            delta_degrees -= 36000;
                        }
                        while ( delta_degrees <= -18000 )
                        {
                            delta_degrees += 36000;
                        }
                    }
                    LOG( INFO ) << "rotating space " << ( delta_degrees / 100 )
                                << " degrees";
                    if ( RotationTabController::autoTurnMode()
                         == AutoturnModes::SNAP )
                    {
                        int newRotationAngleDeg = static_cast<int>(
                            parent->m_moveCenterTabController.rotation()
                            + delta_degrees );

                        parent->m_moveCenterTabController.setRotation(
                            newRotationAngleDeg );
                    }
                    else if ( RotationTabController::autoTurnMode()
                              == AutoturnModes::LINEAR_SMOOTH_TURN )
                    {
                        m_autoturnLinearSmoothTurnRemaining
                            += static_cast<int>( delta_degrees );
                    }
                } while ( false );

                m_autoturnWallActive[i] = true;
            }
            else if ( ( chaperoneQuad.distance
                        > ( RotationTabController::autoTurnActivationDistance()
                            + RotationTabController::
                                autoTurnDeactivationDistance() ) )
                      && m_autoturnWallActive[i] )
            {
                m_autoturnWallActive[i] = false;
            }
        }
        m_autoturnLastUpdate = currentTime;
        m_autoturnChaperoneDistancesLast = std::move( chaperoneDistances );
    }
    else
    {
        m_autoturnWallActive.clear();
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

AutoturnModes RotationTabController::autoTurnMode() const
{
    return static_cast<AutoturnModes>( static_cast<AutoturnModes>(
        settings::getSetting( settings::IntSetting::ROTATION_autoturnMode ) ) );
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
void RotationTabController::setAutoTurnMode( AutoturnModes value, bool notify )
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
