#include "ChaperoneUtils.h"

#include <cmath>

namespace utils
{
// NOLINTBEGIN(readability-identifier-length)
std::vector<ChaperoneQuadData>
    ChaperoneUtils::_getDistancesToChaperone( const vr::HmdVector3_t& x )
{
    std::vector<ChaperoneQuadData> result;
    for ( uint32_t i = 0; i < _quadsCount; i++ )
    {
        uint32_t const i2 = ( i + 1 ) % _quadsCount;
        vr::HmdVector3_t const& r0 = _corners[i];
        vr::HmdVector3_t const& r1 = _corners[i2];
        float const u_x = r1.v[0] - r0.v[0];
        float const u_z = r1.v[2] - r0.v[2];
        float const r
            = ( ( x.v[0] - r0.v[0] ) * u_x + ( x.v[2] - r0.v[2] ) * u_z )
              / ( u_x * u_x + u_z * u_z );
        // int mode = 0; // 0 .. projected point on segment, 1 .. projected
        // point outside of segment (r0 closer than r1), 2 .. projected point
        // outside of segment (r1 closer than r0)
        float d = NAN;
        float x1_x = NAN;
        float x1_z = NAN;
        if ( r < 0.0f || r > 1.0f )
        { // projected point outside of segment
            float d_x = r0.v[0] - x.v[0];
            float d_z = r0.v[2] - x.v[2];
            // Crazy casts because clang sees the sqrt call as wanting a double.
            float const d1 = static_cast<float>(
                sqrt( static_cast<double>( d_x * d_x + d_z * d_z ) ) );
            d_x = r1.v[0] - x.v[0];
            d_z = r1.v[2] - x.v[2];
            // Crazy casts because clang sees the sqrt call as wanting a double.
            float const d2 = static_cast<float>(
                sqrt( static_cast<double>( d_x * d_x + d_z * d_z ) ) );
            if ( d1 < d2 )
            {
                d = d1;
                x1_x = r0.v[0];
                x1_z = r0.v[2];
            }
            else
            {
                d = d2;
                x1_x = r1.v[0];
                x1_z = r1.v[2];
            }
        }
        else
        { // projected point on segment
            x1_x = r0.v[0] + r * u_x;
            x1_z = r0.v[2] + r * u_z;
            float const d_x = x1_x - x.v[0];
            float const d_z = x1_z - x.v[2];
            // Crazy casts because clang sees the sqrt call as wanting a double.
            d = static_cast<float>(
                sqrt( static_cast<double>( d_x * d_x + d_z * d_z ) ) );
        }
        ChaperoneQuadData computedQuad;
        computedQuad.distance = d;
        computedQuad.nearestPoint = { x1_x, x.v[1], x1_z };
        computedQuad.corners[0] = r0;
        computedQuad.corners[1] = r1;
        result.push_back( computedQuad );
    }
    return result;
    // NOLINTEND(readability-identifier-length)
}

void ChaperoneUtils::loadChaperoneData( bool fromLiveBounds )
{
    std::lock_guard<std::recursive_mutex> const lock( _mutex );

    if ( fromLiveBounds )
    {
        vr::VRChaperoneSetup()->GetLiveCollisionBoundsInfo( nullptr,
                                                            &_quadsCount );
    }
    else
    {
        vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo( nullptr,
                                                               &_quadsCount );
    }

    if ( _quadsCount > 0 )
    {
        std::vector<vr::HmdQuad_t> quadsBuffer;
        quadsBuffer.reserve( _quadsCount );
        if ( fromLiveBounds )
        {
            vr::VRChaperoneSetup()->GetLiveCollisionBoundsInfo(
                quadsBuffer.data(), &_quadsCount );
        }
        else
        {
            vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(
                quadsBuffer.data(), &_quadsCount );
        }

        for ( uint32_t index = 0; index < _quadsCount; index++ )
        {
            _corners[index] = quadsBuffer[index].vCorners[0];
            uint32_t const index2 = ( index + 1 ) % _quadsCount;
            if ( quadsBuffer[index].vCorners[3].v[0]
                     != quadsBuffer[index2].vCorners[0].v[0]
                 || quadsBuffer[index].vCorners[3].v[1]
                        != quadsBuffer[index2].vCorners[0].v[1]
                 || quadsBuffer[index].vCorners[3].v[2]
                        != quadsBuffer[index2].vCorners[0].v[2]
                 || quadsBuffer[index].vCorners[0].v[1] != 0.0f )
            {
                _chaperoneWellFormed = false;
            }
        }
    }
}

} // end namespace utils
