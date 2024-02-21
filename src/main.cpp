#include "utils/setup.h"
#include "settings/settings.h"
#include "openvr/ovr_settings_wrapper.h"
#ifdef _WIN64
#    include <windows.h>
extern "C" __declspec( dllexport ) DWORD NvOptimusEnablement = 0x00000001;
extern "C" __declspec( dllexport ) DWORD AmdPowerXpressRequestHighPerformance
    = 0x00000001;
#endif

INITIALIZE_EASYLOGGINGPP

int main( int argc, char* argv[] )
{
    setUpLogging();

    LOG( INFO ) << "Settings File: "
                << settings::initializeAndGetSettingsPath();

    LOG( INFO ) << settings::getSettingsAndValues();

    QCoreApplication::setAttribute( Qt::AA_Use96Dpi );
    QCoreApplication::setAttribute( Qt::AA_UseDesktopOpenGL );
    MyQApplication mainEventLoop( argc, argv );
    mainEventLoop.setOrganizationName(
        application_strings::applicationOrganizationName );
    mainEventLoop.setApplicationName( application_strings::applicationName );
    mainEventLoop.setApplicationDisplayName(
        application_strings::applicationDisplayName );
    mainEventLoop.setApplicationVersion(
        application_strings::applicationVersionString );

    qInstallMessageHandler( mainQtMessageHandler );

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

        advsettings::OverlayController controller( commandLineArgs.desktopMode,
                                                   commandLineArgs.forceNoSound,
                                                   qmlEngine );

        constexpr auto widgetPath = "res/qml/common/mainwidget.qml";
        const auto path = paths::binaryDirectoryFindFile( widgetPath );

        if ( !path.has_value() )
        {
            LOG( ERROR ) << "Unable to find file '" << widgetPath << "'.";
            throw std::runtime_error(
                "Unable to find critical file. See log for more information." );
        }

        const auto url
            = QUrl::fromLocalFile( QString::fromStdString( ( *path ) ) );

        QQmlComponent component( &qmlEngine, url );
        auto errors = component.errors();
        for ( auto& e : errors )
        {
            LOG( ERROR ) << "QML Error: " << e.toString().toStdString()
                         << std::endl;
        }
        auto quickObj = component.create();
        controller.SetWidget( qobject_cast<QQuickItem*>( quickObj ),
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
                LOG( ERROR ) << e.what();
            }
        }

        if ( commandLineArgs.desktopMode
             || settings::getSetting(
                 settings::BoolSetting::APPLICATION_desktopModeToggle ) )
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
        if ( commandLineArgs.resetSettings )
        {
            // resets all settings if called
            ovr_settings_wrapper::resetAllSettings();
        }

        return mainEventLoop.exec();
    }
    catch ( const std::exception& e )
    {
        LOG( FATAL ) << e.what();
        return ReturnErrorCode::GENERAL_FAILURE;
    }
}
