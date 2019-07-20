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
    initColorOverlay();
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

void VideoTabController::eventLoopTick() {}

void VideoTabController::reloadVideoConfig()
{
    auto settings = OverlayController::appSettings();
    settings->beginGroup( getSettingsName() );
    setBrightnessEnabled(
        settings->value( "brightnessEnabled", false ).toBool(), true );
    m_brightnessOpacityValue
        = settings->value( "brightnessOpacityValue", 0.0f ).toFloat();
    m_brightnessValue = settings->value( "brightnessValue", 1.0f ).toFloat();
    setColorEnabled( settings->value( "colorEnabled", false ).toBool(), true );
    m_colorOpacity = settings->value( "colorOpacity", 0.0f ).toFloat();
    m_colorOpacityPerc = settings->value( "colorOpacityPerc", 1.0f ).toFloat();
    setColorRed( settings->value( "colorRed", 0.0f ).toFloat() );
    setColorGreen( settings->value( "colorGreen", 0.0f ).toFloat() );
    setColorBlue( settings->value( "colorBlue", 0.0f ).toFloat() );
    settings->endGroup();
    LOG( INFO ) << "bright opacityValue: " << m_brightnessOpacityValue
                << " REd value is: " << m_colorRed;
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
    settings->setValue( "colorEnabled", colorEnabled() );
    settings->setValue( "colorOpacity", colorOpacity() );
    settings->setValue( "colorOpacityPerc", colorOpacityPerc() );
    settings->setValue( "colorRed", colorRed() );
    settings->setValue( "colorGreen", colorGreen() );
    settings->setValue( "colorBlue", colorBlue() );
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

    if ( realvalue != m_brightnessOpacityValue )
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
bool VideoTabController::colorEnabled() const
{
    return m_colorEnabled;
}

float VideoTabController::colorOpacity() const
{
    return m_colorOpacity;
}

float VideoTabController::colorOpacityPerc() const
{
    return m_colorOpacityPerc;
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

// setters
void VideoTabController::setColorEnabled( bool value, bool notify )
{
    if ( value != m_colorEnabled )
    {
        m_colorEnabled = value;
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
            emit colorEnabledChanged( value );
        }
    }
}

void VideoTabController::setColorOpacity( float value, bool notify )
{
    if ( value != m_colorOpacity )
    {
        vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayAlpha(
            m_colorOverlayHandle, m_colorOpacity );
        m_colorOpacity = value;
        if ( overlayError != vr::VROverlayError_None )
        {
            LOG( ERROR ) << "Could not set alpha for color overlay: "
                         << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                                overlayError );
        }
        saveVideoConfig();
        if ( notify )
        {
            emit colorOpacityChanged( m_colorOpacity );
        }
    }
}

void VideoTabController::setColorRed( float value, bool notify )
{
    if ( value != m_colorRed )
    {
        m_colorRed = value;
        vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayColor(
            m_colorOverlayHandle, m_colorRed, m_colorGreen, m_colorBlue );
        if ( overlayError != vr::VROverlayError_None )
        {
            LOG( ERROR ) << "Could not set Red for color overlay: "
                         << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                                overlayError );
        }
        saveVideoConfig();
        if ( notify )
        {
            emit colorRedChanged( m_colorRed );
        }
    }
}

void VideoTabController::setColorGreen( float value, bool notify )
{
    if ( value != m_colorGreen )
    {
        m_colorGreen = value;
        vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayColor(
            m_colorOverlayHandle, m_colorRed, m_colorGreen, m_colorBlue );
        if ( overlayError != vr::VROverlayError_None )
        {
            LOG( ERROR ) << "Could not set Green for color overlay: "
                         << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                                overlayError );
        }
        saveVideoConfig();
        if ( notify )
        {
            emit colorGreenChanged( m_colorGreen );
        }
    }
}

void VideoTabController::setColorBlue( float value, bool notify )
{
    if ( value != m_colorBlue )
    {
        m_colorBlue = value;
        vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayColor(
            m_colorOverlayHandle, m_colorRed, m_colorGreen, m_colorBlue );
        if ( overlayError != vr::VROverlayError_None )
        {
            LOG( ERROR ) << "Could not set Blue for color overlay: "
                         << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                                overlayError );
        }
        saveVideoConfig();
        if ( notify )
        {
            emit colorBlueChanged( m_colorBlue );
        }
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
                                                     m_colorOpacity );
    if ( overlayError != vr::VROverlayError_None )
    {
        LOG( ERROR ) << "Could not set alpha: "
                     << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                            overlayError );
    }
}

void VideoTabController::setColorOpacityPerc( float percvalue, bool notify )
{
    // This takes the Perceived value, and converts it to allow more accurate
    // linear positioning. (human perception logarithmic)

    // clamp the "percieved" value to 50% this is to ensure useability.
    if ( percvalue <= 0.5f )
    {
        percvalue = 0.5f;
    }
    float realvalue = static_cast<float>(
        std::pow( static_cast<double>( 1.0f - percvalue ), 0.5555f ) );

    if ( realvalue != m_colorOpacity )
    {
        m_colorOpacityPerc = percvalue;
        if ( realvalue <= 1.0f && realvalue >= 0.0f )
        {
            m_colorOpacity = realvalue;
            vr::VROverlayError overlayError = vr::VROverlay()->SetOverlayAlpha(
                m_colorOverlayHandle, realvalue );
            if ( overlayError != vr::VROverlayError_None )
            {
                LOG( ERROR ) << "Could not set alpha of color overlay: "
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

} // namespace advsettings
