#include "MoveCenterTabController.h"
#include "../overlaycontroller.h"
#include "../overlaywidget.h"
#include "ui_overlaywidget.h"

// application namespace
namespace advsettings {

void MoveCenterTabController::init(OverlayController * parent, OverlayWidget * widget) {
	this->parent = parent;
	this->widget = widget;
	trackingUniverse = vr::VRCompositor()->GetTrackingSpace();
	xOffset = 0.0f;
	yOffset = 0.0f;
	zOffset = 0.0f;
	widget->ui->MoveCenterStandingSwitch->setChecked(true);
	widget->ui->MoveCenterStandingSwitch->setEnabled(false);
	widget->ui->MoveCenterSeatedSwitch->setChecked(false);
	widget->ui->MoveCenterSeatedSwitch->setEnabled(true);
	connect(widget->ui->XMinusButton, SIGNAL(clicked()), this, SLOT(XMinusClicked()));
	connect(widget->ui->XPlusButton, SIGNAL(clicked()), this, SLOT(XPlusClicked()));
	connect(widget->ui->YMinusButton, SIGNAL(clicked()), this, SLOT(YMinusClicked()));
	connect(widget->ui->YPlusButton, SIGNAL(clicked()), this, SLOT(YPlusClicked()));
	connect(widget->ui->ZMinusButton, SIGNAL(clicked()), this, SLOT(ZMinusClicked()));
	connect(widget->ui->ZPlusButton, SIGNAL(clicked()), this, SLOT(ZPlusClicked()));
	connect(widget->ui->RotateCenterSlider, SIGNAL(valueChanged(int)), this, SLOT(RotateSliderChanged(int)));
	connect(widget->ui->RotateCenterButton, SIGNAL(clicked()), this, SLOT(RotateApplyClicked()));
	connect(widget->ui->MoveCenterResetButton, SIGNAL(clicked()), this, SLOT(MoveCenterResetClicked()));
	/*connect(widget->ui->MoveCenterStandingSwitch, SIGNAL(toggled(bool)), this, SLOT(StandingToggled(bool)));
	connect(widget->ui->MoveCenterSeatedSwitch, SIGNAL(toggled(bool)), this, SLOT(SeatedToggled(bool)));*/
}

void MoveCenterTabController::UpdateTab() {
	if (widget) {
		widget->ui->XOffsetLabel->setText(QString::asprintf("%.1f", xOffset));
		widget->ui->YOffsetLabel->setText(QString::asprintf("%.1f", yOffset));
		widget->ui->ZOffsetLabel->setText(QString::asprintf("%.1f", zOffset));
		widget->ui->MoveCenterStandingSwitch->blockSignals(true);
		widget->ui->MoveCenterSeatedSwitch->blockSignals(true);
		widget->ui->RotateCenterSlider->blockSignals(true);
		if (trackingUniverse == vr::TrackingUniverseStanding) {
			widget->ui->MoveCenterStandingSwitch->setChecked(true);
			widget->ui->MoveCenterSeatedSwitch->setChecked(false);
			widget->ui->MoveCenterStandingSwitch->setEnabled(false);
			widget->ui->MoveCenterSeatedSwitch->setEnabled(false);
			widget->ui->RotateCenterButton->setEnabled(true);
			widget->ui->RotateCenterSlider->setEnabled(true);
			widget->ui->RotateCenterLabel->setText(QString::asprintf("%i", rotation));
		} else {
			widget->ui->MoveCenterStandingSwitch->setChecked(false);
			widget->ui->MoveCenterSeatedSwitch->setChecked(true);
			widget->ui->MoveCenterStandingSwitch->setEnabled(false);
			widget->ui->MoveCenterSeatedSwitch->setEnabled(false);
			widget->ui->RotateCenterButton->setEnabled(false);
			widget->ui->RotateCenterSlider->setEnabled(false);
			widget->ui->RotateCenterLabel->setText("-");
		}
		widget->ui->RotateCenterSlider->setValue(rotation);
		widget->ui->MoveCenterStandingSwitch->blockSignals(false);
		widget->ui->MoveCenterSeatedSwitch->blockSignals(false);
		widget->ui->RotateCenterSlider->blockSignals(false);
	}
}

void MoveCenterTabController::XMinusClicked() {
	if (widget) {
		if (rotation == 0) {
			parent->AddOffsetToUniverseCenter(trackingUniverse, 0, -0.5f);
		} else {
			auto angle = rotation * 2 * M_PI / 360.0;
			vr::VRChaperoneSetup()->RevertWorkingCopy();
			parent->AddOffsetToUniverseCenter(trackingUniverse, 0, -0.5f * std::cos(angle), true, false);
			parent->AddOffsetToUniverseCenter(trackingUniverse, 2, -0.5f * std::sin(angle), true, false);
			vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
		}
		xOffset -= 0.5f;
		widget->ui->XOffsetLabel->setText(QString::asprintf("%.1f", xOffset));
	}
}

void MoveCenterTabController::XPlusClicked() {
	if (widget) {
		if (rotation == 0) {
			parent->AddOffsetToUniverseCenter(trackingUniverse, 0, 0.5f);
		} else {
			auto angle = rotation * 2 * M_PI / 360.0;
			vr::VRChaperoneSetup()->RevertWorkingCopy();
			parent->AddOffsetToUniverseCenter(trackingUniverse, 0, 0.5f * std::cos(angle), true, false);
			parent->AddOffsetToUniverseCenter(trackingUniverse, 2, 0.5f * std::sin(angle), true, false);
			vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
		}
		xOffset += 0.5f;
		widget->ui->XOffsetLabel->setText(QString::asprintf("%.1f", xOffset));
	}
}

void MoveCenterTabController::YMinusClicked() {
	if (widget) {
		parent->AddOffsetToUniverseCenter(trackingUniverse, 1, -0.5f);
		yOffset -= 0.5f;
		widget->ui->YOffsetLabel->setText(QString::asprintf("%.1f", yOffset));
	}
}

void MoveCenterTabController::YPlusClicked() {
	if (widget) {
		parent->AddOffsetToUniverseCenter(trackingUniverse, 1, 0.5f);
		yOffset += 0.5f;
		widget->ui->YOffsetLabel->setText(QString::asprintf("%.1f", yOffset));
	}
}

void MoveCenterTabController::ZMinusClicked() {
	if (widget) {
		if (rotation == 0) {
			parent->AddOffsetToUniverseCenter(trackingUniverse, 2, -0.5f);
		} else {
			auto angle = rotation * 2 * M_PI / 360.0;
			vr::VRChaperoneSetup()->RevertWorkingCopy();
			parent->AddOffsetToUniverseCenter(trackingUniverse, 2, -0.5f * std::cos(angle), true, false);
			parent->AddOffsetToUniverseCenter(trackingUniverse, 0, 0.5f * std::sin(angle), true, false);
			vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
		}
		zOffset -= 0.5f;
		widget->ui->ZOffsetLabel->setText(QString::asprintf("%.1f", zOffset));
	}
}

void MoveCenterTabController::ZPlusClicked() {
	if (widget) {
		if (rotation == 0) {
			parent->AddOffsetToUniverseCenter(trackingUniverse, 2, 0.5f);
		} else {
			auto angle = rotation * 2 * M_PI / 360.0;
			vr::VRChaperoneSetup()->RevertWorkingCopy();
			parent->AddOffsetToUniverseCenter(trackingUniverse, 2, 0.5f * std::cos(angle), true, false);
			parent->AddOffsetToUniverseCenter(trackingUniverse, 0, -0.5f * std::sin(angle), true, false);
			vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
		}
		zOffset += 0.5f;
		widget->ui->ZOffsetLabel->setText(QString::asprintf("%.1f", zOffset));
	}
}

void MoveCenterTabController::RotateApplyClicked() {
	int value = widget->ui->RotateCenterSlider->value();
	float angle = (value - rotation) * 2 * M_PI / 360.0;
	parent->RotateUniverseCenter(trackingUniverse, angle);
	rotation = value;
}

void MoveCenterTabController::RotateSliderChanged(int value) {
	widget->ui->RotateCenterLabel->setText(QString::asprintf("%i", value));
}

void MoveCenterTabController::MoveCenterResetClicked() {
	if (widget) {
		vr::VRChaperoneSetup()->RevertWorkingCopy();
		parent->RotateUniverseCenter(trackingUniverse, -rotation * 2 * M_PI / 360.0);
		parent->AddOffsetToUniverseCenter(trackingUniverse, 0, -xOffset, true, false);
		parent->AddOffsetToUniverseCenter(trackingUniverse, 1, -yOffset, true, false);
		parent->AddOffsetToUniverseCenter(trackingUniverse, 2, -zOffset, true, false);
		vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
		xOffset = 0.0f;
		yOffset = 0.0f;
		zOffset = 0.0f;
		rotation = 0;
		widget->ui->XOffsetLabel->setText(QString::asprintf("%.1f", xOffset));
		widget->ui->YOffsetLabel->setText(QString::asprintf("%.1f", yOffset));
		widget->ui->ZOffsetLabel->setText(QString::asprintf("%.1f", zOffset));
		widget->ui->RotateCenterSlider->setValue(rotation);
		if (trackingUniverse == vr::TrackingUniverseStanding) {
			widget->ui->RotateCenterLabel->setText(QString::asprintf("%i", rotation));
		} else {
			widget->ui->RotateCenterLabel->setText("-");
		}
	}
}

void MoveCenterTabController::StandingToggled(bool) {
	MoveCenterResetClicked();
	trackingUniverse = vr::TrackingUniverseStanding;
	UpdateTab();
}

void MoveCenterTabController::SeatedToggled(bool) {
	MoveCenterResetClicked();
	trackingUniverse = vr::TrackingUniverseSeated;
	UpdateTab();
}

void MoveCenterTabController::eventLoopTick(vr::ETrackingUniverseOrigin universe) {
	if (trackingUniverse != universe) {
		if (universe == vr::TrackingUniverseStanding) {
			StandingToggled(true);
		} else if (universe == vr::TrackingUniverseSeated) {
			SeatedToggled(true);
		}
	}
}

} // namespace advconfig
