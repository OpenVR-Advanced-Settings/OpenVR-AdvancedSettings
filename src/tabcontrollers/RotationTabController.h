#pragma once

#include <QObject>
#include <QImage>
#include <memory>
#include <chrono>
#include <openvr.h>
#include <optional>
#include "../utils/ChaperoneUtils.h"
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

namespace FrameRates
{
    // Each of type std::chrono::time_point::duration
    constexpr auto RATE_45HZ = std::chrono::duration_cast<
        std::chrono::steady_clock::time_point::duration>(
        std::chrono::duration<int, std::ratio<1, 45>>( 1 ) );
    constexpr auto RATE_60HZ = std::chrono::duration_cast<
        std::chrono::steady_clock::time_point::duration>(
        std::chrono::duration<int, std::ratio<1, 60>>( 1 ) );
    constexpr auto RATE_72HZ = std::chrono::duration_cast<
        std::chrono::steady_clock::time_point::duration>(
        std::chrono::duration<int, std::ratio<1, 72>>( 1 ) );
    constexpr auto RATE_90HZ = std::chrono::duration_cast<
        std::chrono::steady_clock::time_point::duration>(
        std::chrono::duration<int, std::ratio<1, 90>>( 1 ) );
    constexpr auto RATE_120HZ = std::chrono::duration_cast<
        std::chrono::steady_clock::time_point::duration>(
        std::chrono::duration<int, std::ratio<1, 120>>( 1 ) );
    constexpr auto RATE_144HZ = std::chrono::duration_cast<
        std::chrono::steady_clock::time_point::duration>(
        std::chrono::duration<int, std::ratio<1, 144>>( 1 ) );

    template <typename T> double toDoubleSeconds( T duration )
    {
        return ( static_cast<double>( duration.count() ) * T::period::num )
               / T::period::den;
    }
} // namespace FrameRates

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
    Q_PROPERTY(
        bool viewRatchettingEnabled READ viewRatchettingEnabled WRITE
            setViewRatchettingEnabled NOTIFY viewRatchettingEnabledChanged )
    Q_PROPERTY(
        double viewRatchettingPercent READ viewRatchettingPercent WRITE
            setViewRatchettingPercent NOTIFY viewRatchettingPercentChanged )
    Q_PROPERTY(
        bool autoTurnShowNotification READ autoTurnShowNotification WRITE
            setAutoTurnShowNotification NOTIFY autoTurnShowNotificationChanged )

private:
    OverlayController* parent;

    vr::VROverlayHandle_t m_overlayHandle = vr::k_ulOverlayHandleInvalid;
    std::unique_ptr<QImage> m_autoturnImg;
    std::unique_ptr<QImage> m_noautoturnImg;

    virtual vr::VROverlayHandle_t getNotificationOverlayHandle()
    {
        return m_overlayHandle;
    }

    // Variables
    int m_autoTurnLinearSmoothTurnRemaining = 0;
    std::chrono::steady_clock::time_point m_autoTurnLastUpdate;
    std::vector<bool> m_autoTurnWallActive;
    vr::HmdMatrix34_t m_autoTurnLastHmdUpdate;
    std::vector<utils::ChaperoneQuadData> m_autoTurnChaperoneDistancesLast;
    std::chrono::steady_clock::time_point::duration m_estimatedFrameRate;
    double m_ratchettingLastHmdRotation = 0.0;
    size_t m_ratchettingLastWall = 0;

    std::optional<std::chrono::steady_clock::time_point>
        m_autoTurnNotificationTimestamp;

    bool m_isHMDActive = false;

    void doAutoTurn(
        const vr::TrackedDevicePose_t& poseHmd,
        const std::vector<utils::ChaperoneQuadData>& chaperoneDistances );
    void doVestibularMotion(
        const vr::TrackedDevicePose_t& poseHmd,
        const std::vector<utils::ChaperoneQuadData>& chaperoneDistances );
    void doViewRatchetting(
        const vr::TrackedDevicePose_t& poseHmd,
        const std::vector<utils::ChaperoneQuadData>& chaperoneDistances );

public:
    void initStage1();
    void initStage2( OverlayController* parent );

    void eventLoopTick( vr::TrackedDevicePose_t* devicePoses );

    [[nodiscard]] float boundsVisibility() const;

    [[nodiscard]] bool autoTurnEnabled() const;
    [[nodiscard]] float autoTurnActivationDistance() const;
    [[nodiscard]] float autoTurnDeactivationDistance() const;
    [[nodiscard]] bool autoTurnUseCornerAngle() const;
    [[nodiscard]] bool autoTurnShowNotification() const;
    [[nodiscard]] double cordDetangleAngle() const;
    [[nodiscard]] double minCordTangle() const;
    [[nodiscard]] int autoTurnSpeed() const;
    [[nodiscard]] AutoTurnModes autoTurnModeType() const;
    [[nodiscard]] int autoTurnMode() const;
    [[nodiscard]] bool vestibularMotionEnabled() const;
    [[nodiscard]] double vestibularMotionRadius() const;
    [[nodiscard]] bool viewRatchettingEnabled() const;
    [[nodiscard]] double viewRatchettingPercent() const;

    void setAutoTurnEnabled( bool value, bool notify = true );
    void setAutoTurnShowNotification( bool value, bool notify = true );
    void setAutoTurnActivationDistance( float value, bool notify = true );
    void setAutoTurnDeactivationDistance( float value, bool notify = true );
    void setAutoTurnUseCornerAngle( bool value, bool notify = true );
    void setCordDetangleAngle( double value, bool notify = true );
    void setMinCordTangle( double value, bool notify = true );
    void setAutoTurnSpeed( int value, bool notify = true );
    void setAutoTurnMode( int value, bool notify = true );
    void setVestibularMotionEnabled( bool value, bool notify = true );
    void setVestibularMotionRadius( double value, bool notify = true );
    void setViewRatchettingEnabled( bool value, bool notify = true );
    void setViewRatchettingPercent( double value, bool notify = true );

signals:

    void defaultProfileDisplay();
    void autoTurnEnabledChanged( bool value );
    void autoTurnShowNotificationChanged( bool value );
    void autoTurnActivationDistanceChanged( float value );
    void autoTurnDeactivationDistanceChanged( float value );
    void autoTurnUseCornerAngleChanged( bool value );
    void cordDetangleAngleChanged( double value );
    void minCordTangleChanged( double value );
    void autoTurnSpeedChanged( int value );
    void autoTurnModeChanged( int value );
    void vestibularMotionEnabledChanged( bool value );
    void vestibularMotionRadiusChanged( double value );
    void viewRatchettingEnabledChanged( bool value );
    void viewRatchettingPercentChanged( double value );
};

// Would be nice to do <typename T, T min, T max> but the standard doesn't allow
// for floating point non-types.
// XXX: clang-tidy on github actions needs this
// NOLINTNEXTLINE(altera-id-dependent-backward-branch)
template <typename T> inline T reduceAngle( T angle, T min, T max )
{
    // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
    while ( angle >= max )
    {
        angle -= ( max - min );
    }
    // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
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
