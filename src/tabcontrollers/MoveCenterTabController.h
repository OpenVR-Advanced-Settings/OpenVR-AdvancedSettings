
#pragma once

#include <QObject>
#include <openvr.h>
#include <chrono>
#include <qmath.h>

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
constexpr double k_centidegreesToRadians = M_PI / 18000.0;
constexpr double k_radiansToCentidegrees = 18000.0 / M_PI;
constexpr double k_quaternionInvalidValue = -1000.0;
constexpr double k_quaternionUnderIsInvalidValueThreshold = -900.0;
constexpr double k_terminalVelocity_mps = 50.0;

class OverlayController;

class MoveCenterTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( int trackingUniverse READ trackingUniverse WRITE
                    setTrackingUniverse NOTIFY trackingUniverseChanged )
    Q_PROPERTY(
        float offsetX READ offsetX WRITE setOffsetX NOTIFY offsetXChanged )
    Q_PROPERTY(
        float offsetY READ offsetY WRITE setOffsetY NOTIFY offsetYChanged )
    Q_PROPERTY(
        float offsetZ READ offsetZ WRITE setOffsetZ NOTIFY offsetZChanged )
    Q_PROPERTY(
        int rotation READ rotation WRITE setRotation NOTIFY rotationChanged )
    Q_PROPERTY( int tempRotation READ tempRotation WRITE setTempRotation NOTIFY
                    tempRotationChanged )
    Q_PROPERTY( int snapTurnAngle READ snapTurnAngle WRITE setSnapTurnAngle
                    NOTIFY snapTurnAngleChanged )
    Q_PROPERTY( bool adjustChaperone READ adjustChaperone WRITE
                    setAdjustChaperone NOTIFY adjustChaperoneChanged )
    Q_PROPERTY( bool moveShortcutRight READ moveShortcutRight WRITE
                    setMoveShortcutRight NOTIFY moveShortcutRightChanged )
    Q_PROPERTY( bool moveShortcutLeft READ moveShortcutLeft WRITE
                    setMoveShortcutLeft NOTIFY moveShortcutLeftChanged )
    Q_PROPERTY( bool turnBindLeft READ turnBindLeft WRITE setTurnBindLeft NOTIFY
                    turnBindLeftChanged )
    Q_PROPERTY( bool turnBindRight READ turnBindRight WRITE setTurnBindRight
                    NOTIFY turnBindRightChanged )
    Q_PROPERTY( unsigned dragComfortFactor READ dragComfortFactor WRITE
                    setDragComfortFactor NOTIFY dragComfortFactorChanged )
    Q_PROPERTY( unsigned turnComfortFactor READ turnComfortFactor WRITE
                    setTurnComfortFactor NOTIFY turnComfortFactorChanged )
    Q_PROPERTY( bool heightToggle READ heightToggle WRITE setHeightToggle NOTIFY
                    heightToggleChanged )
    Q_PROPERTY( float heightToggleOffset READ heightToggleOffset WRITE
                    setHeightToggleOffset NOTIFY heightToggleOffsetChanged )
    Q_PROPERTY( float gravityStrength READ gravityStrength WRITE
                    setGravityStrength NOTIFY gravityStrengthChanged )
    Q_PROPERTY( float flingStrength READ flingStrength WRITE setFlingStrength
                    NOTIFY flingStrengthChanged )
    Q_PROPERTY( bool gravityActive READ gravityActive WRITE setGravityActive
                    NOTIFY gravityActiveChanged )
    Q_PROPERTY( bool momentumSave READ momentumSave WRITE setMomentumSave NOTIFY
                    momentumSaveChanged )
    Q_PROPERTY( bool lockXToggle READ lockXToggle WRITE setLockX NOTIFY
                    requireLockXChanged )
    Q_PROPERTY( bool lockYToggle READ lockYToggle WRITE setLockY NOTIFY
                    requireLockYChanged )
    Q_PROPERTY( bool lockZToggle READ lockZToggle WRITE setLockZ NOTIFY
                    requireLockZChanged )

private:
    OverlayController* parent;
    QQuickWindow* widget;

    int m_trackingUniverse = static_cast<int>( vr::TrackingUniverseStanding );
    float m_offsetX = 0.0f;
    float m_offsetY = 0.0f;
    float m_offsetZ = 0.0f;
    float m_oldOffsetX = 0.0f;
    float m_oldOffsetY = 0.0f;
    float m_oldOffsetZ = 0.0f;
    int m_rotation = 0;
    int m_oldRotation = 0;
    int m_tempRotation = 0;
    int m_snapTurnAngle = 4500;
    bool m_adjustChaperone = true;
    bool m_moveShortcutRightPressed = false;
    bool m_moveShortcutLeftPressed = false;
    vr::TrackedDeviceIndex_t m_activeMoveController;
    float m_lastControllerPosition[3];
    bool m_settingsRightHandDragEnabled = false;
    bool m_settingsLeftHandDragEnabled = false;
    bool m_settingsLeftHandTurnEnabled = false;
    bool m_settingsRightHandTurnEnabled = false;
    unsigned m_dragComfortFactor = 0;
    unsigned m_turnComfortFactor = 0;
    bool m_heightToggle = false;
    float m_heightToggleOffset = -1.0f;
    float m_gravityFloor = 0.0f;
    float m_gravityStrength = 9.8f;
    float m_flingStrength = 1.0f;
    bool m_momentumSave = false;
    bool m_lockXToggle = false;
    bool m_lockYToggle = false;
    bool m_lockZToggle = false;
    // Set lastHandQuaternion.w to -1000.0 when last hand is invalid.
    vr::HmdQuaternion_t m_lastHandQuaternion
        = { k_quaternionInvalidValue, 0.0, 0.0, 0.0 };
    vr::HmdQuaternion_t m_handQuaternion;
    // Set lastHmdQuaternion.w to -1000.0 when last hmd pose is invalid.
    vr::HmdQuaternion_t m_lastHmdQuaternion
        = { k_quaternionInvalidValue, 0.0, 0.0, 0.0 };
    vr::HmdQuaternion_t m_hmdQuaternion;
    double m_hmdYawTotal = 0.0;
    vr::ETrackedControllerRole m_activeDragHand
        = vr::TrackedControllerRole_Invalid;
    vr::ETrackedControllerRole m_lastMoveHand
        = vr::TrackedControllerRole_Invalid;
    vr::ETrackedControllerRole m_activeTurnHand
        = vr::TrackedControllerRole_Invalid;
    vr::ETrackedControllerRole m_lastRotateHand
        = vr::TrackedControllerRole_Invalid;
    bool m_leftHandDragPressed = false;
    bool m_rightHandDragPressed = false;
    bool m_overrideLeftHandDragPressed = false;
    bool m_overrideRightHandDragPressed = false;
    bool m_leftHandTurnPressed = false;
    bool m_rightHandTurnPressed = false;
    bool m_overrideLeftHandTurnPressed = false;
    bool m_overrideRightHandTurnPressed = false;
    bool m_swapDragToLeftHandPressed = false;
    bool m_swapDragToRightHandPressed = false;
    bool m_swapDragToLeftHandActivated = false;
    bool m_swapDragToRightHandActivated = false;
    bool m_gravityActive = false;
    bool m_gravityReversed = false;
    bool m_chaperoneCommitted = true;
    bool m_pendingZeroOffsets = true;
    bool m_dashWasOpenPreviousFrame = false;
    bool m_isResetDataStandingUniverse = true;
    bool m_roomSetupModeDetected = false;
    unsigned settingsUpdateCounter = 0;
    int m_hmdRotationStatsUpdateCounter = 0;
    unsigned m_dragComfortFrameSkipCounter = 0;
    unsigned m_turnComfortFrameSkipCounter = 0;
    double m_velocity[3] = { 0.0, 0.0, 0.0 };
    std::chrono::steady_clock::time_point m_lastGravityUpdateTimePoint;
    std::chrono::steady_clock::time_point m_lastDragUpdateTimePoint;
    vr::HmdQuad_t* m_collisionBoundsForReset;
    uint32_t m_collisionBoundsCountForReset = 0;
    vr::HmdMatrix34_t m_universeCenterForReset;
    vr::HmdQuad_t* m_collisionBoundsForOffset;

    void updateHmdRotationCounter( vr::TrackedDevicePose_t hmdPose,
                                   double angle );
    void updateHandDrag( vr::TrackedDevicePose_t* devicePoses, double angle );
    void updateHandTurn( vr::TrackedDevicePose_t* devicePoses, double angle );
    void updateGravity();
    void updateSpace();
    void clampVelocity( double* velocity );
    void updateChaperoneResetData();
    void applyChaperoneResetData();
    void saveUncommittedChaperone();

public:
    void initStage1();
    void initStage2( OverlayController* parent, QQuickWindow* widget );

    void eventLoopTick( vr::ETrackingUniverseOrigin universe,
                        vr::TrackedDevicePose_t* devicePoses );

    float offsetX() const;
    float offsetY() const;
    float offsetZ() const;
    int rotation() const;
    int tempRotation() const;
    int snapTurnAngle() const;
    bool adjustChaperone() const;
    bool moveShortcutRight() const;
    bool moveShortcutLeft() const;
    bool turnBindRight() const;
    bool turnBindLeft() const;
    unsigned dragComfortFactor() const;
    unsigned turnComfortFactor() const;
    bool heightToggle() const;
    float heightToggleOffset() const;
    float gravityStrength() const;
    float flingStrength() const;
    bool gravityActive() const;
    bool momentumSave() const;
    bool lockXToggle() const;
    bool lockYToggle() const;
    bool lockZToggle() const;
    double getHmdYawTotal();
    void resetHmdYawTotal();

    // actions:
    void leftHandSpaceDrag( bool leftHandDragActive );
    void rightHandSpaceDrag( bool rightHandDragActive );
    void optionalOverrideLeftHandSpaceDrag( bool overrideLeftHandDragActive );
    void optionalOverrideRightHandSpaceDrag( bool overrideRightHandDragActive );
    void leftHandSpaceTurn( bool leftHandTurnActive );
    void rightHandSpaceTurn( bool rightHandTurnActive );
    void optionalOverrideLeftHandSpaceTurn( bool overrideLeftHandTurnActive );
    void optionalOverrideRightHandSpaceTurn( bool overrideRightHandTurnActive );
    void swapSpaceDragToLeftHandOverride( bool swapDragToLeftHandActive );
    void swapSpaceDragToRightHandOverride( bool swapDragToRightHandActive );
    void gravityToggleAction( bool gravityToggleJustPressed );
    void gravityReverseAction( bool gravityReverseHeld );
    void heightToggleAction( bool heightToggleJustPressed );
    void resetOffsets( bool resetOffsetsJustPressed );
    void snapTurnLeft( bool snapTurnLeftJustPressed );
    void snapTurnRight( bool snapTurnRightJustPressed );
    void xAxisLockToggle( bool xAxisLockToggleJustPressed );
    void yAxisLockToggle( bool yAxisLockToggleJustPressed );
    void zAxisLockToggle( bool zAxisLockToggleJustPressed );

public slots:
    int trackingUniverse() const;
    void setTrackingUniverse( int value, bool notify = true );

    void setOffsetX( float value, bool notify = true );
    void setOffsetY( float value, bool notify = true );
    void setOffsetZ( float value, bool notify = true );

    void setRotation( int value, bool notify = true );
    void setTempRotation( int value, bool notify = true );
    void setSnapTurnAngle( int value, bool notify = true );

    void setAdjustChaperone( bool value, bool notify = true );
    void setMoveShortcutRight( bool value, bool notify = true );
    void setMoveShortcutLeft( bool value, bool notify = true );
    void setTurnBindRight( bool value, bool notify = true );
    void setTurnBindLeft( bool value, bool notify = true );
    void setDragComfortFactor( unsigned value, bool notify = true );
    void setTurnComfortFactor( unsigned value, bool notify = true );
    void setHeightToggle( bool value, bool notify = true );
    void setHeightToggleOffset( float value, bool notify = true );
    void setGravityStrength( float value, bool notify = true );
    void setFlingStrength( float value, bool notify = true );
    void setGravityActive( bool value, bool notify = true );
    void setMomentumSave( bool value, bool notify = true );

    void modOffsetX( float value, bool notify = true );
    void modOffsetY( float value, bool notify = true );
    void modOffsetZ( float value, bool notify = true );

    void setLockX( bool value, bool notify = true );
    void setLockY( bool value, bool notify = true );
    void setLockZ( bool value, bool notify = true );

    void shutdown();
    void reset();
    void zeroOffsets();

signals:
    void trackingUniverseChanged( int value );
    void offsetXChanged( float value );
    void offsetYChanged( float value );
    void offsetZChanged( float value );
    void rotationChanged( int value );
    void tempRotationChanged( int value );
    void snapTurnAngleChanged( int value );
    void adjustChaperoneChanged( bool value );
    void moveShortcutRightChanged( bool value );
    void moveShortcutLeftChanged( bool value );
    void turnBindRightChanged( bool value );
    void turnBindLeftChanged( bool value );
    void dragComfortFactorChanged( unsigned value );
    void turnComfortFactorChanged( unsigned value );
    void heightToggleChanged( bool value );
    void heightToggleOffsetChanged( float value );
    void gravityStrengthChanged( float value );
    void flingStrengthChanged( float value );
    void gravityActiveChanged( bool value );
    void momentumSaveChanged( bool value );
    void requireLockXChanged( bool value );
    void requireLockYChanged( bool value );
    void requireLockZChanged( bool value );
};

} // namespace advsettings
