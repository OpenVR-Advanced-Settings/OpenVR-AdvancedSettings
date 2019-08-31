#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <openvr.h>

class QQuickWindow;

namespace video_keys
{
constexpr auto k_brightnessOverlayFilename = "/res/img/video/dimmer.png";

} // namespace video_keys

namespace advsettings
{
// forward declaration
class OverlayController;

struct VideoProfile
{
    std::string profileName;

    float supersampling = 1.0f;
    bool anisotropicFiltering = false;
    bool motionSmooth = false;
    bool supersampleOverride = false;
    float colorRed = 1.0f;
    float colorGreen = 1.0f;
    float colorBlue = 1.0f;
    bool brightnessToggle = false;
    float brightnessValue = 1.0f;
};

class VideoTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool brightnessEnabled READ brightnessEnabled WRITE
                    setBrightnessEnabled NOTIFY brightnessEnabledChanged )
    Q_PROPERTY( float brightnessValue READ brightnessValue WRITE
                    setBrightnessValue NOTIFY brightnessValueChanged )
    Q_PROPERTY(
        float colorRed READ colorRed WRITE setColorRed NOTIFY colorRedChanged )
    Q_PROPERTY( float colorGreen READ colorGreen WRITE setColorGreen NOTIFY
                    colorGreenChanged )
    Q_PROPERTY( float colorBlue READ colorBlue WRITE setColorBlue NOTIFY
                    colorBlueChanged )
    Q_PROPERTY( float superSampling READ superSampling WRITE setSuperSampling
                    NOTIFY superSamplingChanged )
    Q_PROPERTY(
        bool allowSupersampleOverride READ allowSupersampleOverride WRITE
            setAllowSupersampleOverride NOTIFY allowSupersampleOverrideChanged )
    Q_PROPERTY( bool motionSmoothing READ motionSmoothing WRITE
                    setMotionSmoothing NOTIFY motionSmoothingChanged )
    Q_PROPERTY( bool allowSupersampleFiltering READ allowSupersampleFiltering
                    WRITE setAllowSupersampleFiltering NOTIFY
                        allowSupersampleFilteringChanged )

private:
    unsigned settingsUpdateCounter = 0;

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

    float m_colorRed = 0.0f;
    float m_colorBlue = 0.0f;
    float m_colorGreen = 0.0f;
    bool m_overlayInit = false;

    float m_superSampling = 1.0;
    bool m_allowSupersampleOverride = false;
    bool m_motionSmoothing = true;
    bool m_allowSupersampleFiltering = true;

    void initColorGain();
    void initSupersampleOverride();
    void initMotionSmoothing();

    void reloadVideoConfig();
    void saveVideoConfig();
    void setBrightnessOpacityValue();

    void initBrightnessOverlay();

    std::vector<VideoProfile> videoProfiles;

    QString getSettingsName()
    {
        return "videoSettings";
    }

    vr::VROverlayHandle_t getBrightnessOverlayHandle()
    {
        return m_brightnessOverlayHandle;
    }

public:
    float brightnessOpacityValue() const;
    // inverse of brightnessValue
    float brightnessValue() const;
    bool brightnessEnabled() const;

    // Color overlay Getters
    bool colorEnabled() const;
    float colorRed() const;
    float colorGreen() const;
    float colorBlue() const;

    bool allowSupersampleOverride() const;
    float superSampling() const;
    bool motionSmoothing() const;
    bool allowSupersampleFiltering() const;

    void initStage1();
    void eventLoopTick();
    void dashboardLoopTick();

    void reloadVideoProfiles();
    void saveVideoProfiles();

    Q_INVOKABLE int getVideoProfileCount();
    Q_INVOKABLE QString getVideoProfileName( unsigned index );

public slots:
    void setBrightnessEnabled( bool value, bool notify = true );
    void setBrightnessValue( float percvalue, bool notify = true );

    void setSuperSampling( float value, bool notify = true );
    void setAllowSupersampleOverride( bool value, bool notify = true );
    // Color overlay Setters
    void setColorRed( float value, bool notify = true );
    void setColorGreen( float value, bool notify = true );
    void setColorBlue( float value, bool notify = true );

    void setMotionSmoothing( bool value, bool notify = true );
    void setAllowSupersampleFiltering( bool value, bool notify = true );

    void addVideoProfile( QString name );
    void applyVideoProfile( unsigned index );
    void deleteVideoProfile( unsigned index );

signals:
    void brightnessEnabledChanged( bool value );
    void brightnessValueChanged( float value );
    void colorRedChanged( float value );
    void colorGreenChanged( float value );
    void colorBlueChanged( float value );

    void superSamplingChanged( float value );
    void allowSupersampleOverrideChanged( bool value );

    void motionSmoothingChanged( bool value );
    void allowSupersampleFilteringChanged( bool value );

    void videoProfilesUpdated();
    void videoProfileAdded();
};

} // namespace advsettings
