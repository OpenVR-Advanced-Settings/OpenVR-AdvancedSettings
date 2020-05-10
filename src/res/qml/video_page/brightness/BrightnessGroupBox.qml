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
        text: "Brightness"
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
            Layout.fillWidth: true

            MyToggleButton {
                id: brightnessToggle
                text: "Toggle On/Off"
                onCheckedChanged: {
                    VideoTabController.setBrightnessEnabled(this.checked, true)                }
            }

            Item {
                Layout.preferredWidth: 150
            }

            MyText {
                text: "Brightness:"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MySlider {
                id: brightnessSlider
                from: 0.10
                to: 1.0
                stepSize: 0.01
                value: 1.0
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = (this.value * 100)
                    brightnessValueText.text = Math.round(val) + "%"
                    VideoTabController.setBrightnessOpacityValue(value.toFixed(2), true)
                }
            }

            MyTextField {
                id: brightnessValueText
                text: "100%"
                keyBoardUID: 901
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 3.0) {
                            val = 3.0
                        } else if (val > 100.0) {
                            val = 100.0
                        }
                        var v = (val/100).toFixed(2)
                            brightnessSlider.value = v
                    }
                    text = Math.round(VideoTabController.brightnessOpacityValue * 100) + "%"
                }
            }
        }
    }

    Component.onCompleted: {
        var brightvalue = VideoTabController.brightnessOpacityValue
        brightnessSlider.value = brightvalue
        brightnessValueText.text = Math.round(brightnessOpacityValue*100)+ "%"
        brightnessToggle.checked = VideoTabController.brightnessEnabled
    }

    Connections {
        target: VideoTabController

        onBrightnessOpacityValueChanged: {
            var brightvalue = VideoTabController.brightnessOpacityValue
            brightnessSlider.value = brightvalue
            brightnessValueText.text = Math.round(brightnessOpacityValue*100)+ "%"
        }
        onBrightnessEnabledChanged: {
            brightnessToggle.checked = VideoTabController.brightnessEnabled
        }
    }
}
