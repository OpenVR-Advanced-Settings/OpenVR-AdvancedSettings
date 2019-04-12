#include "VideoTabController.h"
#include <QQuickWindow>
#include <QApplication>
#include "../overlaycontroller.h"
#include <math.h>

namespace advsettings
{
	void VideoTabController::initStage1() 
	{
		
	}

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
			const auto notifIconPath
				= paths::binaryDirectoryFindFile(video_keys::k_brightnessOverlayFilename);
			if (notifIconPath.has_value())
			{
				vr::VROverlay()->SetOverlayFromFile(m_brightnessNotificationOverlayHandle,
					notifIconPath->c_str());
				vr::VROverlay()->SetOverlayWidthInMeters(
					m_brightnessNotificationOverlayHandle, k_overlayWidth);
				vr::HmdMatrix34_t notificationTransform
					= { { { 1.0f, 0.0f, 0.0f, 0.00f },
				{ 0.0f, 1.0f, 0.0f, 0.00f },
				{ 0.0f, 0.0f, 1.0f, k_hmdDistance } } };
				vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(
					m_brightnessNotificationOverlayHandle,
					vr::k_unTrackedDeviceIndex_Hmd,
					&notificationTransform);
			}
			else
			{
				LOG(ERROR) << "Could not find notification icon \""
					<< video_keys::k_brightnessOverlayFilename << "\"";
			}
		}
		else
		{
			LOG(ERROR) << "Could not create dimmer notification overlay: "
				<< vr::VROverlay()->GetOverlayErrorNameFromEnum(
					overlayError);
		}

		reloadVideoConfig();
	}

	void VideoTabController::eventLoopTick() 
	{

	}

	void VideoTabController::reloadVideoConfig() {
		auto settings = OverlayController::appSettings();
		settings->beginGroup(getSettingsName());
		setBrightnessEnabled(settings->value("brightnessEnabled", false).toBool(), true);
		m_opacityValue = settings->value("opacityValue", 0.0f).toFloat();
		setBrightnessValue(settings->value("brightnessValue", 1.0f).toFloat(), true);
		settings->endGroup();

	}
	

	
	void VideoTabController::saveVideoConfig() {
		auto settings = OverlayController::appSettings();
		settings->beginGroup(getSettingsName());
		settings->setValue("brightnessEnabled", brightnessEnabled());
		settings->setValue("opacityValue", opacityValue());
		settings->setValue("brightnessValue", brightnessValue());
		settings->endGroup();

	}
	
	float VideoTabController::opacityValue() const {
		return m_opacityValue;
	}

	float VideoTabController::brightnessValue() const
	{
		return m_brightnessValue;
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
					LOG(INFO) << "Brightness Overlay toggled on";
				}
			}
			else {
				if (getBrightnessOverlayHandle() != vr::k_ulOverlayHandleInvalid)
				{
					vr::VROverlay()->HideOverlay(getBrightnessOverlayHandle());
					LOG(INFO) << "Brightness Overlay toggled off";
				}
			}
			saveVideoConfig();
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
			m_brightnessValue = percvalue;
			if (realvalue <= 1.0f && realvalue >= 0.0f) {
				m_opacityValue = realvalue;
				vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayAlpha(m_brightnessNotificationOverlayHandle, realvalue);
				if (overlayError != vr::VROverlayError_None) {
					LOG(ERROR) << "Could not set alpha: "
						<< vr::VROverlay()->GetOverlayErrorNameFromEnum(
							overlayError);
				}
				//only saves if opacity Value is valid. [1-0]
				saveVideoConfig();
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