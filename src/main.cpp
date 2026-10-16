#include "utils/setup.h"
#include "settings/settings.h"
#include "openvr/ovr_settings_wrapper.h"
#include "utils/translations.h"

#include <QtLogging>
#include <QtDebug>
#include <memory>

#ifdef _WIN64
#    include <windows.h>
extern "C" __declspec( dllexport ) DWORD NvOptimusEnablement = 0x00000001;
extern "C" __declspec( dllexport ) DWORD AmdPowerXpressRequestHighPerformance
    = 0x00000001;
#endif

int main( int argc, char* argv[] )
{
    setUpLogging();

    qInfo() << "Settings File: " << settings::initializeAndGetSettingsPath();

    qInfo() << settings::getSettingsAndValues();

    QCoreApplication::setAttribute( Qt::AA_Use96Dpi );
    //TODO check if Needed?
    //QCoreApplication::setAttribute( Qt::AA_UseDesktopOpenGL );
    MyQApplication mainEventLoop( argc, argv );
    mainEventLoop.setOrganizationName(
        application_strings::applicationOrganizationName );
    mainEventLoop.setApplicationName( application_strings::applicationName );
    mainEventLoop.setApplicationDisplayName(
        application_strings::applicationDisplayName );
    mainEventLoop.setApplicationVersion(
        application_strings::applicationVersionString );

    translations::initialize();

    const auto commandLineArgs
        = argument::returnCommandLineParser( mainEventLoop );


    //Picks Graphics API, defaulting to vulkan/d3d11 for linux/win
    if ( commandLineArgs.graphicsApi )
    {
        QQuickWindow::setGraphicsApi( *commandLineArgs.graphicsApi );
    }
#ifdef __linux__
    else
    {
        QQuickWindow::setGraphicsApi( QSGRendererInterface::Vulkan );
    }
#endif
#ifdef _WIN32
    else
    {
        QQuickWindow::setGraphicsApi( QSGRendererInterface::Direct3D11 );
    }
#endif

    // It is important that either install_manifest or remove_manifest are true,
    // otherwise the handleManifests function will not behave properly.
    if ( commandLineArgs.forceInstallManifest
         || commandLineArgs.forceRemoveManifest )
    {
        manifest::handleManifests( commandLineArgs.forceInstallManifest,
                                   commandLineArgs.forceRemoveManifest );
    }

    openvr_init::initializeOpenVR(
        openvr_init::OpenVrInitializationType::Overlay );
    try
    {
        QQmlEngine qmlEngine;

        // Just error checking to ensure QRC works properly
        qDebug() << "list of qmldirs";
        QDirIterator it( ":", QDirIterator::Subdirectories );
        while ( it.hasNext() )
        {
           qDebug() << it.next();
        }

        std::unique_ptr<advsettings::OverlayController> controller(
            new advsettings::OverlayController( commandLineArgs.desktopMode,
                                                commandLineArgs.forceNoSound,
                                                qmlEngine ) );
        //const auto url
        //    = QUrl( ":/qt/qml/AdvancedSettings/qml/common/mainwidget.qml" );
        QQmlComponent component( &qmlEngine, ":/qt/qml/AdvancedSettings/qml/common/mainwidget.qml" );
        auto errors = component.errors();
        for ( auto& e : errors )
        {
            qCritical() << "QML Error: "
                        << e.toString().toStdString(); //<< std::endl;
        }
        auto quickObj = component.create();
        controller->SetWidget( qobject_cast<QQuickItem*>( quickObj ),
                               application_strings::applicationDisplayName,
                               application_strings::applicationKey );

        // Attempts to install the application manifest on all "regular" starts.
        if ( !commandLineArgs.forceNoManifest )
        {
            try
            {
                const auto manifestPath = paths::binaryDirectoryFindFile(
                    manifest::kVRManifestName );
                if ( !manifestPath.has_value() )
                {
                    throw std::runtime_error(
                        "Could not find applications manifest." );
                }
                else
                {
                    manifest::installApplicationManifest( *manifestPath );
                }
            }
            catch ( std::exception& e )
            {
                qCritical() << e.what();
            }
        }

        if ( commandLineArgs.desktopMode
             || settings::getSetting(
                 settings::BoolSetting::APPLICATION_desktopModeToggle ) )
        {
            //Qrhi seems to work automatically, it will default to system default,
            //or if using the launch option it will move to correct rendering system
            qDebug() << "Desktop MODE!"; 
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
        if ( commandLineArgs.resetSettings )
        {
            // resets all settings if called
            ovr_settings_wrapper::resetAllSettings();
        }

        return mainEventLoop.exec();
    }
    catch ( const std::exception& e )
    {
        qFatal() << e.what();
        return ReturnErrorCode::GENERAL_FAILURE;
    }
}
