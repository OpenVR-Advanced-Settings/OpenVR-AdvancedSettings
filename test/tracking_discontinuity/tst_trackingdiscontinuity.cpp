#include "../../src/utils/TrackingDiscontinuity.h"
#include "../../src/utils/Matrix.h"
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
    void transientSpikes();
    void sampleDiscontinuities();
    void recenterNearVertical();
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
    QVERIFY( detector.update( pose, 1.01, previous ) );
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

void TrackingDiscontinuityTest::transientSpikes()
{
    for ( const bool position : { false, true } )
    {
        utils::TrackingDiscontinuity detector;
        vr::TrackedDevicePose_t pose{};
        pose.bDeviceIsConnected = pose.bPoseIsValid = true;
        pose.eTrackingResult = vr::TrackingResult_Running_OK;
        vr::HmdMatrix34_t basis;
        utils::initRotationMatrix( basis, 1, 0.0f );
        auto origin = basis;
        auto normal = basis;
        normal.m[0][3] = 2.0f;
        normal.m[1][3] = 1.6f;
        vr::HmdMatrix34_t previous{};
        pose.mDeviceToAbsoluteTracking = normal;
        QVERIFY( !detector.update( pose, 0.0, previous ) );
        double rotation = 0.0;
        for ( int frame = 1; frame <= 4; ++frame )
        {
            auto sample = normal;
            if ( frame % 2 != 0 )
            {
                if ( position )
                    sample.m[0][3] += 1.0f;
                else
                {
                    utils::initRotationMatrix( sample, 1, 0.5f );
                    sample.m[0][3] = normal.m[0][3];
                    sample.m[1][3] = normal.m[1][3];
                }
            }
            pose.mDeviceToAbsoluteTracking = sample;
            QVERIFY( detector.update( pose, frame * 0.01, previous ) );
            QCOMPARE( detector.detection().position, position );
            QCOMPARE( detector.detection().yaw, !position );
            const auto correction = utils::trackingJumpCorrection(
                previous, sample, origin, basis );
            rotation += correction.yaw;
            utils::initRotationMatrix( origin, 1, static_cast<float>( rotation ) );
            for ( unsigned axis = 0; axis < 3; ++axis )
                origin.m[axis][3] = correction.offsets[axis];
            if ( frame % 2 == 0 )
            {
                QVERIFY( std::abs( rotation ) < 0.00001 );
                for ( unsigned axis = 0; axis < 3; ++axis )
                    QVERIFY( std::abs( origin.m[axis][3] ) < 0.00001 );
            }
        }
    }
}

void TrackingDiscontinuityTest::sampleDiscontinuities()
{
    utils::TrackingDiscontinuity detector;
    vr::TrackedDevicePose_t pose{};
    pose.bDeviceIsConnected = pose.bPoseIsValid = true;
    pose.eTrackingResult = vr::TrackingResult_Running_OK;
    utils::initRotationMatrix( pose.mDeviceToAbsoluteTracking, 1, 0.0f );
    vr::HmdMatrix34_t previous{};
    QVERIFY( !detector.update( pose, 0.0, previous ) );
    pose.mDeviceToAbsoluteTracking.m[0][3] = 1.0f;
    QVERIFY( !detector.update( pose, 1.0, previous ) ); // Long sample gap.
    pose.mDeviceToAbsoluteTracking.m[0][3] = 2.0f;
    QVERIFY( detector.update( pose, 1.01, previous ) ); // No settling delay.
    pose.bPoseIsValid = false;
    QVERIFY( !detector.update( pose, 1.02, previous ) );
    pose.bPoseIsValid = true;
    pose.mDeviceToAbsoluteTracking.m[0][3] = 10.0f;
    QVERIFY( !detector.update( pose, 1.03, previous ) ); // Reacquire baseline.
    pose.mDeviceToAbsoluteTracking.m[0][3] = 11.0f;
    QVERIFY( detector.update( pose, 1.04, previous ) );
    QVERIFY( !detector.update(
        pose, std::numeric_limits<double>::quiet_NaN(), previous ) );
    QVERIFY( !detector.update( pose, 1.05, previous ) );
    pose.mDeviceToAbsoluteTracking.m[0][3] = 12.0f;
    QVERIFY( !detector.update( pose, 1.05, previous ) ); // Duplicate timestamp.
    pose.mDeviceToAbsoluteTracking.m[0][3] = 13.0f;
    QVERIFY( !detector.update( pose, 1.04, previous ) ); // Clock moves backwards.
    pose.mDeviceToAbsoluteTracking.m[0][3] = 14.0f;
    QVERIFY( detector.update( pose, 1.06, previous ) );
}

void TrackingDiscontinuityTest::recenterNearVertical()
{
    constexpr double radians = 0.017453292519943295;
    for ( const double pitch : { -91.0, -90.0, 89.0, 90.0, 179.0 } )
    {
        for ( const double yaw : { -1.0, 0.0, 1.0 } )
        {
            vr::HmdMatrix34_t previous;
            vr::HmdMatrix34_t pitched;
            vr::HmdMatrix34_t turn;
            utils::initRotationMatrix(
                previous, 0, static_cast<float>( pitch * radians ) );
            utils::initRotationMatrix(
                pitched, 0, static_cast<float>( ( pitch + 2.0 ) * radians ) );
            utils::initRotationMatrix( turn, 1, static_cast<float>( yaw ) );
            vr::HmdMatrix34_t current{};
            utils::matMul33( current, turn, pitched );
            previous.m[0][3] = current.m[0][3] = 1.0f;
            previous.m[1][3] = current.m[1][3] = 1.6f;
            // Also check a rotated/translated standing basis.
            vr::HmdMatrix34_t origin;
            utils::initRotationMatrix( origin, 1, 0.7f );
            origin.m[0][3] = 3.0f;
            const auto correction = utils::recenterViewCorrection(
                previous, current, origin, origin );
            QVERIFY( std::abs( correction.yaw - yaw ) < 0.00001 );
            if ( yaw == 0.0 )
            {
                for ( const auto offset : correction.offsets )
                    QVERIFY( std::abs( offset ) < 0.00001 );
            }
        }
    }
    vr::HmdMatrix34_t previous;
    vr::HmdMatrix34_t current;
    utils::initRotationMatrix( previous, 0, 0.0f );
    utils::initRotationMatrix( current, 0, static_cast<float>( 180.0 * radians ) );
    QCOMPARE( utils::trackingYawDifference( previous, current ), 0.0 );
}

QTEST_APPLESS_MAIN( TrackingDiscontinuityTest )
#include "tst_trackingdiscontinuity.moc"
