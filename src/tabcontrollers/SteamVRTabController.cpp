#include "SteamVRTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings {


void SteamVRTabController::initStage1() {
	eventLoopTick();
}


void SteamVRTabController::initStage2(OverlayController * parent, QQuickWindow * widget) {
	this->parent = parent;
	this->widget = widget;
}


void SteamVRTabController::eventLoopTick() {
	if (settingsUpdateCounter >= 50) {
		vr::EVRSettingsError vrSettingsError;
		auto ss = vr::VRSettings()->GetFloat(vr::k_pch_SteamVR_Section, vrsettings_steamvr_supersampleScale, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vrsettings_steamvr_supersampleScale << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
			if (m_superSampling != 1.0) {
				LOG(DEBUG) << "OpenVR returns an error and we have a custom supersampling value: " << m_superSampling;
				setSuperSampling(1.0);
			}
		} else if (m_superSampling != ss) {
			LOG(DEBUG) << "OpenVR reports a changed supersampling value: " << m_superSampling << " => " << ss;
			setSuperSampling(ss);
		}
		auto css = vr::VRSettings()->GetFloat(vrsettings_compositor_category, vr::k_pch_SteamVR_RenderTargetMultiplier_Float, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			if (vrSettingsError != vr::VRSettingsError_UnsetSettingHasNoDefault) { // does not appear in the default settings file as of beta 1477423729
				LOG(WARNING) << "Could not read \"compositor::" << vr::k_pch_SteamVR_RenderTargetMultiplier_Float << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
			}
			if (m_compositorSuperSampling != 1.0) {
				LOG(DEBUG) << "OpenVR returns an error and we have a custom compositor supersampling value: " << m_compositorSuperSampling;
				setCompositorSuperSampling(1.0);
			}
		} else if (m_compositorSuperSampling != css) {
			LOG(DEBUG) << "OpenVR reports a changed compositor supersampling value: " << m_compositorSuperSampling << " => " << css;
			setCompositorSuperSampling(css);
		}
		auto air = vr::VRSettings()->GetBool(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_AllowReprojection_Bool, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_SteamVR_AllowReprojection_Bool << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
		setAllowInterleavedReprojection(air);
		auto aar = vr::VRSettings()->GetBool(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_AllowAsyncReprojection_Bool, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_SteamVR_AllowAsyncReprojection_Bool << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
		setAllowAsyncReprojection(aar);
		auto fr = vr::VRSettings()->GetBool(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_ForceReprojection_Bool, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_SteamVR_ForceReprojection_Bool << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
		setForceReprojection(fr);
		auto sf = vr::VRSettings()->GetBool(vr::k_pch_SteamVR_Section, vrsettings_steamvr_allowSupersampleFiltering, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vrsettings_steamvr_allowSupersampleFiltering << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
		setAllowSupersampleFiltering(sf);
		settingsUpdateCounter = 0;
	} else {
		settingsUpdateCounter++;
	}
}


void SteamVRTabController::setSuperSampling(float value, bool notify) {
	bool override = false;
	if (value <= 0.01f) {
		LOG(WARNING) << "Encountered a supersampling value <= 0.01, setting supersampling to 1.0";
		value = 1.0f;
		override = true;
	}
	if (override || m_superSampling != value) {
		LOG(DEBUG) << "Supersampling value changed: " << m_superSampling << " => " << value;
		m_superSampling = value;
		vr::VRSettings()->SetFloat(vr::k_pch_SteamVR_Section, vrsettings_steamvr_supersampleScale, m_superSampling);
		vr::VRSettings()->Sync();
		if (notify) {
			emit superSamplingChanged(m_superSampling);
		}
	}
}


void SteamVRTabController::setCompositorSuperSampling(float value, bool notify) {
	bool override = false;
	if (value <= 0.01f) {
		LOG(WARNING) << "Encountered a compositor supersampling value <= 0.01, setting supersampling to 1.0";
		value = 1.0f;
		override = true;
	}
	if (override || m_compositorSuperSampling != value) {
		LOG(DEBUG) << "Compositor supersampling value changed: " << m_compositorSuperSampling << " => " << value;
		m_compositorSuperSampling = value;
		vr::VRSettings()->SetFloat(vrsettings_compositor_category, vr::k_pch_SteamVR_RenderTargetMultiplier_Float, m_compositorSuperSampling);
		vr::VRSettings()->Sync();
		if (notify) {
			emit compositorSuperSamplingChanged(m_compositorSuperSampling);
		}
	}
}


float SteamVRTabController::superSampling() const {
	return m_superSampling;
}


float SteamVRTabController::compositorSuperSampling() const {
	return m_compositorSuperSampling;
}


bool SteamVRTabController::allowInterleavedReprojection() const {
	return m_allowInterleavedReprojection;
}


bool SteamVRTabController::allowAsyncReprojection() const {
	return m_allowAsyncReprojection;
}


void SteamVRTabController::setAllowInterleavedReprojection(bool value, bool notify) {
	if (m_allowInterleavedReprojection != value) {
		m_allowInterleavedReprojection = value;
		vr::VRSettings()->SetBool(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_AllowReprojection_Bool, m_allowInterleavedReprojection);
		vr::VRSettings()->Sync();
		if (notify) {
			emit allowInterleavedReprojectionChanged(m_allowInterleavedReprojection);
		}
	}
}


void SteamVRTabController::setAllowAsyncReprojection(bool value, bool notify) {
	if (m_allowAsyncReprojection != value) {
		m_allowAsyncReprojection = value;
		vr::VRSettings()->SetBool(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_AllowAsyncReprojection_Bool, m_allowAsyncReprojection);
		vr::VRSettings()->Sync();
		if (notify) {
			emit allowAsyncReprojectionChanged(m_allowAsyncReprojection);
		}
	}
}

bool SteamVRTabController::forceReprojection() const {
	return m_forceReprojection;
}


void SteamVRTabController::setForceReprojection(bool value, bool notify) {
	if (m_forceReprojection != value) {
		m_forceReprojection = value;
		vr::VRSettings()->SetBool(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_ForceReprojection_Bool, m_forceReprojection);
		vr::VRSettings()->Sync();
		if (notify) {
			emit forceReprojectionChanged(m_forceReprojection);
		}
	}
}


bool SteamVRTabController::allowSupersampleFiltering() const {
	return m_allowSupersampleFiltering;
}


void SteamVRTabController::setAllowSupersampleFiltering(bool value, bool notify) {
	if (m_allowSupersampleFiltering != value) {
		m_allowSupersampleFiltering = value;
		vr::VRSettings()->SetBool(vr::k_pch_SteamVR_Section, vrsettings_steamvr_allowSupersampleFiltering, m_allowSupersampleFiltering);
		vr::VRSettings()->Sync();
		if (notify) {
			emit allowSupersampleFilteringChanged(m_allowSupersampleFiltering);
		}
	}
}




void SteamVRTabController::reset() {
	vr::EVRSettingsError vrSettingsError;

	vr::VRSettings()->RemoveKeyInSection(vr::k_pch_SteamVR_Section, vrsettings_steamvr_supersampleScale, &vrSettingsError);
	if (vrSettingsError != vr::VRSettingsError_None) {
		LOG(WARNING) << "Could not remove \"" << vrsettings_steamvr_supersampleScale << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
	}

	setCompositorSuperSampling(1.0f); // Is not in default.vrsettings yet
	vr::VRSettings()->RemoveKeyInSection(vrsettings_compositor_category, vr::k_pch_SteamVR_RenderTargetMultiplier_Float, &vrSettingsError);
	if (vrSettingsError != vr::VRSettingsError_None) {
		LOG(WARNING) << "Could not remove \"" << vrsettings_compositor_category << "::" << vr::k_pch_SteamVR_RenderTargetMultiplier_Float << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
	}

	vr::VRSettings()->RemoveKeyInSection(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_AllowReprojection_Bool, &vrSettingsError);
	if (vrSettingsError != vr::VRSettingsError_None) {
		LOG(WARNING) << "Could not remove \"" << vr::k_pch_SteamVR_AllowReprojection_Bool << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
	}

	vr::VRSettings()->RemoveKeyInSection(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_AllowAsyncReprojection_Bool, &vrSettingsError);
	if (vrSettingsError != vr::VRSettingsError_None) {
		LOG(WARNING) << "Could not remove \"" << vr::k_pch_SteamVR_AllowAsyncReprojection_Bool << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
	}

	vr::VRSettings()->RemoveKeyInSection(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_ForceReprojection_Bool, &vrSettingsError);
	if (vrSettingsError != vr::VRSettingsError_None) {
		LOG(WARNING) << "Could not remove \"" << vr::k_pch_SteamVR_ForceReprojection_Bool << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
	}

	vr::VRSettings()->RemoveKeyInSection(vr::k_pch_SteamVR_Section, vrsettings_steamvr_allowSupersampleFiltering, &vrSettingsError);
	if (vrSettingsError != vr::VRSettingsError_None) {
		LOG(WARNING) << "Could not remove \"" << vrsettings_steamvr_allowSupersampleFiltering << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
	}

	vr::VRSettings()->Sync();
	settingsUpdateCounter = 999; // Easiest way to get default values
}


void SteamVRTabController::restartSteamVR() {
	QString cmd = QString("cmd.exe /C restartvrserver.bat \"") + parent->getVRRuntimePathUrl().toLocalFile() + QString("\"");
	LOG(INFO) << "SteamVR Restart Command: " << cmd;
	QProcess::startDetached(cmd);
}


} // namespace advconfig

