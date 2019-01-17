#ifndef STEAMIVRINPUT_H
#define STEAMIVRINPUT_H

#include <openvr.h>

class SteamIVRInput
{
public:
    void Init( const bool init );
    void Loop();
    bool nextSongSet();

private:
    const std::string m_actionManifestPath
        = "YOUR_PATH_HERE\\action_manifest.json";
    vr::VRActionHandle_t m_nextSongHandler = {};
    vr::VRActionSetHandle_t m_mainSetHandler = {};
    vr::VRActiveActionSet_t m_activeActionSet = {};
    vr::InputDigitalActionData_t m_nextSongData = {};
};

#endif // STEAMIVRINPUT_H
