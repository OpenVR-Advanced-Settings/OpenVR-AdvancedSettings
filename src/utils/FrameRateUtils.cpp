#include "FrameRateUtils.h"
#include <easylogging++.h>

namespace utils
{
/* Adjusts update rate for event loop ticks so that regardless of ticks we
 * maintain the approximate same time value for the updates, if Key is not set
 * in SteamVR we assume HMD is @ 90 hz
 * */
unsigned int adjustUpdateRate( const unsigned int baseRefreshKey )
{
    double updateRate;
    vr::EVRSettingsError vrSettingsError;
    updateRate = static_cast<double>(
        vr::VRSettings()->GetInt32( vr::k_pch_SteamVR_Section,
                                    vr::k_pch_SteamVR_PreferredRefreshRate,
                                    &vrSettingsError ) );

    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not read \""
                       << vr::k_pch_SteamVR_PreferredRefreshRate
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
        return baseRefreshKey;
    }

    // Assume Base 90 hz Refresh
    double multiplier = updateRate / 90.0;
    unsigned int adjustedRefreshKey = static_cast<unsigned int>(
        static_cast<double>( baseRefreshKey ) * multiplier );
    if ( adjustedRefreshKey < 45 )
    {
        return baseRefreshKey;
    }

    return adjustedRefreshKey;
}

} // end namespace utils
