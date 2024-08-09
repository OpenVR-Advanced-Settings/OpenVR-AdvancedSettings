#pragma once

#include <cmath>
#include <openvr.h>

namespace quaternion
{
inline vr::HmdQuaternion_t fromHmdMatrix34( vr::HmdMatrix34_t matrix )
{
    vr::HmdQuaternion_t quat;

    quat.w
        = sqrt( fmax( 0,
                      static_cast<double>( 1 + matrix.m[0][0] + matrix.m[1][1]
                                           + matrix.m[2][2] ) ) )
          / 2;
    quat.x
        = sqrt( fmax( 0,
                      static_cast<double>( 1 + matrix.m[0][0] - matrix.m[1][1]
                                           - matrix.m[2][2] ) ) )
          / 2;
    quat.y
        = sqrt( fmax( 0,
                      static_cast<double>( 1 - matrix.m[0][0] + matrix.m[1][1]
                                           - matrix.m[2][2] ) ) )
          / 2;
    quat.z
        = sqrt( fmax( 0,
                      static_cast<double>( 1 - matrix.m[0][0] - matrix.m[1][1]
                                           + matrix.m[2][2] ) ) )
          / 2;
    quat.x = copysign( quat.x,
                       static_cast<double>( matrix.m[2][1] - matrix.m[1][2] ) );
    quat.y = copysign( quat.y,
                       static_cast<double>( matrix.m[0][2] - matrix.m[2][0] ) );
    quat.z = copysign( quat.z,
                       static_cast<double>( matrix.m[1][0] - matrix.m[0][1] ) );
    return quat;
}

inline vr::HmdQuaternion_t multiply( const vr::HmdQuaternion_t& lhs,
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

inline vr::HmdQuaternion_t conjugate( const vr::HmdQuaternion_t& quat )
{
    return {
        quat.w,
        -quat.x,
        -quat.y,
        -quat.z,
    };
}

inline double getYaw( const vr::HmdQuaternion_t& quat )
{
    double const yawResult
        = atan2( 2.0 * ( quat.y * quat.w + quat.x * quat.z ),
                 ( 2.0 * ( quat.w * quat.w + quat.x * quat.x ) ) - 1.0 );
    return yawResult;
}

inline double getPitch( const vr::HmdQuaternion_t& quat )
{
    // positive forward
    // negative behind

    double const pitchResult
        = atan2( 2.0 * ( quat.x * quat.w + quat.y * quat.z ),
                 1.0 - 2.0 * ( quat.x * quat.x + quat.y * quat.y ) );
    //    double pitchResult
    //= atan2( 2.0 * ( quat.x * quat.w + quat.y * quat.z ),
    //      2.0 * ( quat.w * quat.w + quat.z * quat.z ) - 1.0 );
    return pitchResult;
}

inline double getRoll( const vr::HmdQuaternion_t& quat )
{
    double rollResult = NAN;
    double const sinp = 2 * ( quat.w * quat.z - quat.y * quat.x );
    if ( std::abs( sinp ) >= 1 )

    {
        rollResult = std::copysign( 3.14159265358979323846 / 2, sinp );
    }
    else
    {
        rollResult = std::asin( sinp );
    }
    return rollResult;
}

} // namespace quaternion
