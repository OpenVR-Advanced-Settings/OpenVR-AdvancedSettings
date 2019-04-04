#include "VideoTabController.h"
#include <QQuickWindow>
#include <QApplication>
#include "../overlaycontroller.h"

namespace advsettings
{
	void VideoTabController::initStage1() 
	{
		//reloadVideoConfig();
	}

//TODO args
	void VideoTabController::initStage2(OverlayController* var_parent,
		QQuickWindow* var_widget)
	{
		this->parent = var_parent;
		this->widget = var_widget;

		std::string notifKey = std::string(application_strings::applicationKey)
			+ ".dimnotification";

		vr::VROverlayError overlayError = vr::VROverlay()->CreateOverlay(
			notifKey.c_str(), notifKey.c_str(), &m_brightnessNotificationOverlayHandle);
		if (overlayError == vr::VROverlayError_None)
		{
			constexpr auto notificationIconFilename
				= "/res/img/video/dimmer.png";
			const auto notifIconPath
				= paths::binaryDirectoryFindFile(notificationIconFilename);
			if (notifIconPath.has_value())
			{
				vr::VROverlay()->SetOverlayFromFile(m_brightnessNotificationOverlayHandle,
					notifIconPath->c_str());
				vr::VROverlay()->SetOverlayWidthInMeters(
					m_ulNotificationOverlayHandle, 1.0f);
				vr::HmdMatrix34_t notificationTransform
					= { { { 1.0f, 0.0f, 0.0f, 0.00f },
				{ 0.0f, 1.0f, 0.0f, 0.00f },
				{ 0.0f, 0.0f, 1.0f, -0.3f } } };
				vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(
					m_brightnessNotificationOverlayHandle,
					vr::k_unTrackedDeviceIndex_Hmd,
					&notificationTransform);
			}
			else
			{
				LOG(ERROR) << "Could not find notification icon \""
					<< notificationIconFilename << "\"";
			}
		}
		else
		{
			LOG(ERROR) << "Could not create dimmer notification overlay: "
				<< vr::VROverlay()->GetOverlayErrorNameFromEnum(
					overlayError);
		}
		//emit defaultProfileDisplay();
	}

	void VideoTabController::eventLoopTick() 
	{

	}

	void VideoTabController::reloadVideoConfig() {
		//Others Use Mutex needed?
		auto settings = OverlayController::appSettings();
		settings->beginGroup(getSettingsName());

		//TODO might need emit params
		setBrightnessEnabled(settings->value("brightnessEnabled", false).toBool());
		setBrightnessLevel(settings->value("brightnessValue", 100).toFloat());
		settings->endGroup();

	}

	void VideoTabController::saveVideoConfig() {
		auto settings = OverlayController::appSettings();
		settings->beginGroup(getSettingsName());

		settings->setValue("brightnessEnabled", brightnessEnabled());
		settings->setValue("brightnessValue", brightnessValue());
	}
	void VideoTabController::saveVideoConfig() {

	}
}