#pragma once

#include "../audiomanager.h"

#include <Mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <Endpointvolume.h>


// application namespace
namespace advsettings {

class AudioManagerWindows : public AudioManager, IMMNotificationClient {
friend class AudioNotificationClient;
private:
	AudioTabController* controller = nullptr;
	IMMDeviceEnumerator* audioDeviceEnumerator = nullptr;
	IMMDevice* mirrorAudioDevice = nullptr;
	IAudioEndpointVolume* mirrorAudioEndpointVolume = nullptr;
	IMMDevice* micAudioDevice = nullptr;
	IAudioEndpointVolume* micAudioEndpointVolume = nullptr;

public:
	~AudioManagerWindows();

	virtual void init(AudioTabController* controller) override;

	virtual void setMirrorDevice(const std::string& id) override;
	virtual void deleteMirrorDevice() override;
	virtual bool isMirrorValid() override;
	virtual std::string getMirrorDevName() override;
	virtual float getMirrorVolume() override;
	virtual bool setMirrorVolume(float value) override;
	virtual bool getMirrorMuted() override;
	virtual bool setMirrorMuted(bool value) override;

	virtual bool isMicValid() override;
	virtual std::string getMicDevName() override;
	virtual float getMicVolume() override;
	virtual bool setMicVolume(float value) override;
	virtual bool getMicMuted() override;
	virtual bool setMicMuted(bool value) override;

	// from IMMNotificationClient
	virtual HRESULT QueryInterface(REFIID riid, void ** ppvObject) override;
	virtual ULONG AddRef(void) override;
	virtual ULONG Release(void) override;
	virtual HRESULT OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState) override;
	virtual HRESULT OnDeviceAdded(LPCWSTR pwstrDeviceId) override;
	virtual HRESULT OnDeviceRemoved(LPCWSTR pwstrDeviceId) override;
	virtual HRESULT OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId) override;
	virtual HRESULT OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key) override;

private:
	IMMDeviceEnumerator* getAudioDeviceEnumerator();
	IMMDevice* getDefaultRecordingDevice(IMMDeviceEnumerator* deviceEnumerator);
	IMMDevice* getDevice(IMMDeviceEnumerator* deviceEnumerator, const std::string& id);
	IMMDevice* getDevice(IMMDeviceEnumerator* deviceEnumerator, LPCWSTR id);
	IAudioEndpointVolume* getAudioEndpointVolume(IMMDevice* device);
	std::string getDeviceName(IMMDevice* device);
};

}

