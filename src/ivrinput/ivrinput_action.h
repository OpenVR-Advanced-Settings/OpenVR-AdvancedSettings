#pragma once
#include <openvr.h>
#include <easylogging++.h>

enum class ActionType
{
    Undefined,
    Digital,
};

class Action
{
public:
    Action( const char* const actionName, const ActionType type )
        : m_name( actionName ), m_type( type )
    {
        auto error = vr::VRInput()->GetActionHandle( actionName, &m_handle );

        if ( error != vr::EVRInputError::VRInputError_None )
        {
            LOG( ERROR ) << "Error getting handle for '" << actionName
                         << "'. OpenVR Error: " << error;
        }
    }
    vr::VRActionHandle_t handle()
    {
        return m_handle;
    }
    const char* name()
    {
        return m_name;
    }
    ActionType type()
    {
        return m_type;
    }

private:
    vr::VRActionHandle_t m_handle = 0;
    const char* m_name;
    ActionType m_type = ActionType::Undefined;
};
