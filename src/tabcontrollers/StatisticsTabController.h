
#pragma once

#include <QObject>
#include <openvr.h>

// application namespace
namespace advsettings {

// forward declaration
class OverlayWidget;
class OverlayController;


class StatisticsTabController : public QObject {
	Q_OBJECT

private:
	OverlayController* parent;
	OverlayWidget* widget;

	bool rotationResetFlag = false;
	float rotationMarker = 0.0f;
	int rotationDir = 0;
	int64_t rotationCounter = 0;
	float lastYaw = -1.0f;
	unsigned lastPosTimer = 0;
	float lastHmdPos[3];
	bool lastHmdPosValid = false;
	double hmdDistance = 0.0;
	float leftControllerMaxSpeed = 0.0;
	float rightControllerMaxSpeed = 0.0;
	unsigned compositorStatsPid = 0;
	unsigned droppedFramesOffset = 0;
	unsigned reprojectedFramesOffset = 0;
	unsigned timedOutOffset = 0;

public:
	void init(OverlayController* parent, OverlayWidget* widget);

	public slots:
	void UpdateTab();
	void eventLoopTick(vr::TrackedDevicePose_t* devicePoses);

	void StatsDistanceResetClicked();
	void StatsRotationResetClicked();
	void StatsLeftControllerSpeedResetClicked();
	void StatsRightControllerSpeedResetClicked();
	void DroppedFramesResetClicked();
	void ReprojectedFramesResetClicked();
	void TimedOutResetClicked();
};


} // namespace advsettings
