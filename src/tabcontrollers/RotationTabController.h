
#pragma once

#include <QObject>
#include <memory>
#include <chrono>
#include <thread>
#include <openvr.h>
#include <cmath>
#include "../utils/FrameRateUtils.h"
#include "../utils/ChaperoneUtils.h"
#include "../settings/settings_object.h"
#include "MoveCenterTabController.h"

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;

enum class AutoTurnModes
{
    SNAP,
    LINEAR_SMOOTH_TURN
};

class RotationTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool autoTurnEnabled READ autoTurnEnabled WRITE
                    setAutoTurnEnabled NOTIFY autoTurnEnabledChanged )
    Q_PROPERTY( float autoTurnActivationDistance READ autoTurnActivationDistance
                    WRITE setAutoTurnActivationDistance NOTIFY
                        autoTurnActivationDistanceChanged )
    Q_PROPERTY(
        float autoTurnDeactivationDistance READ autoTurnDeactivationDistance
            WRITE setAutoTurnDeactivationDistance NOTIFY
                autoTurnDeactivationDistanceChanged )
    Q_PROPERTY(
        bool autoTurnUseCornerAngle READ autoTurnUseCornerAngle WRITE
            setAutoTurnUseCornerAngle NOTIFY autoTurnUseCornerAngleChanged )
    Q_PROPERTY( double cordDetangleAngle READ cordDetangleAngle WRITE
                    setCordDetangleAngle NOTIFY cordDetangleAngleChanged )
    Q_PROPERTY( double minCordTangle READ minCordTangle WRITE setMinCordTangle
                    NOTIFY minCordTangleChanged )
    Q_PROPERTY( int autoTurnSpeed READ autoTurnSpeed WRITE setAutoTurnSpeed
                    NOTIFY autoTurnSpeedChanged )
    Q_PROPERTY( int autoTurnMode READ autoTurnMode WRITE setAutoTurnMode NOTIFY
                    autoTurnModeChanged )
    Q_PROPERTY(
        bool vestibularMotionEnabled READ vestibularMotionEnabled WRITE
            setVestibularMotionEnabled NOTIFY vestibularMotionEnabledChanged )
    Q_PROPERTY(
        double vestibularMotionRadius READ vestibularMotionRadius WRITE
            setVestibularMotionRadius NOTIFY vestibularMotionRadiusChanged )

private:
    OverlayController* parent;

    // Variables
    int m_autoTurnLinearSmoothTurnRemaining = 0;
    std::chrono::steady_clock::time_point m_autoTurnLastUpdate;
    std::vector<bool> m_autoTurnWallActive;
    vr::HmdMatrix34_t m_autoTurnLastHmdUpdate;
    std::vector<utils::ChaperoneQuadData> m_autoTurnChaperoneDistancesLast;
    double m_autoTurnRoundingError = 0.0;

    bool m_isHMDActive = false;
    bool m_autoTurnFromNearestWallRequested = false;

    void updateAutoTurn(
        const vr::TrackedDevicePose_t& poseHmd,
        const std::vector<utils::ChaperoneQuadData>& chaperoneDistances );
    void doAutoTurn(
        const vr::TrackedDevicePose_t& poseHmd,
        const std::vector<utils::ChaperoneQuadData>& chaperoneDistances,
        const size_t index );
    void doAutoTurnFromNearestWall(const vr::TrackedDevicePose_t& poseHmd,
            const std::vector<utils::ChaperoneQuadData>& chaperoneDistances);
    void doVestibularMotion(
        const vr::TrackedDevicePose_t& poseHmd,
        const std::vector<utils::ChaperoneQuadData>& chaperoneDistances );

public:
    void initStage1();
    void initStage2( OverlayController* parent );

    void eventLoopTick( vr::TrackedDevicePose_t* devicePoses );

    void requestAutoTurnFromNearestWall();

    float boundsVisibility() const;

    bool autoTurnEnabled() const;
    float autoTurnActivationDistance() const;
    float autoTurnDeactivationDistance() const;
    bool autoTurnUseCornerAngle() const;
    double cordDetangleAngle() const;
    double minCordTangle() const;
    int autoTurnSpeed() const;
    AutoTurnModes autoTurnModeType() const;
    int autoTurnMode() const;
    bool vestibularMotionEnabled() const;
    double vestibularMotionRadius() const;

public slots:
    void setAutoTurnEnabled( bool value, bool notify = true );
    void setAutoTurnActivationDistance( float value, bool notify = true );
    void setAutoTurnDeactivationDistance( float value, bool notify = true );
    void setAutoTurnUseCornerAngle( bool value, bool notify = true );
    void setCordDetangleAngle( double value, bool notify = true );
    void setMinCordTangle( double value, bool notify = true );
    void setAutoTurnSpeed( int value, bool notify = true );
    void setAutoTurnMode( int value, bool notify = true );
    void setVestibularMotionEnabled( bool value, bool notify = true );
    void setVestibularMotionRadius( double value, bool notify = true );

signals:

    void autoTurnEnabledChanged( bool value );
    void autoTurnActivationDistanceChanged( float value );
    void autoTurnDeactivationDistanceChanged( float value );
    void autoTurnUseCornerAngleChanged( bool value );
    void cordDetangleAngleChanged( double value );
    void minCordTangleChanged( double value );
    void autoTurnSpeedChanged( int value );
    void autoTurnModeChanged( int value );
    void vestibularMotionEnabledChanged( bool value );
    void vestibularMotionRadiusChanged( double value );
};

// Would be nice to do <typename T, T min, T max> but the standard doesn't allow
// for floating point non-types.
template <typename T> inline T reduceAngle( T angle, T min, T max )
{
    while ( angle >= max )
    {
        angle -= ( max - min );
    }
    while ( angle < min )
    {
        angle += ( max - min );
    }
    return angle;
}

// Convienience function for incrementing/decrementing an index in a circular
// buffer.
inline size_t circularIndex( const size_t idx,
                             const bool increment,
                             const size_t modulus ) noexcept
{
    return increment ? ( ( idx + 1 ) % modulus )
                     : ( ( idx == 0 ) ? ( modulus - 1 ) : ( idx - 1 ) );
}

} // namespace advsettings
