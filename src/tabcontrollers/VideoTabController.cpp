#include "VideoTabController.h"
#include <QQuickWindow>
#include <QApplication>
#include "../overlaycontroller.h"
#include <math.h>

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
					m_brightnessNotificationOverlayHandle, 1.0f);
				//Places the "overlay just past eye sight 1mx1m
				vr::HmdMatrix34_t notificationTransform
					= { { { 1.0f, 0.0f, 0.0f, 0.00f },
				{ 0.0f, 1.0f, 0.0f, 0.00f },
				{ 0.0f, 0.0f, 1.0f, -0.15f } } };
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

	/*void VideoTabController::reloadVideoConfig() {
		//Others Use Mutex needed?
		auto settings = OverlayController::appSettings();
		settings->beginGroup(getSettingsName());

		//TODO might need emit params
		setBrightnessEnabled(settings->value("brightnessEnabled", false).toBool());
		setBrightnessLevel(settings->value("brightnessValue", 100).toFloat());
		settings->endGroup();

	}
	*/

	/*
	void VideoTabController::saveVideoConfig() {
		auto settings = OverlayController::appSettings();
		settings->beginGroup(getSettingsName());

		settings->setValue("brightnessEnabled", brightnessEnabled());
		settings->setValue("brightnessValue", brightnessValue());
	}
	void VideoTabController::saveVideoConfig() {

	}
	*/

	float VideoTabController::brightnessValue() const
	{
		return (1.0f - m_opacityValue);
	}

	bool VideoTabController::brightnessEnabled() const
	{
		return m_brightnessEnabled;
	}
	

	void VideoTabController::setBrightnessEnabled(bool value, bool notify)
	{
		if (value != m_brightnessEnabled)
		{
			m_brightnessEnabled = value;
			if (value) {
				if(getBrightnessOverlayHandle() != vr::k_ulOverlayHandleInvalid)
				{
					vr::VROverlay()->ShowOverlay(getBrightnessOverlayHandle());
					LOG(INFO) << "overlay is on";
				}
			}
			else {
				if (getBrightnessOverlayHandle() != vr::k_ulOverlayHandleInvalid)
				{
					vr::VROverlay()->HideOverlay(getBrightnessOverlayHandle());
				}
			}
			if (notify)
			{
				emit brightnessEnabledChanged(value);
			}
		}
	}

	void VideoTabController::setBrightnessValue(float percvalue, bool notify)
	{

		//This takes the Perceived value, and converts it to allow more accurate linear positioning. (human perception logarithmic)
		float realvalue = static_cast<float>(sqrt(static_cast<double>(1.0f - percvalue)));

		if (realvalue != m_opacityValue)
		{
			m_opacityValue = realvalue;
			if (realvalue <= 1.0f && realvalue >= 0.0f) {
				vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayAlpha(m_brightnessNotificationOverlayHandle, realvalue);
				if (overlayError != vr::VROverlayError_None) {
					LOG(ERROR) << "Could not set alpha: "
						<< vr::VROverlay()->GetOverlayErrorNameFromEnum(
							overlayError);
				}
			}
			else {
				LOG(WARNING) << "alpha value is invalid setting to 1.0";
				m_opacityValue = 0.0f;
			}


			if (notify)
			{
				emit brightnessValueChanged(percvalue);
			}
		}
	}
}