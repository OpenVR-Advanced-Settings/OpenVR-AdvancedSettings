#include "VideoTabController.h"
#include <QQuickWindow>
#include <QApplication>
#include "../settings/settings.h"
#include "../overlaycontroller.h"
#include <cmath>

namespace advsettings
{
void VideoTabController::initStage1()
{
    // In order to ensure gain is "normal" before applying to either overlay or
    // gain.

    synchGain( true );
    resetGain();

    m_videoDashboardUpdateCounter
        = utils::adjustUpdateRate( k_videoDashboardUpdateCounter );

    initBrightnessOverlay();
    initColorOverlay();
    initMotionSmoothing();
    initSupersampleOverride();
    m_overlayInit = true;
    reloadVideoConfig();
    reloadVideoProfiles();

    if ( brightnessEnabled() )
    {
        setBrightnessEnabled( true, false, true );
    }
}

void VideoTabController::initBrightnessOverlay()
{
    std::string notifKey = std::string( application_strings::applicationKey )
                           + ".brightnessoverlay";

    vr::VROverlayError overlayError = vr::VROverlay()->CreateOverlay(
        notifKey.c_str(), notifKey.c_str(), &m_brightnessOverlayHandle );
    if ( overlayError == vr::VROverlayError_None )
    {
        const auto notifIconPath = paths::binaryDirectoryFindFile(
            video_keys::k_brightnessOverlayFilename );
        if ( notifIconPath.has_value() )
        {
            vr::VROverlay()->SetOverlayFromFile( m_brightnessOverlayHandle,
                                                 notifIconPath->c_str() );
            vr::VROverlay()->SetOverlayWidthInMeters( m_brightnessOverlayHandle,
                                                      k_overlayWidth );
            vr::HmdMatrix34_t notificationTransform
                = { { { 1.0f, 0.0f, 0.0f, 0.00f },
                      { 0.0f, 1.0f, 0.0f, 0.00f },
                      { 0.0f, 0.0f, 1.0f, k_hmdDistance } } };
            vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(
                m_brightnessOverlayHandle,
                vr::k_unTrackedDeviceIndex_Hmd,
                &notificationTransform );
        }
        else
        {
            LOG( ERROR ) << "Could not find brightness overlay \""
                         << video_keys::k_brightnessOverlayFilename << "\"";
        }
    }
    else
    {
        LOG( ERROR ) << "Could not create brightness overlay: "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                            overlayError );
    }
    setBrightnessOpacityValue( brightnessOpacityValue(), false );
}

void VideoTabController::initColorOverlay()
{
    std::string notifKey
        = std::string( application_strings::applicationKey ) + ".coloroverlay";

    vr::VROverlayError overlayError = vr::VROverlay()->CreateOverlay(
        notifKey.c_str(), notifKey.c_str(), &m_colorOverlayHandle );
    if ( overlayError == vr::VROverlayError_None )
    {
        const auto notifIconPath = paths::binaryDirectoryFindFile(
            video_keys::k_colorOverlayFilename );
        if ( notifIconPath.has_value() )
        {
            vr::VROverlay()->SetOverlayFromFile( m_colorOverlayHandle,
                                                 notifIconPath->c_str() );
            vr::VROverlay()->SetOverlayWidthInMeters( m_colorOverlayHandle,
                                                      k_overlayWidth );
            // position it just slightly further out so we don't have to worry
            // about render order.
            vr::HmdMatrix34_t notificationTransform
                = { { { 1.0f, 0.0f, 0.0f, 0.00f },
                      { 0.0f, 1.0f, 0.0f, 0.00f },
                      { 0.0f, 0.0f, 1.0f, ( k_hmdDistance - 0.01f ) } } };
            vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(
                m_colorOverlayHandle,
                vr::k_unTrackedDeviceIndex_Hmd,
                &notificationTransform );
        }
        else
        {
            LOG( ERROR ) << "Could not find Base color overlay: \""
                         << video_keys::k_colorOverlayFilename << "\"";
        }
    }
    else
    {
        LOG( ERROR ) << "Could not create color overlay: "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                            overlayError );
    }
}

void VideoTabController::loadColorOverlay()
{
    vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayColor(
        m_colorOverlayHandle, colorRed(), colorGreen(), colorBlue() );
    if ( overlayError != vr::VROverlayError_None )
    {
        LOG( ERROR ) << "Could not set Colors for color overlay: "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                            overlayError );
    }

    overlayError = vr::VROverlay()->SetOverlayAlpha( m_colorOverlayHandle,
                                                     colorOverlayOpacity() );
    if ( overlayError != vr::VROverlayError_None )
    {
        LOG( ERROR ) << "Could not set alpha: "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                            overlayError );
    }
    emit colorOverlayOpacityChanged( true );
}

void VideoTabController::eventLoopTick() {}

void VideoTabController::dashboardLoopTick()
{
    if ( settingsUpdateCounter >= m_videoDashboardUpdateCounter )
    {
        vr::EVRSettingsError vrSettingsError;

        // checks supersampling override and resynchs if necessry
        // also prints error if can't find.
        auto sso = vr::VRSettings()->GetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_SupersampleManualOverride_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_SupersampleManualOverride_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }

        setAllowSupersampleOverride( sso );
        // checks supersampling and re-synchs if necessary
        auto ss = vr::VRSettings()->GetFloat(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_SupersampleScale_Float,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_SupersampleScale_Float
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
            if ( m_superSampling != 1.0f )
            {
                LOG( DEBUG ) << "OpenVR returns an error and we have a custom "
                                "supersampling value: "
                             << m_superSampling;
                setSuperSampling( 1.0 );
            }
        }
        else if ( fabs( static_cast<double>( m_superSampling - ss ) ) > 0.05 )
        {
            LOG( INFO ) << "OpenVR reports a changed supersampling value: "
                        << m_superSampling << " => " << ss;
            setSuperSampling( ss );
        }
        // checks if Supersampling filter is on and changes if it has been
        // changed elsewhere

        auto sf = vr::VRSettings()->GetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_AllowSupersampleFiltering_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_AllowSupersampleFiltering_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setAllowSupersampleFiltering( sf );

        // Checks if Motion smoothing setting can be read and synchs adv
        // settings to steamvr/openvr
        auto ms
            = vr::VRSettings()->GetBool( vr::k_pch_SteamVR_Section,
                                         vr::k_pch_SteamVR_MotionSmoothing_Bool,
                                         &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_MotionSmoothing_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setMotionSmoothing( ms );

        // Synch's our saved Values of gain to the SteamVR's version
        // This will allow other apps to modify Gain.

        bool valueAdjust = true;
        if ( isOverlayMethodActive() )
        {
            valueAdjust = false;
        }
        synchGain( valueAdjust );

        settingsUpdateCounter = 0;
    }
    else
    {
        settingsUpdateCounter++;
    }
}

void VideoTabController::reloadVideoConfig()
{
    setBrightnessEnabled( brightnessEnabled(), true );
    setColorOverlayEnabled( colorOverlayEnabled(), true );
    setIsOverlayMethodActive( isOverlayMethodActive(), true );
    setColorRed( colorRed() );
    setColorGreen( colorGreen() );
    setColorBlue( colorBlue() );

    vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayAlpha(
        m_brightnessOverlayHandle, brightnessOpacityValue() );
    if ( overlayError != vr::VROverlayError_None )
    {
        LOG( ERROR ) << "Could not set alpha for Brightness Overlay: "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                            overlayError );
    }
    loadColorOverlay();
}

float VideoTabController::brightnessOpacityValue() const
{
    return static_cast<float>( settings::getSetting(
        settings::DoubleSetting::VIDEO_brightnessOpacityValue ) );
}

bool VideoTabController::brightnessEnabled() const
{
    return settings::getSetting(
        settings::BoolSetting::VIDEO_brightnessEnabled );
}

void VideoTabController::setBrightnessEnabled( bool value,
                                               bool notify,
                                               bool keepValue )
{
    if ( value != brightnessEnabled() || keepValue )
    {
        if ( !keepValue )
        {
            settings::setSetting(
                settings::BoolSetting::VIDEO_brightnessEnabled, value );
        }

        auto overlayHandle = getBrightnessOverlayHandle();
        if ( value )
        {
            if ( overlayHandle != vr::k_ulOverlayHandleInvalid )
            {
                setBrightnessOpacityValue( brightnessOpacityValue(), false );
                vr::VROverlay()->ShowOverlay( getBrightnessOverlayHandle() );
                LOG( INFO ) << "Brightness Overlay toggled on";
            }
        }
        else
        {
            if ( overlayHandle != vr::k_ulOverlayHandleInvalid )
            {
                vr::VROverlay()->HideOverlay( getBrightnessOverlayHandle() );
                LOG( INFO ) << "Brightness Overlay toggled off";
            }
        }

        if ( notify )
        {
            emit brightnessEnabledChanged( value );
        }
    }
}

void VideoTabController::setBrightnessOpacityValue( float percvalue,
                                                    bool notify )
{
    // This takes the Perceived value, and converts it to allow more accurate
    // linear positioning. (human perception logarithmic)
    float realvalue = static_cast<float>(
        std::pow( static_cast<double>( 1.0f - percvalue ), 1 / 3. ) );

    settings::setSetting( settings::DoubleSetting::VIDEO_brightnessOpacityValue,
                          static_cast<double>( percvalue ) );

    if ( realvalue >= 0.97f || realvalue <= 0.00f )
    {
        LOG( WARNING ) << "alpha value is invalid setting to 1.0";
        settings::setSetting(
            settings::DoubleSetting::VIDEO_brightnessOpacityValue, 1.0 );
        realvalue = 0;
    }
    vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayAlpha(
        m_brightnessOverlayHandle, realvalue );
    if ( overlayError != vr::VROverlayError_None )
    {
        LOG( ERROR ) << "Could not set alpha for brightness overlay: "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                            overlayError );
    }

    if ( notify )
    {
        emit brightnessOpacityValueChanged( percvalue );
    }
}

/*
 * -------------------------
 * Color Overlay
 * -------------------------
 */

// getters

bool VideoTabController::colorOverlayEnabled() const
{
    return settings::getSetting(
        settings::BoolSetting::VIDEO_colorOverlayEnabled );
}

float VideoTabController::colorOverlayOpacity() const
{
    return static_cast<float>( settings::getSetting(
        settings::DoubleSetting::VIDEO_colorOverlayOpacity ) );
}

float VideoTabController::colorRed() const
{
    return static_cast<float>(
        settings::getSetting( settings::DoubleSetting::VIDEO_colorRed ) );
}

float VideoTabController::colorGreen() const
{
    return static_cast<float>(
        settings::getSetting( settings::DoubleSetting::VIDEO_colorGreen ) );
}

float VideoTabController::colorBlue() const
{
    return static_cast<float>(
        settings::getSetting( settings::DoubleSetting::VIDEO_colorBlue ) );
}

bool VideoTabController::isOverlayMethodActive() const
{
    return settings::getSetting(
        settings::BoolSetting::VIDEO_isOverlayMethodActive );
}

// setters

void VideoTabController::setIsOverlayMethodActive( bool value, bool notify )
{
    resetGain();
    settings::setSetting( settings::BoolSetting::VIDEO_isOverlayMethodActive,
                          value );

    if ( value )
    {
        setColorOverlayEnabled( colorOverlayEnabled(), true, false );
    }
    else
    {
        setColorOverlayEnabled( false, true, false );
    }
    setColor( colorRed(), colorGreen(), colorBlue(), true, true );

    if ( notify )
    {
        emit isOverlayMethodActiveChanged( value );
    }
}

void VideoTabController::setColorOverlayEnabled( bool value,
                                                 bool notify,
                                                 bool keepValue )
{
    if ( value != colorOverlayEnabled() || keepValue )
    {
        if ( !keepValue )
        {
            settings::setSetting(
                settings::BoolSetting::VIDEO_colorOverlayEnabled, value );
        }
        auto overlayHandle = getColorOverlayHandle();
        if ( value )
        {
            if ( overlayHandle != vr::k_ulOverlayHandleInvalid )
            {
                vr::VROverlay()->ShowOverlay( getColorOverlayHandle() );
                LOG( INFO ) << "Color Overlay toggled on";
            }
        }
        else
        {
            if ( overlayHandle != vr::k_ulOverlayHandleInvalid )
            {
                vr::VROverlay()->HideOverlay( getColorOverlayHandle() );
                LOG( INFO ) << "Color Overlay toggled off";
            }
        }

        if ( notify )
        {
            emit colorOverlayEnabledChanged( value );
        }
    }
}

void VideoTabController::setColorOverlayOpacity( float value, bool notify )
{
    if ( fabs( static_cast<double>( value - colorOverlayOpacity() ) ) > .005 )
    {
        if ( value > .85f )
        {
            value = 0.85f;
        }

        settings::setSetting(
            settings::DoubleSetting::VIDEO_colorOverlayOpacity,
            static_cast<double>( value ) );

        vr::VROverlayError overlayError
            = vr::VROverlay()->SetOverlayAlpha( m_colorOverlayHandle, value );
        if ( overlayError != vr::VROverlayError_None )
        {
            LOG( ERROR ) << "Could not set alpha for color overlay: "
                         << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                                overlayError );
        }

        if ( notify )
        {
            emit colorOverlayOpacityChanged( value );
        }
    }
}

void VideoTabController::synchGain( bool setValue )
{
    vr::EVRSettingsError vrSettingsError;

    auto red = vr::VRSettings()->GetFloat(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_HmdDisplayColorGainR_Float,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not read \""
                       << vr::k_pch_SteamVR_HmdDisplayColorGainR_Float
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }
    else
    {
        if ( fabs( static_cast<double>( red - colorRed() ) ) > 0.005
             && setValue )
        {
            settings::setSetting( settings::DoubleSetting::VIDEO_colorRed,
                                  static_cast<double>( red ) );
        }
    }
    auto blue = vr::VRSettings()->GetFloat(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_HmdDisplayColorGainB_Float,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not read \""
                       << vr::k_pch_SteamVR_HmdDisplayColorGainB_Float
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }
    else
    {
        if ( fabs( static_cast<double>( blue - colorBlue() ) ) > 0.005
             && setValue )
        {
            settings::setSetting( settings::DoubleSetting::VIDEO_colorBlue,
                                  static_cast<double>( blue ) );
        }
    }
    auto green = vr::VRSettings()->GetFloat(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_HmdDisplayColorGainG_Float,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not read \""
                       << vr::k_pch_SteamVR_HmdDisplayColorGainG_Float
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }
    else
    {
        if ( fabs( static_cast<double>( green - colorGreen() ) ) > 0.005
             && setValue )
        {
            settings::setSetting( settings::DoubleSetting::VIDEO_colorGreen,
                                  static_cast<double>( green ) );
        }
    }
}

void VideoTabController::setColorRed( float value, bool notify, bool keepValue )
{
    if ( ( fabs( static_cast<double>( value - colorRed() ) ) > .005 )
         || keepValue )
    {
        if ( !keepValue )
        {
            settings::setSetting( settings::DoubleSetting::VIDEO_colorRed,
                                  static_cast<double>( value ) );
        }

        if ( isOverlayMethodActive() )
        {
            vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayColor(
                m_colorOverlayHandle, colorRed(), colorGreen(), colorBlue() );
            if ( overlayError != vr::VROverlayError_None )
            {
                LOG( ERROR ) << "Could not set Red for color overlay: "
                             << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                                    overlayError );
            }
        }
        else
        {
            vr::EVRSettingsError vrSettingsError;
            vr::VRSettings()->SetFloat(
                vr::k_pch_SteamVR_Section,
                vr::k_pch_SteamVR_HmdDisplayColorGainR_Float,
                colorRed(),
                &vrSettingsError );

            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( ERROR ) << "could not set Red Gain Value, Error: "
                             << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                    vrSettingsError );
            }
        }

        LOG( DEBUG ) << "Changed Red Value to: " << colorRed();

        if ( notify )
        {
            emit colorRedChanged( colorRed() );
        }
    }
}

void VideoTabController::setColorGreen( float value,
                                        bool notify,
                                        bool keepValue )
{
    if ( ( fabs( static_cast<double>( value - colorGreen() ) ) > .005 )
         || keepValue )
    {
        if ( !keepValue )
        {
            settings::setSetting( settings::DoubleSetting::VIDEO_colorGreen,
                                  static_cast<double>( value ) );
        }

        if ( isOverlayMethodActive() )
        {
            vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayColor(
                m_colorOverlayHandle, colorRed(), colorGreen(), colorBlue() );
            if ( overlayError != vr::VROverlayError_None )
            {
                LOG( ERROR ) << "Could not set Green for color overlay: "
                             << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                                    overlayError );
            }
        }
        else
        {
            vr::EVRSettingsError vrSettingsError;
            vr::VRSettings()->SetFloat(
                vr::k_pch_SteamVR_Section,
                vr::k_pch_SteamVR_HmdDisplayColorGainG_Float,
                colorGreen(),
                &vrSettingsError );

            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( ERROR ) << "could not set Green Gain Value, Error: "
                             << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                    vrSettingsError );
            }
        }

        LOG( DEBUG ) << "Changed Green Value to: " << colorGreen();

        if ( notify )
        {
            emit colorGreenChanged( colorGreen() );
        }
    }
}

void VideoTabController::setColorBlue( float value,
                                       bool notify,
                                       bool keepValue )
{
    if ( ( fabs( static_cast<double>( value - colorBlue() ) ) > .005 )
         || keepValue )
    {
        if ( !keepValue )
        {
            settings::setSetting( settings::DoubleSetting::VIDEO_colorBlue,
                                  static_cast<double>( value ) );
        }
        if ( isOverlayMethodActive() )
        {
            vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayColor(
                m_colorOverlayHandle, colorRed(), colorGreen(), colorBlue() );
            if ( overlayError != vr::VROverlayError_None )
            {
                LOG( ERROR ) << "Could not set Blue for color overlay: "
                             << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                                    overlayError );
            }
        }
        else
        {
            vr::EVRSettingsError vrSettingsError;
            vr::VRSettings()->SetFloat(
                vr::k_pch_SteamVR_Section,
                vr::k_pch_SteamVR_HmdDisplayColorGainB_Float,
                colorBlue(),
                &vrSettingsError );

            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( ERROR ) << "could not set Blue Gain Value, Error: "
                             << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                    vrSettingsError );
            }
        }

        LOG( DEBUG ) << "Changed Blue Value to: " << colorBlue();

        if ( notify )
        {
            emit colorBlueChanged( colorBlue() );
        }
    }
}

void VideoTabController::resetGain()
{
    vr::EVRSettingsError vrSettingsError;
    vr::VRSettings()->SetFloat( vr::k_pch_SteamVR_Section,
                                vr::k_pch_SteamVR_HmdDisplayColorGainR_Float,
                                1.0f,
                                &vrSettingsError );

    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( ERROR ) << "could not set Red Gain Value, Error: "
                     << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                            vrSettingsError );
    }
    vr::VRSettings()->SetFloat( vr::k_pch_SteamVR_Section,
                                vr::k_pch_SteamVR_HmdDisplayColorGainG_Float,
                                1.0f,
                                &vrSettingsError );

    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( ERROR ) << "could not set Green Gain Value, Error: "
                     << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                            vrSettingsError );
    }

    vr::VRSettings()->SetFloat( vr::k_pch_SteamVR_Section,
                                vr::k_pch_SteamVR_HmdDisplayColorGainB_Float,
                                1.0f,
                                &vrSettingsError );

    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( ERROR ) << "could not set Blue Gain Value, Error: "
                     << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                            vrSettingsError );
    }
}

void VideoTabController::setColor( float R,
                                   float G,
                                   float B,
                                   bool notify,
                                   bool keepValue )
{
    setColorRed( R, notify, keepValue );
    setColorGreen( G, notify, keepValue );
    setColorBlue( B, notify, keepValue );
}

/*
 * -------------------------
 * SuperSampling
 * -------------------------
 */

// Getters

float VideoTabController::superSampling() const
{
    return m_superSampling;
}

bool VideoTabController::allowSupersampleOverride() const
{
    return m_allowSupersampleOverride;
}

void VideoTabController::initSupersampleOverride()
{
    bool temporary = false;
    vr::EVRSettingsError vrSettingsError;
    temporary = vr::VRSettings()->GetBool(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_SupersampleManualOverride_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not get SuperSampling Override State \""
                       << vr::k_pch_SteamVR_SupersampleManualOverride_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }
    setAllowSupersampleOverride( temporary, true );
}

void VideoTabController::setSuperSampling( float value, const bool notify )
{
    bool override = false;
    // Mirrors Desktop Clamp
    if ( value < 0.2f )
    {
        LOG( WARNING ) << "Encountered a supersampling value <= 0.2, setting "
                          "supersampling to 1.0";
        value = 1.0f;
        override = true;
    }

    if ( override
         || fabs( static_cast<double>( m_superSampling - value ) ) > .005 )
    {
        LOG( DEBUG ) << "Supersampling value changed: " << m_superSampling
                     << " => " << value;
        m_superSampling = value;
        vr::VRSettings()->SetFloat( vr::k_pch_SteamVR_Section,
                                    vr::k_pch_SteamVR_SupersampleScale_Float,
                                    m_superSampling );
        if ( notify )
        {
            emit superSamplingChanged( m_superSampling );
        }
    }
}

void VideoTabController::setAllowSupersampleOverride( const bool value,
                                                      const bool notify )
{
    if ( m_allowSupersampleOverride != value )
    {
        m_allowSupersampleOverride = value;
        vr::VRSettings()->SetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_SupersampleManualOverride_Bool,
            m_allowSupersampleOverride );
        if ( notify )
        {
            emit allowSupersampleOverrideChanged( m_allowSupersampleOverride );
        }
    }
}

/*
 * -------------------------
 * motion smoothing/antistropic (misc)
 * -------------------------
 */

bool VideoTabController::motionSmoothing() const
{
    return m_motionSmoothing;
}

void VideoTabController::setMotionSmoothing( const bool value,
                                             const bool notify )
{
    if ( m_motionSmoothing != value )
    {
        m_motionSmoothing = value;
        vr::VRSettings()->SetBool( vr::k_pch_SteamVR_Section,
                                   vr::k_pch_SteamVR_MotionSmoothing_Bool,
                                   m_motionSmoothing );
        if ( notify )
        {
            emit motionSmoothingChanged( m_motionSmoothing );
        }
    }
}

void VideoTabController::initMotionSmoothing()
{
    bool temporary = false;
    vr::EVRSettingsError vrSettingsError;
    temporary
        = vr::VRSettings()->GetBool( vr::k_pch_SteamVR_Section,
                                     vr::k_pch_SteamVR_MotionSmoothing_Bool,
                                     &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not get MotionSmoothing State \""
                       << vr::k_pch_SteamVR_MotionSmoothing_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }
    setMotionSmoothing( temporary, true );
}

bool VideoTabController::allowSupersampleFiltering() const
{
    return m_allowSupersampleFiltering;
}

void VideoTabController::setAllowSupersampleFiltering( const bool value,
                                                       const bool notify )
{
    if ( m_allowSupersampleFiltering != value )
    {
        m_allowSupersampleFiltering = value;
        vr::VRSettings()->SetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_AllowSupersampleFiltering_Bool,
            m_allowSupersampleFiltering );
        if ( notify )
        {
            emit allowSupersampleFilteringChanged(
                m_allowSupersampleFiltering );
        }
    }
}

/* -----------------------------------------*/
/*------------------------------------------*/
/*Profile Logic Functions*/

void VideoTabController::addVideoProfile( const QString name )
{
    VideoProfile* profile = nullptr;
    for ( auto& p : videoProfiles )
    {
        if ( p.profileName.compare( name.toStdString() ) == 0 )
        {
            profile = &p;
            break;
        }
    }
    if ( !profile )
    {
        auto i = videoProfiles.size();
        videoProfiles.emplace_back();
        profile = &videoProfiles[i];
    }
    profile->profileName = name.toStdString();

    profile->supersampleOverride = m_allowSupersampleOverride;
    profile->supersampling = m_superSampling;
    profile->anisotropicFiltering = m_allowSupersampleFiltering;
    profile->motionSmooth = m_motionSmoothing;
    profile->colorRed = colorRed();
    profile->colorGreen = colorGreen();
    profile->colorBlue = colorBlue();
    profile->brightnessToggle = brightnessEnabled();
    profile->brightnessOpacityValue = brightnessOpacityValue();
    profile->opacity = colorOverlayOpacity();
    profile->overlayMethodState = isOverlayMethodActive();

    saveVideoProfiles();
    emit videoProfilesUpdated();
    emit videoProfileAdded();
}

void VideoTabController::applyVideoProfile( const unsigned index )
{
    if ( index < videoProfiles.size() )
    {
        auto& profile = videoProfiles[index];

        setAllowSupersampleOverride( profile.supersampleOverride );
        setSuperSampling( profile.supersampling );
        setAllowSupersampleFiltering( profile.anisotropicFiltering );
        setMotionSmoothing( profile.motionSmooth );
        setIsOverlayMethodActive( profile.overlayMethodState );
        setColorRed( profile.colorRed );
        setColorBlue( profile.colorBlue );
        setColorGreen( profile.colorGreen );
        setBrightnessEnabled( profile.brightnessToggle );
        setBrightnessOpacityValue( profile.brightnessOpacityValue );
        setColorOverlayOpacity( profile.opacity );
    }
}

void VideoTabController::deleteVideoProfile( const unsigned index )
{
    if ( index < videoProfiles.size() )
    {
        auto pos = videoProfiles.begin() + index;
        videoProfiles.erase( pos );
        saveVideoProfiles();
        emit videoProfilesUpdated();
    }
}

void VideoTabController::reloadVideoProfiles()
{
    settings::loadAllObjects( videoProfiles );
}

void VideoTabController::saveVideoProfiles()
{
    settings::saveAllObjects( videoProfiles );
}

int VideoTabController::getVideoProfileCount()
{
    return static_cast<int>( videoProfiles.size() );
}

QString VideoTabController::getVideoProfileName( const unsigned index )
{
    if ( index >= videoProfiles.size() )
    {
        return QString();
    }
    else
    {
        return QString::fromStdString( videoProfiles[index].profileName );
    }
}

/*------------------------------------------*/
/* -----------------------------------------*/

} // namespace advsettings
