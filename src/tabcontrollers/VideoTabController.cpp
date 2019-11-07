#include "VideoTabController.h"
#include <QQuickWindow>
#include <QApplication>
#include "../overlaycontroller.h"
#include <cmath>

namespace advsettings
{
void VideoTabController::initStage1()
{
    // In order to ensure gain is "normal" before applying to either overlay or
    // gain.
    resetGain();

    initBrightnessOverlay();
    initColorOverlay();
    initMotionSmoothing();
    initSupersampleOverride();
    m_overlayInit = true;
    reloadVideoConfig();
    reloadVideoProfiles();

    LOG( ERROR ) << "opacity value is: " << colorOverlayOpacity();
    // Force the color Settings to apply to proper method
    // setColor( colorRed(), colorGreen(), colorBlue(), true, true );
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
        m_colorOverlayHandle, m_colorRed, m_colorGreen, m_colorBlue );
    if ( overlayError != vr::VROverlayError_None )
    {
        LOG( ERROR ) << "Could not set Colors for color overlay: "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                            overlayError );
    }

    overlayError = vr::VROverlay()->SetOverlayAlpha( m_colorOverlayHandle,
                                                     m_colorOverlayOpacity );
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
    // TODO different key
    if ( settingsUpdateCounter >= k_steamVrSettingsUpdateCounter )
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

        settingsUpdateCounter = 0;
    }
    else
    {
        settingsUpdateCounter++;
    }
}

void VideoTabController::reloadVideoConfig()
{
    auto settings = OverlayController::appSettings();
    settings->beginGroup( getSettingsName() );
    setBrightnessEnabled(
        settings->value( "brightnessEnabled", false ).toBool(), true );
    m_brightnessOpacityValue
        = settings->value( "brightnessOpacityValue", 0.0f ).toFloat();
    m_brightnessValue = settings->value( "brightnessValue", 1.0f ).toFloat();
    setColorOverlayEnabled(
        settings->value( "colorOverlayEnabled", false ).toBool(), true );
    setColorOverlayOpacity(
        settings->value( "colorOverlayOpacity", 0.0f ).toFloat(), true );
    setIsOverlayMethodActive(
        settings->value( "isOverlayMethodActive", false ).toBool(), true );
    setColorRed( settings->value( "colorRed", 1.0f ).toFloat() );
    setColorGreen( settings->value( "colorGreen", 1.0f ).toFloat() );
    setColorBlue( settings->value( "colorBlue", 1.0f ).toFloat() );

    settings->endGroup();
    setBrightnessOpacityValue();
    loadColorOverlay();
    settings->sync();
}

void VideoTabController::saveVideoConfig()
{
    auto settings = OverlayController::appSettings();
    settings->beginGroup( getSettingsName() );
    settings->setValue( "brightnessEnabled", brightnessEnabled() );
    settings->setValue( "brightnessOpacityValue", brightnessOpacityValue() );
    settings->setValue( "brightnessValue", brightnessValue() );
    settings->setValue( "colorOverlayEnabled", colorOverlayEnabled() );
    settings->setValue( "colorOverlayOpacity", colorOverlayOpacity() );
    settings->setValue( "colorRed", colorRed() );
    settings->setValue( "colorGreen", colorGreen() );
    settings->setValue( "colorBlue", colorBlue() );
    settings->setValue( "isOverlayMethodActive", isOverlayMethodActive() );

    settings->endGroup();
    settings->sync();
}

float VideoTabController::brightnessOpacityValue() const
{
    return m_brightnessOpacityValue;
}

float VideoTabController::brightnessValue() const
{
    return m_brightnessValue;
}

bool VideoTabController::brightnessEnabled() const
{
    return m_brightnessEnabled;
}

void VideoTabController::setBrightnessEnabled( bool value,
                                               bool notify,
                                               bool keepValue )
{
    if ( value != m_brightnessEnabled || keepValue )
    {
        if ( !keepValue )
        {
            m_brightnessEnabled = value;
        }
        auto overlayHandle = getBrightnessOverlayHandle();
        if ( value )
        {
            if ( overlayHandle != vr::k_ulOverlayHandleInvalid )
            {
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
        saveVideoConfig();
        if ( notify )
        {
            emit brightnessEnabledChanged( value );
        }
    }
}

void VideoTabController::setBrightnessValue( float percvalue, bool notify )
{
    // This takes the Perceived value, and converts it to allow more accurate
    // linear positioning. (human perception logarithmic)
    float realvalue = static_cast<float>(
        std::pow( static_cast<double>( 1.0f - percvalue ), 1 / 3. ) );

    if ( fabs( static_cast<double>( realvalue - m_brightnessOpacityValue ) )
         > .005 )
    {
        m_brightnessValue = percvalue;
        if ( realvalue <= 1.0f && realvalue >= 0.0f )
        {
            m_brightnessOpacityValue = realvalue;
            vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayAlpha(
                m_brightnessOverlayHandle, realvalue );
            if ( overlayError != vr::VROverlayError_None )
            {
                LOG( ERROR ) << "Could not set alpha: "
                             << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                                    overlayError );
            }
            // only saves if opacity Value is valid. [1-0]
            saveVideoConfig();
        }
        else
        {
            LOG( WARNING ) << "alpha value is invalid setting to 1.0";
            m_brightnessOpacityValue = 0.0f;
        }

        if ( notify )
        {
            emit brightnessValueChanged( percvalue );
        }
    }
}

void VideoTabController::setBrightnessOpacityValue()
{
    vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayAlpha(
        m_brightnessOverlayHandle, m_brightnessOpacityValue );
    if ( overlayError != vr::VROverlayError_None )
    {
        LOG( ERROR ) << "Could not set alpha: "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                            overlayError );
    }
    emit brightnessValueChanged( m_brightnessValue );
}

/*
 * -------------------------
 * Color Overlay
 * -------------------------
 */

// getters

bool VideoTabController::colorOverlayEnabled() const
{
    return m_colorOverlayEnabled;
}

float VideoTabController::colorOverlayOpacity() const
{
    return m_colorOverlayOpacity;
}

float VideoTabController::colorRed() const
{
    return m_colorRed;
}

float VideoTabController::colorGreen() const
{
    return m_colorGreen;
}

float VideoTabController::colorBlue() const
{
    return m_colorBlue;
}

bool VideoTabController::isOverlayMethodActive() const
{
    return m_isOverlayMethodActive;
}

// setters

void VideoTabController::setIsOverlayMethodActive( bool value, bool notify )
{
    resetGain();
    m_isOverlayMethodActive = value;
    setColorOverlayEnabled( colorOverlayEnabled(), true, true );
    setColor( colorRed(), colorGreen(), colorBlue(), true, true );

    saveVideoConfig();
    if ( notify )
    {
        emit isOverlayMethodActiveChanged( value );
    }
}

void VideoTabController::setColorOverlayEnabled( bool value,
                                                 bool notify,
                                                 bool keepValue )
{
    if ( value != m_colorOverlayEnabled || keepValue )
    {
        if ( !keepValue )
        {
            m_colorOverlayEnabled = value;
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
        saveVideoConfig();
        if ( notify )
        {
            emit colorOverlayEnabledChanged( value );
        }
    }
}

void VideoTabController::setColorOverlayOpacity( float value, bool notify )
{
    if ( value != m_colorOverlayOpacity )
    {
        vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayAlpha(
            m_colorOverlayHandle, m_colorOverlayOpacity );

        m_colorOverlayOpacity = value;

        if ( overlayError != vr::VROverlayError_None )
        {
            LOG( ERROR ) << "Could not set alpha for color overlay: "
                         << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                                overlayError );
        }
        saveVideoConfig();
        if ( notify )
        {
            emit colorOverlayOpacityChanged( m_colorOverlayOpacity );
        }
    }
}

void VideoTabController::setColorRed( float value, bool notify, bool keepValue )
{
    if ( ( fabs( static_cast<double>( value - m_colorRed ) ) > .005 )
         || keepValue )
    {
        if ( !keepValue )
        {
            m_colorRed = value;
        }

        if ( isOverlayMethodActive() )
        {
            vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayColor(
                m_colorOverlayHandle, m_colorRed, m_colorGreen, m_colorBlue );
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
                m_colorRed,
                &vrSettingsError );

            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( ERROR ) << "could not set Red Gain Value, Error: "
                             << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                    vrSettingsError );
            }
        }

        LOG( DEBUG ) << "Changed Red Value to: " << m_colorRed;
        saveVideoConfig();
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit colorRedChanged( m_colorRed );
        }
    }
}

void VideoTabController::setColorGreen( float value,
                                        bool notify,
                                        bool keepValue )
{
    if ( ( fabs( static_cast<double>( value - m_colorGreen ) ) > .005 )
         || keepValue )
    {
        if ( !keepValue )
        {
            m_colorGreen = value;
        }

        if ( isOverlayMethodActive() )
        {
            vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayColor(
                m_colorOverlayHandle, m_colorRed, m_colorGreen, m_colorBlue );
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
                m_colorGreen,
                &vrSettingsError );

            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( ERROR ) << "could not set Green Gain Value, Error: "
                             << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                    vrSettingsError );
            }
        }

        LOG( DEBUG ) << "Changed Green Value to: " << m_colorGreen;
        saveVideoConfig();
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit colorGreenChanged( m_colorGreen );
        }
    }
}

void VideoTabController::setColorBlue( float value,
                                       bool notify,
                                       bool keepValue )
{
    if ( ( fabs( static_cast<double>( value - m_colorBlue ) ) > .005 )
         || keepValue )
    {
        if ( !keepValue )
        {
            m_colorBlue = value;
        }
        if ( isOverlayMethodActive() )
        {
            vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayColor(
                m_colorOverlayHandle, m_colorRed, m_colorGreen, m_colorBlue );
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
                m_colorBlue,
                &vrSettingsError );

            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( ERROR ) << "could not set Blue Gain Value, Error: "
                             << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                    vrSettingsError );
            }
        }

        LOG( DEBUG ) << "Changed Blue Value to: " << m_colorBlue;
        saveVideoConfig();
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit colorBlueChanged( m_colorBlue );
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
    vr::VRSettings()->Sync();
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
        vr::VRSettings()->Sync();
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
        vr::VRSettings()->Sync();
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
        vr::VRSettings()->Sync();
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
    vr::VRSettings()->Sync();
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
        vr::VRSettings()->Sync();
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
    profile->colorRed = m_colorRed;
    profile->colorGreen = m_colorGreen;
    profile->colorBlue = m_colorBlue;
    profile->brightnessToggle = m_brightnessEnabled;
    profile->brightnessValue = m_brightnessValue;
    profile->opacity = m_colorOverlayOpacity;
    profile->overlayMethodState = m_isOverlayMethodActive;

    saveVideoProfiles();
    OverlayController::appSettings()->sync();
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
        setBrightnessValue( profile.brightnessValue );
        setColorOverlayOpacity( profile.opacity );
        vr::VRSettings()->Sync( true );
    }
}

void VideoTabController::deleteVideoProfile( const unsigned index )
{
    if ( index < videoProfiles.size() )
    {
        auto pos = videoProfiles.begin() + index;
        videoProfiles.erase( pos );
        saveVideoProfiles();
        OverlayController::appSettings()->sync();
        emit videoProfilesUpdated();
    }
}

void VideoTabController::reloadVideoProfiles()
{
    videoProfiles.clear();
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "VideoSettings" );
    auto profileCount = settings->beginReadArray( "videoProfiles" );
    for ( int i = 0; i < profileCount; i++ )
    {
        settings->setArrayIndex( i );
        videoProfiles.emplace_back();
        auto& entry = videoProfiles[static_cast<size_t>( i )];
        entry.profileName
            = settings->value( "profileName" ).toString().toStdString();

        entry.supersampleOverride
            = settings->value( "supersamplingOverride", false ).toBool();
        entry.supersampling
            = settings->value( "supersampling", 1.0f ).toFloat();
        entry.anisotropicFiltering
            = settings->value( "anisotropicFiltering", true ).toBool();
        entry.motionSmooth = settings->value( "motionSmooth", true ).toBool();

        entry.colorRed = settings->value( "colorRed", 1.0f ).toFloat();
        entry.colorBlue = settings->value( "colorBlue", 1.0f ).toFloat();
        entry.colorGreen = settings->value( "colorGreen", 1.0f ).toFloat();

        entry.brightnessToggle
            = settings->value( "brightnessToggle", false ).toBool();
        entry.brightnessValue
            = settings->value( "brightnessValue", 1.0f ).toFloat();
        entry.opacity = settings->value( "opacity", 0.0f ).toFloat();
        entry.overlayMethodState
            = settings->value( "overlayMethodState", false ).toBool();
    }
    settings->endArray();
    settings->endGroup();
}

void VideoTabController::saveVideoProfiles()
{
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "VideoSettings" );
    settings->beginWriteArray( "videoProfiles" );
    unsigned i = 0;
    for ( auto& p : videoProfiles )
    {
        settings->setArrayIndex( static_cast<int>( i ) );
        settings->setValue( "profileName",
                            QString::fromStdString( p.profileName ) );

        settings->setValue( "supersamplingOverride", p.supersampleOverride );
        settings->setValue( "supersampling", p.supersampling );
        settings->setValue( "anisotropicFiltering", p.anisotropicFiltering );
        settings->setValue( "motionSmooth", p.motionSmooth );

        settings->setValue( "colorRed", p.colorRed );
        settings->setValue( "colorBlue", p.colorBlue );
        settings->setValue( "colorGreen", p.colorGreen );

        settings->setValue( "brightnessToggle", p.brightnessToggle );
        settings->setValue( "brightnessValue", p.brightnessValue );

        settings->setValue( "opacity", p.opacity );
        settings->setValue( "overlayMethodState", p.overlayMethodState );

        i++;
    }
    settings->endArray();
    settings->endGroup();
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
