
#pragma once

#include <QObject>
#include <openvr.h>

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;

class StatisticsTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( float hmdDistanceMoved READ hmdDistanceMoved )
    Q_PROPERTY( float hmdRotations READ hmdRotations )
    Q_PROPERTY( float leftControllerMaxSpeed READ leftControllerMaxSpeed )
    Q_PROPERTY( float rightControllerMaxSpeed READ rightControllerMaxSpeed )
    Q_PROPERTY( int presentedFrames READ presentedFrames )
    Q_PROPERTY( int droppedFrames READ droppedFrames )
    Q_PROPERTY( int reprojectedFrames READ reprojectedFrames )
    Q_PROPERTY( int timedOut READ timedOut )
    Q_PROPERTY( float totalReprojectedRatio READ totalReprojectedRatio )

private:
    OverlayController* parent;

    bool rotationResetFlag = false;
    float rotationOffset = 0.0f;
    int rotationDir = 0;
    int64_t rotationCounter = 0;
    float lastYaw = -1.0f;
    unsigned lastPosTimer = 0;
    float lastHmdPos[3];
    bool lastHmdPosValid = false;

    float m_hmdRotation = 0.0f;
    double m_hmdDistanceMoved = 0.0;

    float m_leftControllerMaxSpeed = 0.0f;
    float m_rightControllerMaxSpeed = 0.0f;

    vr::Compositor_CumulativeStats m_cumStats;
    unsigned m_presentedFramesOffset = 0;
    unsigned m_droppedFramesOffset = 0;
    unsigned m_reprojectedFramesOffset = 0;
    unsigned m_timedOutOffset = 0;
    unsigned m_totalRatioPresentedOffset = 0;
    unsigned m_totalRatioReprojectedOffset = 0;

public:
    void initStage2( OverlayController* parent );

    void eventLoopTick( vr::TrackedDevicePose_t* devicePoses,
                        float leftSpeed,
                        float rightSpeed );

    float hmdDistanceMoved() const;
    float hmdRotations() const;
    float rightControllerMaxSpeed() const;
    float leftControllerMaxSpeed() const;

    unsigned presentedFrames() const;
    unsigned droppedFrames() const;
    unsigned reprojectedFrames() const;
    unsigned timedOut() const;
    float totalReprojectedRatio() const;

public slots:
    void statsDistanceResetClicked();
    void statsRotationResetClicked();
    void statsLeftControllerSpeedResetClicked();
    void statsRightControllerSpeedResetClicked();
    void presentedFramesResetClicked();
    void droppedFramesResetClicked();
    void reprojectedFramesResetClicked();
    void timedOutResetClicked();
    void totalRatioResetClicked();
};

} // namespace advsettings
