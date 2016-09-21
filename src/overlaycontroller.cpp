#include "overlaycontroller.h"
#include "overlaywidget.h"
#include "ui_overlaywidget.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QApplication>
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

	OverlayController::~OverlayController() {
		m_pPumpEventsTimer.reset();
		vr::VR_Shutdown();
		m_pScene.reset();
		m_pFbo.reset();
		m_pOffscreenSurface.reset();
		m_pOpenGLContext.reset();
	}

	void OverlayController::Init() {
		// Loading the OpenVR Runtime
		auto initError = vr::VRInitError_None;
		vr::VR_Init(&initError, vr::VRApplication_Overlay);
		if (initError != vr::VRInitError_None) {
			throw std::runtime_error(std::string("Failed to initialize OpenVR: " + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(initError))));
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

		m_pScene.reset(new QGraphicsScene());
		connect(m_pScene.get(), SIGNAL(changed(const QList<QRectF>&)), this, SLOT(OnSceneChanged(const QList<QRectF>&)));
	}


	void OverlayController::SetWidget(OverlayWidget *pWidget, const std::string& name, const std::string& key) {
		// all of the mouse handling stuff requires that the widget be at 0,0
		pWidget->move(0, 0);
		m_pScene->addWidget(pWidget);
		m_pWidget = pWidget;
		pWidget->ui->VersionLabel->setText(OverlayController::applicationVersionString);

		if (!vr::VROverlay()) {
			QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "Is OpenVR running?");
			throw std::runtime_error(std::string("No Overlay interface"));
		}
		vr::VROverlayError overlayError = vr::VROverlay()->CreateDashboardOverlay(key.c_str(), name.c_str(), &m_ulOverlayHandle, &m_ulOverlayThumbnailHandle);
		if (overlayError != vr::VROverlayError_None) {
			if (overlayError == vr::VROverlayError_KeyInUse) {
				QMessageBox::critical(nullptr, "OpenVR Advanced Settings Overlay", "Another instance is already running.");
			}
			throw std::runtime_error(std::string("Failed to create Overlay: " + std::string(vr::VROverlay()->GetOverlayErrorNameFromEnum(overlayError))));
		}
		vr::VROverlay()->SetOverlayWidthInMeters(m_ulOverlayHandle, 2.5f);
		vr::VROverlay()->SetOverlayInputMethod(m_ulOverlayHandle, vr::VROverlayInputMethod_Mouse);
		std::string thumbIconPath = QApplication::applicationDirPath().toStdString() + "\\res\\thumbicon.png";
		if (QFile::exists(QString::fromStdString(thumbIconPath))) {
			vr::VROverlay()->SetOverlayFromFile(m_ulOverlayThumbnailHandle, thumbIconPath.c_str());
		} else {
			LOG(ERROR) << "Could not find thumbnail icon \"" << thumbIconPath << "\"";
		}

		m_pPumpEventsTimer.reset(new QTimer(this));
		connect(m_pPumpEventsTimer.get(), SIGNAL(timeout()), this, SLOT(OnTimeoutPumpEvents()));
		m_pPumpEventsTimer->setInterval(20);
		m_pPumpEventsTimer->start();

		QOpenGLFramebufferObjectFormat fboFormat;
		fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
		fboFormat.setTextureTarget(GL_TEXTURE_2D);

		m_pFbo.reset(new QOpenGLFramebufferObject(pWidget->width(), pWidget->height(), fboFormat));

		vr::HmdVector2_t vecWindowSize = {
			(float)pWidget->width(),
			(float)pWidget->height()
		};
		vr::VROverlay()->SetOverlayMouseScale(m_ulOverlayHandle, &vecWindowSize);

		steamVRTabController.init(this, pWidget);
		chaperoneTabController.init(this, pWidget);
		moveCenterTabController.init(this, pWidget);
		fixFloorTabController.init(this, pWidget);
		statisticsTabController.init(this, pWidget);
		settingsTabController.init(this, pWidget);
	}


	void OverlayController::OnSceneChanged(const QList<QRectF>&) {
		// skip rendering if the overlay isn't visible
		if (!vr::VROverlay() || !vr::VROverlay()->IsOverlayVisible(m_ulOverlayHandle) && !vr::VROverlay()->IsOverlayVisible(m_ulOverlayThumbnailHandle))
			return;

		m_pOpenGLContext->makeCurrent(m_pOffscreenSurface.get());
		m_pFbo->bind();

		QOpenGLPaintDevice device(m_pFbo->size());
		QPainter painter(&device);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

		m_pScene->render(&painter);

		m_pFbo->release();

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
		m_pOpenGLContext->functions()->glFlush(); // We need to flush otherwise the texture may be empty.
	}


	void OverlayController::OnTimeoutPumpEvents() {
		if (!vr::VRSystem())
			return;

		/*
		// tell OpenVR to make some events for us
		for( vr::TrackedDeviceIndex_t unDeviceId = 1; unDeviceId < vr::k_unControllerStateAxisCount; unDeviceId++ ) {
		if( vr::VROverlay()->HandleControllerOverlayInteractionAsMouse( m_ulOverlayHandle, unDeviceId ) ) {
		break;
		}
		}
		*/

		vr::TrackedDevicePose_t devicePoses[vr::k_unMaxTrackedDeviceCount];
		vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0.0f, devicePoses, vr::k_unMaxTrackedDeviceCount);
		fixFloorTabController.eventLoopTick(devicePoses);
		statisticsTabController.eventLoopTick(devicePoses);
		moveCenterTabController.eventLoopTick(vr::VRCompositor()->GetTrackingSpace());

		vr::VREvent_t vrEvent;
		while (vr::VROverlay()->PollNextOverlayEvent(m_ulOverlayHandle, &vrEvent, sizeof(vrEvent))) {
			switch (vrEvent.eventType) {
			case vr::VREvent_MouseMove: {
				QPointF ptNewMouse(vrEvent.data.mouse.x, vrEvent.data.mouse.y);
				QPoint ptGlobal = ptNewMouse.toPoint();
				QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseMove);
				mouseEvent.setWidget(NULL);
				mouseEvent.setPos(ptNewMouse);
				mouseEvent.setScenePos(ptGlobal);
				mouseEvent.setScreenPos(ptGlobal);
				mouseEvent.setLastPos(m_ptLastMouse);
				mouseEvent.setLastScenePos(m_pWidget->mapToGlobal(m_ptLastMouse.toPoint()));
				mouseEvent.setLastScreenPos(m_pWidget->mapToGlobal(m_ptLastMouse.toPoint()));
				mouseEvent.setButtons(m_lastMouseButtons);
				mouseEvent.setButton(Qt::NoButton);
				mouseEvent.setModifiers(0);
				mouseEvent.setAccepted(false);

				m_ptLastMouse = ptNewMouse;
				QApplication::sendEvent(m_pScene.get(), &mouseEvent);

				OnSceneChanged(QList<QRectF>());
			}
										break;

			case vr::VREvent_MouseButtonDown: {
				Qt::MouseButton button = vrEvent.data.mouse.button == vr::VRMouseButton_Right ? Qt::RightButton : Qt::LeftButton;

				m_lastMouseButtons |= button;

				QPoint ptGlobal = m_ptLastMouse.toPoint();
				QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMousePress);
				mouseEvent.setWidget(NULL);
				mouseEvent.setPos(m_ptLastMouse);
				mouseEvent.setButtonDownPos(button, m_ptLastMouse);
				mouseEvent.setButtonDownScenePos(button, ptGlobal);
				mouseEvent.setButtonDownScreenPos(button, ptGlobal);
				mouseEvent.setScenePos(ptGlobal);
				mouseEvent.setScreenPos(ptGlobal);
				mouseEvent.setLastPos(m_ptLastMouse);
				mouseEvent.setLastScenePos(ptGlobal);
				mouseEvent.setLastScreenPos(ptGlobal);
				mouseEvent.setButtons(m_lastMouseButtons);
				mouseEvent.setButton(button);
				mouseEvent.setModifiers(0);
				mouseEvent.setAccepted(false);

				QApplication::sendEvent(m_pScene.get(), &mouseEvent);
			}
											  break;

			case vr::VREvent_MouseButtonUp: {
				Qt::MouseButton button = vrEvent.data.mouse.button == vr::VRMouseButton_Right ? Qt::RightButton : Qt::LeftButton;
				m_lastMouseButtons &= ~button;

				QPoint ptGlobal = m_ptLastMouse.toPoint();
				QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseRelease);
				mouseEvent.setWidget(NULL);
				mouseEvent.setPos(m_ptLastMouse);
				mouseEvent.setScenePos(ptGlobal);
				mouseEvent.setScreenPos(ptGlobal);
				mouseEvent.setLastPos(m_ptLastMouse);
				mouseEvent.setLastScenePos(ptGlobal);
				mouseEvent.setLastScreenPos(ptGlobal);
				mouseEvent.setButtons(m_lastMouseButtons);
				mouseEvent.setButton(button);
				mouseEvent.setModifiers(0);
				mouseEvent.setAccepted(false);

				QApplication::sendEvent(m_pScene.get(), &mouseEvent);
			}
											break;

			case vr::VREvent_OverlayShown: {
				m_pWidget->repaint();
				UpdateWidget();
			}
										   break;

			case vr::VREvent_Quit: {
				LOG(INFO) << "Received quit request.";
				vr::VRSystem()->AcknowledgeQuit_Exiting(); // Let us buy some time just in case
				moveCenterTabController.MoveCenterResetClicked();
				QApplication::exit();
			}
								   break;

			case vr::VREvent_DashboardActivated: {
				LOG(INFO) << "Dashboard activated";
				dashboardVisible = true;
			}
												 break;

			case vr::VREvent_DashboardDeactivated: {
				LOG(INFO) << "Dashboard deactivated";
				dashboardVisible = false;
			}
												   break;
			}
		}

		if (m_ulOverlayThumbnailHandle != vr::k_ulOverlayHandleInvalid) {
			while (vr::VROverlay()->PollNextOverlayEvent(m_ulOverlayThumbnailHandle, &vrEvent, sizeof(vrEvent))) {
				switch (vrEvent.eventType) {
				case vr::VREvent_OverlayShown: {
					m_pWidget->repaint();
					UpdateWidget();
				}
											   break;
				}
			}
		}
	}

	void OverlayController::UpdateWidget() {
		steamVRTabController.UpdateTab();
		chaperoneTabController.UpdateTab();
		moveCenterTabController.UpdateTab();
		fixFloorTabController.UpdateTab();
		statisticsTabController.UpdateTab();
		settingsTabController.UpdateTab();
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
			vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(&newPos);
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


} // namespace advconfig
