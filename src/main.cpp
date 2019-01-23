#include "overlaycontroller.h"
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

INITIALIZE_EASYLOGGINGPP

enum ReturnErrorCode
{
    SUCCESS = 0,
    GENERAL_FAILURE = -1,
    OPENVR_INIT_ERROR = -2,
};

namespace argument
{
// Checks whether a specific string was passed as a launch argument.
bool CheckCommandLineArgument( int argc,
                               char* argv[],
                               const std::string parameter )
{
    // The old way was one giant loop that tested every single parameter.
    // Having an individual loop for all args most likely has a very small
    // performance penalty compared to the original solution, but the gains in
    // readability and ease of extension of not having a hundred line+ for loop
    // are worth it.
    for ( int i = 0; i < argc; i++ )
    {
        if ( std::string( argv[i] ) == parameter )
        {
            return true;
        }
    }
    return false;
}

constexpr auto kDesktopMode = "-desktop";
constexpr auto kNoSound = "-nosound";
constexpr auto kNoManifest = "-nomanifest";
constexpr auto kInstallManifest = "-installmanifest";
constexpr auto kRemoveManifest = "-removemanifest";
constexpr auto kEnableBindingsInterface = "-bindingsinterface";
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
// program is called with "-installmanifest" by the installer) and every time
// the program is launched without both "-desktop" and "-nomanifest".
// The OpenVR initialization is necessary for both removing and installing
// manifests.
[[noreturn]] void handleManifests( const bool installManifest,
                                   const bool removeManifest )
{
    int exit_code = ReturnErrorCode::SUCCESS;
    auto openvr_init_status = vr::VRInitError_None;
    vr::VR_Init( &openvr_init_status, vr::VRApplication_Utility );
    if ( openvr_init_status == vr::VRInitError_None )
    {
        try
        {
            const auto manifestPath = QDir::cleanPath(
                QDir( QCoreApplication::applicationDirPath() )
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
    }
    else
    {
        exit_code = ReturnErrorCode::OPENVR_INIT_ERROR;
        LOG( ERROR ) << std::string(
            "Failed to initialize OpenVR: "
            + std::string( vr::VR_GetVRInitErrorAsEnglishDescription(
                  openvr_init_status ) ) );
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

int main( int argc, char* argv[] )
{
    setUpLogging( argc, argv );

    // Initializers in if statements is a C++17 feature. Should be removed after
    // making sure CI works with this.
    if ( constexpr int i = 2; i > 10 )
    {
        // static_assert with no message is a C++17 feature. Should be removed
        // after making sure CI works with this.
        static_assert( i < 5 );
    }

    const bool desktopMode = argument::CheckCommandLineArgument(
        argc, argv, argument::kDesktopMode );
    const bool noSound
        = argument::CheckCommandLineArgument( argc, argv, argument::kNoSound );
    const bool noManifest = argument::CheckCommandLineArgument(
        argc, argv, argument::kNoManifest );
    const bool installManifest = argument::CheckCommandLineArgument(
        argc, argv, argument::kInstallManifest );
    const bool removeManifest = argument::CheckCommandLineArgument(
        argc, argv, argument::kRemoveManifest );
    const bool bindingsInterface = argument::CheckCommandLineArgument(
        argc, argv, argument::kEnableBindingsInterface );

    // If a command line arg is set, make sure the logs reflect that.
    LOG_IF( desktopMode, INFO ) << "Desktop mode enabled.";
    LOG_IF( noSound, INFO ) << "Sound effects disabled.";
    LOG_IF( noManifest, INFO ) << "vrmanifest disabled.";
    LOG_IF( installManifest, INFO ) << "Install manifest enabled.";
    LOG_IF( removeManifest, INFO ) << "Remove manifest enabled.";
    LOG_IF( bindingsInterface, INFO ) << "Bindings interface enabled.";

    if ( bindingsInterface )
    {
        // This is a temporary solution until Valve fixes overlays not
        // showing up in the bindings interface. See function description for
        // more info.
        openvr_init::openBindingsMenu();
    }

    // It is important that either install_manifest or remove_manifest are true,
    // otherwise the handleManifests function will not behave properly.
    if ( installManifest || removeManifest )
    {
        // The function does not return, it exits inside the function
        // with an appropriate exit code.
        manifest::handleManifests( installManifest, removeManifest );
    }

    try
    {
        // QCoreApplication (QApplication inherits it.) can parse the args from
        // argc and argv. We don't use this since it's relatively slow and we
        // need the args before the main Qt stuff happens. If the arg parsing
        // needs of the program grow it could be useful to utilize the Qt arg
        // parsing.
        MyQApplication mainEventLoop( argc, argv );
        mainEventLoop.setOrganizationName(
            advsettings::OverlayController::applicationOrganizationName );
        mainEventLoop.setApplicationName(
            advsettings::OverlayController::applicationName );
        mainEventLoop.setApplicationDisplayName(
            advsettings::OverlayController::applicationDisplayName );
        mainEventLoop.setApplicationVersion(
            advsettings::OverlayController::applicationVersionString );

        qInstallMessageHandler( mainQtMessageHandler );

        // QSettings contains a platform independant way of storing settings in
        // an .ini file.
        QSettings appSettings( QSettings::IniFormat,
                               QSettings::UserScope,
                               mainEventLoop.organizationName(),
                               mainEventLoop.applicationName() );
        advsettings::OverlayController::setAppSettings( &appSettings );
        LOG( INFO ) << "Settings File: "
                    << appSettings.fileName().toStdString();

        // The QML Engine is necessary for instantiating QML files.
        QQmlEngine qmlEngine;

        // The OverlayController handles the majority of the application specfic
        // things. It contains all the other tabs.
        advsettings::OverlayController controller(
            desktopMode, noSound, qmlEngine );

        QString path = QStandardPaths::locate(
            QStandardPaths::AppDataLocation,
            QStringLiteral( "res/qml/mainwidget.qml" ) );

        QQmlComponent component( &qmlEngine, QUrl::fromLocalFile( path ) );
        auto errors = component.errors();
        for ( auto& e : errors )
        {
            LOG( ERROR ) << "QML Error: " << e.toString().toStdString()
                         << std::endl;
        }
        auto quickObj = component.create();
        controller.SetWidget(
            qobject_cast<QQuickItem*>( quickObj ),
            advsettings::OverlayController::applicationDisplayName,
            advsettings::OverlayController::applicationKey );

        // Attempts to install the application manifest on all "regular" starts.
        if ( !desktopMode && !noManifest )
        {
            try
            {
                const auto manifestPath = QDir::cleanPath(
                    QDir( QCoreApplication::applicationDirPath() )
                        .absoluteFilePath( manifest::kVRManifestName ) );
                manifest::installApplicationManifest( manifestPath );
            }
            catch ( std::exception& e )
            {
                LOG( ERROR ) << e.what();
            }
        }

        // Creates an identical settings menu on the desktop as well as in VR.
        if ( desktopMode )
        {
            auto m_pWindow = new QQuickWindow();
            qobject_cast<QQuickItem*>( quickObj )
                ->setParentItem( m_pWindow->contentItem() );
            m_pWindow->setGeometry(
                0,
                0,
                static_cast<int>(
                    qobject_cast<QQuickItem*>( quickObj )->width() ),
                static_cast<int>(
                    qobject_cast<QQuickItem*>( quickObj )->height() ) );
            m_pWindow->show();
        }

        return mainEventLoop.exec();
    }
    catch ( const std::exception& e )
    {
        LOG( FATAL ) << e.what();
        return ReturnErrorCode::GENERAL_FAILURE;
    }
}
