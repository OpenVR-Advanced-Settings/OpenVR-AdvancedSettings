#pragma once

#include <openvr.h>

/* Wrapper for OpenVR's undocumented IVRPaths_002 interface
* RE'd and subject to change without notice.
*/
namespace ovr_paths_wrapper
{
enum class PathsError
{
    NoError,
    Unavailable,
    ReadFailed,
    WriteFailed,
};

PathsError readStandingTransform( vr::HmdMatrix34_t& transform );
PathsError readSeatedTransform( vr::HmdMatrix34_t& transform );
PathsError writeStandingTransform( const vr::HmdMatrix34_t& transform );
PathsError writeSeatedTransform( const vr::HmdMatrix34_t& transform );
} // namespace ovr_paths_wrapper
