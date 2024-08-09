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
#include <QQuickGraphicsConfiguration>
#include <QOpenGLExtraFunctions>
#include <QCursor>
#include <QProcess>
#include <QMessageBox>
#include <QtLogging>
#include <QtDebug>
#include <cmath>
#include <openvr.h>
#include "openvr/ovr_overlay_wrapper.h"
#include "openvr/ovr_settings_wrapper.h"
#include "utils/Matrix.h"
#include "keyboard_input/input_sender.h"
#include "settings/settings.h"
#include "utils/update_rate.h"
#include <qapplication.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <rhi/qrhi.h>
#include <stdexcept>

// application namespace
namespace advsettings
{
int verifyCustomTickRate( const int tickRate )
{
    if ( tickRate < 1 )
    {
        return 1;
    }
    if ( tickRate > k_maxCustomTickRate )
    {
        return k_maxCustomTickRate;
    }

    return tickRate;
}

OverlayController::OverlayController( bool desktopMode,
                                      bool noSound,
                                      QQmlEngine& qmlEngine )
    : QObject(), m_desktopMode( desktopMode ), m_noSound( noSound ),
      m_verifiedCustomTickRateMs( verifyCustomTickRate( settings::getSetting(
          settings::IntSetting::APPLICATION_customTickRateMs ) ) ),
      m_actions(), m_alarm()
{
    // Arbitrarily chosen Max Length of Directory path, should be sufficient for
    // Any set-up
    const uint32_t maxLength = 16192;
    uint32_t requiredLength = 0;

    std::array<char, maxLength> tempRuntimePath;
    bool const pathIsGood = vr::VR_GetRuntimePath(
        tempRuntimePath.data(), maxLength, &requiredLength );

    // Throw Error If over 16k characters in path string
    if ( !pathIsGood )
    {
        qCritical() << "Error Finding VR Runtime Path, Attempting Recovery: ";
        uint32_t const maxLengthRe = requiredLength;
        qInfo() << "Open VR reporting Required path length of: " << maxLengthRe;
    }

    m_runtimePathUrl = QUrl::fromLocalFile( tempRuntimePath.data() );
    qInfo() << "VR Runtime Path: " << m_runtimePathUrl.toLocalFile();

    const float initVol = static_cast<float>( soundVolume() );
    m_activationSoundEffect.setSource( QUrl( "qrc:/sounds/click.wav" ) );
    m_activationSoundEffect.setVolume( initVol );

    m_focusChangedSoundEffect.setSource( QUrl( "qrc:/sounds/focus.wav" ) );
    m_focusChangedSoundEffect.setVolume( initVol );

    m_alarm01SoundEffect.setSource( QUrl( "qrc:/sounds/alarm01.wav" ) );
    m_alarm01SoundEffect.setVolume( 1.0 );

    // If we have desktop mode flag ignore waht toggle says otherwise we use
    // toggle
    if ( !m_desktopMode )
    {
        m_desktopMode = desktopModeToggle();
    }

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
    m_videoTabController.initStage1();
    m_rotationTabController.initStage1();

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
        []( QQmlEngine*, QJSEngine* )
        {
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
        []( QQmlEngine*, QJSEngine* )
        {
            QObject* obj = &( objectAddress->m_steamVRTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "ChaperoneTabController",
        []( QQmlEngine*, QJSEngine* )
        {
            QObject* obj = &( objectAddress->m_chaperoneTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "MoveCenterTabController",
        []( QQmlEngine*, QJSEngine* )
        {
            QObject* obj = &( objectAddress->m_moveCenterTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "FixFloorTabController",
        []( QQmlEngine*, QJSEngine* )
        {
            QObject* obj = &( objectAddress->m_fixFloorTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "AudioTabController",
        []( QQmlEngine*, QJSEngine* )
        {
            QObject* obj = &( objectAddress->m_audioTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "StatisticsTabController",
        []( QQmlEngine*, QJSEngine* )
        {
            QObject* obj = &( objectAddress->m_statisticsTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "SettingsTabController",
        []( QQmlEngine*, QJSEngine* )
        {
            QObject* obj = &( objectAddress->m_settingsTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "UtilitiesTabController",
        []( QQmlEngine*, QJSEngine* )
        {
            QObject* obj = &( objectAddress->m_utilitiesTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "VideoTabController",
        []( QQmlEngine*, QJSEngine* )
        {
            QObject* obj = &( objectAddress->m_videoTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<SteamVRTabController>(
        qmlSingletonImportName,
        1,
        0,
        "RotationTabController",
        []( QQmlEngine*, QJSEngine* )
        {
            QObject* obj = &( objectAddress->m_rotationTabController );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );
    qmlRegisterSingletonType<alarm_clock::VrAlarm>(
        qmlSingletonImportName,
        1,
        0,
        "VrAlarm",
        []( QQmlEngine*, QJSEngine* )
        {
            QObject* obj = &( objectAddress->m_alarm );
            QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );
            return obj;
        } );

    // Grab local version number
    QStringList verNumericalString
        = QString( application_strings::applicationVersionString ).split( "-" );
    QStringList verMajorMinorPatchString = verNumericalString[0].split( "." );
    m_localVersionMajor = verMajorMinorPatchString[0].toInt();
    m_localVersionMinor = verMajorMinorPatchString[1].toInt();
    m_localVersionPatch = verMajorMinorPatchString[2].toInt();

    // Init network manager
    connect( netManager,
             SIGNAL( finished( QNetworkReply* ) ),
             this,
             SLOT( OnNetworkReply( QNetworkReply* ) ) );

    if ( !disableVersionCheck() )
    {
        QNetworkRequest netRequest;
        netRequest.setUrl( QUrl( application_strings::versionCheckUrl ) );
        netManager->get( netRequest );
    }
    else
    {
        qInfo() << "Version Check: Feature disabled. Not checking version.";
    }

    qInfo() << "OPENSSL VERSION: "
            << QSslSocket::sslLibraryBuildVersionString();
}

OverlayController::~OverlayController()
{
    Shutdown();
}

void OverlayController::exitApp()
{
    // save to settings that shutdown was safe
    setPreviousShutdownSafe( true );

    settings::saveAllSettings();

    m_moveCenterTabController.shutdown();
    // Un-mute mic before Exiting VR, as it is set at system level Not
    // Vr level.
    // m_audioTabController.setMicMuted( false, false );
    m_audioTabController.shutdown();
    m_chaperoneTabController.shutdown();

    Shutdown();
    QApplication::exit();

    qInfo() << "All systems exited.";
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit( EXIT_SUCCESS );
    // Does not fallthrough
}

void OverlayController::Shutdown()
{
    disconnect( &m_pumpEventsTimer,
                SIGNAL( timeout() ),
                this,
                SLOT( OnTimeoutPumpEvents() ) );
    m_pumpEventsTimer.stop();

    if ( m_pRenderTimer )
    {
        disconnect( &m_renderControl,
                    SIGNAL( renderRequested() ),
                    this,
                    SLOT( OnRenderRequest() ) );
        disconnect( &m_renderControl,
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
    m_render_pass_descriptor.reset();
    m_render_target.reset();
    m_pFBTexture.reset();
}

void OverlayController::SetWidget( QQuickItem* quickItem,
                                   const std::string& name,
                                   const std::string& key )
{
    if ( !m_desktopMode )
    {
        vr::VROverlayError const overlayError
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

        {
            QImage thumbiconImg( QString( ":/icons/thumbicon.png" ) );
            ovr_overlay_wrapper::setOverlayFromQImage(
                m_ulOverlayThumbnailHandle, thumbiconImg );
        }

        // Too many render calls in too short time overwhelm Qt and an
        // assertion gets thrown. Therefore we use an timer to delay render
        // calls
        m_pRenderTimer.reset( new QTimer() );
        m_pRenderTimer->moveToThread( qApp->thread() );
        m_pRenderTimer->setSingleShot( true );
        m_pRenderTimer->setInterval( 5 );
        connect( m_pRenderTimer.get(),
                 SIGNAL( timeout() ),
                 this,
                 SLOT( renderOverlay() ) );

#if QT_CONFIG( vulkan )
        if ( m_window.graphicsApi() == QSGRendererInterface::Vulkan )
        {
            m_vulkanInstance.reset( new QVulkanInstance );
            m_vulkanInstance->setExtensions(
                QQuickGraphicsConfiguration::preferredInstanceExtensions() );
            if ( !m_vulkanInstance->create() )
                throw std::runtime_error( "Cannot create vulkan instance" );

            m_window.setVulkanInstance( m_vulkanInstance.get() );
        }
#endif

        if ( !m_renderControl.initialize() )
            throw std::runtime_error( "could not initialize m_renderControl" );

        QRhi* rhi = this->rhi();

        qInfo() << "Started with" << rhi->backendName();

        m_pFBTexture.reset( rhi->newTexture(
            QRhiTexture::RGBA8,
            quickItem->size().toSize(),
            1,
            QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource ) );
        if ( !m_pFBTexture->create() )
            throw std::runtime_error( "RhiTexture not created" );

        m_render_target.reset(
            rhi->newTextureRenderTarget( { m_pFBTexture.get() } ) );
        m_render_pass_descriptor.reset(
            m_render_target->newCompatibleRenderPassDescriptor() );
        m_render_target->setRenderPassDescriptor(
            m_render_pass_descriptor.get() );
        if ( !m_render_target->create() )
            throw std::runtime_error( "failed to create render target" );

        auto qqrt
            = QQuickRenderTarget::fromRhiRenderTarget( m_render_target.get() );

        m_window.setRenderTarget( qqrt );
        quickItem->setParentItem( m_window.contentItem() );
        m_window.setGeometry( 0,
                              0,
                              static_cast<int>( quickItem->width() ),
                              static_cast<int>( quickItem->height() ) );

        vr::HmdVector2_t const vecWindowSize
            = { static_cast<float>( quickItem->width() ),
                static_cast<float>( quickItem->height() ) };
        vr::VROverlay()->SetOverlayMouseScale( m_ulOverlayHandle,
                                               &vecWindowSize );

        connect( &m_renderControl,
                 SIGNAL( renderRequested() ),
                 this,
                 SLOT( OnRenderRequest() ) );
        connect( &m_renderControl,
                 SIGNAL( sceneChanged() ),
                 this,
                 SLOT( OnRenderRequest() ) );
    }

    connect( &m_pumpEventsTimer,
             SIGNAL( timeout() ),
             this,
             SLOT( OnTimeoutPumpEvents() ) );

    // Every 1ms we check if the current frame has advanced (for vsync)
    m_pumpEventsTimer.setInterval( 1 );

    m_pumpEventsTimer.start();

    m_steamVRTabController.initStage2( this );
    m_chaperoneTabController.initStage2( this );
    m_fixFloorTabController.initStage2( this );
    m_audioTabController.initStage2( this );
    m_statisticsTabController.initStage2( this );
    m_settingsTabController.initStage2( this );
    m_utilitiesTabController.initStage2( this );
    m_moveCenterTabController.initStage2( this );
    m_rotationTabController.initStage2( this );
    m_videoTabController.initStage2();

    if ( autoApplyChaperoneEnabled() )
    {
        m_chaperoneTabController.reloadChaperoneProfiles();
        auto chapindex
            = m_chaperoneTabController.getChaperoneProfileIndexFromName(
                autoApplyChaperoneName() );
        if ( chapindex.first )
        {
            qInfo() << "Auto Applying Chaperone";
            m_chaperoneTabController.applyChaperoneProfile( chapindex.second );
            // This should be the way to stop room-setup from starting... as it
            // sends steamvr a signal that it is completed
            vr::VRChaperoneSetup()->CommitWorkingCopy(
                vr::EChaperoneConfigFile_Live );
            return;
        }
        qWarning() << "Profile Not Found for Auto Apply Chaperone!";
    }
}

void OverlayController::OnRenderRequest()
{
    if ( m_pRenderTimer && !m_pRenderTimer->isActive() )
    {
        m_pRenderTimer->start();
    }
}

void OverlayController::SetOverlayFromQRhiTexture( QRhiTexture& tex )
{
    vr::Texture_t vrTex = {};
    QRhi* rhi = this->rhi();

    switch ( rhi->backend() )
    {
    case QRhi::OpenGLES2:
    {
        // NOLINTNEXTLINE(performance-no-int-to-ptr)
        vrTex.handle = reinterpret_cast<void*>( tex.nativeTexture().object );
        vrTex.eType = vr::TextureType_OpenGL;
        vrTex.eColorSpace = vr::ColorSpace_Auto;
    }
    break;
#if QT_CONFIG( vulkan )
    case QRhi::Vulkan:
    {
        const QRhiVulkanNativeHandles* vulkan_handles
            = reinterpret_cast<const QRhiVulkanNativeHandles*>(
                rhi->nativeHandles() );
        vr::VRVulkanTextureData_t vulkan = {
            .m_nImage = tex.nativeTexture().object,
            .m_pDevice = vulkan_handles->dev,
            .m_pPhysicalDevice = vulkan_handles->physDev,
            .m_pInstance = vulkan_handles->inst->vkInstance(),
            .m_pQueue = vulkan_handles->gfxQueue,
            .m_nQueueFamilyIndex = vulkan_handles->gfxQueueIdx,
            .m_nWidth = static_cast<uint32_t>( tex.pixelSize().width() ),
            .m_nHeight = static_cast<uint32_t>( tex.pixelSize().height() ),
            .m_nFormat = static_cast<uint32_t>( tex.format() ),
            .m_nSampleCount = static_cast<uint32_t>( tex.sampleCount() ),
        };
        vrTex.handle = &vulkan;
        vrTex.eType = vr::TextureType_Vulkan;
        vrTex.eColorSpace = vr::ColorSpace_Auto;
    }
    break;
#endif
    default:
        qFatal() << "unimplemented backend:" << rhi->backend();
        throw std::runtime_error( "unimplemented backend" );
    }
    vr::VROverlay()->SetOverlayTexture( m_ulOverlayHandle, &vrTex );
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
        m_renderControl.polishItems();
        m_renderControl.beginFrame();
        m_renderControl.sync();
        m_renderControl.render();
        m_renderControl.endFrame();

        SetOverlayFromQRhiTexture( *m_pFBTexture );
    }
}

bool OverlayController::pollNextEvent( vr::VROverlayHandle_t ulOverlayHandle,
                                       vr::VREvent_t* pEvent )
{
    if ( isDesktopMode() )
    {
        return vr::VRSystem()->PollNextEvent( pEvent, sizeof( vr::VREvent_t ) );
    }

    return vr::VROverlay()->PollNextOverlayEvent(
        ulOverlayHandle, pEvent, sizeof( vr::VREvent_t ) );
}

QPointF OverlayController::getMousePositionForEvent( vr::VREvent_Mouse_t mouse )
{
    float mouse_y = mouse.y;
#ifdef __linux__
    float const height = static_cast<float>( m_window.height() );
    mouse_y = height - mouse_y;
#endif
    return { static_cast<double>(mouse.x), static_cast<double>(mouse_y) };
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
    // Execution order for moveCenterTabController actions is important.
    // Don't reorder these. Override actions must always come after normal
    // because active priority is set based on which action is "newest"
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
    m_moveCenterTabController.applyOffsets( m_actions.applyOffsets() );
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
        // strictly speaking this is not the most elegant solution, but
        // should work well enough.
        if ( !pushToTalkEnabled )
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
    if ( m_actions.keyboardOne() )
    {
        const auto commands = settings::getSetting(
            settings::StringSetting::KEYBOARDSHORTCUT_keyboardOne );

        sendStringAsInput( commands );
    }

    if ( m_actions.keyboardTwo() )
    {
        const auto commands = settings::getSetting(
            settings::StringSetting::KEYBOARDSHORTCUT_keyboardTwo );

        sendStringAsInput( commands );
    }

    if ( m_actions.keyboardThree() )
    {
        const auto commands = settings::getSetting(
            settings::StringSetting::KEYBOARDSHORTCUT_keyboardThree );

        sendStringAsInput( commands );
    }
    // Press Key One
    if ( m_actions.keyPressMisc() && !m_keyPressOneState )
    {
        const auto commands = settings::getSetting(
            settings::StringSetting::KEYBOARDSHORTCUT_keyPressMisc );
        sendFirstCharAsInput( commands, KeyStatus::Down );
        m_keyPressOneState = true;
    }
    if ( m_keyPressOneState && !m_actions.keyPressMisc() )
    {
        const auto commands = settings::getSetting(
            settings::StringSetting::KEYBOARDSHORTCUT_keyPressMisc );
        sendFirstCharAsInput( commands, KeyStatus::Up );
        m_keyPressOneState = false;
    }

    // Press Key Two
    if ( m_actions.keyPressSystem() && !m_keyPressTwoState )
    {
        const auto commands = settings::getSetting(
            settings::StringSetting::KEYBOARDSHORTCUT_keyPressSystem );
        sendFirstCharAsInput( commands, KeyStatus::Down );
        m_keyPressTwoState = true;
    }
    if ( m_keyPressTwoState && !m_actions.keyPressSystem() )
    {
        const auto commands = settings::getSetting(
            settings::StringSetting::KEYBOARDSHORTCUT_keyPressSystem );
        sendFirstCharAsInput( commands, KeyStatus::Up );
        m_keyPressTwoState = false;
    }
}

void OverlayController::processExclusiveInputBinding()
{
    if ( m_actions.exclusiveInputToggle() && exclusiveInputEnabled() )
    {
        m_exclusiveState = !m_exclusiveState;
        m_actions.systemActionSetOnlyEnabled( !m_exclusiveState );
        m_actions.actionSetPriorityToggle( m_exclusiveState );
    }
}

void OverlayController::processRotationBindings()
{
    if ( m_actions.autoTurnToggle() )
    {
        m_rotationTabController.setAutoTurnEnabled(
            !( m_rotationTabController.autoTurnEnabled() ) );
    }
}
/*!
Checks if an action has been activated and dispatches the related action if
it has been.
*/
void OverlayController::processInputBindings()
{
    processExclusiveInputBinding();

    processMediaKeyBindings();

    processMotionBindings();

    processPushToTalkBindings();

    processChaperoneBindings();

    processKeyboardBindings();

    processRotationBindings();
}

bool OverlayController::exclusiveInputEnabled() const
{
    return settings::getSetting(
        settings::BoolSetting::APPLICATION_enableExclusiveInput );
}

void OverlayController::setExclusiveInputEnabled( bool value, bool notify )
{
    settings::setSetting(
        settings::BoolSetting::APPLICATION_enableExclusiveInput, value );
    // Note: These Calls technically modify data that could be accessed
    // elsewhere and cause threading issues. Since this should only be
    // accessible in dashboard, and input is disabled while dashboard is up,
    // there should be no reason to lock it. If it becomes an issue this needs
    // to be locked with a mutex or similiar against the
    // processexclusivebindings
    if ( value )
    {
        // Re-Enable Required SteamVR key every toggle just in case.
        ovr_settings_wrapper::setBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_AllowGlobalActionSetPriority,
            value );

        // To setup Exclusive Input OVRAS actions should be off besides System
        // (+haptics) which are always on.
        m_actions.systemActionSetOnlyEnabled( true );
    }
    else
    {
        // Enable All Action Sets
        m_actions.systemActionSetOnlyEnabled( false );
        // Remove All priority
        m_actions.actionSetPriorityToggle( false );
    }
    if ( notify )
    {
        emit exclusiveInputEnabledChanged( value );
    }
}
void OverlayController::setAutoApplyChaperoneEnabled( bool value, bool notify )
{
    settings::setSetting( settings::BoolSetting::APPLICATION_autoApplyChaperone,
                          value );
    if ( notify )
    {
        emit autoApplyChaperoneEnabledChanged( value );
    }
}

std::string OverlayController::autoApplyChaperoneName()
{
    return settings::getSetting(
        settings::StringSetting::APPLICATION_autoApplyChaperoneName );
}

Q_INVOKABLE void OverlayController::setAutoChapProfileName( int index )
{
    std::string const value
        = m_chaperoneTabController.getChaperoneProfileName( index )
              .toStdString();
    settings::setSetting(
        settings::StringSetting::APPLICATION_autoApplyChaperoneName, value );
}

bool OverlayController::autoApplyChaperoneEnabled() const
{
    return settings::getSetting(
        settings::BoolSetting::APPLICATION_autoApplyChaperone );
}

bool OverlayController::crashRecoveryDisabled() const
{
    return settings::getSetting(
        settings::BoolSetting::APPLICATION_crashRecoveryDisabled2 );
}

void OverlayController::setCrashRecoveryDisabled( bool value, bool notify )
{
    settings::setSetting(
        settings::BoolSetting::APPLICATION_crashRecoveryDisabled2, value );
    if ( notify )
    {
        emit crashRecoveryDisabledChanged( value );
    }
}

bool OverlayController::vsyncDisabled() const
{
    return settings::getSetting(
        settings::BoolSetting::APPLICATION_vsyncDisabled );
}

void OverlayController::setVsyncDisabled( bool value, bool notify )
{
    settings::setSetting( settings::BoolSetting::APPLICATION_vsyncDisabled,
                          value );
    if ( notify )
    {
        emit vsyncDisabledChanged( value );
    }
}

bool OverlayController::enableDebug() const
{
    return settings::getSetting(
        settings::BoolSetting::APPLICATION_enableDebug );
}

void OverlayController::setEnableDebug( bool value, bool notify )
{
    settings::setSetting( settings::BoolSetting::APPLICATION_enableDebug,
                          value );
    if ( notify )
    {
        emit enableDebugChanged( value );
    }
}

bool OverlayController::disableVersionCheck() const
{
    return settings::getSetting(
        settings::BoolSetting::APPLICATION_disableVersionCheck );
}

void OverlayController::setDisableVersionCheck( bool value, bool notify )
{
    if ( !value )
    {
        QNetworkRequest netRequest;
        netRequest.setUrl( QUrl( application_strings::versionCheckUrl ) );
        netManager->get( netRequest );
    }
    settings::setSetting(
        settings::BoolSetting::APPLICATION_disableVersionCheck, value );
    if ( notify )
    {
        emit disableVersionCheckChanged( value );
    }
}

bool OverlayController::newVersionDetected() const
{
    return m_newVersionDetected;
}

void OverlayController::setNewVersionDetected( bool value, bool notify )
{
    if ( m_newVersionDetected == value )
    {
        return;
    }
    m_newVersionDetected = value;
    if ( notify )
    {
        emit newVersionDetectedChanged( m_newVersionDetected );
    }
}

QString OverlayController::versionCheckText() const
{
    return m_versionCheckText;
}

void OverlayController::setVersionCheckText( QString value, bool notify )
{
    m_versionCheckText = value;
    qInfo() << "m_versionCheckText = " << m_versionCheckText;
    if ( notify )
    {
        emit versionCheckTextChanged( m_versionCheckText );
    }
}

int OverlayController::debugState() const
{
    return settings::getSetting( settings::IntSetting::APPLICATION_debugState );
}

void OverlayController::setDebugState( int value, bool notify )
{
    settings::setSetting( settings::IntSetting::APPLICATION_debugState, value );

    if ( notify )
    {
        emit debugStateChanged( value );
    }
}

void OverlayController::setPreviousShutdownSafe( bool value )
{
    settings::setSetting(
        settings::BoolSetting::APPLICATION_previousShutdownSafe, value );
}

int OverlayController::customTickRateMs() const
{
    return m_verifiedCustomTickRateMs;
}

void OverlayController::setCustomTickRateMs( int value, bool notify )
{
    const auto verifiedTickRate = verifyCustomTickRate( value );

    settings::setSetting( settings::IntSetting::APPLICATION_customTickRateMs,
                          verifiedTickRate );
    m_verifiedCustomTickRateMs = verifiedTickRate;

    if ( notify )
    {
        emit customTickRateMsChanged( verifiedTickRate );
    }
}

// vsync implementation:
// this function triggers every 1ms
// this function should remain lightweight and only check if it's time to
// run mainEventLoop() or not.
void OverlayController::OnTimeoutPumpEvents()
{
    if ( vsyncDisabled() )
    {
        // check if it's time for a custom tick rate tick
        if ( m_customTickRateCounter > customTickRateMs() )
        {
            mainEventLoop();
            m_customTickRateCounter = 0;
            updateRate.incrementCounter();
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
            // main event loop. (this function should trigger about every
            // 11ms assuming 90fps compositor)
            mainEventLoop();
            updateRate.incrementCounter();

            // wait for the next frame after executing our main event loop
            // once.
            m_lastFrame = m_currentFrame;
            m_vsyncTooLateCounter = 0;
        }
        else if ( m_vsyncTooLateCounter >= k_nonVsyncTickRate )
        {
            mainEventLoop();
            // m_lastFrame = m_currentFrame + 1 skips the next vsync frame
            // in case it was just about to trigger, to prevent double
            // updates faster than 11ms.
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
            QPointF const ptNewMouse
                = getMousePositionForEvent( vrEvent.data.mouse );
            if ( ptNewMouse != m_ptLastMouse )
            {
                QMouseEvent mouseEvent( QEvent::MouseMove,
                                        ptNewMouse,
                                        m_window.mapToGlobal( ptNewMouse ),
                                        Qt::NoButton,
                                        m_lastMouseButtons,
                                        Qt::NoModifier );
                m_ptLastMouse = ptNewMouse;
                QCoreApplication::sendEvent( &m_window, &mouseEvent );
                OnRenderRequest();
            }
        }
        break;

        case vr::VREvent_MouseButtonDown:
        {
            QPointF const ptNewMouse
                = getMousePositionForEvent( vrEvent.data.mouse );
            Qt::MouseButton const button
                = vrEvent.data.mouse.button == vr::VRMouseButton_Right
                      ? Qt::RightButton
                      : Qt::LeftButton;
            m_lastMouseButtons |= button;
            QMouseEvent mouseEvent( QEvent::MouseButtonPress,
                                    ptNewMouse,
                                    m_window.mapToGlobal( ptNewMouse ),
                                    button,
                                    m_lastMouseButtons,
                                    Qt::NoModifier );
            QCoreApplication::sendEvent( &m_window, &mouseEvent );
        }
        break;

        case vr::VREvent_MouseButtonUp:
        {
            QPointF const ptNewMouse
                = getMousePositionForEvent( vrEvent.data.mouse );
            Qt::MouseButton const button
                = vrEvent.data.mouse.button == vr::VRMouseButton_Right
                      ? Qt::RightButton
                      : Qt::LeftButton;
            m_lastMouseButtons &= ~button;
            QMouseEvent mouseEvent( QEvent::MouseButtonRelease,
                                    ptNewMouse,
                                    m_window.mapToGlobal( ptNewMouse ),
                                    button,
                                    m_lastMouseButtons,
                                    Qt::NoModifier );
            QCoreApplication::sendEvent( &m_window, &mouseEvent );
        }
        break;

        case vr::VREvent_ScrollSmooth:
        {
            // Wheel speed is defined as 1/8 of a degree
            QWheelEvent wheelEvent(
                m_ptLastMouse,
                m_window.mapToGlobal( m_ptLastMouse ),
                QPoint(),
                QPoint( static_cast<int>( vrEvent.data.scroll.xdelta
                                          * ( 360.0f * 8.0f ) ),
                        static_cast<int>( vrEvent.data.scroll.ydelta
                                          * ( 360.0f * 8.0f ) ) ),
                m_lastMouseButtons,
                Qt::KeyboardModifiers::fromInt( 0 ),
                Qt::ScrollPhase::NoScrollPhase,
                false );
            QCoreApplication::sendEvent( &m_window, &wheelEvent );
        }
        break;

        case vr::VREvent_OverlayShown:
        {
            m_window.update();
        }
        break;

        case vr::VREvent_Quit:
        {
            qInfo() << "Received quit request.";
            vr::VRSystem()->AcknowledgeQuit_Exiting(); // Let us buy some
                                                       // time just in case

            exitApp();
            // Won't fallthrough, but also exitApp() wont, but QT won't
            // acknowledge
            // NOLINTNEXTLINE(concurrency-mt-unsafe)
            exit( EXIT_SUCCESS );
        }

        case vr::VREvent_DashboardActivated:
        {
            qDebug() << "Dashboard activated";
            m_dashboardVisible = true;
        }
        break;

        case vr::VREvent_DashboardDeactivated:
        {
            qDebug() << "Dashboard deactivated";
            m_dashboardVisible = false;
            settings::saveChangedSettings();
        }
        break;

        case vr::VREvent_KeyboardDone:
        {
            std::array<char, 1024> keyboardBuffer;
            vr::VROverlay()->GetKeyboardText( keyboardBuffer.data(), 1024 );
            emit keyBoardInputSignal( QString( keyboardBuffer.data() ),
                                      static_cast<unsigned long>(
                                          vrEvent.data.keyboard.uUserValue ) );
        }
        break;

        case vr::VREvent_SeatedZeroPoseReset:
        case vr::VREvent_StandingZeroPoseReset:
        {
            m_incomingReset = true;
        }
        break;

        // Multiple ChaperoneUniverseHasChanged are often
        // emitted at the same time (some with a little bit of
        // delay) There is no sure way to recognize redundant
        // events, we can only exclude redundant events during
        // the same call of OnTimeoutPumpEvents() INFO Removed
        // logging on play space mover for possible crashing
        // issues.
        case vr::VREvent_ChaperoneUniverseHasChanged:
        {
            uint64_t const previousUniverseId
                = vrEvent.data.chaperone.m_nPreviousUniverse;
            uint64_t const currentUniverseId
                = vrEvent.data.chaperone.m_nCurrentUniverse;
            qInfo() << "(VREvent) ChaperoneUniverseHasChanged... Previous : "
                    << previousUniverseId << " Current:" << currentUniverseId;
            if ( !chaperoneDataAlreadyUpdated )
            {
                m_chaperoneUtils.loadChaperoneData();
                chaperoneDataAlreadyUpdated = true;
            }
            if ( previousUniverseId == 0
                 && !m_moveCenterTabController.isInitComplete() )
            {
                m_moveCenterTabController.zeroOffsets();
            }
        }
        break;
        case vr::VREvent_Input_ActionManifestReloaded:
        {
            // qWarning() << "Action Manifest Reloaded";
            if ( m_steamVRTabController.perAppBindEnabled() )
            {
                m_steamVRTabController.applyAllCustomBindings();
            }
        }
        break;
        default:
            break;
        }
    }
    if ( m_incomingReset )
    {
        m_incomingReset = false;
        qInfo() << "Reset zero event recorded";
        m_moveCenterTabController.incomingZeroReset();
    }

    std::array<vr::TrackedDevicePose_t, vr::k_unMaxTrackedDeviceCount>
        devicePoses;
    vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(
        vr::TrackingUniverseStanding,
        0.0f,
        devicePoses.data(),
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
    auto universe = vr::VRCompositor()->GetTrackingSpace();
    m_moveCenterTabController.eventLoopTick( universe, devicePoses.data() );
    m_utilitiesTabController.eventLoopTick();
    m_statisticsTabController.eventLoopTick(
        devicePoses.data(), leftSpeed, rightSpeed );
    m_chaperoneTabController.eventLoopTick( universe, devicePoses.data() );
    m_audioTabController.eventLoopTick();
    m_rotationTabController.eventLoopTick( devicePoses.data() );

    m_alarm.eventLoopTick();

    if ( vr::VROverlay()->IsDashboardVisible() || m_desktopMode )
    {
        m_settingsTabController.dashboardLoopTick();
        m_steamVRTabController.dashboardLoopTick();
        m_fixFloorTabController.dashboardLoopTick( devicePoses.data() );
        m_videoTabController.dashboardLoopTick();
        m_chaperoneTabController.dashboardLoopTick();
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
                m_window.update();
            }
            default:
                break;
            }
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
        utils::initRotationMatrix( rotMat, utils::MatrixAxis_Y, yAngle );
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
    std::array<float, 3> offsetArray = { 0, 0, 0 };
    offsetArray[axisId] = offset;
    AddOffsetToCollisionBounds( offsetArray, commit );
}

void OverlayController::AddOffsetToCollisionBounds( std::array<float, 3> offset,
                                                    bool commit )
{
    // Apparently Valve sanity-checks the y-coordinates of the collision
    // bounds (and only the y-coordinates) I can move the bounds on the
    // xz-plane, I can make the "ceiling" of the chaperone cage
    // lower/higher, but when I dare to set one single lower corner to
    // something non-zero, every corner gets its y-coordinates reset to the
    // defaults.
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
        std::vector<vr::HmdQuad_t> collisionBounds;
        collisionBounds.resize( collisionBoundsCount );
        vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(
            collisionBounds.data(), &collisionBoundsCount );
        for ( auto& bounds : collisionBounds )
        {
            for ( unsigned cIndex = 0; cIndex < 4; cIndex++ )
            {
                bounds.vCorners[cIndex].v[0] += offset[0];

                // keep the lower corners on the ground so it doesn't reset
                // all y cooridinates. this causes the caperone to "grow" up
                // instead of not moving up at all. note that Valve still
                // forces a minimum height so we can't go into the ground
                if ( bounds.vCorners[cIndex].v[1] != 0 )
                {
                    bounds.vCorners[cIndex].v[1] += offset[1];
                }
                bounds.vCorners[cIndex].v[2] += offset[2];
            }
        }
        vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(
            collisionBounds.data(), collisionBoundsCount );
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
        std::vector<vr::HmdQuad_t> collisionBounds;
        collisionBounds.resize( collisionBoundsCount );
        vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(
            collisionBounds.data(), &collisionBoundsCount );

        vr::HmdMatrix34_t rotMat;
        utils::initRotationMatrix( rotMat, utils::MatrixAxis_Y, angle );
        for ( auto& bounds : collisionBounds )
        {
            for ( unsigned cIndex = 0; cIndex < 4; cIndex++ )
            {
                auto& corner = bounds.vCorners[cIndex];
                vr::HmdVector3_t newVal;
                utils::matMul33( newVal, rotMat, corner );
                corner = newVal;
            }
        }
        vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(
            collisionBounds.data(), collisionBoundsCount );
    }
    if ( commit && collisionBoundsCount > 0 )
    {
        vr::VRChaperoneSetup()->CommitWorkingCopy(
            vr::EChaperoneConfigFile_Live );
    }
}

bool OverlayController::isPreviousShutdownSafe()
{
    return settings::getSetting(
        settings::BoolSetting::APPLICATION_previousShutdownSafe );
}

QString OverlayController::getVersionString()
{
    return { application_strings::applicationVersionString };
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
        0,
        "Advanced Settings Overlay",
        1024,
        existingText.toStdString().c_str(),
        userValue );
    setKeyboardPos();
}

void OverlayController::setKeyboardPos()
{
    vr::HmdVector2_t emptyvec;
    emptyvec.v[0] = 0;
    emptyvec.v[1] = 0;
    vr::HmdRect2_t empty;
    empty.vTopLeft = emptyvec;
    empty.vBottomRight = emptyvec;
    vr::VROverlay()->SetKeyboardPositionForOverlay( m_ulOverlayHandle, empty );
}

void OverlayController::setSoundVolume( double dvalue, bool notify )
{
    auto value = static_cast<float>( dvalue );
    m_activationSoundEffect.setVolume( value );
    m_focusChangedSoundEffect.setVolume( value );
    // leaving alarm sound alone for now as chaperone warning setting effects it
    // m_alarm01SoundEffect.setVolume( value);
    settings::setSetting( settings::DoubleSetting::APPLICATION_appVolume,
                          dvalue );
    if ( notify )
    {
        emit soundVolumeChanged( dvalue );
    }
}

double OverlayController::soundVolume() const
{
    return settings::getSetting(
        settings::DoubleSetting::APPLICATION_appVolume );
}

void OverlayController::setDesktopModeToggle( bool value, bool notify )
{
    settings::setSetting( settings::BoolSetting::APPLICATION_desktopModeToggle,
                          value );
    if ( notify )
    {
        emit desktopModeToggleChanged( value );
    }
    settings::saveAllSettings();
}
bool OverlayController::desktopModeToggle() const
{
    return settings::getSetting(
        settings::BoolSetting::APPLICATION_desktopModeToggle );
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
    m_alarm01SoundEffect.setVolume( vol );
}

void OverlayController::cancelAlarm01Sound()
{
    m_alarm01SoundEffect.stop();
}

void OverlayController::OnNetworkReply( QNetworkReply* reply )
{
    if ( reply->error() == QNetworkReply::NoError )
    {
        QByteArray const replyByteData = reply->readAll();
        qInfo() << "Version Check: Recieved Data: " << replyByteData;
        QJsonParseError parseError;
        m_remoteVersionJsonDocument
            = QJsonDocument::fromJson( replyByteData, &parseError );
        if ( parseError.error == QJsonParseError::NoError )
        {
            m_remoteVersionJsonObject = m_remoteVersionJsonDocument.object();
            m_remoteVersionMajor
                = m_remoteVersionJsonObject.value( "major" ).toInt();
            m_remoteVersionMinor
                = m_remoteVersionJsonObject.value( "minor" ).toInt();
            m_remoteVersionPatch
                = m_remoteVersionJsonObject.value( "patch" ).toInt();
            m_updateMessage
                = m_remoteVersionJsonObject.value( "updateMessage" ).toString();
            m_optionalMessage
                = m_remoteVersionJsonObject.value( "optionalMessage" )
                      .toString();

            // this is a little convoluted to ensure if our local version is
            // somehow higher than remote, it doesn't detect an update from
            // just higher remote "minor" or "patch" values.
            if ( ( m_remoteVersionMajor > m_localVersionMajor )
                 || ( m_remoteVersionMajor == m_localVersionMajor
                      && m_remoteVersionMinor > m_localVersionMinor )
                 || ( m_remoteVersionMajor == m_localVersionMajor
                      && m_remoteVersionMinor == m_localVersionMinor
                      && m_remoteVersionPatch > m_localVersionPatch ) )
            {
                setNewVersionDetected( true );
                if ( m_updateMessage.length() > 0 )
                {
                    setVersionCheckText( m_updateMessage );
                }
                else
                {
                    setVersionCheckText(
                        "Newer version ("
                        + QString::number( m_remoteVersionMajor ) + "."
                        + QString::number( m_remoteVersionMinor ) + "."
                        + QString::number( m_remoteVersionPatch )
                        + ") available." );
                }
                qInfo() << "Version Check: Newer version ("
                        << m_remoteVersionMajor << "." << m_remoteVersionMinor
                        << "." << m_remoteVersionPatch << ") available.";
            }
            else
            {
                if ( m_optionalMessage.length() > 0 )
                {
                    setVersionCheckText( m_optionalMessage );
                }
                setNewVersionDetected( false );
                qInfo() << "Version Check: Installed version is latest "
                           "release.";
            }
        }
        else
        {
            qCritical()
                << "Version Check: Error parsing json. QJsonParseError = "
                << parseError.error;
        }
    }
    else
    {
        qCritical() << "Version Check: Error connecting to network. "
                       "QNetworkReply::NetworkError = "
                    << reply->error();
    }
    reply->deleteLater();
}

} // namespace advsettings
