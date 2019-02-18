#pragma once

#include <openvr.h>
#include <cmath>

namespace utils
{
inline vr::HmdMatrix34_t& initRotationMatrix( vr::HmdMatrix34_t& matrix,
                                              unsigned axisId,
                                              float angle )
{
    switch ( axisId )
    {
    case 0: // x-axis
        matrix.m[0][0] = 1.0f;
        matrix.m[0][1] = 0.0f;
        matrix.m[0][2] = 0.0f;
        matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = 0.0f;
        matrix.m[1][1] = std::cos( angle );
        matrix.m[1][2] = -std::sin( angle );
        matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = 0.0f;
        matrix.m[2][1] = std::sin( angle );
        matrix.m[2][2] = std::cos( angle );
        matrix.m[2][3] = 0.0f;
        break;
    case 1: // y-axis
        matrix.m[0][0] = std::cos( angle );
        matrix.m[0][1] = 0.0f;
        matrix.m[0][2] = std::sin( angle );
        matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = 0.0f;
        matrix.m[1][1] = 1.0f;
        matrix.m[1][2] = 0.0f;
        matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = -std::sin( angle );
        matrix.m[2][1] = 0.0f;
        matrix.m[2][2] = std::cos( angle );
        matrix.m[2][3] = 0.0f;
        break;
    case 2: // z-axis
        matrix.m[0][0] = std::cos( angle );
        matrix.m[0][1] = -std::sin( angle );
        matrix.m[0][2] = 0.0f;
        matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = std::sin( angle );
        matrix.m[1][1] = std::cos( angle );
        matrix.m[1][2] = 0.0f;
        matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = 0.0f;
        matrix.m[2][1] = 0.0f;
        matrix.m[2][2] = 1.0f;
        matrix.m[2][3] = 0.0f;
        break;
    default:
        break;
    }
    return matrix;
}

inline vr::HmdMatrix34_t& matMul33( vr::HmdMatrix34_t& result,
                                    const vr::HmdMatrix34_t& a,
                                    const vr::HmdMatrix34_t& b )
{
    for ( unsigned i = 0; i < 3; i++ )
    {
        for ( unsigned j = 0; j < 3; j++ )
        {
            result.m[i][j] = 0.0f;
            for ( unsigned k = 0; k < 3; k++ )
            {
                result.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return result;
}

inline vr::HmdVector3_t& matMul33( vr::HmdVector3_t& result,
                                   const vr::HmdMatrix34_t& a,
                                   const vr::HmdVector3_t& b )
{
    for ( unsigned i = 0; i < 3; i++ )
    {
        result.v[i] = 0.0f;
        for ( unsigned k = 0; k < 3; k++ )
        {
            result.v[i] += a.m[i][k] * b.v[k];
        };
    }
    return result;
}

inline vr::HmdVector3_t& matMul33( vr::HmdVector3_t& result,
                                   const vr::HmdVector3_t& a,
                                   const vr::HmdMatrix34_t& b )
{
    for ( unsigned i = 0; i < 3; i++ )
    {
        result.v[i] = 0.0f;
        for ( unsigned k = 0; k < 3; k++ )
        {
            result.v[i] += a.v[k] * b.m[k][i];
        };
    }
    return result;
}

inline vr::HmdQuaternion_t getQuaternion( vr::HmdMatrix34_t matrix )
{
    vr::HmdQuaternion_t q;

    q.w = sqrt( fmax( 0,
                      static_cast<float>( 1 + matrix.m[0][0] + matrix.m[1][1]
                                          + matrix.m[2][2] ) ) )
          / 2;
    q.x = sqrt( fmax( 0,
                      static_cast<float>( 1 + matrix.m[0][0] - matrix.m[1][1]
                                          - matrix.m[2][2] ) ) )
          / 2;
    q.y = sqrt( fmax( 0,
                      static_cast<float>( 1 - matrix.m[0][0] + matrix.m[1][1]
                                          - matrix.m[2][2] ) ) )
          / 2;
    q.z = sqrt( fmax( 0,
                      static_cast<float>( 1 - matrix.m[0][0] - matrix.m[1][1]
                                          + matrix.m[2][2] ) ) )
          / 2;
    q.x = copysign( q.x,
                    static_cast<float>( matrix.m[2][1] - matrix.m[1][2] ) );
    q.y = copysign( q.y,
                    static_cast<float>( matrix.m[0][2] - matrix.m[2][0] ) );
    q.z = copysign( q.z,
                    static_cast<float>( matrix.m[1][0] - matrix.m[0][1] ) );
    return q;
}

inline vr::HmdQuaternion_t multiplyQuaternion( const vr::HmdQuaternion_t& lhs,
                                               const vr::HmdQuaternion_t& rhs )
{
    return { ( lhs.w * rhs.w ) - ( lhs.x * rhs.x ) - ( lhs.y * rhs.y )
                 - ( lhs.z * rhs.z ),
             ( lhs.w * rhs.x ) + ( lhs.x * rhs.w ) + ( lhs.y * rhs.z )
                 - ( lhs.z * rhs.y ),
             ( lhs.w * rhs.y ) + ( lhs.y * rhs.w ) + ( lhs.z * rhs.x )
                 - ( lhs.x * rhs.z ),
             ( lhs.w * rhs.z ) + ( lhs.z * rhs.w ) + ( lhs.x * rhs.y )
                 - ( lhs.y * rhs.x ) };
}

inline vr::HmdQuaternion_t
    quaternionConjugate( const vr::HmdQuaternion_t& quat )
{
    return {
        quat.w,
        -quat.x,
        -quat.y,
        -quat.z,
    };
}

} // end namespace utils
