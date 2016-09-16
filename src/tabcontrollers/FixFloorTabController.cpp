#include "FixFloorTabController.h"
#include "../overlaycontroller.h"
#include "../overlaywidget.h"
#include "ui_overlaywidget.h"

// application namespace
namespace advsettings {


void FixFloorTabController::init(OverlayController * parent, OverlayWidget * widget) {
	this->parent = parent;
	this->widget = widget;
	connect(widget->ui->FixFloorButton, SIGNAL(clicked()), this, SLOT(FixFloorClicked()));
	connect(widget->ui->UndoFixFloorButton, SIGNAL(clicked()), this, SLOT(UndoFixFloorClicked()));
}

void FixFloorTabController::UpdateTab() {
	if (widget) {
		if (state == 1) {
			widget->ui->FixFloorButton->setEnabled(false);
		} else {
			widget->ui->FixFloorButton->setEnabled(true);
		}
		if (floorOffset != 0.0f) {
			widget->ui->UndoFixFloorButton->setEnabled(true);
		} else {
			widget->ui->UndoFixFloorButton->setEnabled(false);
		}
	}
}

void FixFloorTabController::eventLoopTick(vr::TrackedDevicePose_t* devicePoses) {
	if (state == 1) {
		if (measurementCount == 0) {
			// Get Controller ids for left/right hand
			auto leftId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
			if (leftId == vr::k_unTrackedDeviceIndexInvalid) {
				widget->ui->FixFloorStatusLabel->setText("No left controller found.");
				statusMessageTimeout = 100;
				state = 2;
				UpdateTab();
				return;
			}
			auto rightId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
			if (rightId == vr::k_unTrackedDeviceIndexInvalid) {
				widget->ui->FixFloorStatusLabel->setText("No right controller found.");
				statusMessageTimeout = 100;
				state = 2;
				UpdateTab();
				return;
			}
			// Get poses
			vr::TrackedDevicePose_t* leftPose = devicePoses + leftId;
			vr::TrackedDevicePose_t* rightPose = devicePoses + rightId;
			if (!leftPose->bPoseIsValid || !leftPose->bDeviceIsConnected || leftPose->eTrackingResult != vr::TrackingResult_Running_OK) {
				widget->ui->FixFloorStatusLabel->setText("Left controller tracking problems.");
				statusMessageTimeout = 100;
				state = 2;
				UpdateTab();
				return;
			} else if (!rightPose->bPoseIsValid || !rightPose->bDeviceIsConnected || rightPose->eTrackingResult != vr::TrackingResult_Running_OK) {
				widget->ui->FixFloorStatusLabel->setText("Right controller tracking problems.");
				statusMessageTimeout = 100;
				state = 2;
				UpdateTab();
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
			widget->ui->FixFloorStatusLabel->setText("Fixing ... OK");
			statusMessageTimeout = 50;
			state = 2;
			UpdateTab();
		} else {
			tempOffset += (double)devicePoses[referenceController].mDeviceToAbsoluteTracking.m[1][3];
			measurementCount++;
		}
	} else if (state == 2) {
		if (statusMessageTimeout == 0) {
			if (widget) {
				widget->ui->FixFloorStatusLabel->setText("");
			}
			state = 0;
			UpdateTab();
		} else {
			statusMessageTimeout--;
		}
	}
}

void FixFloorTabController::FixFloorClicked() {
	if (widget) {
		widget->ui->FixFloorStatusLabel->setText("Fixing ...");
		widget->ui->FixFloorButton->setEnabled(false);
		widget->ui->UndoFixFloorButton->setEnabled(false);
		measurementCount = 0;
		state = 1;
	}
}

void FixFloorTabController::UndoFixFloorClicked() {
	parent->AddOffsetToUniverseCenter(vr::TrackingUniverseStanding, 1, -floorOffset);
	LOG(INFO) << "Fix Floor: Undo Floor Offset = " << -floorOffset;
	floorOffset = 0.0f;
	widget->ui->FixFloorStatusLabel->setText("Undo ... OK");
	statusMessageTimeout = 100;
	state = 2;
	UpdateTab();
}

} // namespace advconfig
