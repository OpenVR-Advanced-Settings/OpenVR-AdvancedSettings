#pragma once

#include <openvr.h>
#include <cmath>

namespace quaternion
{
inline vr::HmdQuaternion_t fromHmdMatrix34( vr::HmdMatrix34_t matrix )
{
    vr::HmdQuaternion_t q;

    q.w = sqrt( fmax( 0,
                      static_cast<double>( 1 + matrix.m[0][0] + matrix.m[1][1]
                                           + matrix.m[2][2] ) ) )
          / 2;
    q.x = sqrt( fmax( 0,
                      static_cast<double>( 1 + matrix.m[0][0] - matrix.m[1][1]
                                           - matrix.m[2][2] ) ) )
          / 2;
    q.y = sqrt( fmax( 0,
                      static_cast<double>( 1 - matrix.m[0][0] + matrix.m[1][1]
                                           - matrix.m[2][2] ) ) )
          / 2;
    q.z = sqrt( fmax( 0,
                      static_cast<double>( 1 - matrix.m[0][0] - matrix.m[1][1]
                                           + matrix.m[2][2] ) ) )
          / 2;
    q.x = copysign( q.x,
                    static_cast<double>( matrix.m[2][1] - matrix.m[1][2] ) );
    q.y = copysign( q.y,
                    static_cast<double>( matrix.m[0][2] - matrix.m[2][0] ) );
    q.z = copysign( q.z,
                    static_cast<double>( matrix.m[1][0] - matrix.m[0][1] ) );
    return q;
}

// Axis must be a unit vector, angle is in radians
inline vr::HmdQuaternion_t fromAxisAngle( double axisX,
                                          double axisY,
                                          double axisZ,
                                          double angle )
{
    double s = std::sin( angle / 2.0 );
    return { std::cos( angle / 2.0 ), axisX * s, axisY * s, axisZ * s };
}

inline vr::HmdQuaternion_t multiply( const vr::HmdQuaternion_t& lhs,
                                     const vr::HmdQuaternion_t& rhs );
//TODO check
// Angles in radians, OpenVR axes: roll about Z, pitch about X, yaw about Y.
// Applied intrinsically as yaw, then pitch, then roll.
inline vr::HmdQuaternion_t
    fromEulerAngles( double roll, double pitch, double yaw )
{
    vr::HmdQuaternion_t qRoll = fromAxisAngle( 0.0, 0.0, 1.0, roll );
    vr::HmdQuaternion_t qPitch = fromAxisAngle( 1.0, 0.0, 0.0, pitch );
    vr::HmdQuaternion_t qYaw = fromAxisAngle( 0.0, 1.0, 0.0, yaw );
    return multiply( multiply( qYaw, qPitch ), qRoll );
}

inline vr::HmdMatrix33_t toHmdMatrix33( const vr::HmdQuaternion_t& quat )
{
    // Normalize so a slightly drifted quaternion still yields a pure rotation
    double norm = std::sqrt( quat.w * quat.w + quat.x * quat.x
                             + quat.y * quat.y + quat.z * quat.z );
    if ( norm == 0.0 )
    {
        return { { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } } };
    }
    double w = quat.w / norm;
    double x = quat.x / norm;
    double y = quat.y / norm;
    double z = quat.z / norm;

    vr::HmdMatrix33_t matrix;

    matrix.m[0][0] = static_cast<float>( 1.0 - 2.0 * ( y * y + z * z ) );
    matrix.m[0][1] = static_cast<float>( 2.0 * ( x * y - z * w ) );
    matrix.m[0][2] = static_cast<float>( 2.0 * ( x * z + y * w ) );

    matrix.m[1][0] = static_cast<float>( 2.0 * ( x * y + z * w ) );
    matrix.m[1][1] = static_cast<float>( 1.0 - 2.0 * ( x * x + z * z ) );
    matrix.m[1][2] = static_cast<float>( 2.0 * ( y * z - x * w ) );

    matrix.m[2][0] = static_cast<float>( 2.0 * ( x * z - y * w ) );
    matrix.m[2][1] = static_cast<float>( 2.0 * ( y * z + x * w ) );
    matrix.m[2][2] = static_cast<float>( 1.0 - 2.0 * ( x * x + y * y ) );

    return matrix;
}

// Rotation only, translation is zero
inline vr::HmdMatrix34_t toHmdMatrix34( const vr::HmdQuaternion_t& quat )
{
    vr::HmdMatrix33_t rotation = toHmdMatrix33( quat );
    vr::HmdMatrix34_t matrix;
    for ( unsigned i = 0; i < 3; i++ )
    {
        for ( unsigned j = 0; j < 3; j++ )
        {
            matrix.m[i][j] = rotation.m[i][j];
        }
        matrix.m[i][3] = 0.0f;
    }
    return matrix;
}

// Inverse of a rotation + translation matrix
inline vr::HmdMatrix34_t invertRigid( const vr::HmdMatrix34_t& matrix )
{
    const auto& m = matrix.m;
    vr::HmdMatrix34_t inverse;
    for ( unsigned i = 0; i < 3; i++ )
    {
        for ( unsigned j = 0; j < 3; j++ )
        {
            inverse.m[i][j] = m[j][i];
        }
    }
    for ( unsigned i = 0; i < 3; i++ )
    {
        inverse.m[i][3] = -( inverse.m[i][0] * m[0][3]
                             + inverse.m[i][1] * m[1][3]
                             + inverse.m[i][2] * m[2][3] );
    }
    return inverse;
}

// Keeps only the yaw and translation of a pose. Yaw comes from the local Z
// axis so pitch and roll can't leak into it.
inline vr::HmdMatrix34_t yawOnly( const vr::HmdMatrix34_t& pose )
{
    vr::HmdMatrix34_t result = toHmdMatrix34( fromAxisAngle(
        0.0,
        1.0,
        0.0,
        std::atan2( static_cast<double>( pose.m[0][2] ),
                    static_cast<double>( pose.m[2][2] ) ) ) );
    result.m[0][3] = pose.m[0][3];
    result.m[1][3] = pose.m[1][3];
    result.m[2][3] = pose.m[2][3];
    return result;
}

inline bool nearlyEqual( const vr::HmdMatrix34_t& a,
                         const vr::HmdMatrix34_t& b,
                         float tolerance )
{
    for ( unsigned i = 0; i < 3; i++ )
    {
        for ( unsigned j = 0; j < 4; j++ )
        {
            if ( std::abs( a.m[i][j] - b.m[i][j] ) > tolerance )
            {
                return false;
            }
        }
    }
    return true;
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
    double yawResult
        = atan2( 2.0 * ( quat.y * quat.w + quat.x * quat.z ),
                 ( 2.0 * ( quat.w * quat.w + quat.x * quat.x ) ) - 1.0 );
    return yawResult;
}

inline double getPitch( const vr::HmdQuaternion_t& quat )
{
    // positive forward
    // negative behind

    double pitchResult
        = atan2( 2.0 * ( quat.x * quat.w + quat.y * quat.z ),
                 1.0 - 2.0 * ( quat.x * quat.x + quat.y * quat.y ) );
    //    double pitchResult
    //= atan2( 2.0 * ( quat.x * quat.w + quat.y * quat.z ),
    //      2.0 * ( quat.w * quat.w + quat.z * quat.z ) - 1.0 );
    return pitchResult;
}

inline double getRoll( const vr::HmdQuaternion_t& quat )
{
    double rollResult;
    double sinp = 2 * ( quat.w * quat.z - quat.y * quat.x );
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
