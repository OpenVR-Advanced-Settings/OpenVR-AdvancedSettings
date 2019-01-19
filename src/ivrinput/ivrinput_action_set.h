#pragma once
#include <openvr.h>
#include <easylogging++.h>

namespace input
{
class ActionSet
{
public:
    ActionSet( const char* setName ) : m_name( setName )
    {
        auto error = vr::VRInput()->GetActionSetHandle( setName, &m_handle );
        if ( error != vr::EVRInputError::VRInputError_None )
        {
            LOG( ERROR ) << "Error getting handle for '" << setName
                         << "'. OpenVR Error: " << error;
        }
    }
    std::string name()
    {
        return m_name;
    }
    vr::VRActionSetHandle_t handle()
    {
        return m_handle;
    }

private:
    const std::string m_name;
    vr::VRActionSetHandle_t m_handle = 0;
};

} // namespace input
