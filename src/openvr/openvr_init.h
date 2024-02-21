#pragma once

namespace openvr_init
{
enum class OpenVrInitializationType
{
    Overlay,
    Utility,
};

void initializeOpenVR( const OpenVrInitializationType initType, int count = 0 );

} // namespace openvr_init
