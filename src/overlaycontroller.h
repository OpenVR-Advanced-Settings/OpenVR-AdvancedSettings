
#pragma once

#include <openvr.h>
#include <QtCore/QtCore>
// because of incompatibilities with QtOpenGL and GLEW we need to cherry pick includes
#include <QVector2D>
#include <QMatrix4x4>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QtGui/QOpenGLContext>
#include <QtWidgets/QGraphicsScene>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include <memory>
#include "logging.h"

#include "tabcontrollers/SteamVRTabController.h"
#include "tabcontrollers/ChaperoneTabController.h"
#include "tabcontrollers/MoveCenterTabController.h"
#include "tabcontrollers/FixFloorTabController.h"
#include "tabcontrollers/StatisticsTabController.h"
#include "tabcontrollers/SettingsTabController.h"


// application namespace
namespace advsettings {

// forward declaration
class OverlayWidget;

class OverlayController : public QObject {
	Q_OBJECT

public:
	static constexpr const char* applicationKey = "matzman666.AdvancedSettings";
	static constexpr const char* applicationName = "Advanced Settings";
	static constexpr const char* applicationVersionString = "v1.1";

private:
	OverlayWidget *m_pWidget;

	vr::VROverlayHandle_t m_ulOverlayHandle = vr::k_ulOverlayHandleInvalid;
	vr::VROverlayHandle_t m_ulOverlayThumbnailHandle = vr::k_ulOverlayHandleInvalid;

	std::unique_ptr<QOpenGLContext> m_pOpenGLContext;
	std::unique_ptr<QGraphicsScene> m_pScene;
	std::unique_ptr<QOpenGLFramebufferObject> m_pFbo;
	std::unique_ptr<QOffscreenSurface> m_pOffscreenSurface;

	std::unique_ptr<QTimer> m_pPumpEventsTimer;
	bool dashboardVisible = false;

	QPointF m_ptLastMouse;
	Qt::MouseButtons m_lastMouseButtons = 0;

	SteamVRTabController steamVRTabController;
	ChaperoneTabController chaperoneTabController;
	MoveCenterTabController moveCenterTabController;
	FixFloorTabController fixFloorTabController;
	StatisticsTabController statisticsTabController;
	SettingsTabController settingsTabController;

public:
    OverlayController() : QObject() {}
	virtual ~OverlayController();

	void Init();

	bool isDashboardVisible() {
		return dashboardVisible;
	}

	void SetWidget(OverlayWidget *pWidget, const std::string& name, const std::string& key = "");

	void AddOffsetToUniverseCenter(vr::ETrackingUniverseOrigin universe, unsigned axisId, float offset, bool adjustBounds = true, bool commit = true);
	void RotateUniverseCenter(vr::ETrackingUniverseOrigin universe, float yAngle, bool adjustBounds = true, bool commit = true); // around y axis
	void AddOffsetToCollisionBounds(unsigned axisId, float offset, bool commit = true);
	void RotateCollisionBounds(float angle, bool commit = true); // around y axis

public slots:
	void OnSceneChanged( const QList<QRectF>& );
	void OnTimeoutPumpEvents();

	void UpdateWidget();
};

} // namespace advsettings
