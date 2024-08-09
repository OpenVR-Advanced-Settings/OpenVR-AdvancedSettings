#include "openvr/openvr_init.h"
#include "overlaycontroller.h"
#include "utils/setup.h"
#include "settings/settings.h"
#include "openvr/ovr_settings_wrapper.h"

#include <QtLogging>
#include <QtDebug>

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
    QCoreApplication::setAttribute( Qt::AA_UseDesktopOpenGL );
    MyQApplication const mainEventLoop( argc, argv );
    mainEventLoop.setOrganizationName(
        application_strings::applicationOrganizationName );
    mainEventLoop.setApplicationName( application_strings::applicationName );
    mainEventLoop.setApplicationDisplayName(
        application_strings::applicationDisplayName );
    mainEventLoop.setApplicationVersion(
        application_strings::applicationVersionString );

    const auto commandLineArgs
        = argument::returnCommandLineParser( mainEventLoop );

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

        std::unique_ptr<advsettings::OverlayController> controller(
            new advsettings::OverlayController( commandLineArgs.desktopMode,
                                                commandLineArgs.forceNoSound,
                                                qmlEngine ) );

        const auto url
            = QUrl( "qrc:/qt/qml/AdvancedSettings/common/mainwidget.qml" );

        QQmlComponent component( &qmlEngine, url );
        auto errors = component.errors();
        for ( auto& err : errors )
        {
            qCritical() << "QML Error: "
                        << err.toString().toStdString(); //<< std::endl;
        }
        auto* quickObj = component.create();
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

                manifest::installApplicationManifest( *manifestPath );
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
            auto* m_pWindow = new QQuickWindow();
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
