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
		checkPttStatus();
		eventLoopMutex.unlock();
	}

	bool AudioTabController::pttChangeValid() {
		return audioManager && audioManager->isMicValid();
	}

	void AudioTabController::onPttStart() {
		setMicMuted(false);
	}

	void AudioTabController::onPttEnabled() {
		setMicMuted(true);
	}

	void AudioTabController::onPttStop() {
		setMicMuted(true);
	}

	void AudioTabController::onPttDisabled() {
		setMicMuted(false);
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
