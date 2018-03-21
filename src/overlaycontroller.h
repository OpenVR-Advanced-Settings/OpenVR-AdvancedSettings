
#pragma once

#include <openvr.h>
#include <QtCore/QtCore>
// because of incompatibilities with QtOpenGL and GLEW we need to cherry pick includes
#include <QVector2D>
#include <QMatrix4x4>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QObject>
#include <QSettings>
#include <QQmlEngine>
#include <QtGui/QOpenGLContext>
#include <QtWidgets/QGraphicsScene>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQuickRenderControl>
#include <QSoundEffect>
#include <memory>
#include "logging.h"
#include "utils/ChaperoneUtils.h"

#include "tabcontrollers/SteamVRTabController.h"
#include "tabcontrollers/ChaperoneTabController.h"
#include "tabcontrollers/MoveCenterTabController.h"
#include "tabcontrollers/FixFloorTabController.h"
#include "tabcontrollers/AudioTabController.h"
#include "tabcontrollers/StatisticsTabController.h"
#include "tabcontrollers/SettingsTabController.h"
#include "tabcontrollers/ReviveTabController.h"
#include "tabcontrollers/UtilitiesTabController.h"
#include "tabcontrollers/AccessibilityTabController.h"



// application namespace
namespace advsettings {

class OverlayController : public QObject {
	Q_OBJECT
	Q_PROPERTY(bool desktopMode READ isDesktopMode)

public:
	static constexpr const char* applicationKey = "matzman666.AdvancedSettings";
	static constexpr const char* applicationName = "Advanced Settings";
	static constexpr const char* applicationVersionString = "v2.5";

private:
	vr::VROverlayHandle_t m_ulOverlayHandle = vr::k_ulOverlayHandleInvalid;
	vr::VROverlayHandle_t m_ulOverlayThumbnailHandle = vr::k_ulOverlayHandleInvalid;

	std::unique_ptr<QQuickRenderControl> m_pRenderControl;
	std::unique_ptr<QQuickWindow> m_pWindow;
	std::unique_ptr<QOpenGLFramebufferObject> m_pFbo;
	std::unique_ptr<QOpenGLContext> m_pOpenGLContext;
	std::unique_ptr<QOffscreenSurface> m_pOffscreenSurface;

	std::unique_ptr<QTimer> m_pPumpEventsTimer;
	std::unique_ptr<QTimer> m_pRenderTimer;
	bool dashboardVisible = false;

	QPoint m_ptLastMouse;
	Qt::MouseButtons m_lastMouseButtons = 0;

	bool desktopMode;
	bool noSound;

	QUrl m_runtimePathUrl;

	utils::ChaperoneUtils m_chaperoneUtils;

	QSoundEffect activationSoundEffect;
	QSoundEffect focusChangedSoundEffect;
	QSoundEffect alarm01SoundEffect;

public: // I know it's an ugly hack to make them public to enable external access, but I am too lazy to implement getters.
	SteamVRTabController steamVRTabController;
	ChaperoneTabController chaperoneTabController;
	MoveCenterTabController moveCenterTabController;
	FixFloorTabController fixFloorTabController;
	AudioTabController audioTabController;
	StatisticsTabController statisticsTabController;
	SettingsTabController settingsTabController;
	ReviveTabController reviveTabController;
	UtilitiesTabController utilitiesTabController;
	AccessibilityTabController accessibilityTabController;

private:
    OverlayController(bool desktopMode, bool noSound) : QObject(), desktopMode(desktopMode), noSound(noSound) {}

public:
	virtual ~OverlayController();

	void Init(QQmlEngine* qmlEngine);
	void Shutdown();

	bool isDashboardVisible() {
		return dashboardVisible;
	}

	void SetWidget(QQuickItem* quickItem, const std::string& name, const std::string& key = "");

	void AddOffsetToUniverseCenter(vr::ETrackingUniverseOrigin universe, unsigned axisId, float offset, bool adjustBounds = true, bool commit = true);
	void AddOffsetToUniverseCenter(vr::ETrackingUniverseOrigin universe, float offset[3], bool adjustBounds = true, bool commit = true);
	void RotateUniverseCenter(vr::ETrackingUniverseOrigin universe, float yAngle, bool adjustBounds = true, bool commit = true); // around y axis
	void AddOffsetToCollisionBounds(unsigned axisId, float offset, bool commit = true);
	void AddOffsetToCollisionBounds(float offset[3], bool commit = true);
	void RotateCollisionBounds(float angle, bool commit = true); // around y axis

	bool isDesktopMode() { return desktopMode; };

	utils::ChaperoneUtils& chaperoneUtils() noexcept { return m_chaperoneUtils; }

	Q_INVOKABLE QString getVersionString();
	Q_INVOKABLE QUrl getVRRuntimePathUrl();

	Q_INVOKABLE bool soundDisabled();

	const vr::VROverlayHandle_t& overlayHandle();
	const vr::VROverlayHandle_t& overlayThumbnailHandle();

	bool pollNextEvent(vr::VROverlayHandle_t ulOverlayHandle, vr::VREvent_t* pEvent);

public slots:
	void renderOverlay();
	void OnRenderRequest();
	void OnTimeoutPumpEvents();

	void showKeyboard(QString existingText, unsigned long userValue = 0);

	void playActivationSound();
	void playFocusChangedSound();
	void playAlarm01Sound(bool loop = false);
	void setAlarm01SoundVolume(float vol);
	void cancelAlarm01Sound();

signals:
	void keyBoardInputSignal(QString input, unsigned long userValue = 0);

private:
	static QSettings* _appSettings;
	static std::unique_ptr<OverlayController> singleton;

public:
	static OverlayController* getInstance() {
		return singleton.get();
	}

	static OverlayController* createInstance(bool desktopMode, bool noSound) {
		singleton.reset(new advsettings::OverlayController(desktopMode, noSound));
		return singleton.get();
	}

	static QSettings* appSettings() { return _appSettings; }

	static void setAppSettings(QSettings* settings) { _appSettings = settings; }
};

} // namespace advsettings
