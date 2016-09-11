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
#include <exception>
#include <iostream>
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
	format.setMajorVersion( 4 );
	format.setMinorVersion( 1 );
	format.setProfile( QSurfaceFormat::CompatibilityProfile );

	m_pOpenGLContext.reset(new QOpenGLContext());
	m_pOpenGLContext->setFormat( format );
	if (!m_pOpenGLContext->create()) {
		throw std::runtime_error("Could not create OpenGL context");
	}

	// create an offscreen surface to attach the context and FBO to
	m_pOffscreenSurface.reset(new QOffscreenSurface());
	m_pOffscreenSurface->create();
	m_pOpenGLContext->makeCurrent( m_pOffscreenSurface.get() );

	m_pScene.reset(new QGraphicsScene());
	connect( m_pScene.get(), SIGNAL(changed(const QList<QRectF>&)), this, SLOT( OnSceneChanged(const QList<QRectF>&)) );
}


void OverlayController::SetWidget(OverlayWidget *pWidget, const std::string& name, const std::string& key) {
	// all of the mouse handling stuff requires that the widget be at 0,0
	pWidget->move(0, 0);
	m_pScene->addWidget(pWidget);
	m_pWidget = pWidget;

	if (!vr::VROverlay()) {
		throw std::runtime_error(std::string("No Overlay interface"));
	}
	vr::VROverlayError overlayError = vr::VROverlay()->CreateDashboardOverlay(key.c_str(), name.c_str(), &m_ulOverlayHandle, &m_ulOverlayThumbnailHandle);
	if (overlayError != vr::VROverlayError_None) {
		throw std::runtime_error(std::string("Failed to create Overlay: " + std::string(vr::VROverlay()->GetOverlayErrorNameFromEnum(overlayError))));
	}
	vr::VROverlay()->SetOverlayWidthInMeters(m_ulOverlayHandle, 2.0f);
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

	m_pFbo.reset(new QOpenGLFramebufferObject(pWidget->width(), pWidget->height(), GL_TEXTURE_2D));

	vr::HmdVector2_t vecWindowSize = {
		(float)pWidget->width(),
		(float)pWidget->height()
	};
	vr::VROverlay()->SetOverlayMouseScale(m_ulOverlayHandle, &vecWindowSize);

	chaperoneTabController.init(this, pWidget);
	moveCenterTabController.init(this, pWidget);
	fixFloorTabController.init(this, pWidget);
	settingsTabController.init(this, pWidget);
}


void OverlayController::OnSceneChanged( const QList<QRectF>& ) {
	// skip rendering if the overlay isn't visible
    if( !vr::VROverlay() || !vr::VROverlay()->IsOverlayVisible( m_ulOverlayHandle ) && !vr::VROverlay()->IsOverlayVisible( m_ulOverlayThumbnailHandle ) )
        return;

	m_pOpenGLContext->makeCurrent( m_pOffscreenSurface.get() );
	m_pFbo->bind();
	
	QOpenGLPaintDevice device( m_pFbo->size() );
	QPainter painter( &device );

	m_pScene->render( &painter );

	m_pFbo->release();

	GLuint unTexture = m_pFbo->texture();
	if( unTexture != 0 ) {
#if defined _WIN64 || defined _LP64
		// To avoid any compiler warning because of cast to a larger pointer type (warning C4312 on VC)
		vr::Texture_t texture = { (void*)((uint64_t)unTexture), vr::API_OpenGL, vr::ColorSpace_Auto };
#else
		vr::Texture_t texture = { (void*)unTexture, vr::API_OpenGL, vr::ColorSpace_Auto };
#endif
        vr::VROverlay()->SetOverlayTexture( m_ulOverlayHandle, &texture );
	}
	m_pOpenGLContext->functions()->glFlush(); // We need to flush otherwise the texture may be empty.
}


void OverlayController::OnTimeoutPumpEvents() {
    if( !vr::VRSystem() )
		return;

	/*
	// tell OpenVR to make some events for us
	for( vr::TrackedDeviceIndex_t unDeviceId = 1; unDeviceId < vr::k_unControllerStateAxisCount; unDeviceId++ ) {
        if( vr::VROverlay()->HandleControllerOverlayInteractionAsMouse( m_ulOverlayHandle, unDeviceId ) ) {
			break;
		}
	}
	*/

	fixFloorTabController.eventLoopTick();

	vr::VREvent_t vrEvent;
    while( vr::VROverlay()->PollNextOverlayEvent( m_ulOverlayHandle, &vrEvent, sizeof( vrEvent )  ) ) {
		switch( vrEvent.eventType ) {
			case vr::VREvent_MouseMove: {
				QPointF ptNewMouse( vrEvent.data.mouse.x, vrEvent.data.mouse.y );
				QPoint ptGlobal = ptNewMouse.toPoint();
				QGraphicsSceneMouseEvent mouseEvent( QEvent::GraphicsSceneMouseMove );
				mouseEvent.setWidget( NULL );
				mouseEvent.setPos( ptNewMouse );
				mouseEvent.setScenePos( ptGlobal );
				mouseEvent.setScreenPos( ptGlobal );
				mouseEvent.setLastPos( m_ptLastMouse );
				mouseEvent.setLastScenePos( m_pWidget->mapToGlobal( m_ptLastMouse.toPoint() ) );
				mouseEvent.setLastScreenPos( m_pWidget->mapToGlobal( m_ptLastMouse.toPoint() ) );
				mouseEvent.setButtons( m_lastMouseButtons );
				mouseEvent.setButton( Qt::NoButton );
				mouseEvent.setModifiers( 0 );
				mouseEvent.setAccepted( false );

				m_ptLastMouse = ptNewMouse;
				QApplication::sendEvent( m_pScene.get(), &mouseEvent );

				OnSceneChanged( QList<QRectF>() );
			}
			break;

			case vr::VREvent_MouseButtonDown: {
				Qt::MouseButton button = vrEvent.data.mouse.button == vr::VRMouseButton_Right ? Qt::RightButton : Qt::LeftButton;

				m_lastMouseButtons |= button;

				QPoint ptGlobal = m_ptLastMouse.toPoint();
				QGraphicsSceneMouseEvent mouseEvent( QEvent::GraphicsSceneMousePress );
				mouseEvent.setWidget( NULL );
				mouseEvent.setPos( m_ptLastMouse );
				mouseEvent.setButtonDownPos( button, m_ptLastMouse );
				mouseEvent.setButtonDownScenePos( button, ptGlobal);
				mouseEvent.setButtonDownScreenPos( button, ptGlobal );
				mouseEvent.setScenePos( ptGlobal );
				mouseEvent.setScreenPos( ptGlobal );
				mouseEvent.setLastPos( m_ptLastMouse );
				mouseEvent.setLastScenePos( ptGlobal );
				mouseEvent.setLastScreenPos( ptGlobal );
				mouseEvent.setButtons( m_lastMouseButtons );
				mouseEvent.setButton( button );
				mouseEvent.setModifiers( 0 );
				mouseEvent.setAccepted( false );

				QApplication::sendEvent( m_pScene.get(), &mouseEvent );
			}
			break;

		case vr::VREvent_MouseButtonUp: {
				Qt::MouseButton button = vrEvent.data.mouse.button == vr::VRMouseButton_Right ? Qt::RightButton : Qt::LeftButton;
				m_lastMouseButtons &= ~button;

				QPoint ptGlobal = m_ptLastMouse.toPoint();
				QGraphicsSceneMouseEvent mouseEvent( QEvent::GraphicsSceneMouseRelease );
				mouseEvent.setWidget( NULL );
				mouseEvent.setPos( m_ptLastMouse );
				mouseEvent.setScenePos( ptGlobal );
				mouseEvent.setScreenPos( ptGlobal );
				mouseEvent.setLastPos( m_ptLastMouse );
				mouseEvent.setLastScenePos( ptGlobal );
				mouseEvent.setLastScreenPos( ptGlobal );
				mouseEvent.setButtons( m_lastMouseButtons );
				mouseEvent.setButton( button );
				mouseEvent.setModifiers( 0 );
				mouseEvent.setAccepted( false );

				QApplication::sendEvent( m_pScene.get(), &mouseEvent );
			}
			break;

		case vr::VREvent_OverlayShown: {
				m_pWidget->repaint();
				UpdateWidget();
			}
			break;

        case vr::VREvent_Quit:
			LOG(INFO) << "Received quit request.";
			vr::VRSystem()->AcknowledgeQuit_Exiting(); // Let us buy some time just in case
			moveCenterTabController.MoveCenterResetClicked();
            QApplication::exit();
            break;
		}
	}

    if( m_ulOverlayThumbnailHandle != vr::k_ulOverlayHandleInvalid ) {
        while( vr::VROverlay()->PollNextOverlayEvent( m_ulOverlayThumbnailHandle, &vrEvent, sizeof( vrEvent)  ) ) {
            switch( vrEvent.eventType ) {
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
	chaperoneTabController.UpdateTab();
	moveCenterTabController.UpdateTab();
	fixFloorTabController.UpdateTab();
	settingsTabController.UpdateTab();
}

void OverlayController::AddOffsetToStandingPosition(unsigned axisId, float offset) {
	vr::VRChaperoneSetup()->RevertWorkingCopy();
	vr::HmdMatrix34_t curPos;
	vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(&curPos);
	curPos.m[0][3] += curPos.m[0][axisId] * offset;
	curPos.m[1][3] += curPos.m[1][axisId] * offset;
	curPos.m[2][3] += curPos.m[2][axisId] * offset;
	vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(&curPos);
	vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
}

void ChaperoneTabController::init(OverlayController * parent, OverlayWidget * widget) {
	this->parent = parent;
	this->widget = widget;

	connect(widget->ui->CenterMarkerToggle, SIGNAL(toggled(bool)), this, SLOT(CenterMarkerToggled(bool)));
	connect(widget->ui->PlaySpaceToggle, SIGNAL(toggled(bool)), this, SLOT(PlaySpaceToggled(bool)));
	connect(widget->ui->ForceBoundsToggle, SIGNAL(toggled(bool)), this, SLOT(ForceBoundsToggled(bool)));
	connect(widget->ui->FadeDistanceSlider, SIGNAL(valueChanged(int)), this, SLOT(FadeDistanceChanged(int)));
	connect(widget->ui->ColorAlphaSlider, SIGNAL(valueChanged(int)), this, SLOT(ColorAlphaChanged(int)));
	connect(widget->ui->ResetDefaultsToggle, SIGNAL(clicked()), this, SLOT(ResetDefaultsClicked()));
}

void ChaperoneTabController::UpdateTab() {
	if (widget && vr::VRSettings()) {
		widget->ui->CenterMarkerToggle->blockSignals(true);
		widget->ui->PlaySpaceToggle->blockSignals(true);
		widget->ui->ForceBoundsToggle->blockSignals(true);
		widget->ui->FadeDistanceSlider->blockSignals(true);
		widget->ui->ColorAlphaSlider->blockSignals(true);

		auto centerMarkerOn = vr::VRSettings()->GetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_CenterMarkerOn_Bool, false);
		widget->ui->CenterMarkerToggle->setChecked(centerMarkerOn);
		auto playSpaceOn = vr::VRSettings()->GetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_PlaySpaceOn_Bool, false);
		widget->ui->PlaySpaceToggle->setChecked(playSpaceOn);
		widget->ui->ForceBoundsToggle->setChecked(forceBoundsFlag);
		auto fadeDistance = vr::VRSettings()->GetFloat(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_FadeDistance_Float, 0.7f);
		widget->ui->FadeDistanceSlider->setValue(fadeDistance*100);
		widget->ui->FadeDistanceLabel->setText(QString::asprintf("%.2f", fadeDistance));
		auto colorAlpha = vr::VRSettings()->GetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaA_Int32, 255);
		widget->ui->ColorAlphaSlider->setValue(colorAlpha*100/255);
		widget->ui->ColorAlphaLabel->setText(QString::asprintf("%i%%", colorAlpha*100/255));

		widget->ui->CenterMarkerToggle->blockSignals(false);
		widget->ui->PlaySpaceToggle->blockSignals(false);
		widget->ui->ForceBoundsToggle->blockSignals(false);
		widget->ui->FadeDistanceSlider->blockSignals(false);
		widget->ui->ColorAlphaSlider->blockSignals(false);
	}
}

void ChaperoneTabController::CenterMarkerToggled(bool value) {
	if (vr::VRSettings()) {
		vr::VRSettings()->SetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_CenterMarkerOn_Bool, value);
		vr::VRSettings()->Sync();
	}
}

void ChaperoneTabController::PlaySpaceToggled(bool value) {
	if (vr::VRSettings()) {
		vr::VRSettings()->SetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_PlaySpaceOn_Bool, value);
		vr::VRSettings()->Sync();
	}
}

void ChaperoneTabController::ForceBoundsToggled(bool value) {
	if (vr::VRChaperone()) {
		forceBoundsFlag = value;
		vr::VRChaperone()->ForceBoundsVisible(value);
	}
}

void ChaperoneTabController::FadeDistanceChanged(int value) {
	if (vr::VRSettings()) {
		float fval= (float)value / 100;
		vr::VRSettings()->SetFloat(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_FadeDistance_Float, fval);
		vr::VRSettings()->Sync();
		widget->ui->FadeDistanceLabel->setText(QString::asprintf("%.2f", fval));
	}
}

void ChaperoneTabController::ColorAlphaChanged(int value) {
	if (vr::VRSettings()) {
		float fval = (float)value / 100;
		vr::VRSettings()->SetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaA_Int32, 255*fval);
		vr::VRSettings()->Sync();
		widget->ui->ColorAlphaLabel->setText(QString::asprintf("%i%%", value));
	}
}

void ChaperoneTabController::ResetDefaultsClicked() {
	widget->ui->CenterMarkerToggle->setChecked(false);
	widget->ui->PlaySpaceToggle->setChecked(false);
	widget->ui->ForceBoundsToggle->setChecked(false);
	widget->ui->FadeDistanceSlider->setValue(70);
	widget->ui->ColorAlphaSlider->setValue(100);
}

void MoveCenterTabController::init(OverlayController * parent, OverlayWidget * widget) {
	this->parent = parent;
	this->widget = widget;
	xOffset = 0.0f;
	yOffset = 0.0f;
	zOffset = 0.0f;
	connect(widget->ui->XMinusButton, SIGNAL(clicked()), this, SLOT(XMinusClicked()));
	connect(widget->ui->XPlusButton, SIGNAL(clicked()), this, SLOT(XPlusClicked()));
	connect(widget->ui->YMinusButton, SIGNAL(clicked()), this, SLOT(YMinusClicked()));
	connect(widget->ui->YPlusButton, SIGNAL(clicked()), this, SLOT(YPlusClicked()));
	connect(widget->ui->ZMinusButton, SIGNAL(clicked()), this, SLOT(ZMinusClicked()));
	connect(widget->ui->ZPlusButton, SIGNAL(clicked()), this, SLOT(ZPlusClicked()));
	connect(widget->ui->MoveCenterResetButton, SIGNAL(clicked()), this, SLOT(MoveCenterResetClicked()));
}

void MoveCenterTabController::UpdateTab() {
	if (widget) {
		widget->ui->XOffsetLabel->setText(QString::asprintf("%.1f", xOffset));
		widget->ui->YOffsetLabel->setText(QString::asprintf("%.1f", yOffset));
		widget->ui->ZOffsetLabel->setText(QString::asprintf("%.1f", zOffset));
	}
}

void MoveCenterTabController::XMinusClicked() {
	if (widget) {
		parent->AddOffsetToStandingPosition(0, -0.5f);
		xOffset -= 0.5f;
		widget->ui->XOffsetLabel->setText(QString::asprintf("%.1f", xOffset));
	}
}

void MoveCenterTabController::XPlusClicked() {
	if (widget) {
		parent->AddOffsetToStandingPosition(0, 0.5f);
		xOffset += 0.5f;
		widget->ui->XOffsetLabel->setText(QString::asprintf("%.1f", xOffset));
	}
}

void MoveCenterTabController::YMinusClicked() {
	if (widget) {
		parent->AddOffsetToStandingPosition(1, -0.5f);
		yOffset -= 0.5f;
		widget->ui->YOffsetLabel->setText(QString::asprintf("%.1f", yOffset));
	}
}

void MoveCenterTabController::YPlusClicked() {
	if (widget) {
		parent->AddOffsetToStandingPosition(1, 0.5f);
		yOffset += 0.5f;
		widget->ui->YOffsetLabel->setText(QString::asprintf("%.1f", yOffset));
	}
}

void MoveCenterTabController::ZMinusClicked() {
	if (widget) {
		parent->AddOffsetToStandingPosition(2, -0.5f);
		zOffset -= 0.5f;
		widget->ui->ZOffsetLabel->setText(QString::asprintf("%.1f", zOffset));
	}
}

void MoveCenterTabController::ZPlusClicked() {
	if (widget) {
		parent->AddOffsetToStandingPosition(2, 0.5f);
		zOffset += 0.5f;
		widget->ui->ZOffsetLabel->setText(QString::asprintf("%.1f", zOffset));
	}
}

void MoveCenterTabController::MoveCenterResetClicked() {
	if (widget) {
		parent->AddOffsetToStandingPosition(0, -xOffset);
		parent->AddOffsetToStandingPosition(1, -yOffset);
		parent->AddOffsetToStandingPosition(2, -zOffset);
		xOffset = 0.0f;
		yOffset = 0.0f;
		zOffset = 0.0f;
		widget->ui->XOffsetLabel->setText(QString::asprintf("%.1f", xOffset));
		widget->ui->YOffsetLabel->setText(QString::asprintf("%.1f", yOffset));
		widget->ui->ZOffsetLabel->setText(QString::asprintf("%.1f", zOffset));
	}
}

void FixFloorTabController::init(OverlayController * parent, OverlayWidget * widget) {
	this->parent = parent;
	this->widget = widget;
	connect(widget->ui->FixFloorButton, SIGNAL(clicked()), this, SLOT(FixFloorClicked()));
	connect(widget->ui->UndoFixFloorButton, SIGNAL(clicked()), this, SLOT(UndoFixFloorClicked()));
}

void FixFloorTabController::UpdateTab() {
	if (widget) {
		if (state == 1) {
			widget->ui->FixFloorButton->setEnabled(false);
		} else {
			widget->ui->FixFloorButton->setEnabled(true);
		}
		if (floorOffset != 0.0f) {
			widget->ui->UndoFixFloorButton->setEnabled(true);
		} else {
			widget->ui->UndoFixFloorButton->setEnabled(false);
		}
	}
}

void FixFloorTabController::eventLoopTick() {
	if (state == 1) {
		if (measurementCount == 0) {
			// Get Controller ids for left/right hand
			auto leftId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
			if (leftId == vr::k_unTrackedDeviceIndexInvalid) {
				widget->ui->FixFloorStatusLabel->setText("No left controller found.");
				statusMessageTimeout = 100;
				state = 2;
				UpdateTab();
				return;
			}
			auto rightId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
			if (rightId == vr::k_unTrackedDeviceIndexInvalid) {
				widget->ui->FixFloorStatusLabel->setText("No right controller found.");
				statusMessageTimeout = 100;
				state = 2;
				UpdateTab();
				return;
			}
			// Get poses
			vr::TrackedDevicePose_t devicePoses[vr::k_unMaxTrackedDeviceCount];
			vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0.0f, devicePoses, vr::k_unMaxTrackedDeviceCount);
			vr::TrackedDevicePose_t* leftPose = devicePoses + leftId;
			vr::TrackedDevicePose_t* rightPose = devicePoses + rightId;
			if (!leftPose->bPoseIsValid || !leftPose->bDeviceIsConnected || leftPose->eTrackingResult != vr::TrackingResult_Running_OK) {
				widget->ui->FixFloorStatusLabel->setText("Left controller tracking problems.");
				statusMessageTimeout = 100;
				state = 2;
				UpdateTab();
				return;
			} else if (!rightPose->bPoseIsValid || !rightPose->bDeviceIsConnected || rightPose->eTrackingResult != vr::TrackingResult_Running_OK) {
				widget->ui->FixFloorStatusLabel->setText("Right controller tracking problems.");
				statusMessageTimeout = 100;
				state = 2;
				UpdateTab();
				return;
			} else {
				// The controller with the lowest y-pos is the floor fix reference
				if (leftPose->mDeviceToAbsoluteTracking.m[1][3] < rightPose->mDeviceToAbsoluteTracking.m[1][3]) {
					referenceController = leftId;
				} else {
					referenceController = rightId;
				}
				tempOffset = (double)devicePoses[referenceController].mDeviceToAbsoluteTracking.m[1][3];
				measurementCount = 1;
			}
			
		} else if (measurementCount >= 50) {
			floorOffset = -controllerUpOffsetCorrection + (float)(tempOffset / (double)measurementCount);
			LOG(INFO) << "Fix Floor: Floor Offset = " << floorOffset;
			parent->AddOffsetToStandingPosition(1, floorOffset);
			widget->ui->FixFloorStatusLabel->setText("Fixing ... OK");
			statusMessageTimeout = 50;
			state = 2;
			UpdateTab();
		} else {
			vr::TrackedDevicePose_t devicePoses[vr::k_unMaxTrackedDeviceCount];
			vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0.0f, devicePoses, vr::k_unMaxTrackedDeviceCount);
			tempOffset += (double)devicePoses[referenceController].mDeviceToAbsoluteTracking.m[1][3];
			measurementCount++;
		}
	} else if (state == 2) {
		if (statusMessageTimeout == 0) {
			if (widget) {
				widget->ui->FixFloorStatusLabel->setText("");
			}
			state = 0;
			UpdateTab();
		} else {
			statusMessageTimeout--;
		} 
	}
}

void FixFloorTabController::FixFloorClicked() {
	if (widget) {
		widget->ui->FixFloorStatusLabel->setText("Fixing ...");
		widget->ui->FixFloorButton->setEnabled(false);
		widget->ui->UndoFixFloorButton->setEnabled(false);
		measurementCount = 0;
		state = 1;
	}
}

void FixFloorTabController::UndoFixFloorClicked() {
	parent->AddOffsetToStandingPosition(1, -floorOffset);
	LOG(INFO) << "Fix Floor: Undo Floor Offset = " << -floorOffset;
	floorOffset = 0.0f;
	widget->ui->FixFloorStatusLabel->setText("Undo ... OK");
	statusMessageTimeout = 100;
	state = 2;
	UpdateTab();
}



void SettingsTabController::init(OverlayController * parent, OverlayWidget * widget) {
	this->parent = parent;
	this->widget = widget;
	connect(widget->ui->AutoStartToggle, SIGNAL(toggled(bool)), this, SLOT(AutoStartToggled(bool)));
}

void SettingsTabController::UpdateTab() {
	if (widget) {
		widget->ui->AutoStartToggle->blockSignals(true);
		if (vr::VRApplications()->GetApplicationAutoLaunch(OverlayController::applicationKey)) {
			widget->ui->AutoStartToggle->setChecked(true);
		} else {
			widget->ui->AutoStartToggle->setChecked(false);
		}
		widget->ui->AutoStartToggle->blockSignals(false);
	}
}

void SettingsTabController::AutoStartToggled(bool value) {
	LOG(INFO) << "Setting auto start: " << value;
	auto apperror = vr::VRApplications()->SetApplicationAutoLaunch(OverlayController::applicationKey, value);
	if (apperror != vr::VRApplicationError_None) {
		LOG(ERROR) << "Could not set auto start: " << vr::VRApplications()->GetApplicationsErrorNameFromEnum(apperror);
	}
}


} // namespace advconfig
