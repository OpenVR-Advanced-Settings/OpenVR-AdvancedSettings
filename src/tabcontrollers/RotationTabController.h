
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

struct RotationProfile : settings::ISettingsObject
{
    std::string profileName;

    // Settings
    bool autoTurnEnabled = true;
    float activationDistance
        = 0.4f; // TODO: Have some indicator in UI that lower values are
    // more walking space
    float deactivateDistance = 0.15f;
    bool autoTurnUseCornerAngle = true; // Turn the exact corner of the
                                        // angle, rather than the headset
    double cordDetanglingAngle = 1000 * k_centidegreesToRadians;
    double autoTurnMinCordTangle = 2 * M_PI;
    int autoTurnLinearTurnSpeed = 9000; // centidegrees/sec

    AutoTurnModes autoTurnMode = AutoTurnModes::LINEAR_SMOOTH_TURN;

    /*  22.0: generally considered imperceptable to humans
     *  11.0: light
     *  5.5:  strong
     *  1.0:  extreme
     */
    bool autoTurnVestibularMotionEnabled = false;
    double autoTurnVestibularMotionRadius = 11.0;

    virtual settings::SettingsObjectData saveSettings() const override
    {
        settings::SettingsObjectData o;
        o.addValue( autoTurnEnabled );
        o.addValue( static_cast<double>( activationDistance ) );
        o.addValue( static_cast<double>( deactivateDistance ) );
        o.addValue( autoTurnUseCornerAngle );
        o.addValue( cordDetanglingAngle );
        o.addValue( autoTurnMinCordTangle );
        o.addValue( autoTurnLinearTurnSpeed );
        o.addValue( static_cast<int>( autoTurnMode ) );
        o.addValue( autoTurnVestibularMotionEnabled );
        o.addValue( autoTurnVestibularMotionRadius );

        return o;
    }

    virtual void loadSettings( settings::SettingsObjectData& obj ) override
    {
        // Settings
        autoTurnEnabled = obj.getNextValueOrDefault( true );
        activationDistance
            = static_cast<float>( obj.getNextValueOrDefault( 0.4 ) );
        deactivateDistance
            = static_cast<float>( obj.getNextValueOrDefault( 0.15 ) );
        autoTurnUseCornerAngle = obj.getNextValueOrDefault( true );
        cordDetanglingAngle
            = obj.getNextValueOrDefault( 1000 * k_centidegreesToRadians );
        autoTurnMinCordTangle = obj.getNextValueOrDefault( 2 * M_PI );
        autoTurnLinearTurnSpeed = obj.getNextValueOrDefault( 9000 );
        autoTurnMode = static_cast<AutoTurnModes>( obj.getNextValueOrDefault(
            static_cast<int>( AutoTurnModes::LINEAR_SMOOTH_TURN ) ) );
        autoTurnVestibularMotionEnabled = obj.getNextValueOrDefault( false );
        autoTurnVestibularMotionRadius = obj.getNextValueOrDefault( 11.0 );
    }

    virtual std::string settingsName() const override
    {
        return "RotationTabController::RotationProfile";
    }
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

    void doAutoTurn(
        const vr::TrackedDevicePose_t& poseHmd,
        const std::vector<utils::ChaperoneQuadData>& chaperoneDistances );
    void doVestibularMotion(
        const vr::TrackedDevicePose_t& poseHmd,
        const std::vector<utils::ChaperoneQuadData>& chaperoneDistances );

public:
    void initStage1();
    void initStage2( OverlayController* parent );

    void eventLoopTick( vr::TrackedDevicePose_t* devicePoses );

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
