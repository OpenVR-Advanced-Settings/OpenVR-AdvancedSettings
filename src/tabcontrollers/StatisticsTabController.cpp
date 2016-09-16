#include "StatisticsTabController.h"
#include "../overlaycontroller.h"
#include "../overlaywidget.h"
#include "ui_overlaywidget.h"

// application namespace
namespace advsettings {


void StatisticsTabController::init(OverlayController * parent, OverlayWidget * widget) {
	this->parent = parent;
	this->widget = widget;
	connect(widget->ui->StatsDistanceResetButton, SIGNAL(clicked()), this, SLOT(StatsDistanceResetClicked()));
	connect(widget->ui->StatsRotationResetButton, SIGNAL(clicked()), this, SLOT(StatsRotationResetClicked()));
	connect(widget->ui->StatsLeftMaxSpeedResetButton, SIGNAL(clicked()), this, SLOT(StatsLeftControllerSpeedResetClicked()));
	connect(widget->ui->StatsRightMaxSpeedResetButton, SIGNAL(clicked()), this, SLOT(StatsRightControllerSpeedResetClicked()));
	connect(widget->ui->DroppedFramesResetButton, SIGNAL(clicked()), this, SLOT(DroppedFramesResetClicked()));
	connect(widget->ui->ReprojectedFramesResetButton, SIGNAL(clicked()), this, SLOT(ReprojectedFramesResetClicked()));
	connect(widget->ui->TimedOutResetButton, SIGNAL(clicked()), this, SLOT(TimedOutResetClicked()));

}

void StatisticsTabController::UpdateTab() {
	if (widget) {
		widget->ui->StatsDistanceLabel->setText(QString::asprintf("%.1f m", hmdDistance));
		widget->ui->StatsLeftMaxSpeedLabel->setText(QString::asprintf("%.1f m/s", leftControllerMaxSpeed));
		widget->ui->StatsRightMaxSpeedLabel->setText(QString::asprintf("%.1f m/s", rightControllerMaxSpeed));
		vr::Compositor_CumulativeStats pStats;
		vr::VRCompositor()->GetCumulativeStats(&pStats, sizeof(vr::Compositor_CumulativeStats));
		if (pStats.m_nPid != compositorStatsPid) {
			droppedFramesOffset = 0;
			reprojectedFramesOffset = 0;
			timedOutOffset = 0;
			compositorStatsPid = pStats.m_nPid;
		}
		widget->ui->DroppedFramesLabel->setText(QString::asprintf("%i", pStats.m_nNumDroppedFrames - droppedFramesOffset));
		widget->ui->ReprojectedFramesLabel->setText(QString::asprintf("%i", pStats.m_nNumReprojectedFrames - reprojectedFramesOffset));
		widget->ui->TimedOutLabel->setText(QString::asprintf("%i", pStats.m_nNumTimedOut - timedOutOffset));
	}
}

void StatisticsTabController::eventLoopTick(vr::TrackedDevicePose_t* devicePoses) {
	auto& m = devicePoses->mDeviceToAbsoluteTracking.m;

	// Hmd Distance //
	if (lastPosTimer == 0) {
		if (devicePoses[0].bPoseIsValid && devicePoses[0].eTrackingResult == vr::TrackingResult_Running_OK) {
			if (!lastHmdPosValid) {
				lastHmdPosValid = true;
			} else {
				auto delta = std::sqrt(std::pow(m[0][3] - lastHmdPos[0], 2) /*+ std::pow(m[1][3] - lastHmdPos[1], 2)*/ + std::pow(m[2][3] - lastHmdPos[2], 2));
				if (delta >= 0.01f) {
					hmdDistance += delta;
				}
			}
			lastHmdPos[0] = m[0][3];
			lastHmdPos[1] = m[1][3];
			lastHmdPos[2] = m[2][3];
		} else {
			lastHmdPosValid = false;
		}
	}


	// Controller speeds //
	auto leftId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
	if (leftId != vr::k_unTrackedDeviceIndexInvalid && devicePoses[leftId].bPoseIsValid && devicePoses[leftId].eTrackingResult == vr::TrackingResult_Running_OK) {
		auto& vel = devicePoses[leftId].vVelocity.v;
		auto lspeed = std::sqrt(std::pow(vel[0], 2) + std::pow(vel[1], 2) + std::pow(vel[2], 2));
		if (lspeed > leftControllerMaxSpeed) {
			leftControllerMaxSpeed = lspeed;
		}
	}

	auto rightId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
	if (rightId != vr::k_unTrackedDeviceIndexInvalid && devicePoses[rightId].bPoseIsValid && devicePoses[rightId].eTrackingResult == vr::TrackingResult_Running_OK) {
		auto& vel = devicePoses[rightId].vVelocity.v;
		auto rspeed = std::sqrt(std::pow(vel[0], 2) + std::pow(vel[1], 2) + std::pow(vel[2], 2));
		if (rspeed > rightControllerMaxSpeed) {
			rightControllerMaxSpeed = rspeed;
		}
	}

	// Hmd Rotation //

	/*
	| Intrinsic y-x'-z" rotation matrix:
	| cr*cy+sp*sr*sy | cr*sp*sy-cy*sr | cp*sy |
	| cp*sr          | cp*cr          |-sp    |
	| cy*sp*sr-cr*sy | cr*cy*sp+sr*sy | cp*cy |

	yaw = atan2(cp*sy, cp*cy) [pi, -pi], CCW
	pitch = -asin(-sp) [pi/2, -pi/2]
	roll = atan2(cp*sr, cp*cr) [pi, -pi], CW
	*/

	auto yaw = 0.0;
	if (devicePoses[0].bPoseIsValid && devicePoses[0].eTrackingResult == vr::TrackingResult_Running_OK) {
		yaw = std::atan2(m[0][2], m[2][2]);
		if (yaw < 0.0f) {
			yaw += 2 * M_PI; // map to [0, 2*pi], CCW
		}
		if (rotationResetFlag) {
			rotationDir = 0;
			rotationCounter = 0;
			rotationMarker = yaw;
			rotationResetFlag = false;
			lastYaw = -1.0f;
		} else if (lastYaw < 0.0f && yaw != rotationMarker) {
			lastYaw = yaw;
		} else if (lastYaw != yaw && yaw != rotationMarker) {
			auto diff = yaw - lastYaw;
			int mode = 0;
			if (std::abs(diff) > M_PI) {
				if (diff < -M_PI && (rotationMarker < yaw || rotationMarker > lastYaw)) { // CCW overflow
					mode = 1;
				} else if (diff > M_PI && (rotationMarker > yaw || rotationMarker < lastYaw)) { // CW overflow
					mode = -1;
				}
			} else {
				if (lastYaw < rotationMarker && yaw > rotationMarker) {
					mode = 1;
				} else if (lastYaw > rotationMarker && yaw < rotationMarker) {
					mode = -1;
				}
			}
			if (mode > 0) {
				if (rotationCounter == 0 && rotationDir <= 0) {
					rotationDir = 1;
				} else {
					rotationCounter++;
				}
			} else if (mode < 0) {
				if (rotationCounter == 0 && rotationDir >= 0) {
					rotationDir = -1;
				} else {
					rotationCounter--;
				}
			}
			lastYaw = yaw;
		}
	} else {
		lastYaw = -1;
	}

	if (parent->isDashboardVisible()) {
		if (lastPosTimer == 0) {
			widget->ui->StatsDistanceLabel->setText(QString::asprintf("%.1f m", hmdDistance));
		}
		widget->ui->StatsLeftMaxSpeedLabel->setText(QString::asprintf("%.1f m/s", leftControllerMaxSpeed));
		widget->ui->StatsRightMaxSpeedLabel->setText(QString::asprintf("%.1f m/s", rightControllerMaxSpeed));
		float displayCounter = rotationCounter;
		std::string dirStr;
		if (rotationDir == 0) {
			dirStr = "CCW";
		} else if (rotationDir > 0) {
			if (yaw > rotationMarker) {
				displayCounter += (yaw - rotationMarker) / (2 * M_PI);
			} else {
				displayCounter += 1.0f + (yaw - rotationMarker) / (2 * M_PI);
			}
			dirStr = "CCW";
		} else {
			if (yaw > rotationMarker) {
				displayCounter = -displayCounter + 1.0f - (yaw - rotationMarker) / (2 * M_PI);
			} else {
				displayCounter = -displayCounter - (yaw - rotationMarker) / (2 * M_PI);
			}
			dirStr = "CW";
		}
		widget->ui->StatsRotationLabel->setText(QString::asprintf("%.1f %s", displayCounter, dirStr.c_str()));
		vr::Compositor_CumulativeStats pStats;
		vr::VRCompositor()->GetCumulativeStats(&pStats, sizeof(vr::Compositor_CumulativeStats));
		if (pStats.m_nPid != compositorStatsPid) {
			droppedFramesOffset = 0;
			reprojectedFramesOffset = 0;
			timedOutOffset = 0;
			compositorStatsPid = pStats.m_nPid;
		}
		widget->ui->DroppedFramesLabel->setText(QString::asprintf("%i", pStats.m_nNumDroppedFrames - droppedFramesOffset));
		widget->ui->ReprojectedFramesLabel->setText(QString::asprintf("%i", pStats.m_nNumReprojectedFrames - reprojectedFramesOffset));
		widget->ui->TimedOutLabel->setText(QString::asprintf("%i", pStats.m_nNumTimedOut - timedOutOffset));
	}
	if (lastPosTimer == 0) {
		lastPosTimer = 10;
	} else {
		lastPosTimer--;
	}
}

void StatisticsTabController::StatsDistanceResetClicked() {
	lastHmdPosValid = false;
	hmdDistance = 0.0f;
	widget->ui->StatsDistanceLabel->setText(QString::asprintf("%.1f m", hmdDistance));
}

void StatisticsTabController::StatsRotationResetClicked() {
	rotationResetFlag = true;
}

void StatisticsTabController::StatsLeftControllerSpeedResetClicked() {
	leftControllerMaxSpeed = 0;
	widget->ui->StatsLeftMaxSpeedLabel->setText(QString::asprintf("%.1f m/s", leftControllerMaxSpeed));
}

void StatisticsTabController::StatsRightControllerSpeedResetClicked() {
	rightControllerMaxSpeed = 0;
	widget->ui->StatsRightMaxSpeedLabel->setText(QString::asprintf("%.1f m/s", rightControllerMaxSpeed));
}

void StatisticsTabController::DroppedFramesResetClicked() {
	vr::Compositor_CumulativeStats pStats;
	vr::VRCompositor()->GetCumulativeStats(&pStats, sizeof(vr::Compositor_CumulativeStats));
	droppedFramesOffset = pStats.m_nNumDroppedFrames;
}

void StatisticsTabController::ReprojectedFramesResetClicked() {
	vr::Compositor_CumulativeStats pStats;
	vr::VRCompositor()->GetCumulativeStats(&pStats, sizeof(vr::Compositor_CumulativeStats));
	reprojectedFramesOffset = pStats.m_nNumReprojectedFrames;
}

void StatisticsTabController::TimedOutResetClicked() {
	vr::Compositor_CumulativeStats pStats;
	vr::VRCompositor()->GetCumulativeStats(&pStats, sizeof(vr::Compositor_CumulativeStats));
	timedOutOffset = pStats.m_nNumTimedOut;
}

} // namespace advconfig
