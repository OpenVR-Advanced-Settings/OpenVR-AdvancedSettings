
#pragma once

#include "openvr.h"
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

// application namespace
namespace advsettings {

// forward declaration
class OverlayWidget;
class OverlayController;


class ChaperoneTabController : public QObject {
	Q_OBJECT

private:
	OverlayController* parent;
	OverlayWidget* widget;

	bool forceBoundsFlag = false;

public:
	void init(OverlayController* parent, OverlayWidget* widget);

public slots:
	void UpdateTab();

	void CenterMarkerToggled(bool value);
	void PlaySpaceToggled(bool value);
	void ForceBoundsToggled(bool value);
	void FadeDistanceChanged(int value);
	void ColorAlphaChanged(int value);
	void ResetDefaultsClicked();
};


class MoveCenterTabController : public QObject {
	Q_OBJECT

private:
	OverlayController* parent;
	OverlayWidget* widget;

	float xOffset = 0.0f;
	float yOffset = 0.0f;
	float zOffset = 0.0f;

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
	void MoveCenterResetClicked();
};


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
	void eventLoopTick();

	void FixFloorClicked();
	void UndoFixFloorClicked();
};


class SettingsTabController : public QObject {
	Q_OBJECT

private:
	OverlayController* parent;
	OverlayWidget* widget;

public:
	void init(OverlayController* parent, OverlayWidget* widget);

public slots:
	void UpdateTab();

	void AutoStartToggled(bool value);
};


class OverlayController : public QObject {
	Q_OBJECT

public:
	static constexpr const char* applicationKey = "matzman666.AdvancedSettings";
	static constexpr const char* applicationName = "Advanced Settings";

private:
	OverlayWidget *m_pWidget;

	vr::VROverlayHandle_t m_ulOverlayHandle = vr::k_ulOverlayHandleInvalid;
	vr::VROverlayHandle_t m_ulOverlayThumbnailHandle = vr::k_ulOverlayHandleInvalid;

	std::unique_ptr<QOpenGLContext> m_pOpenGLContext;
	std::unique_ptr<QGraphicsScene> m_pScene;
	std::unique_ptr<QOpenGLFramebufferObject> m_pFbo;
	std::unique_ptr<QOffscreenSurface> m_pOffscreenSurface;

	std::unique_ptr<QTimer> m_pPumpEventsTimer;

	QPointF m_ptLastMouse;
	Qt::MouseButtons m_lastMouseButtons = 0;

	ChaperoneTabController chaperoneTabController;
	MoveCenterTabController moveCenterTabController;
	FixFloorTabController fixFloorTabController;
	SettingsTabController settingsTabController;

public:
    OverlayController() : QObject() {}
	virtual ~OverlayController();

	void Init();

	void SetWidget(OverlayWidget *pWidget, const std::string& name, const std::string& key = "");

	void AddOffsetToStandingPosition(unsigned axisId, float offset);

public slots:
	void OnSceneChanged( const QList<QRectF>& );
	void OnTimeoutPumpEvents();

	void UpdateWidget();

};

} // namespace advsettings
