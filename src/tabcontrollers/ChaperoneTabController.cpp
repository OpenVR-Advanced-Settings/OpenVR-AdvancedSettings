#include "ChaperoneTabController.h"
#include "../overlaycontroller.h"
#include "../overlaywidget.h"
#include "ui_overlaywidget.h"

// application namespace
namespace advsettings {


void ChaperoneTabController::init(OverlayController * parent, OverlayWidget * widget) {
	this->parent = parent;
	this->widget = widget;

	connect(widget->ui->CenterMarkerToggle, SIGNAL(toggled(bool)), this, SLOT(CenterMarkerToggled(bool)));
	connect(widget->ui->PlaySpaceToggle, SIGNAL(toggled(bool)), this, SLOT(PlaySpaceToggled(bool)));
	connect(widget->ui->ForceBoundsToggle, SIGNAL(toggled(bool)), this, SLOT(ForceBoundsToggled(bool)));
	connect(widget->ui->FadeDistanceSlider, SIGNAL(valueChanged(int)), this, SLOT(FadeDistanceChanged(int)));
	connect(widget->ui->ColorAlphaSlider, SIGNAL(valueChanged(int)), this, SLOT(ColorAlphaChanged(int)));
	connect(widget->ui->FlipOrientationButton, SIGNAL(clicked()), this, SLOT(FlipOrientationClicked()));
	connect(widget->ui->ReloadChaperoneButton, SIGNAL(clicked()), this, SLOT(ReloadClicked()));
	connect(widget->ui->ResetDefaultsToggle, SIGNAL(clicked()), this, SLOT(ResetDefaultsClicked()));
}

void ChaperoneTabController::UpdateTab() {
	if (widget && vr::VRSettings()) {
		widget->ui->CenterMarkerToggle->blockSignals(true);
		widget->ui->PlaySpaceToggle->blockSignals(true);
		widget->ui->ForceBoundsToggle->blockSignals(true);
		widget->ui->FadeDistanceSlider->blockSignals(true);
		widget->ui->ColorAlphaSlider->blockSignals(true);

		auto centerMarkerOn = vr::VRSettings()->GetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_CenterMarkerOn_Bool, false);
		widget->ui->CenterMarkerToggle->setChecked(centerMarkerOn);
		auto playSpaceOn = vr::VRSettings()->GetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_PlaySpaceOn_Bool, false);
		widget->ui->PlaySpaceToggle->setChecked(playSpaceOn);
		widget->ui->ForceBoundsToggle->setChecked(forceBoundsFlag);
		auto fadeDistance = vr::VRSettings()->GetFloat(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_FadeDistance_Float, 0.7f);
		widget->ui->FadeDistanceSlider->setValue(fadeDistance * 100);
		widget->ui->FadeDistanceLabel->setText(QString::asprintf("%.2f", fadeDistance));
		auto colorAlpha = vr::VRSettings()->GetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaA_Int32, 255);
		widget->ui->ColorAlphaSlider->setValue(colorAlpha * 100 / 255);
		widget->ui->ColorAlphaLabel->setText(QString::asprintf("%i%%", colorAlpha * 100 / 255));

		widget->ui->CenterMarkerToggle->blockSignals(false);
		widget->ui->PlaySpaceToggle->blockSignals(false);
		widget->ui->ForceBoundsToggle->blockSignals(false);
		widget->ui->FadeDistanceSlider->blockSignals(false);
		widget->ui->ColorAlphaSlider->blockSignals(false);
	}
}

void ChaperoneTabController::CenterMarkerToggled(bool value) {
	if (vr::VRSettings()) {
		vr::VRSettings()->SetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_CenterMarkerOn_Bool, value);
		vr::VRSettings()->Sync();
	}
}

void ChaperoneTabController::PlaySpaceToggled(bool value) {
	if (vr::VRSettings()) {
		vr::VRSettings()->SetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_PlaySpaceOn_Bool, value);
		vr::VRSettings()->Sync();
	}
}

void ChaperoneTabController::ForceBoundsToggled(bool value) {
	if (vr::VRChaperone()) {
		forceBoundsFlag = value;
		vr::VRChaperone()->ForceBoundsVisible(value);
	}
}

void ChaperoneTabController::FadeDistanceChanged(int value) {
	if (vr::VRSettings()) {
		float fval = (float)value / 100;
		vr::VRSettings()->SetFloat(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_FadeDistance_Float, fval);
		vr::VRSettings()->Sync();
		widget->ui->FadeDistanceLabel->setText(QString::asprintf("%.2f", fval));
	}
}

void ChaperoneTabController::ColorAlphaChanged(int value) {
	if (vr::VRSettings()) {
		float fval = (float)value / 100;
		vr::VRSettings()->SetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaA_Int32, 255 * fval);
		vr::VRSettings()->Sync();
		widget->ui->ColorAlphaLabel->setText(QString::asprintf("%i%%", value));
	}
}

void ChaperoneTabController::FlipOrientationClicked() {
	parent->RotateUniverseCenter(vr::TrackingUniverseStanding, (float)M_PI);
}

void ChaperoneTabController::ReloadClicked() {
	vr::VRChaperoneSetup()->ReloadFromDisk(vr::EChaperoneConfigFile_Live);
	vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
}

void ChaperoneTabController::ResetDefaultsClicked() {
	widget->ui->CenterMarkerToggle->setChecked(false);
	widget->ui->PlaySpaceToggle->setChecked(false);
	widget->ui->ForceBoundsToggle->setChecked(false);
	widget->ui->FadeDistanceSlider->setValue(70);
	widget->ui->ColorAlphaSlider->setValue(100);
}

} // namespace advconfig
