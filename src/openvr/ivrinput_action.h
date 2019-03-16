#pragma once
#include <openvr.h>
#include <easylogging++.h>

namespace input
{
/*!
The type of function that should be called to get data for the action.
The IVRInput API does not give explicit warnings when the wrong functions are
called. This enum is an attempt at runtime checking.
*/
enum class ActionType
{
    Undefined,
    Digital,
    Analog,
    Haptic,
};

/*!
Represents a single action from the actions manifest. Should be instantiated
with a name similar to that of the one in the actions manifest.
*/
class Action
{
public:
    Action( const char* const actionName ) : m_name( actionName )
    {
        auto error = vr::VRInput()->GetActionHandle( actionName, &m_handle );

        if ( error != vr::EVRInputError::VRInputError_None )
        {
            LOG( ERROR ) << "Error getting handle for '" << actionName
                         << "'. OpenVR Error: " << error;
        }
    }
    /*!
    An API internal handle that identifies an action.
    */
    vr::VRActionHandle_t handle() const noexcept
    {
        return m_handle;
    }
    /*!
    The actions manfiest name of the action. Used for error reporting.
    */
    std::string name() const noexcept
    {
        return m_name;
    }

private:
    vr::VRActionHandle_t m_handle = 0;
    const std::string m_name;
};

class DigitalAction : public Action
{
public:
    DigitalAction( const char* const actionName ) : Action( actionName ) {}
};

class AnalogAction : public Action
{
public:
    AnalogAction( const char* const actionName ) : Action( actionName ) {}
};

} // namespace input
