#include "MoveCenterTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"
#include "../utils/Matrix.h"
#include "../utils/quaternion.h"

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

// application namespace
namespace advsettings
{
using std::chrono::duration_cast;
using std::chrono::milliseconds;
typedef std::chrono::system_clock clock;

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
    value = settings->value( "rotateHand", m_settingsHandTurningEnabled );
    if ( value.isValid() && !value.isNull() )
    {
        m_settingsHandTurningEnabled = value.toBool();
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
    settings->endGroup();
}

void MoveCenterTabController::initStage2( OverlayController* var_parent,
                                          QQuickWindow* var_widget )
{
    this->parent = var_parent;
    this->widget = var_widget;
}

int MoveCenterTabController::trackingUniverse() const
{
    return static_cast<int>( m_trackingUniverse );
}

void MoveCenterTabController::setTrackingUniverse( int value, bool notify )
{
    if ( m_trackingUniverse != value )
    {
        reset();
        m_trackingUniverse = value;
        if ( notify )
        {
            emit trackingUniverseChanged( m_trackingUniverse );
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
        double angle = ( value - m_rotation ) * k_centidegreesToRadians;

        // Revert now because we don't commit in RotateUniverseCenter and
        // AddOffsetToUniverseCenter. We do this so rotation and offset can
        // happen in one go, avoiding positional judder.
        vr::VRChaperoneSetup()->RevertWorkingCopy();

        // Get hmd pose matrix.
        vr::TrackedDevicePose_t
            devicePosesForRot[vr::k_unMaxTrackedDeviceCount];
        vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(
            vr::TrackingUniverseStanding,
            0.0f,
            devicePosesForRot,
            vr::k_unMaxTrackedDeviceCount );
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

        // Rotate the tracking univese center without committing.
        parent->RotateUniverseCenter(
            vr::TrackingUniverseOrigin( m_trackingUniverse ),
            static_cast<float>( angle ),
            m_adjustChaperone,
            false );

        m_rotation = value;
        if ( notify )
        {
            emit rotationChanged( m_rotation );
        }

        // Get rotated offset to apply to universe center.
        // We use rotated coordinates here because we have already applied
        // RotateUniverseCenter. This will be the final offset ready to apply,
        // so it must match the current universe axis rotation.
        double finalAngle = m_rotation * k_centidegreesToRadians;
        double finalHmdRotDiff[3] = { hmdRotDiff[0], 0, hmdRotDiff[2] };
        rotateCoordinates( finalHmdRotDiff, finalAngle );

        // We're done with calculations now so we can down-cast the double
        // values to float for compatilibilty with openvr format
        float finalHmdRotDiffFloat[3]
            = { static_cast<float>( finalHmdRotDiff[0] ),
                static_cast<float>( finalHmdRotDiff[1] ),
                static_cast<float>( finalHmdRotDiff[2] ) };

        // Apply the offset (in rotated coordinates) without commit.
        // We still can't commit yet because it would call
        // vr::VRChaperoneSetup()->RevertWorkingCopy() and we'd lose our
        // uncommitted RotateUniverseCenter.
        parent->AddOffsetToUniverseCenter(
            vr::TrackingUniverseOrigin( m_trackingUniverse ),
            finalHmdRotDiffFloat,
            m_adjustChaperone,
            false );

        // Commit here because we didn't in RotateUniverseCenter and
        // AddOffsetToUniverseCenter to combine into one go.
        vr::VRChaperoneSetup()->CommitWorkingCopy(
            vr::EChaperoneConfigFile_Live );

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

bool MoveCenterTabController::adjustChaperone() const
{
    return m_adjustChaperone;
}

void MoveCenterTabController::setAdjustChaperone( bool value, bool notify )
{
    if ( m_adjustChaperone != value )
    {
        m_adjustChaperone = value;
        if ( m_trackingUniverse == vr::TrackingUniverseStanding )
        {
            double angle = m_rotation * k_centidegreesToRadians;
            double offsetdir = m_adjustChaperone ? -1.0 : 1.0;
            double offset[3] = { offsetdir * static_cast<double>( m_offsetX ),
                                 offsetdir * static_cast<double>( m_offsetY ),
                                 offsetdir * static_cast<double>( m_offsetZ ) };
            rotateCoordinates( offset, angle );

            // We're done with calculations so down-cast to float for
            // compatibility with openvr format

            float offsetFloat[3] = { static_cast<float>( offset[0] ),
                                     static_cast<float>( offset[1] ),
                                     static_cast<float>( offset[2] ) };

            parent->AddOffsetToCollisionBounds( offsetFloat );
        }
        auto settings = OverlayController::appSettings();
        settings->beginGroup( "playspaceSettings" );
        settings->setValue( "adjustChaperone", m_adjustChaperone );
        settings->endGroup();
        settings->sync();
        if ( notify )
        {
            emit adjustChaperoneChanged( m_adjustChaperone );
        }
    }
}

bool MoveCenterTabController::rotateHand() const
{
    return m_settingsHandTurningEnabled;
}

void MoveCenterTabController::setRotateHand( bool value, bool notify )
{
    m_settingsHandTurningEnabled = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "rotateHand", m_settingsHandTurningEnabled );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit rotateHandChanged( m_settingsHandTurningEnabled );
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

void MoveCenterTabController::modOffsetX( float value, bool notify )
{
    // TODO ? possible issue with locking position this way
    if ( !m_lockXToggle )
    {
        double angle = m_rotation * k_centidegreesToRadians;
        double offset[3] = { static_cast<double>( value ), 0, 0 };
        rotateCoordinates( offset, angle );
        float offsetFloat[3] = { static_cast<float>( offset[0] ),
                                 static_cast<float>( offset[1] ),
                                 static_cast<float>( offset[2] ) };
        parent->AddOffsetToUniverseCenter(
            vr::TrackingUniverseOrigin( m_trackingUniverse ),
            offsetFloat,
            m_adjustChaperone );
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
        parent->AddOffsetToUniverseCenter(
            vr::TrackingUniverseOrigin( m_trackingUniverse ),
            1,
            value,
            m_adjustChaperone );
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
        double angle = m_rotation * k_centidegreesToRadians;
        double offset[3] = { 0, 0, static_cast<double>( value ) };
        rotateCoordinates( offset, angle );
        float offsetFloat[3] = { static_cast<float>( offset[0] ),
                                 static_cast<float>( offset[1] ),
                                 static_cast<float>( offset[2] ) };
        parent->AddOffsetToUniverseCenter(
            vr::TrackingUniverseOrigin( m_trackingUniverse ),
            offsetFloat,
            m_adjustChaperone );
        m_offsetZ += value;
        if ( notify )
        {
            emit offsetZChanged( m_offsetZ );
        }
    }
}

void MoveCenterTabController::reset()
{
    vr::VRChaperoneSetup()->RevertWorkingCopy();
    parent->RotateUniverseCenter(
        vr::TrackingUniverseOrigin( m_trackingUniverse ),
        static_cast<float>( -m_rotation * k_centidegreesToRadians ),
        m_adjustChaperone,
        false );
    float offset[3] = { -m_offsetX, -m_offsetY, -m_offsetZ };
    parent->AddOffsetToUniverseCenter(
        vr::TrackingUniverseOrigin( m_trackingUniverse ),
        offset,
        m_adjustChaperone,
        false );
    vr::VRChaperoneSetup()->CommitWorkingCopy( vr::EChaperoneConfigFile_Live );
    m_offsetX = 0.0f;
    m_offsetY = 0.0f;
    m_offsetZ = 0.0f;
    m_rotation = 0;
    emit offsetXChanged( m_offsetX );
    emit offsetYChanged( m_offsetY );
    emit offsetZChanged( m_offsetZ );
    emit rotationChanged( m_rotation );
}

void MoveCenterTabController::zeroOffsets()
{
    m_offsetX = 0.0f;
    m_offsetY = 0.0f;
    m_offsetZ = 0.0f;
    m_rotation = 0;
    emit offsetXChanged( m_offsetX );
    emit offsetYChanged( m_offsetY );
    emit offsetZChanged( m_offsetZ );
    emit rotationChanged( m_rotation );
}

double MoveCenterTabController::getHmdYawTotal()
{
    return m_hmdYawTotal;
}

void MoveCenterTabController::resetHmdYawTotal()
{
    m_hmdYawTotal = 0.0;
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
            // override of left hand still active?
            else if ( m_overrideLeftHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            }
            // check non-override right hand action
            if ( m_rightHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_RightHand;
            }
            // nothing else is pressed, deactivate move
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
            // override of right hand still active?
            else if ( m_overrideRightHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_RightHand;
            }
            // check non-override left hand action
            if ( m_leftHandDragPressed )
            {
                m_activeDragHand = vr::TrackedControllerRole_LeftHand;
            }
            // nothing else is pressed, deactivate move
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
        // otherwise check normal action
        else if ( m_rightHandDragPressed )
        {
            m_activeDragHand = vr::TrackedControllerRole_RightHand;
        }
        // check if we should fall back to non-override left hand
        else if ( m_leftHandDragPressed )
        {
            m_activeDragHand = vr::TrackedControllerRole_LeftHand;
        }
        // nothing else is pressed, deactivate move
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
        // otherwise check normal action
        else if ( m_leftHandDragPressed )
        {
            m_activeDragHand = vr::TrackedControllerRole_LeftHand;
        }
        // check if we should fall back to non-override left hand
        else if ( m_rightHandDragPressed )
        {
            m_activeDragHand = vr::TrackedControllerRole_RightHand;
        }
        // nothing else is pressed, deactivate move
        else
        {
            m_activeDragHand = vr::TrackedControllerRole_Invalid;
        }
    }
    m_overrideRightHandDragPressed = overrideRightHandDragActive;
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
        // check if we should fall back to non-override left hand
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

void MoveCenterTabController::swapSpaceDragToLeftHandOverride(
    bool swapDragToLeftHand )
{
    // temp stuff for compliner warnings
    if ( !swapDragToLeftHand )
    {
        return;
    }
    // TODO STUFF
}

void MoveCenterTabController::swapSpaceDragToRightHandOverride(
    bool swapDragToRightHand )
{
    // temp stuff for compliner warnings
    if ( !swapDragToRightHand )
    {
        return;
    }
    // TODO STUFF
}

void MoveCenterTabController::gravityToggle( bool gravityToggleJustPressed )
{
    // temp stuff for compliner warnings
    if ( !gravityToggleJustPressed )
    {
        return;
    }
    // TODO STUFF
}

void MoveCenterTabController::heightToggle( bool heightToggleJustPressed )
{
    // temp stuff for compliner warnings
    if ( !heightToggleJustPressed )
    {
        return;
    }
    // TODO STUFF
}

void MoveCenterTabController::resetOffsets( bool resetOffsetsJustPressed )
{
    // temp stuff for compliner warnings
    if ( !resetOffsetsJustPressed )
    {
        return;
    }
    // TODO STUFF
}

void MoveCenterTabController::snapTurnLeft( bool snapTurnLeftJustPressed )
{
    // temp stuff for compliner warnings
    if ( !snapTurnLeftJustPressed )
    {
        return;
    }
    // TODO STUFF
}

void MoveCenterTabController::snapTurnRight( bool snapTurnRightJustPressed )
{
    // temp stuff for compliner warnings
    if ( !snapTurnRightJustPressed )
    {
        return;
    }
    // TODO STUFF
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
    m_hmdQuaternion = utils::quaternionFromHmdMatrix34( hmdMatrixAbsolute );

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
    vr::HmdQuaternion_t hmdDiffQuaternion = utils::quaternionMultiply(
        m_hmdQuaternion, utils::quaternionConjugate( m_lastHmdQuaternion ) );

    // Calculate yaw from quaternion.
    double hmdYawDiff = utils::quaternionGetYaw( hmdDiffQuaternion );

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
        if ( m_lastMoveHand != vr::TrackedControllerRole_Invalid )
        {
            emit offsetXChanged( m_offsetX );
            emit offsetYChanged( m_offsetY );
            emit offsetZChanged( m_offsetZ );
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

        // prevents UI from updating if axis movement is locked
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

        rotateCoordinates( diff, angle );

        // Done calculating rotation so we down-cast double to float for
        // openvr format
        float diffFloat[3] = { static_cast<float>( diff[0] ),
                               static_cast<float>( diff[1] ),
                               static_cast<float>( diff[2] ) };
        // If locked removes movement
        if ( m_lockXToggle )
        {
            diffFloat[0] = 0;
        }
        if ( m_lockYToggle )
        {
            diffFloat[1] = 0;
        }
        if ( m_lockZToggle )
        {
            diffFloat[2] = 0;
        }

        // Check if diffFloat is anything before comitting.
        if ( diffFloat[0] != 0 || diffFloat[1] != 0 || diffFloat[2] != 0 )
        {
            parent->AddOffsetToUniverseCenter(
                vr::TrackingUniverseOrigin( m_trackingUniverse ),
                diffFloat,
                m_adjustChaperone );
        }
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
    m_handQuaternion = utils::quaternionFromHmdMatrix34( handMatrixAbsolute );

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
            vr::HmdQuaternion_t handDiffQuaternion = utils::quaternionMultiply(
                m_handQuaternion,
                utils::quaternionConjugate( m_lastHandQuaternion ) );

            // Calculate yaw from quaternion.
            double handYawDiff = utils::quaternionGetYaw( handDiffQuaternion );

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

void MoveCenterTabController::eventLoopTick(
    vr::ETrackingUniverseOrigin universe,
    vr::TrackedDevicePose_t* devicePoses )

{
    if ( settingsUpdateCounter >= k_moveCenterSettingsUpdateCounter )
    {
        if ( parent->isDashboardVisible() )
        {
            setTrackingUniverse( int( universe ) );
        }
        settingsUpdateCounter = 0;
    }
    else
    {
        settingsUpdateCounter++;
    }

    // get current space rotation in radians
    double angle = m_rotation * k_centidegreesToRadians;

    // hmd rotations stats counting doesn't need to be smooth, so we skip some
    // frames for performance
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

    // Smooth drag motion can cause sim-sickness so we check if the user wants
    // to skip frames to reduce vection. We use the factor squared because of
    // logarithmic human perception.
    if ( m_dragComfortFrameSkipCounter
         >= ( m_dragComfortFactor * m_dragComfortFactor ) )
    {
        updateHandDrag( devicePoses, angle );
        m_dragComfortFrameSkipCounter = 0;
    }
    else
    {
        m_dragComfortFrameSkipCounter++;
    }

    // Smooth turn motion can cause sim-sickness so we check if the user wants
    // to skip frames to reduce vection. We use the factor squared because of
    // logarithmic human perception.
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
}
} // namespace advsettings
