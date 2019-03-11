#include "setup.h"

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
void installApplicationManifest( const QString manifestPath )
{
    if ( vr::VRApplications()->IsApplicationInstalled(
             application_strings::applicationKey ) )
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
             application_strings::applicationKey ) )
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
    conf.set( Level::Debug, ConfigurationType::Enabled, confDisabled );

    const auto appDataLocation
        = std::string( "/" ) + application_strings::applicationOrganizationName
          + "/";
    const auto logFilePath = QDir( QStandardPaths::writableLocation(
                                       QStandardPaths::AppDataLocation )
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
