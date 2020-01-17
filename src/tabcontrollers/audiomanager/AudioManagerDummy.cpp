#include "AudioManagerDummy.h"

// Used to get the compiler to shut up about C4100: unreferenced formal
// parameter. The cast is to get GCC to shut up about it.
#define UNREFERENCED_PARAMETER( P ) static_cast<void>( ( P ) )

// application namespace
namespace advsettings
{
void AudioManagerDummy::init( AudioTabController* controller )
{
    // noop
    UNREFERENCED_PARAMETER( controller );
}

void AudioManagerDummy::setPlaybackDevice( const std::string& id, bool notify )
{
    // noop
    UNREFERENCED_PARAMETER( id );
    UNREFERENCED_PARAMETER( notify );
}

std::string AudioManagerDummy::getPlaybackDevName()
{
    return "dummy";
}

std::string AudioManagerDummy::getPlaybackDevId()
{
    return "dummy";
}

void AudioManagerDummy::setMirrorDevice( const std::string& id, bool notify )
{
    // noop
    UNREFERENCED_PARAMETER( id );
    UNREFERENCED_PARAMETER( notify );
}

bool AudioManagerDummy::isMirrorValid()
{
    return false;
}

std::string AudioManagerDummy::getMirrorDevName()
{
    return "dummy";
}

std::string AudioManagerDummy::getMirrorDevId()
{
    return "dummy";
}

float AudioManagerDummy::getMirrorVolume()
{
    return 0;
}

bool AudioManagerDummy::setMirrorVolume( float value )
{
    UNREFERENCED_PARAMETER( value );
    return false;
}

bool AudioManagerDummy::getMirrorMuted()
{
    return true;
}

bool AudioManagerDummy::setMirrorMuted( bool value )
{
    UNREFERENCED_PARAMETER( value );
    return false;
}

bool AudioManagerDummy::isMicValid()
{
    return false;
}

void AudioManagerDummy::setMicDevice( const std::string& id, bool notify )
{
    // noop
    UNREFERENCED_PARAMETER( id );
    UNREFERENCED_PARAMETER( notify );
}

std::string AudioManagerDummy::getMicDevName()
{
    return "dummy";
}

std::string AudioManagerDummy::getMicDevId()
{
    return "dummy";
}

float AudioManagerDummy::getMicVolume()
{
    return 0;
}

bool AudioManagerDummy::setMicVolume( float value )
{
    UNREFERENCED_PARAMETER( value );
    return false;
}

bool AudioManagerDummy::getMicMuted()
{
    return true;
}

bool AudioManagerDummy::setMicMuted( bool value )
{
    UNREFERENCED_PARAMETER( value );
    return false;
}

std::vector<AudioDevice> AudioManagerDummy::getRecordingDevices()
{
    return {};
}

std::vector<AudioDevice> AudioManagerDummy::getPlaybackDevices()
{
    return {};
}

} // namespace advsettings
