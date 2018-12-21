
#pragma once

#include "AudioManager.h"
#include "PttController.h"
//#include "AudioProfileController.h"
#include <memory>

class QQuickWindow;
// application namespace
namespace advsettings {

	// forward declaration
	class OverlayController;

	struct AudioProfile {
		std::string profileName;
		std::string playbackName;
		std::string mirrorName;
		std::string micName;
		float mirrorVol = 0.0;
		float micVol = 0.0;
		bool micMute = false;
		bool mirrorMute = false;
		//TODO all the settings!!! huehue
		//bool showNotification = false;
	};


	class AudioTabController : public PttController {
		Q_OBJECT
		Q_PROPERTY(int playbackDeviceIndex READ playbackDeviceIndex WRITE setPlaybackDeviceIndex NOTIFY playbackDeviceIndexChanged)
		Q_PROPERTY(int mirrorDeviceIndex READ mirrorDeviceIndex WRITE setMirrorDeviceIndex NOTIFY mirrorDeviceIndexChanged)
		//Q_PROPERTY(bool mirrorPresent READ mirrorPresent NOTIFY mirrorPresentChanged)
		//Q_PROPERTY(QString mirrorDevName READ mirrorDevName NOTIFY mirrorDevNameChanged)
		Q_PROPERTY(float mirrorVolume READ mirrorVolume WRITE setMirrorVolume NOTIFY mirrorVolumeChanged)
		Q_PROPERTY(bool mirrorMuted READ mirrorMuted WRITE setMirrorMuted NOTIFY mirrorMutedChanged)
		Q_PROPERTY(int micDeviceIndex READ micDeviceIndex WRITE setMicDeviceIndex NOTIFY micDeviceIndexChanged)
		//Q_PROPERTY(bool micPresent READ micPresent NOTIFY micPresentChanged)
		//Q_PROPERTY(QString micDevName READ micDevName NOTIFY micDevNameChanged)
		Q_PROPERTY(float micVolume READ micVolume WRITE setMicVolume NOTIFY micVolumeChanged)
		Q_PROPERTY(bool micMuted READ micMuted WRITE setMicMuted NOTIFY micMutedChanged)
		Q_PROPERTY(bool micProximitySensorCanMute READ micProximitySensorCanMute WRITE setMicProximitySensorCanMute NOTIFY micProximitySensorCanMuteChanged)
		Q_PROPERTY(bool micReversePtt READ micReversePtt WRITE setMicReversePtt NOTIFY micReversePttChanged)

	private:
		OverlayController* parent;
		QQuickWindow* widget;

		vr::VROverlayHandle_t m_ulNotificationOverlayHandle = vr::k_ulOverlayHandleInvalid;

		int m_playbackDeviceIndex = -1;

		int m_mirrorDeviceIndex = -1;
		float m_mirrorVolume = 1.0;
		bool m_mirrorMuted = false;

		int m_recordingDeviceIndex = -1;
		float m_micVolume = 1.0;
		bool m_micMuted = false;
		bool m_micProximitySensorCanMute = false;
		bool m_micReversePtt = false;

		unsigned settingsUpdateCounter = 0;

		std::unique_ptr<AudioManager> audioManager;
		std::vector<std::pair<std::string, std::string>> m_recordingDevices;
		std::vector<std::pair<std::string, std::string>> m_playbackDevices;
		std::string lastMirrorDevId;

		QString getSettingsName() override { return "audioSettings"; }
		//QString getSettingsNameAudio() override { return "audioSettingsTest"; }
		void onPttStart() override;
		void onPttStop() override;
		void onPttEnabled() override;
		void onPttDisabled() override;
		bool pttChangeValid() override;
		virtual vr::VROverlayHandle_t getNotificationOverlayHandle() override { return m_ulNotificationOverlayHandle; }

		void findPlaybackDeviceIndex(std::string id, bool notify = true);
		void findMirrorDeviceIndex(std::string id, bool notify = true);
		void findMicDeviceIndex(std::string id, bool notify = true);

		void setPlaybackDeviceControl(int index, bool notify = true);

		std::vector<AudioProfile> audioProfiles;
		//std::recursive_mutex eventLoopMutexAudio;

	public:


		void initStage1();
		void initStage2(OverlayController* parent, QQuickWindow* widget);

		void reloadAudioSettings();
		void saveAudioSettings();

		void eventLoopTick();

		int playbackDeviceIndex() const;

		int mirrorDeviceIndex() const;
		float mirrorVolume() const;
		bool mirrorMuted() const;

		int micDeviceIndex() const;
		float micVolume() const;
		bool micMuted() const;
		bool micProximitySensorCanMute() const;
		bool micReversePtt() const;

		void reloadAudioProfiles();
		void saveAudioProfiles();

		Q_INVOKABLE int getPlaybackDeviceCount();
		Q_INVOKABLE QString getPlaybackDeviceName(int index);

		Q_INVOKABLE int getRecordingDeviceCount();
		Q_INVOKABLE QString getRecordingDeviceName(int index);

		//TODO check if needed?
		Q_INVOKABLE unsigned getAudioProfileCount();
		Q_INVOKABLE QString getAudioProfileName(unsigned index);

		void onNewRecordingDevice();
		void onNewPlaybackDevice();
		void onNewMirrorDevice();
		void onDeviceStateChanged();

	public slots:
		void setMirrorVolume(float value, bool notify = true);
		void setMirrorMuted(bool value, bool notify = true);

		void setMicVolume(float value, bool notify = true);
		void setMicMuted(bool value, bool notify = true);
		void setMicProximitySensorCanMute(bool value, bool notify = true);
		void setMicReversePtt(bool value, bool notify = true);

		void setPlaybackDeviceIndex(int value, bool notify = true);
		void setMirrorDeviceIndex(int value, bool notify = true);
		void setMicDeviceIndex(int value, bool notify = true);

		void addAudioProfile(QString name);
		void applyAudioProfile(unsigned index);
		void deleteAudioProfile(unsigned index);

	signals:
		void playbackDeviceIndexChanged(int index);

		void mirrorDeviceIndexChanged(int index);
		void mirrorVolumeChanged(float value);
		void mirrorMutedChanged(bool value);

		void micDeviceIndexChanged(int index);
		void micVolumeChanged(float value);
		void micMutedChanged(bool value);
		void micProximitySensorCanMuteChanged(bool value);
		void micReversePttChanged(bool value);

		void playbackDeviceListChanged();
		void recordingDeviceListChanged();

		void audioProfilesUpdated();
	};

} // namespace advsettings
