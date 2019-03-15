#pragma once
#include <openvr.h>
#include <easylogging++.h>

namespace input
{
/*!
Represents an input source i.e. right controller
*/
class InputSource
{
public:
    InputSource( const char* const inputSourceName ) : m_name( inputSourceName )
    {
        auto error
            = vr::VRInput()->GetInputSourceHandle( inputSourceName, &m_handle );

        if ( error != vr::EVRInputError::VRInputError_None )
        {
            LOG( ERROR ) << "Error getting input handle for '"
                         << inputSourceName
                         << "'. OpenVR Input Error: " << error;
        }
    }
    /*!
    An API internal handle that identifies an input handle.
    */
    vr::VRInputValueHandle_t handle() const noexcept
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
    vr::VRInputValueHandle_t m_handle = 0;
    const std::string m_name;
};

} // namespace input
