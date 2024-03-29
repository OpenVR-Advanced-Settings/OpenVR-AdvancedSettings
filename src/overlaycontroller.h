#pragma once

#include <openvr.h>
#include <QtCore/QtCore>
// because of incompatibilities with QtOpenGL and GLEW we need to cherry pick
// includes
#include <QVector2D>
#include <QMatrix4x4>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QObject>
#include <QQmlEngine>
#include <QtGui/QOpenGLContext>
#include <QtWidgets/QGraphicsScene>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQuickRenderControl>
#include <QSoundEffect>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <memory>
#include <easylogging++.h>

#include "openvr/openvr_init.h"

#include "utils/ChaperoneUtils.h"

#include "tabcontrollers/SteamVRTabController.h"
#include "tabcontrollers/ChaperoneTabController.h"
#include "tabcontrollers/MoveCenterTabController.h"
#include "tabcontrollers/FixFloorTabController.h"
#include "tabcontrollers/AudioTabController.h"
#include "tabcontrollers/StatisticsTabController.h"
#include "tabcontrollers/SettingsTabController.h"
#include "tabcontrollers/UtilitiesTabController.h"
#include "tabcontrollers/VideoTabController.h"
#include "tabcontrollers/RotationTabController.h"

#include "openvr/ivrinput.h"

#include "alarm_clock/vr_alarm.h"

#include "utils/update_rate.h"

namespace application_strings
{
constexpr auto applicationOrganizationName = "AdvancedSettings-Team";
constexpr auto applicationName = "OVR Advanced Settings";
constexpr const char* applicationKey = "steam.overlay.1009850";
constexpr const char* applicationDisplayName = "OVR Advanced Settings";
constexpr const char* versionCheckUrl
    = "https://raw.githubusercontent.com/OpenVR-Advanced-Settings/"
      "OpenVR-AdvancedSettings/master/ver/versioncheck.json";

constexpr const char* applicationVersionString = APPLICATION_VERSION;

} // namespace application_strings

// application namespace
namespace advsettings
{
// k_nonVsyncTickRate determines number of ms we wait to force the next event
// loop tick when vsync is too late due to dropped frames.
constexpr int k_nonVsyncTickRate = 20;
constexpr int k_maxCustomTickRate = 999;
constexpr int k_hmdRotationCounterUpdateRate = 7;

class OverlayController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool m_desktopMode READ isDesktopMode )
    Q_PROPERTY(
        bool crashRecoveryDisabled READ crashRecoveryDisabled WRITE
            setCrashRecoveryDisabled NOTIFY crashRecoveryDisabledChanged )
    Q_PROPERTY( bool vsyncDisabled READ vsyncDisabled WRITE setVsyncDisabled
                    NOTIFY vsyncDisabledChanged )
    Q_PROPERTY( bool enableDebug READ enableDebug WRITE setEnableDebug NOTIFY
                    enableDebugChanged )
    Q_PROPERTY( bool disableVersionCheck READ disableVersionCheck WRITE
                    setDisableVersionCheck NOTIFY disableVersionCheckChanged )
    Q_PROPERTY( bool newVersionDetected READ newVersionDetected WRITE
                    setNewVersionDetected NOTIFY newVersionDetectedChanged )
    Q_PROPERTY( QString versionCheckText READ versionCheckText WRITE
                    setVersionCheckText NOTIFY versionCheckTextChanged )
    Q_PROPERTY( int customTickRateMs READ customTickRateMs WRITE
                    setCustomTickRateMs NOTIFY customTickRateMsChanged )
    Q_PROPERTY( int debugState READ debugState WRITE setDebugState NOTIFY
                    debugStateChanged )
    Q_PROPERTY(
        bool exclusiveInputEnabled READ exclusiveInputEnabled WRITE
            setExclusiveInputEnabled NOTIFY exclusiveInputEnabledChanged )
    Q_PROPERTY( bool autoApplyChaperoneEnabled READ autoApplyChaperoneEnabled
                    WRITE setAutoApplyChaperoneEnabled NOTIFY
                        autoApplyChaperoneEnabledChanged )
    Q_PROPERTY( double soundVolume READ soundVolume WRITE setSoundVolume NOTIFY
                    soundVolumeChanged )
    Q_PROPERTY( bool desktopModeToggle READ desktopModeToggle WRITE
                    setDesktopModeToggle NOTIFY desktopModeToggleChanged )

private:
    vr::VROverlayHandle_t m_ulOverlayHandle = vr::k_ulOverlayHandleInvalid;
    vr::VROverlayHandle_t m_ulOverlayThumbnailHandle
        = vr::k_ulOverlayHandleInvalid;

    QQuickRenderControl m_renderControl;
    QQuickWindow m_window{ &m_renderControl };
    std::unique_ptr<QOpenGLFramebufferObject> m_pFbo;
    QOpenGLContext m_openGLContext;
    QOffscreenSurface m_offscreenSurface;

    QTimer m_pumpEventsTimer;
    std::unique_ptr<QTimer> m_pRenderTimer;
    bool m_dashboardVisible = false;

    QPoint m_ptLastMouse;
    Qt::MouseButtons m_lastMouseButtons = nullptr;

    bool m_desktopMode;
    bool m_noSound;
    bool m_newVersionDetected = false;
    int m_remoteVersionMajor = -1;
    int m_remoteVersionMinor = -1;
    int m_remoteVersionPatch = -1;
    int m_localVersionMajor = -1;
    int m_localVersionMinor = -1;
    int m_localVersionPatch = -1;
    QString m_updateMessage = "";
    QString m_optionalMessage = "";
    QString m_versionCheckText = "";

    QUrl m_runtimePathUrl;

    utils::ChaperoneUtils m_chaperoneUtils;

    QSoundEffect m_activationSoundEffect;
    QSoundEffect m_focusChangedSoundEffect;
    QSoundEffect m_alarm01SoundEffect;

    uint64_t m_currentFrame = 0;
    uint64_t m_lastFrame = 0;
    int m_vsyncTooLateCounter = 0;
    int m_customTickRateCounter = 0;
    int m_verifiedCustomTickRateMs = 0;

    input::SteamIVRInput m_actions;

    alarm_clock::VrAlarm m_alarm;

    QNetworkAccessManager* netManager = new QNetworkAccessManager( this );
    QJsonDocument m_remoteVersionJsonDocument = QJsonDocument();
    QJsonObject m_remoteVersionJsonObject;

public: // I know it's an ugly hack to make them public to enable external
        // access, but I am too lazy to implement getters.
    SteamVRTabController m_steamVRTabController;
    ChaperoneTabController m_chaperoneTabController;
    MoveCenterTabController m_moveCenterTabController;
    FixFloorTabController m_fixFloorTabController;
    AudioTabController m_audioTabController;
    StatisticsTabController m_statisticsTabController;
    SettingsTabController m_settingsTabController;
    UtilitiesTabController m_utilitiesTabController;
    VideoTabController m_videoTabController;
    RotationTabController m_rotationTabController;
    bool m_incomingReset = false;

private:
    QPoint getMousePositionForEvent( vr::VREvent_Mouse_t mouse );
    void processInputBindings();
    void processMediaKeyBindings();
    void processMotionBindings();
    void processPushToTalkBindings();
    void processChaperoneBindings();
    void processKeyboardBindings();
    void processRotationBindings();
    void processExclusiveInputBinding();

    bool m_exclusiveState = false;
    bool m_keyPressOneState = false;
    bool m_keyPressTwoState = false;

public:
    OverlayController( bool desktopMode, bool noSound, QQmlEngine& qmlEngine );
    virtual ~OverlayController();

    void Shutdown();
    Q_INVOKABLE void exitApp();
    Q_INVOKABLE void setAutoChapProfileName( int index );

    bool isDashboardVisible()
    {
        return m_dashboardVisible;
    }

    void SetWidget( QQuickItem* quickItem,
                    const std::string& name,
                    const std::string& key = "" );

    void RotateUniverseCenter( vr::ETrackingUniverseOrigin universe,
                               float yAngle,
                               bool adjustBounds = true,
                               bool commit = true ); // around y axis
    void AddOffsetToCollisionBounds( unsigned axisId,
                                     float offset,
                                     bool commit = true );
    void AddOffsetToCollisionBounds( float offset[3], bool commit = true );
    void RotateCollisionBounds( float angle,
                                bool commit = true ); // around y axis

    bool isDesktopMode()
    {
        return m_desktopMode;
    }

    bool isPreviousShutdownSafe();
    void setPreviousShutdownSafe( bool value );

    utils::ChaperoneUtils& chaperoneUtils() noexcept
    {
        return m_chaperoneUtils;
    }

    Q_INVOKABLE QString getVersionString();
    Q_INVOKABLE QUrl getVRRuntimePathUrl();

    Q_INVOKABLE bool soundDisabled();

    const vr::VROverlayHandle_t& overlayHandle();
    const vr::VROverlayHandle_t& overlayThumbnailHandle();

    bool pollNextEvent( vr::VROverlayHandle_t ulOverlayHandle,
                        vr::VREvent_t* pEvent );
    void mainEventLoop();

    bool crashRecoveryDisabled() const;
    bool exclusiveInputEnabled() const;
    bool autoApplyChaperoneEnabled() const;
    bool enableDebug() const;
    bool disableVersionCheck() const;
    bool newVersionDetected() const;
    bool vsyncDisabled() const;
    QString versionCheckText() const;
    int customTickRateMs() const;
    int debugState() const;
    std::string autoApplyChaperoneName();

    double soundVolume() const;
    bool desktopModeToggle() const;

public slots:
    void renderOverlay();
    void OnRenderRequest();
    void OnTimeoutPumpEvents();
    void OnNetworkReply( QNetworkReply* reply );

    void showKeyboard( QString existingText, unsigned long userValue = 0 );
    void setKeyboardPos();

    void playActivationSound();
    void playFocusChangedSound();
    void playAlarm01Sound( bool loop = false );
    void setAlarm01SoundVolume( float vol );
    void cancelAlarm01Sound();

    void setCrashRecoveryDisabled( bool value, bool notify = true );
    void setExclusiveInputEnabled( bool value, bool notify = true );
    void setEnableDebug( bool value, bool notify = true );
    void setDisableVersionCheck( bool value, bool notify = true );
    void setNewVersionDetected( bool value, bool notify = true );
    void setVersionCheckText( QString value, bool notify = true );
    void setVsyncDisabled( bool value, bool notify = true );
    void setCustomTickRateMs( int value, bool notify = true );
    void setDebugState( int value, bool notify = true );
    void setAutoApplyChaperoneEnabled( bool value, bool notify = true );
    void setSoundVolume( double value, bool notify = true );
    void setDesktopModeToggle( bool value, bool notify = true );

signals:
    void keyBoardInputSignal( QString input, unsigned long userValue = 0 );
    void crashRecoveryDisabledChanged( bool value );
    void exclusiveInputEnabledChanged( bool value );
    void enableDebugChanged( bool value );
    void disableVersionCheckChanged( bool value );
    void newVersionDetectedChanged( bool value );
    void versionCheckTextChanged( QString value );
    void vsyncDisabledChanged( bool value );
    void customTickRateMsChanged( int value );
    void debugStateChanged( int value );
    void autoApplyChaperoneEnabledChanged( bool value );
    void soundVolumeChanged( double value );
    void desktopModeToggleChanged( bool value );
};

} // namespace advsettings
