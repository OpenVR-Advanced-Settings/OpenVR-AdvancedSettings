#pragma once
#include <openvr.h>
#include <easylogging++.h>

namespace input
{
/*!
Represents an action set from the action manifest.
*/
class ActionSet
{
public:
    ActionSet( const char* setName )
    {
        vr::VRActionSetHandle_t handle = 0;
        auto error = vr::VRInput()->GetActionSetHandle( setName, &handle );
        if ( error != vr::EVRInputError::VRInputError_None )
        {
            LOG( ERROR ) << "Error getting handle for '" << setName
                         << "'. OpenVR Error: " << error;
        }

        m_activeActionSet.ulActionSet = handle;
        m_activeActionSet.ulRestrictedToDevice
            = vr::k_ulInvalidInputValueHandle;
        m_activeActionSet.nPriority = 0;
    }
    void setPriority( int32_t priority )
    {
        m_activeActionSet.nPriority = priority;
    }

    vr::VRActiveActionSet_t activeActionSet()
    {
        return m_activeActionSet;
    }

private:
    vr::VRActiveActionSet_t m_activeActionSet = {};
};

} // namespace input
