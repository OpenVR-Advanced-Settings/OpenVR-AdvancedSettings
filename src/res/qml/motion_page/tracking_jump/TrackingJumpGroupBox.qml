import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: trackingJumpGroupBox
    Layout.fillWidth: true
    label: MyText {
        leftPadding: 10
        text: "Tracking Jump Correction (Experimental)"
        bottomPadding: -10
    }
    background: Rectangle {
        color: "transparent"
        border.color: "#d9dbe0"
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        Rectangle {
            color: "#d9dbe0"
            height: 1
            Layout.fillWidth: true
            Layout.bottomMargin: 5
        }
        RowLayout {
            Layout.fillWidth: true
            spacing: 24
            MyToggleButton {
                id: trackingJumpCorrectionToggle
                text: "Enabled"
                onCheckedChanged: MoveCenterTabController.setTrackingJumpCorrection(checked, false)
            }
            MyToggleButton {
                id: preserveRecenterViewToggle
                text: "Preserve View on Recenter"
                onCheckedChanged: MoveCenterTabController.setPreserveRecenterView(checked, false)
            }

            Item { Layout.fillWidth: true }
        }
        GridLayout {
            Layout.fillWidth: true
            columns: 9
            columnSpacing: 12
            rowSpacing: 10
            MyText {
                text: "Position"
                Layout.minimumWidth: implicitWidth
                Layout.rightMargin: 8
            }
            MyText {
                text: "Change:"
                Layout.minimumWidth: implicitWidth
            }
            MySlider {
                id: trackingJumpDistanceSlider
                Layout.fillWidth: true
                Layout.minimumWidth: 60
                Layout.preferredWidth: 140
                from: 0.01
                to: 10
                stepSize: 0.01
                onMoved: MoveCenterTabController.trackingJumpDistance = value
            }
            MyTextField {
                id: trackingJumpDistanceText
                keyBoardUID: 510
                Layout.minimumWidth: 100
                Layout.preferredWidth: 100
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (isFinite(val)) {
                        MoveCenterTabController.trackingJumpDistance = val
                    }
                    text = MoveCenterTabController.trackingJumpDistance.toFixed(2)
                }
            }
            MyText {
                text: "m"
                Layout.minimumWidth: implicitWidth
                Layout.rightMargin: 12
            }
            MyText {
                text: "Speed:"
                Layout.minimumWidth: implicitWidth
            }
            MySlider {
                id: trackingJumpSpeedSlider
                Layout.fillWidth: true
                Layout.minimumWidth: 60
                Layout.preferredWidth: 140
                from: 0.1
                to: 100
                stepSize: 0.1
                onMoved: MoveCenterTabController.trackingJumpSpeed = value
            }
            MyTextField {
                id: trackingJumpSpeedText
                keyBoardUID: 511
                Layout.minimumWidth: 100
                Layout.preferredWidth: 100
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (isFinite(val)) {
                        MoveCenterTabController.trackingJumpSpeed = val
                    }
                    text = MoveCenterTabController.trackingJumpSpeed.toFixed(2)
                }
            }
            MyText {
                text: "m/s"
                Layout.minimumWidth: implicitWidth
            }

            MyText {
                text: "Yaw"
                Layout.minimumWidth: implicitWidth
                Layout.rightMargin: 8
            }
            MyText {
                text: "Change:"
                Layout.minimumWidth: implicitWidth
            }
            MySlider {
                id: trackingJumpAngleSlider
                Layout.fillWidth: true
                Layout.minimumWidth: 60
                Layout.preferredWidth: 140
                from: 1
                to: 180
                stepSize: 1
                onMoved: MoveCenterTabController.trackingJumpAngle = value
            }
            MyTextField {
                id: trackingJumpAngleText
                keyBoardUID: 512
                Layout.minimumWidth: 100
                Layout.preferredWidth: 100
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (isFinite(val)) {
                        MoveCenterTabController.trackingJumpAngle = val
                    }
                    text = MoveCenterTabController.trackingJumpAngle.toFixed(2)
                }
            }
            MyText {
                text: "deg"
                Layout.minimumWidth: implicitWidth
                Layout.rightMargin: 12
            }
            MyText {
                text: "Speed:"
                Layout.minimumWidth: implicitWidth
            }
            MySlider {
                id: trackingJumpAngularSpeedSlider
                Layout.fillWidth: true
                Layout.minimumWidth: 60
                Layout.preferredWidth: 140
                from: 1
                to: 10000
                stepSize: 1
                onMoved: MoveCenterTabController.trackingJumpAngularSpeed = value
            }
            MyTextField {
                id: trackingJumpAngularSpeedText
                keyBoardUID: 513
                Layout.minimumWidth: 100
                Layout.preferredWidth: 100
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (isFinite(val)) {
                        MoveCenterTabController.trackingJumpAngularSpeed = val
                    }
                    text = MoveCenterTabController.trackingJumpAngularSpeed.toFixed(2)
                }
            }
            MyText {
                text: "deg/s"
                Layout.minimumWidth: implicitWidth
            }
        }

    }

    Component.onCompleted: {
        preserveRecenterViewToggle.checked = MoveCenterTabController.preserveRecenterView
        trackingJumpCorrectionToggle.checked = MoveCenterTabController.trackingJumpCorrection
        trackingJumpDistanceText.text = MoveCenterTabController.trackingJumpDistance.toFixed(2)
        trackingJumpDistanceSlider.value = MoveCenterTabController.trackingJumpDistance
        trackingJumpSpeedText.text = MoveCenterTabController.trackingJumpSpeed.toFixed(2)
        trackingJumpSpeedSlider.value = MoveCenterTabController.trackingJumpSpeed
        trackingJumpAngleText.text = MoveCenterTabController.trackingJumpAngle.toFixed(2)
        trackingJumpAngleSlider.value = MoveCenterTabController.trackingJumpAngle
        trackingJumpAngularSpeedText.text = MoveCenterTabController.trackingJumpAngularSpeed.toFixed(2)
        trackingJumpAngularSpeedSlider.value = MoveCenterTabController.trackingJumpAngularSpeed
    }

    Connections {
        target: MoveCenterTabController
        onPreserveRecenterViewChanged: {
            preserveRecenterViewToggle.checked = MoveCenterTabController.preserveRecenterView
        }
        onTrackingJumpCorrectionChanged: {
            trackingJumpCorrectionToggle.checked = MoveCenterTabController.trackingJumpCorrection
        }
        onTrackingJumpDistanceChanged: {
            trackingJumpDistanceText.text = MoveCenterTabController.trackingJumpDistance.toFixed(2)
            trackingJumpDistanceSlider.value = MoveCenterTabController.trackingJumpDistance
        }
        onTrackingJumpSpeedChanged: {
            trackingJumpSpeedText.text = MoveCenterTabController.trackingJumpSpeed.toFixed(2)
            trackingJumpSpeedSlider.value = MoveCenterTabController.trackingJumpSpeed
        }
        onTrackingJumpAngleChanged: {
            trackingJumpAngleText.text = MoveCenterTabController.trackingJumpAngle.toFixed(2)
            trackingJumpAngleSlider.value = MoveCenterTabController.trackingJumpAngle
        }
        onTrackingJumpAngularSpeedChanged: {
            trackingJumpAngularSpeedText.text = MoveCenterTabController.trackingJumpAngularSpeed.toFixed(2)
            trackingJumpAngularSpeedSlider.value = MoveCenterTabController.trackingJumpAngularSpeed
        }
    }
}
