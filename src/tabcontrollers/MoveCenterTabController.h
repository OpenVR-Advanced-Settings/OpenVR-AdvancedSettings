
#pragma once

#include "PttController.h"

class QQuickWindow;
// application namespace
namespace advsettings {

// forward declaration
class OverlayController;


class MoveCenterTabController : public PttController {
	Q_OBJECT
	Q_PROPERTY(int trackingUniverse READ trackingUniverse WRITE setTrackingUniverse NOTIFY trackingUniverseChanged)
	Q_PROPERTY(float offsetX READ offsetX WRITE setOffsetX NOTIFY offsetXChanged)
	Q_PROPERTY(float offsetY READ offsetY WRITE setOffsetY NOTIFY offsetYChanged)
	Q_PROPERTY(float offsetZ READ offsetZ WRITE setOffsetZ NOTIFY offsetZChanged)
	Q_PROPERTY(int rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
	Q_PROPERTY(bool adjustChaperone READ adjustChaperone WRITE setAdjustChaperone NOTIFY adjustChaperoneChanged)

private:
	OverlayController* parent;
	QQuickWindow* widget;

	int m_trackingUniverse = (int)vr::TrackingUniverseStanding;
	float m_offsetX = 0.0f;
	float m_offsetY = 0.0f;
	float m_toggleOffsetY = 0.0f;
	float m_offsetZ = 0.0f;
	int m_rotation = 0;
	bool m_adjustChaperone = true;

	unsigned settingsUpdateCounter = 0;

	QString getSettingsName() override { return "playspaceSettings"; }
	void onPttStart() override;
	void onPttStop() override;
	void onPttDisabled() override;

public:
	void initStage1();
	void initStage2(OverlayController* parent, QQuickWindow* widget);

	void eventLoopTick(vr::ETrackingUniverseOrigin universe);

	float offsetX() const;
	float offsetY() const;
	float offsetZ() const;
	int rotation() const;
	bool adjustChaperone() const;

public slots:
	int trackingUniverse() const;
	void setTrackingUniverse(int value, bool notify = true);

	void setOffsetX(float value, bool notify = true);
	void setOffsetY(float value, bool notify = true);
	void setOffsetZ(float value, bool notify = true);

	void setRotation(int value, bool notify = true);

	void setAdjustChaperone(bool value, bool notify = true);

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
};

} // namespace advsettings
