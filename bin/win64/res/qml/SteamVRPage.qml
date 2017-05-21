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
                text: "Application Supersampling:"
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
                to: 5.0
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

            MyTextField {
                id: steamvrSupersamplingText
                text: "0.0"
                keyBoardUID: 401
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0.1) {
                            val = 0.1
                        }
                        var v = val.toFixed(1)
                        if (v <= steamvrSupersamplingSlider.to) {
                            steamvrSupersamplingSlider.value = v
                        } else {
                            SteamVRTabController.setSuperSampling(v, false)
                        }
                    }
                    text = SteamVRTabController.superSampling.toFixed(1)
                }
            }
        }

        RowLayout {
            spacing: 16

            MyText {
                text: "Compositor Render Target Multiplier*:"
                Layout.preferredWidth: 450
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
                to: 2
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


            MyTextField {
                id: steamvrCompositorSupersamplingText
                text: "0.0"
                keyBoardUID: 402
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0.1) {
                            val = 0.1
                        }
                        var v = val.toFixed(1)
                        if (v <= steamvrCompositorSupersamplingSlider.to) {
                            steamvrCompositorSupersamplingSlider.value = v
                        } else {
                            SteamVRTabController.setCompositorSuperSampling(v, false)
                        }
                    }
                    text = SteamVRTabController.compositorSuperSampling.toFixed(1)
                }
            }
        }

        MyToggleButton {
            id: steamvrAllowSupersampleFilteringToggle
            text: "Enable Advanced Supersample Filtering"
            onCheckedChanged: {
                SteamVRTabController.setAllowSupersampleFiltering(this.checked, false)
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
            text: "Enable Always-on Reprojection"
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
                    SteamVRTabController.reset()
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
            var s1 = SteamVRTabController.superSampling.toFixed(1)
            if (s1 <= steamvrSupersamplingSlider.to) {
                steamvrSupersamplingSlider.value = s1
            }
            steamvrSupersamplingText.text = s1
            var s2 = SteamVRTabController.compositorSuperSampling.toFixed(1)
            if (s2 <= steamvrCompositorSupersamplingSlider.to) {
                steamvrCompositorSupersamplingSlider.value = s2
            }
            steamvrCompositorSupersamplingText.text = s2
            steamvrAllowSupersampleFilteringToggle.checked = SteamVRTabController.allowSupersampleFiltering
            steamvrAllowInterleavedReprojectionToggle.checked = SteamVRTabController.allowInterleavedReprojection
            steamvrAllowAsyncReprojectionToggle.checked = SteamVRTabController.allowAsyncReprojection
            steamvrForceReprojectionToggle.checked = SteamVRTabController.forceReprojection
        }

        Connections {
            target: SteamVRTabController
            onSuperSamplingChanged: {
                var s1 = SteamVRTabController.superSampling.toFixed(1)
                if (s1 <= steamvrSupersamplingSlider.to && Math.abs(steamvrSupersamplingSlider.value-s1) > 0.08) {
                    steamvrSupersamplingSlider.value = s1
                }
                steamvrSupersamplingText.text = s1
            }
            onCompositorSuperSamplingChanged: {
                var s2 = SteamVRTabController.compositorSuperSampling.toFixed(1)
                if (s2 <= steamvrCompositorSupersamplingSlider.to && Math.abs(steamvrCompositorSupersamplingSlider.value-s2) > 0.08) {
                    steamvrCompositorSupersamplingSlider.value = s2
                }
                steamvrCompositorSupersamplingText.text = s2
            }
            onAllowSupersampleFilteringChanged: {
                steamvrAllowSupersampleFilteringToggle.checked = SteamVRTabController.allowSupersampleFiltering
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
