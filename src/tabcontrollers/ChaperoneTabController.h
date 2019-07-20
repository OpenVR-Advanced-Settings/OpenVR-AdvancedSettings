
#pragma once

#include <QObject>
#include <memory>
#include <chrono>
#include <thread>
#include <openvr.h>

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;

struct ChaperoneProfile
{
    std::string profileName;

    bool includesChaperoneGeometry = false;
    unsigned chaperoneGeometryQuadCount = 0;
    std::unique_ptr<vr::HmdQuad_t> chaperoneGeometryQuads = nullptr;
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
    bool enableChaperoneVelocityModifier = false;
    float chaperoneVelocityModifier = 0.0f;
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

    Q_PROPERTY( bool chaperoneVelocityModifierEnabled READ
                    isChaperoneVelocityModifierEnabled WRITE
                        setChaperoneVelocityModifierEnabled NOTIFY
                            chaperoneVelocityModifierEnabledChanged )
    Q_PROPERTY( float chaperoneVelocityModifier READ chaperoneVelocityModifier
                    WRITE setChaperoneVelocityModifier NOTIFY
                        chaperoneVelocityModifierChanged )

private:
    OverlayController* parent;

    float m_visibility = 0.6f;
    float m_fadeDistance = 0.7f;
    float m_fadeDistanceModified = 0.7f;
    float m_height = 2.0f;
    bool m_centerMarker = false;
    bool m_playSpaceMarker = false;
    bool m_forceBounds = false;
    bool m_disableChaperone = false;
    float m_fadeDistanceRemembered = 0.0f;

    bool m_enableChaperoneSwitchToBeginner = false;
    float m_chaperoneSwitchToBeginnerDistance = 0.5f;
    bool m_chaperoneSwitchToBeginnerActive = false;
    int32_t m_chaperoneSwitchToBeginnerLastStyle = 0;

    bool m_enableChaperoneHapticFeedback = true;
    float m_chaperoneHapticFeedbackDistance = 0.5f;
    bool m_chaperoneHapticFeedbackActive = false;
    std::thread m_chaperoneHapticFeedbackThread;

    bool m_enableChaperoneAlarmSound = false;
    bool m_chaperoneAlarmSoundLooping = true;
    bool m_chaperoneAlarmSoundAdjustVolume = false;
    float m_chaperoneAlarmSoundDistance = 0.5f;
    bool m_chaperoneAlarmSoundActive = false;

    bool m_enableChaperoneShowDashboard = false;
    float m_chaperoneShowDashboardDistance = 0.5f;
    bool m_chaperoneShowDashboardActive = false;

    bool m_enableChaperoneVelocityModifier = false;
    float m_chaperoneVelocityModifier = 0.0f;
    float m_chaperoneVelocityModifierCurrent = 1.0f;

    unsigned settingsUpdateCounter = 0;

    bool m_isHapticGood = true;
    bool m_isHMDActive = false;
    bool m_isProxActive = false;
    bool m_HMDHasProx = false;
    bool m_leftHapticClickActivated = false;
    bool m_rightHapticClickActivated = false;

    int m_updateTicksChaperoneReload = 0;

    vr::VRActionHandle_t m_rightActionHandle;
    vr::VRActionHandle_t m_leftActionHandle;
    vr::VRInputValueHandle_t m_leftInputHandle;
    vr::VRInputValueHandle_t m_rightInputHandle;

    std::vector<ChaperoneProfile> chaperoneProfiles;

public:
    ~ChaperoneTabController();

    void initStage1();
    void initStage2( OverlayController* parent );

    void eventLoopTick( vr::TrackedDevicePose_t* devicePoses,
                        float leftSpeed,
                        float rightSpeed,
                        float hmdSpeed );
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

    bool isChaperoneVelocityModifierEnabled() const;
    float chaperoneVelocityModifier() const;

    void reloadChaperoneProfiles();
    void saveChaperoneProfiles();

    Q_INVOKABLE unsigned getChaperoneProfileCount();
    Q_INVOKABLE QString getChaperoneProfileName( unsigned index );

    float getBoundsMaxY();

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

    void setChaperoneVelocityModifierEnabled( bool value, bool notify = true );
    void setChaperoneVelocityModifier( float value, bool notify = true );

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

    void chaperoneVelocityModifierEnabledChanged( bool value );
    void chaperoneVelocityModifierChanged( float value );

    void chaperoneProfilesUpdated();
};

} // namespace advsettings
