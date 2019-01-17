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
    vr::VRActionHandle_t m_nextSongHandler = {};
    vr::VRActionSetHandle_t m_mainSetHandler = {};
    vr::VRActiveActionSet_t m_activeActionSet = {};
    vr::InputDigitalActionData_t m_nextSongData = {};
};

#endif // STEAMIVRINPUT_H
