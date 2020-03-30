
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

enum class AutoturnModes
{
    SNAP,
    LINEAR_SMOOTH_TURN,
    DELTA_X_LINEAR, // unimplemented
    DELTA_X_DYNAMIC, // unimplemented
};

struct RotationProfile : settings::ISettingsObject
{
    std::string profileName;

    // Settings
    bool autoturnEnabled = true;
    float activationDistance
        = 0.4f; // TODO: Have some indicator in UI that lower values are
    // more walking space
    float deactivateDistance = 0.15f;
    bool autoturnUseCornerAngle = true; // Turn the exact corner of the
                                        // angle, rather than the headset
    double cordDetanglingAngle = 1000 * k_centidegreesToRadians;
    double autoturnMinCordTangle = 2 * M_PI;
    int autoturnLinearTurnSpeed = 9000; // centidegrees/sec

    AutoturnModes autoturnMode = AutoturnModes::LINEAR_SMOOTH_TURN;

    /*  22.0: generally considered imperceptable to humans
     *  11.0: light
     *  5.5:  strong
     *  1.0:  extreme
     */
    bool autoturnVestibularMotionEnabled = false;
    double autoturnVestibularMotionRadius = 11.0;

    virtual settings::SettingsObjectData saveSettings() const override
    {
        settings::SettingsObjectData o;
        o.addValue( autoturnEnabled );
        o.addValue( static_cast<double>( activationDistance ) );
        o.addValue( static_cast<double>( deactivateDistance ) );
        o.addValue( autoturnUseCornerAngle );
        o.addValue( cordDetanglingAngle );
        o.addValue( autoturnMinCordTangle );
        o.addValue( autoturnLinearTurnSpeed ); // CHECK if double/int?
        o.addValue( static_cast<int>( autoturnMode ) );
        o.addValue( autoturnVestibularMotionEnabled );
        o.addValue( autoturnVestibularMotionRadius );

        return o;
    }

    virtual void loadSettings( settings::SettingsObjectData& obj ) override
    {
        // supersampling = static_cast<float>( obj.getNextValueOrDefault( 1.0 )
        // );

        // Settings
        autoturnEnabled = obj.getNextValueOrDefault( true );
        activationDistance
            = static_cast<float>( obj.getNextValueOrDefault( 0.4 ) );
        deactivateDistance
            = static_cast<float>( obj.getNextValueOrDefault( 0.15 ) );
        autoturnUseCornerAngle = obj.getNextValueOrDefault( true );
        cordDetanglingAngle
            = obj.getNextValueOrDefault( 1000 * k_centidegreesToRadians );
        autoturnMinCordTangle = obj.getNextValueOrDefault( 2 * M_PI );
        autoturnLinearTurnSpeed
            = obj.getNextValueOrDefault( 9000 ); // TODO check if Int okay?
        autoturnMode = static_cast<AutoturnModes>(
            obj.getNextValueOrDefault( static_cast<int>(
                AutoturnModes::LINEAR_SMOOTH_TURN ) ) ); // TODO check
        autoturnVestibularMotionEnabled = obj.getNextValueOrDefault( false );
        autoturnVestibularMotionRadius = obj.getNextValueOrDefault( 11.0 );
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
    Q_PROPERTY( AutoturnModes autoTurnMode READ autoTurnMode WRITE
                    setAutoTurnMode NOTIFY autoTurnModeChanged )
    Q_PROPERTY(
        bool vestibularMotionEnabled READ vestibularMotionEnabled WRITE
            setVestibularMotionEnabled NOTIFY vestibularMotionEnabledChanged )
    Q_PROPERTY(
        double vestibularMotionRadius READ vestibularMotionRadius WRITE
            setVestibularMotionRadius NOTIFY vestibularMotionRadiusChanged )

private:
    OverlayController* parent;

    // Variables
    int m_autoturnLinearSmoothTurnRemaining = 0;
    std::chrono::steady_clock::time_point m_autoturnLastUpdate;
    std::vector<bool> m_autoturnWallActive;
    vr::HmdMatrix34_t m_autoturnLastHmdUpdate;
    std::vector<utils::ChaperoneQuadData> m_autoturnChaperoneDistancesLast;
    double m_autoturnRoundingError = 0.0;

    bool m_isHMDActive = false;

    void doAutoTurn( vr::TrackedDevicePose_t poseHmd, float minDistance );

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
    AutoturnModes autoTurnMode() const;
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
    void setAutoTurnMode( AutoturnModes value, bool notify = true );
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
    void autoTurnModeChanged( AutoturnModes value );
    void vestibularMotionEnabledChanged( bool value );
    void vestibularMotionRadiusChanged( double value );
};

} // namespace advsettings
