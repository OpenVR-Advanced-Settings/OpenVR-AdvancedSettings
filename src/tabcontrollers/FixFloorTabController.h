
#pragma once

#include <QObject>
#include <openvr.h>

// application namespace
namespace advsettings {

// forward declaration
class OverlayWidget;
class OverlayController;

class FixFloorTabController : public QObject {
	Q_OBJECT

private:
	OverlayController* parent;
	OverlayWidget* widget;

	float controllerUpOffsetCorrection = 0.06f; // Controller touchpad facing upwards
	float controllerDownOffsetCorrection = 0.0f; // Controller touchpad facing downwards (not implemented yet)

	int state = 0; // 0 .. idle, 1 .. measurement in progress, 2 .. waiting for status message timeout
	vr::TrackedDeviceIndex_t referenceController = vr::k_unTrackedDeviceIndexInvalid;
	unsigned measurementCount = 0;
	double tempOffset = 0.0;
	unsigned statusMessageTimeout = 0;
	float floorOffset = 0.0f;

public:
	void init(OverlayController* parent, OverlayWidget* widget);

	public slots:
	void UpdateTab();
	void eventLoopTick(vr::TrackedDevicePose_t* devicePoses);

	void FixFloorClicked();
	void UndoFixFloorClicked();
};

} // namespace advsettings
