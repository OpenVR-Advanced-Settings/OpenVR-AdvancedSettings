#include "utils/setup.h"

INITIALIZE_EASYLOGGINGPP

int main( int argc, char* argv[] )
{
    setUpLogging( argc, argv );

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
            QStringLiteral( "res/qml/common/mainwidget.qml" ) );

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
