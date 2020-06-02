#pragma once
#include "../overlaycontroller.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QStandardPaths>
#include <openvr.h>
#include <iostream>
#include <easylogging++.h>
#include "../openvr/openvr_init.h"

enum ReturnErrorCode
{
    SUCCESS = 0,
    GENERAL_FAILURE = -1,
    OPENVR_INIT_ERROR = -2,
    SETTING_INCORRECT_INDEX = -3,
};

struct CommandLineOptions
{
    const bool desktopMode = false;
    const bool forceNoSound = false;
    const bool forceNoManifest = false;
    const bool forceInstallManifest = false;
    const bool forceRemoveManifest = false;
    const bool resetSettings = false;
};

// Manages the programs control flow and main settings.
class MyQApplication : public QApplication
{
public:
    using QApplication::QApplication;

    // Intercept event calls and log them on exceptions.
    // From the official docs
    // https://doc.qt.io/qt-5/qcoreapplication.html#notify:
    // "Future direction:
    // This function will not be called for objects that live outside the main
    // thread in Qt 6. Applications that need that functionality should find
    // other solutions for their event inspection needs in the meantime. The
    // change may be extended to the main thread, causing this function to be
    // deprecated."
    // Should look into replacements for this function if Qt 6 ever rolls
    // around. There are multiple suggestions for other solutions in the
    // provided link.
    virtual bool notify( QObject* receiver, QEvent* event ) override
    {
        try
        {
            return QApplication::notify( receiver, event );
        }
        catch ( std::exception& e )
        {
            LOG( ERROR ) << "Exception thrown from an event handler: "
                         << e.what();
        }
        return false;
    }
};

void mainQtMessageHandler( QtMsgType type,
                           const QMessageLogContext& context,
                           const QString& msg );

namespace argument
{
constexpr auto k_desktopMode = "desktop-mode";
constexpr auto k_desktopModeDescription
    = "Shows the options panel on the desktop instead of in VR.";

constexpr auto k_forceNoSound = "force-no-sound";
constexpr auto k_forceNoSoundDescription
    = "Forces sounds off for the application.";

constexpr auto k_forceNoManifest = "force-no-manifest";
constexpr auto k_forceNoManifestDescription
    = "Runs the application without installing the applications manifest.";

constexpr auto k_forceInstallManifest = "force-install-manifest";
constexpr auto k_forceInstallManifestDescription
    = "Forces installing the applications manifest into SteamVR. Application "
      "will exit early.";

constexpr auto k_forceRemoveManifest = "force-remove-manifest";
constexpr auto k_forceRemoveManifestDescription
    = "Forces removing the applications manifest. Application will exit early.";
constexpr auto k_resetSettings = "reset-steamvr-settings";
constexpr auto k_resetSettingsDescription
    = "Resets all SteamVR values that can be modified in OVRAS to defaults.";

CommandLineOptions returnCommandLineParser( const MyQApplication& application );

} // namespace argument

namespace manifest
{
// File name of the manifest. Is placed in the same directory as the binary.
constexpr auto kVRManifestName = "manifest.vrmanifest";

void enableApplicationAutostart();
void installApplicationManifest( const std::string manifestPath );
void removeApplicationManifest( const std::string manifestPath );
void reinstallApplicationManifest( const std::string manifestPath );
void forceRemoveApplicationManifest();
[[noreturn]] void handleManifests( const bool installManifest,
                                   const bool removeManifest );

} // namespace manifest

void setUpLogging();
