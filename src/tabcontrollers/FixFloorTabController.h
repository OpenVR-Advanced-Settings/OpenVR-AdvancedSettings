
#pragma once

#include <QObject>
#include <openvr.h>

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;

class FixFloorTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(
        bool canUndo READ canUndo WRITE setCanUndo NOTIFY canUndoChanged )

private:
    OverlayController* parent;

    float controllerUpOffsetCorrection
        = 0.062f; // Controller touchpad facing upwards
    float controllerDownOffsetCorrection
        = 0.006f; // Controller touchpad facing downwards
    float knucklesUpOffsetCorrection = 0.0285f; // touchpad facing up PRELIM
    float knucklesDownOffsetCorrection = 0.031f; // touchpad facing down PRELIM

    int state
        = 0; // 0 .. idle, 1 .. floor fix in progress, 2 .. recenter in progress
    vr::TrackedDeviceIndex_t referenceController
        = vr::k_unTrackedDeviceIndexInvalid;
    unsigned measurementCount = 0;
    double tempOffsetX = 0.0;
    double tempOffsetY = 0.0;
    double tempOffsetZ = 0.0;
    double tempRoll = 0.0;
    float floorOffsetX = 0.0f;
    float floorOffsetY = 0.0f;
    float floorOffsetZ = 0.0f;
    QString statusMessage = "";
    float statusMessageTimeout = 0.0f;
    bool m_canUndo = false;

    enum ControllerType : int
    {
        Controller_Unknown = 0,
        Controller_Wand = 1,
        Controller_Knuckles = 2,
    };

    int getControllerType( vr::TrackedDeviceIndex_t controllerRole );

public:
    void initStage2( OverlayController* parent );

    void eventLoopTick( vr::TrackedDevicePose_t* devicePoses );
    void dashboardLoopTick( vr::TrackedDevicePose_t* devicePoses );

    Q_INVOKABLE QString currentStatusMessage();
    Q_INVOKABLE float currentStatusMessageTimeout();

    bool canUndo() const;

public slots:
    void fixFloorClicked();
    void recenterClicked();
    void undoFixFloorClicked();

    void setCanUndo( bool value, bool notify = true );

signals:
    void statusMessageSignal();
    void measureStartSignal();
    void measureEndSignal();
    void canUndoChanged( bool value );
};

} // namespace advsettings
