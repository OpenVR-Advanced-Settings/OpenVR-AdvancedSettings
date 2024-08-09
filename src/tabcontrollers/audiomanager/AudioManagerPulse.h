#pragma once

#include "AudioManager.h"
#include <pulse/pulseaudio.h>

namespace advsettings
{
class AudioManagerPulse : public AudioManager
{
public:
    void init( AudioTabController* controller ) override;
    ~AudioManagerPulse() override;

    void setPlaybackDevice( const std::string& dev_id,
                            bool notify = true ) override;
    std::string getPlaybackDevName() override;
    std::string getPlaybackDevId() override;

    void setMirrorDevice( const std::string& dev_id,
                          bool notify = true ) override;
    bool isMirrorValid() override;
    std::string getMirrorDevName() override;
    std::string getMirrorDevId() override;
    float getMirrorVolume() override;
    bool setMirrorVolume( float value ) override;
    bool getMirrorMuted() override;
    bool setMirrorMuted( bool value ) override;

    bool isMicValid() override;
    void setMicDevice( const std::string& dev_id, bool notify = true ) override;
    std::string getMicDevName() override;
    std::string getMicDevId() override;
    float getMicVolume() override;
    bool setMicVolume( float value ) override;
    bool getMicMuted() override;
    bool setMicMuted( bool value ) override;

    std::vector<AudioDevice> getRecordingDevices() override;
    std::vector<AudioDevice> getPlaybackDevices() override;

private:
    AudioTabController* m_controller;
};

} // namespace advsettings
