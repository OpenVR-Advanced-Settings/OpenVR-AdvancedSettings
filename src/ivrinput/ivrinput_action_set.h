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
    ActionSet( const char* setName ) : m_name( setName )
    {
        auto error = vr::VRInput()->GetActionSetHandle( setName, &m_handle );
        if ( error != vr::EVRInputError::VRInputError_None )
        {
            LOG( ERROR ) << "Error getting handle for '" << setName
                         << "'. OpenVR Error: " << error;
        }
    }
    /*!
    The actions manfiest name of the set. Used for error reporting.
    */
    std::string name()
    {
        return m_name;
    }
    /*!
    An API internal handle that identifies a set.
    */
    vr::VRActionSetHandle_t handle()
    {
        return m_handle;
    }

private:
    const std::string m_name;
    vr::VRActionSetHandle_t m_handle = 0;
};

} // namespace input
