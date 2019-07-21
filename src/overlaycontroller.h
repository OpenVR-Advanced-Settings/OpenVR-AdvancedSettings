
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
#include "tabcontrollers/ReviveTabController.h"
#include "tabcontrollers/UtilitiesTabController.h"
#include "tabcontrollers/VideoTabController.h"

#include "openvr/ivrinput.h"
#include "openvr/overlay_utils.h"

namespace application_strings
{
constexpr auto applicationOrganizationName = "AdvancedSettings-Team";
constexpr auto applicationName = "OpenVRAdvancedSettings";
constexpr const char* applicationKey = "OVRAS-Team.AdvancedSettings";
constexpr const char* applicationDisplayName = "Advanced Settings";

constexpr const char* applicationVersionString = APPLICATION_VERSION;

} // namespace application_strings

// application namespace
namespace advsettings
{
// These counters set timing for refreshing settings changes in tab ui
// SettingsUpdateCounter values are set as prime numbers to reduce overlap
// of simultaneous settings updates.
// Actual rates of updates are counter * vsync (~11ms)
// Values chosen based on update speed priority
// Avoid setting values to the same numbers.
constexpr int k_audioSettingsUpdateCounter = 89;
constexpr int k_chaperoneSettingsUpdateCounter = 101;
constexpr int k_moveCenterSettingsUpdateCounter = 83;
constexpr int k_reviveSettingsUpdateCounter = 139;
constexpr int k_settingsTabSettingsUpdateCounter = 157;
constexpr int k_steamVrSettingsUpdateCounter = 97;
constexpr int k_utilitiesSettingsUpdateCounter = 19;
// k_nonVsyncTickRate determines number of ms we wait to force the next event
// loop tick when vsync is too late due to dropped frames.
constexpr int k_nonVsyncTickRate = 20;
constexpr int k_maxCustomTickRate = 999;
constexpr int k_hmdRotationCounterUpdateRate = 7;

class OverlayController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool m_desktopMode READ isDesktopMode )
    Q_PROPERTY( bool vsyncDisabled READ vsyncDisabled WRITE setVsyncDisabled
                    NOTIFY vsyncDisabledChanged )
    Q_PROPERTY( int customTickRateMs READ customTickRateMs WRITE
                    setCustomTickRateMs NOTIFY customTickRateMsChanged )

private:
    vr::VROverlayHandle_t m_ulOverlayHandle = vr::k_ulOverlayHandleInvalid;
    vr::VROverlayHandle_t m_ulOverlayThumbnailHandle
        = vr::k_ulOverlayHandleInvalid;

    std::unique_ptr<QQuickRenderControl> m_pRenderControl;
    std::unique_ptr<QQuickWindow> m_pWindow;
    std::unique_ptr<QOpenGLFramebufferObject> m_pFbo;
    std::unique_ptr<QOpenGLContext> m_pOpenGLContext;
    std::unique_ptr<QOffscreenSurface> m_pOffscreenSurface;

    std::unique_ptr<QTimer> m_pPumpEventsTimer;
    std::unique_ptr<QTimer> m_pRenderTimer;
    bool m_dashboardVisible = false;

    QPoint m_ptLastMouse;
    Qt::MouseButtons m_lastMouseButtons = nullptr;

    bool m_desktopMode;
    bool m_noSound;
    bool m_vsyncDisabled = false;
    int m_customTickRateMs = 20;

    QUrl m_runtimePathUrl;

    utils::ChaperoneUtils m_chaperoneUtils;

    QSoundEffect m_activationSoundEffect;
    QSoundEffect m_focusChangedSoundEffect;
    QSoundEffect m_alarm01SoundEffect;

    uint64_t m_currentFrame = 0;
    uint64_t m_lastFrame = 0;
    int m_vsyncTooLateCounter = 0;
    int m_customTickRateCounter = 0;

    input::SteamIVRInput m_actions;

public: // I know it's an ugly hack to make them public to enable external
        // access, but I am too lazy to implement getters.
    SteamVRTabController m_steamVRTabController;
    ChaperoneTabController m_chaperoneTabController;
    MoveCenterTabController m_moveCenterTabController;
    FixFloorTabController m_fixFloorTabController;
    AudioTabController m_audioTabController;
    StatisticsTabController m_statisticsTabController;
    SettingsTabController m_settingsTabController;
    ReviveTabController m_reviveTabController;
    UtilitiesTabController m_utilitiesTabController;
    VideoTabController m_videoTabController;
    overlay::DesktopOverlay m_desktopOverlay;

private:
    QPoint getMousePositionForEvent( vr::VREvent_Mouse_t mouse );
    void processInputBindings();
    void processMediaKeyBindings();
    void processMotionBindings();
    void processPushToTalkBindings();
    void processChaperoneBindings();
    void processKeyboardBindings();

public:
    OverlayController( bool desktopMode, bool noSound, QQmlEngine& qmlEngine );
    virtual ~OverlayController();

    void Shutdown();

    bool isDashboardVisible()
    {
        return m_dashboardVisible;
    }

    void SetWidget( QQuickItem* quickItem,
                    const std::string& name,
                    const std::string& key = "" );

    void AddOffsetToUniverseCenter( vr::ETrackingUniverseOrigin universe,
                                    unsigned axisId,
                                    float offset,
                                    bool adjustBounds = true,
                                    bool commit = true );
    void AddOffsetToUniverseCenter( vr::ETrackingUniverseOrigin universe,
                                    float offset[3],
                                    bool adjustBounds = true,
                                    bool commit = true );
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

    bool vsyncDisabled() const;
    int customTickRateMs() const;

public slots:
    void renderOverlay();
    void OnRenderRequest();
    void OnTimeoutPumpEvents();

    void showKeyboard( QString existingText, unsigned long userValue = 0 );

    void playActivationSound();
    void playFocusChangedSound();
    void playAlarm01Sound( bool loop = false );
    void setAlarm01SoundVolume( float vol );
    void cancelAlarm01Sound();

    void setVsyncDisabled( bool value, bool notify = true );
    void setCustomTickRateMs( int value, bool notify = true );

signals:
    void keyBoardInputSignal( QString input, unsigned long userValue = 0 );
    void vsyncDisabledChanged( bool value );
    void customTickRateMsChanged( int value );

private:
    static QSettings* _appSettings;

public:
    static QSettings* appSettings()
    {
        return _appSettings;
    }

    static void setAppSettings( QSettings* settings )
    {
        _appSettings = settings;
    }
};

} // namespace advsettings
