#pragma once

#include "AudioManager.h"
#include <pulse/pulseaudio.h>

namespace advsettings
{
class AudioManagerPulse : public AudioManager
{
public:
    virtual void init( AudioTabController* controller ) override;
    ~AudioManagerPulse() override;

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

private:
    AudioTabController* m_controller;
};

} // namespace advsettings
