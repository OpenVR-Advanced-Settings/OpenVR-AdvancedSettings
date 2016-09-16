#include "SettingsTabController.h"
#include "../overlaycontroller.h"
#include "../overlaywidget.h"
#include "ui_overlaywidget.h"

// application namespace
namespace advsettings {


void SettingsTabController::init(OverlayController * parent, OverlayWidget * widget) {
	this->parent = parent;
	this->widget = widget;
	connect(widget->ui->AutoStartToggle, SIGNAL(toggled(bool)), this, SLOT(AutoStartToggled(bool)));
}

void SettingsTabController::UpdateTab() {
	if (widget) {
		widget->ui->AutoStartToggle->blockSignals(true);
		if (vr::VRApplications()->GetApplicationAutoLaunch(OverlayController::applicationKey)) {
			widget->ui->AutoStartToggle->setChecked(true);
		} else {
			widget->ui->AutoStartToggle->setChecked(false);
		}
		widget->ui->AutoStartToggle->blockSignals(false);
	}
}

void SettingsTabController::AutoStartToggled(bool value) {
	LOG(INFO) << "Setting auto start: " << value;
	auto apperror = vr::VRApplications()->SetApplicationAutoLaunch(OverlayController::applicationKey, value);
	if (apperror != vr::VRApplicationError_None) {
		LOG(ERROR) << "Could not set auto start: " << vr::VRApplications()->GetApplicationsErrorNameFromEnum(apperror);
	}
}


} // namespace advconfig
