#include "ovr_application_wrapper.h"

namespace ovr_application_wrapper
{
std::string getSceneAppID()
{
    uint32_t pidScene = vr::VRApplications()->GetCurrentSceneProcessId();
    uint32_t bufferLen = 256;
    char key[bufferLen];
    vr::EVRApplicationError error
        = vr::VRApplications()->GetApplicationKeyByProcessId(
            pidScene, key, bufferLen );
    if ( error != vr::VRApplicationError_None )
    {
        LOG( ERROR ) << "Could not Get App ID by PID";
        return "error";
    }
    return std::string( key );
}

} // namespace ovr_application_wrapper
