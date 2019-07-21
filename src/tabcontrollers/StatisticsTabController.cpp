#include "StatisticsTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings
{
void StatisticsTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
}

void StatisticsTabController::eventLoopTick(
    vr::TrackedDevicePose_t* devicePoses,
    float leftSpeed,
    float rightSpeed )
{
    vr::Compositor_CumulativeStats pStats;
    vr::VRCompositor()->GetCumulativeStats(
        &pStats, sizeof( vr::Compositor_CumulativeStats ) );
    if ( pStats.m_nPid != m_cumStats.m_nPid )
    {
        m_cumStats = pStats;
        m_droppedFramesOffset = 0;
        m_reprojectedFramesOffset = 0;
        m_timedOutOffset = 0;
        m_totalRatioPresentedOffset = 0;
        m_totalRatioReprojectedOffset = 0;
    }
    m_cumStats = pStats;

    auto& m = devicePoses->mDeviceToAbsoluteTracking.m;

    // Hmd Distance //
    if ( lastPosTimer == 0 )
    {
        if ( devicePoses[0].bPoseIsValid
             && devicePoses[0].eTrackingResult
                    == vr::TrackingResult_Running_OK )
        {
            if ( !lastHmdPosValid )
            {
                lastHmdPosValid = true;
            }
            else
            {
                float delta = static_cast<float>( std::sqrt(
                    std::pow( m[0][3] - lastHmdPos[0],
                              2 ) /*+ std::pow(m[1][3] - lastHmdPos[1], 2)*/
                    + std::pow( m[2][3] - lastHmdPos[2], 2 ) ) );
                if ( delta >= 0.01f )
                {
                    m_hmdDistanceMoved += static_cast<double>( delta );
                }
            }
            lastHmdPos[0] = m[0][3];
            lastHmdPos[1] = m[1][3];
            lastHmdPos[2] = m[2][3];
        }
        else
        {
            lastHmdPosValid = false;
        }
    }

    // Controller speeds //
    if ( leftSpeed > m_leftControllerMaxSpeed )
    {
        m_leftControllerMaxSpeed = leftSpeed;
    }
    if ( rightSpeed > m_rightControllerMaxSpeed )
    {
        m_rightControllerMaxSpeed = rightSpeed;
    }

    // HMD Rotation //
    double spaceHmdYawTotal
        = parent->m_moveCenterTabController.getHmdYawTotal();
    m_hmdRotation = static_cast<float>( spaceHmdYawTotal / ( 2.0 * M_PI ) );

    if ( lastPosTimer <= 0 )
    {
        lastPosTimer = 10;
    }
    else
    {
        lastPosTimer--;
    }
}

float StatisticsTabController::hmdDistanceMoved() const
{
    return static_cast<float>( m_hmdDistanceMoved );
}

float StatisticsTabController::hmdRotations() const
{
    return m_hmdRotation;
}

float StatisticsTabController::rightControllerMaxSpeed() const
{
    return m_rightControllerMaxSpeed;
}

float StatisticsTabController::leftControllerMaxSpeed() const
{
    return m_leftControllerMaxSpeed;
}

unsigned StatisticsTabController::presentedFrames() const
{
    return m_cumStats.m_nNumFramePresents - m_presentedFramesOffset;
}

unsigned StatisticsTabController::droppedFrames() const
{
    return m_cumStats.m_nNumDroppedFrames - m_droppedFramesOffset;
}

unsigned StatisticsTabController::reprojectedFrames() const
{
    return m_cumStats.m_nNumReprojectedFrames - m_reprojectedFramesOffset;
}

unsigned StatisticsTabController::timedOut() const
{
    return m_cumStats.m_nNumTimedOut - m_timedOutOffset;
}

float StatisticsTabController::totalReprojectedRatio() const
{
    float totalFrames = static_cast<float>(
        ( m_cumStats.m_nNumFramePresents - m_totalRatioPresentedOffset ) );
    float reprojectedFrames = static_cast<float>( (
        m_cumStats.m_nNumReprojectedFrames - m_totalRatioReprojectedOffset ) );
    if ( totalFrames != 0.0f )
    {
        return reprojectedFrames / totalFrames;
    }
    else
    {
        return 0.0;
    }
}

void StatisticsTabController::statsDistanceResetClicked()
{
    lastHmdPosValid = false;
    if ( m_hmdDistanceMoved != 0.0 )
    {
        m_hmdDistanceMoved = 0.0;
    }
}

void StatisticsTabController::statsRotationResetClicked()
{
    // rotationResetFlag = true;
    parent->m_moveCenterTabController.resetHmdYawTotal();
}

void StatisticsTabController::statsLeftControllerSpeedResetClicked()
{
    if ( m_leftControllerMaxSpeed != 0.0f )
    {
        m_leftControllerMaxSpeed = 0.0;
    }
}

void StatisticsTabController::statsRightControllerSpeedResetClicked()
{
    if ( m_rightControllerMaxSpeed != 0.0f )
    {
        m_rightControllerMaxSpeed = 0.0;
    }
}

void StatisticsTabController::presentedFramesResetClicked()
{
    m_presentedFramesOffset = m_cumStats.m_nNumFramePresents;
}

void StatisticsTabController::droppedFramesResetClicked()
{
    m_droppedFramesOffset = m_cumStats.m_nNumDroppedFrames;
}

void StatisticsTabController::reprojectedFramesResetClicked()
{
    m_reprojectedFramesOffset = m_cumStats.m_nNumReprojectedFrames;
}

void StatisticsTabController::timedOutResetClicked()
{
    m_timedOutOffset = m_cumStats.m_nNumTimedOut;
}

void StatisticsTabController::totalRatioResetClicked()
{
    m_totalRatioPresentedOffset = m_cumStats.m_nNumFramePresents;
    m_totalRatioReprojectedOffset = m_cumStats.m_nNumReprojectedFrames;
}

} // namespace advsettings
