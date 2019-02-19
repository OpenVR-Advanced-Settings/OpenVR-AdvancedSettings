#pragma once

#include <string>
#include <vector>

// application namespace
namespace advsettings
{
class AudioTabController;

class AudioManager
{
public:
    virtual ~AudioManager() {}

    virtual void init( AudioTabController* controller ) = 0;

    virtual void setPlaybackDevice( const std::string& id, bool notify = true )
        = 0;
    virtual std::string getPlaybackDevName() = 0;
    virtual std::string getPlaybackDevId() = 0;

    virtual void setMirrorDevice( const std::string& id, bool notify = true )
        = 0;
    virtual bool isMirrorValid() = 0;
    virtual std::string getMirrorDevName() = 0;
    virtual std::string getMirrorDevId() = 0;
    virtual float getMirrorVolume() = 0;
    virtual bool setMirrorVolume( float value ) = 0;
    virtual bool getMirrorMuted() = 0;
    virtual bool setMirrorMuted( bool value ) = 0;

    virtual void setMicDevice( const std::string& id, bool notify = true ) = 0;
    virtual bool isMicValid() = 0;
    virtual std::string getMicDevName() = 0;
    virtual std::string getMicDevId() = 0;
    virtual float getMicVolume() = 0;
    virtual bool setMicVolume( float value ) = 0;
    virtual bool getMicMuted() = 0;
    virtual bool setMicMuted( bool value ) = 0;

    virtual std::vector<std::pair<std::string, std::string>>
        getRecordingDevices() = 0;
    virtual std::vector<std::pair<std::string, std::string>>
        getPlaybackDevices() = 0;
};

} // namespace advsettings
