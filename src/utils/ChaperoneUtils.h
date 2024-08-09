#pragma once

#include <mutex>
#include <openvr.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <array>

namespace utils
{
struct ChaperoneQuadData
{
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    float distance;
    vr::HmdVector3_t nearestPoint;
    std::array<vr::HmdVector3_t, 2> corners;
    // NOLINTEND(misc-non-private-member-variables-in-classes)

    [[nodiscard]] const vr::HmdVector3_t&
        closestCorner( const vr::HmdVector3_t& point ) const
    {
        auto cornerDistanceA = std::pow( point.v[0] - corners[0].v[0], 2.0 )
                               + std::pow( point.v[2] - corners[0].v[2], 2.0 );
        auto cornerDistanceB = std::pow( point.v[0] - corners[1].v[0], 2.0 )
                               + std::pow( point.v[2] - corners[1].v[2], 2.0 );
        return ( cornerDistanceA < cornerDistanceB ) ? corners[0] : corners[1];
    }
};

class ChaperoneUtils
{
private:
    std::recursive_mutex _mutex;
    uint32_t _quadsCount = 0;
    std::vector<vr::HmdVector3_t> _corners;
    bool _chaperoneWellFormed = true;
    std::vector<ChaperoneQuadData>
        _getDistancesToChaperone( const vr::HmdVector3_t& point );

public:
    [[nodiscard]] const vr::HmdVector3_t&
        getCorner( size_t index ) const noexcept
    {
        return _corners[index];
    }
    [[nodiscard]] uint32_t quadsCount() const noexcept
    {
        return _quadsCount;
    }
    [[nodiscard]] bool isChaperoneWellFormed() const noexcept
    {
        return _chaperoneWellFormed;
    }

    std::recursive_mutex& mutex() noexcept
    {
        return _mutex;
    }

    void loadChaperoneData( bool fromLiveBounds = true );

    std::vector<ChaperoneQuadData>
        getDistancesToChaperone( const vr::HmdVector3_t& point,
                                 bool doLock = false )
    {
        if ( doLock )
        {
            std::lock_guard<std::recursive_mutex> const lock( _mutex );
            return _getDistancesToChaperone( point );
        }

        return _getDistancesToChaperone( point );
    }

    ChaperoneQuadData getDistanceToChaperone( const vr::HmdVector3_t& point,
                                              bool doLock = false )
    {
        auto distances = getDistancesToChaperone( point, doLock );
        if ( distances.empty() )
        {
            ChaperoneQuadData ret;
            ret.distance = NAN;
            return ret;
        }

        return *std::min_element(
            distances.begin(),
            distances.end(),
            []( const ChaperoneQuadData& quadA, const ChaperoneQuadData& quadB )
            {
                return std::isnan( quadA.distance )
                       || ( quadA.distance < quadB.distance );
            } );
    }
};

} // end namespace utils
