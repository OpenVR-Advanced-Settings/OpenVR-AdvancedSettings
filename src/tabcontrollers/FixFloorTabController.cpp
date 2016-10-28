#include "FixFloorTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings {


void FixFloorTabController::initStage1() {
}

void FixFloorTabController::initStage2(OverlayController * parent, QQuickWindow * widget) {
	this->parent = parent;
	this->widget = widget;
}

void FixFloorTabController::eventLoopTick(vr::TrackedDevicePose_t* devicePoses) {
	if (state == 1) {
		if (measurementCount == 0) {
			// Get Controller ids for left/right hand
			auto leftId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
			if (leftId == vr::k_unTrackedDeviceIndexInvalid) {
				statusMessage = "No left controller found.";
				statusMessageTimeout = 2.0;
				emit statusMessageSignal();
				emit measureEndSignal();
				state = 0;
				return;
			}
			auto rightId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
			if (rightId == vr::k_unTrackedDeviceIndexInvalid) {
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
			if (!leftPose->bPoseIsValid || !leftPose->bDeviceIsConnected || leftPose->eTrackingResult != vr::TrackingResult_Running_OK) {
				statusMessage = "Left controller tracking problems.";
				statusMessageTimeout = 2.0;
				emit statusMessageSignal();
				emit measureEndSignal();
				state = 0;
				return;
			} else if (!rightPose->bPoseIsValid || !rightPose->bDeviceIsConnected || rightPose->eTrackingResult != vr::TrackingResult_Running_OK) {
				statusMessage = "Right controller tracking problems.";
				statusMessageTimeout = 2.0;
				emit statusMessageSignal();
				emit measureEndSignal();
				state = 0;
				return;
			} else {
				// The controller with the lowest y-pos is the floor fix reference
				if (leftPose->mDeviceToAbsoluteTracking.m[1][3] < rightPose->mDeviceToAbsoluteTracking.m[1][3]) {
					referenceController = leftId;
				} else {
					referenceController = rightId;
				}
				tempOffset = (double)devicePoses[referenceController].mDeviceToAbsoluteTracking.m[1][3];
				measurementCount = 1;
			}

		} else if (measurementCount >= 25) {
			floorOffset = -controllerUpOffsetCorrection + (float)(tempOffset / (double)measurementCount);
			LOG(INFO) << "Fix Floor: Floor Offset = " << floorOffset;
			parent->AddOffsetToUniverseCenter(vr::TrackingUniverseStanding, 1, floorOffset);
			statusMessage = "Fixing ... OK";
			statusMessageTimeout = 1.0;
			emit statusMessageSignal();
			emit measureEndSignal();
			setCanUndo(true);
			state = 0;
		} else {
			tempOffset += (double)devicePoses[referenceController].mDeviceToAbsoluteTracking.m[1][3];
			measurementCount++;
		}
	}
}

QString FixFloorTabController::currentStatusMessage() {
	return statusMessage;
}

float FixFloorTabController::currentStatusMessageTimeout() {
	return statusMessageTimeout;
}

bool FixFloorTabController::canUndo() const {
	return m_canUndo;
}

void FixFloorTabController::setCanUndo(bool value, bool notify) {
	if (m_canUndo != value) {
		m_canUndo = value;
		if (notify) {
			emit canUndoChanged(m_canUndo);
		}
	}
}

void FixFloorTabController::fixFloorClicked() {
	statusMessage = "Fixing ...";
	statusMessageTimeout = 1.0;
	emit statusMessageSignal();
	emit measureStartSignal();
	measurementCount = 0;
	state = 1;
}

void FixFloorTabController::undoFixFloorClicked() {
	parent->AddOffsetToUniverseCenter(vr::TrackingUniverseStanding, 1, -floorOffset);
	LOG(INFO) << "Fix Floor: Undo Floor Offset = " << -floorOffset;
	floorOffset = 0.0f;
	statusMessage = "Undo ... OK";
	statusMessageTimeout = 1.0;
	emit statusMessageSignal();
	setCanUndo(false);
}

} // namespace advconfig
