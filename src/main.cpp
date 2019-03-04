#include "utils/setup.h"

INITIALIZE_EASYLOGGINGPP

int main( int argc, char* argv[] )
{
    setUpLogging( argc, argv );

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

    const auto commandLineArgs
        = argument::returnCommandLineParser( mainEventLoop );

    // It is important that either install_manifest or remove_manifest are true,
    // otherwise the handleManifests function will not behave properly.
    if ( commandLineArgs.forceInstallManifest
         || commandLineArgs.forceRemoveManifest )
    {
        // The function does not return, it exits inside the function
        // with an appropriate exit code.
        manifest::handleManifests( commandLineArgs.forceInstallManifest,
                                   commandLineArgs.forceRemoveManifest );
    }

    openvr_init::initializeOpenVR(
        openvr_init::OpenVrInitializationType::Overlay );

    try
    {
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
        advsettings::OverlayController controller( commandLineArgs.desktopMode,
                                                   commandLineArgs.forceNoSound,
                                                   qmlEngine );

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
        if ( !commandLineArgs.desktopMode && !commandLineArgs.forceNoManifest )
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
        if ( commandLineArgs.desktopMode )
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
