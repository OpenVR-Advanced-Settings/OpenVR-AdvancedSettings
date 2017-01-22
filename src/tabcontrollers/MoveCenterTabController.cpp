#include "MoveCenterTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings {

void MoveCenterTabController::initStage1() {
	setTrackingUniverse(vr::VRCompositor()->GetTrackingSpace());
	auto settings = OverlayController::appSettings();
	settings->beginGroup("playspaceSettings");
	auto value = settings->value("adjustChaperone", m_adjustChaperone);
	settings->endGroup();
	if (value.isValid() && !value.isNull()) {
		m_adjustChaperone = value.toBool();
	}

	reloadPttConfig();
}

void MoveCenterTabController::initStage2(OverlayController * parent, QQuickWindow * widget) {
	this->parent = parent;
	this->widget = widget;
}


int MoveCenterTabController::trackingUniverse() const {
	return (int)m_trackingUniverse;
}

void MoveCenterTabController::setTrackingUniverse(int value, bool notify) {
	if (m_trackingUniverse != value) {
		reset();
		m_trackingUniverse = value;
		if (notify) {
			emit trackingUniverseChanged(m_trackingUniverse);
		}
	}
}

float MoveCenterTabController::offsetX() const {
	return m_offsetX;
}

void MoveCenterTabController::setOffsetX(float value, bool notify) {
	if (m_offsetX != value) {
		modOffsetX(value - m_offsetX, notify);
	}
}

float MoveCenterTabController::offsetY() const {
	return m_offsetY;
}

void MoveCenterTabController::setOffsetY(float value, bool notify) {
	if (m_offsetY != value) {
		modOffsetY(value - m_offsetY, notify);
	}
}

float MoveCenterTabController::offsetZ() const {
	return m_offsetZ;
}

void MoveCenterTabController::setOffsetZ(float value, bool notify) {
	if (m_offsetZ != value) {
		modOffsetZ(value - m_offsetZ, notify);
	}
}

int MoveCenterTabController::rotation() const {
	return m_rotation;
}

void MoveCenterTabController::setRotation(int value, bool notify) {
	if (m_rotation != value) {
		float angle = (value - m_rotation) * 2 * M_PI / 360.0;
		parent->RotateUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, angle, m_adjustChaperone);
		m_rotation = value;
		if (notify) {
			emit rotationChanged(m_rotation);
		}
	}
}

bool MoveCenterTabController::adjustChaperone() const {
	return m_adjustChaperone;
}

void MoveCenterTabController::setAdjustChaperone(bool value, bool notify) {
	if (m_adjustChaperone != value) {
		m_adjustChaperone = value;
		if (m_trackingUniverse == vr::TrackingUniverseStanding) {
			vr::VRChaperoneSetup()->RevertWorkingCopy();
			float offsetdir;
			if (m_adjustChaperone) {
				offsetdir = -1.0;
			} else {
				offsetdir = 1.0;
			}

			if (m_offsetX != 0.0f) {
				if (m_adjustChaperone || m_rotation == 0) {
					parent->AddOffsetToCollisionBounds(0, offsetdir * m_offsetX, false);
				} else {
					auto angle = m_rotation * 2 * M_PI / 360.0;
					parent->AddOffsetToCollisionBounds(0, offsetdir * m_offsetX * std::cos(angle), false);
					parent->AddOffsetToCollisionBounds(2, offsetdir * m_offsetX * std::sin(angle), false);
				}
			}
			if (m_offsetY != 0.0f) {
				parent->AddOffsetToCollisionBounds(1, offsetdir * m_offsetY, false);
			}
			if (m_offsetZ != 0.0f) {
				if (m_adjustChaperone || m_rotation == 0) {
					parent->AddOffsetToCollisionBounds(2, offsetdir * m_offsetZ, false);
				} else {
					auto angle = m_rotation * 2 * M_PI / 360.0;
					parent->AddOffsetToCollisionBounds(2, offsetdir * m_offsetZ * std::cos(angle), false);
					parent->AddOffsetToCollisionBounds(0, -offsetdir * m_offsetZ * std::sin(angle), false);
				}
			}
			if (m_rotation != 0) {
				float angle = m_rotation * 2 * M_PI / 360.0;
				parent->RotateCollisionBounds(offsetdir * angle, false);
			}
			vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
		}
		auto settings = OverlayController::appSettings();
		settings->beginGroup("playspaceSettings");
		settings->setValue("adjustChaperone", m_adjustChaperone);
		settings->endGroup();
		settings->sync();
		if (notify) {
			emit adjustChaperoneChanged(m_adjustChaperone);
		}
	}
}

bool MoveCenterTabController::pttEnabled() const {
	return m_pttEnabled;
}

void MoveCenterTabController::setPttEnabled(bool value, bool notify, bool save) {
	std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
	if (value != m_pttEnabled) {
		m_pttEnabled = value;
		if (notify) {
			emit pttEnabledChanged(value);
		}
		if (save) {
			savePttConfig();
		}
	}
}

void MoveCenterTabController::offsetYToggle(bool enabled) {
	float delta;
	if (enabled) {
		m_toggleOffsetY = m_offsetY;
		delta = -m_toggleOffsetY;
	} else {
		delta = m_toggleOffsetY;
	}
	parent->AddOffsetToUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, 1, delta, m_adjustChaperone);
}

bool MoveCenterTabController::pttActive() const {
	return m_pttActive;
}

bool MoveCenterTabController::pttLeftControllerEnabled() const {
	return m_pttLeftControllerEnabled;
}

void MoveCenterTabController::setPttLeftControllerEnabled(bool value, bool notify, bool save) {
	std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
	if (value != m_pttLeftControllerEnabled) {
		m_pttLeftControllerEnabled = value;
		if (notify) {
			emit pttLeftControllerEnabledChanged(value);
		}
		if (save) {
			savePttConfig();
		}
	}
}

bool MoveCenterTabController::pttRightControllerEnabled() const {
	return m_pttRightControllerEnabled;
}

void MoveCenterTabController::setPttRightControllerEnabled(bool value, bool notify, bool save) {
	std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
	if (value != m_pttRightControllerEnabled) {
		m_pttRightControllerEnabled = value;
		if (notify) {
			emit pttRightControllerEnabledChanged(value);
		}
		if (save) {
			savePttConfig();
		}
	}
}

void MoveCenterTabController::modOffsetX(float value, bool notify) {
	if (m_rotation == 0) {
		parent->AddOffsetToUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, 0, value, m_adjustChaperone);
	} else {
		auto angle = m_rotation * 2 * M_PI / 360.0;
		vr::VRChaperoneSetup()->RevertWorkingCopy();
		parent->AddOffsetToUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, 0, value * std::cos(angle), m_adjustChaperone, false);
		parent->AddOffsetToUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, 2, value * std::sin(angle), m_adjustChaperone, false);
		vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
	}
	m_offsetX += value;
	if (notify) {
		emit offsetXChanged(m_offsetX);
	}
}

void MoveCenterTabController::modOffsetY(float value, bool notify) {
	parent->AddOffsetToUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, 1, value, m_adjustChaperone);
	m_offsetY += value;
	if (notify) {
		emit offsetYChanged(m_offsetY);
	}
}

void MoveCenterTabController::modOffsetZ(float value, bool notify) {
	if (m_rotation == 0) {
		parent->AddOffsetToUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, 2, value, m_adjustChaperone);
	} else {
		auto angle = m_rotation * 2 * M_PI / 360.0;
		vr::VRChaperoneSetup()->RevertWorkingCopy();
		parent->AddOffsetToUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, 2, value * std::cos(angle), m_adjustChaperone, false);
		parent->AddOffsetToUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, 0, -value * std::sin(angle), m_adjustChaperone, false);
		vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
	}
	m_offsetZ += value;
	if (notify) {
		emit offsetZChanged(m_offsetZ);
	}
}

void MoveCenterTabController::reset() {
	vr::VRChaperoneSetup()->RevertWorkingCopy();
	parent->RotateUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, -m_rotation * 2 * M_PI / 360.0, m_adjustChaperone, false);
	parent->AddOffsetToUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, 0, -m_offsetX, m_adjustChaperone, false);
	parent->AddOffsetToUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, 1, -m_offsetY, m_adjustChaperone, false);
	parent->AddOffsetToUniverseCenter((vr::TrackingUniverseOrigin)m_trackingUniverse, 2, -m_offsetZ, m_adjustChaperone, false);
	vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
	m_offsetX = 0.0f;
	m_offsetY = 0.0f;
	m_toggleOffsetY = 0.0f;
	m_offsetZ = 0.0f;
	m_rotation = 0;
	emit offsetXChanged(m_offsetX);
	emit offsetYChanged(m_offsetY);
	emit offsetZChanged(m_offsetZ);
	emit rotationChanged(m_rotation);
}

void MoveCenterTabController::eventLoopTick(vr::ETrackingUniverseOrigin universe) {
	if (!eventLoopMutex.try_lock()) {
		return;
	}
	if (settingsUpdateCounter >= 50) {
		setTrackingUniverse((int)universe);
		settingsUpdateCounter = 0;
	} else {
		settingsUpdateCounter++;
	}
	static auto handleControllerState = [](const vr::VRControllerState_t& state, PttControllerConfig* config) -> bool {
		if (state.ulButtonPressed & config->digitalButtonMask) {
			return true;
		}
		if (config->triggerMode) {
			if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger)
					|| state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger)) {
				return true;
			}
		}
		if (config->touchpadMode) {
			if (((config->touchpadMode & 1) && (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)))
					|| ((config->touchpadMode & 2) && (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)))) {
				if (config->touchpadAreas == (PAD_AREA_LEFT + PAD_AREA_TOP + PAD_AREA_RIGHT + PAD_AREA_BOTTOM)) {
					return true;
				} else {
					float x = state.rAxis[0].x;
					float y = state.rAxis[0].y;
					if (std::abs(x) >= 0.2 || std::abs(y) >= 0.2) { // deadzone in the middle
						if (x < 0 && std::abs(y) < -x && (config->touchpadAreas & PAD_AREA_LEFT)) {
							return true;
						} else if (y > 0 && std::abs(x) < y && (config->touchpadAreas & PAD_AREA_TOP)) {
							return true;
						} else if (x > 0 && std::abs(y) < x && (config->touchpadAreas & PAD_AREA_RIGHT)) {
							return true;
						} else if (y < 0 && std::abs(x) < -y && (config->touchpadAreas & PAD_AREA_BOTTOM)) {
							return true;
						}
					}
				}
			}
		}
		return false;
	};
	if (m_pttEnabled) {
		bool newState = false;
		if (m_pttLeftControllerEnabled) {
			auto leftId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
			if (leftId != vr::k_unTrackedDeviceIndexInvalid) {
				vr::VRControllerState_t state;
				if (vr::VRSystem()->GetControllerState(leftId, &state, sizeof(vr::VRControllerState_t))) {
					newState |= handleControllerState(state, m_pttControllerConfigs);
				}
			}
		}
		if (m_pttRightControllerEnabled) {
			auto rightId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
			if (rightId != vr::k_unTrackedDeviceIndexInvalid) {
				vr::VRControllerState_t state;
				if (vr::VRSystem()->GetControllerState(rightId, &state, sizeof(vr::VRControllerState_t))) {
					newState |= handleControllerState(state, m_pttControllerConfigs + 1);
				}
			}
		}
		if (newState && !m_pttActive) {
			m_pttActive = true;
			offsetYToggle(true);
			emit pttActiveChanged(m_pttActive);
		} else if (!newState && m_pttActive) {
			m_pttActive = false;
			offsetYToggle(false);
			emit pttActiveChanged(m_pttActive);
		}
	}
	eventLoopMutex.unlock();
}

void MoveCenterTabController::setPttControllerConfig(unsigned controller, QVariantList buttons, unsigned triggerMode, unsigned padMode, unsigned padAreas) {
	std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
	if (controller < 2) {
		uint64_t buttonMask = 0;
		for (auto b : buttons) {
			buttonMask |= vr::ButtonMaskFromId((vr::EVRButtonId)b.toInt());
		}
		m_pttControllerConfigs[controller].digitalButtons = buttons;
		m_pttControllerConfigs[controller].digitalButtonMask = buttonMask;
		m_pttControllerConfigs[controller].triggerMode = triggerMode;
		m_pttControllerConfigs[controller].touchpadMode = padMode;
		m_pttControllerConfigs[controller].touchpadAreas = padAreas;
		savePttConfig();
	}
}

void MoveCenterTabController::reloadPttConfig() {
	std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
	auto settings = OverlayController::appSettings();
	settings->beginGroup("playspaceSettings");
	setPttEnabled(settings->value("pttEnabled", false).toBool(), true, false);
	setPttLeftControllerEnabled(settings->value("pttLeftControllerEnabled", false).toBool(), true, false);
	setPttRightControllerEnabled(settings->value("pttRightControllerEnabled", false).toBool(), true, false);
	m_pttControllerConfigs[0].digitalButtons = settings->value("pttDigitalButtons_0").toList();
	uint64_t buttonMask = 0;
	for (auto b : m_pttControllerConfigs[0].digitalButtons) {
		buttonMask |= vr::ButtonMaskFromId((vr::EVRButtonId)b.toInt());
	}
	m_pttControllerConfigs[0].digitalButtonMask = buttonMask;
	m_pttControllerConfigs[0].triggerMode = settings->value("pttTriggerMode_0", 0).toUInt();
	m_pttControllerConfigs[0].touchpadMode = settings->value("pttTouchpadMode_0", 0).toUInt();
	m_pttControllerConfigs[0].touchpadAreas = settings->value("pttTouchPadAreas_0", 0).toUInt();
	m_pttControllerConfigs[1].digitalButtons = settings->value("pttDigitalButtons_1").toList();
	buttonMask = 0;
	for (auto b : m_pttControllerConfigs[0].digitalButtons) {
		buttonMask |= vr::ButtonMaskFromId((vr::EVRButtonId)b.toInt());
	}
	m_pttControllerConfigs[1].digitalButtonMask = buttonMask;
	m_pttControllerConfigs[1].triggerMode = settings->value("pttTriggerMode_1", 0).toUInt();
	m_pttControllerConfigs[1].touchpadMode = settings->value("pttTouchpadMode_1", 0).toUInt();
	m_pttControllerConfigs[1].touchpadAreas = settings->value("pttTouchPadAreas_1", 0).toUInt();
	settings->endGroup();
}

void MoveCenterTabController::savePttConfig() {
	auto settings = OverlayController::appSettings();
	settings->beginGroup("playspaceSettings");
	settings->setValue("pttEnabled", pttEnabled());
	settings->setValue("pttLeftControllerEnabled", pttLeftControllerEnabled());
	settings->setValue("pttRightControllerEnabled", pttRightControllerEnabled());
	settings->setValue("pttDigitalButtons_0", m_pttControllerConfigs[0].digitalButtons);
	settings->setValue("pttTriggerMode_0", m_pttControllerConfigs[0].triggerMode);
	settings->setValue("pttTouchpadMode_0", m_pttControllerConfigs[0].touchpadMode);
	settings->setValue("pttTouchPadAreas_0", m_pttControllerConfigs[0].touchpadAreas);
	settings->setValue("pttDigitalButtons_1", m_pttControllerConfigs[1].digitalButtons);
	settings->setValue("pttTriggerMode_1", m_pttControllerConfigs[1].triggerMode);
	settings->setValue("pttTouchpadMode_1", m_pttControllerConfigs[1].touchpadMode);
	settings->setValue("pttTouchPadAreas_1", m_pttControllerConfigs[1].touchpadAreas);
	settings->endGroup();
}


QVariantList MoveCenterTabController::pttDigitalButtons(unsigned controller) {
	if (controller < 2) {
		return m_pttControllerConfigs[controller].digitalButtons;
	}
	return QVariantList();
}

unsigned long MoveCenterTabController::pttDigitalButtonMask(unsigned controller) {
	if (controller < 2) {
		return m_pttControllerConfigs[controller].digitalButtonMask;
	}
	return 0;
}

unsigned MoveCenterTabController::pttTouchpadMode(unsigned controller) {
	if (controller < 2) {
		return m_pttControllerConfigs[controller].touchpadMode;
	}
	return 0;
}

unsigned MoveCenterTabController::pttTriggerMode(unsigned controller) {
	if (controller < 2) {
		return m_pttControllerConfigs[controller].triggerMode;
	}
	return 0;
}

unsigned MoveCenterTabController::pttTouchpadArea(unsigned controller) {
	if (controller < 2) {
		return m_pttControllerConfigs[controller].touchpadAreas;
	}
	return 0;
}



} // namespace advconfig
