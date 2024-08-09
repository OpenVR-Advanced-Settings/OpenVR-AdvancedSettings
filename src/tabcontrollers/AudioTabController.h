#pragma once

#include <QObject>
#include <mutex>

#include "audiomanager/AudioManager.h"
#include <memory>
#include <QImage>
#include "../settings/settings_object.h"
#include "openvr.h"

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;
struct AudioProfile : settings::ISettingsObject
{
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    std::string profileName;
    std::string playbackName;
    std::string mirrorName;
    std::string micName;
    std::string recordingID;
    std::string playbackID;
    std::string mirrorID;
    float mirrorVol = 0.0;
    float micVol = 1.0;
    bool micMute = false;
    bool mirrorMute = false;
    bool defaultProfile = false;
    // NOLINTEND(misc-non-private-member-variables-in-classes)

    [[nodiscard]] settings::SettingsObjectData saveSettings() const override
    {
        settings::SettingsObjectData sod;

        sod.addValue( profileName );
        sod.addValue( playbackName );
        sod.addValue( mirrorName );
        sod.addValue( micName );
        sod.addValue( recordingID );
        sod.addValue( playbackID );
        sod.addValue( mirrorID );

        sod.addValue( static_cast<double>( mirrorVol ) );
        sod.addValue( static_cast<double>( micVol ) );

        sod.addValue( micMute );
        sod.addValue( mirrorMute );
        sod.addValue( defaultProfile );

        return sod;
    }

    void loadSettings( settings::SettingsObjectData& obj ) override
    {
        profileName = obj.getNextValueOrDefault( "" );
        playbackName = obj.getNextValueOrDefault( "" );
        mirrorName = obj.getNextValueOrDefault( "" );
        micName = obj.getNextValueOrDefault( "" );
        recordingID = obj.getNextValueOrDefault( "" );
        playbackID = obj.getNextValueOrDefault( "" );
        mirrorID = obj.getNextValueOrDefault( "" );

        mirrorVol = static_cast<float>( obj.getNextValueOrDefault( 0.0 ) );
        micVol = static_cast<float>( obj.getNextValueOrDefault( 1.0 ) );
        micMute = obj.getNextValueOrDefault( false );
        mirrorMute = obj.getNextValueOrDefault( false );
        defaultProfile = obj.getNextValueOrDefault( false );
    }

    [[nodiscard]] std::string settingsName() const override
    {
        return "AudioTabController::AudioProfile";
    }
};

class AudioTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( int playbackDeviceIndex READ playbackDeviceIndex WRITE
                    setPlaybackDeviceIndex NOTIFY playbackDeviceIndexChanged )
    Q_PROPERTY( int mirrorDeviceIndex READ mirrorDeviceIndex WRITE
                    setMirrorDeviceIndex NOTIFY mirrorDeviceIndexChanged )
    Q_PROPERTY( float mirrorVolume READ mirrorVolume WRITE setMirrorVolume
                    NOTIFY mirrorVolumeChanged )
    Q_PROPERTY( bool mirrorMuted READ mirrorMuted WRITE setMirrorMuted NOTIFY
                    mirrorMutedChanged )
    Q_PROPERTY( int micDeviceIndex READ micDeviceIndex WRITE setMicDeviceIndex
                    NOTIFY micDeviceIndexChanged )
    Q_PROPERTY( float micVolume READ micVolume WRITE setMicVolume NOTIFY
                    micVolumeChanged )
    Q_PROPERTY(
        bool micMuted READ micMuted WRITE setMicMuted NOTIFY micMutedChanged )
    Q_PROPERTY( bool micProximitySensorCanMute READ micProximitySensorCanMute
                    WRITE setMicProximitySensorCanMute NOTIFY
                        micProximitySensorCanMuteChanged )
    Q_PROPERTY( bool micReversePtt READ micReversePtt WRITE setMicReversePtt
                    NOTIFY micReversePttChanged )
    Q_PROPERTY( bool audioProfileDefault READ audioProfileDefault WRITE
                    setAudioProfileDefault NOTIFY audioProfileDefaultChanged )
    Q_PROPERTY( bool playbackOverride READ playbackOverride WRITE
                    setPlaybackOverride NOTIFY playbackOverrideChanged )
    Q_PROPERTY( bool recordingOverride READ recordingOverride WRITE
                    setRecordingOverride NOTIFY recordingOverrideChanged )

    Q_PROPERTY( bool pttEnabled READ pttEnabled WRITE setPttEnabled NOTIFY
                    pttEnabledChanged )
    Q_PROPERTY( bool pttActive READ pttActive NOTIFY pttActiveChanged )
    Q_PROPERTY( bool pttShowNotification READ pttShowNotification WRITE
                    setPttShowNotification NOTIFY pttShowNotificationChanged )

private:
    OverlayController* parent;
    struct
    {
        vr::VROverlayHandle_t overlayHandle = vr::k_ulOverlayHandleInvalid;
        std::unique_ptr<QImage> pushToTalkImg;
        std::unique_ptr<QImage> pushToMuteImg;
    } m_pushToTalkValues;

    int m_playbackDeviceIndex = -1;

    int m_mirrorDeviceIndex = -1;
    float m_mirrorVolume = 1.0;
    bool m_mirrorMuted = false;

    int m_recordingDeviceIndex = -1;
    float m_micVolume = 1.0;
    bool m_micMuted = false;
    bool m_isDefaultAudioProfile = false;
    bool m_isPlaybackOverride = false;
    bool m_isRecordingOverride = false;

    int m_defaultProfileIndex = -1;

    unsigned settingsUpdateCounter = 0;

    bool m_pttActive = false;

    std::unique_ptr<AudioManager> audioManager;
    std::vector<AudioDevice> m_recordingDevices;
    std::vector<AudioDevice> m_playbackDevices;
    std::string lastMirrorDevId;

    std::recursive_mutex eventLoopMutex;

    void onPttStart();
    void onPttStop();
    void onPttEnabled();
    void onPttDisabled();

    virtual vr::VROverlayHandle_t getNotificationOverlayHandle()
    {
        return m_pushToTalkValues.overlayHandle;
    }

    void findPlaybackDeviceIndex( std::string dev_id, bool notify = true );
    void findMirrorDeviceIndex( std::string dev_id, bool notify = true );
    void findMicDeviceIndex( std::string dev_id, bool notify = true );

    int getPlaybackIndex( std::string str );
    int getRecordingIndex( std::string str );
    int getMirrorIndex( std::string str );

    void removeOtherDefaultProfiles( QString name );

    std::string getPlaybackDeviceID( int index );
    std::string getMirrorDeviceID( int index );
    std::string getRecordingDeviceID( int index );

    void setDefaultPlayback( int index, bool notify = true );
    void setDefaultMirror( int index, bool notify = true );
    void setDefaultMic( int index, bool notify = true );

    void initOverride();

    std::vector<AudioProfile> audioProfiles;

public:
    void initStage1();
    void initStage2( OverlayController* var_parent );

    void reloadAudioSettings();

    void eventLoopTick();

    bool pttChangeValid();

    [[nodiscard]] int playbackDeviceIndex() const;

    [[nodiscard]] int mirrorDeviceIndex() const;
    [[nodiscard]] float mirrorVolume() const;
    [[nodiscard]] bool mirrorMuted() const;

    [[nodiscard]] int micDeviceIndex() const;
    [[nodiscard]] float micVolume() const;
    [[nodiscard]] bool micMuted() const;
    [[nodiscard]] bool micProximitySensorCanMute() const;
    [[nodiscard]] bool micReversePtt() const;
    [[nodiscard]] bool audioProfileDefault() const;
    [[nodiscard]] bool playbackOverride() const;
    [[nodiscard]] bool recordingOverride() const;

    void reloadAudioProfiles();
    void saveAudioProfiles();
    Q_INVOKABLE void applyDefaultProfile();

    Q_INVOKABLE int getPlaybackDeviceCount();
    Q_INVOKABLE QString getPlaybackDeviceName( int index );

    Q_INVOKABLE int getRecordingDeviceCount();
    Q_INVOKABLE QString getRecordingDeviceName( int index );

    Q_INVOKABLE unsigned getAudioProfileCount();
    Q_INVOKABLE QString getAudioProfileName( unsigned index );
    Q_INVOKABLE int getDefaultAudioProfileIndex();

    void onNewRecordingDevice();
    void onNewPlaybackDevice();
    void onNewMirrorDevice();
    void onDeviceStateChanged();
    void shutdown();

    [[nodiscard]] bool pttEnabled() const;
    [[nodiscard]] bool pttActive() const;
    [[nodiscard]] bool pttShowNotification() const;

    void startPtt();
    void stopPtt();

    void setMirrorVolume( float value, bool notify = true );
    void setMirrorMuted( bool value, bool notify = true );

    void setMicVolume( float value, bool notify = true );
    void setMicMuted( bool value, bool notify = true );
    void setMicProximitySensorCanMute( bool value, bool notify = true );
    void setMicReversePtt( bool value, bool notify = true );

    void setPlaybackDeviceIndex( int value, bool notify = true );
    void setMirrorDeviceIndex( int value, bool notify = true );
    void setMicDeviceIndex( int value, bool notify = true );

    void addAudioProfile( QString name );
    void applyAudioProfile( unsigned index );
    void deleteAudioProfile( unsigned index );

    void setPlaybackOverride( bool value, bool notify = true );
    void setRecordingOverride( bool value, bool notify = true );

    void setAudioProfileDefault( bool value, bool notify = true );

    void setPttEnabled( bool value, bool notify = true );
    void setPttShowNotification( bool value, bool notify = true );

signals:
    void playbackDeviceIndexChanged( int index );

    void mirrorDeviceIndexChanged( int index );
    void mirrorVolumeChanged( float value );
    void mirrorMutedChanged( bool value );

    void micDeviceIndexChanged( int index );
    void micVolumeChanged( float value );
    void micMutedChanged( bool value );
    void micProximitySensorCanMuteChanged( bool value );
    void micReversePttChanged( bool value );

    void playbackDeviceListChanged();
    void recordingDeviceListChanged();

    void audioProfilesUpdated();
    void audioProfileAdded();
    void audioProfileDefaultChanged( bool value );

    void playbackOverrideChanged();
    void recordingOverrideChanged();

    void defaultProfileDisplay();

    void pttEnabledChanged( bool value );
    void pttActiveChanged( bool value );
    void pttShowNotificationChanged( bool value );
};
} // namespace advsettings
