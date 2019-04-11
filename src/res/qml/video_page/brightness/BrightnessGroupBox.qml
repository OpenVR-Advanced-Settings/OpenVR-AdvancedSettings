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
                text: "Toggle Brightness Adjustment"
                onCheckedChanged: {
                    VideoTabController.setBrightnessEnabled(this.checked, false)                }
            }

            //Item {
            //    Layout.fillWidth: true
            //}

            MyText {
                text: "Brightness:"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MySlider {
                id: brightnessSlider
                from: 0.00
                to: 1.0
                stepSize: 0.01
                value: 1.0
                onPositionChanged: {
                    var val = (this.position * 100)/2
                    brightnessValueText.text = Math.round(val) + "%"
                }
                onValueChanged: {
                    //TODO change value
                    VideoTabController.setBrightnessValue(value.toFixed(2), false)
                }
            }

            MyTextField {
                id: brightnessValueText
                text: "100.00"
                keyBoardUID: 301
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0.0) {
                            val = 0
                        } else if (val > 100.0) {
                            val = 100.0
                        }
                        var v = (val/100).toFixed(2)
                        if (v <= brightnessSlider.to) {
                            brightnessSlider.value = v
                        } else {
                            VideoTabController.setBrightnessValue(v, false)
                        }
                    }
                    text = Math.round(VideoTabController.boundsVisibility * 100) + "%"
                }
            }
        }
    }

    Component.onCompleted: {
        brightnessSlider.value = VideoTabController.brightnessValue
        brightnessToggle.checked = VideoTabController.brightnessEnabled
    }

    Connections {
        target: VideoTabController

        onBrightnessValueChanged: {
            brightnessSlider.value = VideoTabController.brightnessValue
        }
        onBrightnessEnabledChanged: {
            brightnessToggle.checked = VideoTabController.brightnessEnabled
        }
    }
}
