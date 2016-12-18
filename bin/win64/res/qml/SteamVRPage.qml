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
                    console.debug("Supersampling Slider: valueChanged(", this.value, ")")
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
                        console.debug("Supersampling TextField: valueChanged(", val, ")")
                        var v = val.toFixed(1)
                        steamvrSupersamplingSlider.value = v
                        SteamVRTabController.setSuperSampling(v, false)
                    }
                    text = SteamVRTabController.superSampling.toFixed(1)
                }
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
                    console.debug("Compositor Supersampling Slider: valueChanged(", this.value, ")")
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
                        console.debug("Compositor Supersampling TextField: valueChanged(", val, ")")
                        var v = val.toFixed(1)
                        steamvrCompositorSupersamplingSlider.value = v
                        SteamVRTabController.setCompositorSuperSampling(v, false)
                    }
                    text = SteamVRTabController.compositorSuperSampling.toFixed(1)
                }
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
