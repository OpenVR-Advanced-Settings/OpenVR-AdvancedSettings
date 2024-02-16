#pragma once
#include <pulse/pulseaudio.h>
#include <string>
#include <QtLogging>
#include <QtDebug>
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
    qCritical() << "____";

    qCritical() << "Dumping PulseAudio state: ";
    qCritical() << "mainLoop: " << pulseAudioPointers.mainLoop;
    qCritical() << "api: " << pulseAudioPointers.api;
    qCritical() << "context: " << pulseAudioPointers.context;

    qCritical() << "";

    qCritical() << "Data:";
    qCritical() << "\tdefaultSinkOutputDeviceId: "
                << pulseAudioData.defaultSinkOutputDeviceId;
    qCritical() << "\tdefaultSourceInputDeviceId: "
                << pulseAudioData.defaultSourceInputDeviceId;

    qCritical() << "";

    qCritical() << "\tcurrentDefaultSinkInfo name: "
                << pulseAudioData.currentDefaultSinkInfo.name;
    qCritical() << "\tcurrentDefaultSourceInfo name: "
                << pulseAudioData.currentDefaultSourceInfo.name;

    qCritical() << "";

    qCritical() << "sinkOutputDevices: ";
    if ( pulseAudioData.sinkOutputDevices.size() == 0 )
        qCritical() << "\tOutput devices size zero.";
    for ( const auto& device : pulseAudioData.sinkOutputDevices )
    {
        qCritical() << "\tDevice Name: " << device.name();
        qCritical() << "\tDevice Id: " << device.id();
    }

    qCritical() << "";

    qCritical() << "sourceInputDevices: ";
    if ( pulseAudioData.sourceInputDevices.size() == 0 )
        qCritical() << "\tInput devices size zero.";
    for ( const auto& device : pulseAudioData.sourceInputDevices )
    {
        qCritical() << "\tDevice Name: " << device.name();
        qCritical() << "\tDevice Id: " << device.id();
    }

    qCritical() << "____";
}

PulseAudioIsLastMeaning getIsLastMeaning( const int isLast ) noexcept
{
    if ( isLast < 0 )
    {
        qCritical() << "Error in isLast.";
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
        qCritical() << "proplist not valid.";
    }

    constexpr auto deviceDescription = "device.description";
    if ( !pa_proplist_contains( p, deviceDescription ) )
    {
        qCritical() << "proplist does not contain '" << deviceDescription
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
        qCritical() << "Error in deviceCallback function.";
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

        qDebug() << "Adding device to input: '" << i->name << "', '"
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

        qDebug() << "Adding device to output: '" << i->name << "', '"
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
        qCritical() << "i == 0";
        pulseAudioData.defaultSinkOutputDeviceId = "DDO:ERROR";
        pulseAudioData.defaultSourceInputDeviceId = "DDI:ERROR";
        return;
    }

    // Copy because we don't know how long the pa_server_info* lives for
    pulseAudioData.defaultSinkOutputDeviceId.assign( i->default_sink_name );
    pulseAudioData.defaultSourceInputDeviceId.assign( i->default_source_name );

    loopControl = PulseAudioLoopControl::Stop;

    qDebug() << "getDefaultDevicesCallback done with sink output device: '"
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
        qCritical() << "PA_CONTEXT_TERMINATED in stateCallbackFunction";
        dumpPulseAudioState();
        return;
    case PA_CONTEXT_CONNECTING:
        qDebug() << "PA_CONTEXT_CONNECTING";
        return;
    case PA_CONTEXT_AUTHORIZING:
        qDebug() << "PA_CONTEXT_AUTHORIZING";
        return;
    case PA_CONTEXT_SETTING_NAME:
        qDebug() << "PA_CONTEXT_SETTING_NAME";
        return;
    case PA_CONTEXT_UNCONNECTED:
        qDebug() << "PA_CONTEXT_UNCONNECTED";
        return;
    case PA_CONTEXT_FAILED:
        qDebug() << "PA_CONTEXT_FAILED";
        return;

    case PA_CONTEXT_READY:
        qDebug() << "PA_CONTEXT_READY";
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

    qDebug() << "updateAllPulseData done.";
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
        qCritical() << "Non successful callback operation.";
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
        qCritical() << "setPlaybackDeviceInternal failed to set default sink "
                       "for device '"
                    << id << "'.";
    }

    qDebug() << "setPlaybackDeviceInternal done with id: " << id;
}

std::string getCurrentDefaultPlaybackDeviceName()
{
    updateAllPulseData();

    for ( const auto& dev : pulseAudioData.sinkOutputDevices )
    {
        if ( dev.id() == pulseAudioData.defaultSinkOutputDeviceId )
        {
            qDebug() << "getCurrentDefaultPlaybackDeviceName done with "
                     << dev.name();
            return dev.name();
        }
    }
    qCritical() << "Unable to find default playback device.";

    return "ERROR";
}

std::string getCurrentDefaultPlaybackDeviceId()
{
    updateAllPulseData();

    qDebug() << "getCurrentDefaultPlaybackDeviceId done with "
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
            qDebug() << "getCurrentDefaultRecordingDeviceName done with: "
                     << dev.name();
            return dev.name();
        }
    }
    qCritical() << "Unable to find default playback device.";

    return "ERROR";
}

std::string getCurrentDefaultRecordingDeviceId()
{
    updateAllPulseData();

    qDebug() << "getCurrentDefaultRecordingDeviceId done with "
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
        qCritical() << "Error in sourceOutputCallback function.";
        dumpPulseAudioState();
        return;
    }

    const auto sourceOutputIndex = i->index;
    const auto sourceIndex = pulseAudioData.currentDefaultSourceInfo.index;

    qDebug() << "Attempting to move sourceOutputIndex: '" << sourceOutputIndex
             << "' to sourceIndex '" << sourceIndex
             << "' with source output name " << i->name << ".";

    pa_context_move_source_output_by_index(
        c, sourceOutputIndex, sourceIndex, successCallback, &success );
}

void setMicrophoneDevice( const std::string& id )
{
    qDebug() << "setMicrophoneDevice called with 'id': " << id;

    updateAllPulseData();

    auto success = false;
    pa_context_set_default_source(
        pulseAudioPointers.context, id.c_str(), successCallback, &success );

    customPulseLoop();

    if ( !success )
    {
        qCritical() << "Error setting microphone device for '" << id << "'.";
    }

    updateAllPulseData();

    pa_context_get_source_output_info_list(
        pulseAudioPointers.context, sourceOutputCallback, &success );

    customPulseLoop();

    if ( !success )
    {
        qCritical() << "Error in moving source outputs to new source.";
    }

    updateAllPulseData();

    qDebug() << "setMicrophoneDevice done.";
}

bool setPlaybackVolume( const float volume )
{
    qDebug() << "setPlaybackVolume called with 'volume': " << volume;

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
        qCritical() << "setPlaybackVolume failed to set volume '" << volume
                    << "' for device '"
                    << pulseAudioData.defaultSinkOutputDeviceId << "'.";
    }

    qDebug() << "setPlaybackVolume done with 'success': " << success;

    return success;
}

bool setMicrophoneVolume( const float volume )
{
    qDebug() << "setMicrophoneVolume called with 'volume': " << volume;

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
        qCritical() << "seMicrophoneVolume failed to set volume '" << volume
                    << "' for device '"
                    << pulseAudioData.defaultSourceInputDeviceId << "'.";
    }

    qDebug() << "setMicrophoneVolume done with 'success': " << success;

    return success;
}

bool setMicMuteState( const bool muted )
{
    qDebug() << "setMicMuteState called with 'muted': " << muted;
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
        qCritical() << "setMicMuteState failed to set muted '" << muted
                    << "' for device '"
                    << pulseAudioData.defaultSourceInputDeviceId << "'.";
    }

    qDebug() << "setMicMuteState done with 'success': " << success;

    return success;
}

void restorePulseAudioState()
{
    qDebug() << "restorePulseAudioState called.";

    setPlaybackDeviceInternal( pulseAudioData.originalDefaultOutputDeviceId );
    setPlaybackVolume( pulseAudioData.originalDefaultOutputDeviceVolume );

    setMicrophoneDevice( pulseAudioData.originalDefaultInputDeviceId );
    setMicrophoneVolume( pulseAudioData.originalDefaultInputDeviceVolume );

    qDebug() << "restorePulseAudioState done.";
}

void initializePulseAudio()
{
    qDebug() << "initializePulseAudio called.";

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

    qDebug() << "initializePulseAudio finished.";
}
} // namespace advsettings
