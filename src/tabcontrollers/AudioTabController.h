
#pragma once

#include "AudioManager.h"
#include <QObject>
#include <QString>
#include <QVariant>
#include <string>
#include <memory>
#include <mutex>
#include <openvr.h>

class QQuickWindow;
// application namespace
namespace advsettings {

	// forward declaration
	class OverlayController;

	enum TouchPadArea {
		PAD_AREA_LEFT = (1 << 0),
		PAD_AREA_TOP = (1 << 1),
		PAD_AREA_RIGHT = (1 << 2),
		PAD_AREA_BOTTOM = (1 << 3),
	};

	struct PttControllerConfig {
		QVariantList digitalButtons;
		uint64_t digitalButtonMask = 0;
		unsigned triggerMode = 0; // 0 .. disabled, 1 .. enabled
		unsigned touchpadMode = 0; // 0 .. disabled, 1 .. only touch, 2 .. only press, 3 .. both
		unsigned touchpadAreas = 0;
	};

	struct PttProfile {
		std::string profileName;

		bool showNotification = false;
		bool leftControllerEnabled = false;
		bool rightControllerEnabled = false;
		PttControllerConfig controllerConfigs[2];
	};


	class AudioTabController : public QObject {
		Q_OBJECT
		Q_PROPERTY(bool mirrorPresent READ mirrorPresent NOTIFY mirrorPresentChanged)
		Q_PROPERTY(QString mirrorDevName READ mirrorDevName NOTIFY mirrorDevNameChanged)
		Q_PROPERTY(float mirrorVolume READ mirrorVolume WRITE setMirrorVolume NOTIFY mirrorVolumeChanged)
		Q_PROPERTY(bool mirrorMuted READ mirrorMuted WRITE setMirrorMuted NOTIFY mirrorMutedChanged)
		Q_PROPERTY(bool micPresent READ micPresent NOTIFY micPresentChanged)
		Q_PROPERTY(QString micDevName READ micDevName NOTIFY micDevNameChanged)
		Q_PROPERTY(float micVolume READ micVolume WRITE setMicVolume NOTIFY micVolumeChanged)
		Q_PROPERTY(bool micMuted READ micMuted WRITE setMicMuted NOTIFY micMutedChanged)
		Q_PROPERTY(bool pttEnabled READ pttEnabled WRITE setPttEnabled NOTIFY pttEnabledChanged)
		Q_PROPERTY(bool pttActive READ pttActive NOTIFY pttActiveChanged)
		Q_PROPERTY(bool pttShowNotification READ pttShowNotification WRITE setPttShowNotification NOTIFY pttShowNotificationChanged)
		Q_PROPERTY(bool pttLeftControllerEnabled READ pttLeftControllerEnabled WRITE setPttLeftControllerEnabled NOTIFY pttLeftControllerEnabledChanged)
		Q_PROPERTY(bool pttRightControllerEnabled READ pttRightControllerEnabled WRITE setPttRightControllerEnabled NOTIFY pttRightControllerEnabledChanged)

	private:
		OverlayController* parent;
		QQuickWindow* widget;

		vr::VROverlayHandle_t m_ulNotificationOverlayHandle = vr::k_ulOverlayHandleInvalid;

		bool m_mirrorPresent = false;
		QString m_mirrorDevName = "<None>";
		float m_mirrorVolume = 1.0;
		bool m_mirrorMuted = false;

		bool m_micPresent = false;
		QString m_micDevName = "<None>";
		float m_micVolume = 1.0;
		bool m_micMuted = false;
		
		bool m_pttEnabled = false;
		bool m_pttActive = false;
		bool m_pttShowNotification = false;
		bool m_pttLeftControllerEnabled = false;
		bool m_pttRightControllerEnabled = false;
		PttControllerConfig m_pttControllerConfigs[2];

		unsigned settingsUpdateCounter = 0;

		std::vector<PttProfile> pttProfiles;

		std::unique_ptr<AudioManager> audioManager;
		std::string lastMirrorDevId;

		std::recursive_mutex eventLoopMutex;

	public:
		void initStage1();
		void initStage2(OverlayController* parent, QQuickWindow* widget);

		void eventLoopTick();

		bool mirrorPresent() const;
		QString mirrorDevName() const;
		float mirrorVolume() const;
		bool mirrorMuted() const;

		bool micPresent() const;
		QString micDevName() const;
		float micVolume() const;
		bool micMuted() const;

		bool pttEnabled() const;
		bool pttActive() const;
		bool pttShowNotification() const;
		bool pttLeftControllerEnabled() const;
		bool pttRightControllerEnabled() const;

		void reloadPttProfiles();
		void reloadPttConfig();
		void savePttProfiles();
		void savePttConfig();

		void onNewRecordingDevice();

		Q_INVOKABLE QVariantList pttDigitalButtons(unsigned controller);
		Q_INVOKABLE unsigned long pttDigitalButtonMask(unsigned controller);
		Q_INVOKABLE unsigned pttTouchpadMode(unsigned controller);
		Q_INVOKABLE unsigned pttTriggerMode(unsigned controller);
		Q_INVOKABLE unsigned pttTouchpadArea(unsigned controller);

		Q_INVOKABLE unsigned getPttProfileCount();
		Q_INVOKABLE QString getPttProfileName(unsigned index);

	public slots:
		void setMirrorVolume(float value, bool notify = true);
		void setMirrorMuted(bool value, bool notify = true);

		void setMicVolume(float value, bool notify = true);
		void setMicMuted(bool value, bool notify = true);

		void setPttEnabled(bool value, bool notify = true, bool save = true);
		void setPttShowNotification(bool value, bool notify = true, bool save = true);
		void setPttLeftControllerEnabled(bool value, bool notify = true, bool save = true);
		void setPttRightControllerEnabled(bool value, bool notify = true, bool save = true);

		void setPttControllerConfig(unsigned controller, QVariantList buttons, unsigned triggerMode, unsigned padMode, unsigned padAreas);

		void addPttProfile(QString name);
		void applyPttProfile(unsigned index);
		void deletePttProfile(unsigned index);

	signals:
		void mirrorPresentChanged(bool value);
		void mirrorDevNameChanged(QString value);
		void mirrorVolumeChanged(float value);
		void mirrorMutedChanged(bool value);

		void micPresentChanged(bool value);
		void micDevNameChanged(QString value);
		void micVolumeChanged(float value);
		void micMutedChanged(bool value);

		void pttEnabledChanged(bool value);
		void pttActiveChanged(bool value);
		void pttShowNotificationChanged(bool value);
		void pttLeftControllerEnabledChanged(bool value);
		void pttRightControllerEnabledChanged(bool value);

		void pttProfilesUpdated();
	};

} // namespace advsettings
