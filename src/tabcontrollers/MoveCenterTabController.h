
#pragma once

#include <QObject>
#include <openvr.h>

// application namespace
namespace advsettings {

// forward declaration
class OverlayWidget;
class OverlayController;

class MoveCenterTabController : public QObject {
	Q_OBJECT

private:
	OverlayController* parent;
	OverlayWidget* widget;

	vr::ETrackingUniverseOrigin trackingUniverse = vr::TrackingUniverseStanding;
	float xOffset = 0.0f;
	float yOffset = 0.0f;
	float zOffset = 0.0f;
	int rotation = 0;

public:
	void init(OverlayController* parent, OverlayWidget* widget);

	public slots:
	void UpdateTab();

	void XMinusClicked();
	void XPlusClicked();
	void YMinusClicked();
	void YPlusClicked();
	void ZMinusClicked();
	void ZPlusClicked();
	void RotateApplyClicked();
	void RotateSliderChanged(int value);
	void MoveCenterResetClicked();
	void StandingToggled(bool value);
	void SeatedToggled(bool value);
	void eventLoopTick(vr::ETrackingUniverseOrigin universe);
};

} // namespace advsettings
