#pragma once

namespace advsettings {

	Q_OBJECT
		Q_PROPERTY(int playbackDeviceIndex READ playbackDeviceIndex WRITE
			setPlaybackDeviceIndex NOTIFY playbackDeviceIndexChanged)
		Q_PROPERTY(int mirrorDeviceIndex READ mirrorDeviceIndex WRITE
			setMirrorDeviceIndex NOTIFY mirrorDeviceIndexChanged)

private:
	//TODO
	constexpr auto notificationBrightnessFilename
		= "todo";
	vr::VROverlayHandle_t m_brightnessNotificationOverlayHandle
		= vr::k_ulOverlayHandleInvalid;
	float m_brightnessValue = 1.0;
	bool m_brightnessEnabled = false;

	void reloadVideoConfig();
	void saveVideoConfig();


	QString getSettingsName() override
	{
		return "videoSettings";
	}

	vr::VROverlayHandle_t getBrightnessOverlayHandle()
	{
		return m_brightnessNotificationOverlayHandle;
	}


public:

	void initStage1();
	//TODO ?? on args
	void initStage2(OverlayController* var_parent, QQuickWindow* var_widget);

}