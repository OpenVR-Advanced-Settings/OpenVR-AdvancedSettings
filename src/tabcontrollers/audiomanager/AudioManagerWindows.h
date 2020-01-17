#pragma once

#include "AudioManager.h"

#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <endpointvolume.h>
#include <IPolicyConfig.h>
#include <mutex>

// application namespace
namespace advsettings
{
class AudioManagerWindows : public AudioManager, IMMNotificationClient
{
    friend class AudioNotificationClient;

private:
    std::recursive_mutex _mutex;
    AudioTabController* controller = nullptr;
    IMMDeviceEnumerator* audioDeviceEnumerator = nullptr;
    IMMDevice* playbackAudioDevice = nullptr;
    IMMDevice* mirrorAudioDevice = nullptr;
    IAudioEndpointVolume* mirrorAudioEndpointVolume = nullptr;
    IMMDevice* micAudioDevice = nullptr;
    IAudioEndpointVolume* micAudioEndpointVolume = nullptr;
    IPolicyConfig* policyConfig = nullptr;

public:
    ~AudioManagerWindows() override;

    virtual void init( AudioTabController* controller ) override;

    virtual void setPlaybackDevice( const std::string& id,
                                    bool notify = true ) override;
    virtual std::string getPlaybackDevName() override;
    virtual std::string getPlaybackDevId() override;

    virtual void setMirrorDevice( const std::string& id,
                                  bool notify = true ) override;
    virtual bool isMirrorValid() override;
    virtual std::string getMirrorDevName() override;
    virtual std::string getMirrorDevId() override;
    virtual float getMirrorVolume() override;
    virtual bool setMirrorVolume( float value ) override;
    virtual bool getMirrorMuted() override;
    virtual bool setMirrorMuted( bool value ) override;

    virtual bool isMicValid() override;
    virtual void setMicDevice( const std::string& id,
                               bool notify = true ) override;
    virtual std::string getMicDevName() override;
    virtual std::string getMicDevId() override;
    virtual float getMicVolume() override;
    virtual bool setMicVolume( float value ) override;
    virtual bool getMicMuted() override;
    virtual bool setMicMuted( bool value ) override;

    virtual std::vector<AudioDevice> getRecordingDevices() override;
    virtual std::vector<AudioDevice> getPlaybackDevices() override;

    void deleteMirrorDevice();

    // from IMMNotificationClient
    virtual HRESULT QueryInterface( REFIID riid, void** ppvObject ) override;
    virtual ULONG AddRef( void ) override;
    virtual ULONG Release( void ) override;
    virtual HRESULT OnDeviceStateChanged( LPCWSTR pwstrDeviceId,
                                          DWORD dwNewState ) override;
    virtual HRESULT OnDeviceAdded( LPCWSTR pwstrDeviceId ) override;
    virtual HRESULT OnDeviceRemoved( LPCWSTR pwstrDeviceId ) override;
    virtual HRESULT
        OnDefaultDeviceChanged( EDataFlow flow,
                                ERole role,
                                LPCWSTR pwstrDefaultDeviceId ) override;
    virtual HRESULT OnPropertyValueChanged( LPCWSTR pwstrDeviceId,
                                            const PROPERTYKEY key ) override;

private:
    IMMDeviceEnumerator* getAudioDeviceEnumerator();
    IPolicyConfig* getPolicyConfig();
    IMMDevice*
        getDefaultRecordingDevice( IMMDeviceEnumerator* deviceEnumerator );
    IMMDevice*
        getDefaultPlaybackDevice( IMMDeviceEnumerator* deviceEnumerator );
    IMMDevice* getDevice( IMMDeviceEnumerator* deviceEnumerator,
                          const std::string& id );
    IMMDevice* getDevice( IMMDeviceEnumerator* deviceEnumerator, LPCWSTR id );
    IAudioEndpointVolume* getAudioEndpointVolume( IMMDevice* device );
    std::string getDeviceName( IMMDevice* device );
    std::string getDeviceId( IMMDevice* device );
    std::vector<AudioDevice> getDevices( IMMDeviceEnumerator* deviceEnumerator,
                                         EDataFlow dataFlow );
};

} // namespace advsettings
