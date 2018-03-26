#include "overlaycontroller.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QQuickView>
#include <QApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtWidgets/QWidget>
#include <QMouseEvent>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsEllipseItem>
#include <QOpenGLExtraFunctions>
#include <QCursor>
#include <QProcess>
#include <QMessageBox>
#include <iostream>
#include <cmath>
#include <openvr.h>
#include "logging.h"
#include "utils/Matrix.h"



// application namespace
namespace advsettings {

std::unique_ptr<OverlayController> OverlayController::singleton;

QSettings* OverlayController::_appSettings = nullptr;

OverlayController::~OverlayController() {
	Shutdown();
}

void OverlayController::Init(QQmlEngine* qmlEngine) {
	// Loading the OpenVR Runtime
	auto initError = vr::VRInitError_None;
	vr::VR_Init(&initError, vr::VRApplication_Overlay);
	if (initError != vr::VRInitError_None) {
		if (initError == vr::VRInitError_Init_HmdNotFound || initError == vr::VRInitError_Init_HmdNotFoundPresenceFailed) {
			QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "Could not find HMD!");
		}
		throw std::runtime_error(std::string("Failed to initialize OpenVR: ") + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(initError)));
	}

	m_runtimePathUrl = QUrl::fromLocalFile(vr::VR_RuntimePath());
	LOG(INFO) << "VR Runtime Path: " << m_runtimePathUrl.toLocalFile();

	QString activationSoundFile = m_runtimePathUrl.toLocalFile().append("/content/panorama/sounds/activation.wav");
	QFileInfo activationSoundFileInfo(activationSoundFile);
	if (activationSoundFileInfo.exists() && activationSoundFileInfo.isFile()) {
		activationSoundEffect.setSource(QUrl::fromLocalFile(activationSoundFile));
		activationSoundEffect.setVolume(1.0);
	} else {
		LOG(ERROR) << "Could not find activation sound file " << activationSoundFile;
	}

	QString focusChangedSoundFile = m_runtimePathUrl.toLocalFile().append("/content/panorama/sounds/focus_change.wav");
	QFileInfo focusChangedSoundFileInfo(focusChangedSoundFile);
	if (focusChangedSoundFileInfo.exists() && focusChangedSoundFileInfo.isFile()) {
		focusChangedSoundEffect.setSource(QUrl::fromLocalFile(focusChangedSoundFile));
		focusChangedSoundEffect.setVolume(1.0);
	} else {
		LOG(ERROR) << "Could not find focus changed sound file " << focusChangedSoundFile;
	}

	QString alarm01SoundFile = QApplication::applicationDirPath().append("/res/sounds/alarm01.wav");
	QFileInfo alarm01SoundFileInfo(alarm01SoundFile);
	if (alarm01SoundFileInfo.exists() && alarm01SoundFileInfo.isFile()) {
		alarm01SoundEffect.setSource(QUrl::fromLocalFile(alarm01SoundFile));
		alarm01SoundEffect.setVolume(1.0);
	} else {
		LOG(ERROR) << "Could not find alarm01 sound file " << alarm01SoundFile;
	}

	// Check whether OpenVR is too outdated
	if (!vr::VR_IsInterfaceVersionValid(vr::IVRSystem_Version)) {
		QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "OpenVR version is too outdated. Please update OpenVR.");
		throw std::runtime_error(std::string("OpenVR version is too outdated: Interface version ") + std::string(vr::IVRSystem_Version) + std::string(" not found."));
	} else if (!vr::VR_IsInterfaceVersionValid(vr::IVRSettings_Version)) {
		QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "OpenVR version is too outdated. Please update OpenVR.");
		throw std::runtime_error(std::string("OpenVR version is too outdated: Interface version ") + std::string(vr::IVRSettings_Version) + std::string(" not found."));
	} else if (!vr::VR_IsInterfaceVersionValid(vr::IVROverlay_Version)) {
		QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "OpenVR version is too outdated. Please update OpenVR.");
		throw std::runtime_error(std::string("OpenVR version is too outdated: Interface version ") + std::string(vr::IVROverlay_Version) + std::string(" not found."));
	} else if (!vr::VR_IsInterfaceVersionValid(vr::IVRApplications_Version)) {
		QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "OpenVR version is too outdated. Please update OpenVR.");
		throw std::runtime_error(std::string("OpenVR version is too outdated: Interface version ") + std::string(vr::IVRApplications_Version) + std::string(" not found."));
	} else if (!vr::VR_IsInterfaceVersionValid(vr::IVRChaperone_Version)) {
		QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "OpenVR version is too outdated. Please update OpenVR.");
		throw std::runtime_error(std::string("OpenVR version is too outdated: Interface version ") + std::string(vr::IVRChaperone_Version) + std::string(" not found."));
	} else if (!vr::VR_IsInterfaceVersionValid(vr::IVRChaperoneSetup_Version)) {
		QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "OpenVR version is too outdated. Please update OpenVR.");
		throw std::runtime_error(std::string("OpenVR version is too outdated: Interface version ") + std::string(vr::IVRChaperoneSetup_Version) + std::string(" not found."));
	} else if (!vr::VR_IsInterfaceVersionValid(vr::IVRCompositor_Version)) {
		QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "OpenVR version is too outdated. Please update OpenVR.");
		throw std::runtime_error(std::string("OpenVR version is too outdated: Interface version ") + std::string(vr::IVRCompositor_Version) + std::string(" not found."));
	} else if (!vr::VR_IsInterfaceVersionValid(vr::IVRNotifications_Version)) {
		QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "OpenVR version is too outdated. Please update OpenVR.");
		throw std::runtime_error(std::string("OpenVR version is too outdated: Interface version ") + std::string(vr::IVRNotifications_Version) + std::string(" not found."));
	}

	QSurfaceFormat format;
	// Qt's QOpenGLPaintDevice is not compatible with OpenGL versions >= 3.0
	// NVIDIA does not care, but unfortunately AMD does
	// Are subtle changes to the semantics of OpenGL functions actually covered by the compatibility profile,
	// and this is an AMD bug?
	format.setVersion(2, 1);
	//format.setProfile( QSurfaceFormat::CompatibilityProfile );
	format.setDepthBufferSize(16);
	format.setStencilBufferSize(8);
	format.setSamples(16);

	m_pOpenGLContext.reset(new QOpenGLContext());
	m_pOpenGLContext->setFormat(format);
	if (!m_pOpenGLContext->create()) {
		throw std::runtime_error("Could not create OpenGL context");
	}

	// create an offscreen surface to attach the context and FBO to
	m_pOffscreenSurface.reset(new QOffscreenSurface());
	m_pOffscreenSurface->setFormat(m_pOpenGLContext->format());
	m_pOffscreenSurface->create();
	m_pOpenGLContext->makeCurrent(m_pOffscreenSurface.get());

	if (!vr::VROverlay()) {
		QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "Is OpenVR running?");
		throw std::runtime_error(std::string("No Overlay interface"));
	}

	// Init controllers
	steamVRTabController.initStage1();
	chaperoneTabController.initStage1();
	moveCenterTabController.initStage1();
	fixFloorTabController.initStage1();
	audioTabController.initStage1();
	statisticsTabController.initStage1();
	settingsTabController.initStage1();
	reviveTabController.initStage1(settingsTabController.forceRevivePage());
	utilitiesTabController.initStage1();
	accessibilityTabController.initStage1();

	// Set qml context
	qmlEngine->rootContext()->setContextProperty("applicationVersion", getVersionString());
	qmlEngine->rootContext()->setContextProperty("vrRuntimePath", getVRRuntimePathUrl());

	// Register qml singletons
	qmlRegisterSingletonType<OverlayController>("matzman666.advsettings", 1, 0, "OverlayController", [](QQmlEngine*, QJSEngine*) {
		QObject* obj = getInstance();
		QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
		return obj;
	});
	qmlRegisterSingletonType<SteamVRTabController>("matzman666.advsettings", 1, 0, "SteamVRTabController", [](QQmlEngine*, QJSEngine*) {
		QObject* obj = &getInstance()->steamVRTabController;
		QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
		return obj;
	});
	qmlRegisterSingletonType<SteamVRTabController>("matzman666.advsettings", 1, 0, "ChaperoneTabController", [](QQmlEngine*, QJSEngine*) {
		QObject* obj = &getInstance()->chaperoneTabController;
		QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
		return obj;
	});
	qmlRegisterSingletonType<SteamVRTabController>("matzman666.advsettings", 1, 0, "MoveCenterTabController", [](QQmlEngine*, QJSEngine*) {
		QObject* obj = &getInstance()->moveCenterTabController;
		QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
		return obj;
	});
	qmlRegisterSingletonType<SteamVRTabController>("matzman666.advsettings", 1, 0, "FixFloorTabController", [](QQmlEngine*, QJSEngine*) {
		QObject* obj = &getInstance()->fixFloorTabController;
		QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
		return obj;
	});
	qmlRegisterSingletonType<SteamVRTabController>("matzman666.advsettings", 1, 0, "AudioTabController", [](QQmlEngine*, QJSEngine*) {
		QObject* obj = &getInstance()->audioTabController;
		QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
		return obj;
	});
	qmlRegisterSingletonType<SteamVRTabController>("matzman666.advsettings", 1, 0, "StatisticsTabController", [](QQmlEngine*, QJSEngine*) {
		QObject* obj = &getInstance()->statisticsTabController;
		QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
		return obj;
	});
	qmlRegisterSingletonType<SteamVRTabController>("matzman666.advsettings", 1, 0, "SettingsTabController", [](QQmlEngine*, QJSEngine*) {
		QObject* obj = &getInstance()->settingsTabController;
		QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
		return obj;
	});
	qmlRegisterSingletonType<SteamVRTabController>("matzman666.advsettings", 1, 0, "ReviveTabController", [](QQmlEngine*, QJSEngine*) {
		QObject* obj = &getInstance()->reviveTabController;
		QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
		return obj;
	});
	qmlRegisterSingletonType<SteamVRTabController>("matzman666.advsettings", 1, 0, "UtilitiesTabController", [](QQmlEngine*, QJSEngine*) {
		QObject* obj = &getInstance()->utilitiesTabController;
		QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
		return obj;
	});
	qmlRegisterSingletonType<SteamVRTabController>("matzman666.advsettings", 1, 0, "AccessibilityTabController", [](QQmlEngine*, QJSEngine*) {
		QObject* obj = &getInstance()->accessibilityTabController;
		QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
		return obj;
	});
}

void OverlayController::Shutdown() {
	if (m_pPumpEventsTimer) {
		disconnect(m_pPumpEventsTimer.get(), SIGNAL(timeout()), this, SLOT(OnTimeoutPumpEvents()));
		m_pPumpEventsTimer->stop();
		m_pPumpEventsTimer.reset();
	}
	if (m_pRenderTimer) {
		disconnect(m_pRenderControl.get(), SIGNAL(renderRequested()), this, SLOT(OnRenderRequest()));
		disconnect(m_pRenderControl.get(), SIGNAL(sceneChanged()), this, SLOT(OnRenderRequest()));
		disconnect(m_pRenderTimer.get(), SIGNAL(timeout()), this, SLOT(renderOverlay()));
		m_pRenderTimer->stop();
		m_pRenderTimer.reset();
	}
	m_pWindow.reset();
	m_pRenderControl.reset();
	m_pFbo.reset();
	m_pOpenGLContext.reset();
	m_pOffscreenSurface.reset();
}


void OverlayController::SetWidget(QQuickItem* quickItem, const std::string& name, const std::string& key) {
	if (!desktopMode) {
		vr::VROverlayError overlayError = vr::VROverlay()->CreateDashboardOverlay(key.c_str(), name.c_str(), &m_ulOverlayHandle, &m_ulOverlayThumbnailHandle);
		if (overlayError != vr::VROverlayError_None) {
			if (overlayError == vr::VROverlayError_KeyInUse) {
				QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "Another instance is already running.");
			}
			throw std::runtime_error(std::string("Failed to create Overlay: " + std::string(vr::VROverlay()->GetOverlayErrorNameFromEnum(overlayError))));
		}
		vr::VROverlay()->SetOverlayWidthInMeters(m_ulOverlayHandle, 2.5f);
		vr::VROverlay()->SetOverlayInputMethod(m_ulOverlayHandle, vr::VROverlayInputMethod_Mouse);
		vr::VROverlay()->SetOverlayFlag(m_ulOverlayHandle, vr::VROverlayFlags_SendVRScrollEvents, true);
		std::string thumbIconPath = QApplication::applicationDirPath().toStdString() + "\\res\\thumbicon.png";
		if (QFile::exists(QString::fromStdString(thumbIconPath))) {
			vr::VROverlay()->SetOverlayFromFile(m_ulOverlayThumbnailHandle, thumbIconPath.c_str());
		} else {
			LOG(ERROR) << "Could not find thumbnail icon \"" << thumbIconPath << "\"";
		}

		// Too many render calls in too short time overwhelm Qt and an assertion gets thrown.
		// Therefore we use an timer to delay render calls
		m_pRenderTimer.reset(new QTimer());
		m_pRenderTimer->setSingleShot(true);
		m_pRenderTimer->setInterval(5);
		connect(m_pRenderTimer.get(), SIGNAL(timeout()), this, SLOT(renderOverlay()));

		QOpenGLFramebufferObjectFormat fboFormat;
		fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
		fboFormat.setTextureTarget(GL_TEXTURE_2D);
		m_pFbo.reset(new QOpenGLFramebufferObject(quickItem->width(), quickItem->height(), fboFormat));

		m_pRenderControl.reset(new QQuickRenderControl());
		m_pWindow.reset(new QQuickWindow(m_pRenderControl.get()));
		m_pWindow->setRenderTarget(m_pFbo.get());
		quickItem->setParentItem(m_pWindow->contentItem());
		m_pWindow->setGeometry(0, 0, quickItem->width(), quickItem->height());
		m_pRenderControl->initialize(m_pOpenGLContext.get());

		vr::HmdVector2_t vecWindowSize = {
			(float)quickItem->width(),
			(float)quickItem->height()
		};
		vr::VROverlay()->SetOverlayMouseScale(m_ulOverlayHandle, &vecWindowSize);

		connect(m_pRenderControl.get(), SIGNAL(renderRequested()), this, SLOT(OnRenderRequest()));
		connect(m_pRenderControl.get(), SIGNAL(sceneChanged()), this, SLOT(OnRenderRequest()));
	}

	m_pPumpEventsTimer.reset(new QTimer());
	connect(m_pPumpEventsTimer.get(), SIGNAL(timeout()), this, SLOT(OnTimeoutPumpEvents()));
	m_pPumpEventsTimer->setInterval(20);
	m_pPumpEventsTimer->start();

	steamVRTabController.initStage2(this, m_pWindow.get());
	chaperoneTabController.initStage2(this, m_pWindow.get());
	moveCenterTabController.initStage2(this, m_pWindow.get());
	fixFloorTabController.initStage2(this, m_pWindow.get());
	audioTabController.initStage2(this, m_pWindow.get());
	statisticsTabController.initStage2(this, m_pWindow.get());
	settingsTabController.initStage2(this, m_pWindow.get());
	reviveTabController.initStage2(this, m_pWindow.get());
	utilitiesTabController.initStage2(this, m_pWindow.get());
	accessibilityTabController.initStage2(this, m_pWindow.get());
}


void OverlayController::OnRenderRequest() {
	if (m_pRenderTimer && !m_pRenderTimer->isActive()) {
		m_pRenderTimer->start();
	}
}

void OverlayController::renderOverlay() {
	if (!desktopMode) {
		// skip rendering if the overlay isn't visible
		if (!vr::VROverlay() || !vr::VROverlay()->IsOverlayVisible(m_ulOverlayHandle) && !vr::VROverlay()->IsOverlayVisible(m_ulOverlayThumbnailHandle))
			return;
		m_pRenderControl->polishItems();
		m_pRenderControl->sync();
		m_pRenderControl->render();

		GLuint unTexture = m_pFbo->texture();
		if (unTexture != 0) {
#if defined _WIN64 || defined _LP64
			// To avoid any compiler warning because of cast to a larger pointer type (warning C4312 on VC)
			vr::Texture_t texture = { (void*)((uint64_t)unTexture), vr::TextureType_OpenGL, vr::ColorSpace_Auto };
#else
			vr::Texture_t texture = { (void*)unTexture, vr::TextureType_OpenGL, vr::ColorSpace_Auto };
#endif
			vr::VROverlay()->SetOverlayTexture(m_ulOverlayHandle, &texture);
		}
		m_pOpenGLContext->functions()->glFlush(); // We need to flush otherwise the texture may be empty.*/
	}
}

bool OverlayController::pollNextEvent(vr::VROverlayHandle_t ulOverlayHandle, vr::VREvent_t* pEvent) {
	if (isDesktopMode()) {
		return vr::VRSystem()->PollNextEvent(pEvent, sizeof(vr::VREvent_t));
	} else {
		return vr::VROverlay()->PollNextOverlayEvent(ulOverlayHandle, pEvent, sizeof(vr::VREvent_t));
	}
}

void OverlayController::OnTimeoutPumpEvents() {
	if (!vr::VRSystem())
		return;

	vr::VREvent_t vrEvent;
	bool chaperoneDataAlreadyUpdated = false;
	while (pollNextEvent(m_ulOverlayHandle, &vrEvent)) {
		switch (vrEvent.eventType) {
			case vr::VREvent_MouseMove: {
				QPoint ptNewMouse(vrEvent.data.mouse.x, vrEvent.data.mouse.y);
				if (ptNewMouse != m_ptLastMouse) {
					QMouseEvent mouseEvent( QEvent::MouseMove, ptNewMouse, m_pWindow->mapToGlobal(ptNewMouse), Qt::NoButton, m_lastMouseButtons, 0 );
					m_ptLastMouse = ptNewMouse;
					QCoreApplication::sendEvent(m_pWindow.get(), &mouseEvent);
					OnRenderRequest();
				}
			}
			break;

			case vr::VREvent_MouseButtonDown: {
				QPoint ptNewMouse(vrEvent.data.mouse.x, vrEvent.data.mouse.y);
				Qt::MouseButton button = vrEvent.data.mouse.button == vr::VRMouseButton_Right ? Qt::RightButton : Qt::LeftButton;
				m_lastMouseButtons |= button;
				QMouseEvent mouseEvent(QEvent::MouseButtonPress, ptNewMouse, m_pWindow->mapToGlobal(ptNewMouse), button, m_lastMouseButtons, 0);
				QCoreApplication::sendEvent(m_pWindow.get(), &mouseEvent);
			}
			break;

			case vr::VREvent_MouseButtonUp: {
				QPoint ptNewMouse(vrEvent.data.mouse.x, vrEvent.data.mouse.y);
				Qt::MouseButton button = vrEvent.data.mouse.button == vr::VRMouseButton_Right ? Qt::RightButton : Qt::LeftButton;
				m_lastMouseButtons &= ~button;
				QMouseEvent mouseEvent(QEvent::MouseButtonRelease, ptNewMouse, m_pWindow->mapToGlobal(ptNewMouse), button, m_lastMouseButtons, 0);
				QCoreApplication::sendEvent(m_pWindow.get(), &mouseEvent);
			}
			break;

			case vr::VREvent_Scroll: {
				// Wheel speed is defined as 1/8 of a degree
				QWheelEvent wheelEvent(m_ptLastMouse, m_pWindow->mapToGlobal(m_ptLastMouse), QPoint(),
					QPoint(vrEvent.data.scroll.xdelta * 360.0f * 8.0f, vrEvent.data.scroll.ydelta * 360.0f * 8.0f),
					0, Qt::Vertical, m_lastMouseButtons, 0);
				QCoreApplication::sendEvent(m_pWindow.get(), &wheelEvent);
			}
			break;

			case vr::VREvent_OverlayShown: {
				m_pWindow->update();
			}
			break;

			case vr::VREvent_Quit: {
				LOG(INFO) << "Received quit request.";
				vr::VRSystem()->AcknowledgeQuit_Exiting(); // Let us buy some time just in case
				moveCenterTabController.reset();
				chaperoneTabController.shutdown();
				Shutdown();
				QApplication::exit();
				return;
			}
			break;

			case vr::VREvent_DashboardActivated: {
				LOG(DEBUG) << "Dashboard activated";
				dashboardVisible = true;
			}
			break;

			case vr::VREvent_DashboardDeactivated: {
				LOG(DEBUG) << "Dashboard deactivated";
				dashboardVisible = false;
			}
			break;

			case vr::VREvent_KeyboardDone: {
				char keyboardBuffer[1024];
				vr::VROverlay()->GetKeyboardText(keyboardBuffer, 1024);
				emit keyBoardInputSignal(QString(keyboardBuffer), vrEvent.data.keyboard.uUserValue);
			}
			break;

			// Multiple ChaperoneUniverseHasChanged are often emitted at the same time (some with a little bit of delay)
			// There is no sure way to recognize redundant events, we can only exclude redundant events during the same call of OnTimeoutPumpEvents()  
			case vr::VREvent_ChaperoneUniverseHasChanged:
			case vr::VREvent_ChaperoneDataHasChanged: {
				if (!chaperoneDataAlreadyUpdated) {
					LOG(INFO) << "Re-loading chaperone data ...";
					m_chaperoneUtils.loadChaperoneData();
					LOG(INFO) << "Found " << m_chaperoneUtils.quadsCount() << " chaperone quads.";
					if (m_chaperoneUtils.isChaperoneWellFormed()) {
						LOG(INFO) << "Chaperone data seems to be well-formed.";
					} else {
						LOG(INFO) << "Chaperone data is NOT well-formed.";
					}
					chaperoneDataAlreadyUpdated = true;
				}
			}
			break;
		}
	}

	vr::TrackedDevicePose_t devicePoses[vr::k_unMaxTrackedDeviceCount];
	vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0.0f, devicePoses, vr::k_unMaxTrackedDeviceCount);
	
	// HMD/Controller Velocities
	auto leftId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
	float leftSpeed = 0.0f;
	if (leftId != vr::k_unTrackedDeviceIndexInvalid && devicePoses[leftId].bPoseIsValid && devicePoses[leftId].eTrackingResult == vr::TrackingResult_Running_OK) {
		auto& vel = devicePoses[leftId].vVelocity.v;
		leftSpeed = std::sqrt(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]);
	}
	auto rightId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
	auto rightSpeed = 0.0f;
	if (rightId != vr::k_unTrackedDeviceIndexInvalid && devicePoses[rightId].bPoseIsValid && devicePoses[rightId].eTrackingResult == vr::TrackingResult_Running_OK) {
		auto& vel = devicePoses[rightId].vVelocity.v;
		rightSpeed = std::sqrt(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]);
	}
	auto hmdSpeed = 0.0f;
	if (devicePoses[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid && devicePoses[vr::k_unTrackedDeviceIndex_Hmd].eTrackingResult == vr::TrackingResult_Running_OK) {
		auto& vel = devicePoses[vr::k_unTrackedDeviceIndex_Hmd].vVelocity.v;
		hmdSpeed = std::sqrt(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]);
	}
	
	fixFloorTabController.eventLoopTick(devicePoses);
	statisticsTabController.eventLoopTick(devicePoses, leftSpeed, rightSpeed);
	moveCenterTabController.eventLoopTick(vr::VRCompositor()->GetTrackingSpace(), devicePoses);
	steamVRTabController.eventLoopTick();
	chaperoneTabController.eventLoopTick(devicePoses, leftSpeed, rightSpeed, hmdSpeed);
	settingsTabController.eventLoopTick();
	reviveTabController.eventLoopTick();
	audioTabController.eventLoopTick();
	utilitiesTabController.eventLoopTick();
	accessibilityTabController.eventLoopTick(vr::VRCompositor()->GetTrackingSpace());

	if (m_ulOverlayThumbnailHandle != vr::k_ulOverlayHandleInvalid) {
		while (vr::VROverlay()->PollNextOverlayEvent(m_ulOverlayThumbnailHandle, &vrEvent, sizeof(vrEvent))) {
			switch (vrEvent.eventType) {
				case vr::VREvent_OverlayShown: {
					m_pWindow->update();
				}
				break;
			}
		}
	}
}

void OverlayController::AddOffsetToUniverseCenter(vr::ETrackingUniverseOrigin universe, unsigned axisId, float offset, bool adjustBounds, bool commit) {
	float offsetArray[3] = { 0,0,0 };
	offsetArray[axisId] = offset;
	AddOffsetToUniverseCenter(universe, offsetArray, adjustBounds, commit);
}

void OverlayController::AddOffsetToUniverseCenter(vr::ETrackingUniverseOrigin universe, float offset[3], bool adjustBounds, bool commit) {
	if (offset[0] != 0.0f || offset[1] != 0.0f || offset[2] != 0.0f) {
		if (commit) {
			vr::VRChaperoneSetup()->RevertWorkingCopy();
		}
		vr::HmdMatrix34_t curPos;
		if (universe == vr::TrackingUniverseStanding) {
			vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(&curPos);
		} else {
			vr::VRChaperoneSetup()->GetWorkingSeatedZeroPoseToRawTrackingPose(&curPos);
		}
		for (int i = 0; i < 3; i++) {
			curPos.m[0][3] += curPos.m[0][i] * offset[i];
			curPos.m[1][3] += curPos.m[1][i] * offset[i];
			curPos.m[2][3] += curPos.m[2][i] * offset[i];
		}
		if (universe == vr::TrackingUniverseStanding) {
			vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(&curPos);
		} else {
			vr::VRChaperoneSetup()->SetWorkingSeatedZeroPoseToRawTrackingPose(&curPos);
		}
		if (adjustBounds && universe == vr::TrackingUniverseStanding) {
			float collisionOffset[] = {
				-offset[0],
				-offset[1],
				-offset[2]
			};
			AddOffsetToCollisionBounds(collisionOffset, false);
		}
		if (commit) {
			vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
		}
	}
}

void OverlayController::RotateUniverseCenter(vr::ETrackingUniverseOrigin universe, float yAngle, bool adjustBounds, bool commit) {
	if (yAngle != 0.0f) {
		if (commit) {
			vr::VRChaperoneSetup()->RevertWorkingCopy();
		}
		vr::HmdMatrix34_t curPos;
		if (universe == vr::TrackingUniverseStanding) {
			vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(&curPos);
		} else {
			vr::VRChaperoneSetup()->GetWorkingSeatedZeroPoseToRawTrackingPose(&curPos);
		}

		vr::HmdMatrix34_t rotMat;
		vr::HmdMatrix34_t newPos;
		utils::initRotationMatrix(rotMat, 1, yAngle);
		utils::matMul33(newPos, rotMat, curPos);
		newPos.m[0][3] = curPos.m[0][3];
		newPos.m[1][3] = curPos.m[1][3];
		newPos.m[2][3] = curPos.m[2][3];
		if (universe == vr::TrackingUniverseStanding) {
			vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(&newPos);
		} else {
			vr::VRChaperoneSetup()->SetWorkingSeatedZeroPoseToRawTrackingPose(&newPos);
		}
		if (adjustBounds && universe == vr::TrackingUniverseStanding) {
			RotateCollisionBounds(-yAngle, false);
		}
		if (commit) {
			vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
		}
	}
}


void OverlayController::AddOffsetToCollisionBounds(unsigned axisId, float offset, bool commit) {
	float offsetArray[3] = { 0,0,0 };
	offsetArray[axisId] = offset;
	AddOffsetToCollisionBounds(offsetArray, commit);
}

void OverlayController::AddOffsetToCollisionBounds(float offset[3], bool commit) {
	// Apparently Valve sanity-checks the y-coordinates of the collision bounds (and only the y-coordinates)
	// I can move the bounds on the xz-plane, I can make the "ceiling" of the chaperone cage lower/higher, but when I
	// dare to set one single lower corner to something non-zero, every corner gets its y-coordinates reset to the defaults.
	// I do it anyway, maybe it gets fixed in the future.
	if (commit) {
		vr::VRChaperoneSetup()->RevertWorkingCopy();
	}
	unsigned collisionBoundsCount = 0;
	vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(nullptr, &collisionBoundsCount);
	if (collisionBoundsCount > 0) {
		vr::HmdQuad_t* collisionBounds = new vr::HmdQuad_t[collisionBoundsCount];
		vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(collisionBounds, &collisionBoundsCount);
		for (unsigned b = 0; b < collisionBoundsCount; b++) {
			for (unsigned c = 0; c < 4; c++) {
				collisionBounds[b].vCorners[c].v[0] += offset[0];
				collisionBounds[b].vCorners[c].v[1] += offset[1];
				collisionBounds[b].vCorners[c].v[2] += offset[2];
			}
		}
		vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(collisionBounds, collisionBoundsCount);
		delete collisionBounds;
	}
	if (commit && collisionBoundsCount > 0) {
		vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
	}
}

void OverlayController::RotateCollisionBounds(float angle, bool commit) {
	if (commit) {
		vr::VRChaperoneSetup()->RevertWorkingCopy();
	}
	unsigned collisionBoundsCount = 0;
	vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(nullptr, &collisionBoundsCount);
	if (collisionBoundsCount > 0) {
		vr::HmdQuad_t* collisionBounds = new vr::HmdQuad_t[collisionBoundsCount];
		vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(collisionBounds, &collisionBoundsCount);

		vr::HmdMatrix34_t rotMat;
		utils::initRotationMatrix(rotMat, 1, angle);
		for (unsigned b = 0; b < collisionBoundsCount; b++) {
			for (unsigned c = 0; c < 4; c++) {
				auto& corner = collisionBounds[b].vCorners[c];
				vr::HmdVector3_t newVal;
				utils::matMul33(newVal, rotMat, corner);
				corner = newVal;
			}
		}
		vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(collisionBounds, collisionBoundsCount);
		delete collisionBounds;
	}
	if (commit && collisionBoundsCount > 0) {
		vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
	}
}

QString OverlayController::getVersionString() {
	return QString(applicationVersionString);
}


QUrl OverlayController::getVRRuntimePathUrl() {
	return m_runtimePathUrl;
}


bool OverlayController::soundDisabled() {
	return noSound;
}


const vr::VROverlayHandle_t& OverlayController::overlayHandle() {
	return m_ulOverlayHandle;
}


const vr::VROverlayHandle_t& OverlayController::overlayThumbnailHandle() {
	return m_ulOverlayThumbnailHandle;
}


void OverlayController::showKeyboard(QString existingText, unsigned long userValue) {
	vr::VROverlay()->ShowKeyboardForOverlay(m_ulOverlayHandle, vr::k_EGamepadTextInputModeNormal, vr::k_EGamepadTextInputLineModeSingleLine, "Advanced Settings Overlay", 1024, existingText.toStdString().c_str(), false, userValue);
}


void OverlayController::playActivationSound() {
	if (!noSound) {
		activationSoundEffect.play();
	}
}


void OverlayController::playFocusChangedSound() {
	if (!noSound) {
		focusChangedSoundEffect.play();
	}
}

void OverlayController::playAlarm01Sound(bool loop) {
	if (!noSound && !alarm01SoundEffect.isPlaying()) {
		if (loop) {
			alarm01SoundEffect.setLoopCount(QSoundEffect::Infinite);
		} else {
			alarm01SoundEffect.setLoopCount(1);
		}
		alarm01SoundEffect.play();
	}
}

void OverlayController::setAlarm01SoundVolume(float vol) {
	alarm01SoundEffect.setVolume(vol);
}

void OverlayController::cancelAlarm01Sound() {
	alarm01SoundEffect.stop();
}


} // namespace advconfig
