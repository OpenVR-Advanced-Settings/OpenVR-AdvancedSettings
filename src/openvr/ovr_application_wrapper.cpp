#include "ovr_application_wrapper.h"
constexpr uint32_t bufferLen = 4096;
namespace ovr_application_wrapper
{
std::string getSceneAppID()
{
    uint32_t pidScene = vr::VRApplications()->GetCurrentSceneProcessId();
    char key[bufferLen];
    vr::EVRApplicationError error
        = vr::VRApplications()->GetApplicationKeyByProcessId(
            pidScene, key, bufferLen );
    std::string keys = key;
    if ( error != vr::VRApplicationError_None )
    {
        LOG( ERROR ) << "Could not Get App ID by PID: "
                            + std::to_string( error );
        return "error";
    }
    return keys;
}

} // namespace ovr_application_wrapper
