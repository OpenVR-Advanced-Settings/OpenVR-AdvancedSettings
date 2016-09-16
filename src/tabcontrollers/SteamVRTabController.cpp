#include "SteamVRTabController.h"
#include "../overlaycontroller.h"
#include "../overlaywidget.h"
#include "ui_overlaywidget.h"

// application namespace
namespace advsettings {

void SteamVRTabController::init(OverlayController * parent, OverlayWidget * widget) {
	this->parent = parent;
	this->widget = widget;
	connect(widget->ui->SupersamplingSlider, SIGNAL(valueChanged(int)), this, SLOT(SupersamplingChanged(int)));
	connect(widget->ui->ReprojectionToggle, SIGNAL(toggled(bool)), this, SLOT(ReprojectionToggled(bool)));
	connect(widget->ui->ForceReprojectionToggle, SIGNAL(toggled(bool)), this, SLOT(ForceReprojectionToggled(bool)));
	connect(widget->ui->SteamVRResetButton, SIGNAL(clicked()), this, SLOT(ResetClicked()));
	connect(widget->ui->SteamVRRestartButton, SIGNAL(clicked()), this, SLOT(RestartClicked()));
}

void SteamVRTabController::UpdateTab() {
	if (widget) {
		widget->ui->SupersamplingSlider->blockSignals(true);
		widget->ui->ReprojectionToggle->blockSignals(true);
		widget->ui->ForceReprojectionToggle->blockSignals(true);
		widget->ui->SteamVRResetButton->blockSignals(true);
		widget->ui->SteamVRRestartButton->blockSignals(true);

		auto ss = vr::VRSettings()->GetFloat(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_RenderTargetMultiplier_Float, 1.0);
		widget->ui->SupersamplingSlider->setValue(ss * 10);
		widget->ui->SupersamplingLabel->setText(QString::asprintf("%.1f", ss));
		widget->ui->ReprojectionToggle->setChecked(vr::VRSettings()->GetBool(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_AllowReprojection_Bool, true));
		widget->ui->ForceReprojectionToggle->setChecked(vr::VRSettings()->GetBool(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_ForceReprojection_Bool, false));

		widget->ui->SupersamplingSlider->blockSignals(false);
		widget->ui->ReprojectionToggle->blockSignals(false);
		widget->ui->ForceReprojectionToggle->blockSignals(false);
		widget->ui->SteamVRResetButton->blockSignals(false);
		widget->ui->SteamVRRestartButton->blockSignals(false);
	}
}

void SteamVRTabController::SupersamplingChanged(int value) {
	float fval = (float)value / 10.0f;
	widget->ui->SupersamplingLabel->setText(QString::asprintf("%.1f", fval));
	vr::VRSettings()->SetFloat(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_RenderTargetMultiplier_Float, fval);
	vr::VRSettings()->Sync();
}

void SteamVRTabController::ReprojectionToggled(bool value) {
	vr::VRSettings()->SetBool(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_AllowReprojection_Bool, value);
	vr::VRSettings()->Sync();
}

void SteamVRTabController::ForceReprojectionToggled(bool value) {
	vr::VRSettings()->SetBool(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_ForceReprojection_Bool, value);
	vr::VRSettings()->Sync();
}

void SteamVRTabController::ResetClicked() {
	widget->ui->SupersamplingSlider->setValue(10);
	widget->ui->ReprojectionToggle->setChecked(true);
	widget->ui->ForceReprojectionToggle->setChecked(false);
}

void SteamVRTabController::RestartClicked() {
	QProcess::startDetached("cmd.exe /C restartvrserver.bat");
}

} // namespace advconfig
