import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: brightnessGroupBox
    Layout.fillWidth: true

    title: "Misc"

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            spacing: 16

            MyToggleButton {
                id: videoMotionSmoothingToggle
                text: "Motion Smoothing"
                onCheckedChanged: {
                       VideoTabController.setMotionSmoothing(this.checked, false)
                }
            }
            Item {
                Layout.preferredWidth: 20
            }

            MyToggleButton {
                id: videoAllowSupersampleFilteringToggle
                text: "Advanced SS Filtering"
                onCheckedChanged: {
                    VideoTabController.setAllowSupersampleFiltering(this.checked, false)
                }
            }
            Item {
                Layout.preferredWidth: 20
            }

            MyToggleButton {
                id: videoUseOverlayToggle
                text: "Use Overlay For Color"
                onCheckedChanged: {
                    VideoTabController.setIsOverlayMethodActive(this.checked, true)
                }
            }
        }

    }

    Component.onCompleted: {
        videoAllowSupersampleFilteringToggle.checked = VideoTabController.allowSupersampleFiltering
        videoMotionSmoothingToggle.checked = VideoTabController.motionSmoothing
        videoUseOverlayToggle.checked = VideoTabController.isOverlayMethodActive
    }

    Connections {
        target: VideoTabController
        function onAllowSupersampleFilteringChanged() {
            videoAllowSupersampleFilteringToggle.checked = VideoTabController.allowSupersampleFiltering
        }

        function onMotionSmoothingChanged() {
            videoMotionSmoothingToggle.checked = VideoTabController.motionSmoothing
        }
        function onIsOverlayMethodActiveChanged(){
            videoUseOverlayToggle.checked = VideoTabController.isOverlayMethodActive
        }

    }
}
