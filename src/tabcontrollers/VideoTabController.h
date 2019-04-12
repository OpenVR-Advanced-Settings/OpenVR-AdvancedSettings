#pragma once


#include <QObject>
#include <QString>
#include <QVariant>
#include <openvr.h>


class QQuickWindow;

namespace video_keys {

	constexpr auto k_brightnessOverlayFilename = "/res/img/video/dimmer.png";

}//namespace video_keys

namespace advsettings {
	// forward declaration
	class OverlayController;

	class VideoTabController : public QObject
	{
		Q_OBJECT
			Q_PROPERTY(bool brightnessEnabled READ brightnessEnabled WRITE
				setBrightnessEnabled NOTIFY brightnessEnabledChanged)
			Q_PROPERTY(float brightnessValue READ brightnessValue WRITE
				setBrightnessValue NOTIFY brightnessValueChanged)

	private:

		// how far away the overlay is, any OVERLAY closer will not be dimmed.
		const float k_hmdDistance = -0.15f;
		// how wide overlay is, Increase this value if edges of view are not dimmed.
		const float k_overlayWidth = 1.0f;
		OverlayController * parent;
		QQuickWindow* widget;

		vr::VROverlayHandle_t m_brightnessNotificationOverlayHandle
			= vr::k_ulOverlayHandleInvalid;
		float m_opacityValue = 0.0f;
		float m_brightnessValue = 1.0f;
		bool m_brightnessEnabled = false;


		void reloadVideoConfig();
		void saveVideoConfig();


		QString getSettingsName(){
			return "videoSettings";
		}

		vr::VROverlayHandle_t getBrightnessOverlayHandle()
		{
			return m_brightnessNotificationOverlayHandle;
		}


	public:

		
		float opacityValue() const;
		//inverse of brightnessValue
		float brightnessValue() const;
		bool brightnessEnabled() const;

		void initStage1();
		void initStage2(OverlayController* var_parent, QQuickWindow* var_widget);
		void eventLoopTick();


	public slots:
		void setBrightnessEnabled(bool value, bool notify = true);
		void setBrightnessValue(float percvalue, bool notify = true);


	signals:
		void brightnessEnabledChanged(bool value);
		void brightnessValueChanged(float value);
	};

}// namespace advsettings