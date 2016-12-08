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
#include <exception>
#include <iostream>
#include <cmath>
#include <openvr.h>
#include "logging.h"



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
		throw std::runtime_error(std::string("Failed to initialize OpenVR: " + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(initError))));
	}

	m_runtimePathUrl = QUrl::fromLocalFile(vr::VR_RuntimePath());
	LOG(INFO) << "VR Runtime Path: " << m_runtimePathUrl.toLocalFile();

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
}

void OverlayController::Shutdown() {
	if (m_pPumpEventsTimer) {
		disconnect(m_pPumpEventsTimer.get(), SIGNAL(timeout()), this, SLOT(OnTimeoutPumpEvents()));
		m_pPumpEventsTimer->stop();
	}
	if (m_pRenderTimer) {
		disconnect(m_pRenderControl.get(), SIGNAL(renderRequested()), this, SLOT(OnRenderRequest()));
		disconnect(m_pRenderControl.get(), SIGNAL(sceneChanged()), this, SLOT(OnRenderRequest()));
		disconnect(m_pRenderTimer.get(), SIGNAL(timeout()), this, SLOT(renderOverlay()));
		m_pRenderTimer->stop();
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
			vr::Texture_t texture = { (void*)((uint64_t)unTexture), vr::API_OpenGL, vr::ColorSpace_Auto };
#else
			vr::Texture_t texture = { (void*)unTexture, vr::API_OpenGL, vr::ColorSpace_Auto };
#endif
			vr::VROverlay()->SetOverlayTexture(m_ulOverlayHandle, &texture);
		}
		m_pOpenGLContext->functions()->glFlush(); // We need to flush otherwise the texture may be empty.*/
	}
}


void OverlayController::OnTimeoutPumpEvents() {
	if (!vr::VRSystem())
		return;

	// tell OpenVR to make some events for us
	/*for( vr::TrackedDeviceIndex_t unDeviceId = 1; unDeviceId < vr::k_unControllerStateAxisCount; unDeviceId++ ) {
		if( vr::VROverlay()->HandleControllerOverlayInteractionAsMouse( m_ulOverlayHandle, unDeviceId ) ) {
			break;
		}
	}*/
	
	vr::TrackedDevicePose_t devicePoses[vr::k_unMaxTrackedDeviceCount];
	vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0.0f, devicePoses, vr::k_unMaxTrackedDeviceCount);
	fixFloorTabController.eventLoopTick(devicePoses);
	statisticsTabController.eventLoopTick(devicePoses);
	moveCenterTabController.eventLoopTick(vr::VRCompositor()->GetTrackingSpace());
	steamVRTabController.eventLoopTick();
	chaperoneTabController.eventLoopTick();
	settingsTabController.eventLoopTick();
	audioTabController.eventLoopTick();

	vr::VREvent_t vrEvent;
	while (vr::VROverlay()->PollNextOverlayEvent(m_ulOverlayHandle, &vrEvent, sizeof(vrEvent))) {
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
				Shutdown();
				QApplication::exit();
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
		}
	}

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
	if (offset != 0.0f) {
		if (commit) {
			vr::VRChaperoneSetup()->RevertWorkingCopy();
		}
		vr::HmdMatrix34_t curPos;
		if (universe == vr::TrackingUniverseStanding) {
			vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(&curPos);
		} else {
			vr::VRChaperoneSetup()->GetWorkingSeatedZeroPoseToRawTrackingPose(&curPos);
		}
		curPos.m[0][3] += curPos.m[0][axisId] * offset;
		curPos.m[1][3] += curPos.m[1][axisId] * offset;
		curPos.m[2][3] += curPos.m[2][axisId] * offset;
		if (universe == vr::TrackingUniverseStanding) {
			vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(&curPos);
		} else {
			vr::VRChaperoneSetup()->SetWorkingSeatedZeroPoseToRawTrackingPose(&curPos);
		}
		if (adjustBounds && universe == vr::TrackingUniverseStanding) {
			AddOffsetToCollisionBounds(axisId, -offset, false);
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
		vr::HmdMatrix34_t newPos;
		float yRot[3][3] = {
			{ std::cos(yAngle), 0, std::sin(yAngle) },
			{ 0, 1, 0 },
			{ -std::sin(yAngle), 0, std::cos(yAngle) }
		};
		for (unsigned i = 0; i < 3; i++) {
			for (unsigned j = 0; j < 3; j++) {
				newPos.m[i][j] = 0.0f;
				for (unsigned k = 0; k < 3; k++) {
					newPos.m[i][j] += yRot[i][k] * curPos.m[k][j];
				}
			}
		}
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
				collisionBounds[b].vCorners[c].v[axisId] += offset;
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
		float yRot[3][3] = {
			{ std::cos(angle), 0, std::sin(angle) },
			{ 0, 1, 0 },
			{ -std::sin(angle), 0, std::cos(angle) }
		};
		for (unsigned b = 0; b < collisionBoundsCount; b++) {
			for (unsigned c = 0; c < 4; c++) {
				auto& corner = collisionBounds[b].vCorners[c];
				vr::HmdVector3_t newVal;
				for (unsigned i = 0; i < 3; i++) {
					newVal.v[i] = 0.0f;
					for (unsigned k = 0; k < 3; k++) {
						newVal.v[i] += corner.v[k] * yRot[i][k];
					};
				}
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


void OverlayController::showKeyboard(QString existingText, unsigned long userValue) {
	vr::VROverlay()->ShowKeyboardForOverlay(m_ulOverlayHandle, vr::k_EGamepadTextInputModeNormal, vr::k_EGamepadTextInputLineModeSingleLine, "Advanced Settings Overlay", 1024, existingText.toStdString().c_str(), false, userValue);
}


} // namespace advconfig
