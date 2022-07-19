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
        text: "SuperSampling: (may require refresh of dashboard or restart of SteamVR)"
        bottomPadding: -10
    }
    background: Rectangle {
        color: "transparent"
        border.color: "#d9dbe0"
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            color: "#d9dbe0"
            height: 1
            Layout.fillWidth: true
            Layout.bottomMargin: 5
        }

        RowLayout {
            spacing: 16

            MyToggleButton {
                id: videoAllowSupersampleOverrideToggle
                text: "Toggle Override"
                onCheckedChanged: {
                    VideoTabController.setAllowSupersampleOverride(this.checked, false)
                    VideoTabController.setSuperSampling(videoSupersamplingSlider.value, true)
                    videoSupersamplingText.text = VideoTabController.superSampling.toFixed(2)
                    if(!this.checked){
                        videoSupersamplingText.enabled = false
                        videoSupersamplingSlider.enabled = false
                        videoSupersampleIncrement.enabled = false
                        videoSupersampleDecrement.enabled = false
                    }else{
                        videoSupersamplingText.enabled = true
                        videoSupersamplingSlider.enabled = true
                        videoSupersampleIncrement.enabled = true
                        videoSupersampleDecrement.enabled = true
                    }
                }
            }


            MyPushButton2 {
                id: videoSupersampleDecrement
                text: "-"
                Layout.preferredWidth: 40
                onClicked: {
                    videoSupersamplingSlider.decrease()
                }
            }

            MySlider {
                id: videoSupersamplingSlider
                from: 0.2
                to: 5.0
                stepSize: 0.1
                value: 1.0
                snapMode: Slider.SnapAlways
                Layout.fillWidth: true
                onValueChanged: {
                    var val = this.value
                    videoSupersamplingText.text = val.toFixed(2)
                    VideoTabController.setSuperSampling(this.value, true)
                }
            }

            MyPushButton2 {
                id: videoSupersampleIncrement
                text: "+"
                Layout.preferredWidth: 40
                onClicked: {
                    videoSupersamplingSlider.increase()
                }
            }

            MyTextField {
                id: videoSupersamplingText
                text: "0.0"
                keyBoardUID: 921
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0.1) {
                            val = 0.1
                        }
                        var v = val
                        if (v <= videoSupersamplingSlider.to) {
                            videoSupersamplingSlider.value = v
                        } else {
                            VideoTabController.setSuperSampling(v, false)
                        }
                    }
                    text = VideoTabController.superSampling.toFixed(2)
                }
            }
        }



    }

    Component.onCompleted: {
        var s1 = VideoTabController.superSampling
        if (s1 <= videoSupersamplingSlider.to) {
            videoSupersamplingSlider.value = s1
        }
        videoAllowSupersampleOverrideToggle.checked = VideoTabController.allowSupersampleOverride
        if(!videoAllowSupersampleOverrideToggle.checked){
            videoSupersamplingText.enabled = false
            videoSupersamplingSlider.enabled = false
            videoSupersampleIncrement.enabled = false
            videoSupersampleDecrement.enabled = false
        }
    }

    Connections {
        target: VideoTabController

        onSuperSamplingChanged: {
            var s1 = VideoTabController.superSampling
            videoSupersamplingSlider.value = s1
            videoSupersamplingText.text = s1.toFixed(2)
        }
        onAllowSupersampleOverrideChanged: {
            videoAllowSupersampleOverrideToggle.checked = VideoTabController.allowSupersampleOverride
            var s1 = VideoTabController.superSampling
            videoSupersamplingSlider.value = s1
            videoSupersamplingText.text = s1.toFixed(2)
        }

    }
}
