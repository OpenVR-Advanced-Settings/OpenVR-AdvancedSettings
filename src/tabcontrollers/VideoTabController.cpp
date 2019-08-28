#include "VideoTabController.h"
#include <QQuickWindow>
#include <QApplication>
#include "../overlaycontroller.h"
#include <cmath>

namespace advsettings
{
void VideoTabController::initStage1()
{
    initBrightnessOverlay();
    initColorGain();
    initMotionSmoothing();
    initSupersampleOverride();
    m_overlayInit = true;
    reloadVideoConfig();
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
    settings->endGroup();
    setBrightnessOpacityValue();
    settings->sync();
}

void VideoTabController::saveVideoConfig()
{
    auto settings = OverlayController::appSettings();
    settings->beginGroup( getSettingsName() );
    settings->setValue( "brightnessEnabled", brightnessEnabled() );
    settings->setValue( "brightnessOpacityValue", brightnessOpacityValue() );
    settings->setValue( "brightnessValue", brightnessValue() );
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

void VideoTabController::setBrightnessEnabled( bool value, bool notify )
{
    if ( value != m_brightnessEnabled )
    {
        m_brightnessEnabled = value;
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

// setters

void VideoTabController::setColorRed( float value, bool notify )
{
    if ( fabs( static_cast<double>( value - m_colorRed ) ) > .005 )
    {
        m_colorRed = value;
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
        LOG( INFO ) << "Changed Red Gain to: " << m_colorRed;
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit colorRedChanged( m_colorRed );
        }
    }
}

void VideoTabController::setColorGreen( float value, bool notify )
{
    if ( fabs( static_cast<double>( value - m_colorGreen ) ) > .005 )
    {
        m_colorGreen = value;
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
        LOG( INFO ) << "Changed Green Gain to: " << m_colorGreen;
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit colorGreenChanged( m_colorGreen );
        }
    }
}

void VideoTabController::setColorBlue( float value, bool notify )
{
    if ( fabs( static_cast<double>( value - m_colorBlue ) ) > .005 )
    {
        m_colorBlue = value;
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
        LOG( INFO ) << "Changed Blue Gain to: " << m_colorBlue;
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit colorBlueChanged( m_colorBlue );
        }
    }
}

void VideoTabController::initColorGain()
{
    float temporary = 0.0f;
    vr::EVRSettingsError vrSettingsError;
    temporary = vr::VRSettings()->GetFloat(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_HmdDisplayColorGainR_Float,
        &vrSettingsError );

    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( ERROR ) << "Could not get Red Gain Value ERROR: "
                     << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                            vrSettingsError );
    }
    else
    {
        m_colorRed = temporary;
        setColorRed( temporary );
    }

    temporary = vr::VRSettings()->GetFloat(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_HmdDisplayColorGainG_Float,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( ERROR ) << "Could not get Green Gain Value ERROR: "
                     << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                            vrSettingsError );
    }
    else
    {
        m_colorGreen = temporary;
        setColorGreen( temporary );
    }

    temporary = vr::VRSettings()->GetFloat(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_HmdDisplayColorGainB_Float,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( ERROR ) << "Could not get Blue Gain Value ERROR: "
                     << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                            vrSettingsError );
    }
    else
    {
        m_colorBlue = temporary;
        setColorBlue( temporary );
    }
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

    // TODO delta comparison
    if ( override || m_superSampling != value )
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

/*------------------------------------------*/
/* -----------------------------------------*/

} // namespace advsettings
