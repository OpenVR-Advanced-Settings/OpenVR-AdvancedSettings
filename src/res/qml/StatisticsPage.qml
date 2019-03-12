import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "common"


MyStackViewPage {
    headerText: "Statistics"

    content: ColumnLayout {
        spacing: 18

        GridLayout {
            columns: 3

            MyText {
                text: "HMD Distance Moved:"
            }

            MyText {
                id: statsHmdMovedText
                text: "-00.0"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MyPushButton {
                text: "Reset"
                onClicked: {
                    StatisticsTabController.statsDistanceResetClicked()
                }
            }

            MyText {
                text: "HMD Rotations:"
            }

            MyText {
                id: statsHmdRotationText
                text: "0.0 CCW"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MyPushButton {
                text: "Reset"
                onClicked: {
                    StatisticsTabController.statsRotationResetClicked()
                }
            }

            MyText {
                text: "Left Controller Max Speed:"
            }

            MyText {
                id: statsLeftControllerSpeedText
                text: "99.9 m/s"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MyPushButton {
                text: "Reset"
                onClicked: {
                    StatisticsTabController.statsLeftControllerSpeedResetClicked()
                }
            }

            MyText {
                text: "Right Controller Max Speed:"
            }

            MyText {
                id: statsRightControllerSpeedText
                text: "99.9 m/s"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MyPushButton {
                text: "Reset"
                onClicked: {
                    StatisticsTabController.statsRightControllerSpeedResetClicked()
                }
            }
        }

        GridLayout {
            columns: 3
            Layout.topMargin: 32

            MyText {
                text: "Presented Frames:"
            }

            MyText {
                id: statsPresentedFramesText
                text: "000"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MyPushButton {
                text: "Reset"
                onClicked: {
                    StatisticsTabController.presentedFramesResetClicked()
                }
            }

            MyText {
                text: "Dropped Frames:"
            }

            MyText {
                id: statsDroppedFramesText
                text: "000"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MyPushButton {
                text: "Reset"
                onClicked: {
                    StatisticsTabController.droppedFramesResetClicked()
                }
            }

            MyText {
                text: "Reprojected Frames:"
            }

            MyText {
                id: statsReprojectionFramesText
                text: "000"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MyPushButton {
                text: "Reset"
                onClicked: {
                    StatisticsTabController.reprojectedFramesResetClicked()
                }
            }

            MyText {
                text: "Timed Out:"
            }

            MyText {
                id: statsTimedOutText
                text: "000"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MyPushButton {
                text: "Reset"
                onClicked: {
                    StatisticsTabController.timedOutResetClicked()
                }
            }

            MyText {
                text: "Reprojection Ratio:"
            }

            MyText {
                id: statstotalRatioText
                text: "0.0"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MyPushButton {
                text: "Reset"
                onClicked: {
                    StatisticsTabController.totalRatioResetClicked()
                }
            }
        }
        Item {
            Layout.fillHeight: true
        }

        function updateStatistics() {
            statsHmdMovedText.text = StatisticsTabController.hmdDistanceMoved.toFixed(1) + " m"
            var rotations = StatisticsTabController.hmdRotations
            if (rotations > 0) {
                statsHmdRotationText.text = rotations.toFixed(2) + " CCW"
            } else {
                statsHmdRotationText.text = -rotations.toFixed(2) + " CW"
            }
            statsLeftControllerSpeedText.text = "    " + StatisticsTabController.leftControllerMaxSpeed.toFixed(1) + " m/s"
            statsRightControllerSpeedText.text = "    " + StatisticsTabController.rightControllerMaxSpeed.toFixed(1) + " m/s"
            statsPresentedFramesText.text = StatisticsTabController.presentedFrames
            statsDroppedFramesText.text = StatisticsTabController.droppedFrames
            statsReprojectionFramesText.text = StatisticsTabController.reprojectedFrames
            statsTimedOutText.text = StatisticsTabController.timedOut
            statstotalRatioText.text = (StatisticsTabController.totalReprojectedRatio*100.0).toFixed(1) + "%"
        }

        Timer {
            id: statisticsUpdateTimer
            repeat: true
            interval: 100
            onTriggered: {
                parent.updateStatistics()
            }
        }

        onVisibleChanged: {
            if (visible) {
                updateStatistics()
                statisticsUpdateTimer.start()
            } else {
                statisticsUpdateTimer.stop()
            }
        }

    }

}
