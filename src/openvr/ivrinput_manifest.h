#pragma once
#include <openvr.h>
#include <QtLogging>
#include <QtDebug>
#include "../utils/paths.h"

namespace input
{
/*!
Initializes IVRInput to point at the correct manifest. Can't be done in the
constructor body since objects being initialized require this to be set.
*/
class Manifest
{
public:
    Manifest()
    {
        const auto actionManifestPath
            = paths::binaryDirectoryFindFile( "action_manifest.json" );

        if ( !actionManifestPath.has_value() )
        {
            qCritical() << "Could not find action manifest. Action manifests "
                           "not initialized.";
            return;
        }

        const auto* const validManifestPath = actionManifestPath->c_str();

        auto error = vr::VRInput()->SetActionManifestPath( validManifestPath );
        if ( error != vr::EVRInputError::VRInputError_None )
        {
            qCritical() << "Error setting action manifest path: "
                        << validManifestPath << ". OpenVR Error: " << error;
        }
    }

    // Explicitly deleted because this object should do nothing but run the
    // constructor during initialization.
    // Destructor left blank on purpose since there are no terminating calls.
    ~Manifest() {}
    // Copy constructor
    Manifest( const Manifest& ) = delete;
    // Copy assignment
    Manifest& operator=( const Manifest& ) = delete;
    // Move constructor
    Manifest( const Manifest&& ) = delete;
    // Move assignment
    Manifest& operator=( const Manifest&& ) = delete;
};

} // namespace input
