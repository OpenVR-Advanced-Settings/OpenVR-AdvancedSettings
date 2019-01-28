#include "MoveCenterTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"
#include "../utils/Matrix.h"

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
    value = settings->value( "rotateHand", m_rotateHand );
    if ( value.isValid() && !value.isNull() )
    {
        m_rotateHand = value.toBool();
    }
    value = settings->value( "moveShortcutRight", m_moveShortcutRightEnabled );
    if ( value.isValid() && !value.isNull() )
    {
        m_moveShortcutRightEnabled = value.toBool();
    }
    value = settings->value( "moveShortcutLeft", m_moveShortcutLeftEnabled );
    if ( value.isValid() && !value.isNull() )
    {
        m_moveShortcutLeftEnabled = value.toBool();
    }
    value = settings->value( "requireDoubleClick", m_requireDoubleClick );
    if ( value.isValid() && !value.isNull() )
    {
        m_requireDoubleClick = value.toBool();
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
    lastMoveButtonClick[0] = lastMoveButtonClick[1] = clock::now();
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
        double angle = ( value - m_rotation ) * M_PI / 18000.0;

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
        double oldHmdXyz[3]
            = { oldHmdPos.m[0][3], oldHmdPos.m[1][3], oldHmdPos.m[2][3] };
        double newHmdXyz[3]
            = { oldHmdPos.m[0][3], oldHmdPos.m[1][3], oldHmdPos.m[2][3] };

        // Convert oldHmdXyz into un-rotated coordinates.
        double oldAngle = -m_rotation * M_PI / 18000.0;
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
        double finalAngle = m_rotation * M_PI / 18000.0;
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
        m_offsetX += hmdRotDiff[0];
        m_offsetZ += hmdRotDiff[2];
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
            double angle = m_rotation * M_PI / 18000.0;
            double offsetdir = m_adjustChaperone ? -1.0 : 1.0;
            double offset[3] = { offsetdir * m_offsetX,
                                 offsetdir * m_offsetY,
                                 offsetdir * m_offsetZ };
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
    return m_rotateHand;
}

void MoveCenterTabController::setRotateHand( bool value, bool notify )
{
    m_rotateHand = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "rotateHand", m_rotateHand );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit rotateHandChanged( m_rotateHand );
    }
}

bool MoveCenterTabController::moveShortcutRight() const
{
    return m_moveShortcutRightEnabled;
}

void MoveCenterTabController::setMoveShortcutRight( bool value, bool notify )
{
    m_moveShortcutRightEnabled = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "moveShortcutRight", m_moveShortcutRightEnabled );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit moveShortcutRightChanged( m_moveShortcutRightEnabled );
    }
}

bool MoveCenterTabController::moveShortcutLeft() const
{
    return m_moveShortcutLeftEnabled;
}

void MoveCenterTabController::setMoveShortcutLeft( bool value, bool notify )
{
    m_moveShortcutLeftEnabled = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "moveShortcutLeft", m_moveShortcutLeftEnabled );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit moveShortcutLeftChanged( m_moveShortcutLeftEnabled );
    }
}

bool MoveCenterTabController::requireDoubleClick() const
{
    return m_requireDoubleClick;
}

void MoveCenterTabController::setRequireDoubleClick( bool value, bool notify )
{
    m_requireDoubleClick = value;
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "playspaceSettings" );
    settings->setValue( "requireDoubleClick", m_requireDoubleClick );
    settings->endGroup();
    settings->sync();
    if ( notify )
    {
        emit requireDoubleClickChanged( m_requireDoubleClick );
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
        double angle = m_rotation * M_PI / 18000.0;
        double offset[3] = { value, 0, 0 };
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
        double angle = m_rotation * M_PI / 18000.0;
        double offset[3] = { 0, 0, value };
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
        static_cast<float>( -m_rotation * M_PI / 18000.0 ),
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

// START of move bindings:

void MoveCenterTabController::leftHandPlayspaceMove( bool leftHandMoveActive )
{
    if ( m_moveShortcutLeftEnabled )
    {
        // cancel if other hand is active in override
        if ( !m_overrideRightHandMovePressed )
        {
            // detect new press
            if ( leftHandMoveActive && !m_leftHandMovePressed )
            {
                m_activeMoveHand = vr::TrackedControllerRole_LeftHand;
            }
            // detect new release
            else if ( !leftHandMoveActive && m_leftHandMovePressed )
            {
                // check overrides
                // pass back to right hand?
                if ( m_overrideRightHandMovePressed )
                {
                    m_activeMoveHand = vr::TrackedControllerRole_RightHand;
                }
                // override of left hand still active?
                else if ( m_overrideLeftHandMovePressed )
                {
                    m_activeMoveHand = vr::TrackedControllerRole_LeftHand;
                }
                // check non-override right hand action
                if ( m_rightHandMovePressed )
                {
                    m_activeMoveHand = vr::TrackedControllerRole_RightHand;
                }
                // nothing else is pressed, deactivate move
                else
                {
                    m_activeMoveHand = vr::TrackedControllerRole_Invalid;
                }
            }
        }
        m_leftHandMovePressed = leftHandMoveActive;
    }
}

void MoveCenterTabController::rightHandPlayspaceMove( bool rightHandMoveActive )
{
    if ( m_moveShortcutRightEnabled )
    {
        // cancel if other hand is active in override
        if ( !m_overrideLeftHandMovePressed )
        {
            // detect new press
            if ( rightHandMoveActive && !m_rightHandMovePressed )
            {
                m_activeMoveHand = vr::TrackedControllerRole_RightHand;
            }
            // detect new release
            else if ( !rightHandMoveActive && m_rightHandMovePressed )
            {
                // check overrides
                // pass back to left hand?
                if ( m_overrideLeftHandMovePressed )
                {
                    m_activeMoveHand = vr::TrackedControllerRole_LeftHand;
                }
                // override of right hand still active?
                else if ( m_overrideRightHandMovePressed )
                {
                    m_activeMoveHand = vr::TrackedControllerRole_RightHand;
                }
                // check non-override left hand action
                if ( m_leftHandMovePressed )
                {
                    m_activeMoveHand = vr::TrackedControllerRole_LeftHand;
                }
                // nothing else is pressed, deactivate move
                else
                {
                    m_activeMoveHand = vr::TrackedControllerRole_Invalid;
                }
            }
        }
        m_rightHandMovePressed = rightHandMoveActive;
    }
}

void MoveCenterTabController::optionalOverrideLeftHandPlayspaceMove(
    bool overrideLeftHandMoveActive )
{
    if ( m_moveShortcutLeftEnabled )
    {
        // detect new press
        if ( overrideLeftHandMoveActive && !m_overrideLeftHandMovePressed )
        {
            m_activeMoveHand = vr::TrackedControllerRole_LeftHand;
            // stop any active rotate because we're in override mode
            m_activeRotateHand = vr::TrackedControllerRole_Invalid;
        }
        // detect new release
        else if ( !overrideLeftHandMoveActive && m_overrideLeftHandMovePressed )
        {
            // check if we should pass back to other hand
            // give priority to override action
            if ( m_overrideRightHandMovePressed )
            {
                m_activeMoveHand = vr::TrackedControllerRole_RightHand;
            }
            // otherwise check normal action
            else if ( m_rightHandMovePressed )
            {
                m_activeMoveHand = vr::TrackedControllerRole_RightHand;
            }
            // check if we should fall back to non-override left hand
            else if ( m_leftHandMovePressed )
            {
                m_activeMoveHand = vr::TrackedControllerRole_LeftHand;
            }
            // nothing else is pressed, deactivate move
            else
            {
                m_activeMoveHand = vr::TrackedControllerRole_Invalid;
            }
        }
        m_overrideLeftHandMovePressed = overrideLeftHandMoveActive;
    }
}

void MoveCenterTabController::optionalOverrideRightHandPlayspaceMove(
    bool overrideRightHandMoveActive )
{
    if ( m_moveShortcutRightEnabled )
    {
        // detect new press
        if ( overrideRightHandMoveActive && !m_overrideRightHandMovePressed )
        {
            m_activeMoveHand = vr::TrackedControllerRole_RightHand;
            // stop any active rotate because we're in override mode
            m_activeRotateHand = vr::TrackedControllerRole_Invalid;
        }
        // detect new release
        else if ( !overrideRightHandMoveActive
                  && m_overrideRightHandMovePressed )
        {
            // check if we should pass back to other hand
            // give priority to override action
            if ( m_overrideLeftHandMovePressed )
            {
                m_activeMoveHand = vr::TrackedControllerRole_LeftHand;
            }
            // otherwise check normal action
            else if ( m_leftHandMovePressed )
            {
                m_activeMoveHand = vr::TrackedControllerRole_LeftHand;
            }
            // check if we should fall back to non-override left hand
            else if ( m_rightHandMovePressed )
            {
                m_activeMoveHand = vr::TrackedControllerRole_RightHand;
            }
            // nothing else is pressed, deactivate move
            else
            {
                m_activeMoveHand = vr::TrackedControllerRole_Invalid;
            }
        }
        m_overrideRightHandMovePressed = overrideRightHandMoveActive;
    }
}

// END of move bindings

// START of rotate bindgins

void MoveCenterTabController::leftHandPlayspaceRotate(
    bool leftHandRotateActive )
{
    if ( m_rotateHand )
    {
        // cancel if other hand is active in override
        if ( !m_overrideRightHandRotatePressed )
        {
            // detect new press
            if ( leftHandRotateActive && !m_leftHandRotatePressed )
            {
                m_activeRotateHand = vr::TrackedControllerRole_LeftHand;
            }
            // detect new release
            else if ( !leftHandRotateActive && m_leftHandRotatePressed )
            {
                // check overrides
                // pass back to right hand?
                if ( m_overrideRightHandRotatePressed )
                {
                    m_activeRotateHand = vr::TrackedControllerRole_RightHand;
                }
                // override of left hand still active?
                else if ( m_overrideLeftHandRotatePressed )
                {
                    m_activeRotateHand = vr::TrackedControllerRole_LeftHand;
                }
                // check non-override right hand action
                if ( m_rightHandRotatePressed )
                {
                    m_activeRotateHand = vr::TrackedControllerRole_RightHand;
                }
                // nothing else is pressed, deactivate rotate
                else
                {
                    m_activeRotateHand = vr::TrackedControllerRole_Invalid;
                }
            }
        }
        m_leftHandRotatePressed = leftHandRotateActive;
    }
}

void MoveCenterTabController::rightHandPlayspaceRotate(
    bool rightHandRotateActive )
{
    if ( m_rotateHand )
    {
        // cancel if other hand is active in override
        if ( !m_overrideLeftHandRotatePressed )
        {
            // detect new press
            if ( rightHandRotateActive && !m_rightHandRotatePressed )
            {
                m_activeRotateHand = vr::TrackedControllerRole_RightHand;
            }
            // detect new release
            else if ( !rightHandRotateActive && m_rightHandRotatePressed )
            {
                // check overrides
                // pass back to left hand?
                if ( m_overrideLeftHandRotatePressed )
                {
                    m_activeRotateHand = vr::TrackedControllerRole_LeftHand;
                }
                // override of right hand still active?
                else if ( m_overrideRightHandRotatePressed )
                {
                    m_activeRotateHand = vr::TrackedControllerRole_RightHand;
                }
                // check non-override left hand action
                if ( m_leftHandRotatePressed )
                {
                    m_activeRotateHand = vr::TrackedControllerRole_LeftHand;
                }
                // nothing else is pressed, deactivate rotate
                else
                {
                    m_activeRotateHand = vr::TrackedControllerRole_Invalid;
                }
            }
        }
        m_rightHandRotatePressed = rightHandRotateActive;
    }
}

void MoveCenterTabController::optionalOverrideLeftHandPlayspaceRotate(
    bool overrideLeftHandRotateActive )
{
    if ( m_rotateHand )
    {
        // detect new press
        if ( overrideLeftHandRotateActive && !m_overrideLeftHandRotatePressed )
        {
            m_activeRotateHand = vr::TrackedControllerRole_LeftHand;
            // stop any active move because we're in override mode
            m_activeMoveHand = vr::TrackedControllerRole_Invalid;
        }
        // detect new release
        else if ( !overrideLeftHandRotateActive
                  && m_overrideLeftHandRotatePressed )
        {
            // check if we should pass back to other hand
            // give priority to override action
            if ( m_overrideRightHandRotatePressed )
            {
                m_activeRotateHand = vr::TrackedControllerRole_RightHand;
            }
            // otherwise check normal action
            else if ( m_rightHandRotatePressed )
            {
                m_activeRotateHand = vr::TrackedControllerRole_RightHand;
            }
            // check if we should fall back to non-override left hand
            else if ( m_leftHandRotatePressed )
            {
                m_activeRotateHand = vr::TrackedControllerRole_LeftHand;
            }
            // nothing else is pressed, deactivate rotate
            else
            {
                m_activeRotateHand = vr::TrackedControllerRole_Invalid;
            }
        }
        m_overrideLeftHandRotatePressed = overrideLeftHandRotateActive;
    }
}

void MoveCenterTabController::optionalOverrideRightHandPlayspaceRotate(
    bool overrideRightHandRotateActive )
{
    if ( m_rotateHand )
    {
        // detect new press
        if ( overrideRightHandRotateActive
             && !m_overrideRightHandRotatePressed )
        {
            m_activeRotateHand = vr::TrackedControllerRole_RightHand;
            // stop any active move because we're in override mode
            m_activeMoveHand = vr::TrackedControllerRole_Invalid;
        }
        // detect new release
        else if ( !overrideRightHandRotateActive
                  && m_overrideRightHandRotatePressed )
        {
            // check if we should pass back to other hand
            // give priority to override action
            if ( m_overrideLeftHandRotatePressed )
            {
                m_activeRotateHand = vr::TrackedControllerRole_LeftHand;
            }
            // otherwise check normal action
            else if ( m_leftHandRotatePressed )
            {
                m_activeRotateHand = vr::TrackedControllerRole_LeftHand;
            }
            // check if we should fall back to non-override left hand
            else if ( m_rightHandRotatePressed )
            {
                m_activeRotateHand = vr::TrackedControllerRole_RightHand;
            }
            // nothing else is pressed, deactivate rotate
            else
            {
                m_activeRotateHand = vr::TrackedControllerRole_Invalid;
            }
        }
        m_overrideRightHandRotatePressed = overrideRightHandRotateActive;
    }
}

// END of rotate bindings.

/*
bool isMoveShortCutPressed( vr::ETrackedControllerRole hand )
{
    auto handId
        = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole( hand );
    if ( handId == vr::k_unTrackedDeviceIndexInvalid
         || handId >= vr::k_unMaxTrackedDeviceCount )
    {
        return false;
    }

    vr::VRControllerState_t state;
    if ( vr::VRSystem()->GetControllerState(
             handId, &state, sizeof( vr::VRControllerState_t ) ) )
    {
        return state.ulButtonPressed
               & vr::ButtonMaskFromId( vr::k_EButton_ApplicationMenu );
    }

    return false;
}
*/

/*
vr::ETrackedControllerRole MoveCenterTabController::getMoveShortcutHand()
{
    auto activeHand = m_activeMoveController;

    bool rightPressed
        = m_moveShortcutRightEnabled
          && isMoveShortCutPressed( vr::TrackedControllerRole_RightHand );
    bool leftPressed
        = m_moveShortcutLeftEnabled
          && isMoveShortCutPressed( vr::TrackedControllerRole_LeftHand );
    bool checkDoubleClick = m_requireDoubleClick
                            && activeHand == vr::TrackedControllerRole_Invalid;

    auto now = clock::now();
    // if we start pressing the shortcut on a controller we set the active one
    // to it
    if ( rightPressed && !m_moveShortcutRightPressed )
    {
        auto elapsed
            = duration_cast<milliseconds>( now - lastMoveButtonClick[0] )
                  .count();
        if ( !checkDoubleClick || elapsed < 250 )
        {
            activeHand = vr::TrackedControllerRole_RightHand;
        }
        lastMoveButtonClick[0] = now;
    }
    if ( leftPressed && !m_moveShortcutLeftPressed )
    {
        auto elapsed
            = duration_cast<milliseconds>( now - lastMoveButtonClick[1] )
                  .count();
        if ( !checkDoubleClick || elapsed < 250 )
        {
            activeHand = vr::TrackedControllerRole_LeftHand;
        }
        lastMoveButtonClick[1] = now;
    }

    // if we let down of a shortcut we set the active hand to any remaining
    // pressed down hand
    if ( !rightPressed && m_moveShortcutRightPressed )
    {
        activeHand = leftPressed ? vr::TrackedControllerRole_LeftHand
                                 : vr::TrackedControllerRole_Invalid;
    }
    if ( !leftPressed && m_moveShortcutLeftPressed )
    {
        activeHand = rightPressed ? vr::TrackedControllerRole_RightHand
                                  : vr::TrackedControllerRole_Invalid;
    }

    if ( !leftPressed && !rightPressed )
    {
        activeHand = vr::TrackedControllerRole_Invalid;
        if ( m_activeMoveController != vr::TrackedControllerRole_Invalid )
        {
            // grace period while switching hands
            lastMoveButtonClick[0] = lastMoveButtonClick[1] = now;
        }
    }

    m_activeMoveController = activeHand;
    m_moveShortcutRightPressed = rightPressed;
    m_moveShortcutLeftPressed = leftPressed;

    return activeHand;
}
*/

void MoveCenterTabController::eventLoopTick(
    vr::ETrackingUniverseOrigin universe,
    vr::TrackedDevicePose_t* devicePoses )
{
    // 149 was chosen because it's a prime number (to reduce overlap of
    // simultaneous settings updates).
    // Actual rate of updates is 149 * vsync (~11ms)
    // Values chosen based on update speed priority
    // Values in other tabs are as follows (avoid using same values):
    // AccessibiltyTabController: 151
    // AudioTabController: 89
    // ChaperoneTabController: 101
    // ReviveTabController: 139
    // SettingsTabController: 157
    // SteamVRTabController: 97
    // UtilitiesTabController: 19
    if ( settingsUpdateCounter >= 149 )
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

    double angle = m_rotation * M_PI / 18000.0;

    // START of hmd rotation stats tracking:
    // Check if hmd is tracking ok and do everything
    if ( devicePoses[0].bPoseIsValid
         && devicePoses[0].eTrackingResult == vr::TrackingResult_Running_OK )
    {
        // Get hmd pose matrix (in rotated coordinates)
        vr::HmdMatrix34_t hmdMatrix = devicePoses[0].mDeviceToAbsoluteTracking;

        // Set up (un)rotation matrix
        vr::HmdMatrix34_t hmdMatrixRotMat;
        vr::HmdMatrix34_t hmdMatrixAbsolute;
        utils::initRotationMatrix(
            hmdMatrixRotMat, 1, static_cast<float>( angle ) );

        // Get hmdMatrixAbsolute in un-rotated coordinates.
        utils::matMul33( hmdMatrixAbsolute, hmdMatrixRotMat, hmdMatrix );

        // Convert pose matrix to quaternion
        hmdQuaternion = utils::getQuaternion( hmdMatrixAbsolute );

        // Get rotation change of hmd
        // Checking if set to -1000.0 placeholder for invalid hmd pose.
        if ( lastHmdQuaternion.w < -900.0 )
        {
            lastHmdQuaternion = hmdQuaternion;
        }

        else
        {
            // Construct a quaternion representing difference between old
            // hmd pose and new hmd pose.
            vr::HmdQuaternion_t hmdDiffQuaternion = utils::multiplyQuaternion(
                hmdQuaternion,
                utils::quaternionConjugate( lastHmdQuaternion ) );

            // Calculate yaw from quaternion.
            double hmdYawDiff = atan2(
                2.0
                    * ( hmdDiffQuaternion.y * hmdDiffQuaternion.w
                        + hmdDiffQuaternion.x * hmdDiffQuaternion.z ),
                -1.0
                    + 2.0
                          * ( hmdDiffQuaternion.w * hmdDiffQuaternion.w
                              + hmdDiffQuaternion.x * hmdDiffQuaternion.x ) );

            // Apply yaw difference to m_hmdYawTotal.
            m_hmdYawTotal += hmdYawDiff;
            lastHmdQuaternion = hmdQuaternion;
        }

        // end check if hmdpose is ok
    }
    else
    {
        // set lastHmdQuaternion.w to placeholder -1000.0 for invalid
        lastHmdQuaternion.w = -1000.0;
    } // END of hmd rotation stats tracking

    /*
        if ( m_activeMoveHand == vr::TrackedControllerRole_Invalid )
        {
            emit offsetXChanged( m_offsetX );
            emit offsetYChanged( m_offsetY );
            emit offsetZChanged( m_offsetZ );

            // Set lastHandQuaternion.w to placeholder value for invalid
       (-1000.0)
            // when we release move shortcut.
            if ( m_rotateHand )
            {
                lastHandQuaternion.w = -1000.0;
            }
            m_lastMoveHand = m_activeMoveHand;
            return;
        }
    */
    auto moveHandId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
        m_activeMoveHand );
    auto rotateHandId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
        m_activeRotateHand );

    // START of hand move
    if ( m_activeMoveHand == vr::TrackedControllerRole_Invalid
         || moveHandId == vr::k_unTrackedDeviceIndexInvalid
         || moveHandId >= vr::k_unMaxTrackedDeviceCount )
    {
        if ( m_lastMoveHand != vr::TrackedControllerRole_Invalid )
        {
            emit offsetXChanged( m_offsetX );
            emit offsetYChanged( m_offsetY );
            emit offsetZChanged( m_offsetZ );
        }
        m_lastMoveHand = m_activeMoveHand;
    }
    else
    {
        vr::TrackedDevicePose_t* movePose = devicePoses + moveHandId;
        if ( !movePose->bPoseIsValid || !movePose->bDeviceIsConnected
             || movePose->eTrackingResult != vr::TrackingResult_Running_OK )
        {
            m_lastMoveHand = m_activeMoveHand;
        }
        else
        {
            double relativeControllerPosition[]
                = { movePose->mDeviceToAbsoluteTracking.m[0][3],
                    movePose->mDeviceToAbsoluteTracking.m[1][3],
                    movePose->mDeviceToAbsoluteTracking.m[2][3] };

            rotateCoordinates( relativeControllerPosition, -angle );
            float absoluteControllerPosition[] = {
                static_cast<float>( relativeControllerPosition[0] ) + m_offsetX,
                static_cast<float>( relativeControllerPosition[1] ) + m_offsetY,
                static_cast<float>( relativeControllerPosition[2] ) + m_offsetZ,
            };

            if ( m_lastMoveHand == m_activeMoveHand )
            {
                double diff[3] = {
                    absoluteControllerPosition[0] - m_lastControllerPosition[0],
                    absoluteControllerPosition[1] - m_lastControllerPosition[1],
                    absoluteControllerPosition[2] - m_lastControllerPosition[2],
                };

                // offset is un-rotated coordinates

                // prevents UI from updating if axis movement is locked
                if ( !m_lockXToggle )
                {
                    m_offsetX += diff[0];
                }
                if ( !m_lockYToggle )
                {
                    m_offsetY += diff[1];
                }
                if ( !m_lockZToggle )
                {
                    m_offsetZ += diff[2];
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
                if ( diffFloat[0] != 0 || diffFloat[1] != 0
                     || diffFloat[2] != 0 )
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
            m_lastMoveHand = m_activeMoveHand;
        }
    } // END of hand move

    // START of hand rotation

    if ( m_activeRotateHand == vr::TrackedControllerRole_Invalid
         || rotateHandId == vr::k_unTrackedDeviceIndexInvalid
         || rotateHandId >= vr::k_unMaxTrackedDeviceCount )
    {
        if ( m_lastRotateHand != vr::TrackedControllerRole_Invalid )
        {
            // Set lastHandQuaternion.w to placeholder value for invalid
            // (-1000.0)
            lastHandQuaternion.w = -1000.0;
        }
        m_lastRotateHand = m_activeRotateHand;
    }
    else
    {
        vr::TrackedDevicePose_t* rotatePose = devicePoses + rotateHandId;
        if ( !rotatePose->bPoseIsValid || !rotatePose->bDeviceIsConnected
             || rotatePose->eTrackingResult != vr::TrackingResult_Running_OK )
        {
            m_lastRotateHand = m_activeRotateHand;
        }
        else
        {
            // Get hand's rotation.
            // handMatrix is in rotated coordinates.
            vr::HmdMatrix34_t handMatrix
                = rotatePose->mDeviceToAbsoluteTracking;

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
            handQuaternion = utils::getQuaternion( handMatrixAbsolute );

            if ( m_lastRotateHand == m_activeRotateHand )
            {
                // Get rotation change of hand.
                // Checking if set to -1000.0 placeholder for invalid hand.
                if ( lastHandQuaternion.w < -900.0 )
                {
                    lastHandQuaternion = handQuaternion;
                }

                else
                {
                    // Construct a quaternion representing difference
                    // between old hand and new hand.
                    vr::HmdQuaternion_t handDiffQuaternion
                        = utils::multiplyQuaternion(
                            handQuaternion,
                            utils::quaternionConjugate( lastHandQuaternion ) );

                    // Calculate yaw from quaternion.
                    double handYawDiff = atan2(
                        2.0
                            * ( handDiffQuaternion.y * handDiffQuaternion.w
                                + handDiffQuaternion.x * handDiffQuaternion.z ),
                        -1.0
                            + 2.0
                                  * ( handDiffQuaternion.w
                                          * handDiffQuaternion.w
                                      + handDiffQuaternion.x
                                            * handDiffQuaternion.x ) );

                    int newRotationAngleDeg = static_cast<int>(
                        round( handYawDiff * 18000.0 / M_PI ) + m_rotation );

                    // Keep angle within -18000 ~ 18000
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
            lastHandQuaternion = handQuaternion;
            m_lastRotateHand = m_activeRotateHand;
        }
    } // END of hand rotation
}
} // namespace advsettings
