#include "ChaperoneUtils.h"
#include <iostream>
#include <cmath>

namespace utils
{
std::vector<ChaperoneQuadData>
    ChaperoneUtils::_getDistancesToChaperone( const vr::HmdVector3_t& x )
{
    std::vector<ChaperoneQuadData> result;
    vr::HmdVector3_t* _cornersPtr = _corners.get();
    for ( uint32_t i = 0; i < _quadsCount; i++ )
    {
        uint32_t i2 = ( i + 1 ) % _quadsCount;
        vr::HmdVector3_t& r0 = _cornersPtr[i];
        vr::HmdVector3_t& r1 = _cornersPtr[i2];
        float u_x = r1.v[0] - r0.v[0];
        float u_z = r1.v[2] - r0.v[2];
        float r = ( ( x.v[0] - r0.v[0] ) * u_x + ( x.v[2] - r0.v[2] ) * u_z )
                  / ( u_x * u_x + u_z * u_z );
        // int mode = 0; // 0 .. projected point on segment, 1 .. projected
        // point outside of segment (r0 closer than r1), 2 .. projected point
        // outside of segment (r1 closer than r0)
        float d;
        float x1_x;
        float x1_z;
        if ( r < 0.0f || r > 1.0f )
        { // projected point outside of segment
            float d_x = r0.v[0] - x.v[0];
            float d_z = r0.v[2] - x.v[2];
            // Crazy casts because clang sees the sqrt call as wanting a double.
            float d1 = static_cast<float>(
                sqrt( static_cast<double>( d_x * d_x + d_z * d_z ) ) );
            d_x = r1.v[0] - x.v[0];
            d_z = r1.v[2] - x.v[2];
            // Crazy casts because clang sees the sqrt call as wanting a double.
            float d2 = static_cast<float>(
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
            float d_x = x1_x - x.v[0];
            float d_z = x1_z - x.v[2];
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
}

void ChaperoneUtils::loadChaperoneData( bool fromLiveBounds )
{
    std::lock_guard<std::recursive_mutex> lock( _mutex );

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
        std::unique_ptr<vr::HmdQuad_t> quadsBuffer(
            new vr::HmdQuad_t[_quadsCount] );
        vr::HmdQuad_t* quadsBufferPtr = quadsBuffer.get();
        _corners.reset( reinterpret_cast<vr::HmdVector3_t*>(
            new vr::HmdQuad_t[_quadsCount] ) );
        vr::HmdVector3_t* _cornersPtr = _corners.get();
        if ( fromLiveBounds )
        {
            vr::VRChaperoneSetup()->GetLiveCollisionBoundsInfo( quadsBufferPtr,
                                                                &_quadsCount );
        }
        else
        {
            vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(
                quadsBufferPtr, &_quadsCount );
        }

        for ( uint32_t i = 0; i < _quadsCount; i++ )
        {
            _cornersPtr[i] = quadsBufferPtr[i].vCorners[0];
            uint32_t i2 = ( i + 1 ) % _quadsCount;
            if ( quadsBufferPtr[i].vCorners[3].v[0]
                     != quadsBufferPtr[i2].vCorners[0].v[0]
                 || quadsBufferPtr[i].vCorners[3].v[1]
                        != quadsBufferPtr[i2].vCorners[0].v[1]
                 || quadsBufferPtr[i].vCorners[3].v[2]
                        != quadsBufferPtr[i2].vCorners[0].v[2]
                 || quadsBufferPtr[i].vCorners[0].v[1] != 0.0f )
            {
                _chaperoneWellFormed = false;
            }
        }
    }
}

} // end namespace utils
