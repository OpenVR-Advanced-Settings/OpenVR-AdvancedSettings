
#pragma once

#include "AudioManager.h"
#include "PttController.h"
#include <memory>

class QQuickWindow;
// application namespace
namespace advsettings {

	// forward declaration
	class OverlayController;


	class AudioTabController : public PttController {
		Q_OBJECT
		Q_PROPERTY(bool mirrorPresent READ mirrorPresent NOTIFY mirrorPresentChanged)
		Q_PROPERTY(QString mirrorDevName READ mirrorDevName NOTIFY mirrorDevNameChanged)
		Q_PROPERTY(float mirrorVolume READ mirrorVolume WRITE setMirrorVolume NOTIFY mirrorVolumeChanged)
		Q_PROPERTY(bool mirrorMuted READ mirrorMuted WRITE setMirrorMuted NOTIFY mirrorMutedChanged)
		Q_PROPERTY(bool micPresent READ micPresent NOTIFY micPresentChanged)
		Q_PROPERTY(QString micDevName READ micDevName NOTIFY micDevNameChanged)
		Q_PROPERTY(float micVolume READ micVolume WRITE setMicVolume NOTIFY micVolumeChanged)
		Q_PROPERTY(bool micMuted READ micMuted WRITE setMicMuted NOTIFY micMutedChanged)

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

		unsigned settingsUpdateCounter = 0;

		std::unique_ptr<AudioManager> audioManager;
		std::string lastMirrorDevId;

		QString getSettingsName() override { return "audioSettings"; }
		void onPttStart() override;
		void onPttStop() override;
		void onPttEnabled() override;
		void onPttDisabled() override;
		bool pttChangeValid() override;
		virtual vr::VROverlayHandle_t getNotificationOverlayHandle() override { return m_ulNotificationOverlayHandle; }

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

		void onNewRecordingDevice();

	public slots:
		void setMirrorVolume(float value, bool notify = true);
		void setMirrorMuted(bool value, bool notify = true);

		void setMicVolume(float value, bool notify = true);
		void setMicMuted(bool value, bool notify = true);

	signals:
		void mirrorPresentChanged(bool value);
		void mirrorDevNameChanged(QString value);
		void mirrorVolumeChanged(float value);
		void mirrorMutedChanged(bool value);

		void micPresentChanged(bool value);
		void micDevNameChanged(QString value);
		void micVolumeChanged(float value);
		void micMutedChanged(bool value);
	};

} // namespace advsettings
