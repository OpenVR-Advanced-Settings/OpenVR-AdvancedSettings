#include "AudioManagerPulse.h"
#include "../AudioTabController.h"

#include <cstdint>
#include <pulse/pulseaudio.h>
#include <string>
#include <QtLogging>
#include <QtDebug>
#include "AudioManager.h"

namespace advsettings
{
using std::string;

enum PulseAudioIsLastMeaning : std::uint8_t
{
    Error,
    RealDevice,
    PreviousDeviceWasLastReal,
};

enum PulseAudioLoopControl : std::uint8_t
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

namespace
{
    PulseAudioLoopControl loopControl = PulseAudioLoopControl::Run;
} // namespace

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
    if ( pulseAudioData.sinkOutputDevices.empty() )
        qCritical() << "\tOutput devices size zero.";
    for ( const auto& device : pulseAudioData.sinkOutputDevices )
    {
        qCritical() << "\tDevice Name: " << device.name();
        qCritical() << "\tDevice Id: " << device.id();
    }

    qCritical() << "";

    qCritical() << "sourceInputDevices: ";
    if ( pulseAudioData.sourceInputDevices.empty() )
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

std::string getDeviceName( pa_proplist* prop_list )
{
    if ( !prop_list )
    {
        qCritical() << "proplist not valid.";
    }

    constexpr auto deviceDescription = "device.description";
    if ( !pa_proplist_contains( prop_list, deviceDescription ) )
    {
        qCritical() << "proplist does not contain '" << deviceDescription
                    << "'.";
        return "ERROR";
    }

    return { pa_proplist_gets( prop_list, deviceDescription ) };
}

template <class T> void deviceCallback( const T* info, const int isLast )
{
    static_assert(
        std::is_same_v<pa_source_info, T> || std::is_same_v<pa_sink_info, T>,
        "Function should only be used with pa_source_info or pa_sink_info." );

    const auto deviceState = getIsLastMeaning( isLast );
    if ( deviceState == PulseAudioIsLastMeaning::PreviousDeviceWasLastReal )
    {
        loopControl = PulseAudioLoopControl::Stop;
        return;
    }
    if ( deviceState == PulseAudioIsLastMeaning::Error )
    {
        qCritical() << "Error in deviceCallback function.";
        dumpPulseAudioState();
        loopControl = PulseAudioLoopControl::Stop;
        return;
    }

    if constexpr ( std::is_same_v<pa_source_info, T> )
    {
        if ( info->name == pulseAudioData.defaultSourceInputDeviceId )
        {
            pulseAudioData.currentDefaultSourceInfo = *info;
        }

        /*
        qDebug() << "Adding device to input: '" << i->name << "', '"
                 << getDeviceName( i->proplist ) << "'.";
                 */
        pulseAudioData.sourceInputDevices.push_back(
            AudioDevice( info->name, getDeviceName( info->proplist ) ) );
    }

    else if constexpr ( std::is_same_v<pa_sink_info, T> )
    {
        if ( info->name == pulseAudioData.defaultSinkOutputDeviceId )
        {
            pulseAudioData.currentDefaultSinkInfo = *info;
        }

        /*
        qDebug() << "Adding device to output: '" << i->name << "', '"
                 << getDeviceName( i->proplist ) << "'.";
                 */
        pulseAudioData.sinkOutputDevices.push_back(
            AudioDevice( info->name, getDeviceName( info->proplist ) ) );
    }
}

void setInputDevicesCallback( pa_context* /*ctx*/,
                              const pa_source_info* info,
                              int isLast,
                              void* /*userdata*/ )
{
    deviceCallback( info, isLast );
}

void setOutputDevicesCallback( pa_context* /*ctx*/,
                               const pa_sink_info* out_info,
                               int isLast,
                               void* /*userdata*/ )
{
    deviceCallback( out_info, isLast );
}

void getDefaultDevicesCallback( pa_context* /*ctx*/,
                                const pa_server_info* out_info,
                                void* /*userdata*/ )
{
    if ( !out_info )
    {
        qCritical() << "i == 0";
        pulseAudioData.defaultSinkOutputDeviceId = "DDO:ERROR";
        pulseAudioData.defaultSourceInputDeviceId = "DDI:ERROR";
        return;
    }

    // Copy because we don't know how long the pa_server_info* lives for
    pulseAudioData.defaultSinkOutputDeviceId.assign(
        out_info->default_sink_name );
    pulseAudioData.defaultSourceInputDeviceId.assign(
        out_info->default_source_name );

    loopControl = PulseAudioLoopControl::Stop;

    /*
    qDebug() << "getDefaultDevicesCallback done with sink output device: '"
             << pulseAudioData.defaultSinkOutputDeviceId
             << "' and source input '"
             << pulseAudioData.defaultSourceInputDeviceId << "'.";
             */
}

void stateCallbackFunction( pa_context* ctx, void* /*userdata*/ )
{
    switch ( pa_context_get_state( ctx ) )
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

void successCallback( pa_context* /*ctx*/, int success, void* successVariable )
{
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

void setPlaybackDeviceInternal( const std::string& dev_id )
{
    updateAllPulseData();

    auto success = false;
    pa_context_set_default_sink(
        pulseAudioPointers.context, dev_id.c_str(), successCallback, &success );

    customPulseLoop();

    if ( !success )
    {
        qCritical() << "setPlaybackDeviceInternal failed to set default sink "
                       "for device '"
                    << dev_id << "'.";
    }

    qDebug() << "setPlaybackDeviceInternal done with id: " << dev_id;
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

    const auto valid = !pulseAudioData.defaultSourceInputDeviceId.empty();

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

void sourceOutputCallback( pa_context* ctx,
                           const pa_source_output_info* info,
                           int isLast,
                           void* success )
{
    const auto deviceState = getIsLastMeaning( isLast );
    if ( deviceState == PulseAudioIsLastMeaning::PreviousDeviceWasLastReal )
    {
        loopControl = PulseAudioLoopControl::Stop;
        return;
    }
    if ( deviceState == PulseAudioIsLastMeaning::Error )
    {
        qCritical() << "Error in sourceOutputCallback function.";
        dumpPulseAudioState();
        return;
    }

    const auto sourceOutputIndex = info->index;
    const auto sourceIndex = pulseAudioData.currentDefaultSourceInfo.index;

    qDebug() << "Attempting to move sourceOutputIndex: '" << sourceOutputIndex
             << "' to sourceIndex '" << sourceIndex
             << "' with source output name " << info->name << ".";

    pa_context_move_source_output_by_index(
        ctx, sourceOutputIndex, sourceIndex, successCallback, success );
}

void setMicrophoneDevice( const std::string& dev_id )
{
    qDebug() << "setMicrophoneDevice called with 'id': " << dev_id;

    updateAllPulseData();

    auto success = false;
    pa_context_set_default_source(
        pulseAudioPointers.context, dev_id.c_str(), successCallback, &success );

    customPulseLoop();

    if ( !success )
    {
        qCritical() << "Error setting microphone device for '" << dev_id
                    << "'.";
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

void AudioManagerPulse::setPlaybackDevice( const std::string& dev_id,
                                           bool notify )
{
    setPlaybackDeviceInternal( dev_id );

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

void AudioManagerPulse::setMirrorDevice( const std::string& /*dev_id*/,
                                         bool /*notify*/ )
{
    // noop
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

bool AudioManagerPulse::setMirrorVolume( float /*value*/ )
{
    return false;
}

bool AudioManagerPulse::getMirrorMuted()
{
    return true;
}

bool AudioManagerPulse::setMirrorMuted( bool /*value*/ )
{
    return false;
}

bool AudioManagerPulse::isMicValid()
{
    return isMicrophoneValid();
}

void AudioManagerPulse::setMicDevice( const std::string& dev_id, bool notify )
{
    setMicrophoneDevice( dev_id );

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
