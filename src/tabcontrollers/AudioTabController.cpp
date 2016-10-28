#include "AudioTabController.h"
#include <QQuickWindow>
#include <QApplication>
#include "../overlaycontroller.h"
#include "audiomanager/AudioManagerWindows.h"

// application namespace
namespace advsettings {

	void AudioTabController::initStage1() {
		vr::EVRSettingsError vrSettingsError;
		audioManager.reset(new AudioManagerWindows());
		audioManager->init(this);
		char mirrorDeviceId[1024];
		vr::VRSettings()->GetString(vr::k_pch_audio_Section, vr::k_pch_audio_OnPlaybackMirrorDevice_String, mirrorDeviceId, 1024, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_audio_OnPlaybackMirrorDevice_String << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
		audioManager->setMirrorDevice(mirrorDeviceId);
		lastMirrorDevId = mirrorDeviceId;
		m_micPresent = audioManager->isMicValid();
		m_micVolume = audioManager->getMicVolume();
		m_micMuted = audioManager->getMicMuted();
		m_micDevName = QString::fromStdString(audioManager->getMicDevName());
		m_mirrorPresent = audioManager->isMirrorValid();
		m_mirrorVolume = audioManager->getMirrorVolume();
		m_mirrorMuted = audioManager->getMirrorMuted();
		m_mirrorDevName = QString::fromStdString(audioManager->getMirrorDevName());
		reloadPttProfiles();
		reloadPttConfig();
		eventLoopTick();
	}


	void AudioTabController::initStage2(OverlayController * parent, QQuickWindow * widget) {
		this->parent = parent;
		this->widget = widget;

		std::string notifKey = std::string(OverlayController::applicationKey) + ".pptnotification";
		vr::VROverlayError overlayError = vr::VROverlay()->CreateOverlay(notifKey.c_str(), notifKey.c_str(), &m_ulNotificationOverlayHandle);
		if (overlayError == vr::VROverlayError_None) {
			std::string notifIconPath = QApplication::applicationDirPath().toStdString() + "/res/qml/ptt_notification.png";
			if (QFile::exists(QString::fromStdString(notifIconPath))) {
				vr::VROverlay()->SetOverlayFromFile(m_ulNotificationOverlayHandle, notifIconPath.c_str());
				vr::VROverlay()->SetOverlayWidthInMeters(m_ulNotificationOverlayHandle, 0.02f);
				vr::HmdMatrix34_t notificationTransform = {
					1.0f, 0.0f, 0.0f, 0.12f,
					0.0f, 1.0f, 0.0f, 0.08f,
					0.0f, 0.0f, 1.0f, -0.3f
				};
				vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(m_ulNotificationOverlayHandle, vr::k_unTrackedDeviceIndex_Hmd, &notificationTransform);
			} else {
				LOG(ERROR) << "Could not find notification icon \"" << notifIconPath << "\"";
			}
		} else {
			LOG(ERROR) << "Could not create ptt notification overlay: " << vr::VROverlay()->GetOverlayErrorNameFromEnum(overlayError);
		}
	}


	bool AudioTabController::mirrorPresent() const {
		return m_mirrorPresent;
	}


	QString AudioTabController::mirrorDevName() const {
		return m_mirrorDevName;
	}


	float AudioTabController::mirrorVolume() const {
		return m_mirrorVolume;
	}


	bool AudioTabController::mirrorMuted() const {
		return m_mirrorMuted;
	}


	bool AudioTabController::micPresent() const {
		return m_micPresent;
	}


	QString AudioTabController::micDevName() const {
		return m_micDevName;
	}


	float AudioTabController::micVolume() const {
		return m_micVolume;
	}


	bool AudioTabController::micMuted() const {
		return m_micMuted;
	}


	bool AudioTabController::pttEnabled() const {
		return m_pttEnabled;
	}


	bool AudioTabController::pttActive() const {
		return m_pttActive;
	}


	bool AudioTabController::pttShowNotification() const {
		return m_pttShowNotification;
	}


	bool AudioTabController::pttLeftControllerEnabled() const {
		return m_pttLeftControllerEnabled;
	}


	bool AudioTabController::pttRightControllerEnabled() const {
		return m_pttRightControllerEnabled;
	}

	void AudioTabController::setPttControllerConfig(unsigned controller, QVariantList buttons, unsigned triggerMode, unsigned padMode, unsigned padAreas) {
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

	void AudioTabController::reloadPttProfiles() {
		pttProfiles.clear();
		auto settings = OverlayController::appSettings();
		settings->beginGroup("audioSettings");
		auto profileCount = settings->beginReadArray("pttProfiles");
		for (int i = 0; i < profileCount; i++) {
			settings->setArrayIndex(i);
			pttProfiles.emplace_back();
			auto& entry = pttProfiles[i];
			entry.profileName = settings->value("profileName").toString().toStdString();
			entry.showNotification = settings->value("showNotification", true).toBool();
			entry.leftControllerEnabled = settings->value("leftControllerEnabled", false).toBool();
			entry.rightControllerEnabled = settings->value("rightControllerEnabled", false).toBool();
			entry.controllerConfigs[0].digitalButtons = settings->value("digitalButtons_0").toList();
			uint64_t buttonMask = 0;
			for (auto b : entry.controllerConfigs[0].digitalButtons) {
				buttonMask |= vr::ButtonMaskFromId((vr::EVRButtonId)b.toInt());
			}
			entry.controllerConfigs[0].digitalButtonMask = buttonMask;
			entry.controllerConfigs[0].triggerMode = settings->value("triggerMode_0", 0).toUInt();
			entry.controllerConfigs[0].touchpadMode = settings->value("touchpadMode_0", 0).toUInt();
			entry.controllerConfigs[0].touchpadAreas = settings->value("touchPadAreas_0", 0).toUInt();
			entry.controllerConfigs[1].digitalButtons = settings->value("digitalButtons_1").toList();
			buttonMask = 0;
			for (auto b : entry.controllerConfigs[1].digitalButtons) {
				buttonMask |= vr::ButtonMaskFromId((vr::EVRButtonId)b.toInt());
			}
			entry.controllerConfigs[1].digitalButtonMask = buttonMask;
			entry.controllerConfigs[1].triggerMode = settings->value("triggerMode_1", 0).toUInt();
			entry.controllerConfigs[1].touchpadMode = settings->value("touchpadMode_1", 0).toUInt();
			entry.controllerConfigs[1].touchpadAreas = settings->value("touchPadAreas_1", 0).toUInt();
		}
		settings->endArray();
		settings->endGroup();
	}

	void AudioTabController::savePttProfiles() {
		auto settings = OverlayController::appSettings();
		settings->beginGroup("audioSettings");
		settings->beginWriteArray("pttProfiles");
		unsigned i = 0;
		for (auto& p : pttProfiles) {
			settings->setArrayIndex(i);
			settings->setValue("profileName", QString::fromStdString(p.profileName));
			settings->setValue("showNotification", p.showNotification);
			settings->setValue("leftControllerEnabled", p.leftControllerEnabled);
			settings->setValue("rightControllerEnabled", p.rightControllerEnabled);
			settings->setValue("digitalButtons_0", p.controllerConfigs[0].digitalButtons);
			settings->setValue("triggerMode_0", p.controllerConfigs[0].triggerMode);
			settings->setValue("touchpadMode_0", p.controllerConfigs[0].touchpadMode);
			settings->setValue("touchPadAreas_0", p.controllerConfigs[0].touchpadAreas);
			settings->setValue("digitalButtons_1", p.controllerConfigs[1].digitalButtons);
			settings->setValue("triggerMode_1", p.controllerConfigs[1].triggerMode);
			settings->setValue("touchpadMode_1", p.controllerConfigs[1].touchpadMode);
			settings->setValue("touchPadAreas_1", p.controllerConfigs[1].touchpadAreas);
			i++;
		}
		settings->endArray();
		settings->endGroup();
	}


	void AudioTabController::reloadPttConfig() {
		std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
		auto settings = OverlayController::appSettings();
		settings->beginGroup("audioSettings");
		setPttEnabled(settings->value("pttEnabled", false).toBool(), true, false);
		setPttLeftControllerEnabled(settings->value("pttLeftControllerEnabled", false).toBool(), true, false);
		setPttRightControllerEnabled(settings->value("pttRightControllerEnabled", false).toBool(), true, false);
		setPttShowNotification(settings->value("pttShowNotification", true).toBool(), true, false);
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

	void AudioTabController::savePttConfig() {
		auto settings = OverlayController::appSettings();
		settings->beginGroup("audioSettings");
		settings->setValue("pttEnabled", pttEnabled());
		settings->setValue("pttLeftControllerEnabled", pttLeftControllerEnabled());
		settings->setValue("pttRightControllerEnabled", pttRightControllerEnabled());
		settings->setValue("pttShowNotification", pttShowNotification());
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

	void logControllerState(const vr::VRControllerState_t& state, const std::string& prefix) {
		if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_ApplicationMenu) << " pressed";
		}
		if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_ApplicationMenu) << " touched";
		}
		if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Grip)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Grip) << " pressed";
		}
		if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_Grip)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Grip) << " touched";
		}
		if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_DPad_Left)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_DPad_Left) << " pressed";
		}
		if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_DPad_Left)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_DPad_Left) << " touched";
		}
		if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_DPad_Up)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_DPad_Up) << " pressed";
		}
		if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_DPad_Up)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_DPad_Up) << " touched";
		}
		if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_DPad_Right)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_DPad_Right) << " pressed";
		}
		if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_DPad_Right)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_DPad_Right) << " touched";
		}
		if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_DPad_Down)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_DPad_Down) << " pressed";
		}
		if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_DPad_Down)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_DPad_Down) << " touched";
		}
		if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_A)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_A) << " pressed";
		}
		if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_A)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_A) << " touched";
		}
		if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Axis0)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis0) << " pressed";
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis0) << " x: " << state.rAxis[0].x << "  y: " << state.rAxis[0].y;
		}
		if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_Axis0)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis0) << " touched";
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis0) << " x: " << state.rAxis[0].x << "  y: " << state.rAxis[0].y;
		}
		if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Axis1)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis1) << " pressed";
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis1) << " x: " << state.rAxis[1].x << "  y: " << state.rAxis[0].y;
		}
		if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_Axis1)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis1) << " touched";
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis1) << " x: " << state.rAxis[1].x << "  y: " << state.rAxis[0].y;
		}
		if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Axis2)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis2) << " pressed";
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis2) << " x: " << state.rAxis[2].x << "  y: " << state.rAxis[0].y;
		}
		if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_Axis2)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis2) << " touched";
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis2) << " x: " << state.rAxis[2].x << "  y: " << state.rAxis[0].y;
		}
		if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Axis3)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis3) << " pressed";
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis3) << " x: " << state.rAxis[3].x << "  y: " << state.rAxis[0].y;
		}
		if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_Axis3)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis3) << " touched";
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis3) << " x: " << state.rAxis[3].x << "  y: " << state.rAxis[0].y;
		}
		if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Axis4)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis4) << " pressed";
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis4) << " x: " << state.rAxis[4].x << "  y: " << state.rAxis[0].y;
		}
		if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_Axis4)) {
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis4) << " touched";
			LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_Axis4) << " x: " << state.rAxis[4].x << "  y: " << state.rAxis[0].y;
		}
	}

	void AudioTabController::eventLoopTick() {
		if (!eventLoopMutex.try_lock()) {
			return;
		}
		if (settingsUpdateCounter >= 50) {
			vr::EVRSettingsError vrSettingsError;
			char mirrorDeviceId[1024];
			vr::VRSettings()->GetString(vr::k_pch_audio_Section, vr::k_pch_audio_OnPlaybackMirrorDevice_String, mirrorDeviceId, 1024, &vrSettingsError);
			if (vrSettingsError != vr::VRSettingsError_None) {
				LOG(WARNING) << "Could not read \"" << vr::k_pch_audio_OnPlaybackMirrorDevice_String << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
			}
			if (lastMirrorDevId.compare(mirrorDeviceId) != 0) {
				audioManager->setMirrorDevice(mirrorDeviceId);
				if (audioManager->isMirrorValid()) {
					if (!m_mirrorPresent) {
						m_mirrorPresent = true;
						emit mirrorPresentChanged(m_mirrorPresent);
					}
					m_mirrorDevName = QString::fromStdString(audioManager->getMirrorDevName());
					emit mirrorDevNameChanged(m_mirrorDevName);
				} else if (m_mirrorPresent) {
					m_mirrorPresent = false;
					emit mirrorPresentChanged(m_mirrorPresent);
				}
				lastMirrorDevId = mirrorDeviceId;
			}
			if (m_mirrorPresent) {
				setMirrorVolume(audioManager->getMirrorVolume());
				setMirrorMuted(audioManager->getMirrorMuted());
			}
			if (m_micPresent) {
				setMicVolume(audioManager->getMicVolume());
				setMicMuted(audioManager->getMicMuted());
			}
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
					if (vr::VRSystem()->GetControllerState(leftId, &state)) {
						//logControllerState(state, "Left ");
						newState |= handleControllerState(state, m_pttControllerConfigs);
					}
				}
			}
			if (m_pttRightControllerEnabled) {
				auto rightId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
				if (rightId != vr::k_unTrackedDeviceIndexInvalid) {
					vr::VRControllerState_t state;
					if (vr::VRSystem()->GetControllerState(rightId, &state)) {
						//logControllerState(state, "Right ");
						newState |= handleControllerState(state, m_pttControllerConfigs + 1);
					}
				}
			}
			if (newState && !m_pttActive) {
				if (audioManager && audioManager->isMicValid()) {
					m_pttActive = true;
					setMicMuted(false);
					if (m_pttShowNotification && m_ulNotificationOverlayHandle != vr::k_ulOverlayHandleInvalid) {
						vr::VROverlay()->ShowOverlay(m_ulNotificationOverlayHandle);
					}
					emit pttActiveChanged(m_pttActive);
				}
			} else if (!newState && m_pttActive) {
				if (audioManager && audioManager->isMicValid()) {
					m_pttActive = false;
					setMicMuted(true);
					if (m_pttShowNotification && m_ulNotificationOverlayHandle != vr::k_ulOverlayHandleInvalid) {
						vr::VROverlay()->HideOverlay(m_ulNotificationOverlayHandle);
					}
					emit pttActiveChanged(m_pttActive);
				}
			}
		}
		eventLoopMutex.unlock();
	}

	void AudioTabController::addPttProfile(QString name) {
		auto i = pttProfiles.size();
		pttProfiles.emplace_back();
		auto& profile = pttProfiles[i];
		profile.profileName = name.toStdString();
		profile.showNotification = m_pttShowNotification;
		profile.leftControllerEnabled = m_pttLeftControllerEnabled;
		profile.rightControllerEnabled = m_pttRightControllerEnabled;
		profile.controllerConfigs[0].digitalButtons = m_pttControllerConfigs[0].digitalButtons;
		profile.controllerConfigs[0].digitalButtonMask = m_pttControllerConfigs[0].digitalButtonMask;
		profile.controllerConfigs[0].triggerMode = m_pttControllerConfigs[0].triggerMode;
		profile.controllerConfigs[0].touchpadMode = m_pttControllerConfigs[0].touchpadMode;
		profile.controllerConfigs[0].touchpadAreas = m_pttControllerConfigs[0].touchpadAreas;
		profile.controllerConfigs[1].digitalButtons = m_pttControllerConfigs[1].digitalButtons;
		profile.controllerConfigs[1].digitalButtonMask = m_pttControllerConfigs[1].digitalButtonMask;
		profile.controllerConfigs[1].triggerMode = m_pttControllerConfigs[1].triggerMode;
		profile.controllerConfigs[1].touchpadMode = m_pttControllerConfigs[1].touchpadMode;
		profile.controllerConfigs[1].touchpadAreas = m_pttControllerConfigs[1].touchpadAreas;
		savePttProfiles();
		OverlayController::appSettings()->sync();
		emit pttProfilesUpdated();
	}

	void AudioTabController::applyPttProfile(unsigned index) {
		std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
		if (index < pttProfiles.size()) {
			auto& profile = pttProfiles[index];
			setPttShowNotification(profile.showNotification);
			setPttLeftControllerEnabled(profile.leftControllerEnabled);
			setPttRightControllerEnabled(profile.rightControllerEnabled);
			m_pttControllerConfigs[0].digitalButtons = profile.controllerConfigs[0].digitalButtons;
			m_pttControllerConfigs[0].digitalButtonMask = profile.controllerConfigs[0].digitalButtonMask;
			m_pttControllerConfigs[0].triggerMode = profile.controllerConfigs[0].triggerMode;
			m_pttControllerConfigs[0].touchpadMode = profile.controllerConfigs[0].touchpadMode;
			m_pttControllerConfigs[0].touchpadAreas = profile.controllerConfigs[0].touchpadAreas;
			m_pttControllerConfigs[1].digitalButtons = profile.controllerConfigs[1].digitalButtons;
			m_pttControllerConfigs[1].digitalButtonMask = profile.controllerConfigs[1].digitalButtonMask;
			m_pttControllerConfigs[1].triggerMode = profile.controllerConfigs[1].triggerMode;
			m_pttControllerConfigs[1].touchpadMode = profile.controllerConfigs[1].touchpadMode;
			m_pttControllerConfigs[1].touchpadAreas = profile.controllerConfigs[1].touchpadAreas;
		}
	}

	void AudioTabController::deletePttProfile(unsigned index) {
		if (index < pttProfiles.size()) {
			auto pos = pttProfiles.begin() + index;
			pttProfiles.erase(pos);
			savePttProfiles();
			OverlayController::appSettings()->sync();
			emit pttProfilesUpdated();
		}
	}

	QVariantList AudioTabController::pttDigitalButtons(unsigned controller) {
		if (controller < 2) {
			return m_pttControllerConfigs[controller].digitalButtons;
		}
		return QVariantList();
	}

	unsigned long AudioTabController::pttDigitalButtonMask(unsigned controller) {
		if (controller < 2) {
			return m_pttControllerConfigs[controller].digitalButtonMask;
		}
		return 0;
	}

	unsigned AudioTabController::pttTouchpadMode(unsigned controller) {
		if (controller < 2) {
			return m_pttControllerConfigs[controller].touchpadMode;
		}
		return 0;
	}

	unsigned AudioTabController::pttTriggerMode(unsigned controller) {
		if (controller < 2) {
			return m_pttControllerConfigs[controller].triggerMode;
		}
		return 0;
	}

	unsigned AudioTabController::pttTouchpadArea(unsigned controller) {
		if (controller < 2) {
			return m_pttControllerConfigs[controller].touchpadAreas;
		}
		return 0;
	}

	unsigned AudioTabController::getPttProfileCount() {
		return (unsigned)pttProfiles.size();
	}

	QString AudioTabController::getPttProfileName(unsigned index) {
		if (index < pttProfiles.size()) {
			return QString::fromStdString(pttProfiles[index].profileName);
		}
		return "";
	}

	void AudioTabController::setMirrorVolume(float value, bool notify) {
		std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
		if (value != m_mirrorVolume) {
			m_mirrorVolume = value;
			if (audioManager->isMirrorValid()) {
				audioManager->setMirrorVolume(value);
			}
			if (notify) {
				emit mirrorVolumeChanged(value);
			}
		}
	}

	void AudioTabController::setMirrorMuted(bool value, bool notify) {
		std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
		if (value != m_mirrorMuted) {
			m_mirrorMuted = value;
			if (audioManager->isMirrorValid()) {
				audioManager->setMirrorMuted(value);
			}
			if (notify) {
				emit mirrorMutedChanged(value);
			}
		}
	}

	void AudioTabController::setMicVolume(float value, bool notify) {
		std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
		if (value != m_micVolume) {
			m_micVolume = value;
			if (audioManager->isMicValid()) {
				audioManager->setMicVolume(value);
			}
			if (notify) {
				emit micVolumeChanged(value);
			}
		}
	}

	void AudioTabController::setMicMuted(bool value, bool notify) {
		std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
		if (value != m_micMuted) {
			m_micMuted = value;
			if (audioManager->isMicValid()) {
				audioManager->setMicMuted(value);
			}
			if (notify) {
				emit micMutedChanged(value);
			}
		}
	}

	void AudioTabController::setPttEnabled(bool value, bool notify, bool save) {
		std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
		if (value != m_pttEnabled) {
			m_pttEnabled = value;
			if (value) {
				setMicMuted(true);
			} else {
				setMicMuted(false);
			}
			if (notify) {
				emit pttEnabledChanged(value);
			}
			if (save) {
				savePttConfig();
			}
		}
	}

	void AudioTabController::setPttShowNotification(bool value, bool notify, bool save) {
		std::lock_guard<std::recursive_mutex> lock(eventLoopMutex);
		if (value != m_pttShowNotification) {
			m_pttShowNotification = value;
			if (notify) {
				emit pttShowNotificationChanged(value);
			}
			if (save) {
				savePttConfig();
			}
		}
	}

	void AudioTabController::setPttLeftControllerEnabled(bool value, bool notify, bool save) {
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

	void AudioTabController::setPttRightControllerEnabled(bool value, bool notify, bool save) {
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


	void AudioTabController::onNewRecordingDevice() {
		if (audioManager->isMicValid()) {
			if (!m_micPresent) {
				m_micPresent = true;
				emit micPresentChanged(m_micPresent);
			}
			setMicVolume(audioManager->getMicVolume());
			setMicMuted(audioManager->getMicMuted());
			m_micDevName = QString::fromStdString(audioManager->getMicDevName());

			emit micDevNameChanged(m_micDevName);
		} else {
			if (m_micPresent) {
				m_micPresent = false;
				emit micPresentChanged(m_micPresent);
			}
		}
	}

} // namespace advconfig
