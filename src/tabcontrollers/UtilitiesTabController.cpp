#include "UtilitiesTabController.h"
#include <QQuickWindow>
#include <QApplication>
#include <QtLogging>
#include <QtDebug>
#include "../overlaycontroller.h"
#include "../keyboard_input/input_sender.h"
#include "../keyboard_input/keyboard_input.h"
#include "../settings/settings.h"
#include "../utils/update_rate.h"
#include "../media_keys/media_keys.h"
#include <openvr.h>
#include "openvr/ovr_overlay_wrapper.h"
#include "openvr/ovr_system_wrapper.h"
#include <qimage.h>

// application namespace
namespace advsettings
{
void UtilitiesTabController::initStage2( OverlayController* var_parent )
{
    this->m_parent = var_parent;
}

void UtilitiesTabController::sendKeyboardInput( QString input )
{
    keyboardinput::sendKeyboardInput( input );
}

void UtilitiesTabController::sendKeyboardEnter()
{
    keyboardinput::sendKeyboardEnter();
}

void UtilitiesTabController::sendKeyboardAltEnter()
{
    keyboardinput::sendKeyboardAltEnter();
}

void UtilitiesTabController::sendKeyboardAltTab()
{
    keyboardinput::sendKeyboardAltTab();
}

void UtilitiesTabController::sendKeyboardCtrlC()
{
    keyboardinput::sendKeyboardCtrlC();
}

void UtilitiesTabController::sendKeyboardCtrlV()
{
    keyboardinput::sendKeyboardCtrlV();
}

void UtilitiesTabController::sendKeyboardBackspace( const int count )
{
    keyboardinput::sendKeyboardBackspace( count );
}

void UtilitiesTabController::sendKeyboardRShiftTilde1()
{
    keyboardinput::sendKeyboardRShiftTilde1();
}

void UtilitiesTabController::sendKeyboardRShiftTilde2()
{
    keyboardinput::sendKeyboardRShiftTilde2();
}

void UtilitiesTabController::sendKeyboardRShiftTilde3()
{
    keyboardinput::sendKeyboardRShiftTilde3();
}

void UtilitiesTabController::sendKeyboardRShiftTilde4()
{
    keyboardinput::sendKeyboardRShiftTilde4();
}

void UtilitiesTabController::sendKeyboardRShiftTilde5()
{
    keyboardinput::sendKeyboardRShiftTilde5();
}

void UtilitiesTabController::sendKeyboardRShiftTilde6()
{
    keyboardinput::sendKeyboardRShiftTilde6();
}

void UtilitiesTabController::sendKeyboardRShiftTilde7()
{
    keyboardinput::sendKeyboardRShiftTilde7();
}

void UtilitiesTabController::sendKeyboardRShiftTilde8()
{
    keyboardinput::sendKeyboardRShiftTilde8();
}

void UtilitiesTabController::sendKeyboardRShiftTilde9()
{
    keyboardinput::sendKeyboardRShiftTilde9();
}

void UtilitiesTabController::sendKeyboardRShiftTilde0()
{
    keyboardinput::sendKeyboardRShiftTilde0();
}

void UtilitiesTabController::sendKeyboardRShiftTilde1Delayed()
{
    keyboardinput::sendKeyboardRShiftTilde1Delayed();
}

void UtilitiesTabController::sendKeyboardRShiftTilde2Delayed()
{
    keyboardinput::sendKeyboardRShiftTilde2Delayed();
}

void UtilitiesTabController::sendKeyboardRShiftTilde3Delayed()
{
    keyboardinput::sendKeyboardRShiftTilde3Delayed();
}

void UtilitiesTabController::sendKeyboardRShiftTilde4Delayed()
{
    keyboardinput::sendKeyboardRShiftTilde4Delayed();
}

void UtilitiesTabController::sendKeyboardRShiftTilde5Delayed()
{
    keyboardinput::sendKeyboardRShiftTilde5Delayed();
}

void UtilitiesTabController::sendKeyboardRShiftTilde6Delayed()
{
    keyboardinput::sendKeyboardRShiftTilde6Delayed();
}

void UtilitiesTabController::sendKeyboardRShiftTilde7Delayed()
{
    keyboardinput::sendKeyboardRShiftTilde7Delayed();
}

void UtilitiesTabController::sendKeyboardRShiftTilde8Delayed()
{
    keyboardinput::sendKeyboardRShiftTilde8Delayed();
}

void UtilitiesTabController::sendKeyboardRShiftTilde9Delayed()
{
    keyboardinput::sendKeyboardRShiftTilde9Delayed();
}

void UtilitiesTabController::sendKeyboardRShiftTilde0Delayed()
{
    keyboardinput::sendKeyboardRShiftTilde0Delayed();
}

void UtilitiesTabController::sendMediaNextSong()
{
    keyboardinput::sendMediaNextSong();
}
void UtilitiesTabController::sendMediaPreviousSong()
{
    keyboardinput::sendMediaPreviousSong();
}
void UtilitiesTabController::sendMediaPausePlay()
{
    keyboardinput::sendMediaPausePlay();
}
void UtilitiesTabController::sendMediaStopSong()
{
    keyboardinput::sendMediaStopSong();
}

void UtilitiesTabController::sendKeyboardOne()
{
    const auto commands = settings::getSetting(
        settings::StringSetting::KEYBOARDSHORTCUT_keyboardOne );

    sendStringAsInput( commands );
}
void UtilitiesTabController::sendKeyboardTwo()
{
    const auto commands = settings::getSetting(
        settings::StringSetting::KEYBOARDSHORTCUT_keyboardTwo );

    sendStringAsInput( commands );
}
void UtilitiesTabController::sendKeyboardThree()
{
    const auto commands = settings::getSetting(
        settings::StringSetting::KEYBOARDSHORTCUT_keyboardThree );

    sendStringAsInput( commands );
}

bool UtilitiesTabController::vrcDebug() const
{
    return settings::getSetting( settings::BoolSetting::UTILITY_vrcDebug );
}

void UtilitiesTabController::setVrcDebug( bool value, bool notify )
{
    settings::setSetting( settings::BoolSetting::UTILITY_vrcDebug, value );
    if ( notify )
    {
        emit vrcDebugChanged( value );
    }
}

bool UtilitiesTabController::trackerOvlEnabled() const
{
    return settings::getSetting(
        settings::BoolSetting::UTILITY_trackerOverlayEnabled );
}

void UtilitiesTabController::setTrackerOvlEnabled( bool value, bool notify )
{
    settings::setSetting( settings::BoolSetting::UTILITY_trackerOverlayEnabled,
                          value );
    if ( notify )
    {
        emit trackerOvlEnabledChanged( value );
    }
    if ( !value )
    {
        destroyBatteryOverlays();
    }
}

QString getBatteryIconPath( int batteryState )
{
    constexpr auto batteryPrefix = "/res/img/battery/battery_";

    const auto batteryFullPath
        = ( batteryPrefix + QString::number( batteryState ) + ".png" )
              .toStdString();

    const auto batteryPath = paths::binaryDirectoryFindFile( batteryFullPath );

    if ( !batteryPath.has_value() )
    {
        qCritical() << "Unable to find battery image number '" << batteryState
                    << "'.";
        return "";
    }

    return QString::fromStdString( *batteryPath );
}

vr::VROverlayHandle_t UtilitiesTabController::createBatteryOverlay(
    vr::TrackedDeviceIndex_t index,
    BatteryOverlayStyle style )
{
    vr::VROverlayHandle_t handle = vr::k_ulOverlayHandleInvalid;
    std::string const batteryKey
        = std::string( application_strings::applicationKey ) + ".battery."
          + std::to_string( index );
    vr::VROverlayError const overlayError = vr::VROverlay()->CreateOverlay(
        batteryKey.c_str(), batteryKey.c_str(), &handle );
    if ( overlayError == vr::VROverlayError_None )
    {
        m_batteryImgs[0] = std::unique_ptr<QImage>(
            new QImage( QString( ":/battery/battery_0.png" ) ) );
        m_batteryImgs[1] = std::unique_ptr<QImage>(
            new QImage( QString( ":/battery/battery_1.png" ) ) );
        m_batteryImgs[2] = std::unique_ptr<QImage>(
            new QImage( QString( ":/battery/battery_2.png" ) ) );
        m_batteryImgs[3] = std::unique_ptr<QImage>(
            new QImage( QString( ":/battery/battery_3.png" ) ) );
        m_batteryImgs[4] = std::unique_ptr<QImage>(
            new QImage( QString( ":/battery/battery_4.png" ) ) );
        m_batteryImgs[5] = std::unique_ptr<QImage>(
            new QImage( QString( ":/battery/battery_5.png" ) ) );
        ovr_overlay_wrapper::setOverlayFromQImage( handle, *m_batteryImgs[0] );
        vr::VROverlay()->SetOverlayWidthInMeters( handle, 0.045f );
        vr::HmdMatrix34_t notificationTransform;
        if ( style == BatteryOverlayStyle_Tundra )
        {
            notificationTransform = { { { 1.0f, 0.0f, 0.0f, 0.00f },
                                        { 0.0f, -1.0f, 0.0f, 0.0081f },
                                        { 0.0f, 0.0f, -1.0f, -0.028f } } };
        }
        else
        {
            notificationTransform = { { { 1.0f, 0.0f, 0.0f, 0.00f },
                                        { 0.0f, -1.0f, 0.0f, 0.0081f },
                                        { 0.0f, 0.0f, -1.0f, -0.0178f } } };
        }
        vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(
            handle, index, &notificationTransform );
        qInfo() << "Created battery overlay for device " << index;
    }
    else
    {
        qCritical() << "Could not create ptt notification overlay: "
                    << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                           overlayError );
    }

    return handle;
}

void UtilitiesTabController::destroyBatteryOverlays()
{
    for ( auto& boh : m_batteryOverlayHandles )
    {
        if ( boh == 0 )
        {
            continue;
        }
        auto err = vr::VROverlay()->DestroyOverlay( boh );
        boh = 0;
        if ( err != vr::VROverlayError_None )
        {
            qCritical() << "Could not Delete Battery Overlay: "
                        << vr::VROverlay()->GetOverlayErrorNameFromEnum( err );
        }
    }
}

void UtilitiesTabController::handleTrackerBatOvl()
{
    // attach battery overlay to all tracked devices that aren't a
    // controller or hmd
    // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
    int const max = vr::k_unMaxTrackedDeviceCount;
    // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
    for ( int index = 0; index < max; index++ )
    {
        vr::ETrackedDeviceClass const deviceClass
            = vr::VRSystem()->GetTrackedDeviceClass( index );
        if ( deviceClass
             == vr::ETrackedDeviceClass::TrackedDeviceClass_GenericTracker )
        {
            if ( m_batteryOverlayHandles[index] == 0 )
            {
                auto type = ovr_system_wrapper::getStringTrackedProperty(
                    index, vr::Prop_RenderModelName_String );
                auto style
                    = type.first == ovr_system_wrapper::SystemError::NoError
                              && type.second.find( "tundra" )
                                     != std::string::npos
                          ? BatteryOverlayStyle_Tundra
                          : BatteryOverlayStyle_Default;

                qInfo() << "Creating battery overlay for device " << index;
                m_batteryOverlayHandles[index]
                    = createBatteryOverlay( index, style );
                m_batteryVisible[index] = true;
            }

            bool const shouldShow = vr::VROverlay()->IsDashboardVisible();

            if ( shouldShow != m_batteryVisible[index] )
            {
                if ( shouldShow )
                {
                    vr::VROverlay()->ShowOverlay(
                        m_batteryOverlayHandles[index] );
                }
                else
                {
                    vr::VROverlay()->HideOverlay(
                        m_batteryOverlayHandles[index] );
                }
                m_batteryVisible[index] = shouldShow;
            }

            bool const hasBatteryStatus
                = vr::VRSystem()->GetBoolTrackedDeviceProperty(
                    index,
                    vr::ETrackedDeviceProperty::
                        Prop_DeviceProvidesBatteryStatus_Bool );
            if ( hasBatteryStatus )
            {
                float const battery
                    = vr::VRSystem()->GetFloatTrackedDeviceProperty(
                        index,
                        vr::ETrackedDeviceProperty::
                            Prop_DeviceBatteryPercentage_Float );
                int const batteryState = static_cast<int>(
                    ceil( static_cast<double>( battery * 5 ) ) );

                if ( batteryState != m_batteryState[index] )
                {
                    qInfo()
                        << "Updating battery overlay for device " << index
                        << " to " << batteryState << "(" << battery << ")"
                        << QString::number( m_batteryOverlayHandles[index] );
                    ovr_overlay_wrapper::setOverlayFromQImage(
                        m_batteryOverlayHandles[index],
                        *m_batteryImgs[batteryState] );
                    m_batteryState[index] = batteryState;
                }
            }
        }
    }
}

void UtilitiesTabController::eventLoopTick()
{
    if ( updateRate.shouldSubjectNotRun(
             UpdateSubject::UtilitiesTabController ) )
    {
        return;
    }
    if ( trackerOvlEnabled() )
    {
        handleTrackerBatOvl();
    }
}

} // namespace advsettings
