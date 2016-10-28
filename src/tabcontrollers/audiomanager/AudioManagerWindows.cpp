#include "audiomanagerwindows.h"
#include <exception>
#include "../../logging.h"
#include "../AudioTabController.h"

#include <locale>
#include <codecvt>


// application namespace
namespace advsettings {

AudioManagerWindows::~AudioManagerWindows() {
	audioDeviceEnumerator->UnregisterEndpointNotificationCallback((IMMNotificationClient*)this);
	if (mirrorAudioEndpointVolume) {
		mirrorAudioEndpointVolume->Release();
	}
	if (micAudioEndpointVolume) {
		micAudioEndpointVolume->Release();
	}
	if (mirrorAudioDevice) {
		mirrorAudioDevice->Release();
	}
	if (micAudioDevice) {
		micAudioDevice->Release();
	}
	audioDeviceEnumerator->Release();
}

void AudioManagerWindows::init(AudioTabController* controller) {
	audioDeviceEnumerator = getAudioDeviceEnumerator();
	if (!audioDeviceEnumerator) {
		throw std::exception("Could not create audio device enumerator");
	}
	micAudioDevice = getDefaultRecordingDevice(audioDeviceEnumerator);
	if (micAudioDevice) {
		micAudioEndpointVolume = getAudioEndpointVolume(micAudioDevice);
	} else {
		LOG(WARNING) << "Could not find a default recording device.";
	}
	this->controller = controller;
	audioDeviceEnumerator->RegisterEndpointNotificationCallback((IMMNotificationClient*)this);
}


void AudioManagerWindows::setMirrorDevice(const std::string& id) {
	if (id.empty()) {
		deleteMirrorDevice();
	} else {
		mirrorAudioDevice = getDevice(audioDeviceEnumerator, id);
		if (mirrorAudioDevice) {
			mirrorAudioEndpointVolume = getAudioEndpointVolume(mirrorAudioDevice);
		} else {
			LOG(WARNING) << "Could not find mirror device \"" << id << "\".";
		}
	}
}

void AudioManagerWindows::deleteMirrorDevice() {
	if (mirrorAudioEndpointVolume) {
		mirrorAudioEndpointVolume->Release();
		mirrorAudioEndpointVolume = nullptr;
	}
	if (mirrorAudioDevice) {
		mirrorAudioDevice->Release();
		mirrorAudioDevice = nullptr;
	}
}

bool AudioManagerWindows::isMirrorValid() {
	return mirrorAudioEndpointVolume != nullptr;
}

std::string AudioManagerWindows::getMirrorDevName() {
	if (mirrorAudioDevice) {
		return getDeviceName(mirrorAudioDevice);
	}
	return "";
}

float AudioManagerWindows::getMirrorVolume() {
	float value;
	if (mirrorAudioEndpointVolume && mirrorAudioEndpointVolume->GetMasterVolumeLevelScalar(&value) >= 0) {
		return value;
	} else {
		return 0.0;
	}
}

bool AudioManagerWindows::setMirrorVolume(float value) {
	if (mirrorAudioEndpointVolume) {
		if (mirrorAudioEndpointVolume->SetMasterVolumeLevelScalar(value, nullptr) >= 0) {
			return true;
		}
	}
	return false;
}

bool AudioManagerWindows::getMirrorMuted() {
	BOOL value;
	if (mirrorAudioEndpointVolume && mirrorAudioEndpointVolume->GetMute(&value) >= 0) {
		return value;
	} else {
		return false;
	}
}

bool AudioManagerWindows::setMirrorMuted(bool value) {
	if (mirrorAudioEndpointVolume) {
		if (mirrorAudioEndpointVolume->SetMute(value, nullptr) >= 0) {
			return true;
		}
	}
	return false;
}


bool AudioManagerWindows::isMicValid() {
	return micAudioEndpointVolume != nullptr;
}

std::string AudioManagerWindows::getMicDevName() {
	if (micAudioDevice) {
		return getDeviceName(micAudioDevice);
	}
	return "";
}

float AudioManagerWindows::getMicVolume() {
	float value;
	if (micAudioEndpointVolume && micAudioEndpointVolume->GetMasterVolumeLevelScalar(&value) >= 0) {
		return value;
	} else {
		return 0.0;
	}
}

bool AudioManagerWindows::setMicVolume(float value) {
	if (micAudioEndpointVolume) {
		if (micAudioEndpointVolume->SetMasterVolumeLevelScalar(value, nullptr) >= 0) {
			return true;
		}
	}
	return false;
}

bool AudioManagerWindows::getMicMuted() {
	BOOL value;
	if (micAudioEndpointVolume && micAudioEndpointVolume->GetMute(&value) >= 0) {
		return value;
	} else {
		return false;
	}
}

bool AudioManagerWindows::setMicMuted(bool value) {
	if (micAudioEndpointVolume) {
		if (micAudioEndpointVolume->SetMute(value, nullptr) >= 0) {
			return true;
		}
	}
	return false;
}


IMMDeviceEnumerator* AudioManagerWindows::getAudioDeviceEnumerator() {
	IMMDeviceEnumerator* pEnumerator;
	if (CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator) < 0) {
		return nullptr;
	}
	return pEnumerator;
}

IMMDevice* AudioManagerWindows::getDefaultRecordingDevice(IMMDeviceEnumerator* deviceEnumerator) {
	IMMDevice* pDevice;
	if (deviceEnumerator->GetDefaultAudioEndpoint(eCapture, eCommunications, &pDevice) < 0) {
		return nullptr;
	}
	return pDevice;
}

IMMDevice* AudioManagerWindows::getDevice(IMMDeviceEnumerator* deviceEnumerator, const std::string& id) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wid = converter.from_bytes(id);
	return getDevice(deviceEnumerator, wid.c_str());
}

IMMDevice * AudioManagerWindows::getDevice(IMMDeviceEnumerator * deviceEnumerator, LPCWSTR id) {
	IMMDevice* pDevice;
	if (deviceEnumerator->GetDevice(id, &pDevice) < 0) {
		return nullptr;
	}
	return pDevice;
}

IAudioEndpointVolume * AudioManagerWindows::getAudioEndpointVolume(IMMDevice* device) {
	IAudioEndpointVolume * pEndpointVolume;
	if (device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (void**)&pEndpointVolume) < 0) {
		return nullptr;
	}
	return pEndpointVolume;
}

std::string AudioManagerWindows::getDeviceName(IMMDevice* device) {
	IPropertyStore *pProps = nullptr;
	PROPVARIANT varString;
	if (device->OpenPropertyStore(STGM_READ, &pProps) >= 0 && pProps->GetValue(PKEY_Device_FriendlyName, &varString) >= 0) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::string name = converter.to_bytes(varString.pwszVal);
		return name;
	}
	return "";
}


HRESULT AudioManagerWindows::QueryInterface(REFIID riid, void ** ppvObject) {
	if (IID_IUnknown == riid) {
		AddRef();
		*ppvObject = (IUnknown*)this;
	} else if (__uuidof(IMMNotificationClient) == riid) {
		AddRef();
		*ppvObject = (IMMNotificationClient*)this;
	} else {
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG AudioManagerWindows::AddRef(void) {
	return 1; // We don't need reference counting
}

ULONG AudioManagerWindows::Release(void) {
	return 1; // We don't need reference counting
}

HRESULT AudioManagerWindows::OnDeviceStateChanged(LPCWSTR, DWORD) {
	return S_OK;
}

HRESULT AudioManagerWindows::OnDeviceAdded(LPCWSTR) {
	return S_OK;
}

HRESULT AudioManagerWindows::OnDeviceRemoved(LPCWSTR) {
	return S_OK;
}

HRESULT AudioManagerWindows::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId) {
	if (flow == eCapture && role == eMultimedia) {
		if (audioDeviceEnumerator) {
			auto device = getDevice(audioDeviceEnumerator, pwstrDefaultDeviceId);
			if (device != micAudioDevice) {
				if (micAudioDevice) {
					micAudioDevice->Release();
				}
				micAudioDevice = device;
				if (micAudioDevice) {
					micAudioEndpointVolume = getAudioEndpointVolume(micAudioDevice);
				} else {
					std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
					std::string name = converter.to_bytes(pwstrDefaultDeviceId);
					LOG(WARNING) << "Could not find recording device \"" << name << "\".";
				}
				controller->onNewRecordingDevice();
			}
		}
	}
	/*char* pszFlow = (char*)"?????";
	char* pszRole = (char*)"?????";

	switch (flow) {
	case eRender:
		pszFlow = (char*)"eRender";
		break;
	case eCapture:
		pszFlow = (char*)"eCapture";
		break;
	}

	switch (role) {
	case eConsole:
		pszRole = (char*)"eConsole";
		break;
	case eMultimedia:
		pszRole = (char*)"eMultimedia";
		break;
	case eCommunications:
		pszRole = (char*)"eCommunications";
		break;
	}

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string name = converter.to_bytes(pwstrDefaultDeviceId);
	printf(" %s -->New default device: flow = %s, role = %s\n", name.c_str(), pszFlow, pszRole);
	fflush(stdout);*/
	return S_OK;
}

HRESULT AudioManagerWindows::OnPropertyValueChanged(LPCWSTR, const PROPERTYKEY) {
	return S_OK;
}

}
