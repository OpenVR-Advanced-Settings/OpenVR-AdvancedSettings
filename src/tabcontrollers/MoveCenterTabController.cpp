#include "MoveCenterTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"
#include "../utils/Matrix.h"
#include "../quaternion/quaternion.h"

void rotateCoordinates( double coordinates[3], double angle )
{
    if ( angle == 0 )
    {
        return;
    }
    double s = sin( angle );
    double c = cos( angle );
    double newX = coordinates[0] * c - coordinates[2] * s;
    double newZ = coordinates[0] * s + coordinates[2] * c;
    coordinates[0] = newX;
    coordinates[2] = newZ;
}

void rotateFloatCoordinates( float coordinates[3], float angle )
{
    if ( angle == 0 )
    {
        return;
    }
    // crazy casts for compiler warnings
    float s = static_cast<float>( sin( static_cast<double>( angle ) ) );
    float c = static_cast<float>( cos( static_cast<double>( angle ) ) );
    float newX = coordinates[0] * c - coordinates[2] * s;
    float newZ = coordinates[0] * s + coordinates[2] * c;
    coordinates[0] = newX;
    coordinates[2] = newZ;
}

// application namespace
namespace advsettings
{
void MoveCenterTabController::initStage1()
{
    setTrackingUniverse( vr::VRCompositor()->GetTrackingSpace() );
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    auto value = settings->value( "adjustChaperone", m_adjustChaperone );
    if ( value.isValid() && !value.isNull() )
    {
        m_adjustChaperone = value.toBool();
    }
    value = settings->value( "moveShortcutRight",
                             m_settingsRightHandDragEnabled );
    if ( value.isValid() && !value.isNull() )
    {
        m_settingsRightHandDragEnabled = value.toBool();
    }
    value
        = settings->value( "moveShortcutLeft", m_settingsLeftHandDragEnabled );
    if ( value.isValid() && !value.isNull() )
    {
        m_settingsLeftHandDragEnabled = value.toBool();
    }
    value = settings->value( "turnBindLeft", m_settingsLeftHandTurnEnabled );
    if ( value.isValid() && !value.isNull() )
    {
        m_settingsLeftHandTurnEnabled = value.toBool();
    }
    value = settings->value( "turnBindRight", m_settingsRightHandTurnEnabled );
    if ( value.isValid() && !value.isNull() )
    {
        m_settingsRightHandTurnEnabled = value.toBool();
    }
    value = settings->value( "dragBounds", m_dragBounds );
    if ( value.isValid() && !value.isNull() )
    {
        m_dragBounds = value.toBool();
    }
    value = settings->value( "turnBounds", m_turnBounds );
    if ( value.isValid() && !value.isNull() )
    {
        m_turnBounds = value.toBool();
    }
    value = settings->value( "dragComfortFactor", m_dragComfortFactor );
    if ( value.isValid() && !value.isNull() )
    {
        m_dragComfortFactor = value.toUInt();
    }
    value = settings->value( "turnComfortFactor", m_turnComfortFactor );
    if ( value.isValid() && !value.isNull() )
    {
        m_turnComfortFactor = value.toUInt();
    }
    value = settings->value( "snapTurnAngle", m_snapTurnAngle );
    if ( value.isValid() && !value.isNull() )
    {
        m_snapTurnAngle = value.toInt();
    }
    value = settings->value( "smoothTurnRate", m_smoothTurnRate );
    if ( value.isValid() && !value.isNull() )
    {
        m_smoothTurnRate = value.toInt();
    }
    value = settings->value( "heightToggleOffset", m_heightToggleOffset );
    if ( value.isValid() && !value.isNull() )
    {
        m_heightToggleOffset = value.toFloat();
    }
    value = settings->value( "gravityStrength", m_gravityStrength );
    if ( value.isValid() && !value.isNull() )
    {
        m_gravityStrength = value.toFloat();
    }
    value = settings->value( "flingStrength", m_flingStrength );
    if ( value.isValid() && !value.isNull() )
    {
        m_flingStrength = value.toFloat();
    }
    value = settings->value( "momentumSave", m_momentumSave );
    if ( value.isValid() && !value.isNull() )
    {
        m_momentumSave = value.toBool();
    }
    value = settings->value( "lockXToggle", m_lockXToggle );
    if ( value.isValid() && !value.isNull() )
    {
        m_lockXToggle = value.toBool();
    }
    value = settings->value( "lockYToggle", m_lockYToggle );
    if ( value.isValid() && !value.isNull() )
    {
        m_lockYToggle = value.toBool();
    }
    value = settings->value( "lockZToggle", m_lockZToggle );
    if ( value.isValid() && !value.isNull() )
    {
        m_lockZToggle = value.toBool();
    }
    value = settings->value( "showLogMatricesButton", m_showLogMatricesButton );
    if ( value.isValid() && !value.isNull() )
    {
        m_showLogMatricesButton = value.toBool();
    }
    value = settings->value( "allowExternalEdits", m_allowExternalEdits );
    if ( value.isValid() && !value.isNull() )
    {
        m_allowExternalEdits = value.toBool();
    }
    value = settings->value( "oldStyleMotion", m_oldStyleMotion );
    if ( value.isValid() && !value.isNull() )
    {
        m_oldStyleMotion = value.toBool();
    }
    value = settings->value( "universeCenteredRotation",
                             m_universeCenteredRotation );
    if ( value.isValid() && !value.isNull() )
    {
        m_universeCenteredRotation = value.toBool();
    }
    settings->endGroup();
    m_lastDragUpdateTimePoint = std::chrono::steady_clock::now();
    m_lastGravityUpdateTimePoint = std::chrono::steady_clock::now();
}

void MoveCenterTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
    zeroOffsets();
    outputLogSettings();
}

void MoveCenterTabController::outputLogSettings()
{
    if ( !m_adjustChaperone )
    {
        LOG( WARNING ) << "LOADED SETTINGS: Adjust Chaperone DISABLED";
    }
    if ( m_settingsRightHandDragEnabled )
    {
        LOG( INFO ) << "LOADED SETTINGS: Right Hand Space-Drag Bind Enabled";
    }
    if ( m_settingsLeftHandDragEnabled )
    {
        LOG( INFO ) << "LOADED SETTINGS: Left Hand Space-Drag Bind Enabled";
    }
    if ( m_settingsRightHandTurnEnabled )
    {
        LOG( INFO ) << "LOADED SETTINGS: Right Hand Space-Turn Bind Enabled";
    }
    if ( m_settingsLeftHandTurnEnabled )
    {
        LOG( INFO ) << "LOADED SETTINGS: Left Hand Space-Turn Bind Enabled";
    }
    if ( m_dragComfortFactor > 0 )
    {
        LOG( INFO ) << "LOADED SETTINGS: Space-Drag Comfort Factor = "
                    << m_dragComfortFactor;
    }
    if ( m_turnComfortFactor > 0 )
    {
        LOG( INFO ) << "LOADED SETTINGS: Space-Turn Comfort Factor = "
                    << m_turnComfortFactor;
    }
    if ( m_lockXToggle )
    {
        LOG( INFO ) << "LOADED SETTINGS: X Axis Locked";
    }
    if ( m_lockYToggle )
    {
        LOG( INFO ) << "LOADED SETTINGS: Y Axis Locked";
    }
    if ( m_lockZToggle )
    {
        LOG( INFO ) << "LOADED SETTINGS: Z Axis Locked";
    }
    if ( m_allowExternalEdits )
    {
        LOG( INFO ) << "LOADED SETTINGS: Allow External Edits Enabled";
    }
    if ( m_oldStyleMotion )
    {
        LOG( INFO ) << "LOADED SETTINGS: Old-Style Motion Enabled";
    }
    if ( m_universeCenteredRotation )
    {
        LOG( INFO ) << "LOADED SETTINGS: Universe-Centered Rotation Enabled";
    }
    if ( m_dragBounds )
    {
        LOG( INFO ) << "LOADED SETTINGS: Space Drag Force Bounds Enabled";
    }
    if ( m_turnBounds )
    {
        LOG( INFO ) << "LOADED SETTINGS: Space Turn Force Bounds Enabled";
    }
}

void MoveCenterTabController::outputLogPoses()
{
    vr::TrackedDevicePose_t devicePosesStanding[vr::k_unMaxTrackedDeviceCount];

    vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(
        vr::TrackingUniverseStanding,
        0.0f,
        devicePosesStanding,
        vr::k_unMaxTrackedDeviceCount );

    vr::HmdMatrix34_t hmdStanding
        = devicePosesStanding[0].mDeviceToAbsoluteTracking;

    LOG( INFO ) << "HMD POSE (standing universe)";
    outputLogHmdMatrix( hmdStanding );

    vr::TrackedDevicePose_t devicePosesSeated[vr::k_unMaxTrackedDeviceCount];

    vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(
        vr::TrackingUniverseSeated,
        0.0f,
        devicePosesSeated,
        vr::k_unMaxTrackedDeviceCount );

    vr::HmdMatrix34_t hmdSeated
        = devicePosesSeated[0].mDeviceToAbsoluteTracking;

    LOG( INFO ) << "HMD POSE (seated universe)";
    outputLogHmdMatrix( hmdSeated );

    auto leftHand = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
        vr::TrackedControllerRole_LeftHand );
    auto rightHand = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
        vr::TrackedControllerRole_RightHand );

    vr::HmdMatrix34_t leftHandMatrix
        = devicePosesStanding[leftHand].mDeviceToAbsoluteTracking;
    vr::HmdMatrix34_t rightHandMatrix
        = devicePosesStanding[rightHand].mDeviceToAbsoluteTracking;

    LOG( INFO ) << "LEFT Hand Controller Pose (standing universe)";
    outputLogHmdMatrix( leftHandMatrix );

    LOG( INFO ) << "RIGHT Hand Controller Pose (standing universe)";
    outputLogHmdMatrix( rightHandMatrix );

    vr::HmdMatrix34_t standingZero;
    vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(
        &standingZero );
    LOG( INFO ) << "GetWorkingStandingZeroPoseToRawTrackingPose";
    outputLogHmdMatrix( standingZero );

    vr::HmdMatrix34_t seatedZero;
    vr::VRChaperoneSetup()->GetWorkingSeatedZeroPoseToRawTrackingPose(
        &seatedZero );
    LOG( INFO ) << "GetWorkingSeatedZeroPoseToRawTrackingPose";
    outputLogHmdMatrix( seatedZero );

    vr::HmdMatrix34_t seatedZeroLive;
    vr::VRChaperoneSetup()->GetLiveSeatedZeroPoseToRawTrackingPose(
        &seatedZeroLive );
    LOG( INFO ) << "GetLiveSeatedZeroPoseToRawTrackingPose";
    outputLogHmdMatrix( seatedZeroLive );

    vr::HmdMatrix34_t seatedToStandingAbsolute
        = vr::VRSystem()->GetSeatedZeroPoseToStandingAbsoluteTrackingPose();
    LOG( INFO ) << "GetSeatedZeroPoseToStandingAbsoluteTrackingPose";
    outputLogHmdMatrix( seatedToStandingAbsolute );

    LOG( INFO ) << "m_universeCenterForReset";
    outputLogHmdMatrix( m_universeCenterForReset );

    LOG( INFO ) << "m_seatedCenterForReset";
    outputLogHmdMatrix( m_seatedCenterForReset );
}

void MoveCenterTabController::outputLogHmdMatrix( vr::HmdMatrix34_t hmdMatrix )
{
    LOG( INFO ) << hmdMatrix.m[0][0] << "\t\t\t" << hmdMatrix.m[0][1]
                << "\t\t\t" << hmdMatrix.m[0][2] << "\t\t\t"
                << hmdMatrix.m[0][3];
    LOG( INFO ) << hmdMatrix.m[1][0] << "\t\t\t" << hmdMatrix.m[1][1]
                << "\t\t\t" << hmdMatrix.m[1][2] << "\t\t\t"
                << hmdMatrix.m[1][3];
    LOG( INFO ) << hmdMatrix.m[2][0] << "\t\t\t" << hmdMatrix.m[2][1]
                << "\t\t\t" << hmdMatrix.m[2][2] << "\t\t\t"
                << hmdMatrix.m[2][3];

    float atan2Yaw = std::atan2( hmdMatrix.m[0][2], hmdMatrix.m[2][2] );
    LOG( INFO ) << "atan2 Yaw Calculation:  " << atan2Yaw << "radians  "
                << ( ( static_cast<double>( atan2Yaw )
                       * k_radiansToCentidegrees )
                     / 100 )
                << "degrees";

    vr::HmdQuaternion_t hmdQuat = quaternion::fromHmdMatrix34( hmdMatrix );
    double hmdQuatYaw = quaternion::getYaw( hmdQuat );
    LOG( INFO ) << "Quaternion Yaw Calculation:  " << hmdQuatYaw << "radians  "
                << ( ( hmdQuatYaw * k_radiansToCentidegrees ) / 100 )
                << "degrees";
    LOG( INFO ) << "";
}

int MoveCenterTabController::trackingUniverse() const
{
    return static_cast<int>( m_trackingUniverse );
}

void MoveCenterTabController::setTrackingUniverse( int value, bool notify )
{
    if ( m_trackingUniverse != value )
    {
        vr::VRChaperoneSetup()->HideWorkingSetPreview();
        if ( !m_roomSetupModeDetected && value == vr::TrackingUniverseStanding )
        {
            reset();
        }

        m_trackingUniverse = value;
        if ( notify )
        {
            emit trackingUniverseChanged( m_trackingUniverse );
            LOG( INFO ) << "Tracking Universe: " << m_trackingUniverse;
        }
    }
}

float MoveCenterTabController::offsetX() const
{
    return m_offsetX;
}

void MoveCenterTabController::setOffsetX( float value, bool notify )
{
    if ( m_offsetX != value )
    {
        modOffsetX( value - m_offsetX, notify );
    }
}

float MoveCenterTabController::offsetY() const
{
    return m_offsetY;
}

void MoveCenterTabController::setOffsetY( float value, bool notify )
{
    if ( m_offsetY != value )
    {
        modOffsetY( value - m_offsetY, notify );
    }
}

float MoveCenterTabController::offsetZ() const
{
    return m_offsetZ;
}

void MoveCenterTabController::setOffsetZ( float value, bool notify )
{
    if ( m_offsetZ != value )
    {
        modOffsetZ( value - m_offsetZ, notify );
    }
}

int MoveCenterTabController::rotation() const
{
    return m_rotation;
}

int MoveCenterTabController::tempRotation() const
{
    return m_tempRotation;
}

void MoveCenterTabController::setRotation( int value, bool notify )
{
    if ( m_rotation != value )
    {
        if ( m_universeCenteredRotation )
        {
            m_rotation = value;
            if ( notify )
            {
                emit rotationChanged( m_rotation );
            }
            return;
        }

        double angle = ( value - m_rotation ) * k_centidegreesToRadians;

        // Get hmd pose matrix.
        vr::TrackedDevicePose_t
            devicePosesForRot[vr::k_unMaxTrackedDeviceCount];

        // source must be current universe
        if ( m_trackingUniverse == vr::TrackingUniverseSeated )
        {
            vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(
                vr::TrackingUniverseSeated,
                0.0f,
                devicePosesForRot,
                vr::k_unMaxTrackedDeviceCount );
        }
        else
        {
            vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(
                vr::TrackingUniverseStanding,
                0.0f,
                devicePosesForRot,
                vr::k_unMaxTrackedDeviceCount );
        }

        vr::HmdMatrix34_t oldHmdPos
            = devicePosesForRot[0].mDeviceToAbsoluteTracking;

        // Set up xyz coordinate values from pose matrix.
        double oldHmdXyz[3] = { static_cast<double>( oldHmdPos.m[0][3] ),
                                static_cast<double>( oldHmdPos.m[1][3] ),
                                static_cast<double>( oldHmdPos.m[2][3] ) };
        double newHmdXyz[3] = { static_cast<double>( oldHmdPos.m[0][3] ),
                                static_cast<double>( oldHmdPos.m[1][3] ),
                                static_cast<double>( oldHmdPos.m[2][3] ) };

        // Convert oldHmdXyz into un-rotated coordinates.
        double oldAngle = -m_rotation * k_centidegreesToRadians;
        rotateCoordinates( oldHmdXyz, oldAngle );

        // Set newHmdXyz to have additional rotation from incoming angle change.
        rotateCoordinates( newHmdXyz, oldAngle - angle );

        // find difference in x,z offset due to incoming angle change
        // (coordinates are in un-rotated axis).
        double hmdRotDiff[3]
            = { oldHmdXyz[0] - newHmdXyz[0], 0, oldHmdXyz[2] - newHmdXyz[2] };

        m_rotation = value;
        if ( notify )
        {
            emit rotationChanged( m_rotation );
        }

        // Update UI offsets.
        m_offsetX += static_cast<float>( hmdRotDiff[0] );
        m_offsetZ += static_cast<float>( hmdRotDiff[2] );
        if ( notify )
        {
            emit offsetXChanged( m_offsetX );
            emit offsetZChanged( m_offsetZ );
        }
    }
}

void MoveCenterTabController::setTempRotation( int value, bool notify )
{
    m_tempRotation = value;
    if ( notify )
    {
        emit tempRotationChanged( m_tempRotation );
    }
}

int MoveCenterTabController::snapTurnAngle() const
{
    return m_snapTurnAngle;
}

void MoveCenterTabController::setSnapTurnAngle( int value, bool notify )
{
    if ( m_snapTurnAngle != value )
    {
        m_snapTurnAngle = value;
        auto settings = OverlayController::appSettings();
        settings->beginGroup( "playspaceSettings" );
        settings->setValue( "snapTurnAngle", m_snapTurnAngle );
        settings->endGroup();
        settings->sync();
        if ( notify )
        {
            emit snapTurnAngleChanged( m_snapTurnAngle );
        }
    }
}

int MoveCenterTabController::smoothTurnRate() const
{
    return m_smoothTurnRate;
}

void MoveCenterTabController::setSmoothTurnRate( int value, bool notify )
{
    if ( m_smoothTurnRate != value )
    {
        m_smoothTurnRate = value;
        auto settings = OverlayController::appSettings();
        settings->beginGroup( "playspaceSettings" );
        settings->setValue( "smoothTurnRate", m_smoothTurnRate );
        settings->endGroup();
        settings->sync();
        if ( notify )
        {
            emit smoothTurnRateChanged( m_smoothTurnRate );
        }
    }
}

bool MoveCenterTabController::adjustChaperone() const
{
    return m_adjustChaperone;
}

void MoveCenterTabController::setAdjustChaperone( bool value, bool notify )
{
    if ( m_adjustChaperone != value )
    {
        m_adjustChaperone = value;
        auto settings = OverlayController::appSettings();
        settings->beginGroup( "playspaceSettings" );
        settings->setValue( "adjustChaperone", m_adjustChaperone );
        settings->endGroup();
        settings->sync();
        if ( notify )
        {
            emit adjustChaperoneChanged( m_adjustChaperone );
        }
        if ( !m_adjustChaperone )
        {
            LOG( WARNING ) << "CHANGED SETTINGS: Adjust Chaperone DISABLED";
        }
        else
        {
            LOG( INFO ) << "CHANGED SETTINGS: Adjust Chaperone Set: "
                        << m_adjustChaperone;
        }
    }
}

bool MoveCenterTabController::moveShortcutRight() const
{
    return m_settingsRightHandDragEnabled;
}

void MoveCenterTabController::setMoveShortcutRight( bool value, bool notify )
{
    m_settingsRightHandDragEnabled = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "moveShortcutRight", m_settingsRightHandDragEnabled );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit moveShortcutRightChanged( m_settingsRightHandDragEnabled );
    }
    LOG( INFO ) << "CHANGED SETTINGS: Right Hand Space-Drag Bind Enable Set: "
                << m_settingsRightHandDragEnabled;
}

bool MoveCenterTabController::moveShortcutLeft() const
{
    return m_settingsLeftHandDragEnabled;
}

void MoveCenterTabController::setMoveShortcutLeft( bool value, bool notify )
{
    m_settingsLeftHandDragEnabled = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "moveShortcutLeft", m_settingsLeftHandDragEnabled );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit moveShortcutLeftChanged( m_settingsLeftHandDragEnabled );
    }
    LOG( INFO ) << "CHANGED SETTINGS: Left Hand Space-Drag Bind Enable Set: "
                << m_settingsLeftHandDragEnabled;
}

bool MoveCenterTabController::turnBindLeft() const
{
    return m_settingsLeftHandTurnEnabled;
}

void MoveCenterTabController::setTurnBindLeft( bool value, bool notify )
{
    m_settingsLeftHandTurnEnabled = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "turnBindLeft", m_settingsLeftHandTurnEnabled );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit turnBindLeftChanged( m_settingsLeftHandTurnEnabled );
    }
    LOG( INFO ) << "CHANGED SETTINGS: Left Hand Space-Turn Bind Enable Set: "
                << m_settingsLeftHandTurnEnabled;
}

bool MoveCenterTabController::turnBindRight() const
{
    return m_settingsRightHandTurnEnabled;
}

void MoveCenterTabController::setTurnBindRight( bool value, bool notify )
{
    m_settingsRightHandTurnEnabled = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "turnBindRight", m_settingsRightHandTurnEnabled );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit turnBindRightChanged( m_settingsRightHandTurnEnabled );
    }
    LOG( INFO ) << "CHANGED SETTINGS: Right Hand Space-Turn Bind Enable Set: "
                << m_settingsRightHandTurnEnabled;
}

bool MoveCenterTabController::dragBounds() const
{
    return m_dragBounds;
}

void MoveCenterTabController::setDragBounds( bool value, bool notify )
{
    // detect deactivate
    if ( m_dragBounds && !value )
    {
        // set force bounds back to default on deactivate
        vr::VRChaperone()->ForceBoundsVisible(
            parent->m_chaperoneTabController.forceBounds() );
    }
    m_dragBounds = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "dragBounds", m_dragBounds );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit dragBoundsChanged( m_dragBounds );
    }
    LOG( INFO ) << "CHANGED SETTINGS: Space Drag Force Bounds set: "
                << m_dragBounds;
}

bool MoveCenterTabController::turnBounds() const
{
    return m_turnBounds;
}

void MoveCenterTabController::setTurnBounds( bool value, bool notify )
{
    // detect deactivate
    if ( m_turnBounds && !value )
    {
        // set force bounds back to default on deactivate
        vr::VRChaperone()->ForceBoundsVisible(
            parent->m_chaperoneTabController.forceBounds() );
    }
    m_turnBounds = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "turnBounds", m_turnBounds );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit turnBoundsChanged( m_turnBounds );
    }
    LOG( INFO ) << "CHANGED SETTINGS: Space Turn Force Bounds set: "
                << m_turnBounds;
}

unsigned MoveCenterTabController::dragComfortFactor() const
{
    return m_dragComfortFactor;
}

void MoveCenterTabController::setDragComfortFactor( unsigned value,
                                                    bool notify )
{
    m_dragComfortFactor = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "dragComfortFactor", m_dragComfortFactor );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit dragComfortFactorChanged( m_dragComfortFactor );
    }
}

unsigned MoveCenterTabController::turnComfortFactor() const
{
    return m_turnComfortFactor;
}

void MoveCenterTabController::setTurnComfortFactor( unsigned value,
                                                    bool notify )
{
    m_turnComfortFactor = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "turnComfortFactor", m_turnComfortFactor );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit turnComfortFactorChanged( m_turnComfortFactor );
    }
}

bool MoveCenterTabController::heightToggle() const
{
    return m_heightToggle;
}

void MoveCenterTabController::setHeightToggle( bool value, bool notify )
{
    // detect new activate
    if ( !m_heightToggle && value )
    {
        // Don't move bump around on y axis if gravity is active
        if ( !m_gravityActive )
        {
            m_offsetY += m_heightToggleOffset;
            emit offsetYChanged( m_offsetY );
        }
        m_gravityFloor = m_heightToggleOffset;
    }
    // detect new deactivate
    else if ( m_heightToggle && !value )
    {
        // Don't move bump around on y axis if gravity is active
        if ( !m_gravityActive )
        {
            m_offsetY -= m_heightToggleOffset;
            emit offsetYChanged( m_offsetY );
        }
        m_gravityFloor = 0.0f;
    }

    m_heightToggle = value;
    if ( notify )
    {
        emit heightToggleChanged( m_heightToggle );
    }
}

float MoveCenterTabController::heightToggleOffset() const
{
    return m_heightToggleOffset;
}

void MoveCenterTabController::setHeightToggleOffset( float value, bool notify )
{
    m_heightToggleOffset = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "heightToggleOffset", m_heightToggleOffset );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit heightToggleOffsetChanged( m_heightToggleOffset );
    }
}

float MoveCenterTabController::gravityStrength() const
{
    return m_gravityStrength;
}

void MoveCenterTabController::setGravityStrength( float value, bool notify )
{
    m_gravityStrength = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "gravityStrength", m_gravityStrength );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit gravityStrengthChanged( m_gravityStrength );
    }
    m_lastGravityUpdateTimePoint = std::chrono::steady_clock::now();
}

float MoveCenterTabController::flingStrength() const
{
    return m_flingStrength;
}

void MoveCenterTabController::setFlingStrength( float value, bool notify )
{
    m_flingStrength = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "flingStrength", m_flingStrength );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit flingStrengthChanged( m_flingStrength );
    }
}

bool MoveCenterTabController::gravityActive() const
{
    return m_gravityActive;
}

void MoveCenterTabController::setGravityActive( bool value, bool notify )
{
    // skip if there's no change
    if ( m_gravityActive == value )
    {
        return;
    }

    // detect new activate
    if ( !m_gravityActive && value )
    {
        // zero out velocity if we aren't saving previous momentum
        if ( !m_momentumSave )
        {
            m_velocity[0] = 0.0;
            m_velocity[1] = 0.0;
            m_velocity[2] = 0.0;
        }
        // make sure our time slice calculation doesn't use a slice from the
        // previous activation of gravity
        m_lastGravityUpdateTimePoint = std::chrono::steady_clock::now();
    }
    m_gravityActive = value;
    if ( notify )
    {
        emit gravityActiveChanged( m_gravityActive );
    }
}

bool MoveCenterTabController::momentumSave() const
{
    return m_momentumSave;
}

void MoveCenterTabController::setMomentumSave( bool value, bool notify )
{
    m_momentumSave = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "momentumSave", m_momentumSave );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit momentumSaveChanged( m_momentumSave );
    }
}

bool MoveCenterTabController::lockXToggle() const
{
    return m_lockXToggle;
}

void MoveCenterTabController::setLockX( bool value, bool notify )
{
    m_lockXToggle = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "lockXToggle", m_lockXToggle );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit requireLockXChanged( m_lockXToggle );
    }
}

bool MoveCenterTabController::lockYToggle() const
{
    return m_lockYToggle;
}

void MoveCenterTabController::setLockY( bool value, bool notify )
{
    m_lockYToggle = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "lockYToggle", m_lockYToggle );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit requireLockYChanged( m_lockYToggle );
    }
}

bool MoveCenterTabController::lockZToggle() const
{
    return m_lockZToggle;
}

void MoveCenterTabController::setLockZ( bool value, bool notify )
{
    m_lockZToggle = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "lockZToggle", m_lockZToggle );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit requireLockZChanged( m_lockZToggle );
    }
}

bool MoveCenterTabController::showLogMatricesButton() const
{
    return m_showLogMatricesButton;
}

void MoveCenterTabController::setShowLogMatricesButton( bool value,
                                                        bool notify )
{
    m_showLogMatricesButton = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "showLogMatricesButton", m_showLogMatricesButton );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit showLogMatricesButtonChanged( m_showLogMatricesButton );
    }
}

bool MoveCenterTabController::allowExternalEdits() const
{
    return m_allowExternalEdits;
}

void MoveCenterTabController::setAllowExternalEdits( bool value, bool notify )
{
    m_allowExternalEdits = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "allowExternalEdits", m_allowExternalEdits );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit allowExternalEditsChanged( m_allowExternalEdits );
    }
    LOG( INFO ) << "CHANGED SETTINGS: Allow External Edits Set: "
                << m_allowExternalEdits;
}

bool MoveCenterTabController::oldStyleMotion() const
{
    return m_oldStyleMotion;
}

void MoveCenterTabController::setOldStyleMotion( bool value, bool notify )
{
    // detect incoming change to old style, and hide working set
    if ( value && !m_oldStyleMotion )
    {
        vr::VRChaperoneSetup()->HideWorkingSetPreview();
    }

    m_oldStyleMotion = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "oldStyleMotion", m_oldStyleMotion );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit oldStyleMotionChanged( m_oldStyleMotion );
    }
    LOG( INFO ) << "CHANGED SETTINGS: Old-Style Motion Set: "
                << m_oldStyleMotion;
}

bool MoveCenterTabController::universeCenteredRotation() const
{
    return m_universeCenteredRotation;
}

void MoveCenterTabController::setUniverseCenteredRotation( bool value,
                                                           bool notify )
{
    m_universeCenteredRotation = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "universeCenteredRotation",
                        m_universeCenteredRotation );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit universeCenteredRotationChanged( m_universeCenteredRotation );
    }
    LOG( INFO ) << "CHANGED SETTINGS: Universe-Centered Rotation Set: "
                << m_universeCenteredRotation;
}

void MoveCenterTabController::modOffsetX( float value, bool notify )
{
    if ( !m_lockXToggle )
    {
        m_offsetX += value;
        if ( notify )
        {
            emit offsetXChanged( m_offsetX );
        }
    }
}

void MoveCenterTabController::modOffsetY( float value, bool notify )
{
    if ( !m_lockYToggle )
    {
        m_offsetY += value;
        if ( notify )
        {
            emit offsetYChanged( m_offsetY );
        }
    }
}

void MoveCenterTabController::modOffsetZ( float value, bool notify )
{
    if ( !m_lockZToggle )
    {
        m_offsetZ += value;
        if ( notify )
        {
            emit offsetZChanged( m_offsetZ );
        }
    }
}

void MoveCenterTabController::shutdown()
{
    reset();
    vr::VRChaperoneSetup()->HideWorkingSetPreview();
}

void MoveCenterTabController::incomingSeatedReset()
{
    updateSeatedResetData();
}

void MoveCenterTabController::reset()
{
    vr::VRChaperoneSetup()->HideWorkingSetPreview();
    m_heightToggle = false;
    emit heightToggleChanged( m_heightToggle );
    m_oldOffsetX = 0.0f;
    m_oldOffsetY = 0.0f;
    m_oldOffsetZ = 0.0f;
    m_oldRotation = 0;
    m_offsetX = 0.0f;
    m_offsetY = 0.0f;
    m_offsetZ = 0.0f;
    m_rotation = 0;
    applyChaperoneResetData();
    emit offsetXChanged( m_offsetX );
    emit offsetYChanged( m_offsetY );
    emit offsetZChanged( m_offsetZ );
    emit rotationChanged( m_rotation );
}

void MoveCenterTabController::zeroOffsets()
{
    if ( vr::VRChaperone()->GetCalibrationState()
         == vr::ChaperoneCalibrationState_OK )
    {
        m_oldOffsetX = 0.0f;
        m_oldOffsetY = 0.0f;
        m_oldOffsetZ = 0.0f;
        m_oldRotation = 0;
        m_offsetX = 0.0f;
        m_offsetY = 0.0f;
        m_offsetZ = 0.0f;
        m_rotation = 0;
        emit offsetXChanged( m_offsetX );
        emit offsetYChanged( m_offsetY );
        emit offsetZChanged( m_offsetZ );
        emit rotationChanged( m_rotation );
        updateChaperoneResetData();
        m_pendingZeroOffsets = false;
        if ( m_roomSetupModeDetected )
        {
            LOG( INFO ) << "room setup EXIT detected";
            m_roomSetupModeDetected = false;
        }

        LOG( INFO ) << "SUCCESS: Chaperone Data Updated and Offsets zeroed out";
    }
    else
    {
        if ( !m_pendingZeroOffsets )
        {
            LOG( INFO ) << "PENDING: Chaperone Data Update and Offsets zeroing";
        }
        m_pendingZeroOffsets = true;
    }
}

double MoveCenterTabController::getHmdYawTotal()
{
    return m_hmdYawTotal;
}

void MoveCenterTabController::resetHmdYawTotal()
{
    m_hmdYawTotal = 0.0;
}

void MoveCenterTabController::clampVelocity( double* velocity )
{
    for ( int i = 0; i < 3; i++ )
    {
        // too fast! clamp to terminal velocity while preserving +/- sign
        if ( std::abs( velocity[i] ) >= k_terminalVelocity_mps )
        {
            velocity[i] = copysign( k_terminalVelocity_mps, velocity[i] );
        }
    }
}

void MoveCenterTabController::updateSeatedResetData()
{
    reset();
    vr::VRChaperoneSetup()->RevertWorkingCopy();
    vr::TrackedDevicePose_t devicePosesStanding[vr::k_unMaxTrackedDeviceCount];
    vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(
        vr::TrackingUniverseStanding,
        0.0f,
        devicePosesStanding,
        vr::k_unMaxTrackedDeviceCount );
    vr::HmdMatrix34_t hmdMatrix
        = devicePosesStanding[0].mDeviceToAbsoluteTracking;

    vr::HmdQuaternion_t hmdQuat = quaternion::fromHmdMatrix34( hmdMatrix );
    double hmdYaw = quaternion::getYaw( hmdQuat );
    int hmdYawCentideg = static_cast<int>( hmdYaw * k_radiansToCentidegrees );
    // Keep angle within -18000 ~ 18000 centidegrees
    if ( hmdYawCentideg > 18000 )
    {
        hmdYawCentideg -= 36000;
    }
    else if ( hmdYawCentideg < -18000 )
    {
        hmdYawCentideg += 36000;
    }

    m_seatedCenterForReset = m_universeCenterForReset;
    m_seatedHeight = hmdMatrix.m[1][3];
    m_seatedCenterForReset.m[1][3] += m_seatedHeight;

    m_offsetX = hmdMatrix.m[0][3];
    m_offsetY = 0.0f;
    m_offsetZ = hmdMatrix.m[2][3];
    m_rotation = hmdYawCentideg;
    emit offsetXChanged( m_offsetX );
    emit offsetYChanged( m_offsetY );
    emit offsetZChanged( m_offsetZ );
    emit rotationChanged( m_rotation );
    unsigned checkQuadCount = 0;
    vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo( nullptr,
                                                           &checkQuadCount );
    if ( checkQuadCount > 0 )
    {
        parent->chaperoneUtils().loadChaperoneData( false );
    }
}

void MoveCenterTabController::updateChaperoneResetData()
{
    vr::VRChaperoneSetup()->HideWorkingSetPreview();
    vr::VRChaperoneSetup()->RevertWorkingCopy();
    unsigned currentQuadCount = 0;
    vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo( nullptr,
                                                           &currentQuadCount );
    m_collisionBoundsForReset = new vr::HmdQuad_t[currentQuadCount];
    m_collisionBoundsForOffset = new vr::HmdQuad_t[currentQuadCount];
    m_collisionBoundsCountForReset = currentQuadCount;
    vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(
        m_collisionBoundsForReset, &currentQuadCount );

    vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(
        &m_universeCenterForReset );

    // update seated center to standing offset by current hmd Y value.
    m_seatedCenterForReset = m_universeCenterForReset;
    vr::TrackedDevicePose_t devicePosesStanding[vr::k_unMaxTrackedDeviceCount];
    vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(
        vr::TrackingUniverseStanding,
        0.0f,
        devicePosesStanding,
        vr::k_unMaxTrackedDeviceCount );
    vr::HmdMatrix34_t hmdMatrix
        = devicePosesStanding[0].mDeviceToAbsoluteTracking;
    m_seatedHeight = hmdMatrix.m[1][3];
    m_seatedCenterForReset.m[1][3] += m_seatedHeight;

    if ( m_collisionBoundsCountForReset > 0 )
    {
        float universeCenterForResetYaw
            = std::atan2( m_universeCenterForReset.m[0][2],
                          m_universeCenterForReset.m[2][2] );

        // we want to store m_collisionBoundsForOffset as spacially relative to
        // m_universeCenterForReset, so:

        // for every quad in the chaperone bounds...
        for ( unsigned quad = 0; quad < m_collisionBoundsCountForReset; quad++ )
        {
            // at every corner in that quad...
            for ( unsigned corner = 0; corner < 4; corner++ )
            {
                // copy from m_collisionBoundsForReset
                m_collisionBoundsForOffset[quad].vCorners[corner].v[0]
                    = m_collisionBoundsForReset[quad].vCorners[corner].v[0];
                m_collisionBoundsForOffset[quad].vCorners[corner].v[1]
                    = m_collisionBoundsForReset[quad].vCorners[corner].v[1];
                m_collisionBoundsForOffset[quad].vCorners[corner].v[2]
                    = m_collisionBoundsForReset[quad].vCorners[corner].v[2];

                // unrotate by universe center's yaw
                rotateFloatCoordinates(
                    m_collisionBoundsForOffset[quad].vCorners[corner].v,
                    -universeCenterForResetYaw );
            }
        }
    }

    unsigned checkQuadCount = 0;
    vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo( nullptr,
                                                           &checkQuadCount );
    if ( checkQuadCount > 0 )
    {
        parent->chaperoneUtils().loadChaperoneData( false );
    }
}

void MoveCenterTabController::applyChaperoneResetData()
{
    vr::VRChaperoneSetup()->HideWorkingSetPreview();
    vr::VRChaperoneSetup()->RevertWorkingCopy();
    if ( m_collisionBoundsCountForReset > 0 )
    {
        vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(
            m_collisionBoundsForReset, m_collisionBoundsCountForReset );
    }
    vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(
        &m_universeCenterForReset );
    vr::VRChaperoneSetup()->SetWorkingSeatedZeroPoseToRawTrackingPose(
        &m_seatedCenterForReset );

    vr::VRChaperoneSetup()->CommitWorkingCopy( vr::EChaperoneConfigFile_Live );

    unsigned checkQuadCount = 0;
    vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo( nullptr,
                                                           &checkQuadCount );
    if ( checkQuadCount > 0 )
    {
        parent->chaperoneUtils().loadChaperoneData( false );
    }
}

// START of drag bindings:

void MoveCenterTabController::leftHandSpaceDrag( bool leftHandDragActive )
{
    if ( !m_settingsLeftHandDragEnabled )
    {
        return;
    }
    // cancel if other hand is active in override
    if ( !m_overrideRightHandDragPressed )
    {
        // detect new press
        if ( leftHandDragActive && !m_leftHandDragPressed )
        {
            m_activeDragHand = vr::TrackedControllerRole_LeftHand;
        }
        // detect new release
        else if ( !leftHandDragActive && m_leftHandDragPressed )
        {
            // check overrides
            // pass back to right hand?
            if ( m_overrideRightHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_RightHand;
            }
            // otherwise check right hand for activated swap drag
            if ( m_swapDragToRightHandActivated )
            {
                m_activeDragHand = vr::TrackedControllerRole_RightHand;
            }
            // override of left hand still active?
            else if ( m_overrideLeftHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            }
            // swap activation of left hand still active?
            else if ( m_swapDragToLeftHandActivated )
            {
                m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            }
            // check non-override right hand action
            if ( m_rightHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_RightHand;
            }
            // nothing else is pressed, deactivate drag
            else
            {
                m_activeDragHand = vr::TrackedControllerRole_Invalid;
            }
        }
    }
    m_leftHandDragPressed = leftHandDragActive;
}

void MoveCenterTabController::rightHandSpaceDrag( bool rightHandDragActive )
{
    if ( !m_settingsRightHandDragEnabled )
    {
        return;
    }
    // cancel if other hand is active in override
    if ( !m_overrideLeftHandDragPressed )
    {
        // detect new press
        if ( rightHandDragActive && !m_rightHandDragPressed )
        {
            m_activeDragHand = vr::TrackedControllerRole_RightHand;
        }
        // detect new release
        else if ( !rightHandDragActive && m_rightHandDragPressed )
        {
            // check overrides
            // pass back to left hand?
            if ( m_overrideLeftHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            }
            // otherwise check left hand for activated swap drag
            if ( m_swapDragToLeftHandActivated )
            {
                m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            }
            // override of right hand still active?
            else if ( m_overrideRightHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_RightHand;
            }
            // swap activation of right hand still active?
            else if ( m_swapDragToRightHandActivated )
            {
                m_activeDragHand = vr::TrackedControllerRole_RightHand;
            }
            // check non-override left hand action
            if ( m_leftHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            }
            // nothing else is pressed, deactivate drag
            else
            {
                m_activeDragHand = vr::TrackedControllerRole_Invalid;
            }
        }
    }
    m_rightHandDragPressed = rightHandDragActive;
}

void MoveCenterTabController::optionalOverrideLeftHandSpaceDrag(
    bool overrideLeftHandDragActive )
{
    if ( !m_settingsLeftHandDragEnabled )
    {
        return;
    }
    // detect new press
    if ( overrideLeftHandDragActive && !m_overrideLeftHandDragPressed )
    {
        m_activeDragHand = vr::TrackedControllerRole_LeftHand;
        // stop any active rotate because we're in override mode
        m_activeTurnHand = vr::TrackedControllerRole_Invalid;
    }
    // detect new release
    else if ( !overrideLeftHandDragActive && m_overrideLeftHandDragPressed )
    {
        // check if we should pass back to other hand
        // give priority to override action
        if ( m_overrideRightHandDragPressed )
        {
            m_activeDragHand = vr::TrackedControllerRole_RightHand;
        }
        // otherwise check right hand for activated swap drag
        if ( m_swapDragToRightHandActivated )
        {
            m_activeDragHand = vr::TrackedControllerRole_RightHand;
        }
        // otherwise check normal action
        else if ( m_rightHandDragPressed )
        {
            m_activeDragHand = vr::TrackedControllerRole_RightHand;
        }
        // swap activation of left hand still active?
        else if ( m_swapDragToLeftHandActivated )
        {
            m_activeDragHand = vr::TrackedControllerRole_LeftHand;
        }
        // check if we should fall back to non-override left hand
        else if ( m_leftHandDragPressed )
        {
            m_activeDragHand = vr::TrackedControllerRole_LeftHand;
        }
        // nothing else is pressed, deactivate drag
        else
        {
            m_activeDragHand = vr::TrackedControllerRole_Invalid;
        }
    }
    m_overrideLeftHandDragPressed = overrideLeftHandDragActive;
}

void MoveCenterTabController::optionalOverrideRightHandSpaceDrag(
    bool overrideRightHandDragActive )
{
    if ( !m_settingsRightHandDragEnabled )
    {
        return;
    }
    // detect new press
    if ( overrideRightHandDragActive && !m_overrideRightHandDragPressed )
    {
        m_activeDragHand = vr::TrackedControllerRole_RightHand;
        // stop any active rotate because we're in override mode
        m_activeTurnHand = vr::TrackedControllerRole_Invalid;
    }
    // detect new release
    else if ( !overrideRightHandDragActive && m_overrideRightHandDragPressed )
    {
        // check if we should pass back to other hand
        // give priority to override action
        if ( m_overrideLeftHandDragPressed )
        {
            m_activeDragHand = vr::TrackedControllerRole_LeftHand;
        }
        // otherwise check left hand for activated swap drag
        if ( m_swapDragToLeftHandActivated )
        {
            m_activeDragHand = vr::TrackedControllerRole_LeftHand;
        }
        // otherwise check normal action
        else if ( m_leftHandDragPressed )
        {
            m_activeDragHand = vr::TrackedControllerRole_LeftHand;
        }
        // swap activation of right hand still active?
        else if ( m_swapDragToRightHandActivated )
        {
            m_activeDragHand = vr::TrackedControllerRole_RightHand;
        }
        // check if we should fall back to non-override right hand
        else if ( m_rightHandDragPressed )
        {
            m_activeDragHand = vr::TrackedControllerRole_RightHand;
        }
        // nothing else is pressed, deactivate drag
        else
        {
            m_activeDragHand = vr::TrackedControllerRole_Invalid;
        }
    }
    m_overrideRightHandDragPressed = overrideRightHandDragActive;
}

void MoveCenterTabController::swapSpaceDragToLeftHandOverride(
    bool swapDragToLeftHand )
{
    if ( !m_settingsLeftHandDragEnabled )
    {
        return;
    }
    // detect new press
    if ( swapDragToLeftHand && !m_swapDragToLeftHandPressed )
    {
        // check for active right hand drag to swap from
        if ( m_activeDragHand == vr::TrackedControllerRole_RightHand )
        {
            m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            // stop any active rotate because we're in override mode
            m_activeTurnHand = vr::TrackedControllerRole_Invalid;
            // set this hand's swap action to successfully activated for
            // tracking if we should pass the drag back if another hand is
            // released
            m_swapDragToLeftHandActivated = true;
        }
    }
    // detect new release
    else if ( !swapDragToLeftHand && m_swapDragToLeftHandPressed )
    {
        // check if the left hand swap had even activated
        if ( m_swapDragToLeftHandActivated
             && m_activeDragHand == vr::TrackedControllerRole_LeftHand )
        {
            // check if we should pass back to other hand
            // give priority to override action
            if ( m_overrideRightHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_RightHand;
            }
            // otherwise check if other hand's swap drag was active
            else if ( m_swapDragToRightHandActivated )
            {
                m_activeDragHand = vr::TrackedControllerRole_RightHand;
            }
            // otherwise check if we retain left hand via its override
            // action
            else if ( m_overrideLeftHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            }
            // otherwise check normal action on right hand
            else if ( m_rightHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_RightHand;
            }
            // otherwise check if we retain left hand via its normal action
            else if ( m_leftHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            }
            // nothing else is pressed, deactivate drag.
            else
            {
                m_activeDragHand = vr::TrackedControllerRole_Invalid;
            }
        }
    }
    m_swapDragToLeftHandPressed = swapDragToLeftHand;
    // we only set activated flag when the swap was successfull, but we always
    // deactivate when swap action isn't pressed.
    if ( !swapDragToLeftHand )
    {
        m_swapDragToLeftHandActivated = false;
    }
    // deactivate turning even when not detecting a new press as long as a swap
    // is successfully activated now
    if ( m_swapDragToLeftHandActivated )
    {
        m_activeTurnHand = vr::TrackedControllerRole_Invalid;
    }
}

void MoveCenterTabController::swapSpaceDragToRightHandOverride(
    bool swapDragToRightHand )
{
    if ( !m_settingsRightHandDragEnabled )
    {
        return;
    }
    // detect new press
    if ( swapDragToRightHand && !m_swapDragToRightHandPressed )
    {
        // check for active left hand drag to swap from
        if ( m_activeDragHand == vr::TrackedControllerRole_LeftHand )
        {
            m_activeDragHand = vr::TrackedControllerRole_RightHand;
            // stop any active rotate because we're in override mode
            m_activeTurnHand = vr::TrackedControllerRole_Invalid;
            // set this hand's swap action to successfully activated for
            // tracking if we should pass the drag back if another hand is
            // released
            m_swapDragToRightHandActivated = true;
        }
    }
    // detect new release
    else if ( !swapDragToRightHand && m_swapDragToRightHandPressed )
    {
        // check if the right hand swap had even activated
        if ( m_swapDragToRightHandActivated
             && m_activeDragHand == vr::TrackedControllerRole_RightHand )
        {
            // check if we should pass back to other hand
            // give priority to override action
            if ( m_overrideLeftHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            }
            // otherwise check if other hand's swap drag was active
            else if ( m_swapDragToLeftHandActivated )
            {
                m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            }
            // otherwise check if we retain right hand via its override
            // action
            else if ( m_overrideRightHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_RightHand;
            }
            // otherwise check normal action on left hand
            else if ( m_leftHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            }
            // otherwise check if we retain right hand via its normal action
            else if ( m_rightHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_RightHand;
            }
            // nothing else is pressed, deactivate drag.
            else
            {
                m_activeDragHand = vr::TrackedControllerRole_Invalid;
            }
        }
    }

    m_swapDragToRightHandPressed = swapDragToRightHand;

    // we only set activated flag when the swap was successfull, but we always
    // deactivate when swap action isn't pressed.
    if ( !swapDragToRightHand )
    {
        m_swapDragToRightHandActivated = false;
    }
    // deactivate turning even when not detecting a new press as long as a swap
    // is successfully activated now
    if ( m_swapDragToRightHandActivated )
    {
        m_activeTurnHand = vr::TrackedControllerRole_Invalid;
    }
}

// END of drag bindings

// START of turn bindgins

void MoveCenterTabController::leftHandSpaceTurn( bool leftHandTurnActive )
{
    if ( !m_settingsLeftHandTurnEnabled )
    {
        return;
    }
    // cancel if other hand is active in override
    if ( !m_overrideRightHandTurnPressed )
    {
        // detect new press
        if ( leftHandTurnActive && !m_leftHandTurnPressed )
        {
            m_activeTurnHand = vr::TrackedControllerRole_LeftHand;
        }
        // detect new release
        else if ( !leftHandTurnActive && m_leftHandTurnPressed )
        {
            // check overrides
            // pass back to right hand?
            if ( m_overrideRightHandTurnPressed )
            {
                m_activeTurnHand = vr::TrackedControllerRole_RightHand;
            }
            // override of left hand still active?
            else if ( m_overrideLeftHandTurnPressed )
            {
                m_activeTurnHand = vr::TrackedControllerRole_LeftHand;
            }
            // check non-override right hand action
            if ( m_rightHandTurnPressed )
            {
                m_activeTurnHand = vr::TrackedControllerRole_RightHand;
            }
            // nothing else is pressed, deactivate rotate
            else
            {
                m_activeTurnHand = vr::TrackedControllerRole_Invalid;
            }
        }
    }
    m_leftHandTurnPressed = leftHandTurnActive;
}

void MoveCenterTabController::rightHandSpaceTurn( bool rightHandTurnActive )
{
    if ( !m_settingsRightHandTurnEnabled )
    {
        return;
    }
    // cancel if other hand is active in override
    if ( !m_overrideLeftHandTurnPressed )
    {
        // detect new press
        if ( rightHandTurnActive && !m_rightHandTurnPressed )
        {
            m_activeTurnHand = vr::TrackedControllerRole_RightHand;
        }
        // detect new release
        else if ( !rightHandTurnActive && m_rightHandTurnPressed )
        {
            // check overrides
            // pass back to left hand?
            if ( m_overrideLeftHandTurnPressed )
            {
                m_activeTurnHand = vr::TrackedControllerRole_LeftHand;
            }
            // override of right hand still active?
            else if ( m_overrideRightHandTurnPressed )
            {
                m_activeTurnHand = vr::TrackedControllerRole_RightHand;
            }
            // check non-override left hand action
            if ( m_leftHandTurnPressed )
            {
                m_activeTurnHand = vr::TrackedControllerRole_LeftHand;
            }
            // nothing else is pressed, deactivate rotate
            else
            {
                m_activeTurnHand = vr::TrackedControllerRole_Invalid;
            }
        }
    }
    m_rightHandTurnPressed = rightHandTurnActive;
}

void MoveCenterTabController::optionalOverrideLeftHandSpaceTurn(
    bool overrideLeftHandTurnActive )
{
    if ( !m_settingsLeftHandTurnEnabled )
    {
        return;
    }
    // detect new press
    if ( overrideLeftHandTurnActive && !m_overrideLeftHandTurnPressed )
    {
        m_activeTurnHand = vr::TrackedControllerRole_LeftHand;
        // stop any active move because we're in override mode
        m_activeDragHand = vr::TrackedControllerRole_Invalid;
    }
    // detect new release
    else if ( !overrideLeftHandTurnActive && m_overrideLeftHandTurnPressed )
    {
        // check if we should pass back to other hand
        // give priority to override action
        if ( m_overrideRightHandTurnPressed )
        {
            m_activeTurnHand = vr::TrackedControllerRole_RightHand;
        }
        // otherwise check normal action
        else if ( m_rightHandTurnPressed )
        {
            m_activeTurnHand = vr::TrackedControllerRole_RightHand;
        }
        // check if we should fall back to non-override left hand
        else if ( m_leftHandTurnPressed )
        {
            m_activeTurnHand = vr::TrackedControllerRole_LeftHand;
        }
        // nothing else is pressed, deactivate rotate
        else
        {
            m_activeTurnHand = vr::TrackedControllerRole_Invalid;
        }
    }
    m_overrideLeftHandTurnPressed = overrideLeftHandTurnActive;
}

void MoveCenterTabController::optionalOverrideRightHandSpaceTurn(
    bool overrideRightHandTurnActive )
{
    if ( !m_settingsRightHandTurnEnabled )
    {
        return;
    }
    // detect new press
    if ( overrideRightHandTurnActive && !m_overrideRightHandTurnPressed )
    {
        m_activeTurnHand = vr::TrackedControllerRole_RightHand;
        // stop any active move because we're in override mode
        m_activeDragHand = vr::TrackedControllerRole_Invalid;
    }
    // detect new release
    else if ( !overrideRightHandTurnActive && m_overrideRightHandTurnPressed )
    {
        // check if we should pass back to other hand
        // give priority to override action
        if ( m_overrideLeftHandTurnPressed )
        {
            m_activeTurnHand = vr::TrackedControllerRole_LeftHand;
        }
        // otherwise check normal action
        else if ( m_leftHandTurnPressed )
        {
            m_activeTurnHand = vr::TrackedControllerRole_LeftHand;
        }
        // check if we should fall back to non-override right hand
        else if ( m_rightHandTurnPressed )
        {
            m_activeTurnHand = vr::TrackedControllerRole_RightHand;
        }
        // nothing else is pressed, deactivate rotate
        else
        {
            m_activeTurnHand = vr::TrackedControllerRole_Invalid;
        }
    }
    m_overrideRightHandTurnPressed = overrideRightHandTurnActive;
}

// END of turn bindings.

// START of other bindings.

void MoveCenterTabController::gravityToggleAction(
    bool gravityToggleJustPressed )
{
    if ( !gravityToggleJustPressed )
    {
        return;
    }

    if ( !m_gravityActive )
    {
        setGravityActive( true );
    }
    else
    {
        setGravityActive( false );
    }
}

void MoveCenterTabController::gravityReverseAction( bool gravityReverseHeld )
{
    // detect new press or new release
    if ( ( gravityReverseHeld && !m_gravityReversed )
         || ( !gravityReverseHeld && m_gravityReversed ) )
    {
        setGravityStrength( m_gravityStrength * -1.0f );
    }
    m_gravityReversed = gravityReverseHeld;
}

void MoveCenterTabController::heightToggleAction( bool heightToggleJustPressed )
{
    if ( !heightToggleJustPressed )
    {
        return;
    }

    if ( !m_heightToggle )
    {
        setHeightToggle( true );
    }
    else
    {
        setHeightToggle( false );
    }
}

void MoveCenterTabController::resetOffsets( bool resetOffsetsJustPressed )
{
    if ( resetOffsetsJustPressed )
    {
        reset();
    }
}

void MoveCenterTabController::snapTurnLeft( bool snapTurnLeftJustPressed )
{
    if ( !snapTurnLeftJustPressed )
    {
        return;
    }

    int newRotationAngleDeg = m_rotation - m_snapTurnAngle;
    // Keep angle within -18000 ~ 18000 centidegrees
    if ( newRotationAngleDeg > 18000 )
    {
        newRotationAngleDeg -= 36000;
    }
    else if ( newRotationAngleDeg < -18000 )
    {
        newRotationAngleDeg += 36000;
    }

    setRotation( newRotationAngleDeg );
}

void MoveCenterTabController::snapTurnRight( bool snapTurnRightJustPressed )
{
    if ( !snapTurnRightJustPressed )
    {
        return;
    }

    int newRotationAngleDeg = m_rotation + m_snapTurnAngle;
    // Keep angle within -18000 ~ 18000 centidegrees
    if ( newRotationAngleDeg > 18000 )
    {
        newRotationAngleDeg -= 36000;
    }
    else if ( newRotationAngleDeg < -18000 )
    {
        newRotationAngleDeg += 36000;
    }

    setRotation( newRotationAngleDeg );
}

void MoveCenterTabController::smoothTurnLeft( bool smoothTurnLeftActive )
{
    if ( !smoothTurnLeftActive )
    {
        return;
    }

    // Activates every tick. m_smoothTurnRate effectively becomes a percentage
    // of a degree per tick. A setting of 100 would equal 90 degrees/sec or 15
    // RPM with a framerate of 90fps
    int newRotationAngleDeg = m_rotation - m_smoothTurnRate;
    // Keep angle within -18000 ~ 18000 centidegrees
    if ( newRotationAngleDeg > 18000 )
    {
        newRotationAngleDeg -= 36000;
    }
    else if ( newRotationAngleDeg < -18000 )
    {
        newRotationAngleDeg += 36000;
    }

    setRotation( newRotationAngleDeg );
}

void MoveCenterTabController::smoothTurnRight( bool smoothTurnRightActive )
{
    if ( !smoothTurnRightActive )
    {
        return;
    }

    // Activates every tick. m_smoothTurnRate effectively becomes a percentage
    // of a degree per tick. A setting of 100 would equal 90 degrees/sec or 15
    // RPM with a framerate of 90fps
    int newRotationAngleDeg = m_rotation + m_smoothTurnRate;
    // Keep angle within -18000 ~ 18000 centidegrees
    if ( newRotationAngleDeg > 18000 )
    {
        newRotationAngleDeg -= 36000;
    }
    else if ( newRotationAngleDeg < -18000 )
    {
        newRotationAngleDeg += 36000;
    }

    setRotation( newRotationAngleDeg );
}

void MoveCenterTabController::xAxisLockToggle( bool xAxisLockToggleJustPressed )
{
    if ( !xAxisLockToggleJustPressed )
    {
        return;
    }

    if ( !m_lockXToggle )
    {
        setLockX( true );
    }
    else
    {
        setLockX( false );
    }
}

void MoveCenterTabController::yAxisLockToggle( bool yAxisLockToggleJustPressed )
{
    if ( !yAxisLockToggleJustPressed )
    {
        return;
    }

    if ( !m_lockYToggle )
    {
        setLockY( true );
    }
    else
    {
        setLockY( false );
    }
}

void MoveCenterTabController::zAxisLockToggle( bool zAxisLockToggleJustPressed )
{
    if ( !zAxisLockToggleJustPressed )
    {
        return;
    }

    if ( !m_lockZToggle )
    {
        setLockZ( true );
    }
    else
    {
        setLockZ( false );
    }
}

// END of other bindings

void MoveCenterTabController::saveUncommittedChaperone()
{
    if ( !m_chaperoneCommitted )
    {
        // double commit... once to ensure before we HideWorkingSetPreview the
        // working set is synced. Commit a second time as a workaround for WMR
        // and Rift not getting the commit during active ShowWorkingSetPreview
        vr::VRChaperoneSetup()->CommitWorkingCopy(
            vr::EChaperoneConfigFile_Live );
        vr::VRChaperoneSetup()->HideWorkingSetPreview();
        vr::VRChaperoneSetup()->CommitWorkingCopy(
            vr::EChaperoneConfigFile_Live );
        m_chaperoneCommitted = true;
        unsigned checkQuadCount = 0;
        vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(
            nullptr, &checkQuadCount );
        if ( checkQuadCount > 0 )
        {
            parent->chaperoneUtils().loadChaperoneData( false );
        }
    }
}

void MoveCenterTabController::updateHmdRotationCounter(
    vr::TrackedDevicePose_t hmdPose,
    double angle )
{
    // If hmd tracking is bad, set m_lastHmdQuaternion invalid
    if ( !hmdPose.bPoseIsValid
         || hmdPose.eTrackingResult != vr::TrackingResult_Running_OK )
    {
        m_lastHmdQuaternion.w = k_quaternionInvalidValue;
        return;
    }

    // Get hmd pose matrix (in rotated coordinates)
    vr::HmdMatrix34_t hmdMatrix = hmdPose.mDeviceToAbsoluteTracking;

    // Set up (un)rotation matrix
    vr::HmdMatrix34_t hmdMatrixRotMat;
    vr::HmdMatrix34_t hmdMatrixAbsolute;
    utils::initRotationMatrix(
        hmdMatrixRotMat, 1, static_cast<float>( angle ) );

    // Get hmdMatrixAbsolute in un-rotated coordinates.
    utils::matMul33( hmdMatrixAbsolute, hmdMatrixRotMat, hmdMatrix );

    // Convert pose matrix to quaternion
    m_hmdQuaternion = quaternion::fromHmdMatrix34( hmdMatrixAbsolute );

    // Get rotation change of hmd
    // Checking for invalid quaternion using < because == isn't guaranteed
    // for doubles comparison.
    if ( m_lastHmdQuaternion.w < k_quaternionUnderIsInvalidValueThreshold )
    {
        m_lastHmdQuaternion = m_hmdQuaternion;
        return;
    }
    // Construct a quaternion representing difference between old
    // hmd pose and new hmd pose.
    vr::HmdQuaternion_t hmdDiffQuaternion = quaternion::multiply(
        m_hmdQuaternion, quaternion::conjugate( m_lastHmdQuaternion ) );

    // Calculate yaw from quaternion.
    double hmdYawDiff = quaternion::getYaw( hmdDiffQuaternion );

    // Apply yaw difference to m_hmdYawTotal.
    m_hmdYawTotal += hmdYawDiff;
    m_lastHmdQuaternion = m_hmdQuaternion;
}

void MoveCenterTabController::updateHandDrag(
    vr::TrackedDevicePose_t* devicePoses,
    double angle )
{
    auto moveHandId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
        m_activeDragHand );

    if ( m_activeDragHand == vr::TrackedControllerRole_Invalid
         || moveHandId == vr::k_unTrackedDeviceIndexInvalid
         || moveHandId >= vr::k_unMaxTrackedDeviceCount )
    {
        // detect new release of drag bind
        if ( m_lastMoveHand != vr::TrackedControllerRole_Invalid )
        {
            emit offsetXChanged( m_offsetX );
            emit offsetYChanged( m_offsetY );
            emit offsetZChanged( m_offsetZ );

            // reset gravity update timepoint whenever a space drag was just
            // released
            m_lastGravityUpdateTimePoint = std::chrono::steady_clock::now();
        }
        m_lastMoveHand = m_activeDragHand;
        return;
    }

    vr::TrackedDevicePose_t* movePose = devicePoses + moveHandId;
    if ( !movePose->bPoseIsValid || !movePose->bDeviceIsConnected
         || movePose->eTrackingResult != vr::TrackingResult_Running_OK )
    {
        m_lastMoveHand = m_activeDragHand;
        return;
    }

    double relativeControllerPosition[] = {
        static_cast<double>( movePose->mDeviceToAbsoluteTracking.m[0][3] ),
        static_cast<double>( movePose->mDeviceToAbsoluteTracking.m[1][3] ),
        static_cast<double>( movePose->mDeviceToAbsoluteTracking.m[2][3] )
    };

    rotateCoordinates( relativeControllerPosition, -angle );
    float absoluteControllerPosition[] = {
        static_cast<float>( relativeControllerPosition[0] ) + m_offsetX,
        static_cast<float>( relativeControllerPosition[1] ) + m_offsetY,
        static_cast<float>( relativeControllerPosition[2] ) + m_offsetZ,
    };

    if ( m_lastMoveHand == m_activeDragHand )
    {
        double diff[3] = {
            static_cast<double>( absoluteControllerPosition[0]
                                 - m_lastControllerPosition[0] ),
            static_cast<double>( absoluteControllerPosition[1]
                                 - m_lastControllerPosition[1] ),
            static_cast<double>( absoluteControllerPosition[2]
                                 - m_lastControllerPosition[2] ),
        };

        // offset is un-rotated coordinates

        // prevents updating if axis movement is locked
        if ( !m_lockXToggle )
        {
            m_offsetX += static_cast<float>( diff[0] );
        }
        if ( !m_lockYToggle )
        {
            m_offsetY += static_cast<float>( diff[1] );
        }
        if ( !m_lockZToggle )
        {
            m_offsetZ += static_cast<float>( diff[2] );
        }

        double secondsSinceLastDragUpdate
            = std::chrono::duration<double>( std::chrono::steady_clock::now()
                                             - m_lastDragUpdateTimePoint )
                  .count();

        m_velocity[0] = ( diff[0] / secondsSinceLastDragUpdate )
                        * static_cast<double>( m_flingStrength );
        m_velocity[1] = ( diff[1] / secondsSinceLastDragUpdate )
                        * static_cast<double>( m_flingStrength );
        m_velocity[2] = ( diff[2] / secondsSinceLastDragUpdate )
                        * static_cast<double>( m_flingStrength );
    }
    m_lastControllerPosition[0] = absoluteControllerPosition[0];
    m_lastControllerPosition[1] = absoluteControllerPosition[1];
    m_lastControllerPosition[2] = absoluteControllerPosition[2];
    m_lastMoveHand = m_activeDragHand;
}

void MoveCenterTabController::updateHandTurn(
    vr::TrackedDevicePose_t* devicePoses,
    double angle )
{
    auto rotateHandId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
        m_activeTurnHand );

    if ( m_activeTurnHand == vr::TrackedControllerRole_Invalid
         || rotateHandId == vr::k_unTrackedDeviceIndexInvalid
         || rotateHandId >= vr::k_unMaxTrackedDeviceCount )
    {
        if ( m_lastRotateHand != vr::TrackedControllerRole_Invalid )
        {
            m_lastHandQuaternion.w = k_quaternionInvalidValue;
        }
        m_lastRotateHand = m_activeTurnHand;
        return;
    }
    vr::TrackedDevicePose_t* rotatePose = devicePoses + rotateHandId;
    if ( !rotatePose->bPoseIsValid || !rotatePose->bDeviceIsConnected
         || rotatePose->eTrackingResult != vr::TrackingResult_Running_OK )
    {
        m_lastRotateHand = m_activeTurnHand;
        return;
    }
    // Get hand's rotation.
    // handMatrix is in rotated coordinates.
    vr::HmdMatrix34_t handMatrix = rotatePose->mDeviceToAbsoluteTracking;

    // We need un-rotated coordinates for valid comparison between
    // handQuaternion and lastHandQuaternion. Set up (un)rotation
    // matrix.
    vr::HmdMatrix34_t handMatrixRotMat;
    vr::HmdMatrix34_t handMatrixAbsolute;
    utils::initRotationMatrix(
        handMatrixRotMat, 1, static_cast<float>( angle ) );

    // Get handMatrixAbsolute in un-rotated coordinates.
    utils::matMul33( handMatrixAbsolute, handMatrixRotMat, handMatrix );

    // Convert pose matrix to quaternion
    m_handQuaternion = quaternion::fromHmdMatrix34( handMatrixAbsolute );

    if ( m_lastRotateHand == m_activeTurnHand )
    {
        // Get rotation change of hand.
        // Checking for invalid quaternion using < because == isn't
        // guaranteed for doubles comparison.
        if ( m_lastHandQuaternion.w < k_quaternionUnderIsInvalidValueThreshold )
        {
            m_lastHandQuaternion = m_handQuaternion;
        }

        else
        {
            // Construct a quaternion representing difference
            // between old hand and new hand.
            vr::HmdQuaternion_t handDiffQuaternion = quaternion::multiply(
                m_handQuaternion,
                quaternion::conjugate( m_lastHandQuaternion ) );

            // Calculate yaw from quaternion.
            double handYawDiff = quaternion::getYaw( handDiffQuaternion );

            int newRotationAngleDeg = static_cast<int>(
                round( handYawDiff * k_radiansToCentidegrees ) + m_rotation );

            // Keep angle within -18000 ~ 18000 centidegrees
            if ( newRotationAngleDeg > 18000 )
            {
                newRotationAngleDeg -= 36000;
            }
            else if ( newRotationAngleDeg < -18000 )
            {
                newRotationAngleDeg += 36000;
            }

            setRotation( newRotationAngleDeg );
        }
    }
    m_lastHandQuaternion = m_handQuaternion;
    m_lastRotateHand = m_activeTurnHand;
}

void MoveCenterTabController::updateGravity()
{
    // prevent velocity underflow
    if ( std::isnan( m_velocity[0] ) )
    {
        m_velocity[0] = 0.0;
    }
    if ( std::isnan( m_velocity[1] ) )
    {
        m_velocity[1] = 0.0;
    }
    if ( std::isnan( m_velocity[2] ) )
    {
        m_velocity[2] = 0.0;
    }

    // clamp to terminal velocity
    clampVelocity( m_velocity );

    // make axis lock checkboxes lock velocity on that axis
    if ( m_lockXToggle )
    {
        m_velocity[0] = 0.0;
    }
    if ( m_lockYToggle )
    {
        m_velocity[1] = 0.0;
    }
    if ( m_lockZToggle )
    {
        m_velocity[2] = 0.0;
    }

    double secondsSinceLastGravityUpdate
        = std::chrono::duration<double>( std::chrono::steady_clock::now()
                                         - m_lastGravityUpdateTimePoint )
              .count();

    // are we falling?
    // note: up is negative y
    // note: set to always fall if gravity reversed (strength < 0)
    if ( ( m_offsetY < m_gravityFloor ) || ( m_gravityStrength < 0 ) )
    {
        // check if we're about to land
        // note: we don't land if gravity is reversed (strength < 0)
        if ( ( m_offsetY
                   + static_cast<float>( m_velocity[1]
                                         * secondsSinceLastGravityUpdate )
               >= m_gravityFloor )
             && m_gravityStrength >= 0 )
        {
            // get ratio of how much from y velocity applied to overcome y
            // offset and get down to ground.
            double ratioVelocityScaledByTouchdown
                = 1
                  - ( ( static_cast<double>( m_offsetY - m_gravityFloor )
                        + ( m_velocity[1] * secondsSinceLastGravityUpdate ) )
                      / ( m_velocity[1] * secondsSinceLastGravityUpdate ) );
            // apply that ratio to X and Z velocties
            m_velocity[0] *= ratioVelocityScaledByTouchdown;
            m_velocity[2] *= ratioVelocityScaledByTouchdown;

            // update offsets
            m_offsetX += static_cast<float>( m_velocity[0]
                                             * secondsSinceLastGravityUpdate );
            m_offsetY = m_gravityFloor;
            m_offsetZ += static_cast<float>( m_velocity[2]
                                             * secondsSinceLastGravityUpdate );
            emit offsetXChanged( m_offsetX );
            emit offsetYChanged( m_offsetY );
            emit offsetZChanged( m_offsetZ );
        }

        // otherwise we're still falling
        else
        {
            // apply offset from velocity
            m_offsetX += static_cast<float>( m_velocity[0]
                                             * secondsSinceLastGravityUpdate );
            m_offsetY += static_cast<float>( m_velocity[1]
                                             * secondsSinceLastGravityUpdate );
            m_offsetZ += static_cast<float>( m_velocity[2]
                                             * secondsSinceLastGravityUpdate );
            emit offsetXChanged( m_offsetX );
            emit offsetYChanged( m_offsetY );
            emit offsetZChanged( m_offsetZ );

            // accelerate downward velocity for the next update
            // note: downward is positive y
            m_velocity[1] = m_velocity[1]
                            + ( static_cast<double>( m_gravityStrength )
                                * secondsSinceLastGravityUpdate );
            return;
        }
    }
    // check if we're underground and force us to ground level.
    // note: downward is positive y
    if ( m_offsetY > m_gravityFloor )
    {
        m_offsetY = m_gravityFloor;
        emit offsetYChanged( m_offsetY );
    }
    // Touchdown! We've landed, velocity set to 0.
    m_velocity[0] = 0.0;
    m_velocity[1] = 0.0;
    m_velocity[2] = 0.0;
}

void MoveCenterTabController::updateSpace()
{
    // If all offsets and rotation are still the same...
    if ( m_offsetX == m_oldOffsetX && m_offsetY == m_oldOffsetY
         && m_offsetZ == m_oldOffsetZ && m_rotation == m_oldRotation )
    {
        // ... wait for the comfort mode delay and commit chaperone
        if ( m_dragComfortFactor >= m_turnComfortFactor )
        {
            if ( m_dragComfortFrameSkipCounter
                 >= ( m_dragComfortFactor * m_dragComfortFactor ) )
            {
                saveUncommittedChaperone();
            }
        }
        else if ( m_turnComfortFrameSkipCounter
                  >= ( m_turnComfortFactor * m_turnComfortFactor ) )
        {
            saveUncommittedChaperone();
        }
        return;
    }

    if ( m_allowExternalEdits && m_oldOffsetX == 0.0f && m_oldOffsetY == 0.0f
         && m_oldOffsetZ == 0.0f && m_oldRotation == 0 )
    {
        vr::VRChaperoneSetup()->ReloadFromDisk( vr::EChaperoneConfigFile_Live );
        vr::VRChaperoneSetup()->CommitWorkingCopy(
            vr::EChaperoneConfigFile_Live );
        updateChaperoneResetData();
    }

    vr::HmdMatrix34_t offsetUniverseCenter;

    // set offsetUniverseCenter to the current angle
    vr::HmdMatrix34_t rotationMatrix;
    utils::initRotationMatrix(
        rotationMatrix,
        1,
        static_cast<float>( m_rotation * k_centidegreesToRadians ) );
    utils::matMul33(
        offsetUniverseCenter, rotationMatrix, m_universeCenterForReset );
    // fill in matrix coordinates for basis universe zero point
    offsetUniverseCenter.m[0][3] = m_universeCenterForReset.m[0][3];
    offsetUniverseCenter.m[1][3] = m_universeCenterForReset.m[1][3];
    offsetUniverseCenter.m[2][3] = m_universeCenterForReset.m[2][3];

    // move offsetUniverseCenter to the current offsets
    offsetUniverseCenter.m[0][3]
        += m_universeCenterForReset.m[0][0] * m_offsetX;
    offsetUniverseCenter.m[1][3]
        += m_universeCenterForReset.m[1][0] * m_offsetX;
    offsetUniverseCenter.m[2][3]
        += m_universeCenterForReset.m[2][0] * m_offsetX;

    offsetUniverseCenter.m[0][3]
        += m_universeCenterForReset.m[0][1] * m_offsetY;
    offsetUniverseCenter.m[1][3]
        += m_universeCenterForReset.m[1][1] * m_offsetY;
    offsetUniverseCenter.m[2][3]
        += m_universeCenterForReset.m[2][1] * m_offsetY;

    offsetUniverseCenter.m[0][3]
        += m_universeCenterForReset.m[0][2] * m_offsetZ;
    offsetUniverseCenter.m[1][3]
        += m_universeCenterForReset.m[1][2] * m_offsetZ;
    offsetUniverseCenter.m[2][3]
        += m_universeCenterForReset.m[2][2] * m_offsetZ;

    // keep the seated origin synced with offsets if in seated mode
    if ( m_trackingUniverse == vr::TrackingUniverseSeated )
    {
        vr::HmdMatrix34_t offsetSeatedCenter = offsetUniverseCenter;

        offsetSeatedCenter.m[1][3] += m_seatedHeight;

        vr::VRChaperoneSetup()->SetWorkingSeatedZeroPoseToRawTrackingPose(
            &offsetSeatedCenter );
    }

    if ( m_adjustChaperone )
    {
        // make a copy of our bounds and
        // reorient relative to new universe center

        float offsetUniverseCenterYaw = std::atan2(
            offsetUniverseCenter.m[0][2], offsetUniverseCenter.m[2][2] );

        vr::HmdQuad_t* updatedBounds
            = new vr::HmdQuad_t[m_collisionBoundsCountForReset];

        // for every quad in the chaperone bounds...
        for ( unsigned quad = 0; quad < m_collisionBoundsCountForReset; quad++ )
        {
            // at every corner in that quad...
            for ( unsigned corner = 0; corner < 4; corner++ )
            {
                // copy the corner's xyz coordinates
                updatedBounds[quad].vCorners[corner].v[0]
                    = m_collisionBoundsForOffset[quad].vCorners[corner].v[0];

                updatedBounds[quad].vCorners[corner].v[1]
                    = m_collisionBoundsForOffset[quad].vCorners[corner].v[1];

                updatedBounds[quad].vCorners[corner].v[2]
                    = m_collisionBoundsForOffset[quad].vCorners[corner].v[2];

                // cancel universe center's xyz offsets to each corner's
                // position and shift over by original center position so that
                // we are mirroring the offset as reflected about the original
                // origin
                updatedBounds[quad].vCorners[corner].v[0]
                    -= offsetUniverseCenter.m[0][3]
                       - m_universeCenterForReset.m[0][3];
                // but don't touch y=0 values to keep floor corners rooted down
                if ( updatedBounds[quad].vCorners[corner].v[1] != 0 )
                {
                    updatedBounds[quad].vCorners[corner].v[1]
                        -= offsetUniverseCenter.m[1][3]
                           - m_universeCenterForReset.m[1][3];
                }
                updatedBounds[quad].vCorners[corner].v[2]
                    -= offsetUniverseCenter.m[2][3]
                       - m_universeCenterForReset.m[2][3];

                // rotate by universe center's yaw
                rotateFloatCoordinates( updatedBounds[quad].vCorners[corner].v,
                                        offsetUniverseCenterYaw );
            }
        }

        // update chaperone working set preview (this does not commit)
        vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(
            updatedBounds, m_collisionBoundsCountForReset );
        delete[] updatedBounds;
    }
    vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(
        &offsetUniverseCenter );

    if ( m_oldStyleMotion )
    {
        vr::VRChaperoneSetup()->CommitWorkingCopy(
            vr::EChaperoneConfigFile_Live );
        m_chaperoneCommitted = true;
    }
    else
    {
        vr::VRChaperoneSetup()->ShowWorkingSetPreview();
        m_chaperoneCommitted = false;
    }

    // loadChaperoneData( false ), false so that we don't load live data, and
    // reference the working set instead.
    if ( m_collisionBoundsCountForReset > 0 )
    {
        parent->chaperoneUtils().loadChaperoneData( false );
    }

    m_oldOffsetX = m_offsetX;
    m_oldOffsetY = m_offsetY;
    m_oldOffsetZ = m_offsetZ;
    m_oldRotation = m_rotation;
}

void MoveCenterTabController::eventLoopTick(
    vr::ETrackingUniverseOrigin universe,
    vr::TrackedDevicePose_t* devicePoses )

{
    // detect if room setup is running
    if ( universe == vr::TrackingUniverseRawAndUncalibrated )
    {
        if ( !m_roomSetupModeDetected )
        {
            m_roomSetupModeDetected = true;

            // give up on any pending zero offsets so they aren't applied in the
            // middle of room setup.
            m_pendingZeroOffsets = false;

            LOG( INFO ) << "room setup ENTRY detected";
            vr::VRChaperoneSetup()->HideWorkingSetPreview();
            reset();
            vr::VRChaperoneSetup()->RevertWorkingCopy();
        }
        setTrackingUniverse( int( universe ) );
        return;
    }

    // detect entering seated mode
    if ( universe == vr::TrackingUniverseSeated && !m_seatedModeDetected )
    {
        m_seatedModeDetected = true;
        LOG( INFO ) << "seated mode ENTRY detected";
    }

    // detect exiting seated mode
    if ( universe == vr::TrackingUniverseStanding && m_seatedModeDetected )
    {
        m_seatedModeDetected = false;
        reset();
        LOG( INFO ) << "seated mode EXIT detected";
    }

    // If we're trying to redifine the origin point, but can't becaues of bad
    // chaperone calibration, we keep trying and hold off on motion features
    // until we get ChaperoneCalibrationState_OK

    // if we were in room setup, but got this far in the loop, universe is no
    // longer raw mode and we detect room setup exit and zeroOffsets() to apply
    // new setup.
    if ( m_pendingZeroOffsets || m_roomSetupModeDetected )
    {
        zeroOffsets();
        // m_roomSetupModeDetected is set to false in zeroOffsets() if it's
        // successful.
    }
    else
    {
        setTrackingUniverse( int( universe ) );

        // get current space rotation in radians
        double angle = m_rotation * k_centidegreesToRadians;

        // hmd rotations stats counting doesn't need to be smooth, so we skip
        // some frames for performance
        if ( m_hmdRotationStatsUpdateCounter >= k_hmdRotationCounterUpdateRate )
        {
            // device pose index 0 is always the hmd
            updateHmdRotationCounter( devicePoses[0], angle );
            m_hmdRotationStatsUpdateCounter = 0;
        }
        else
        {
            m_hmdRotationStatsUpdateCounter++;
        }

        // only update dynamic motion if the dash is closed
        if ( !parent->isDashboardVisible() )
        {
            // detect new dash closed
            if ( m_dashWasOpenPreviousFrame )
            {
                // reset velocity time points on dash closed so we don't factor
                // in the time motion was paused during the open dash
                m_lastDragUpdateTimePoint = std::chrono::steady_clock::now();
                m_lastGravityUpdateTimePoint = std::chrono::steady_clock::now();
            }

            // force chaperone bounds visible if turn or drag settings require
            if ( m_dragBounds
                 && m_activeDragHand != vr::TrackedControllerRole_Invalid )
            {
                vr::VRChaperone()->ForceBoundsVisible( true );
            }
            else if ( m_turnBounds
                      && m_activeTurnHand != vr::TrackedControllerRole_Invalid )
            {
                vr::VRChaperone()->ForceBoundsVisible( true );
            }
            // only set back to default every frame if setting is enabled
            else if ( m_turnBounds || m_dragBounds )
            {
                vr::VRChaperone()->ForceBoundsVisible(
                    parent->m_chaperoneTabController.forceBounds() );
            }

            // Smooth turn motion can cause sim-sickness so we check if the user
            // wants to skip frames to reduce vection. We use the factor squared
            // because of logarithmic human perception.
            if ( m_turnComfortFrameSkipCounter
                 >= ( m_turnComfortFactor * m_turnComfortFactor ) )
            {
                updateHandTurn( devicePoses, angle );
                m_turnComfortFrameSkipCounter = 0;
            }
            else
            {
                m_turnComfortFrameSkipCounter++;
            }

            // Smooth drag motion can cause sim-sickness so we check if the user
            // wants to skip frames to reduce vection. We use the factor squared
            // because of logarithmic human perception.
            if ( m_dragComfortFrameSkipCounter
                 >= ( m_dragComfortFactor * m_dragComfortFactor ) )
            {
                updateHandDrag( devicePoses, angle );
                m_lastDragUpdateTimePoint = std::chrono::steady_clock::now();
                m_dragComfortFrameSkipCounter = 0;
            }
            else
            {
                m_dragComfortFrameSkipCounter++;
            }

            if ( m_gravityActive
                 && m_activeDragHand == vr::TrackedControllerRole_Invalid )
            {
                updateGravity();
                m_lastGravityUpdateTimePoint = std::chrono::steady_clock::now();
            }
            m_dashWasOpenPreviousFrame = false;
        }
        else
        {
            // dash is open this frame
            m_dashWasOpenPreviousFrame = true;
        }
        updateSpace();
    }
}
} // namespace advsettings
