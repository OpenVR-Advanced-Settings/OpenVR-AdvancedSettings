#include "SettingsTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings {


void SettingsTabController::initStage1() {
	m_autoStartEnabled = vr::VRApplications()->GetApplicationAutoLaunch(OverlayController::applicationKey);
}

void SettingsTabController::initStage2(OverlayController * parent, QQuickWindow * widget) {
	this->parent = parent;
	this->widget = widget;
}

void SettingsTabController::eventLoopTick() {
	if (settingsUpdateCounter >= 50) {
		setAutoStartEnabled(vr::VRApplications()->GetApplicationAutoLaunch(OverlayController::applicationKey));
		settingsUpdateCounter = 0;
	} else {
		settingsUpdateCounter++;
	}
}

bool SettingsTabController::autoStartEnabled() const {
	return m_autoStartEnabled;
}

void SettingsTabController::setAutoStartEnabled(bool value, bool notify) {
	if (m_autoStartEnabled != value) {
		m_autoStartEnabled = value;
		auto apperror = vr::VRApplications()->SetApplicationAutoLaunch(OverlayController::applicationKey, m_autoStartEnabled);
		if (apperror != vr::VRApplicationError_None) {
			LOG(ERROR) << "Could not set auto start: " << vr::VRApplications()->GetApplicationsErrorNameFromEnum(apperror);
		}
		if (notify) {
			emit autoStartEnabledChanged(m_autoStartEnabled);
		}
	}
}

} // namespace advconfig
