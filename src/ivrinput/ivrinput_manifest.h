#pragma once
#include <QStandardPaths>
#include <openvr.h>
#include <easylogging++.h>

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
        QString actionManifestPath = QStandardPaths::locate(
            QStandardPaths::AppDataLocation,
            QStringLiteral( "action_manifest.json" ) );

        auto error = vr::VRInput()->SetActionManifestPath(
            actionManifestPath.toStdString().c_str() );
        if ( error != vr::EVRInputError::VRInputError_None )
        {
            LOG( ERROR ) << "Error setting action manifest path: "
                         << actionManifestPath << ". OpenVR Error: " << error;
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
