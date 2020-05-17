#include "setup.h"
#ifdef ENABLE_DEBUG_LOGGING
constexpr auto debugLoggingEnabled = true;
#else
constexpr auto debugLoggingEnabled = false;
#endif

// The default Qt message handler prints to stdout on X11 and to the debugger on
// Windows. That is borderline useless for us, therefore we create our own
// message handler.
void mainQtMessageHandler( QtMsgType type,
                           const QMessageLogContext& context,
                           const QString& msg )
{
    // context contains null pointers on release builds. Cast to get rid of
    // unreferenced formal parameter warnings.
    static_cast<void>( context );

    QByteArray localMsg = msg.toLocal8Bit();
    switch ( type )
    {
    case QtDebugMsg:
        LOG( DEBUG ) << "QTMSGH: " << localMsg.constData();
        break;
    case QtInfoMsg:
        LOG( INFO ) << "QTMSGH: " << localMsg.constData();
        break;
    case QtWarningMsg:
        LOG( WARNING ) << "QTMSGH: " << localMsg.constData();
        break;
    case QtCriticalMsg:
        LOG( ERROR ) << "QTMSGH: " << localMsg.constData();
        break;
    case QtFatalMsg:
        LOG( FATAL ) << "QTMSGH: " << localMsg.constData();
        break;
    }
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
    const bool resetSettingsEnabled = parser.isSet( resetSettings );
    LOG_IF( resetSettingsEnabled, INFO ) << "Reset SteamVR Settings.";

    const CommandLineOptions commandLineArgs{
        desktopModeEnabled,         forceNoSoundEnabled,
        forceNoManifestEnabled,     forceInstallManifestEnabled,
        forceRemoveManifestEnabled, resetSettingsEnabled
    };

    LOG( INFO ) << "Command line arguments processed.";

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
        LOG( INFO ) << "Manifest Is already Installed for Key "
                    << application_strings::applicationKey;
    }
    else
    {
        const auto app_error = vr::VRApplications()->AddApplicationManifest(
            manifestPath.c_str() );
        LOG( INFO ) << "Manifest Installed at: " << manifestPath;
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
        LOG( INFO ) << "Attempting to Remove Manifest At:  " << manifestPath;
    }
    if ( vr::VRApplications()->IsApplicationInstalled(
             application_strings::applicationKey ) )
    {
        LOG( ERROR ) << "Removal Failed, run Advanced Settings from previous "
                        "install directory OR manually delete it in Steam "
                        "install at Steam\\config\\appconfig.json";
    }
    else
    {
        LOG( INFO ) << "Manifest Removed Successfully";
    }
}

// Removes and then installs the application manifest.
void reinstallApplicationManifest( const std::string manifestPath )
{
    if ( vr::VRApplications()->IsApplicationInstalled(
             application_strings::applicationKey ) )
    {
        LOG( INFO ) << "Manifest Previously Installed";
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
        LOG( INFO ) << "No Manifest Installed";
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
            LOG( INFO ) << "Manifest reinstalled.";
        }
        else if ( removeManifest )
        {
            forceRemoveApplicationManifest();
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

void setUpLogging()
{
    el::Loggers::addFlag( el::LoggingFlag::DisableApplicationAbortOnFatalLog );

    el::Configurations conf;
    conf.setToDefault();

    using el::ConfigurationType;
    using el::Level;

    conf.set( Level::Global,
              ConfigurationType::Format,
              "[%level] %datetime{%Y-%M-%d %H:%m:%s}: %msg" );
    constexpr auto logFileName = "AdvancedSettings.log";
    conf.set( Level::Global, ConfigurationType::Filename, logFileName );

    constexpr auto confEnabled = "true";
    conf.set( Level::Global, ConfigurationType::Enabled, confEnabled );
    conf.set( Level::Global, ConfigurationType::ToFile, confEnabled );
    conf.set( Level::Global, ConfigurationType::ToStandardOutput, confEnabled );

    constexpr auto TwoMegabytesInBytes = "2097152";
    conf.set(
        Level::Global, ConfigurationType::MaxLogFileSize, TwoMegabytesInBytes );

    constexpr auto confDisabled = "false";
    conf.set( Level::Trace, ConfigurationType::Enabled, confDisabled );

    if constexpr ( debugLoggingEnabled )
    {
        conf.set( Level::Debug, ConfigurationType::Enabled, confEnabled );
    }
    else
    {
        conf.set( Level::Debug, ConfigurationType::Enabled, confDisabled );
    }

    const auto appDataLocation
        = std::string( "/" ) + application_strings::applicationOrganizationName
          + "/";
    const auto logFilePath
        = QDir( QString::fromStdString( *paths::settingsDirectory() )
                + appDataLocation.c_str() )
              .absoluteFilePath( logFileName );
    conf.set( el::Level::Global,
              el::ConfigurationType::Filename,
              QDir::toNativeSeparators( logFilePath ).toStdString() );

    conf.setRemainingToDefault();

    el::Loggers::reconfigureAllLoggers( conf );

    LOG( INFO ) << "Application started (Version "
                << application_strings::applicationVersionString << ")";
    LOG( INFO ) << "Log File: " << logFilePath;
}
