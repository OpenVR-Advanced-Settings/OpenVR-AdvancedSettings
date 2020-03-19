
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

struct ChaperoneProfile : settings::ISettingsObject
{
    std::string profileName;

    bool includesChaperoneGeometry = false;
    unsigned chaperoneGeometryQuadCount = 0;
    std::vector<vr::HmdQuad_t> chaperoneGeometryQuads;
    vr::HmdMatrix34_t standingCenter;
    float playSpaceAreaX = 0.0f;
    float playSpaceAreaZ = 0.0f;

    bool includesVisibility = false;
    float visibility = 0.6f;

    bool includesFadeDistance = false;
    float fadeDistance = 0.7f;

    bool includesCenterMarker = false;
    bool centerMarker = false;

    bool includesPlaySpaceMarker = false;
    bool playSpaceMarker = false;

    bool includesFloorBoundsMarker = false;
    bool floorBoundsMarker = false;

    bool includesBoundsColor = false;
    int boundsColor[3] = { 0, 0, 0 };

    bool includesChaperoneStyle = false;
    int chaperoneStyle = 0;

    bool includesForceBounds = false;
    bool forceBounds = false;

    bool includesProximityWarningSettings = false;
    bool enableChaperoneSwitchToBeginner = false;
    float chaperoneSwitchToBeginnerDistance = 0.0f;
    bool enableChaperoneHapticFeedback = false;
    float chaperoneHapticFeedbackDistance = 0.0f;
    bool enableChaperoneAlarmSound = false;
    bool chaperoneAlarmSoundLooping = true;
    bool chaperoneAlarmSoundAdjustVolume = false;
    float chaperoneAlarmSoundDistance = 0.0f;
    bool enableChaperoneShowDashboard = false;
    float chaperoneShowDashboardDistance = 0.0f;

    virtual settings::SettingsObjectData saveSettings() const override
    {
        settings::SettingsObjectData o;

        o.addValue( profileName );

        o.addValue( includesChaperoneGeometry );
        o.addValue( static_cast<int>( chaperoneGeometryQuadCount ) );

        if ( chaperoneGeometryQuadCount > 0 )
        {
            for ( auto& arrayMember : chaperoneGeometryQuads )
            {
                for ( auto& corner : arrayMember.vCorners )
                {
                    for ( auto& vector : corner.v )
                    {
                        o.addValue( static_cast<double>( vector ) );
                    }
                }
            }
        }

        for ( int i = 0; i < 3; ++i )
        {
            for ( int j = 0; j < 4; ++j )
            {
                o.addValue( static_cast<double>( standingCenter.m[i][j] ) );
            }
        }

        o.addValue( static_cast<double>( playSpaceAreaX ) );
        o.addValue( static_cast<double>( playSpaceAreaZ ) );

        o.addValue( includesVisibility );
        o.addValue( static_cast<double>( visibility ) );

        o.addValue( includesFadeDistance );
        o.addValue( static_cast<double>( fadeDistance ) );

        o.addValue( includesCenterMarker );
        o.addValue( centerMarker );

        o.addValue( includesPlaySpaceMarker );
        o.addValue( playSpaceMarker );

        o.addValue( includesFloorBoundsMarker );
        o.addValue( floorBoundsMarker );

        o.addValue( includesBoundsColor );

        for ( const auto& c : boundsColor )
        {
            o.addValue( c );
        }

        o.addValue( includesChaperoneStyle );
        o.addValue( chaperoneStyle );

        o.addValue( includesForceBounds );
        o.addValue( forceBounds );

        o.addValue( includesProximityWarningSettings );
        o.addValue( enableChaperoneSwitchToBeginner );
        o.addValue( static_cast<double>( chaperoneSwitchToBeginnerDistance ) );
        o.addValue( enableChaperoneHapticFeedback );
        o.addValue( static_cast<double>( chaperoneHapticFeedbackDistance ) );
        o.addValue( enableChaperoneAlarmSound );
        o.addValue( chaperoneAlarmSoundLooping );
        o.addValue( chaperoneAlarmSoundAdjustVolume );
        o.addValue( static_cast<double>( chaperoneAlarmSoundDistance ) );
        o.addValue( enableChaperoneShowDashboard );
        o.addValue( static_cast<double>( chaperoneShowDashboardDistance ) );

        return o;
    }

    virtual void loadSettings( settings::SettingsObjectData& obj ) override
    {
        profileName = obj.getNextValueOrDefault( "" );

        includesChaperoneGeometry = obj.getNextValueOrDefault( false );
        chaperoneGeometryQuadCount
            = static_cast<unsigned>( obj.getNextValueOrDefault( 0 ) );

        // chaperoneGeometryQuads = nullptr;
        const auto chaperoneGeometryQuadsValid
            = obj.getNextValueOrDefault( false );
        if ( chaperoneGeometryQuadsValid )
        {
            for ( int i = 0; i < static_cast<int>( chaperoneGeometryQuadCount );
                  ++i )
            {
                chaperoneGeometryQuads.emplace_back();
            }

            for ( auto& arrayMember : chaperoneGeometryQuads )
            {
                for ( auto& corner : arrayMember.vCorners )
                {
                    for ( auto& vector : corner.v )
                    {
                        vector = static_cast<float>(
                            obj.getNextValueOrDefault( 0.0 ) );
                    }
                }
            }
        }

        for ( int i = 0; i < 3; ++i )
        {
            for ( int j = 0; j < 4; ++j )
            {
                standingCenter.m[i][j]
                    = static_cast<float>( obj.getNextValueOrDefault( 0.0 ) );
            }
        }

        playSpaceAreaX = static_cast<float>( obj.getNextValueOrDefault( 0.0 ) );
        playSpaceAreaZ = static_cast<float>( obj.getNextValueOrDefault( 0.0 ) );

        includesVisibility = obj.getNextValueOrDefault( false );
        visibility = static_cast<float>( obj.getNextValueOrDefault( 0.6 ) );

        includesFadeDistance = obj.getNextValueOrDefault( false );
        fadeDistance = static_cast<float>( obj.getNextValueOrDefault( 0.7 ) );

        includesCenterMarker = obj.getNextValueOrDefault( false );
        centerMarker = obj.getNextValueOrDefault( false );

        includesPlaySpaceMarker = obj.getNextValueOrDefault( false );
        playSpaceMarker = obj.getNextValueOrDefault( false );

        includesFloorBoundsMarker = obj.getNextValueOrDefault( false );
        floorBoundsMarker = obj.getNextValueOrDefault( false );

        includesBoundsColor = obj.getNextValueOrDefault( false );

        for ( auto& c : boundsColor )
        {
            c = obj.getNextValueOrDefault( 0 );
        }

        includesChaperoneStyle = obj.getNextValueOrDefault( false );
        chaperoneStyle = obj.getNextValueOrDefault( 0 );

        includesForceBounds = obj.getNextValueOrDefault( false );
        forceBounds = obj.getNextValueOrDefault( false );

        includesProximityWarningSettings = obj.getNextValueOrDefault( false );
        enableChaperoneSwitchToBeginner = obj.getNextValueOrDefault( false );
        chaperoneSwitchToBeginnerDistance
            = static_cast<float>( obj.getNextValueOrDefault( 0.0 ) );
        enableChaperoneHapticFeedback = obj.getNextValueOrDefault( false );
        chaperoneHapticFeedbackDistance
            = static_cast<float>( obj.getNextValueOrDefault( 0.0 ) );
        enableChaperoneAlarmSound = obj.getNextValueOrDefault( false );
        chaperoneAlarmSoundLooping = obj.getNextValueOrDefault( true );
        chaperoneAlarmSoundAdjustVolume = obj.getNextValueOrDefault( false );
        chaperoneAlarmSoundDistance
            = static_cast<float>( obj.getNextValueOrDefault( 0.0 ) );
        enableChaperoneShowDashboard = obj.getNextValueOrDefault( false );
        chaperoneShowDashboardDistance
            = static_cast<float>( obj.getNextValueOrDefault( 0.0 ) );
    }

    virtual std::string settingsName() const override
    {
        return "ChaperoneTabController::ChaperoneProfile";
    }
};

class ChaperoneTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( float boundsVisibility READ boundsVisibility WRITE
                    setBoundsVisibility NOTIFY boundsVisibilityChanged )
    Q_PROPERTY( float fadeDistance READ fadeDistance WRITE setFadeDistance
                    NOTIFY fadeDistanceChanged )
    Q_PROPERTY( float height READ height WRITE setHeight NOTIFY heightChanged )
    Q_PROPERTY( bool centerMarker READ centerMarker WRITE setCenterMarker NOTIFY
                    centerMarkerChanged )
    Q_PROPERTY( bool playSpaceMarker READ playSpaceMarker WRITE
                    setPlaySpaceMarker NOTIFY playSpaceMarkerChanged )
    Q_PROPERTY( bool forceBounds READ forceBounds WRITE setForceBounds NOTIFY
                    forceBoundsChanged )
    Q_PROPERTY( bool disableChaperone READ disableChaperone WRITE
                    setDisableChaperone NOTIFY disableChaperoneChanged )

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
        bool chaperoneAlarmSoundEnabled READ isChaperoneAlarmSoundEnabled WRITE
            setChaperoneAlarmSoundEnabled NOTIFY
                chaperoneAlarmSoundEnabledChanged )
    Q_PROPERTY(
        bool chaperoneAlarmSoundLooping READ isChaperoneAlarmSoundLooping WRITE
            setChaperoneAlarmSoundLooping NOTIFY
                chaperoneAlarmSoundLoopingChanged )
    Q_PROPERTY( bool chaperoneAlarmSoundAdjustVolume READ
                    isChaperoneAlarmSoundAdjustVolume WRITE
                        setChaperoneAlarmSoundAdjustVolume NOTIFY
                            chaperoneAlarmSoundAdjustVolumeChanged )
    Q_PROPERTY(
        float chaperoneAlarmSoundDistance READ chaperoneAlarmSoundDistance WRITE
            setChaperoneAlarmSoundDistance NOTIFY
                chaperoneAlarmSoundDistanceChanged )

    Q_PROPERTY(
        bool chaperoneShowDashboardEnabled READ isChaperoneShowDashboardEnabled
            WRITE setChaperoneShowDashboardEnabled NOTIFY
                chaperoneShowDashboardEnabledChanged )
    Q_PROPERTY(
        float chaperoneShowDashboardDistance READ chaperoneShowDashboardDistance
            WRITE setChaperoneShowDashboardDistance NOTIFY
                chaperoneShowDashboardDistanceChanged )

private:
    OverlayController* parent;

    float m_visibility = 0.6f;
    float m_fadeDistance = 0.7f;
    float m_height = 2.0f;
    bool m_centerMarker = false;
    bool m_playSpaceMarker = false;
    bool m_forceBounds = false;

    float m_activationDistance
        = 0.4f; // TODO: Have some indicator in UI that lower values are
    // more walking space
    float m_deactivateDistance = 0.15f;
    double m_cordDetanglingAngle = 10000 * k_centidegreesToRadians;
    double m_autoturnMinCordTangle = 2 * M_PI;
    int m_autoturnLinearTurnSpeed = 9000; // centidegrees/sec
    enum class AutoturnModes
    {
        SNAP,
        LINEAR_SMOOTH_TURN,
        DELTA_X_LINEAR, // unimplemented
        DELTA_X_DYNAMIC, // unimplemented
    } m_autoturnMode
        = AutoturnModes::LINEAR_SMOOTH_TURN;
    int m_autoturnLinearSmoothTurnRemaining = 0;
    std::chrono::steady_clock::time_point m_autoturnLastUpdate;
    std::vector<bool> m_autoturnWallActive;

    /*  22.0: generally considered imperceptable to humans
     *  11.0: moderate
     *  5.5:  strong
     *  1.0:  extreme
     */
    bool m_autoturnVestibularMotionEnabled = true;
    double m_autoturnVestibularMotionRadius = 11.0;

    bool m_chaperoneSwitchToBeginnerActive = false;
    int32_t m_chaperoneSwitchToBeginnerLastStyle = 0;

    bool m_chaperoneHapticFeedbackActive = false;
    std::thread m_chaperoneHapticFeedbackThread;

    bool m_chaperoneAlarmSoundActive = false;

    bool m_chaperoneShowDashboardActive = false;

    unsigned settingsUpdateCounter = 0;
    void updateChaperoneSettings();

    bool m_isHapticGood = true;
    bool m_isHMDActive = false;
    bool m_isProxActive = false;
    bool m_HMDHasProx = false;
    bool m_leftHapticClickActivated = false;
    bool m_rightHapticClickActivated = false;

    bool m_autosaveComplete = false;

    int m_updateTicksChaperoneReload = 0;

    unsigned int m_chaperoneSettingsUpdateCounter = 101;

    vr::VRActionHandle_t m_rightActionHandle;
    vr::VRActionHandle_t m_leftActionHandle;
    vr::VRInputValueHandle_t m_leftInputHandle;
    vr::VRInputValueHandle_t m_rightInputHandle;

    std::vector<ChaperoneProfile> chaperoneProfiles;

public:
    ~ChaperoneTabController();

    void initStage1();
    void initStage2( OverlayController* parent );

    void eventLoopTick( vr::TrackedDevicePose_t* devicePoses );
    void handleChaperoneWarnings( float distance );

    float boundsVisibility() const;
    float fadeDistance() const;
    float height() const;
    bool centerMarker() const;
    bool playSpaceMarker() const;
    bool forceBounds() const;
    bool disableChaperone() const;

    void setRightHapticActionHandle( vr::VRActionHandle_t handle );
    void setLeftHapticActionHandle( vr::VRActionHandle_t handle );
    void setRightInputHandle( vr::VRInputValueHandle_t handle );
    void setLeftInputHandle( vr::VRInputValueHandle_t handle );

    void setProxState( bool value );

    bool isChaperoneSwitchToBeginnerEnabled() const;
    float chaperoneSwitchToBeginnerDistance() const;

    bool isChaperoneHapticFeedbackEnabled() const;
    float chaperoneHapticFeedbackDistance() const;

    bool isChaperoneAlarmSoundEnabled() const;
    bool isChaperoneAlarmSoundLooping() const;
    bool isChaperoneAlarmSoundAdjustVolume() const;
    float chaperoneAlarmSoundDistance() const;

    bool isChaperoneShowDashboardEnabled() const;
    float chaperoneShowDashboardDistance() const;

    void reloadChaperoneProfiles();
    void saveChaperoneProfiles();

    Q_INVOKABLE unsigned getChaperoneProfileCount();
    Q_INVOKABLE QString getChaperoneProfileName( unsigned index );

    std::pair<bool, unsigned>
        getChaperoneProfileIndexFromName( std::string name );
    void createNewAutosaveProfile();

    // actions
    void addLeftHapticClick( bool leftHapticClickPressed );
    void addRightHapticClick( bool rightHapticClickPressed );

public slots:
    void setBoundsVisibility( float value, bool notify = true );
    void setFadeDistance( float value, bool notify = true );
    void setHeight( float value, bool notify = true );
    void updateHeight( float value, bool notify = true );
    void setCenterMarker( bool value, bool notify = true );
    void setPlaySpaceMarker( bool value, bool notify = true );
    void setForceBounds( bool value, bool notify = true );
    void setDisableChaperone( bool value, bool notify = true );

    void setChaperoneSwitchToBeginnerEnabled( bool value, bool notify = true );
    void setChaperoneSwitchToBeginnerDistance( float value,
                                               bool notify = true );

    void setChaperoneHapticFeedbackEnabled( bool value, bool notify = true );
    void setChaperoneHapticFeedbackDistance( float value, bool notify = true );

    void setChaperoneAlarmSoundEnabled( bool value, bool notify = true );
    void setChaperoneAlarmSoundLooping( bool value, bool notify = true );
    void setChaperoneAlarmSoundAdjustVolume( bool value, bool notify = true );
    void setChaperoneAlarmSoundDistance( float value, bool notify = true );

    void setChaperoneShowDashboardEnabled( bool value, bool notify = true );
    void setChaperoneShowDashboardDistance( float value, bool notify = true );

    void flipOrientation( double degrees = 180 );
    void reloadFromDisk();

    void addChaperoneProfile( QString name,
                              bool includeGeometry,
                              bool includeVisbility,
                              bool includeFadeDistance,
                              bool includeCenterMarker,
                              bool includePlaySpaceMarker,
                              bool includeFloorBounds,
                              bool includeBoundsColor,
                              bool includeChaperoneStyle,
                              bool includeForceBounds,
                              bool includesProximityWarningSettings );
    void applyChaperoneProfile( unsigned index );
    void deleteChaperoneProfile( unsigned index );
    void applyAutosavedProfile();

    void reset();

    void shutdown();

signals:
    void boundsVisibilityChanged( float value );
    void fadeDistanceChanged( float value );
    void heightChanged( float value );
    void centerMarkerChanged( bool value );
    void playSpaceMarkerChanged( bool value );
    void forceBoundsChanged( bool value );
    void disableChaperoneChanged( bool value );

    void chaperoneSwitchToBeginnerEnabledChanged( bool value );
    void chaperoneSwitchToBeginnerDistanceChanged( float value );

    void chaperoneHapticFeedbackEnabledChanged( bool value );
    void chaperoneHapticFeedbackDistanceChanged( float value );

    void chaperoneAlarmSoundEnabledChanged( bool value );
    void chaperoneAlarmSoundLoopingChanged( bool value );
    void chaperoneAlarmSoundAdjustVolumeChanged( bool value );
    void chaperoneAlarmSoundDistanceChanged( float value );

    void chaperoneShowDashboardEnabledChanged( bool value );
    void chaperoneShowDashboardDistanceChanged( float value );

    void chaperoneProfilesUpdated();
};

} // namespace advsettings
