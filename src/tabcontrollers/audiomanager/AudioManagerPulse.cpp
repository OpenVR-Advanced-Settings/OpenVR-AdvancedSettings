#include <easylogging++.h>
#include "AudioManagerPulse.h"
#include "AudioManagerPulse_internal.h"
#include "../AudioTabController.h"

// Used to get the compiler to shut up about C4100: unreferenced formal
// parameter. The cast is to get GCC to shut up about it.
#define UNREFERENCED_PARAMETER( P ) static_cast<void>( ( P ) )

// application namespace
namespace advsettings
{
AudioManagerPulse::~AudioManagerPulse()
{
    restorePulseAudioState();
}

void AudioManagerPulse::init( AudioTabController* controller )
{
    m_controller = controller;

    initializePulseAudio();
}

void AudioManagerPulse::setPlaybackDevice( const std::string& id, bool notify )
{
    setPlaybackDeviceInternal( id );

    if ( notify )
    {
        m_controller->onNewPlaybackDevice();
    }
}

std::string AudioManagerPulse::getPlaybackDevName()
{
    return getCurrentDefaultPlaybackDeviceName();
}

std::string AudioManagerPulse::getPlaybackDevId()
{
    return getCurrentDefaultPlaybackDeviceId();
}

void AudioManagerPulse::setMirrorDevice( const std::string& id, bool notify )
{
    // noop
    UNREFERENCED_PARAMETER( id );
    UNREFERENCED_PARAMETER( notify );
}

bool AudioManagerPulse::isMirrorValid()
{
    return false;
}

std::string AudioManagerPulse::getMirrorDevName()
{
    return "dummy";
}

std::string AudioManagerPulse::getMirrorDevId()
{
    return "dummy";
}

float AudioManagerPulse::getMirrorVolume()
{
    return 0;
}

bool AudioManagerPulse::setMirrorVolume( float value )
{
    UNREFERENCED_PARAMETER( value );
    return false;
}

bool AudioManagerPulse::getMirrorMuted()
{
    return true;
}

bool AudioManagerPulse::setMirrorMuted( bool value )
{
    UNREFERENCED_PARAMETER( value );
    return false;
}

bool AudioManagerPulse::isMicValid()
{
    return isMicrophoneValid();
}

void AudioManagerPulse::setMicDevice( const std::string& id, bool notify )
{
    setMicrophoneDevice( id );

    if ( notify )
    {
        m_controller->onNewRecordingDevice();
    }
}

std::string AudioManagerPulse::getMicDevName()
{
    return getCurrentDefaultRecordingDeviceName();
}

std::string AudioManagerPulse::getMicDevId()
{
    return getCurrentDefaultRecordingDeviceId();
}

float AudioManagerPulse::getMicVolume()
{
    return getMicrophoneVolume();
}

bool AudioManagerPulse::setMicVolume( float value )
{
    if ( value > 1.0f )
    {
        value = 1.0f;
    }
    else if ( value < 0.0f )
    {
        value = 0.0f;
    }

    return setMicrophoneVolume( value );
}

bool AudioManagerPulse::getMicMuted()
{
    return getMicrophoneMuted();
}

bool AudioManagerPulse::setMicMuted( bool value )
{
    return setMicMuteState( value );
}

std::vector<AudioDevice> AudioManagerPulse::getRecordingDevices()
{
    return returnRecordingDevices();
}

std::vector<AudioDevice> AudioManagerPulse::getPlaybackDevices()
{
    return returnPlaybackDevices();
}

} // namespace advsettings
