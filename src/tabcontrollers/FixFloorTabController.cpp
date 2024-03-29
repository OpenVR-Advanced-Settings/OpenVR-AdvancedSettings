#include "FixFloorTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings
{
void FixFloorTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
}

void FixFloorTabController::dashboardLoopTick(
    vr::TrackedDevicePose_t* devicePoses )
{
    if ( state > 0 )
    {
        if ( measurementCount == 0 )
        {
            // Get Controller ids for left/right hand
            auto leftId
                = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
                    vr::TrackedControllerRole_LeftHand );
            if ( leftId == vr::k_unTrackedDeviceIndexInvalid )
            {
                statusMessage = "No left controller found.";
                statusMessageTimeout = 2.0;
                emit statusMessageSignal();
                emit measureEndSignal();
                state = 0;
                return;
            }
            auto rightId
                = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
                    vr::TrackedControllerRole_RightHand );
            if ( rightId == vr::k_unTrackedDeviceIndexInvalid )
            {
                statusMessage = "No right controller found.";
                statusMessageTimeout = 2.0;
                emit statusMessageSignal();
                emit measureEndSignal();
                state = 0;
                return;
            }
            // Get poses
            vr::TrackedDevicePose_t* leftPose = devicePoses + leftId;
            vr::TrackedDevicePose_t* rightPose = devicePoses + rightId;
            if ( !leftPose->bPoseIsValid || !leftPose->bDeviceIsConnected
                 || leftPose->eTrackingResult != vr::TrackingResult_Running_OK )
            {
                statusMessage = "Left controller tracking problems.";
                statusMessageTimeout = 2.0;
                emit statusMessageSignal();
                emit measureEndSignal();
                state = 0;
                return;
            }
            else if ( !rightPose->bPoseIsValid || !rightPose->bDeviceIsConnected
                      || rightPose->eTrackingResult
                             != vr::TrackingResult_Running_OK )
            {
                statusMessage = "Right controller tracking problems.";
                statusMessageTimeout = 2.0;
                emit statusMessageSignal();
                emit measureEndSignal();
                state = 0;
                return;
            }
            else
            {
                // The controller with the lowest y-pos is the floor fix
                // reference

                if ( leftPose->mDeviceToAbsoluteTracking.m[1][3]
                     < rightPose->mDeviceToAbsoluteTracking.m[1][3] )
                {
                    referenceController = leftId;
                }
                else
                {
                    referenceController = rightId;
                }

                auto& m = devicePoses[referenceController]
                              .mDeviceToAbsoluteTracking.m;
                tempOffsetX = static_cast<double>( m[0][3] );
                tempOffsetY = static_cast<double>( m[1][3] );
                tempOffsetZ = static_cast<double>( m[2][3] );

                /*
                | Intrinsic y-x'-z" rotation matrix:
                | cr*cy+sp*sr*sy | cr*sp*sy-cy*sr | cp*sy |
                | cp*sr          | cp*cr          |-sp    |
                | cy*sp*sr-cr*sy | cr*cy*sp+sr*sy | cp*cy |

                yaw = atan2(cp*sy, cp*cy) [pi, -pi], CCW
                pitch = -asin(-sp) [pi/2, -pi/2]
                roll = atan2(cp*sr, cp*cr) [pi, -pi], CW
                */
                tempRoll = std::atan2( static_cast<double>( m[1][0] ),
                                       static_cast<double>( m[1][1] ) );
                measurementCount = 1;
            }
        }
        else
        {
            measurementCount++;
            auto& m
                = devicePoses[referenceController].mDeviceToAbsoluteTracking.m;

            double rollDiff = std::atan2( static_cast<double>( m[1][0] ),
                                          static_cast<double>( m[1][1] ) )
                              - tempRoll;
            if ( rollDiff > M_PI )
            {
                rollDiff -= 2.0 * M_PI;
            }
            else if ( rollDiff < -M_PI )
            {
                rollDiff += 2.0 * M_PI;
            }
            tempRoll += rollDiff / static_cast<double>( measurementCount );
            if ( tempRoll > M_PI )
            {
                tempRoll -= 2.0 * M_PI;
            }
            else if ( tempRoll < -M_PI )
            {
                tempRoll += 2.0 * M_PI;
            }

            if ( measurementCount >= 25 )
            {
                int type = 0;
                if ( std::abs( tempRoll ) <= M_PI_2 )
                {
                    type = getControllerType( referenceController );
                    if ( type == Controller_Wand )
                    {
                        floorOffsetY = static_cast<float>( tempOffsetY )
                                       - controllerUpOffsetCorrection;
                    }
                    else if ( type == Controller_Knuckles )
                    {
                        // Production

                        floorOffsetY = static_cast<float>( tempOffsetY )
                                       - knucklesUpOffsetCorrection;

                        // Generate Offset
                        // Comment out production and un-comment this.
                        // Only if Floor is known to be good (zero with wand)

                        /*
                        floorOffsetY = static_cast<float>(tempOffsetY);
                        LOG(INFO) << "Offset For Knuckles up is: " <<
                        floorOffsetY;
                        */
                    }
                    else
                    {
                        floorOffsetY = static_cast<float>( tempOffsetY );
                    }
                }
                else
                {
                    type = getControllerType( referenceController );
                    if ( type == Controller_Wand )
                    {
                        floorOffsetY = static_cast<float>( tempOffsetY )
                                       - controllerDownOffsetCorrection;
                    }
                    else if ( type == Controller_Knuckles )
                    {
                        // Production

                        floorOffsetY = static_cast<float>( tempOffsetY )
                                       - knucklesDownOffsetCorrection;

                        // Generate Offset
                        // Comment out production and un-comment this.
                        // Only if Floor is known to be good (zero with wand)

                        /*
                        floorOffsetY = static_cast<float>(tempOffsetY);
                        LOG(INFO) << "Offset For Knuckles up is: " <<
                        floorOffsetY;
                        */
                    }
                    else
                    {
                        floorOffsetY = static_cast<float>( tempOffsetY );
                    }
                }

                floorOffsetX = static_cast<float>( tempOffsetX );
                floorOffsetZ = static_cast<float>( tempOffsetZ );

                LOG( INFO )
                    << "Fix Floor and adjust space center: Floor Offset = ["
                    << floorOffsetX << ", " << floorOffsetY << ", "
                    << floorOffsetZ << "]";
                float offset[3] = { 0, 0, 0 };
                offset[1] = floorOffsetY;
                if ( state == 2 )
                {
                    offset[0] = floorOffsetX;
                    offset[2] = floorOffsetZ;
                }
                parent->m_moveCenterTabController.addOffset( offset );
                statusMessage
                    = ( state == 2 ) ? "Recentering ... Ok" : "Fixing ... OK";
                statusMessageTimeout = 1.0;
                emit statusMessageSignal();
                emit measureEndSignal();
                setCanUndo( true );
                state = 0;
                parent->m_moveCenterTabController.zeroOffsets();
                // this reset fixes a bug where fixed floor wouldn't show in WMR
                parent->m_moveCenterTabController.reset();
            }
        }
    }
}

int FixFloorTabController::getControllerType(
    vr::TrackedDeviceIndex_t controllerRole )
{
    const uint32_t maxLength = 64;
    /*
    vr::k_unMaxPropertyStringSize is Theoretical Max Size, however it is
    1024*32, and its pretty unrealistic to expect a controller name to be that
    big. We are just going to set 64 as an arbitrary size, and print error to
    log if too small
    */
    char controllerTypeString[maxLength];
    vr::ETrackedPropertyError error;
    auto stringLength = vr::VRSystem()->GetStringTrackedDeviceProperty(
        controllerRole,
        vr::Prop_ControllerType_String,
        controllerTypeString,
        maxLength,
        &error );
    if ( error != vr::TrackedProp_Success )
    {
        LOG( ERROR ) << "Error With Controller Type: "
                     << vr::VRSystem()->GetPropErrorNameFromEnum( error );
    }
    else if ( stringLength == 0 )
    {
        LOG( ERROR ) << "Device Index not valid";
    }
    else if ( strcmp( controllerTypeString, "knuckles" ) == 0 )
    {
        return Controller_Knuckles;
    }
    else if ( strcmp( controllerTypeString, "vive_controller" ) == 0 )
    {
        return Controller_Wand;
    }
    return Controller_Unknown;
}

QString FixFloorTabController::currentStatusMessage()
{
    return statusMessage;
}

float FixFloorTabController::currentStatusMessageTimeout()
{
    return statusMessageTimeout;
}

bool FixFloorTabController::canUndo() const
{
    return m_canUndo;
}

void FixFloorTabController::setCanUndo( bool value, bool notify )
{
    if ( m_canUndo != value )
    {
        m_canUndo = value;
        if ( notify )
        {
            emit canUndoChanged( m_canUndo );
        }
    }
}

void FixFloorTabController::fixFloorClicked()
{
    parent->m_moveCenterTabController.reset();
    statusMessage = "Fixing ...";
    statusMessageTimeout = 1.0;
    emit statusMessageSignal();
    emit measureStartSignal();
    measurementCount = 0;
    state = 1;
}

void FixFloorTabController::recenterClicked()
{
    parent->m_moveCenterTabController.sendSeatedRecenter();
}

void FixFloorTabController::undoFixFloorClicked()
{
    float off[3] = { -floorOffsetX, -floorOffsetY, -floorOffsetZ };
    parent->m_moveCenterTabController.addOffset( off );
    LOG( INFO ) << "Fix Floor: Undo Floor Offset = [" << -floorOffsetX << ", "
                << -floorOffsetY << ", " << -floorOffsetZ << "]";
    floorOffsetY = 0.0f;
    statusMessage = "Undo ... OK";
    statusMessageTimeout = 1.0;
    emit statusMessageSignal();
    setCanUndo( false );
    parent->m_moveCenterTabController.zeroOffsets();
}

} // namespace advsettings
