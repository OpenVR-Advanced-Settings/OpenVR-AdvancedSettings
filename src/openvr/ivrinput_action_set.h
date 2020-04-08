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
    vr::VRActiveActionSet_t activeActionSet()
    {
        return m_activeActionSet;
    }

    void setActionSetPriority( int32_t priorityVal )
    {
        if ( priorityVal > 0x01FFFFFF )
        {
            LOG( ERROR ) << " Action Set Priority set to Reserved Action Set "
                            "Priority: Setting Priority to Max";
            priorityVal = 0x01FFFFFF;
        }
        else if ( priorityVal < 0 )
        {
            LOG( WARNING ) << " Action Set Priority set to < 0 something may "
                              "have gone wrong";
        }
        else if ( priorityVal < 0x01000000 )
        {
            LOG( DEBUG ) << " Priority set to: " << priorityVal
                         << " Priority is not active";
        }
        else
        {
            LOG( DEBUG ) << " Priority set to: " << priorityVal
                         << " Priority is active";
        }
        m_activeActionSet.nPriority = static_cast<int>( priorityVal );
        return;
    }

private:
    vr::VRActiveActionSet_t m_activeActionSet = {};
};

} // namespace input
