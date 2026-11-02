#pragma once

#include <openvr.h>
#include <cmath>

namespace utils
{
inline bool validTrackingPose( const vr::TrackedDevicePose_t& pose )
{
    if ( !pose.bDeviceIsConnected || !pose.bPoseIsValid
         || pose.eTrackingResult != vr::TrackingResult_Running_OK )
    {
        return false;
    }
    for ( const auto& row : pose.mDeviceToAbsoluteTracking.m )
    {
        for ( const auto value : row )
        {
            if ( !std::isfinite( value ) )
            {
                return false;
            }
        }
    }
    return true;
}

struct TrackingJumpThresholds
{
    double distance = 0.5;
    double speed = 1.0;
    double angle = 5.0;
    double angularSpeed = 720.0;
};

struct TrackingJumpCorrection
{
    float offsets[3];
    double yaw;
};


inline double trackingYawDifference( const vr::HmdMatrix34_t& previous,
                                     const vr::HmdMatrix34_t& current )
{
    double sine = 0.0;
    double cosine = 0.0;
    for ( unsigned axis = 0; axis < 3; ++axis )
    {
        sine += static_cast<double>( current.m[0][axis] ) * previous.m[2][axis]
                - static_cast<double>( current.m[2][axis] ) * previous.m[0][axis];
        cosine += static_cast<double>( current.m[0][axis] ) * previous.m[0][axis]
                  + static_cast<double>( current.m[2][axis] ) * previous.m[2][axis];
    }

    if ( std::hypot( sine, cosine ) < 1e-6 )
    {
        return 0.0;
    }
    return std::atan2( sine, cosine );
}

inline TrackingJumpCorrection
    trackingJumpCorrection( const vr::HmdMatrix34_t& previous,
                            const vr::HmdMatrix34_t& current,
                            const vr::HmdMatrix34_t& origin,
                            const vr::HmdMatrix34_t& basis )
{
    const double delta = trackingYawDifference( previous, current );
    const double x = origin.m[0][3] - previous.m[0][3];
    const double z = origin.m[2][3] - previous.m[2][3];
    const double translated[3]
        = { std::cos( delta ) * x + std::sin( delta ) * z + current.m[0][3]
                - basis.m[0][3],
            origin.m[1][3] + current.m[1][3] - previous.m[1][3] - basis.m[1][3],
            -std::sin( delta ) * x + std::cos( delta ) * z + current.m[2][3]
                - basis.m[2][3] };
    TrackingJumpCorrection result{};
    result.yaw = delta;
    for ( unsigned axis = 0; axis < 3; ++axis )
    {
        for ( unsigned row = 0; row < 3; ++row )
        {
            result.offsets[axis]
                += static_cast<float>( basis.m[row][axis] * translated[row] );
        }
    }
    return result;
}

inline TrackingJumpCorrection
    recenterViewCorrection( const vr::HmdMatrix34_t& previousView,
                            const vr::HmdMatrix34_t& currentView,
                            const vr::HmdMatrix34_t& origin,
                            const vr::HmdMatrix34_t& basis )
{
    vr::HmdMatrix34_t previousRaw{};
    vr::HmdMatrix34_t currentRaw{};
    for ( unsigned row = 0; row < 3; ++row )
    {
        for ( unsigned column = 0; column < 4; ++column )
        {
            for ( unsigned axis = 0; axis < 3; ++axis )
            {
                previousRaw.m[row][column]
                    += origin.m[row][axis] * previousView.m[axis][column];
                currentRaw.m[row][column]
                    += origin.m[row][axis] * currentView.m[axis][column];
            }
        }
        previousRaw.m[row][3] += origin.m[row][3];
        currentRaw.m[row][3] += origin.m[row][3];
    }
    return trackingJumpCorrection( previousRaw, currentRaw, origin, basis );
}

struct TrackingJumpDetection
{
    bool position = false;
    bool yaw = false;
    double distance = 0.0;
    double speed = 0.0;
    double angle = 0.0;
    double angularSpeed = 0.0;
};

class TrackingDiscontinuity
{
public:
    TrackingJumpDetection detection() const
    {
        return m_detection;
    }

    void reset()
    {
        m_valid = false;
        m_detection = {};
    }

    bool update( const vr::TrackedDevicePose_t& pose,
                 double time,
                 vr::HmdMatrix34_t& previous,
                 const TrackingJumpThresholds& thresholds = {} )
    {
        m_detection = {};
        if ( !validTrackingPose( pose ) || !std::isfinite( time ) )
        {
            reset();
            return false;
        }
        const auto& matrix = pose.mDeviceToAbsoluteTracking;
        const double elapsed = time - m_time;
        bool detected = false;
        if ( m_valid && elapsed > 0.0 && elapsed <= k_maxSampleGap )
        {
            double distanceSquared = 0.0;
            for ( unsigned axis = 0; axis < 3; ++axis )
            {
                const double delta = matrix.m[axis][3] - m_previous.m[axis][3];
                distanceSquared += delta * delta;
            }
            const double difference = trackingYawDifference( m_previous, matrix );
            const bool upright
                = matrix.m[1][1] > 0.5f && m_previous.m[1][1] > 0.5f;
            m_detection.distance = std::sqrt( distanceSquared );
            m_detection.speed = m_detection.distance / elapsed;
            m_detection.angle = std::abs( difference ) / k_degreesToRadians;
            m_detection.angularSpeed = m_detection.angle / elapsed;
            m_detection.position = m_detection.distance > thresholds.distance
                                   && m_detection.speed > thresholds.speed;
            m_detection.yaw
                = upright && m_detection.angle > thresholds.angle
                  && m_detection.angularSpeed > thresholds.angularSpeed;
            detected = m_detection.position || m_detection.yaw;
        }
        previous = m_previous;
        m_previous = matrix;
        m_time = time;
        m_valid = true;
        return detected;
    }

private:
    static constexpr double k_maxSampleGap = 0.1;
    static constexpr double k_degreesToRadians = 0.017453292519943295;
    TrackingJumpDetection m_detection;
    bool m_valid = false;
    double m_time = 0.0;
    vr::HmdMatrix34_t m_previous{};
};
} // end namespace utils
