#pragma once


#include <QObject>
#include <QString>
#include <QVariant>
#include <openvr.h>
//#include "../overlaycontroller.h"


class QQuickWindow;

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
		//TODO
		//constexpr auto notificationBrightnessFilename
		OverlayController * parent;
		QQuickWindow* widget;

		vr::VROverlayHandle_t m_brightnessNotificationOverlayHandle
			= vr::k_ulOverlayHandleInvalid;
		float m_opacityValue = 0.0f;
		bool m_brightnessEnabled = false;

		//void reloadVideoConfig();
		//void saveVideoConfig();


		QString getSettingsName(){
			return "videoSettings";
		}

		vr::VROverlayHandle_t getBrightnessOverlayHandle()
		{
			return m_brightnessNotificationOverlayHandle;
		}


	public:
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

}// namespace