#pragma once
#include <pulse/pulseaudio.h>
#include <string>
#include <easylogging++.h>
#include "AudioManager.h"

// Used to get the compiler to shut up about C4100: unreferenced formal
// parameter. The cast is to get GCC to shut up about it.
#define UNREFERENCED_PARAMETER( P ) static_cast<void>( ( P ) )

namespace advsettings
{
enum class PulseAudioIsLastMeaning
{
    Error,
    RealDevice,
    PreviousDeviceWasLastReal,
};

enum class PulseAudioLoopControl
{
    Stop,
    Run,
};

struct
{
    pa_mainloop* mainLoop;
    pa_mainloop_api* api;
    pa_context* context;
} pulseAudioPointers;

struct
{
    std::string defaultSinkOutputDeviceId;
    std::string defaultSourceInputDeviceId;

    std::string originalDefaultOutputDeviceId;
    std::string originalDefaultInputDeviceId;

    float originalDefaultOutputDeviceVolume;
    float originalDefaultInputDeviceVolume;

    std::vector<AudioDevice> sinkOutputDevices;
    std::vector<AudioDevice> sourceInputDevices;

    pa_sink_info currentDefaultSinkInfo;
    pa_source_info currentDefaultSourceInfo;
} pulseAudioData;

static PulseAudioLoopControl loopControl = PulseAudioLoopControl::Run;

void customPulseLoop()
{
    while ( loopControl == PulseAudioLoopControl::Run )
    {
        constexpr auto noReturnValue = nullptr;
        constexpr auto blockForEvents = 1;
        pa_mainloop_iterate(
            pulseAudioPointers.mainLoop, blockForEvents, noReturnValue );
    }

    loopControl = PulseAudioLoopControl::Run;
}

// Error function
void dumpPulseAudioState()
{
    LOG( ERROR ) << "____";

    LOG( ERROR ) << "Dumping PulseAudio state: ";
    LOG( ERROR ) << "mainLoop: " << pulseAudioPointers.mainLoop;
    LOG( ERROR ) << "api: " << pulseAudioPointers.api;
    LOG( ERROR ) << "context: " << pulseAudioPointers.context;

    LOG( ERROR ) << "";

    LOG( ERROR ) << "Data:";
    LOG( ERROR ) << "\tdefaultSinkOutputDeviceId: "
                 << pulseAudioData.defaultSinkOutputDeviceId;
    LOG( ERROR ) << "\tdefaultSourceInputDeviceId: "
                 << pulseAudioData.defaultSourceInputDeviceId;

    LOG( ERROR ) << "";

    LOG( ERROR ) << "\tcurrentDefaultSinkInfo name: "
                 << pulseAudioData.currentDefaultSinkInfo.name;
    LOG( ERROR ) << "\tcurrentDefaultSourceInfo name: "
                 << pulseAudioData.currentDefaultSourceInfo.name;

    LOG( ERROR ) << "";

    LOG( ERROR ) << "sinkOutputDevices: ";
    LOG_IF( pulseAudioData.sinkOutputDevices.size() == 0, ERROR )
        << "\tOutput devices size zero.";
    for ( const auto& device : pulseAudioData.sinkOutputDevices )
    {
        LOG( ERROR ) << "\tDevice Name: " << device.name();
        LOG( ERROR ) << "\tDevice Id: " << device.id();
    }

    LOG( ERROR ) << "";

    LOG( ERROR ) << "sourceInputDevices: ";
    LOG_IF( pulseAudioData.sourceInputDevices.size() == 0, ERROR )
        << "\tInput devices size zero.";
    for ( const auto& device : pulseAudioData.sourceInputDevices )
    {
        LOG( ERROR ) << "\tDevice Name: " << device.name();
        LOG( ERROR ) << "\tDevice Id: " << device.id();
    }

    LOG( ERROR ) << "____";
}

PulseAudioIsLastMeaning getIsLastMeaning( const int isLast ) noexcept
{
    if ( isLast < 0 )
    {
        LOG( ERROR ) << "Error in isLast.";
        dumpPulseAudioState();
        return PulseAudioIsLastMeaning::Error;
    }

    if ( isLast > 0 )
    {
        return PulseAudioIsLastMeaning::PreviousDeviceWasLastReal;
    }

    return PulseAudioIsLastMeaning::RealDevice;
}

std::string getDeviceName( pa_proplist* p )
{
    if ( !p )
    {
        LOG( ERROR ) << "proplist not valid.";
    }

    constexpr auto deviceDescription = "device.description";
    if ( !pa_proplist_contains( p, deviceDescription ) )
    {
        LOG( ERROR ) << "proplist does not contain '" << deviceDescription
                     << "'.";
        return "ERROR";
    }

    std::string s;
    s.assign( pa_proplist_gets( p, deviceDescription ) );

    return s;
}

template <class T> void deviceCallback( const T* i, const int isLast )
{
    static_assert(
        std::is_same<pa_source_info, T>::value
            || std::is_same<pa_sink_info, T>::value,
        "Function should only be used with pa_source_info or pa_sink_info." );

    const auto deviceState = getIsLastMeaning( isLast );
    if ( deviceState == PulseAudioIsLastMeaning::PreviousDeviceWasLastReal )
    {
        loopControl = PulseAudioLoopControl::Stop;
        return;
    }
    else if ( deviceState == PulseAudioIsLastMeaning::Error )
    {
        LOG( ERROR ) << "Error in deviceCallback function.";
        dumpPulseAudioState();
        loopControl = PulseAudioLoopControl::Stop;
        return;
    }

    if constexpr ( std::is_same<pa_source_info, T>::value )
    {
        if ( i->name == pulseAudioData.defaultSourceInputDeviceId )
        {
            pulseAudioData.currentDefaultSourceInfo = *i;
        }

        LOG( DEBUG ) << "Adding device to input: '" << i->name << "', '"
                     << getDeviceName( i->proplist ) << "'.";
        pulseAudioData.sourceInputDevices.push_back(
            AudioDevice( i->name, getDeviceName( i->proplist ) ) );
    }

    else if constexpr ( std::is_same<pa_sink_info, T>::value )
    {
        if ( i->name == pulseAudioData.defaultSinkOutputDeviceId )
        {
            pulseAudioData.currentDefaultSinkInfo = *i;
        }

        LOG( DEBUG ) << "Adding device to output: '" << i->name << "', '"
                     << getDeviceName( i->proplist ) << "'.";
        pulseAudioData.sinkOutputDevices.push_back(
            AudioDevice( i->name, getDeviceName( i->proplist ) ) );
    }
}

void setInputDevicesCallback( pa_context* c,
                              const pa_source_info* i,
                              int isLast,
                              void* userdata )
{
    UNREFERENCED_PARAMETER( userdata );
    UNREFERENCED_PARAMETER( c );

    deviceCallback( i, isLast );
}

void setOutputDevicesCallback( pa_context* c,
                               const pa_sink_info* i,
                               int isLast,
                               void* userdata )
{
    UNREFERENCED_PARAMETER( userdata );
    UNREFERENCED_PARAMETER( c );

    deviceCallback( i, isLast );
}

void getDefaultDevicesCallback( pa_context* c,
                                const pa_server_info* i,
                                void* userdata )
{
    UNREFERENCED_PARAMETER( c );
    UNREFERENCED_PARAMETER( userdata );

    if ( !i )
    {
        LOG( ERROR ) << "i == 0";
        pulseAudioData.defaultSinkOutputDeviceId = "DDO:ERROR";
        pulseAudioData.defaultSourceInputDeviceId = "DDI:ERROR";
        return;
    }

    // Copy because we don't know how long the pa_server_info* lives for
    pulseAudioData.defaultSinkOutputDeviceId.assign( i->default_sink_name );
    pulseAudioData.defaultSourceInputDeviceId.assign( i->default_source_name );

    loopControl = PulseAudioLoopControl::Stop;

    LOG( DEBUG ) << "getDefaultDevicesCallback done with sink output device: '"
                 << pulseAudioData.defaultSinkOutputDeviceId
                 << "' and source input '"
                 << pulseAudioData.defaultSourceInputDeviceId << "'.";
}

void stateCallbackFunction( pa_context* c, void* userdata )
{
    UNREFERENCED_PARAMETER( c );
    UNREFERENCED_PARAMETER( userdata );

    switch ( pa_context_get_state( c ) )
    {
    case PA_CONTEXT_TERMINATED:
        LOG( ERROR ) << "PA_CONTEXT_TERMINATED in stateCallbackFunction";
        dumpPulseAudioState();
        return;
    case PA_CONTEXT_CONNECTING:
        LOG( DEBUG ) << "PA_CONTEXT_CONNECTING";
        return;
    case PA_CONTEXT_AUTHORIZING:
        LOG( DEBUG ) << "PA_CONTEXT_AUTHORIZING";
        return;
    case PA_CONTEXT_SETTING_NAME:
        LOG( DEBUG ) << "PA_CONTEXT_SETTING_NAME";
        return;
    case PA_CONTEXT_UNCONNECTED:
        LOG( DEBUG ) << "PA_CONTEXT_UNCONNECTED";
        return;
    case PA_CONTEXT_FAILED:
        LOG( DEBUG ) << "PA_CONTEXT_FAILED";
        return;

    case PA_CONTEXT_READY:
        LOG( DEBUG ) << "PA_CONTEXT_READY";
        loopControl = PulseAudioLoopControl::Stop;
        return;
    }
}

void updateAllPulseData()
{
    constexpr auto noCustomUserdata = nullptr;

    pa_context_get_server_info( pulseAudioPointers.context,
                                getDefaultDevicesCallback,
                                noCustomUserdata );
    customPulseLoop();

    pulseAudioData.sinkOutputDevices.clear();
    pa_context_get_sink_info_list( pulseAudioPointers.context,
                                   setOutputDevicesCallback,
                                   noCustomUserdata );
    customPulseLoop();

    pulseAudioData.sourceInputDevices.clear();
    pa_context_get_source_info_list(
        pulseAudioPointers.context, setInputDevicesCallback, noCustomUserdata );
    customPulseLoop();

    LOG( DEBUG ) << "updateAllPulseData done.";
}

void successCallback( pa_context* c, int success, void* successVariable )
{
    UNREFERENCED_PARAMETER( c );

    if ( successVariable != nullptr )
    {
        *static_cast<bool*>( successVariable ) = success;
    }

    if ( !success )
    {
        LOG( ERROR ) << "Non successful callback operation.";
        dumpPulseAudioState();
    }

    loopControl = PulseAudioLoopControl::Stop;
}

void setPlaybackDeviceInternal( const std::string& id )
{
    updateAllPulseData();

    auto success = false;
    pa_context_set_default_sink(
        pulseAudioPointers.context, id.c_str(), successCallback, &success );

    customPulseLoop();

    if ( !success )
    {
        LOG( ERROR ) << "setPlaybackDeviceInternal failed to set default sink "
                        "for device '"
                     << id << "'.";
    }

    LOG( DEBUG ) << "setPlaybackDeviceInternal done with id: " << id;
}

std::string getCurrentDefaultPlaybackDeviceName()
{
    updateAllPulseData();

    for ( const auto& dev : pulseAudioData.sinkOutputDevices )
    {
        if ( dev.id() == pulseAudioData.defaultSinkOutputDeviceId )
        {
            LOG( DEBUG ) << "getCurrentDefaultPlaybackDeviceName done with "
                         << dev.name();
            return dev.name();
        }
    }
    LOG( ERROR ) << "Unable to find default playback device.";

    return "ERROR";
}

std::string getCurrentDefaultPlaybackDeviceId()
{
    updateAllPulseData();

    LOG( DEBUG ) << "getCurrentDefaultPlaybackDeviceId done with "
                 << pulseAudioData.defaultSinkOutputDeviceId;

    return pulseAudioData.defaultSinkOutputDeviceId;
}

std::string getCurrentDefaultRecordingDeviceName()
{
    updateAllPulseData();

    for ( const auto& dev : pulseAudioData.sourceInputDevices )
    {
        if ( dev.id() == pulseAudioData.defaultSourceInputDeviceId )
        {
            LOG( DEBUG ) << "getCurrentDefaultRecordingDeviceName done with: "
                         << dev.name();
            return dev.name();
        }
    }
    LOG( ERROR ) << "Unable to find default playback device.";

    return "ERROR";
}

std::string getCurrentDefaultRecordingDeviceId()
{
    updateAllPulseData();

    LOG( DEBUG ) << "getCurrentDefaultRecordingDeviceId done with "
                 << pulseAudioData.defaultSourceInputDeviceId;

    return pulseAudioData.defaultSourceInputDeviceId;
}

std::vector<AudioDevice> returnRecordingDevices()
{
    updateAllPulseData();

    return pulseAudioData.sourceInputDevices;
}

std::vector<AudioDevice> returnPlaybackDevices()
{
    updateAllPulseData();

    return pulseAudioData.sinkOutputDevices;
}

bool isMicrophoneValid()
{
    updateAllPulseData();

    const auto valid = pulseAudioData.defaultSourceInputDeviceId != "";

    return valid;
}

float getMicrophoneVolume()
{
    updateAllPulseData();

    const auto linearVolume = pa_sw_volume_to_linear(
        pa_cvolume_avg( &pulseAudioData.currentDefaultSourceInfo.volume ) );

    return static_cast<float>( linearVolume );
}

bool getMicrophoneMuted()
{
    updateAllPulseData();

    return pulseAudioData.currentDefaultSourceInfo.mute;
}

void sourceOutputCallback( pa_context* c,
                           const pa_source_output_info* i,
                           int isLast,
                           void* success )
{
    const auto deviceState = getIsLastMeaning( isLast );
    if ( deviceState == PulseAudioIsLastMeaning::PreviousDeviceWasLastReal )
    {
        loopControl = PulseAudioLoopControl::Stop;
        return;
    }
    else if ( deviceState == PulseAudioIsLastMeaning::Error )
    {
        LOG( ERROR ) << "Error in sourceOutputCallback function.";
        dumpPulseAudioState();
        return;
    }

    const auto sourceOutputIndex = i->index;
    const auto sourceIndex = pulseAudioData.currentDefaultSourceInfo.index;

    LOG( DEBUG ) << "Attempting to move sourceOutputIndex: '"
                 << sourceOutputIndex << "' to sourceIndex '" << sourceIndex
                 << "' with source output name " << i->name << ".";

    pa_context_move_source_output_by_index(
        c, sourceOutputIndex, sourceIndex, successCallback, &success );
}

void setMicrophoneDevice( const std::string& id )
{
    LOG( DEBUG ) << "setMicrophoneDevice called with 'id': " << id;

    updateAllPulseData();

    auto success = false;
    pa_context_set_default_source(
        pulseAudioPointers.context, id.c_str(), successCallback, &success );

    customPulseLoop();

    if ( !success )
    {
        LOG( ERROR ) << "Error setting microphone device for '" << id << "'.";
    }

    updateAllPulseData();

    pa_context_get_source_output_info_list(
        pulseAudioPointers.context, sourceOutputCallback, &success );

    customPulseLoop();

    if ( !success )
    {
        LOG( ERROR ) << "Error in moving source outputs to new source.";
    }

    updateAllPulseData();

    LOG( DEBUG ) << "setMicrophoneDevice done.";
}

bool setPlaybackVolume( const float volume )
{
    LOG( DEBUG ) << "setPlaybackVolume called with 'volume': " << volume;

    updateAllPulseData();

    auto pulseVolume = pulseAudioData.currentDefaultSinkInfo.volume;
    const auto vol = pa_sw_volume_from_linear( static_cast<double>( volume ) );

    pa_cvolume_set( &pulseVolume, pulseVolume.channels, vol );

    auto success = false;
    pa_context_set_sink_volume_by_name(
        pulseAudioPointers.context,
        pulseAudioData.defaultSinkOutputDeviceId.c_str(),
        &pulseVolume,
        successCallback,
        &success );

    customPulseLoop();

    if ( !success )
    {
        LOG( ERROR ) << "setPlaybackVolume failed to set volume '" << volume
                     << "' for device '"
                     << pulseAudioData.defaultSinkOutputDeviceId << "'.";
    }

    LOG( DEBUG ) << "setPlaybackVolume done with 'success': " << success;

    return success;
}

bool setMicrophoneVolume( const float volume )
{
    LOG( DEBUG ) << "setMicrophoneVolume called with 'volume': " << volume;

    updateAllPulseData();

    auto pulseVolume = pulseAudioData.currentDefaultSourceInfo.volume;
    const auto vol = pa_sw_volume_from_linear( static_cast<double>( volume ) );

    pa_cvolume_set( &pulseVolume, pulseVolume.channels, vol );

    auto success = false;
    pa_context_set_source_volume_by_name(
        pulseAudioPointers.context,
        pulseAudioData.defaultSourceInputDeviceId.c_str(),
        &pulseVolume,
        successCallback,
        &success );

    customPulseLoop();

    if ( !success )
    {
        LOG( ERROR ) << "seMicrophoneVolume failed to set volume '" << volume
                     << "' for device '"
                     << pulseAudioData.defaultSourceInputDeviceId << "'.";
    }

    LOG( DEBUG ) << "setMicrophoneVolume done with 'success': " << success;

    return success;
}

bool setMicMuteState( const bool muted )
{
    LOG( DEBUG ) << "setMicMuteState called with 'muted': " << muted;
    bool success = false;

    pa_context_set_source_mute_by_name(
        pulseAudioPointers.context,
        pulseAudioData.defaultSourceInputDeviceId.c_str(),
        muted,
        successCallback,
        &success );

    customPulseLoop();

    if ( !success )
    {
        LOG( ERROR ) << "setMicMuteState failed to set muted '" << muted
                     << "' for device '"
                     << pulseAudioData.defaultSourceInputDeviceId << "'.";
    }

    LOG( DEBUG ) << "setMicMuteState done with 'success': " << success;

    return success;
}

void restorePulseAudioState()
{
    LOG( DEBUG ) << "restorePulseAudioState called.";

    setPlaybackDeviceInternal( pulseAudioData.originalDefaultOutputDeviceId );
    setPlaybackVolume( pulseAudioData.originalDefaultOutputDeviceVolume );

    setMicrophoneDevice( pulseAudioData.originalDefaultInputDeviceId );
    setMicrophoneVolume( pulseAudioData.originalDefaultInputDeviceVolume );

    LOG( DEBUG ) << "restorePulseAudioState done.";
}

void initializePulseAudio()
{
    LOG( DEBUG ) << "initializePulseAudio called.";

    pulseAudioPointers.mainLoop = pa_mainloop_new();

    pulseAudioPointers.api = pa_mainloop_get_api( pulseAudioPointers.mainLoop );

    pulseAudioPointers.context
        = pa_context_new( pulseAudioPointers.api, "openvr-advanced-settings" );

    constexpr auto noCustomUserdata = nullptr;
    pa_context_set_state_callback(
        pulseAudioPointers.context, stateCallbackFunction, noCustomUserdata );

    constexpr auto useDefaultServer = nullptr;
    constexpr auto useDefaultSpawnApi = nullptr;
    pa_context_connect( pulseAudioPointers.context,
                        useDefaultServer,
                        PA_CONTEXT_NOFLAGS,
                        useDefaultSpawnApi );
    customPulseLoop();

    pulseAudioData.originalDefaultInputDeviceId
        = getCurrentDefaultRecordingDeviceId();

    pulseAudioData.originalDefaultInputDeviceVolume
        = static_cast<float>( pa_sw_volume_to_linear( pa_cvolume_avg(
            &pulseAudioData.currentDefaultSourceInfo.volume ) ) );

    pulseAudioData.originalDefaultOutputDeviceId
        = getCurrentDefaultPlaybackDeviceId();

    pulseAudioData.originalDefaultOutputDeviceVolume
        = static_cast<float>( pa_sw_volume_to_linear(
            pa_cvolume_avg( &pulseAudioData.currentDefaultSinkInfo.volume ) ) );

    LOG( DEBUG ) << "initializePulseAudio finished.";
}
} // namespace advsettings
