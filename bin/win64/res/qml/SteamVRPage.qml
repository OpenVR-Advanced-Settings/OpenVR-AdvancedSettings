import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0

MyStackViewPage {
    id: myStackViewPage1
    headerText: "SteamVR Settings"

    content: ColumnLayout {
        spacing: 18

        RowLayout {
            spacing: 16

            MyText {
                text: "Application Supersampling*:"
                Layout.preferredWidth: 350
                Layout.rightMargin: 12
            }

            MyPushButton2 {
                text: "-"
                Layout.preferredWidth: 40
                onClicked: {
                    steamvrSupersamplingSlider.decrease()
                }
            }

            MySlider {
                id: steamvrSupersamplingSlider
                from: 0.1
                to: 4
                stepSize: 0.1
                value: 1.0
                snapMode: Slider.SnapAlways
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = this.from + ( this.position  * (this.to - this.from))
                    steamvrSupersamplingText.text = val.toFixed(1)
                }
                onValueChanged: {
                    SteamVRTabController.setSuperSampling(this.value.toFixed(1), false)
                }
            }

            MyPushButton2 {
                text: "+"
                Layout.preferredWidth: 40
                onClicked: {
                    steamvrSupersamplingSlider.increase()
                }
            }

            MyText {
                id: steamvrSupersamplingText
                text: "9.9"
                horizontalAlignment: Text.AlignRight
                Layout.minimumWidth: 64
            }
        }

        RowLayout {
            spacing: 16

            MyText {
                text: "Compositor Supersampling*:"
                Layout.preferredWidth: 350
                Layout.rightMargin: 12
            }

            MyPushButton2 {
                text: "-"
                Layout.preferredWidth: 40
                onClicked: {
                    steamvrCompositorSupersamplingSlider.decrease()
                }
            }

            MySlider {
                id: steamvrCompositorSupersamplingSlider
                from: 0.1
                to: 4
                stepSize: 0.1
                value: 1.0
                snapMode: Slider.SnapAlways
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = this.from + ( this.position  * (this.to - this.from))
                    steamvrCompositorSupersamplingText.text = val.toFixed(1)
                }
                onValueChanged: {
                    SteamVRTabController.setCompositorSuperSampling(this.value.toFixed(1), false)
                }
            }

            MyPushButton2 {
                text: "+"
                Layout.preferredWidth: 40
                onClicked: {
                    steamvrCompositorSupersamplingSlider.increase()
                }
            }

            MyText {
                id: steamvrCompositorSupersamplingText
                text: "9.9"
                horizontalAlignment: Text.AlignRight
                Layout.minimumWidth: 64
            }
        }

        MyToggleButton {
            id: steamvrAllowAsyncReprojectionToggle
            text: "Allow Asynchronous Reprojection"
            onCheckedChanged: {
                SteamVRTabController.setAllowAsyncReprojection(this.checked, false)
            }
        }

        MyToggleButton {
            id: steamvrAllowInterleavedReprojectionToggle
            text: "Allow Interleaved Reprojection"
            onCheckedChanged: {
                SteamVRTabController.setAllowInterleavedReprojection(this.checked, false)
            }
        }

        MyToggleButton {
            id: steamvrForceReprojectionToggle
            text: "Enable Always-on Reprojection*"
            onCheckedChanged: {
                SteamVRTabController.setForceReprojection(this.checked, false)
            }
        }

        Item { Layout.fillHeight: true; Layout.fillWidth: true}

        MyText {
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignRight
            text: "* Requires Restart"
            font.pointSize: 18
        }

        RowLayout {
            Layout.fillWidth: true

            MyPushButton {
                id: steamVRResetButton
                text: "Reset"
                Layout.preferredWidth: 250
                onClicked: {
                    steamvrSupersamplingSlider.value = 1.0
                    steamvrCompositorSupersamplingSlider.value = 1.0
                    steamvrAllowInterleavedReprojectionToggle.checked = true
                    steamvrAllowAsyncReprojectionToggle.checked = true
                    steamvrForceReprojectionToggle.checked = false
                }
            }

            Item { Layout.fillWidth: true}

            MyPushButton {
                id: steamVRRestartButton
                text: "Restart SteamVR"
                Layout.preferredWidth: 250
                onClicked: {
                    SteamVRTabController.restartSteamVR()
                }
            }
        }

        Component.onCompleted: {
            steamvrSupersamplingSlider.value = SteamVRTabController.superSampling
            steamvrCompositorSupersamplingSlider.value = SteamVRTabController.compositorSuperSampling
            steamvrAllowInterleavedReprojectionToggle.checked = SteamVRTabController.allowInterleavedReprojection
            steamvrAllowAsyncReprojectionToggle.checked = SteamVRTabController.allowAsyncReprojection
            steamvrForceReprojectionToggle.checked = SteamVRTabController.forceReprojection
        }

        Connections {
            target: SteamVRTabController
            onSuperSamplingChanged: {
                if (Math.abs(steamvrSupersamplingSlider.value-SteamVRTabController.superSampling) > 0.08) {
                    steamvrSupersamplingSlider.value = SteamVRTabController.superSampling
                }
            }
            onCompositorSuperSamplingChanged: {
                if (Math.abs(steamvrCompositorSupersamplingSlider.value-SteamVRTabController.compositorSuperSampling) > 0.08) {
                    steamvrCompositorSupersamplingSlider.value = SteamVRTabController.compositorSuperSampling
                }
            }
            onAllowInterleavedReprojectionChanged: {
                steamvrAllowInterleavedReprojectionToggle.checked = SteamVRTabController.allowInterleavedReprojection
            }
            onAllowAsyncReprojectionChanged: {
                steamvrAllowAsyncReprojectionToggle.checked = SteamVRTabController.allowAsyncReprojection
            }
            onForceReprojectionChanged: {
                steamvrForceReprojectionToggle.checked = SteamVRTabController.forceReprojection
            }
        }
    }
}
