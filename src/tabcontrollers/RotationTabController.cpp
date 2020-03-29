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
    m_chaperoneSettingsUpdateCounter
        = utils::adjustUpdateRate( k_chaperoneSettingsUpdateCounter );
    eventLoopTick( nullptr );
}

void RotationTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
}

RotationTabController::~RotationTabController()
{
    m_chaperoneHapticFeedbackActive = false;
    if ( m_chaperoneHapticFeedbackThread.joinable() )
    {
        m_chaperoneHapticFeedbackThread.join();
    }
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
        if ( m_autoturnEnabled )
        {
            doAutoTurn( poseHmd, minDistance );
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
        if ( m_autoturnVestibularMotionEnabled )
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

                double rotationAmount
                    = ( distanceChange
                        / ( 2.0 * M_PI * m_autoturnVestibularMotionRadius ) )
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

        if ( m_autoturnMode == AutoturnModes::LINEAR_SMOOTH_TURN
             && m_autoturnLinearSmoothTurnRemaining != 0 )
        {
            // TODO: implement angular acceleration max?
            auto deltaMillis
                = std::chrono::duration_cast<std::chrono::milliseconds>(
                      currentTime - m_autoturnLastUpdate )
                      .count();
            auto miniDeltaAngle = static_cast<int>(
                std::abs( ( deltaMillis * m_autoturnLinearTurnSpeed ) / 1000 )
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
            if ( chaperoneQuad.distance <= m_activationDistance
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
                                <= m_cordDetanglingAngle )
                              && ( std::abs( parent->m_moveCenterTabController
                                                 .getHmdYawTotal() )
                                   > m_autoturnMinCordTangle ) )
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
                    if ( m_autoturnUseCornerAngle && cornerShared )
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
                    if ( m_autoturnMode == AutoturnModes::SNAP )
                    {
                        int newRotationAngleDeg = static_cast<int>(
                            parent->m_moveCenterTabController.rotation()
                            + delta_degrees );

                        parent->m_moveCenterTabController.setRotation(
                            newRotationAngleDeg );
                    }
                    else if ( m_autoturnMode
                              == AutoturnModes::LINEAR_SMOOTH_TURN )
                    {
                        m_autoturnLinearSmoothTurnRemaining
                            += static_cast<int>( delta_degrees );
                    }
                } while ( false );

                m_autoturnWallActive[i] = true;
            }
            else if ( ( chaperoneQuad.distance
                        > ( m_activationDistance + m_deactivateDistance ) )
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
    // TODO settings/save
}

float RotationTabController::autoTurnActivationDistance() const
{
    // TODO settings/save
}

float RotationTabController::autoTurnDeactivationDistance() const
{
    // TODO settings/save
}

double RotationTabController::cordDetangleAngle() const
{
    // TODO settings/save
}

double RotationTabController::minCordAngle() const
{
    // TODO settings/save
}

int RotationTabController::autoTurnSpeed() const
{
    // TODO settings/save
}

RotationTabController::AutoturnModes RotationTabController::autoTurnMode() const
{
    // TODO settings/save
}

bool RotationTabController::vestibularMotionEnabled() const
{
    // TODO settings/save
}

double RotationTabController::vestibularMotionRadius() const
{
    // TODO settings/save
}

// Setters

void RotationTabController::setAutoTurnEnabled( bool value, bool notify )
{
    // TODO logic
    if ( notify )
    {
        emit autoTurnEnabledChanged( value );
    }
}
void RotationTabController::setAutoTurnActivationDistance( float value,
                                                           bool notify )
{
    // TODO logic
    if ( notify )
    {
        emit autoTurnActivationDistanceChanged( value );
    }
}
void RotationTabController::setAutoTurnDeactivationDistance( float value,
                                                             bool notify )
{
    // TODO logic
    if ( notify )
    {
        emit autoTurnDeactivationDistanceChanged( value );
    }
}
void RotationTabController::setCordDetangleAngle( double value, bool notify )
{
    // TODO logic
    if ( notify )
    {
        emit cordDetangleAngleChanged( value );
    }
}
void RotationTabController::minCordAngle( double value, bool notify )
{
    // TODO logic
    if ( notify )
    {
        emit minCordAngleChanged( value );
    }
}
void RotationTabController::setAutoTurnSpeed( int value, bool notify )
{
    // TODO logic
    if ( notify )
    {
        emit autoTurnSpeedChanged( value );
    }
}
void RotationTabController::setAutoTurnMode(
    RotationTabController::AutoturnModes value,
    bool notify )
{
    // TODO logic
    if ( notify )
    {
        emit autoTurnModeChanged( value );
    }
}
void RotationTabController::setVestibularMotionEnabled( bool value,
                                                        bool notify )
{
    // TODO logic
    if ( notify )
    {
        emit vestibularMotionEnabledChanged( value );
    }
}
void RotationTabController::setVestibularMotionRadius( double value,
                                                       bool notify )
{
    // TODO logic
    if ( notify )
    {
        emit vestibularMotionRadiusChanged( value );
    }
}

} // namespace advsettings
