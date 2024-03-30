#include "ovr_application_wrapper.h"
#include <QtLogging>
#include <QtDebug>
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
        qWarning() << "Could not Get App ID by PID: " + std::to_string( error );
        qWarning() << "Attempting to find through Enumeration";
        auto appCount = vr::VRApplications()->GetApplicationCount();
        const uint32_t bufferMax = vr::k_unMaxApplicationKeyLength;
        char cStringOut[bufferMax];
        for ( uint32_t i = 0; i < appCount; i++ )
        {
            auto error2 = vr::VRApplications()->GetApplicationKeyByIndex(
                i, cStringOut, bufferMax );
            std::string appID( cStringOut );
            if ( appID.find( "steam.app" ) != std::string::npos )
            {
                qWarning() << "Found App ID through Enumeration (only "
                              "steam apps caught): "
                           << appID;
                if ( error2 == vr::VRApplicationError_None )
                {
                    return appID;
                }
                qWarning() << "Error getting app id via Enum: " << error2;
            }
        }
        qCritical() << "Unable to Find App ID by PID or by Enumeration";
        return "error";
    }
    return keys;
}

} // namespace ovr_application_wrapper
