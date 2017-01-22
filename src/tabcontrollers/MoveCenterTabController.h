
#pragma once

#include "AudioTabController.h"
#include <QObject>
#include <QVariant>
#include <openvr.h>
#include <mutex>

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
	Q_PROPERTY(bool pttEnabled READ pttEnabled WRITE setPttEnabled NOTIFY pttEnabledChanged)
	Q_PROPERTY(bool pttActive READ pttActive NOTIFY pttActiveChanged)
	Q_PROPERTY(bool pttLeftControllerEnabled READ pttLeftControllerEnabled WRITE setPttLeftControllerEnabled NOTIFY pttLeftControllerEnabledChanged)
	Q_PROPERTY(bool pttRightControllerEnabled READ pttRightControllerEnabled WRITE setPttRightControllerEnabled NOTIFY pttRightControllerEnabledChanged)

private:
	void offsetYToggle(bool enabled);

	OverlayController* parent;
	QQuickWindow* widget;

	int m_trackingUniverse = (int)vr::TrackingUniverseStanding;
	float m_offsetX = 0.0f;
	float m_offsetY = 0.0f;
	float m_toggleOffsetY = 0.0f;
	float m_offsetZ = 0.0f;
	int m_rotation = 0;
	bool m_adjustChaperone = true;

	bool m_pttEnabled = false;
	bool m_pttActive = false;
	bool m_pttShowNotification = false;
	bool m_pttLeftControllerEnabled = false;
	bool m_pttRightControllerEnabled = false;
	PttControllerConfig m_pttControllerConfigs[2];

	unsigned settingsUpdateCounter = 0;

	std::recursive_mutex eventLoopMutex;

public:
	void initStage1();
	void initStage2(OverlayController* parent, QQuickWindow* widget);

	void eventLoopTick(vr::ETrackingUniverseOrigin universe);

	float offsetX() const;
	float offsetY() const;
	float offsetZ() const;
	int rotation() const;
	bool adjustChaperone() const;

	bool pttEnabled() const;
	bool pttActive() const;
	bool pttLeftControllerEnabled() const;
	bool pttRightControllerEnabled() const;
	void reloadPttConfig();
	void savePttConfig();

	Q_INVOKABLE QVariantList pttDigitalButtons(unsigned controller);
	Q_INVOKABLE unsigned long pttDigitalButtonMask(unsigned controller);
	Q_INVOKABLE unsigned pttTouchpadMode(unsigned controller);
	Q_INVOKABLE unsigned pttTriggerMode(unsigned controller);
	Q_INVOKABLE unsigned pttTouchpadArea(unsigned controller);

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

	void setPttEnabled(bool value, bool notify = true, bool save = true);
	void setPttLeftControllerEnabled(bool value, bool notify = true, bool save = true);
	void setPttRightControllerEnabled(bool value, bool notify = true, bool save = true);

	void setPttControllerConfig(unsigned controller, QVariantList buttons, unsigned triggerMode, unsigned padMode, unsigned padAreas);

signals:
	void trackingUniverseChanged(int value);
	void offsetXChanged(float value);
	void offsetYChanged(float value);
	void offsetZChanged(float value);
	void rotationChanged(int value);
	void adjustChaperoneChanged(bool value);

	void pttEnabledChanged(bool value);
	void pttActiveChanged(bool value);
	void pttLeftControllerEnabledChanged(bool value);
	void pttRightControllerEnabledChanged(bool value);
};

} // namespace advsettings
