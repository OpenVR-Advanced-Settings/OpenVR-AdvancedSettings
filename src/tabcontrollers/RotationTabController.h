
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
        o.addValue( static_cast<double>( cordDetanglingAngle ) );
        o.addValue( static_cast<double>( autoturnMinCordTangle ) );
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
        autoturnMode = static_cast<AutoturnModes>obj.getNextValueOrDefault(AutoturnModes::LINEAR_SMOOTH_TURN));//TODO check
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
    /*Q_PROPERTY( float boundsVisibility READ boundsVisibility WRITE
                    setBoundsVisibility NOTIFY boundsVisibilityChanged )
    Q_PROPERTY( float fadeDistance READ fadeDistance WRITE setFadeDistance
                    NOTIFY fadeDistanceChanged )
    Q_PROPERTY( float height READ height WRITE setHeight NOTIFY
    heightChanged ) Q_PROPERTY( bool centerMarker READ centerMarker WRITE
    setCenterMarker NOTIFY centerMarkerChanged ) Q_PROPERTY( bool
    playSpaceMarker READ playSpaceMarker WRITE setPlaySpaceMarker NOTIFY
    playSpaceMarkerChanged ) Q_PROPERTY( bool forceBounds READ forceBounds
    WRITE setForceBounds NOTIFY forceBoundsChanged ) Q_PROPERTY( bool
    disableChaperone READ disableChaperone WRITE setDisableChaperone NOTIFY
    disableChaperoneChanged )

    Q_PROPERTY( bool chaperoneSwitchToBeginnerEnabled READ
                    isChaperoneSwitchToBeginnerEnabled WRITE
                        setChaperoneSwitchToBeginnerEnabled NOTIFY
                            chaperoneSwitchToBeginnerEnabledChanged )
    Q_PROPERTY( float chaperoneSwitchToBeginnerDistance READ
                    chaperoneSwitchToBeginnerDistance WRITE
                        setChaperoneSwitchToBeginnerDistance NOTIFY
                            chaperoneSwitchToBeginnerDistanceChanged )

    Q_PROPERTY( bool chaperoneHapticFeedbackEnabled READ
                    isChaperoneHapticFeedbackEnabled WRITE
                        setChaperoneHapticFeedbackEnabled NOTIFY
                            chaperoneHapticFeedbackEnabledChanged )
    Q_PROPERTY( float chaperoneHapticFeedbackDistance READ
                    chaperoneHapticFeedbackDistance WRITE
                        setChaperoneHapticFeedbackDistance NOTIFY
                            chaperoneHapticFeedbackDistanceChanged )

    Q_PROPERTY(
        bool chaperoneAlarmSoundEnabled READ isChaperoneAlarmSoundEnabled
    WRITE setChaperoneAlarmSoundEnabled NOTIFY
                chaperoneAlarmSoundEnabledChanged )
    Q_PROPERTY(
        bool chaperoneAlarmSoundLooping READ isChaperoneAlarmSoundLooping
    WRITE setChaperoneAlarmSoundLooping NOTIFY
                chaperoneAlarmSoundLoopingChanged )
    Q_PROPERTY( bool chaperoneAlarmSoundAdjustVolume READ
                    isChaperoneAlarmSoundAdjustVolume WRITE
                        setChaperoneAlarmSoundAdjustVolume NOTIFY
                            chaperoneAlarmSoundAdjustVolumeChanged )
    Q_PROPERTY(
        float chaperoneAlarmSoundDistance READ chaperoneAlarmSoundDistance
    WRITE setChaperoneAlarmSoundDistance NOTIFY
                chaperoneAlarmSoundDistanceChanged )

    Q_PROPERTY(
        bool chaperoneShowDashboardEnabled READ
    isChaperoneShowDashboardEnabled WRITE setChaperoneShowDashboardEnabled
    NOTIFY chaperoneShowDashboardEnabledChanged ) Q_PROPERTY( float
    chaperoneShowDashboardDistance READ chaperoneShowDashboardDistance WRITE
    setChaperoneShowDashboardDistance NOTIFY
                chaperoneShowDashboardDistanceChanged )
                */
    Q_PROPERTY( bool autoTurnEnabled READ autoTurnEnabled WRITE
                    setAutoTurnEnabled NOTIFY autoTurnEnabledChanged )
    Q_PROPERTY( float autoTurnActivationDistance READ autoTurnActivationDistance
                    WRITE setAutoTurnActivationDistance NOTIFY
                        autoTurnActivationDistanceChanged )
    Q_PROPERTY(
        float autoTurnDeactivationDistance READ autoTurnDeactivationDistance
            WRITE setAutoTurnDeactivationDistance NOTIFY
                autoTurnDeactivationDistanceChanged )
    Q_PROPERTY( double cordDetangleAngle READ cordDetangleAngle WRITE
                    setCordDetangleAngle NOTIFY cordDetangleAngleChanged )
    Q_PROPERTY( double minCordAngle READ minCordAngle WRITE setMinCordAngle
                    NOTIFY minCordAngleChanged )
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

    /*float m_visibility = 0.6f;
    float m_fadeDistance = 0.7f;
    float m_height = 2.0f;
    bool m_centerMarker = false;
    bool m_playSpaceMarker = false;
    bool m_forceBounds = false;
*/

    // Variables
    int m_autoturnLinearSmoothTurnRemaining = 0;
    std::chrono::steady_clock::time_point m_autoturnLastUpdate;
    std::vector<bool> m_autoturnWallActive;
    vr::HmdMatrix34_t m_autoturnLastHmdUpdate;
    std::vector<utils::ChaperoneQuadData> m_autoturnChaperoneDistancesLast;
    double m_autoturnRoundingError = 0.0;

    bool m_chaperoneSwitchToBeginnerActive = false;
    int32_t m_chaperoneSwitchToBeginnerLastStyle = 0;

    bool m_chaperoneHapticFeedbackActive = false;
    std::thread m_chaperoneHapticFeedbackThread;

    bool m_chaperoneAlarmSoundActive = false;

    bool m_chaperoneShowDashboardActive = false;

    unsigned settingsUpdateCounter = 0;
    void updateChaperoneSettings();

    bool m_isHMDActive = false;

    bool m_autosaveComplete = false;

    int m_updateTicksChaperoneReload = 0;

    unsigned int m_chaperoneSettingsUpdateCounter = 101;

    vr::VRActionHandle_t m_rightActionHandle;
    vr::VRActionHandle_t m_leftActionHandle;
    vr::VRInputValueHandle_t m_leftInputHandle;
    vr::VRInputValueHandle_t m_rightInputHandle;

    void doAutoTurn( vr::TrackedDevicePose_t poseHmd, float minDistance );

public:
    ~RotationTabController();

    void initStage1();
    void initStage2( OverlayController* parent );

    void eventLoopTick( vr::TrackedDevicePose_t* devicePoses );

    float boundsVisibility() const;

    bool autoTurnEnabled() const;
    float autoTurnActivationDistance() const;
    float autoTurnDeactivationDistance() const;
    double cordDetangleAngle() const;
    double minCordAngle() const;
    int autoTurnSpeed() const;
    AutoturnModes autoTurnMode() const;
    bool vestibularMotionEnabled() const;
    double vestibularMotionRadius() const;

    void setProxState( bool value );

    std::pair<bool, unsigned>
        getChaperoneProfileIndexFromName( std::string name );
    void createNewAutosaveProfile();

    // actions
    void addLeftHapticClick( bool leftHapticClickPressed );
    void addRightHapticClick( bool rightHapticClickPressed );

public slots:
    void setAutoTurnEnabled( bool value, bool notify = true );
    void setAutoTurnActivationDistance( float value, bool notify = true );
    void setAutoTurnDeactivationDistance( float value, bool notify = true );
    void setCordDetangleAngle( double value, bool notify = true );
    void minCordAngle( double value, bool notify = true );
    void setAutoTurnSpeed( int value, bool notify = true );
    void setAutoTurnMode( AutoturnModes value, bool notify = true );
    void setVestibularMotionEnabled( bool value, bool notify = true );
    void setVestibularMotionRadius( double value, bool notify = true );

    void reset();

    void shutdown();

signals:

    void autoTurnEnabledChanged( bool value );
    void autoTurnActivationDistanceChanged( float value );
    void autoTurnDeactivationDistanceChanged( float value );
    void cordDetangleAngleChanged( double value );
    void minCordAngleChanged( double value );
    void autoTurnSpeedChanged( int value );
    void autoTurnModeChanged( AutoturnModes value );
    void vestibularMotionEnabledChanged( bool value );
    void vestibularMotionRadiusChanged( double value );
};

} // namespace advsettings
