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
#include <easylogging++.h>
#include "utils/Matrix.h"
#include "openvr/overlay_utils.h"
#include "keyboard_input/input_sender.h"

// application namespace
namespace advsettings
{
QSettings* OverlayController::_appSettings = nullptr;

OverlayController::OverlayController( bool desktopMode,
                                      bool noSound,
                                      QQmlEngine& qmlEngine )
    : QObject(), m_desktopMode( desktopMode ), m_noSound( noSound ), m_actions()
{
    // Arbitrarily chosen Max Length of Directory path, should be sufficient for
    // Any set-up
    const uint32_t maxLength = 16192;
    uint32_t requiredLength;

    char tempRuntimePath[maxLength];
    bool pathIsGood
        = vr::VR_GetRuntimePath( tempRuntimePath, maxLength, &requiredLength );

    // Throw Error If over 16k characters in path string
    if ( !pathIsGood )
    {
        LOG( ERROR ) << "Error Finding VR Runtime Path, Attempting Recovery: ";
        uint32_t maxLengthRe = requiredLength;
        LOG( INFO ) << "Open VR reporting Required path length of: "
                    << maxLengthRe;
    }

    m_runtimePathUrl = QUrl::fromLocalFile( tempRuntimePath );
    LOG( INFO ) << "VR Runtime Path: " << m_runtimePathUrl.toLocalFile();

    QString activationSoundFile = m_runtimePathUrl.toLocalFile().append(
        "/content/panorama/sounds/activation.wav" );
    QFileInfo activationSoundFileInfo( activationSoundFile );
    if ( activationSoundFileInfo.exists() && activationSoundFileInfo.isFile() )
    {
        m_activationSoundEffect.setSource(
            QUrl::fromLocalFile( activationSoundFile ) );
        m_activationSoundEffect.setVolume( 1.0 );
    }
    else
    {
        LOG( ERROR ) << "Could not find activation sound file "
                     << activationSoundFile;
    }

    QString focusChangedSoundFile = m_runtimePathUrl.toLocalFile().append(
        "/content/panorama/sounds/focus_change.wav" );
    QFileInfo focusChangedSoundFileInfo( focusChangedSoundFile );
    if ( focusChangedSoundFileInfo.exists()
         && focusChangedSoundFileInfo.isFile() )
    {
        m_focusChangedSoundEffect.setSource(
            QUrl::fromLocalFile( focusChangedSoundFile ) );
        m_focusChangedSoundEffect.setVolume( 1.0 );
    }
    else
    {
        LOG( ERROR ) << "Could not find focus changed sound file "
                     << focusChangedSoundFile;
    }

    constexpr auto alarmFileName = "res/sounds/alarm01.wav";
    const auto alarm01SoundFile
        = paths::binaryDirectoryFindFile( alarmFileName );

    if ( alarm01SoundFile.has_value() )
    {
        m_alarm01SoundEffect.setSource( QUrl::fromLocalFile(
            QString::fromStdString( ( *alarm01SoundFile ) ) ) );
        m_alarm01SoundEffect.setVolume( 1.0 );
    }
    else
    {
        LOG( ERROR ) << "Could not find alarm01 sound file " << alarmFileName;
    }

    QSurfaceFormat format;
    // Qt's QOpenGLPaintDevice is not compatible with OpenGL versions >= 3.0
    // NVIDIA does not care, but unfortunately AMD does
    // Are subtle changes to the semantics of OpenGL functions actually covered
    // by the compatibility profile, and this is an AMD bug?
    format.setVersion( 2, 1 );
    // format.setProfile( QSurfaceFormat::CompatibilityProfile );
    format.setDepthBufferSize( 16 );
    format.setStencilBufferSize( 8 );
    format.setSamples( 16 );

    m_pOpenGLContext.reset( new QOpenGLContext() );
    m_pOpenGLContext->setFormat( format );
    if ( !m_pOpenGLContext->create() )
    {
        throw std::runtime_error( "Could not create OpenGL context" );
    }

    // create an offscreen surface to attach the context and FBO to
    m_pOffscreenSurface.reset( new QOffscreenSurface() );
    m_pOffscreenSurface->setFormat( m_pOpenGLContext->format() );
    m_pOffscreenSurface->create();
    m_pOpenGLContext->makeCurrent( m_pOffscreenSurface.get() );

    if ( !vr::VROverlay() )
    {
        QMessageBox::critical(
            nullptr, "OpenVR Advanced Settings Overlay", "Is OpenVR running?" );
        throw std::runtime_error( std::string( "No Overlay interface" ) );
    }

    // Init controllers
    m_steamVRTabController.initStage1();
    m_chaperoneTabController.initStage1();
    m_moveCenterTabController.initStage1();
    m_audioTabController.initStage1();
    m_settingsTabController.initStage1();
    m_utilitiesTabController.initStage1();
    m_videoTabController.initStage1();

    // init action handles

    m_chaperoneTabController.setLeftHapticActionHandle(
        m_actions.leftHapticActionHandle() );
    m_chaperoneTabController.setRightHapticActionHandle(
        m_actions.rightHapticActionHandle() );
    m_chaperoneTabController.setLeftInputHandle( m_actions.leftInputHandle() );
    m_chaperoneTabController.setRightInputHandle(
        m_actions.rightInputHandle() );

    // Set qml context
    qmlEngine.rootContext()->setContextProperty( "applicationVersion",
                                                 getVersionString() );
    qmlEngine.rootContext()->setContextProperty( "vrRuntimePath",
                                                 getVRRuntimePathUrl() );

    // Pretty disgusting trick to allow qmlRegisterSingletonType to continue
    // working with the lambdas that were already there. The callback function
    // in qmlRegisterSingletonType won't work with any lambdas that capture the
    // environment. The alternative to making a static pointer to this was
    // rewriting all QML to not be singletons, which should probably be done
    // whenever possible.
    static OverlayController* const objectAddress = this;
    constexpr auto qmlSingletonImportName = "ovras.advsettings";
    qmlRegisterSingletonType<OverlayController>(
        qmlSingletonImportName,
        1,
        0,
        "OverlayController",
        []( QQmlEngine*, QJSEngine* ) {
            QObject* obj = objectAddress;
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    // It is unknown if it it intended for the generic in
    // qmlRegisterSingletonType to be <SteamVRTabController> in all the
    // remaining function calls, or if it's just a copy paste accident that
    // happens to work.
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "SteamVRTabController",
        []( QQmlEngine*, QJSEngine* ) {
            QObject* obj = &( objectAddress->m_steamVRTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "ChaperoneTabController",
        []( QQmlEngine*, QJSEngine* ) {
            QObject* obj = &( objectAddress->m_chaperoneTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "MoveCenterTabController",
        []( QQmlEngine*, QJSEngine* ) {
            QObject* obj = &( objectAddress->m_moveCenterTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "FixFloorTabController",
        []( QQmlEngine*, QJSEngine* ) {
            QObject* obj = &( objectAddress->m_fixFloorTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "AudioTabController",
        []( QQmlEngine*, QJSEngine* ) {
            QObject* obj = &( objectAddress->m_audioTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "StatisticsTabController",
        []( QQmlEngine*, QJSEngine* ) {
            QObject* obj = &( objectAddress->m_statisticsTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "SettingsTabController",
        []( QQmlEngine*, QJSEngine* ) {
            QObject* obj = &( objectAddress->m_settingsTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "UtilitiesTabController",
        []( QQmlEngine*, QJSEngine* ) {
            QObject* obj = &( objectAddress->m_utilitiesTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "VideoTabController",
        []( QQmlEngine*, QJSEngine* ) {
            QObject* obj = &( objectAddress->m_videoTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<overlay::DesktopOverlay>(
        qmlSingletonImportName,
        1,
        0,
        "DesktopOverlay",
        []( QQmlEngine*, QJSEngine* ) {
            QObject* obj = &( objectAddress->m_desktopOverlay );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );

    // Force keyboard shortcut settings to appear in config file.
    constexpr auto settingsKeyboardName = "keyboardShortcuts";
    appSettings()->beginGroup( settingsKeyboardName );

    constexpr auto defaultDiscordMuteBindings = "^>m";
    if ( appSettings()->value( "keyboardOne", "" ) == "" )
    {
        appSettings()->setValue( "keyboardOne", defaultDiscordMuteBindings );
    }

    if ( appSettings()->value( "keyboardTwo", "" ) == "" )
    {
        appSettings()->setValue( "keyboardTwo", defaultDiscordMuteBindings );
    }

    if ( appSettings()->value( "keyboardThree", "" ) == "" )
    {
        appSettings()->setValue( "keyboardThree", defaultDiscordMuteBindings );
    }
    appSettings()->endGroup();

    // Keep the settings for vsyncDisabled here in main overlaycontroller
    appSettings()->beginGroup( "applicationSettings" );
    auto value = appSettings()->value( "vsyncDisabled", m_vsyncDisabled );
    if ( value.isValid() && !value.isNull() )
    {
        m_vsyncDisabled = value.toBool();
    }
    value = appSettings()->value( "customTickRateMs", m_customTickRateMs );
    if ( value.isValid() && !value.isNull() )
    {
        // keep sane tickrate (between 1~k_maxCustomTickRate)
        if ( value.toInt() < 1 )
        {
            m_customTickRateMs = 1;
        }
        else if ( value.toInt() > k_maxCustomTickRate )
        {
            m_customTickRateMs = k_maxCustomTickRate;
        }
        else
        {
            m_customTickRateMs = value.toInt();
        }
    }
    appSettings()->endGroup();
}

OverlayController::~OverlayController()
{
    Shutdown();
}

void OverlayController::Shutdown()
{
    if ( m_pPumpEventsTimer )
    {
        disconnect( m_pPumpEventsTimer.get(),
                    SIGNAL( timeout() ),
                    this,
                    SLOT( OnTimeoutPumpEvents() ) );
        m_pPumpEventsTimer->stop();
        m_pPumpEventsTimer.reset();
    }
    if ( m_pRenderTimer )
    {
        disconnect( m_pRenderControl.get(),
                    SIGNAL( renderRequested() ),
                    this,
                    SLOT( OnRenderRequest() ) );
        disconnect( m_pRenderControl.get(),
                    SIGNAL( sceneChanged() ),
                    this,
                    SLOT( OnRenderRequest() ) );
        disconnect( m_pRenderTimer.get(),
                    SIGNAL( timeout() ),
                    this,
                    SLOT( renderOverlay() ) );
        m_pRenderTimer->stop();
        m_pRenderTimer.reset();
    }
    m_pWindow.reset();
    m_pRenderControl.reset();
    m_pFbo.reset();
    m_pOpenGLContext.reset();
    m_pOffscreenSurface.reset();
}

void OverlayController::SetWidget( QQuickItem* quickItem,
                                   const std::string& name,
                                   const std::string& key )
{
    if ( !m_desktopMode )
    {
        vr::VROverlayError overlayError
            = vr::VROverlay()->CreateDashboardOverlay(
                key.c_str(),
                name.c_str(),
                &m_ulOverlayHandle,
                &m_ulOverlayThumbnailHandle );
        if ( overlayError != vr::VROverlayError_None )
        {
            if ( overlayError == vr::VROverlayError_KeyInUse )
            {
                QMessageBox::critical( nullptr,
                                       "OpenVR Advanced Settings Overlay",
                                       "Another instance is already running." );
            }
            throw std::runtime_error( std::string(
                "Failed to create Overlay: "
                + std::string( vr::VROverlay()->GetOverlayErrorNameFromEnum(
                    overlayError ) ) ) );
        }
        vr::VROverlay()->SetOverlayWidthInMeters( m_ulOverlayHandle, 2.5f );
        vr::VROverlay()->SetOverlayInputMethod(
            m_ulOverlayHandle, vr::VROverlayInputMethod_Mouse );
        vr::VROverlay()->SetOverlayFlag(
            m_ulOverlayHandle,
            vr::VROverlayFlags_SendVRSmoothScrollEvents,
            true );

        constexpr auto thumbiconFilename = "res/img/icons/thumbicon.png";
        const auto thumbIconPath
            = paths::binaryDirectoryFindFile( thumbiconFilename );
        if ( thumbIconPath.has_value() )
        {
            vr::VROverlay()->SetOverlayFromFile( m_ulOverlayThumbnailHandle,
                                                 thumbIconPath->c_str() );
        }
        else
        {
            LOG( ERROR ) << "Could not find thumbnail icon \""
                         << thumbiconFilename << "\"";
        }

        // Too many render calls in too short time overwhelm Qt and an assertion
        // gets thrown. Therefore we use an timer to delay render calls
        m_pRenderTimer.reset( new QTimer() );
        m_pRenderTimer->setSingleShot( true );
        m_pRenderTimer->setInterval( 5 );
        connect( m_pRenderTimer.get(),
                 SIGNAL( timeout() ),
                 this,
                 SLOT( renderOverlay() ) );

        QOpenGLFramebufferObjectFormat fboFormat;
        fboFormat.setAttachment(
            QOpenGLFramebufferObject::CombinedDepthStencil );
        fboFormat.setTextureTarget( GL_TEXTURE_2D );
        m_pFbo.reset( new QOpenGLFramebufferObject(
            static_cast<int>( quickItem->width() ),
            static_cast<int>( quickItem->height() ),
            fboFormat ) );

        m_pRenderControl.reset( new QQuickRenderControl() );
        m_pWindow.reset( new QQuickWindow( m_pRenderControl.get() ) );
        m_pWindow->setRenderTarget( m_pFbo.get() );
        quickItem->setParentItem( m_pWindow->contentItem() );
        m_pWindow->setGeometry( 0,
                                0,
                                static_cast<int>( quickItem->width() ),
                                static_cast<int>( quickItem->height() ) );
        m_pRenderControl->initialize( m_pOpenGLContext.get() );

        vr::HmdVector2_t vecWindowSize
            = { static_cast<float>( quickItem->width() ),
                static_cast<float>( quickItem->height() ) };
        vr::VROverlay()->SetOverlayMouseScale( m_ulOverlayHandle,
                                               &vecWindowSize );

        connect( m_pRenderControl.get(),
                 SIGNAL( renderRequested() ),
                 this,
                 SLOT( OnRenderRequest() ) );
        connect( m_pRenderControl.get(),
                 SIGNAL( sceneChanged() ),
                 this,
                 SLOT( OnRenderRequest() ) );
    }

    m_pPumpEventsTimer.reset( new QTimer() );
    connect( m_pPumpEventsTimer.get(),
             SIGNAL( timeout() ),
             this,
             SLOT( OnTimeoutPumpEvents() ) );

    // Every 1ms we check if the current frame has advanced (for vysnc)
    m_pPumpEventsTimer->setInterval( 1 );

    m_pPumpEventsTimer->start();

    m_steamVRTabController.initStage2( this );
    m_chaperoneTabController.initStage2( this );
    m_fixFloorTabController.initStage2( this );
    m_audioTabController.initStage2();
    m_statisticsTabController.initStage2( this );
    m_settingsTabController.initStage2( this );
    m_utilitiesTabController.initStage2( this );
    m_moveCenterTabController.initStage2( this );
}

void OverlayController::OnRenderRequest()
{
    if ( m_pRenderTimer && !m_pRenderTimer->isActive() )
    {
        m_pRenderTimer->start();
    }
}

void OverlayController::renderOverlay()
{
    if ( !m_desktopMode )
    {
        // skip rendering if the overlay isn't visible
        if ( !vr::VROverlay()
             || ( !vr::VROverlay()->IsOverlayVisible( m_ulOverlayHandle )
                  && !vr::VROverlay()->IsOverlayVisible(
                      m_ulOverlayThumbnailHandle ) ) )
            return;
        m_pRenderControl->polishItems();
        m_pRenderControl->sync();
        m_pRenderControl->render();

        GLuint unTexture = m_pFbo->texture();
        if ( unTexture != 0 )
        {
#if defined _WIN64 || defined _LP64
            // To avoid any compiler warning because of cast to a larger pointer
            // type (warning C4312 on VC)
            vr::Texture_t texture = { reinterpret_cast<void*>(
                                          static_cast<uint64_t>( unTexture ) ),
                                      vr::TextureType_OpenGL,
                                      vr::ColorSpace_Auto };
#else
            vr::Texture_t texture = { reinterpret_cast<void*>( unTexture ),
                                      vr::TextureType_OpenGL,
                                      vr::ColorSpace_Auto };
#endif
            vr::VROverlay()->SetOverlayTexture( m_ulOverlayHandle, &texture );
        }
        m_pOpenGLContext->functions()->glFlush(); // We need to flush otherwise
                                                  // the texture may be empty.*/
    }
}

bool OverlayController::pollNextEvent( vr::VROverlayHandle_t ulOverlayHandle,
                                       vr::VREvent_t* pEvent )
{
    if ( isDesktopMode() )
    {
        return vr::VRSystem()->PollNextEvent( pEvent, sizeof( vr::VREvent_t ) );
    }
    else
    {
        return vr::VROverlay()->PollNextOverlayEvent(
            ulOverlayHandle, pEvent, sizeof( vr::VREvent_t ) );
    }
}

QPoint OverlayController::getMousePositionForEvent( vr::VREvent_Mouse_t mouse )
{
    float y = mouse.y;
#ifdef __linux__
    float h = static_cast<float>( m_pWindow->height() );
    y = h - y;
#endif
    return QPoint( static_cast<int>( mouse.x ), static_cast<int>( y ) );
}

void OverlayController::processMediaKeyBindings()
{
    if ( m_actions.nextSong() )
    {
        m_utilitiesTabController.sendMediaNextSong();
    }
    if ( m_actions.previousSong() )
    {
        m_utilitiesTabController.sendMediaPreviousSong();
    }
    if ( m_actions.pausePlaySong() )
    {
        m_utilitiesTabController.sendMediaPausePlay();
    }
    if ( m_actions.stopSong() )
    {
        m_utilitiesTabController.sendMediaStopSong();
    }
}

void OverlayController::processMotionBindings()
{
    // Execution order for moveCenterTabController actions is important. Don't
    // reorder these. Override actions must always come after normal because
    // active priority is set based on which action is "newest"
    // normal actions:
    m_moveCenterTabController.leftHandSpaceDrag(
        m_actions.leftHandSpaceDrag() );
    m_moveCenterTabController.rightHandSpaceDrag(
        m_actions.rightHandSpaceDrag() );
    m_moveCenterTabController.leftHandSpaceTurn(
        m_actions.leftHandSpaceTurn() );
    m_moveCenterTabController.rightHandSpaceTurn(
        m_actions.rightHandSpaceTurn() );
    m_moveCenterTabController.gravityToggleAction( m_actions.gravityToggle() );
    m_moveCenterTabController.gravityReverseAction(
        m_actions.gravityReverse() );
    m_moveCenterTabController.heightToggleAction( m_actions.heightToggle() );
    m_moveCenterTabController.resetOffsets( m_actions.resetOffsets() );
    m_moveCenterTabController.snapTurnLeft( m_actions.snapTurnLeft() );
    m_moveCenterTabController.snapTurnRight( m_actions.snapTurnRight() );
    m_moveCenterTabController.smoothTurnLeft( m_actions.smoothTurnLeft() );
    m_moveCenterTabController.smoothTurnRight( m_actions.smoothTurnRight() );
    m_moveCenterTabController.xAxisLockToggle( m_actions.xAxisLockToggle() );
    m_moveCenterTabController.yAxisLockToggle( m_actions.yAxisLockToggle() );
    m_moveCenterTabController.zAxisLockToggle( m_actions.zAxisLockToggle() );

    // override actions:
    m_moveCenterTabController.optionalOverrideLeftHandSpaceDrag(
        m_actions.optionalOverrideLeftHandSpaceDrag() );
    m_moveCenterTabController.optionalOverrideRightHandSpaceDrag(
        m_actions.optionalOverrideRightHandSpaceDrag() );
    m_moveCenterTabController.optionalOverrideLeftHandSpaceTurn(
        m_actions.optionalOverrideLeftHandSpaceTurn() );
    m_moveCenterTabController.optionalOverrideRightHandSpaceTurn(
        m_actions.optionalOverrideRightHandSpaceTurn() );
    m_moveCenterTabController.swapSpaceDragToLeftHandOverride(
        m_actions.swapSpaceDragToLeftHandOverride() );
    m_moveCenterTabController.swapSpaceDragToRightHandOverride(
        m_actions.swapSpaceDragToRightHandOverride() );
}

void OverlayController::processChaperoneBindings()
{
    if ( m_actions.chaperoneToggle() )
    {
        m_chaperoneTabController.setDisableChaperone(
            !( m_chaperoneTabController.disableChaperone() ), true );
    }
    m_chaperoneTabController.setProxState( m_actions.proxState() );
    m_chaperoneTabController.addLeftHapticClick(
        m_actions.addLeftHapticClick() );
    m_chaperoneTabController.addRightHapticClick(
        m_actions.addRightHapticClick() );
}

void OverlayController::processPushToTalkBindings()
{
    const auto pushToTalkCannotChange = !m_audioTabController.pttChangeValid();
    const auto pushToTalkEnabled = m_audioTabController.pttEnabled();

    const auto proxSensorActivated = m_actions.proxState();
    const auto useProxSensor = m_audioTabController.micProximitySensorCanMute();

    if ( useProxSensor )
    {
        if ( !proxSensorActivated )
        {
            m_audioTabController.setMicMuted( true );
            return;
        }
        // strictly speaking this is not the most elegant solution, but should
        // work well enough.
        else if ( !pushToTalkEnabled )
        {
            m_audioTabController.setMicMuted( false );
        }
    }

    if ( pushToTalkCannotChange || !pushToTalkEnabled )
    {
        return;
    }

    const auto pushToTalkButtonActivated = m_actions.pushToTalk();
    const auto pushToTalkCurrentlyActive = m_audioTabController.pttActive();

    if ( pushToTalkButtonActivated && !pushToTalkCurrentlyActive )
    {
        m_audioTabController.startPtt();
    }
    else if ( !pushToTalkButtonActivated && pushToTalkCurrentlyActive )
    {
        m_audioTabController.stopPtt();
    }
}

void OverlayController::processKeyboardBindings()
{
    constexpr auto settingsKeyboardName = "keyboardShortcuts";

    if ( m_actions.keyboardOne() )
    {
        appSettings()->beginGroup( settingsKeyboardName );
        const auto commands
            = appSettings()->value( "keyboardOne" ).toString().toStdString();
        appSettings()->endGroup();

        sendStringAsInput( commands );
    }

    if ( m_actions.keyboardTwo() )
    {
        appSettings()->beginGroup( settingsKeyboardName );
        const auto commands
            = appSettings()->value( "keyboardTwo" ).toString().toStdString();
        appSettings()->endGroup();

        sendStringAsInput( commands );
    }

    if ( m_actions.keyboardThree() )
    {
        appSettings()->beginGroup( settingsKeyboardName );
        const auto commands
            = appSettings()->value( "keyboardThree" ).toString().toStdString();
        appSettings()->endGroup();

        sendStringAsInput( commands );
    }
}
/*!
Checks if an action has been activated and dispatches the related action if it
has been.
*/
void OverlayController::processInputBindings()
{
    processMediaKeyBindings();

    processMotionBindings();

    processPushToTalkBindings();

    processChaperoneBindings();

    processKeyboardBindings();
}

bool OverlayController::vsyncDisabled() const
{
    return m_vsyncDisabled;
}

void OverlayController::setVsyncDisabled( bool value, bool notify )
{
    if ( m_vsyncDisabled == value )
    {
        return;
    }
    m_vsyncDisabled = value;
    appSettings()->beginGroup( "applicationSettings" );
    appSettings()->setValue( "vsyncDisabled", m_vsyncDisabled );
    appSettings()->endGroup();
    appSettings()->sync();
    if ( notify )
    {
        emit vsyncDisabledChanged( m_vsyncDisabled );
    }
}

int OverlayController::customTickRateMs() const
{
    return m_customTickRateMs;
}

void OverlayController::setCustomTickRateMs( int value, bool notify )
{
    if ( m_customTickRateMs == value )
    {
        return;
    }
    // keep m_customTickRateMs sane (between 1~k_maxCustomTickRate)
    if ( value < 1 )
    {
        m_customTickRateMs = 1;
    }
    else if ( value > k_maxCustomTickRate )
    {
        m_customTickRateMs = k_maxCustomTickRate;
    }
    else
    {
        m_customTickRateMs = value;
    }

    appSettings()->beginGroup( "applicationSettings" );
    appSettings()->setValue( "customTickRateMs", m_customTickRateMs );
    appSettings()->endGroup();
    appSettings()->sync();
    if ( notify )
    {
        emit customTickRateMsChanged( m_customTickRateMs );
    }
}

// vsync implementation:
// this function triggers every 1ms
// this function should remain lightweight and only check if it's time to run
// mainEventLoop() or not.
void OverlayController::OnTimeoutPumpEvents()
{
    if ( m_vsyncDisabled )
    {
        // check if it's time for a custom tick rate tick
        if ( m_customTickRateCounter > m_customTickRateMs )
        {
            mainEventLoop();
            m_customTickRateCounter = 0;
        }
        else
        {
            m_customTickRateCounter++;
        }
    }

    // vsync is enabled
    else
    {
        // get the current frame number from the VRSystem frame counter
        vr::VRSystem()->GetTimeSinceLastVsync( nullptr, &m_currentFrame );

        // Check if we are in the next frame yet
        if ( m_currentFrame > m_lastFrame )
        {
            // If the frame has advanced since last check, it's time for our
            // main event loop. (this function should trigger about every 11ms
            // assuming 90fps compositor)
            mainEventLoop();

            // wait for the next frame after executing our main event loop once.
            m_lastFrame = m_currentFrame;
            m_vsyncTooLateCounter = 0;
        }
        else if ( m_vsyncTooLateCounter >= k_nonVsyncTickRate )
        {
            mainEventLoop();
            // m_lastFrame = m_currentFrame + 1 skips the next vsync frame in
            // case it was just about to trigger, to prevent double updates
            // faster than 11ms.
            m_lastFrame = m_currentFrame + 1;
            m_vsyncTooLateCounter = 0;
        }
        else
        {
            m_vsyncTooLateCounter++;
        }
    }
}

void OverlayController::mainEventLoop()
{
    if ( !vr::VRSystem() )
        return;

    m_actions.UpdateStates();

    processInputBindings();

    vr::VREvent_t vrEvent;
    bool chaperoneDataAlreadyUpdated = false;
    while ( pollNextEvent( m_ulOverlayHandle, &vrEvent ) )
    {
        switch ( vrEvent.eventType )
        {
        case vr::VREvent_MouseMove:
        {
            QPoint ptNewMouse = getMousePositionForEvent( vrEvent.data.mouse );
            if ( ptNewMouse != m_ptLastMouse )
            {
                QMouseEvent mouseEvent( QEvent::MouseMove,
                                        ptNewMouse,
                                        m_pWindow->mapToGlobal( ptNewMouse ),
                                        Qt::NoButton,
                                        m_lastMouseButtons,
                                        nullptr );
                m_ptLastMouse = ptNewMouse;
                QCoreApplication::sendEvent( m_pWindow.get(), &mouseEvent );
                OnRenderRequest();
            }
        }
        break;

        case vr::VREvent_MouseButtonDown:
        {
            QPoint ptNewMouse = getMousePositionForEvent( vrEvent.data.mouse );
            Qt::MouseButton button
                = vrEvent.data.mouse.button == vr::VRMouseButton_Right
                      ? Qt::RightButton
                      : Qt::LeftButton;
            m_lastMouseButtons |= button;
            QMouseEvent mouseEvent( QEvent::MouseButtonPress,
                                    ptNewMouse,
                                    m_pWindow->mapToGlobal( ptNewMouse ),
                                    button,
                                    m_lastMouseButtons,
                                    nullptr );
            QCoreApplication::sendEvent( m_pWindow.get(), &mouseEvent );
        }
        break;

        case vr::VREvent_MouseButtonUp:
        {
            QPoint ptNewMouse = getMousePositionForEvent( vrEvent.data.mouse );
            Qt::MouseButton button
                = vrEvent.data.mouse.button == vr::VRMouseButton_Right
                      ? Qt::RightButton
                      : Qt::LeftButton;
            m_lastMouseButtons &= ~button;
            QMouseEvent mouseEvent( QEvent::MouseButtonRelease,
                                    ptNewMouse,
                                    m_pWindow->mapToGlobal( ptNewMouse ),
                                    button,
                                    m_lastMouseButtons,
                                    nullptr );
            QCoreApplication::sendEvent( m_pWindow.get(), &mouseEvent );
        }
        break;

        case vr::VREvent_ScrollSmooth:
        {
            // Wheel speed is defined as 1/8 of a degree
            QWheelEvent wheelEvent(
                m_ptLastMouse,
                m_pWindow->mapToGlobal( m_ptLastMouse ),
                QPoint(),
                QPoint( static_cast<int>( vrEvent.data.scroll.xdelta
                                          * ( 360.0f * 8.0f ) ),
                        static_cast<int>( vrEvent.data.scroll.ydelta
                                          * ( 360.0f * 8.0f ) ) ),
                0,
                Qt::Vertical,
                m_lastMouseButtons,
                nullptr );
            QCoreApplication::sendEvent( m_pWindow.get(), &wheelEvent );
        }
        break;

        case vr::VREvent_OverlayShown:
        {
            m_pWindow->update();
        }
        break;

        case vr::VREvent_Quit:
        {
            LOG( INFO ) << "Received quit request.";
            vr::VRSystem()->AcknowledgeQuit_Exiting(); // Let us buy some
                                                       // time just in case
            m_moveCenterTabController.shutdown();
            // Un-mute mic before Exiting VR, as it is set at system level Not
            // Vr level.
            // m_audioTabController.setMicMuted( false, false );
            m_audioTabController.shutdown();
            m_chaperoneTabController.shutdown();
            Shutdown();
            QApplication::exit();

            LOG( INFO ) << "All systems exited.";
            exit( EXIT_SUCCESS );
            // Does not fallthrough
        }

        case vr::VREvent_DashboardActivated:
        {
            LOG( DEBUG ) << "Dashboard activated";
            m_desktopOverlay.update();
            m_dashboardVisible = true;
        }
        break;

        case vr::VREvent_DashboardDeactivated:
        {
            LOG( DEBUG ) << "Dashboard deactivated";
            m_dashboardVisible = false;
        }
        break;

        case vr::VREvent_KeyboardDone:
        {
            char keyboardBuffer[1024];
            vr::VROverlay()->GetKeyboardText( keyboardBuffer, 1024 );
            emit keyBoardInputSignal( QString( keyboardBuffer ),
                                      static_cast<unsigned long>(
                                          vrEvent.data.keyboard.uUserValue ) );
        }
        break;

        case vr::VREvent_SeatedZeroPoseReset:
        {
            m_moveCenterTabController.incomingSeatedReset();
            LOG( INFO ) << "Game Triggered Seated Zero-Position Reset";
        }
        break;

        // Multiple ChaperoneUniverseHasChanged are often emitted at the
        // same time (some with a little bit of delay) There is no sure way
        // to recognize redundant events, we can only exclude redundant
        // events during the same call of OnTimeoutPumpEvents() INFO Removed
        // logging on play space mover for possible crashing issues.
        case vr::VREvent_ChaperoneUniverseHasChanged:
        case vr::VREvent_ChaperoneDataHasChanged:
        {
            if ( !chaperoneDataAlreadyUpdated )
            {
                // LOG(INFO) << "Re-loading chaperone data ...";
                m_chaperoneUtils.loadChaperoneData();
                // LOG(INFO) << "Found " << m_chaperoneUtils.quadsCount() <<
                // " chaperone quads."; if
                // (m_chaperoneUtils.isChaperoneWellFormed()) { LOG(INFO) <<
                // "Chaperone data seems to be well-formed.";
                //} else {
                // LOG(INFO) << "Chaperone data is NOT well-formed.";
                //}
                chaperoneDataAlreadyUpdated = true;
            }
        }
        break;
        }
    }

    vr::TrackedDevicePose_t devicePoses[vr::k_unMaxTrackedDeviceCount];
    vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(
        vr::TrackingUniverseStanding,
        0.0f,
        devicePoses,
        vr::k_unMaxTrackedDeviceCount );

    // HMD/Controller Velocities
    auto leftId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
        vr::TrackedControllerRole_LeftHand );
    float leftSpeed = 0.0f;
    if ( leftId != vr::k_unTrackedDeviceIndexInvalid
         && devicePoses[leftId].bPoseIsValid
         && devicePoses[leftId].eTrackingResult
                == vr::TrackingResult_Running_OK )
    {
        auto& vel = devicePoses[leftId].vVelocity.v;
        leftSpeed
            = std::sqrt( vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2] );
    }
    auto rightId = vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(
        vr::TrackedControllerRole_RightHand );
    auto rightSpeed = 0.0f;
    if ( rightId != vr::k_unTrackedDeviceIndexInvalid
         && devicePoses[rightId].bPoseIsValid
         && devicePoses[rightId].eTrackingResult
                == vr::TrackingResult_Running_OK )
    {
        auto& vel = devicePoses[rightId].vVelocity.v;
        rightSpeed
            = std::sqrt( vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2] );
    }
    auto hmdSpeed = 0.0f;
    if ( devicePoses[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid
         && devicePoses[vr::k_unTrackedDeviceIndex_Hmd].eTrackingResult
                == vr::TrackingResult_Running_OK )
    {
        auto& vel = devicePoses[vr::k_unTrackedDeviceIndex_Hmd].vVelocity.v;
        hmdSpeed
            = std::sqrt( vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2] );
    }
    m_moveCenterTabController.eventLoopTick(
        vr::VRCompositor()->GetTrackingSpace(), devicePoses );
    m_utilitiesTabController.eventLoopTick();
    m_statisticsTabController.eventLoopTick(
        devicePoses, leftSpeed, rightSpeed );
    m_chaperoneTabController.eventLoopTick(
        devicePoses, leftSpeed, rightSpeed, hmdSpeed );
    m_audioTabController.eventLoopTick();

    if ( vr::VROverlay()->IsDashboardVisible() )
    {
        m_settingsTabController.dashboardLoopTick();
        m_steamVRTabController.dashboardLoopTick();
        m_fixFloorTabController.dashboardLoopTick( devicePoses );
        m_videoTabController.dashboardLoopTick();
    }

    if ( m_ulOverlayThumbnailHandle != vr::k_ulOverlayHandleInvalid )
    {
        while ( vr::VROverlay()->PollNextOverlayEvent(
            m_ulOverlayThumbnailHandle, &vrEvent, sizeof( vrEvent ) ) )
        {
            switch ( vrEvent.eventType )
            {
            case vr::VREvent_OverlayShown:
            {
                m_pWindow->update();
            }
            break;
            }
        }
    }
}

void OverlayController::AddOffsetToUniverseCenter(
    vr::ETrackingUniverseOrigin universe,
    unsigned axisId,
    float offset,
    bool adjustBounds,
    bool commit )
{
    float offsetArray[3] = { 0, 0, 0 };
    offsetArray[axisId] = offset;
    AddOffsetToUniverseCenter( universe, offsetArray, adjustBounds, commit );
}

void OverlayController::AddOffsetToUniverseCenter(
    vr::ETrackingUniverseOrigin universe,
    float offset[3],
    bool adjustBounds,
    bool commit )
{
    if ( offset[0] != 0.0f || offset[1] != 0.0f || offset[2] != 0.0f )
    {
        if ( commit )
        {
            vr::VRChaperoneSetup()->HideWorkingSetPreview();
            vr::VRChaperoneSetup()->RevertWorkingCopy();
        }
        vr::HmdMatrix34_t curPos;
        if ( universe == vr::TrackingUniverseStanding )
        {
            vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(
                &curPos );
        }
        else
        {
            vr::VRChaperoneSetup()->GetWorkingSeatedZeroPoseToRawTrackingPose(
                &curPos );
        }
        for ( int i = 0; i < 3; i++ )
        {
            curPos.m[0][3] += curPos.m[0][i] * offset[i];
            curPos.m[1][3] += curPos.m[1][i] * offset[i];
            curPos.m[2][3] += curPos.m[2][i] * offset[i];
        }
        if ( universe == vr::TrackingUniverseStanding )
        {
            vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(
                &curPos );
        }
        else
        {
            vr::VRChaperoneSetup()->SetWorkingSeatedZeroPoseToRawTrackingPose(
                &curPos );
        }
        if ( adjustBounds && universe == vr::TrackingUniverseStanding )
        {
            float collisionOffset[] = { -offset[0], -offset[1], -offset[2] };
            AddOffsetToCollisionBounds( collisionOffset, false );
        }
        if ( commit )
        {
            vr::VRChaperoneSetup()->CommitWorkingCopy(
                vr::EChaperoneConfigFile_Live );
        }
    }
}

void OverlayController::RotateUniverseCenter(
    vr::ETrackingUniverseOrigin universe,
    float yAngle,
    bool adjustBounds,
    bool commit )
{
    if ( yAngle != 0.0f )
    {
        if ( commit )
        {
            vr::VRChaperoneSetup()->HideWorkingSetPreview();
            vr::VRChaperoneSetup()->RevertWorkingCopy();
        }
        vr::HmdMatrix34_t curPos;
        if ( universe == vr::TrackingUniverseStanding )
        {
            vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(
                &curPos );
        }
        else
        {
            vr::VRChaperoneSetup()->GetWorkingSeatedZeroPoseToRawTrackingPose(
                &curPos );
        }

        vr::HmdMatrix34_t rotMat;
        vr::HmdMatrix34_t newPos;
        utils::initRotationMatrix( rotMat, 1, yAngle );
        utils::matMul33( newPos, rotMat, curPos );
        newPos.m[0][3] = curPos.m[0][3];
        newPos.m[1][3] = curPos.m[1][3];
        newPos.m[2][3] = curPos.m[2][3];
        if ( universe == vr::TrackingUniverseStanding )
        {
            vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(
                &newPos );
        }
        else
        {
            vr::VRChaperoneSetup()->SetWorkingSeatedZeroPoseToRawTrackingPose(
                &newPos );
        }
        if ( adjustBounds && universe == vr::TrackingUniverseStanding )
        {
            RotateCollisionBounds( -yAngle, false );
        }
        if ( commit )
        {
            vr::VRChaperoneSetup()->CommitWorkingCopy(
                vr::EChaperoneConfigFile_Live );
        }
    }
}

void OverlayController::AddOffsetToCollisionBounds( unsigned axisId,
                                                    float offset,
                                                    bool commit )
{
    float offsetArray[3] = { 0, 0, 0 };
    offsetArray[axisId] = offset;
    AddOffsetToCollisionBounds( offsetArray, commit );
}

void OverlayController::AddOffsetToCollisionBounds( float offset[3],
                                                    bool commit )
{
    // Apparently Valve sanity-checks the y-coordinates of the collision bounds
    // (and only the y-coordinates) I can move the bounds on the xz-plane, I can
    // make the "ceiling" of the chaperone cage lower/higher, but when I dare to
    // set one single lower corner to something non-zero, every corner gets its
    // y-coordinates reset to the defaults.
    if ( commit )
    {
        vr::VRChaperoneSetup()->HideWorkingSetPreview();
        vr::VRChaperoneSetup()->RevertWorkingCopy();
    }
    unsigned collisionBoundsCount = 0;
    vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(
        nullptr, &collisionBoundsCount );
    if ( collisionBoundsCount > 0 )
    {
        vr::HmdQuad_t* collisionBounds
            = new vr::HmdQuad_t[collisionBoundsCount];
        vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(
            collisionBounds, &collisionBoundsCount );
        for ( unsigned b = 0; b < collisionBoundsCount; b++ )
        {
            for ( unsigned c = 0; c < 4; c++ )
            {
                collisionBounds[b].vCorners[c].v[0] += offset[0];

                // keep the lower corners on the ground so it doesn't reset all
                // y cooridinates. this causes the caperone to "grow" up instead
                // of not moving up at all. note that Valve still forces a
                // minimum height so we can't go into the ground
                if ( collisionBounds[b].vCorners[c].v[1] != 0 )
                {
                    collisionBounds[b].vCorners[c].v[1] += offset[1];
                }
                collisionBounds[b].vCorners[c].v[2] += offset[2];
            }
        }
        vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(
            collisionBounds, collisionBoundsCount );
        delete[] collisionBounds;
    }
    if ( commit && collisionBoundsCount > 0 )
    {
        vr::VRChaperoneSetup()->CommitWorkingCopy(
            vr::EChaperoneConfigFile_Live );
    }
}

void OverlayController::RotateCollisionBounds( float angle, bool commit )
{
    if ( commit )
    {
        vr::VRChaperoneSetup()->HideWorkingSetPreview();
        vr::VRChaperoneSetup()->RevertWorkingCopy();
    }
    unsigned collisionBoundsCount = 0;
    vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(
        nullptr, &collisionBoundsCount );
    if ( collisionBoundsCount > 0 )
    {
        vr::HmdQuad_t* collisionBounds
            = new vr::HmdQuad_t[collisionBoundsCount];
        vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(
            collisionBounds, &collisionBoundsCount );

        vr::HmdMatrix34_t rotMat;
        utils::initRotationMatrix( rotMat, 1, angle );
        for ( unsigned b = 0; b < collisionBoundsCount; b++ )
        {
            for ( unsigned c = 0; c < 4; c++ )
            {
                auto& corner = collisionBounds[b].vCorners[c];
                vr::HmdVector3_t newVal;
                utils::matMul33( newVal, rotMat, corner );
                corner = newVal;
            }
        }
        vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(
            collisionBounds, collisionBoundsCount );
        delete[] collisionBounds;
    }
    if ( commit && collisionBoundsCount > 0 )
    {
        vr::VRChaperoneSetup()->CommitWorkingCopy(
            vr::EChaperoneConfigFile_Live );
    }
}

QString OverlayController::getVersionString()
{
    return QString( application_strings::applicationVersionString );
}

QUrl OverlayController::getVRRuntimePathUrl()
{
    return m_runtimePathUrl;
}

bool OverlayController::soundDisabled()
{
    return m_noSound;
}

const vr::VROverlayHandle_t& OverlayController::overlayHandle()
{
    return m_ulOverlayHandle;
}

const vr::VROverlayHandle_t& OverlayController::overlayThumbnailHandle()
{
    return m_ulOverlayThumbnailHandle;
}

void OverlayController::showKeyboard( QString existingText,
                                      unsigned long userValue )
{
    vr::VROverlay()->ShowKeyboardForOverlay(
        m_ulOverlayHandle,
        vr::k_EGamepadTextInputModeNormal,
        vr::k_EGamepadTextInputLineModeSingleLine,
        "Advanced Settings Overlay",
        1024,
        existingText.toStdString().c_str(),
        false,
        userValue );
}

void OverlayController::playActivationSound()
{
    if ( !m_noSound )
    {
        m_activationSoundEffect.play();
    }
}

void OverlayController::playFocusChangedSound()
{
    if ( !m_noSound )
    {
        m_focusChangedSoundEffect.play();
    }
}

void OverlayController::playAlarm01Sound( bool loop )
{
    if ( !m_noSound && !m_alarm01SoundEffect.isPlaying() )
    {
        if ( loop )
        {
            m_alarm01SoundEffect.setLoopCount( QSoundEffect::Infinite );
        }
        else
        {
            m_alarm01SoundEffect.setLoopCount( 1 );
        }
        m_alarm01SoundEffect.play();
    }
}

void OverlayController::setAlarm01SoundVolume( float vol )
{
    m_alarm01SoundEffect.setVolume( static_cast<double>( vol ) );
}

void OverlayController::cancelAlarm01Sound()
{
    m_alarm01SoundEffect.stop();
}

} // namespace advsettings
