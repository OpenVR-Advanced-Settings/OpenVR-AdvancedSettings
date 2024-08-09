#include "AudioTabController.h"
#include <QQuickWindow>
#include <QApplication>
#include <QtLogging>
#include <QtDebug>
#include "../overlaycontroller.h"
#include "../settings/settings.h"
#include "../settings/settings_object.h"
#include "../utils/update_rate.h"
#include "openvr/ovr_overlay_wrapper.h"
#ifdef _WIN32
#    include "audiomanager/AudioManagerWindows.h"
#elif __linux__
#    include "audiomanager/AudioManagerPulse.h"
#else
#    include "audiomanager/AudioManagerDummy.h"
#endif

// application namespace
namespace advsettings
{
void AudioTabController::initStage1()
{
    vr::EVRSettingsError vrSettingsError = {};
#ifdef _WIN32
    audioManager.reset( new AudioManagerWindows() );
#elif __linux__
    audioManager.reset( new AudioManagerPulse() );
#else
    audioManager.reset( new AudioManagerDummy() );
#endif
    audioManager->init( this );
    initOverride();
    m_playbackDevices = audioManager->getPlaybackDevices();
    m_recordingDevices = audioManager->getRecordingDevices();
    findPlaybackDeviceIndex( audioManager->getPlaybackDevId(), false );
    std::array<char, 1024> deviceId;
    vr::VRSettings()->GetString( vr::k_pch_audio_Section,
                                 vr::k_pch_audio_PlaybackMirrorDevice_String,
                                 deviceId.data(),
                                 1024,
                                 &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        qWarning() << "Could not read \""
                   << vr::k_pch_audio_PlaybackMirrorDevice_String
                   << "\" setting: "
                   << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                          vrSettingsError );
    }
    else
    {
        audioManager->setMirrorDevice( deviceId.data() );
        findMirrorDeviceIndex( audioManager->getMirrorDevId(), false );
        lastMirrorDevId = deviceId.data();
        m_mirrorVolume = audioManager->getMirrorVolume();
        m_mirrorMuted = audioManager->getMirrorMuted();
    }
    findMicDeviceIndex( audioManager->getMicDevId(), false );
    m_micVolume = audioManager->getMicVolume();
    m_micMuted = audioManager->getMicMuted();
    // Temporarily disabled audio profiles for 1.8.19 until api is clear.
    // reloadAudioProfiles();
    reloadAudioSettings();

    eventLoopTick();
}

void AudioTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
    const auto pushToTalkOverlayKey
        = std::string( application_strings::applicationKey )
          + ".pptnotification";

    const auto overlayError
        = vr::VROverlay()->CreateOverlay( pushToTalkOverlayKey.c_str(),
                                          pushToTalkOverlayKey.c_str(),
                                          &m_pushToTalkValues.overlayHandle );
    if ( overlayError != vr::VROverlayError_None )
    {
        qCritical() << "Could not create ptt notification overlay: "
                    << vr::VROverlay()->GetOverlayErrorNameFromEnum(
                           overlayError );

        emit defaultProfileDisplay();

        return;
    }

    m_pushToTalkValues.pushToTalkImg.reset(
        new QImage( QString( ":/microphone/ptt_notification.png" ) ) );
    m_pushToTalkValues.pushToMuteImg.reset(
        new QImage( QString( ":/microphone/ptm_notification.png" ) ) );

    auto* pushToImg = m_pushToTalkValues.pushToTalkImg.get();
    if ( settings::getSetting( settings::BoolSetting::AUDIO_micReversePtt ) )
    {
        pushToImg = m_pushToTalkValues.pushToMuteImg.get();
    }

    ovr_overlay_wrapper::setOverlayFromQImage( m_pushToTalkValues.overlayHandle,
                                               *pushToImg );
    vr::VROverlay()->SetOverlayWidthInMeters( m_pushToTalkValues.overlayHandle,
                                              0.02f );
    vr::HmdMatrix34_t const notificationTransform
        = { { { 1.0f, 0.0f, 0.0f, 0.12f },
              { 0.0f, 1.0f, 0.0f, 0.08f },
              { 0.0f, 0.0f, 1.0f, -0.3f } } };
    vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(
        m_pushToTalkValues.overlayHandle,
        vr::k_unTrackedDeviceIndex_Hmd,
        &notificationTransform );

    emit defaultProfileDisplay();
}

void AudioTabController::reloadAudioSettings()
{
    std::lock_guard<std::recursive_mutex> const lock( eventLoopMutex );

    setMicProximitySensorCanMute(
        settings::getSetting(
            settings::BoolSetting::AUDIO_micProximitySensorCanMute ),
        false );

    setMicReversePtt(
        settings::getSetting( settings::BoolSetting::AUDIO_micReversePtt ) );
}

float AudioTabController::mirrorVolume() const
{
    return m_mirrorVolume;
}

bool AudioTabController::mirrorMuted() const
{
    return m_mirrorMuted;
}

float AudioTabController::micVolume() const
{
    return m_micVolume;
}

bool AudioTabController::micMuted() const
{
    return m_micMuted;
}

bool AudioTabController::micProximitySensorCanMute() const
{
    return settings::getSetting(
        settings::BoolSetting::AUDIO_micProximitySensorCanMute );
}

bool AudioTabController::micReversePtt() const
{
    return settings::getSetting( settings::BoolSetting::AUDIO_micReversePtt );
}

bool AudioTabController::audioProfileDefault() const
{
    return m_isDefaultAudioProfile;
}

void AudioTabController::eventLoopTick()
{
    if ( updateRate.shouldSubjectNotRun( UpdateSubject::AudioTabController ) )
    {
        return;
    }

    if ( !eventLoopMutex.try_lock() )
    {
        return;
    }

    vr::EVRSettingsError vrSettingsError = {};
    std::array<char, 1024> mirrorDeviceId;
    vr::VRSettings()->GetString( vr::k_pch_audio_Section,
                                 vr::k_pch_audio_PlaybackMirrorDevice_String,
                                 mirrorDeviceId.data(),
                                 1024,
                                 &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        qWarning() << "Could not read \""
                   << vr::k_pch_audio_PlaybackMirrorDevice_String
                   << "\" setting: "
                   << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                          vrSettingsError );
    }
    if ( lastMirrorDevId.compare( mirrorDeviceId.data() ) != 0 )
    {
        audioManager->setMirrorDevice( mirrorDeviceId.data() );
        findMirrorDeviceIndex( audioManager->getMirrorDevId() );
        lastMirrorDevId = mirrorDeviceId.data();
    }
    if ( m_mirrorDeviceIndex >= 0 )
    {
        setMirrorVolume( audioManager->getMirrorVolume() );
        setMirrorMuted( audioManager->getMirrorMuted() );
    }
    if ( m_recordingDeviceIndex >= 0 )
    {
        setMicVolume( audioManager->getMicVolume() );
        setMicMuted( audioManager->getMicMuted() );
    }

    eventLoopMutex.unlock();
}

bool AudioTabController::pttChangeValid()
{
    return audioManager && audioManager->isMicValid();
}

void AudioTabController::onPttStart()
{
    if ( micReversePtt() )
    {
        setMicMuted( true );
    }
    else
    {
        setMicMuted( false );
    }
}

void AudioTabController::onPttEnabled()
{
    if ( micReversePtt() )
    {
        setMicMuted( false );
    }
    else
    {
        setMicMuted( true );
    }
}

void AudioTabController::onPttStop()
{
    if ( micReversePtt() )
    {
        setMicMuted( false );
    }
    else
    {
        setMicMuted( true );
    }
}

void AudioTabController::onPttDisabled()
{
    setMicMuted( false );
}

void AudioTabController::setMirrorVolume( float value, bool notify )
{
    std::lock_guard<std::recursive_mutex> const lock( eventLoopMutex );
    if ( value != m_mirrorVolume )
    {
        m_mirrorVolume = value;
        if ( audioManager->isMirrorValid() )
        {
            audioManager->setMirrorVolume( value );
        }
        if ( notify )
        {
            emit mirrorVolumeChanged( value );
        }
    }
}

void AudioTabController::setMirrorMuted( bool value, bool notify )
{
    std::lock_guard<std::recursive_mutex> const lock( eventLoopMutex );
    if ( value != m_mirrorMuted )
    {
        m_mirrorMuted = value;
        if ( audioManager->isMirrorValid() )
        {
            audioManager->setMirrorMuted( value );
        }
        if ( notify )
        {
            emit mirrorMutedChanged( value );
        }
    }
}

void AudioTabController::setMicVolume( float value, bool notify )
{
    std::lock_guard<std::recursive_mutex> const lock( eventLoopMutex );
    if ( value != m_micVolume )
    {
        m_micVolume = value;
        if ( audioManager->isMicValid() )
        {
            audioManager->setMicVolume( value );
        }
        if ( notify )
        {
            emit micVolumeChanged( value );
        }
    }
}

void AudioTabController::setMicMuted( bool value, bool notify )
{
    std::lock_guard<std::recursive_mutex> const lock( eventLoopMutex );

    if ( value != m_micMuted )
    {
        m_micMuted = value;
        if ( audioManager->isMicValid() )
        {
            audioManager->setMicMuted( value );
        }
        if ( notify )
        {
            emit micMutedChanged( value );
        }
    }
}

void AudioTabController::setMicProximitySensorCanMute( bool value, bool notify )
{
    std::lock_guard<std::recursive_mutex> const lock( eventLoopMutex );

    settings::setSetting(
        settings::BoolSetting::AUDIO_micProximitySensorCanMute, value );

    if ( notify )
    {
        emit micProximitySensorCanMuteChanged( value );
    }
}

void AudioTabController::setMicReversePtt( bool value, bool notify )
{
    std::lock_guard<std::recursive_mutex> const lock( eventLoopMutex );

    settings::setSetting( settings::BoolSetting::AUDIO_micReversePtt, value );

    // TODO: if reenabled use `ovr_overlay_wrapper::setOverlayFromQImage`
    // instead
    if ( value )
    {
        // vr::VROverlay()->SetOverlayFromFile(m_ptt_overlayHandle,/*pt_mute*/);
    }
    else
    {
        // vr::VROverlay()->SetOverlayFromFile(m_ptt_overlayHandle,/*pt_talk*/);
    }

    if ( pttEnabled() )
    {
        if ( m_pttActive )
        {
            setMicMuted( value );
        }
        else
        {
            setMicMuted( !value );
        }
    }
    if ( notify )
    {
        emit micReversePttChanged( value );
    }
}

void AudioTabController::setAudioProfileDefault( bool value, bool notify )
{
    std::lock_guard<std::recursive_mutex> const lock( eventLoopMutex );
    if ( value != m_isDefaultAudioProfile )
    {
        m_isDefaultAudioProfile = value;
        if ( notify )
        {
            emit audioProfileDefaultChanged( value );
        }
    }
}

void AudioTabController::onNewRecordingDevice()
{
    findMicDeviceIndex( audioManager->getMicDevId() );
}

void AudioTabController::onNewPlaybackDevice()
{
    findPlaybackDeviceIndex( audioManager->getPlaybackDevId() );
}

void AudioTabController::onNewMirrorDevice()
{
    auto devid = audioManager->getMirrorDevId();
    if ( devid.empty() )
    {
        m_mirrorDeviceIndex = -1;
        emit mirrorDeviceIndexChanged( m_mirrorDeviceIndex );
    }
    else
    {
        findMirrorDeviceIndex( devid );
    }
}

void AudioTabController::onDeviceStateChanged()
{
    // I'm too lazy to find out which device has changed, so let's invalidate
    // all device lists
    m_playbackDevices = audioManager->getPlaybackDevices();
    m_recordingDevices = audioManager->getRecordingDevices();
    findPlaybackDeviceIndex( audioManager->getPlaybackDevId(), false );
    findMirrorDeviceIndex( audioManager->getMirrorDevId(), false );
    findMicDeviceIndex( audioManager->getMicDevId(), false );
    emit playbackDeviceListChanged();
    emit recordingDeviceListChanged();
}

int AudioTabController::getPlaybackDeviceCount()
{
    return static_cast<int>( m_playbackDevices.size() );
}

bool indexIsValid( const int index,
                   const std::vector<AudioDevice>& devices,
                   const std::string& functionName ) noexcept
{
    if ( index < 0 )
    {
        qCritical() << functionName
                    << " called with index below zero: " << index;
        return false;
    }

    if ( static_cast<size_t>( index ) >= devices.size() )
    {
        qCritical() << functionName
                    << " called with index greater than size of "
                       "devices '"
                    << devices.size() << "', index '" << index << "'.";
        return false;
    }

    return true;
}

QString AudioTabController::getPlaybackDeviceName( int index )
{
    if ( indexIsValid( index, m_playbackDevices, "getPlaybackDeviceName" ) )
    {
        return QString::fromStdString(
            m_playbackDevices.at( static_cast<size_t>( index ) ).name() );
    }

    return "<PBN:ERROR>";
}

std::string AudioTabController::getPlaybackDeviceID( int index )
{
    if ( indexIsValid( index, m_playbackDevices, "getPlaybackDeviceID" ) )
    {
        return m_playbackDevices.at( static_cast<size_t>( index ) ).id();
    }

    return "<PBI:ERROR>";
}

std::string AudioTabController::getMirrorDeviceID( int index )
{
    return getPlaybackDeviceID( index );
}

int AudioTabController::getRecordingDeviceCount()
{
    return static_cast<int>( m_recordingDevices.size() );
}

std::string AudioTabController::getRecordingDeviceID( int index )
{
    if ( indexIsValid( index, m_recordingDevices, "getRecordingDeviceID" ) )
    {
        return m_recordingDevices.at( static_cast<size_t>( index ) ).id();
    }

    return "<RCI:ERROR>";
}

QString AudioTabController::getRecordingDeviceName( int index )
{
    if ( indexIsValid( index, m_recordingDevices, "getRecordingDeviceName" ) )
    {
        return QString::fromStdString(
            m_recordingDevices.at( static_cast<size_t>( index ) ).name() );
    }

    return "<RCN:ERROR>";
}

int AudioTabController::playbackDeviceIndex() const
{
    return m_playbackDeviceIndex;
}

int AudioTabController::mirrorDeviceIndex() const
{
    return m_mirrorDeviceIndex;
}

int AudioTabController::micDeviceIndex() const
{
    return m_recordingDeviceIndex;
}

void AudioTabController::setPlaybackDeviceIndex( int index, bool notify )
{
    if ( index != m_playbackDeviceIndex )
    {
        if ( index >= 0
             && static_cast<size_t>( index ) < m_playbackDevices.size()
             && index != m_mirrorDeviceIndex )
        {
            // Code to only change the Device and not apply changes to SteamVR
            audioManager->setPlaybackDevice(
                m_playbackDevices[static_cast<size_t>( index )].id(), notify );
        }
        else if ( notify )
        {
            emit playbackDeviceIndexChanged( m_playbackDeviceIndex );
        }
    }
}

void AudioTabController::setMirrorDeviceIndex( int index, bool notify )
{
    vr::EVRSettingsError vrSettingsError = {};
    if ( index != m_mirrorDeviceIndex )
    {
        if ( index == -1 )
        {
            vr::VRSettings()->RemoveKeyInSection(
                vr::k_pch_audio_Section,
                vr::k_pch_audio_PlaybackMirrorDevice_String,
                &vrSettingsError );
            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                qWarning() << "Could not remove \""
                           << vr::k_pch_audio_PlaybackMirrorDevice_String
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
            }
            else
            {
                audioManager->setMirrorDevice( "", notify );
            }
        }
        else if ( index >= 0
                  && static_cast<size_t>( index ) < m_playbackDevices.size()
                  && index != m_playbackDeviceIndex
                  && index != m_mirrorDeviceIndex )
        {
            vr::VRSettings()->SetString(
                vr::k_pch_audio_Section,
                vr::k_pch_audio_PlaybackMirrorDevice_String,
                m_playbackDevices[static_cast<size_t>( index )].id().c_str(),
                &vrSettingsError );
            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                qWarning() << "Could not write \""
                           << vr::k_pch_audio_PlaybackMirrorDevice_String
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
            }
            else
            {
                audioManager->setMirrorDevice(
                    m_playbackDevices[static_cast<size_t>( index )].id(),
                    notify );
            }
        }
        else if ( notify )
        {
            emit mirrorDeviceIndexChanged( m_mirrorDeviceIndex );
        }
    }
}

void AudioTabController::setMicDeviceIndex( int index, bool notify )
{
    std::lock_guard<std::recursive_mutex> const lock( eventLoopMutex );
    if ( index != m_recordingDeviceIndex )
    {
        if ( index >= 0
             && static_cast<size_t>( index ) < m_recordingDevices.size() )
        {
            // un-mute mic before switching to ensure on exit all recording
            // devices will be un-muted
            bool tempProx = false;
            if ( micProximitySensorCanMute() )
            {
                setMicProximitySensorCanMute( false );
                tempProx = true;
            }
            setMicMuted( false, true );
            // code to just change Mic
            audioManager->setMicDevice(
                m_recordingDevices[static_cast<size_t>( index )].id(), notify );
            m_recordingDeviceIndex = index;
            if ( tempProx )
            {
                setMicProximitySensorCanMute( true );
            }
        }
        if ( notify )
        {
            emit micDeviceIndexChanged( m_recordingDeviceIndex );
        }
    }
}

void AudioTabController::findPlaybackDeviceIndex( std::string dev_id,
                                                  bool notify )
{
    int index = 0;
    bool deviceFound = false;
    for ( auto dev : m_playbackDevices )
    {
        if ( dev.id().compare( dev_id ) == 0 )
        {
            deviceFound = true;
            break;
        }

        ++index;
    }
    if ( deviceFound )
    {
        m_playbackDeviceIndex = index;
        if ( notify )
        {
            emit playbackDeviceIndexChanged( index );
        }
    }
}

void AudioTabController::findMirrorDeviceIndex( std::string dev_id,
                                                bool notify )
{
    int index = 0;
    bool deviceFound = false;
    for ( auto dev : m_playbackDevices )
    {
        if ( dev.id().compare( dev_id ) == 0 )
        {
            deviceFound = true;
            break;
        }

        ++index;
    }
    if ( deviceFound && m_mirrorDeviceIndex != index )
    {
        m_mirrorDeviceIndex = index;
        if ( notify )
        {
            emit mirrorDeviceIndexChanged( index );
        }
    }
}

void AudioTabController::findMicDeviceIndex( std::string dev_id, bool notify )
{
    int index = 0;
    bool deviceFound = false;
    for ( auto dev : m_recordingDevices )
    {
        if ( dev.id().compare( dev_id ) == 0 )
        {
            deviceFound = true;
            break;
        }

        ++index;
    }
    if ( deviceFound )
    {
        m_recordingDeviceIndex = index;
        if ( notify )
        {
            emit micDeviceIndexChanged( index );
        }
    }
}

void AudioTabController::reloadAudioProfiles()
{
    settings::loadAllObjects( audioProfiles );
}

void AudioTabController::saveAudioProfiles()
{
    settings::saveAllObjects( audioProfiles );
}

/*
Name: addAudioProfile

Inputs: args: Qstring name - the name of the audio profile
other: none


Output: return: none
other: audioProfile to audioProfiles array

Description: Creates an audioProfile, and adds it to the working copy of
audioProfiles[]

*/

void AudioTabController::addAudioProfile( QString name )
{
    AudioProfile* profile = nullptr;
    for ( auto& pro : audioProfiles )
    {
        if ( pro.profileName.compare( name.toStdString() ) == 0 )
        {
            profile = &pro;
            break;
        }
    }
    if ( !profile )
    {
        auto index = audioProfiles.size();
        audioProfiles.emplace_back();
        profile = &audioProfiles[index];
    }
    profile->profileName = name.toStdString();
    profile->playbackName
        = getPlaybackDeviceName( m_playbackDeviceIndex ).toStdString();
    profile->mirrorName
        = getPlaybackDeviceName( m_mirrorDeviceIndex ).toStdString();
    profile->micName
        = getRecordingDeviceName( m_recordingDeviceIndex ).toStdString();
    profile->micMute = m_micMuted;
    profile->mirrorMute = m_mirrorMuted;
    profile->mirrorVol = m_mirrorVolume;
    profile->micVol = m_micVolume;
    profile->defaultProfile = m_isDefaultAudioProfile;

    profile->playbackID = getPlaybackDeviceID( m_playbackDeviceIndex );
    profile->mirrorID = getMirrorDeviceID( m_mirrorDeviceIndex );
    profile->recordingID = getRecordingDeviceID( m_recordingDeviceIndex );

    if ( m_isDefaultAudioProfile )
    {
        setDefaultMic( m_recordingDeviceIndex );
        setDefaultPlayback( m_playbackDeviceIndex );

        removeOtherDefaultProfiles( name );

        setAudioProfileDefault( false );
    }
    saveAudioProfiles();
    emit audioProfilesUpdated();
    emit audioProfileAdded();
}

/*
Name: applyAudioProfile

Inputs: args: index - index of audioProfile in audioProfiles[]
other: audioProfile based on index


Output: return: none
other: none

Description: Applies the required logic to activate the audio profile.

*/

void AudioTabController::applyAudioProfile( unsigned index )
{
    std::lock_guard<std::recursive_mutex> const lock( eventLoopMutex );
    if ( index < audioProfiles.size() )
    {
        auto& profile = audioProfiles[index];
        int const mInd = getMirrorIndex( profile.mirrorID );
        int const pInd = getPlaybackIndex( profile.playbackID );

        // Needed to keep remembering when swtiching from mirror/main etc.
        // TODO OPTI can possibly clean up logic to reduce overhead in future.
        setMicMuted( false, false );
        setMirrorMuted( false, false );

        if ( ( m_playbackDeviceIndex == mInd )
             && ( m_mirrorDeviceIndex == pInd ) )
        {
            setMirrorDeviceIndex( -1, true );
        }
        if ( m_playbackDeviceIndex == mInd )
        {
            setPlaybackDeviceIndex( pInd, true );
            setMirrorDeviceIndex( mInd, true );
        }
        else
        {
            setMirrorDeviceIndex( mInd, true );
            setPlaybackDeviceIndex( pInd, true );
        }

        setMicDeviceIndex( getRecordingIndex( profile.recordingID ), true );
        setMicMuted( profile.micMute, true );
        setMirrorMuted( profile.mirrorMute, true );
        setMicVolume( profile.micVol, true );
        setMirrorVolume( profile.mirrorVol, true );
    }
}

/*
Name: deleteAudioProfile

Inputs: args: index - index of audioProfile in audioProfiles[]
other: audioProfile based on index


Output: return: none
other: none

Description: Removes selected Audio Profile

*/
void AudioTabController::deleteAudioProfile( unsigned index )
{
    if ( index < audioProfiles.size() )
    {
        auto pos = audioProfiles.begin() + index;
        // Remove PlayBack and Mic from Steam API Mirror Is handled @ shutdown
        // This is necessary because Mirror Device does not appear to be handled
        // via native windows api.
        if ( audioProfiles.at( index ).defaultProfile )
        {
            vr::EVRSettingsError vrSettingsError = {};
            vr::VRSettings()->RemoveKeyInSection(
                vr::k_pch_audio_Section,
                vr::k_pch_audio_PlaybackDeviceOverrideName_String,
                &vrSettingsError );
            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                qWarning() << "Could not remove \""
                           << vr::k_pch_audio_PlaybackDeviceOverrideName_String
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
            }

            vr::VRSettings()->RemoveKeyInSection(
                vr::k_pch_audio_Section,
                vr::k_pch_audio_RecordingDeviceOverrideName_String,
                &vrSettingsError );
            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                qWarning() << "Could not remove \""
                           << vr::k_pch_audio_RecordingDeviceOverrideName_String
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
            }
        }
        audioProfiles.erase( pos );
        saveAudioProfiles();
        emit audioProfilesUpdated();
    }
}

// 1.8.19 changes
//********************

bool AudioTabController::playbackOverride() const
{
    return m_isPlaybackOverride;
}

bool AudioTabController::recordingOverride() const
{
    return m_isRecordingOverride;
}
void AudioTabController::setPlaybackOverride( bool value, bool notify )
{
    if ( value != m_isPlaybackOverride )
    {
        m_isPlaybackOverride = value;
        vr::EVRSettingsError vrSettingsError = {};
        vr::VRSettings()->SetBool(
            vr::k_pch_audio_Section,
            vr::k_pch_audio_EnablePlaybackDeviceOverride_Bool,
            m_isPlaybackOverride,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            qCritical() << "Could not set playback override setting: "
                        << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                               vrSettingsError );
        }
        if ( notify )
        {
            emit playbackOverrideChanged();
        }
    }
}
void AudioTabController::setRecordingOverride( bool value, bool notify )
{
    if ( value != m_isRecordingOverride )
    {
        m_isRecordingOverride = value;
        vr::EVRSettingsError vrSettingsError = {};
        vr::VRSettings()->SetBool(
            vr::k_pch_audio_Section,
            vr::k_pch_audio_EnableRecordingDeviceOverride_Bool,
            m_isRecordingOverride,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            qCritical() << "Could not set recording override setting: "
                        << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                               vrSettingsError );
        }
        if ( notify )
        {
            emit playbackOverrideChanged();
        }
    }
}

void AudioTabController::initOverride()
{
    vr::EVRSettingsError vrSettingsError = {};
    auto temp = vr::VRSettings()->GetBool(
        vr::k_pch_audio_Section,
        vr::k_pch_audio_EnableRecordingDeviceOverride_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        qCritical() << "Could not get recording override setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
        qInfo() << "Setting recording override to false";
        setRecordingOverride( false );
    }
    else
    {
        setRecordingOverride( temp );
    }
    temp = vr::VRSettings()->GetBool(
        vr::k_pch_audio_Section,
        vr::k_pch_audio_EnablePlaybackDeviceOverride_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        qCritical() << "Could not get playback override setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
        qInfo() << "Setting playback override to false";
        setPlaybackOverride( false );
    }
    else
    {
        setPlaybackOverride( temp );
    }
}
//********************

unsigned AudioTabController::getAudioProfileCount()
{
    return static_cast<unsigned int>( audioProfiles.size() );
}

QString AudioTabController::getAudioProfileName( unsigned index )
{
    if ( index < audioProfiles.size() )
    {
        return QString::fromStdString( audioProfiles[index].profileName );
    }
    return "";
}

/*
Name: getPlaybackIndex,  getRecordingIndex, and getMirrorIndex
input: string, of microphone/playback device ID
output: integer for use In: setMicDeviceIndex(int,bool),
setMirrorDeviceIndex(int,bool) setPlayBackDeviceIndex(int,bool)
description: Gets proper index value for selecting specific devices.
*/
int AudioTabController::getPlaybackIndex( std::string str )
{
    // Unsigned to avoid two casts. it won't be negative because we only
    // increment it.
    for ( unsigned int i = 0; i < m_playbackDevices.size(); i++ )
    {
        if ( str.compare( m_playbackDevices[i].id() ) == 0 )
        {
            return static_cast<int>( i );
        }
    }
    return m_playbackDeviceIndex;
}

int AudioTabController::getRecordingIndex( std::string str )
{
    // Unsigned to avoid two casts. it won't be negative because we only
    // increment it.
    for ( unsigned int i = 0; i < m_recordingDevices.size(); i++ )
    {
        if ( str.compare( m_recordingDevices[i].id() ) == 0 )
        {
            return static_cast<int>( i );
        }
    }
    return m_recordingDeviceIndex;
}

int AudioTabController::getMirrorIndex( std::string str )
{
    // Unsigned to avoid two casts. it won't be negative because we only
    // increment it.
    for ( unsigned int i = 0; i < m_playbackDevices.size(); i++ )
    {
        if ( str.compare( m_playbackDevices[i].id() ) == 0 )
        {
            return static_cast<int>( i );
        }
    }
    return -1;
}

/*
 Name: setDefaultPlayback/Mirror/Mic

 input int index = index of m_playbackDevices to set

 description: sets the SteamVR "on launch" audio options

 NOTE: We Assume Index passed is good, and valid
*/

void AudioTabController::setDefaultPlayback( int index, bool notify )
{
    vr::EVRSettingsError vrSettingsError = {};
    vr::VRSettings()->SetString(
        vr::k_pch_audio_Section,
        vr::k_pch_audio_PlaybackDeviceOverrideName_String,
        m_playbackDevices[static_cast<size_t>( index )].id().c_str(),
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        qWarning() << "Could not write \""
                   << vr::k_pch_audio_PlaybackDeviceOverrideName_String
                   << "\" setting: "
                   << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                          vrSettingsError );
    }
    else
    {
        audioManager->setPlaybackDevice(
            m_playbackDevices[static_cast<size_t>( index )].id(), notify );
    }
}

void AudioTabController::setDefaultMic( int index, bool notify )
{
    vr::EVRSettingsError vrSettingsError = {};
    vr::VRSettings()->SetString(
        vr::k_pch_audio_Section,
        vr::k_pch_audio_RecordingDeviceOverrideName_String,
        m_recordingDevices[static_cast<size_t>( index )].id().c_str(),
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        qWarning() << "Could not write \""
                   << vr::k_pch_audio_RecordingDeviceOverrideName_String
                   << "\" setting: "
                   << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                          vrSettingsError );
    }
    else
    {
        audioManager->setMicDevice(
            m_recordingDevices[static_cast<size_t>( index )].id(), notify );
    }
}

void AudioTabController::setDefaultMirror( int index, bool notify )
{
    if ( index == -1 )
    {
        vr::EVRSettingsError vrSettingsError = {};
        vr::VRSettings()->RemoveKeyInSection(
            vr::k_pch_audio_Section,
            vr::k_pch_audio_PlaybackMirrorDevice_String,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            qWarning() << "Could not remove \""
                       << vr::k_pch_audio_PlaybackMirrorDevice_String
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
        }
        else
        {
            audioManager->setMirrorDevice( "", notify );
        }
    }
    else
    {
        vr::EVRSettingsError vrSettingsError = {};
        vr::VRSettings()->SetString(
            vr::k_pch_audio_Section,
            vr::k_pch_audio_PlaybackMirrorDevice_String,
            m_playbackDevices[static_cast<size_t>( index )].id().c_str(),
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            qWarning() << "Could not write \""
                       << vr::k_pch_audio_PlaybackMirrorDevice_String
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
        }
        else
        {
            audioManager->setMirrorDevice(
                m_playbackDevices[static_cast<size_t>( index )].id(), notify );
        }
    }
}

/*
Name: removeOtherDefaultProfiles

input: QString name - name of new default profile
output: none

description: checks all profiles and removes any OTHERS that are set as default.
*/
void AudioTabController::removeOtherDefaultProfiles( QString name )
{
    AudioProfile* profile = nullptr;
    for ( auto& pro : audioProfiles )
    {
        if ( pro.profileName.compare( name.toStdString() ) != 0 )
        {
            profile = &pro;
            profile->defaultProfile = false;
        }
    }
}

/*
Name: applyDefaultProfile

input: none
output: none

description: checks all profiles and applies the one (if any) with the default
flag);
*/

void AudioTabController::applyDefaultProfile()
{
    for ( unsigned i = 0; i < audioProfiles.size(); i++ )
    {
        auto& profile = audioProfiles[i];
        if ( profile.defaultProfile )
        {
            applyAudioProfile( i );
            m_defaultProfileIndex = static_cast<int>( i );
            break;
        }
    }
}

int AudioTabController::getDefaultAudioProfileIndex()
{
    return m_defaultProfileIndex;
}

/* ---------------------------*/
/*----------------------------*/

void AudioTabController::shutdown()
{
    setMicMuted( false, true );
    std::string mID;
    bool hasDefaultProfile = false;

    // Un-mutes mic to prevent confusion on exit.

    setMicMuted( false );

    // handles Setting Mirror back to User based settings
    // since we have to change the Mirror Device via valve api.
    // possibly could change windows tab controller to allow us to do
    // natively.
    for ( unsigned i = 0; i < audioProfiles.size(); i++ )
    {
        auto& profile = audioProfiles[i];
        if ( profile.defaultProfile )
        {
            mID = profile.mirrorID;
            hasDefaultProfile = true;
            break;
        }
    }
    if ( hasDefaultProfile )
    {
        setMirrorDeviceIndex( getMirrorIndex( mID ) );
    }
    else
    {
        setMirrorDeviceIndex( -1 );
    }
    qInfo() << "Audio Tab Controller Has Shut Down";
}

bool AudioTabController::pttEnabled() const
{
    return settings::getSetting( settings::BoolSetting::AUDIO_pttEnabled );
}

bool AudioTabController::pttActive() const
{
    return m_pttActive;
}

bool AudioTabController::pttShowNotification() const
{
    return settings::getSetting(
        settings::BoolSetting::AUDIO_pttShowNotification );
}

void AudioTabController::startPtt()
{
    m_pttActive = true;
    onPttStart();
    if ( pttShowNotification()
         && getNotificationOverlayHandle() != vr::k_ulOverlayHandleInvalid )
    {
        vr::VROverlay()->ShowOverlay( getNotificationOverlayHandle() );
    }
    emit pttActiveChanged( m_pttActive );
}

void AudioTabController::stopPtt()
{
    m_pttActive = false;
    onPttStop();
    if ( pttShowNotification()
         && getNotificationOverlayHandle() != vr::k_ulOverlayHandleInvalid )
    {
        vr::VROverlay()->HideOverlay( getNotificationOverlayHandle() );
    }
    emit pttActiveChanged( m_pttActive );
}

void AudioTabController::setPttEnabled( bool value, bool notify )
{
    std::lock_guard<std::recursive_mutex> const lock( eventLoopMutex );

    settings::setSetting( settings::BoolSetting::AUDIO_pttEnabled, value );

    if ( value )
    {
        onPttEnabled();
    }
    else
    {
        onPttDisabled();
    }

    if ( notify )
    {
        emit pttEnabledChanged( value );
    }
}

void AudioTabController::setPttShowNotification( bool value, bool notify )
{
    std::lock_guard<std::recursive_mutex> const lock( eventLoopMutex );

    settings::setSetting( settings::BoolSetting::AUDIO_pttShowNotification,
                          value );

    if ( notify )
    {
        emit pttShowNotificationChanged( value );
    }
}

} // namespace advsettings
