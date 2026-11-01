#include "../../src/utils/TrackingDiscontinuity.h"
#include <QtTest>
#include <limits>

class TrackingDiscontinuityTest : public QObject
{
    Q_OBJECT

private slots:
    void trackingSequence();
    void configurableThresholds();
    void correctionPreservesPose();
    void recenterCompensation();
};

void TrackingDiscontinuityTest::trackingSequence()
{
    utils::TrackingDiscontinuity detector;
    vr::TrackedDevicePose_t pose{};
    pose.bDeviceIsConnected = true;
    pose.bPoseIsValid = true;
    pose.eTrackingResult = vr::TrackingResult_Running_OK;
    for ( unsigned axis = 0; axis < 3; ++axis )
        pose.mDeviceToAbsoluteTracking.m[axis][axis] = 1.0f;
    vr::HmdMatrix34_t previous{};
    for ( int frame = 0; frame < 100; ++frame )
        QVERIFY( !detector.update( pose, frame * 0.01, previous ) );
    pose.mDeviceToAbsoluteTracking.m[0][3] = 1.0f;
    QVERIFY( detector.update( pose, 1.0, previous ) );
    QVERIFY( detector.detection().position );
    QVERIFY( !detector.detection().yaw );
    QVERIFY( previous.m[0][3] == 0.0f );
    pose.mDeviceToAbsoluteTracking.m[0][3] = 2.0f;
    QVERIFY( !detector.update( pose, 1.01, previous ) );
    pose.bPoseIsValid = false;
    QVERIFY( !detector.update( pose, 1.02, previous ) );
    pose.bPoseIsValid = true;
    pose.mDeviceToAbsoluteTracking.m[0][3] = 20.0f;
    QVERIFY( !detector.update( pose, 1.03, previous ) );
    for ( int frame = 104; frame < 200; ++frame )
        QVERIFY( !detector.update( pose, frame * 0.01, previous ) );
    pose.mDeviceToAbsoluteTracking.m[0][0] = 0.0f;
    pose.mDeviceToAbsoluteTracking.m[0][2] = 1.0f;
    pose.mDeviceToAbsoluteTracking.m[2][0] = -1.0f;
    pose.mDeviceToAbsoluteTracking.m[2][2] = 0.0f;
    QVERIFY( detector.update( pose, 2.0, previous ) );
    QVERIFY( !detector.detection().position );
    QVERIFY( detector.detection().yaw );
    pose.mDeviceToAbsoluteTracking.m[0][3] = 100.0f;
    QVERIFY( !detector.update( pose, 3.0, previous ) );
    pose.mDeviceToAbsoluteTracking.m[0][3]
        = std::numeric_limits<float>::quiet_NaN();
    QVERIFY( !detector.update( pose, 3.01, previous ) );
}

void TrackingDiscontinuityTest::configurableThresholds()
{
    for ( const double distance : { 0.2, 2.0 } )
    {
        for ( const double speed : { 1.0, 100.0 } )
        {
            utils::TrackingDiscontinuity detector;
            vr::TrackedDevicePose_t pose{};
            pose.bDeviceIsConnected = pose.bPoseIsValid = true;
            pose.eTrackingResult = vr::TrackingResult_Running_OK;
            for ( unsigned axis = 0; axis < 3; ++axis )
                pose.mDeviceToAbsoluteTracking.m[axis][axis] = 1.0f;
            vr::HmdMatrix34_t previous{};
            const utils::TrackingJumpThresholds thresholds{
                distance, speed, 45.0, 720.0
            };
            for ( int frame = 0; frame < 100; ++frame )
                QVERIFY( !detector.update(
                    pose, frame * 0.01, previous, thresholds ) );
            pose.mDeviceToAbsoluteTracking.m[0][3] = 1.0f;
            QVERIFY( detector.update( pose, 1.01, previous, thresholds )
                     == ( distance < 1.0 && speed < 50.0 ) );
        }
    }
}

void TrackingDiscontinuityTest::correctionPreservesPose()
{
    for ( const double angle : { -1.0, 0.0, 1.0 } )
    {
        vr::HmdMatrix34_t basis{};
        basis.m[0][0] = basis.m[2][2] = 0.0f;
        basis.m[0][2] = 1.0f;
        basis.m[2][0] = -1.0f;
        basis.m[1][1] = 1.0f;
        basis.m[0][3] = 4.0f;
        vr::HmdMatrix34_t origin = basis;
        origin.m[0][3] = 5.0f;
        origin.m[1][3] = 2.0f;
        origin.m[2][3] = 3.0f;
        vr::HmdMatrix34_t previous{};
        for ( unsigned axis = 0; axis < 3; ++axis )
            previous.m[axis][axis] = 1.0f;
        previous.m[0][3] = 1.0f;
        previous.m[2][3] = 2.0f;
        auto current = previous;
        current.m[0][0] = current.m[2][2]
            = static_cast<float>( std::cos( angle ) );
        current.m[0][2] = static_cast<float>( std::sin( angle ) );
        current.m[2][0] = -current.m[0][2];
        current.m[0][3] = static_cast<float>(
            std::cos( angle ) + 2.0 * std::sin( angle ) + 10.0 );
        current.m[1][3] = 6.0f;
        current.m[2][3] = static_cast<float>( -std::sin( angle )
                                              + 2.0 * std::cos( angle ) - 7.0 );
        const auto result
            = utils::trackingJumpCorrection( previous, current, origin, basis );
        double corrected[3] = {};
        for ( unsigned row = 0; row < 3; ++row )
        {
            corrected[row] = basis.m[row][3];
            for ( unsigned axis = 0; axis < 3; ++axis )
                corrected[row] += basis.m[row][axis] * result.offsets[axis];
        }
        QVERIFY( std::abs( corrected[0]
                           - ( 5.0 * std::cos( angle ) + 3.0 * std::sin( angle )
                               + 10.0 ) )
                 < 0.00001 );
        QVERIFY( std::abs( corrected[1] - 8.0 ) < 0.00001 );
        QVERIFY( std::abs( corrected[2]
                           - ( -5.0 * std::sin( angle )
                               + 3.0 * std::cos( angle ) - 7.0 ) )
                 < 0.00001 );
        QVERIFY( std::abs( result.yaw - angle ) < 0.00001 );
    }
}

void TrackingDiscontinuityTest::recenterCompensation()
{
    vr::HmdMatrix34_t previous{};
    for ( unsigned axis = 0; axis < 3; ++axis )
        previous.m[axis][axis] = 1.0f;
    previous.m[0][3] = 1.0f;
    previous.m[2][3] = 2.0f;
    vr::HmdMatrix34_t origin{};
    origin.m[0][2] = 1.0f;
    origin.m[1][1] = 1.0f;
    origin.m[2][0] = -1.0f;
    origin.m[0][3] = 10.0f;
    vr::HmdMatrix34_t current{};
    current.m[0][2] = -1.0f;
    current.m[1][1] = 1.0f;
    current.m[2][0] = 1.0f;
    current.m[0][3] = -2.0f;
    current.m[2][3] = -9.0f;
    const auto correction
        = utils::recenterViewCorrection( previous, current, origin, origin );
    QVERIFY( std::abs( correction.offsets[0] ) < 0.00001 );
    QVERIFY( std::abs( correction.offsets[1] ) < 0.00001 );
    QVERIFY( std::abs( correction.offsets[2] + 10.0 ) < 0.00001 );
    QVERIFY( std::abs( correction.yaw + 1.5707963267948966 ) < 0.00001 );
    const auto unchanged
        = utils::recenterViewCorrection( previous, previous, origin, origin );
    for ( const auto offset : unchanged.offsets )
        QVERIFY( std::abs( offset ) < 0.00001 );
    QVERIFY( std::abs( unchanged.yaw ) < 0.00001 );
}

QTEST_APPLESS_MAIN( TrackingDiscontinuityTest )
#include "tst_trackingdiscontinuity.moc"
