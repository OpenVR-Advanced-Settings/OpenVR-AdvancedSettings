#pragma once
#include <openvr.h>
#include <easylogging++.h>

namespace input
{
enum class ActionType
{
    Undefined,
    Digital,
    Analog,
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
    vr::VRActionHandle_t handle() const noexcept
    {
        return m_handle;
    }
    std::string name() const noexcept
    {
        return m_name;
    }
    ActionType type() const noexcept
    {
        return m_type;
    }

private:
    vr::VRActionHandle_t m_handle = 0;
    const std::string m_name;
    const ActionType m_type = ActionType::Undefined;
};

} // namespace input
