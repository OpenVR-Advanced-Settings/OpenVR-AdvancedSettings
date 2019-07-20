#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <openvr.h>

class QQuickWindow;

namespace video_keys
{
constexpr auto k_brightnessOverlayFilename = "/res/img/video/dimmer.png";
constexpr auto k_colorOverlayFilename = "/res/img/video/color.png";

} // namespace video_keys

namespace advsettings
{
// forward declaration
class OverlayController;

class VideoTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool brightnessEnabled READ brightnessEnabled WRITE
                    setBrightnessEnabled NOTIFY brightnessEnabledChanged )
    Q_PROPERTY( float brightnessValue READ brightnessValue WRITE
                    setBrightnessValue NOTIFY brightnessValueChanged )
    Q_PROPERTY( bool colorEnabled READ colorEnabled WRITE setColorEnabled NOTIFY
                    colorEnabledChanged )
    Q_PROPERTY( float colorOpacity READ colorOpacity WRITE setColorOpacity
                    NOTIFY colorOpacityChanged )
    Q_PROPERTY( float colorOpacityPerc READ colorOpacityPerc WRITE
                    setColorOpacityPerc NOTIFY colorOpacityPercChanged )
    Q_PROPERTY(
        float colorRed READ colorRed WRITE setColorRed NOTIFY colorRedChanged )
    Q_PROPERTY( float colorGreen READ colorGreen WRITE setColorGreen NOTIFY
                    colorGreenChanged )
    Q_PROPERTY( float colorBlue READ colorBlue WRITE setColorBlue NOTIFY
                    colorBlueChanged )

private:
    // how far away the overlay is, any OVERLAY closer will not be dimmed.
    const float k_hmdDistance = -0.15f;
    // how wide overlay is, Increase this value if edges of view are not
    // dimmed.
    const float k_overlayWidth = 1.0f;

    vr::VROverlayHandle_t m_brightnessOverlayHandle
        = vr::k_ulOverlayHandleInvalid;

    vr::VROverlayHandle_t m_colorOverlayHandle = vr::k_ulOverlayHandleInvalid;

    float m_brightnessOpacityValue = 0.0f;
    float m_brightnessValue = 1.0f;
    bool m_brightnessEnabled = false;

    // color member vars
    bool m_colorEnabled = false;
    float m_colorOpacity = 1.0f;
    float m_colorOpacityPerc = 0.0f;
    float m_colorRed = 0.0f;
    float m_colorBlue = 0.0f;
    float m_colorGreen = 0.0f;
    bool m_overlayInit = false;

    void reloadVideoConfig();
    void saveVideoConfig();
    void setBrightnessOpacityValue();

    void initBrightnessOverlay();
    void initColorOverlay();

    void loadColorOverlay();

    QString getSettingsName()
    {
        return "videoSettings";
    }

    vr::VROverlayHandle_t getBrightnessOverlayHandle()
    {
        return m_brightnessOverlayHandle;
    }

    vr::VROverlayHandle_t getColorOverlayHandle()
    {
        return m_colorOverlayHandle;
    }

public:
    float brightnessOpacityValue() const;
    // inverse of brightnessValue
    float brightnessValue() const;
    bool brightnessEnabled() const;

    // Color overlay Getters
    bool colorEnabled() const;
    float colorOpacity() const;
    float colorRed() const;
    float colorGreen() const;
    float colorBlue() const;
    float colorOpacityPerc() const;

    void initStage1();
    void eventLoopTick();

public slots:
    void setBrightnessEnabled( bool value, bool notify = true );
    void setBrightnessValue( float percvalue, bool notify = true );

    // Color overlay Setters
    void setColorEnabled( bool value, bool notify = true );
    void setColorOpacity( float value, bool notify = true );
    void setColorRed( float value, bool notify = true );
    void setColorGreen( float value, bool notify = true );
    void setColorBlue( float value, bool notify = true );
    void setColorOpacityPerc( float value, bool notify = true );

signals:
    void brightnessEnabledChanged( bool value );
    void brightnessValueChanged( float value );

    void colorEnabledChanged( bool value );
    void colorOpacityChanged( float value );
    void colorRedChanged( float value );
    void colorGreenChanged( float value );
    void colorBlueChanged( float value );
    void colorOpacityPercChanged( float value );
};

} // namespace advsettings
