
#pragma once

#include <QObject>
#include <openvr.h>

class QQuickWindow;
// application namespace
namespace advsettings {

// forward declaration
class OverlayController;


class MoveCenterTabController : public QObject {
	Q_OBJECT
	Q_PROPERTY(int trackingUniverse READ trackingUniverse WRITE setTrackingUniverse NOTIFY trackingUniverseChanged)
	Q_PROPERTY(float offsetX READ offsetX WRITE setOffsetX NOTIFY offsetXChanged)
	Q_PROPERTY(float offsetY READ offsetY WRITE setOffsetY NOTIFY offsetYChanged)
	Q_PROPERTY(float offsetZ READ offsetZ WRITE setOffsetZ NOTIFY offsetZChanged)
	Q_PROPERTY(int rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
	Q_PROPERTY(bool adjustChaperone READ adjustChaperone WRITE setAdjustChaperone NOTIFY adjustChaperoneChanged)
	Q_PROPERTY(bool moveShortcutRight READ moveShortcutRight WRITE setMoveShortcutRight NOTIFY moveShortcutRightChanged)
	Q_PROPERTY(bool moveShortcutLeft READ moveShortcutLeft WRITE setMoveShortcutLeft NOTIFY moveShortcutLeftChanged)

private:
	OverlayController* parent;
	QQuickWindow* widget;

	int m_trackingUniverse = (int)vr::TrackingUniverseStanding;
	float m_offsetX = 0.0f;
	float m_offsetY = 0.0f;
	float m_offsetZ = 0.0f;
	int m_rotation = 0;
	bool m_adjustChaperone = true;
	bool m_moveShortcutRightPressed = false;
	bool m_moveShortcutLeftPressed = false;
	vr::ETrackedControllerRole m_activeMoveController;
	float m_lastControllerPosition[3];
	bool m_moveShortcutRightEnabled = false;
	bool m_moveShortcutLeftEnabled = false;

	unsigned settingsUpdateCounter = 0;

	vr::ETrackedControllerRole getMoveShortcutHand();

public:
	void initStage1();
	void initStage2(OverlayController* parent, QQuickWindow* widget);

	void eventLoopTick(vr::ETrackingUniverseOrigin universe, vr::TrackedDevicePose_t* devicePoses);

	float offsetX() const;
	float offsetY() const;
	float offsetZ() const;
	int rotation() const;
	bool adjustChaperone() const;
	bool moveShortcutRight() const;
	bool moveShortcutLeft() const;

public slots:
	int trackingUniverse() const;
	void setTrackingUniverse(int value, bool notify = true);

	void setOffsetX(float value, bool notify = true);
	void setOffsetY(float value, bool notify = true);
	void setOffsetZ(float value, bool notify = true);

	void setRotation(int value, bool notify = true);

	void setAdjustChaperone(bool value, bool notify = true);

	void setMoveShortcutRight(bool value, bool notify = true);
	void setMoveShortcutLeft(bool value, bool notify = true);

	void modOffsetX(float value, bool notify = true);
	void modOffsetY(float value, bool notify = true);
	void modOffsetZ(float value, bool notify = true);
	void reset();

signals:
	void trackingUniverseChanged(int value);
	void offsetXChanged(float value);
	void offsetYChanged(float value);
	void offsetZChanged(float value);
	void rotationChanged(int value);
	void adjustChaperoneChanged(bool value);
	void moveShortcutRightChanged(bool value);
	void moveShortcutLeftChanged(bool value);
};

} // namespace advsettings
