import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: brightnessGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Misc:"
        bottomPadding: -10
    }
    background: Rectangle {
        color: "transparent"
        border.color: "#ffffff"
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            color: "#ffffff"
            height: 1
            Layout.fillWidth: true
            Layout.bottomMargin: 5
        }

        RowLayout {
            spacing: 16

            MyToggleButton {
                id: videoMotionSmoothingToggle
                text: "Motion Smoothing"
                onCheckedChanged: {
                       VideoTabController.setMotionSmoothing(this.checked, false)
                }
            }
            MyText {
                Layout.preferredWidth: 200
                text: " "
            }

            MyToggleButton {
                id: videoAllowSupersampleFilteringToggle
                text: "Antistropic Filtering"
                onCheckedChanged: {
                    VideoTabController.setAllowSupersampleFiltering(this.checked, false)
                }
            }
        }

    }

    Component.onCompleted: {
        videoAllowSupersampleFilteringToggle.checked = VideoTabController.allowSupersampleFiltering
        videoMotionSmoothingToggle.checked = VideoTabController.motionSmoothing

    }

    Connections {
        target: VideoTabController
        onAllowSupersampleFilteringChanged: {
            videoAllowSupersampleFilteringToggle.checked = VideoTabController.allowSupersampleFiltering
        }

        onMotionSmoothingChanged: {
            videoMotionSmoothingToggle.checked = VideoTabController.motionSmoothing
        }

    }
}
