#include "StatisticsTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings {

void StatisticsTabController::initStage1() {
}

void StatisticsTabController::initStage2(OverlayController * parent, QQuickWindow * widget) {
	this->parent = parent;
	this->widget = widget;
}

void StatisticsTabController::eventLoopTick(vr::TrackedDevicePose_t* devicePoses) {
	vr::Compositor_CumulativeStats pStats;
	vr::VRCompositor()->GetCumulativeStats(&pStats, sizeof(vr::Compositor_CumulativeStats));
	if (pStats.m_nPid != m_cumStats.m_nPid) {
		m_cumStats = pStats;
		m_droppedFramesOffset = 0;
		m_reprojectedFramesOffset = 0;
		m_timedOutOffset = 0;
		m_totalRatioPresentedOffset = 0;
		m_totalRatioReprojectedOffset = 0;
	}
	m_cumStats = pStats;

	auto& m = devicePoses->mDeviceToAbsoluteTracking.m;

	// Hmd Distance //
	if (lastPosTimer == 0) {
		if (devicePoses[0].bPoseIsValid && devicePoses[0].eTrackingResult == vr::TrackingResult_Running_OK) {
			if (!lastHmdPosValid) {
				lastHmdPosValid = true;
			} else {
				auto delta = std::sqrt(std::pow(m[0][3] - lastHmdPos[0], 2) /*+ std::pow(m[1][3] - lastHmdPos[1], 2)*/ + std::pow(m[2][3] - lastHmdPos[2], 2));
				if (delta >= 0.01f) {
					m_hmdDistanceMoved += delta;
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
		if (lspeed > m_leftControllerMaxSpeed) {
			m_leftControllerMaxSpeed = lspeed;
		}
	}

	auto rightId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
	if (rightId != vr::k_unTrackedDeviceIndexInvalid && devicePoses[rightId].bPoseIsValid && devicePoses[rightId].eTrackingResult == vr::TrackingResult_Running_OK) {
		auto& vel = devicePoses[rightId].vVelocity.v;
		auto rspeed = std::sqrt(std::pow(vel[0], 2) + std::pow(vel[1], 2) + std::pow(vel[2], 2));
		if (rspeed > m_rightControllerMaxSpeed) {
			m_rightControllerMaxSpeed = rspeed;
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

	double yawRaw = 0.0;
	double yaw = 0.0;
	bool rotationChanged = false;
	if (devicePoses[0].bPoseIsValid && devicePoses[0].eTrackingResult == vr::TrackingResult_Running_OK) {
		yawRaw = std::atan2(m[0][2], m[2][2]);
		if (yawRaw < 0.0f) {
			yawRaw += 2 * M_PI; // map to [0, 2*pi], CCW
		}
		yaw = yawRaw - rotationOffset;
		if (yaw < 0.0) {
			yaw = 2 * M_PI + yaw;
		}
		if (rotationResetFlag) {
			rotationDir = 0;
			rotationCounter = 0;
			rotationOffset = yawRaw;
			rotationResetFlag = false;
			lastYaw = -1.0f;
			rotationChanged = true;
		} else if (lastYaw < 0.0f && yaw > 0.0) {
			lastYaw = yaw;
			rotationChanged = true;
			if (yaw <= M_PI) {
				rotationDir = 1;
			} else {
				rotationDir = -1;
			}
		} else if (std::abs(lastYaw - yaw) >= 0.01 && yaw > 0.0) {
			auto diff = yaw - lastYaw;
			int mode = 0;
			if (std::abs(diff) > M_PI) {
				if (diff < -M_PI) { // CCW overflow
					mode = 1;
				} else if (diff > M_PI) { // CW overflow
					mode = -1;
				}
			} else {
				if (lastYaw < 0.0 && yaw > 0.0) {
					mode = 1;
				} else if (lastYaw > 0.0 && yaw < 0.0) {
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
			rotationChanged = true;
		}
	} else {
		lastYaw = -1;
	}

	if (rotationChanged) {
		m_hmdRotation = (float)rotationCounter;
		if (rotationDir > 0) {
			m_hmdRotation += yaw / (2 * M_PI);
		} else if (rotationDir < 0) {
			m_hmdRotation += -1.0f + yaw / (2 * M_PI);
		}
	}
	if (lastPosTimer == 0) {
		lastPosTimer = 10;
	} else {
		lastPosTimer--;
	}
}

float StatisticsTabController::hmdDistanceMoved() const {
	return m_hmdDistanceMoved;
}

float StatisticsTabController::hmdRotations() const {
	return m_hmdRotation;
}

float StatisticsTabController::rightControllerMaxSpeed() const {
	return m_rightControllerMaxSpeed;
}

float StatisticsTabController::leftControllerMaxSpeed() const {
	return m_leftControllerMaxSpeed;
}

unsigned StatisticsTabController::presentedFrames() const {
	return m_cumStats.m_nNumFramePresents - m_presentedFramesOffset;
}

unsigned StatisticsTabController::droppedFrames() const {
	return m_cumStats.m_nNumDroppedFrames - m_droppedFramesOffset;
}

unsigned StatisticsTabController::reprojectedFrames() const {
	return m_cumStats.m_nNumReprojectedFrames - m_reprojectedFramesOffset;
}

unsigned StatisticsTabController::timedOut() const {
	return m_cumStats.m_nNumTimedOut - m_timedOutOffset;
}

float StatisticsTabController::totalReprojectedRatio() const {
	float totalFrames = (float)(m_cumStats.m_nNumFramePresents - m_totalRatioPresentedOffset);
	float reprojectedFrames = (float)(m_cumStats.m_nNumReprojectedFrames - m_totalRatioReprojectedOffset);
	if (totalFrames != 0.0f) {
		return reprojectedFrames / totalFrames;
	} else {
		return 0.0;
	}
}

void StatisticsTabController::statsDistanceResetClicked() {
	lastHmdPosValid = false;
	if (m_hmdDistanceMoved != 0.0) {
		m_hmdDistanceMoved = 0.0;
	}
}

void StatisticsTabController::statsRotationResetClicked() {
	rotationResetFlag = true;
}

void StatisticsTabController::statsLeftControllerSpeedResetClicked() {
	if (m_leftControllerMaxSpeed != 0.0) {
		m_leftControllerMaxSpeed = 0.0;
	}
}

void StatisticsTabController::statsRightControllerSpeedResetClicked() {
	if (m_rightControllerMaxSpeed != 0.0) {
		m_rightControllerMaxSpeed = 0.0;
	}
}

void StatisticsTabController::presentedFramesResetClicked() {
	m_presentedFramesOffset = m_cumStats.m_nNumFramePresents;
}

void StatisticsTabController::droppedFramesResetClicked() {
	m_droppedFramesOffset = m_cumStats.m_nNumDroppedFrames;
}

void StatisticsTabController::reprojectedFramesResetClicked() {
	m_reprojectedFramesOffset = m_cumStats.m_nNumReprojectedFrames;
}

void StatisticsTabController::timedOutResetClicked() {
	m_timedOutOffset = m_cumStats.m_nNumTimedOut;
}

void StatisticsTabController::totalRatioResetClicked() {
	m_totalRatioPresentedOffset = m_cumStats.m_nNumFramePresents;
	m_totalRatioReprojectedOffset = m_cumStats.m_nNumReprojectedFrames;
}

} // namespace advconfig
