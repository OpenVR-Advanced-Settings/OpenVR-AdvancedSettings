#pragma once
#include "../overlaycontroller.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QSettings>
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
};

struct CommandLineOptions
{
    const bool desktopMode = false;
    const bool forceNoSound = false;
    const bool forceNoManifest = false;
    const bool forceInstallManifest = false;
    const bool forceRemoveManifest = false;
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

// The default Qt message handler prints to stdout on X11 and to the debugger on
// Windows. That is borderline useless for us, therefore we create our own
// message handler.
void mainQtMessageHandler( QtMsgType type,
                           const QMessageLogContext& context,
                           const QString& msg )
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch ( type )
    {
    case QtDebugMsg:
        LOG( DEBUG ) << localMsg.constData() << " (" << context.file << ":"
                     << context.line << ")";
        break;
    case QtInfoMsg:
        LOG( INFO ) << localMsg.constData() << " (" << context.file << ":"
                    << context.line << ")";
        break;
    case QtWarningMsg:
        LOG( WARNING ) << localMsg.constData() << " (" << context.file << ":"
                       << context.line << ")";
        break;
    case QtCriticalMsg:
        LOG( ERROR ) << localMsg.constData() << " (" << context.file << ":"
                     << context.line << ")";
        break;
    case QtFatalMsg:
        LOG( FATAL ) << localMsg.constData() << " (" << context.file << ":"
                     << context.line << ")";
        break;
    }
}

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

CommandLineOptions returnCommandLineParser( const MyQApplication& application )
{
    QCommandLineParser parser;

    parser.setApplicationDescription(
        "Advanced Settings overlay for SteamVR." );
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption desktopMode( k_desktopMode, k_desktopModeDescription );
    parser.addOption( desktopMode );

    QCommandLineOption forceNoSound( k_forceNoSound,
                                     k_forceNoSoundDescription );
    parser.addOption( forceNoSound );

    QCommandLineOption forceNoManifest( k_forceNoManifest,
                                        k_forceNoManifestDescription );
    parser.addOption( forceNoManifest );

    QCommandLineOption forceInstallManifest(
        k_forceInstallManifest, k_forceInstallManifestDescription );
    parser.addOption( forceInstallManifest );

    QCommandLineOption forceRemoveManifest( k_forceRemoveManifest,
                                            k_forceRemoveManifestDescription );
    parser.addOption( forceRemoveManifest );

    parser.process( application );

    const bool desktopModeEnabled = parser.isSet( desktopMode );
    LOG_IF( desktopModeEnabled, INFO ) << "Desktop mode enabled.";

    const bool forceNoSoundEnabled = parser.isSet( forceNoSound );
    LOG_IF( forceNoSoundEnabled, INFO ) << "Sound effects disabled.";

    const bool forceNoManifestEnabled = parser.isSet( forceNoManifest );
    LOG_IF( forceNoManifestEnabled, INFO )
        << "Running without applications manifest.";

    const bool forceInstallManifestEnabled
        = parser.isSet( forceInstallManifest );
    LOG_IF( forceInstallManifestEnabled, INFO )
        << "Forcing install of applications manifest.";

    const bool forceRemoveManifestEnabled = parser.isSet( forceRemoveManifest );
    LOG_IF( forceRemoveManifestEnabled, INFO )
        << "Forcing removal of applications manifest.";

    const CommandLineOptions commandLineArgs{ desktopModeEnabled,
                                              forceNoSoundEnabled,
                                              forceNoManifestEnabled,
                                              forceInstallManifestEnabled,
                                              forceRemoveManifestEnabled };

    LOG( INFO ) << "Command line arguments processed.";

    return commandLineArgs;
}
} // namespace argument

namespace manifest
{
// File name of the manifest. Is placed in the same directory as the binary.
constexpr auto kVRManifestName = "manifest.vrmanifest";

// Enables autostart of OpenVR Advanced Settings.
void enableApplicationAutostart()
{
    const auto app_error = vr::VRApplications()->SetApplicationAutoLaunch(
        advsettings::OverlayController::applicationKey, true );
    if ( app_error != vr::VRApplicationError_None )
    {
        throw std::runtime_error(
            std::string( "Could not set auto start: " )
            + std::string(
                  vr::VRApplications()->GetApplicationsErrorNameFromEnum(
                      app_error ) ) );
    }
}

// Installs the application manifest. If a manifest with the same application
// key is already installed, nothing will happen.
// OpenVR must be initialized before calling this function.
void installApplicationManifest( const QString manifestPath )
{
    if ( vr::VRApplications()->IsApplicationInstalled(
             advsettings::OverlayController::applicationKey ) )
    {
        // We don't want to disrupt applications that are already installed.
        return;
    }

    const auto app_error = vr::VRApplications()->AddApplicationManifest(
        QDir::toNativeSeparators( manifestPath ).toStdString().c_str() );
    if ( app_error != vr::VRApplicationError_None )
    {
        throw std::runtime_error(
            std::string( "Could not add application manifest: " )
            + std::string(
                  vr::VRApplications()->GetApplicationsErrorNameFromEnum(
                      app_error ) ) );
    }
}

// Removes the application manifest.
void removeApplicationManifest( const QString manifestPath )
{
    if ( !QFile::exists( manifestPath ) )
    {
        throw std::runtime_error(
            std::string( "Could not find application manifest: " )
            + manifestPath.toStdString() );
    }

    if ( vr::VRApplications()->IsApplicationInstalled(
             advsettings::OverlayController::applicationKey ) )
    {
        vr::VRApplications()->RemoveApplicationManifest(
            QDir::toNativeSeparators( manifestPath ).toStdString().c_str() );
    }
}

// Removes and then installs the application manifest.
void reinstallApplicationManifest( const QString manifestPath )
{
    if ( !QFile::exists( manifestPath ) )
    {
        throw std::runtime_error(
            std::string( "Could not find application manifest: " )
            + manifestPath.toStdString() );
    }

    if ( vr::VRApplications()->IsApplicationInstalled(
             advsettings::OverlayController::applicationKey ) )
    {
        // String size was arbitrarily chosen by original author.
        constexpr auto kStringSize = 1024;
        char oldApplicationWorkingDir[kStringSize] = { 0 };
        auto app_error = vr::VRApplicationError_None;
        vr::VRApplications()->GetApplicationPropertyString(
            advsettings::OverlayController::applicationKey,
            vr::VRApplicationProperty_WorkingDirectory_String,
            oldApplicationWorkingDir,
            kStringSize,
            &app_error );

        if ( app_error != vr::VRApplicationError_None )
        {
            throw std::runtime_error(
                "Could not find working directory of already "
                "installed application: "
                + std::string(
                      vr::VRApplications()->GetApplicationsErrorNameFromEnum(
                          app_error ) ) );
        }

        const auto oldManifestPath
            = QDir::cleanPath( QDir( oldApplicationWorkingDir )
                                   .absoluteFilePath( kVRManifestName ) );

        removeApplicationManifest( manifestPath );
    }

    installApplicationManifest( manifestPath );
}

// Initializes OpenVR and calls the relevant manifest functions.
// The .vrmanifest is used by SteamVR however the exact functiontionality is not
// documented in the official documentation.
// The manifest is installed upon using the NSIS installer for windows (this
// program is called with "--force-install-manifest" by the installer) and every
// time the program is launched without both "--desktop-mode" and
// "--force-no-manifest". The OpenVR initialization is necessary for both
// removing and installing manifests.
[[noreturn]] void handleManifests( const bool installManifest,
                                   const bool removeManifest )
{
    openvr_init::initializeOpenVR(
        openvr_init::OpenVrInitializationType::Utility );

    int exit_code = ReturnErrorCode::SUCCESS;
    try
    {
        const auto manifestPath
            = QDir::cleanPath( QDir( QCoreApplication::applicationDirPath() )
                                   .absoluteFilePath( kVRManifestName ) );

        if ( installManifest )
        {
            reinstallApplicationManifest( manifestPath );
            enableApplicationAutostart();
            LOG( INFO ) << "Manifest reinstalled.";
        }
        else if ( removeManifest )
        {
            removeApplicationManifest( manifestPath );
            LOG( INFO ) << "Manifest removed.";
        }
    }
    catch ( std::exception& e )
    {
        exit_code = ReturnErrorCode::GENERAL_FAILURE;
        LOG( ERROR ) << e.what();
    }

    vr::VR_Shutdown();
    exit( exit_code );
}
} // namespace manifest

// Sets up the logging library and outputs startup logging data.
// argc and argv are necessary for the START_EASYLOGGINGPP() call.
void setUpLogging( int argc, char* argv[] )
{
    constexpr auto logConfigDefault
        = "* GLOBAL:\n"
          "	FORMAT = \"[%level] %datetime{%Y-%M-%d %H:%m:%s}: %msg\"\n"
          "	FILENAME = \"AdvancedSettings.log\"\n"
          "	ENABLED = true\n"
          "	TO_FILE = true\n"
          "	TO_STANDARD_OUTPUT = true\n"
          "	MAX_LOG_FILE_SIZE = 2097152 ## 2MB\n"
          "* TRACE:\n"
          "	ENABLED = false\n"
          "* DEBUG:\n"
          "	ENABLED = false\n";

    // This allows easylogging++ to parse command line arguments.
    // For a full list go here
    // https://github.com/zuhd-org/easyloggingpp#application-arguments
    // The parsed args are mostly setting the verbosity level or setting the
    // conf file.
    START_EASYLOGGINGPP( argc, argv );

    el::Loggers::addFlag( el::LoggingFlag::DisableApplicationAbortOnFatalLog );

    // If there is a default logging config file present, use that. If not, use
    // the default settings.
    constexpr auto logConfigFileName = "logging.conf";
    const auto logconfigfile
        = QFileInfo( logConfigFileName ).absoluteFilePath();
    el::Configurations conf;
    if ( QFile::exists( logconfigfile ) )
    {
        conf.parseFromFile( logconfigfile.toStdString() );
    }
    else
    {
        conf.parseFromText( logConfigDefault );
    }

    // This places the log file in
    // Roaming/AppData/matzman666/OpenVRAdvancedSettings/AdvancedSettings.log.
    // The log file placement has been broken since at least git tag "v2.7".
    // It was being placed in the working dir of the executable.
    // The change hasn't been documented anywhere, so it is likely that it was
    // unintentional. This fixes the probable regression until a new path is
    // decided on.
    constexpr auto appDataFolders = "/matzman666/OpenVRAdvancedSettings";
    const QString logFilePath = QDir( QStandardPaths::writableLocation(
                                          QStandardPaths::AppDataLocation )
                                      + appDataFolders )
                                    .absoluteFilePath( "AdvancedSettings.log" );
    conf.set( el::Level::Global,
              el::ConfigurationType::Filename,
              QDir::toNativeSeparators( logFilePath ).toStdString() );

    conf.setRemainingToDefault();

    el::Loggers::reconfigureAllLoggers( conf );

    LOG( INFO ) << "Application started (Version "
                << advsettings::OverlayController::applicationVersionString
                << ")";
    LOG( INFO ) << "Log Config: "
                << QDir::toNativeSeparators( logconfigfile ).toStdString();
    LOG( INFO ) << "Log File: " << logFilePath;
}
