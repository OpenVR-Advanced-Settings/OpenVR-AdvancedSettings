#pragma once

#include <openvr.h>
#include <cmath>

namespace utils
{
// TODO possibly expand on this to include other orientations?
// This is Forward, and Up orientation matrix
constexpr vr::HmdMatrix34_t k_forwardUpMatrix
    = { { { 1.0f, 0.0f, 0.0f, 0.0f },
          { 0.0f, 0.0f, 1.0f, 0.0f },
          { 0.0f, -1.0f, 0.0f, 0.0f } } };

typedef enum eMatrixAxis
{
    MatrixAxis_X,
    MatrixAxis_Y,
    MatrixAxis_Z,
} MatrixAxis;

inline vr::HmdMatrix34_t& initRotationMatrix( vr::HmdMatrix34_t& matrix,
                                              MatrixAxis axisId,
                                              float angle )
{
    switch ( axisId )
    {
    case MatrixAxis_X:
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
    case MatrixAxis_Y:
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
    case MatrixAxis_Z:
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
                                    const vr::HmdMatrix34_t& lhs,
                                    const vr::HmdMatrix34_t& rhs )
{
    for ( unsigned i = 0; i < 3; i++ )
    {
        for ( unsigned j = 0; j < 3; j++ )
        {
            result.m[i][j] = 0.0f;
            for ( unsigned k = 0; k < 3; k++ )
            {
                result.m[i][j] += lhs.m[i][k] * rhs.m[k][j];
            }
        }
    }
    return result;
}

inline vr::HmdVector3_t& matMul33( vr::HmdVector3_t& result,
                                   const vr::HmdMatrix34_t& lhs,
                                   const vr::HmdVector3_t& rhs )
{
    for ( unsigned i = 0; i < 3; i++ )
    {
        result.v[i] = 0.0f;
        for ( unsigned k = 0; k < 3; k++ )
        {
            result.v[i] += lhs.m[i][k] * rhs.v[k];
        };
    }
    return result;
}

inline vr::HmdVector3_t& matMul33( vr::HmdVector3_t& result,
                                   const vr::HmdVector3_t& lhs,
                                   const vr::HmdMatrix34_t& rhs )
{
    for ( unsigned i = 0; i < 3; i++ )
    {
        result.v[i] = 0.0f;
        for ( unsigned k = 0; k < 3; k++ )
        {
            result.v[i] += lhs.v[k] * rhs.m[k][i];
        };
    }
    return result;
}

} // end namespace utils
