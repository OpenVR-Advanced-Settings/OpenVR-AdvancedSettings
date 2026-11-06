#include "setup.h"
#include "../settings/internal/settings_internal.h"
#include <QtLogging>
#include <QtDebug>
#include <QDateTime>
#ifdef ENABLE_DEBUG_LOGGING
constexpr auto debugLoggingEnabled = true;
#else
constexpr auto debugLoggingEnabled = false;
#endif

QtMessageHandler originalMessageHandler = nullptr;
static FILE* f;

// Maps a QtMsgType to its single-letter tag (D/I/W/C/F) so we can format the
// leading bracket in mainQtMessageHandler.
static QString severityTag( QtMsgType type )
{
    switch ( type )
    {
    case QtDebugMsg:     return "D";
    case QtInfoMsg:      return "I";
    case QtWarningMsg:   return "W";
    case QtCriticalMsg:  return "C";
    case QtFatalMsg:     return "F";
    default:             return "?";
    }
}

// The default Qt message handler prints to stdout on X11 and to the debugger on
// Windows. That is borderline useless for us, therefore we create our own
// message handler.
void mainQtMessageHandler( QtMsgType type,
                            const QMessageLogContext& context,
                            const QString& msg )
{
    Q_UNUSED( context );
    const QString timestamp =
        QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss.zzz" );
    // Format: [Level] yyyy-MM-dd hh:mm:ss.zzz message
    const QString line
        = QString( "[%1] %2 %3" ).arg( severityTag( type ), timestamp, msg );
    if ( f )
    {
        fprintf( f, "%s\n", qPrintable( line ) );
        fflush( f );
    }
    if ( originalMessageHandler )
        ( *originalMessageHandler )( type, context, msg );
}

namespace argument
{
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

    QCommandLineOption resetSettings( k_resetSettings,
                                      k_resetSettingsDescription );
    parser.addOption( resetSettings );

    QCommandLineOption graphicsApi(
        k_graphicsApi, k_graphicsApiDescription, k_graphicsApiValueName );
    parser.addOption( graphicsApi );

    parser.process( application );

    const bool desktopModeEnabled = parser.isSet( desktopMode );
    if ( desktopModeEnabled )
        qInfo() << "Desktop mode enabled.";

    const bool forceNoSoundEnabled = parser.isSet( forceNoSound );
    if ( forceNoSoundEnabled )
        qInfo() << "Sound effects disabled.";

    const bool forceNoManifestEnabled = parser.isSet( forceNoManifest );
    if ( forceNoManifestEnabled )
        qInfo() << "Running without applications manifest.";

    const bool forceInstallManifestEnabled
        = parser.isSet( forceInstallManifest );
    if ( forceInstallManifestEnabled )
        qInfo() << "Forcing install of applications manifest.";

    const bool forceRemoveManifestEnabled = parser.isSet( forceRemoveManifest );
    if ( forceRemoveManifestEnabled )
        qInfo() << "Forcing removal of applications manifest.";
    const bool resetSettingsEnabled = parser.isSet( resetSettings );
    if ( resetSettingsEnabled )
        qInfo() << "Reset SteamVR Settings.";

    std::optional<QSGRendererInterface::GraphicsApi> graphicsApiValue;
    if ( parser.isSet( graphicsApi ) )
    {
        const QString value = parser.value( graphicsApi ).toLower();
        if ( value == "vulkan" )
            graphicsApiValue = QSGRendererInterface::Vulkan;
        if ( value == "opengl" )
            graphicsApiValue = QSGRendererInterface::OpenGL;
#ifdef _WIN32
        if ( value == "d3d11" )
            graphicsApiValue = QSGRendererInterface::Direct3D11;
#endif

        if ( graphicsApiValue )
            qInfo() << "Graphics API override:" << value;
        else
            qCritical() << "Unknown graphics API" << value
                        << ", using Qt's default.";
    }

    const CommandLineOptions commandLineArgs{
        desktopModeEnabled,         forceNoSoundEnabled,
        forceNoManifestEnabled,     forceInstallManifestEnabled,
        forceRemoveManifestEnabled, resetSettingsEnabled,
        graphicsApiValue
    };

    qInfo() << "Command line arguments processed.";

    return commandLineArgs;
}
} // namespace argument

namespace manifest
{
// Enables autostart of OpenVR Advanced Settings.
void enableApplicationAutostart()
{
    const auto app_error = vr::VRApplications()->SetApplicationAutoLaunch(
        application_strings::applicationKey, true );
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
void installApplicationManifest( const std::string manifestPath )
{
    if ( vr::VRApplications()->IsApplicationInstalled(
             application_strings::applicationKey ) )
    {
        qInfo() << "Manifest Is already Installed for Key "
                << application_strings::applicationKey;
    }
    else
    {
        const auto app_error = vr::VRApplications()->AddApplicationManifest(
            manifestPath.c_str() );
        qInfo() << "Manifest Installed at: " << manifestPath;
        if ( app_error != vr::VRApplicationError_None )
        {
            throw std::runtime_error(
                std::string( "Could not add application manifest: " )
                + std::string(
                    vr::VRApplications()->GetApplicationsErrorNameFromEnum(
                        app_error ) ) );
        }
    }
}

// Removes the application manifest.
void removeApplicationManifest( const std::string manifestPath )
{
    if ( vr::VRApplications()->IsApplicationInstalled(
             application_strings::applicationKey ) )
    {
        vr::VRApplications()->RemoveApplicationManifest( manifestPath.c_str() );
        qInfo() << "Attempting to Remove Manifest At:  " << manifestPath;
    }
    if ( vr::VRApplications()->IsApplicationInstalled(
             application_strings::applicationKey ) )
    {
        qCritical() << "Removal Failed, run Advanced Settings from previous "
                       "install directory OR manually delete it in Steam "
                       "install at Steam\\config\\appconfig.json";
    }
    else
    {
        qInfo() << "Manifest Removed Successfully";
    }
}

// Removes and then installs the application manifest.
void reinstallApplicationManifest( const std::string manifestPath )
{
    if ( vr::VRApplications()->IsApplicationInstalled(
             application_strings::applicationKey ) )
    {
        qInfo() << "Manifest Previously Installed";
        // String size was arbitrarily chosen by original author.
        constexpr auto kStringSize = 1024;
        char oldApplicationWorkingDir[kStringSize] = { 0 };
        auto app_error = vr::VRApplicationError_None;
        vr::VRApplications()->GetApplicationPropertyString(
            application_strings::applicationKey,
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
            = QDir::toNativeSeparators(
                  QDir::cleanPath( QDir( oldApplicationWorkingDir )
                                       .absoluteFilePath( kVRManifestName ) ) )
                  .toStdString();
        removeApplicationManifest( oldManifestPath );
    }

    installApplicationManifest( manifestPath );
}

void forceRemoveApplicationManifest()
{
    if ( vr::VRApplications()->IsApplicationInstalled(
             application_strings::applicationKey ) )
    {
        // String size was arbitrarily chosen by original author.
        constexpr auto kStringSize = 1024;
        char oldApplicationWorkingDir[kStringSize] = { 0 };
        auto app_error = vr::VRApplicationError_None;
        vr::VRApplications()->GetApplicationPropertyString(
            application_strings::applicationKey,
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
            = QDir::toNativeSeparators(
                  QDir::cleanPath( QDir( oldApplicationWorkingDir )
                                       .absoluteFilePath( kVRManifestName ) ) )
                  .toStdString();
        removeApplicationManifest( oldManifestPath );
    }
    else
    {
        qInfo() << "No Manifest Installed";
    }
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
            = paths::binaryDirectoryFindFile( manifest::kVRManifestName );

        if ( !manifestPath.has_value() )
        {
            throw std::runtime_error( "Could not find application manifest." );
        }
        if ( installManifest )
        {
            reinstallApplicationManifest( *manifestPath );
            enableApplicationAutostart();
            qInfo() << "Manifest reinstalled.";
        }
        else if ( removeManifest )
        {
            forceRemoveApplicationManifest();
            qInfo() << "Manifest removed.";
        }
    }
    catch ( std::exception& e )
    {
        exit_code = ReturnErrorCode::GENERAL_FAILURE;
        qCritical() << e.what();
    }

    vr::VR_Shutdown();
    exit( exit_code );
}
} // namespace manifest

void setUpLogging()
{
    const auto logPath
        = QFileInfo( settings::getQSettings().fileName() ).absolutePath()
          + "/AdvancedSettings.log";
    f = fopen( logPath.toStdString().c_str(), "a" );
    originalMessageHandler = qInstallMessageHandler( mainQtMessageHandler );
}
