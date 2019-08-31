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
        text: "Color Adjustment"
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

            MyText {
                text: "Red:"
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: 75
            }

            MySlider {
                id: colorRedSlider
                from: 0.01
                to: 2.0
                stepSize: 0.01
                value: 1.0
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = (this.value * 100)
                    colorRedText.text = Math.round(val) + "%"
                }
                onValueChanged: {

                    VideoTabController.setColorRed(value.toFixed(2), false)
                }
            }

            MyTextField {
                id: colorRedText
                text: "100%"
                keyBoardUID: 912
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 5) {
                            val = 5
                        } else if (val > 100.0) {
                            val = 100.0
                        }
                        var v = (val/100).toFixed(2)
                            colorRedSlider.value = v
                    }
                    text = Math.round(VideoTabController.colorRed * 100) + "%"
                }
            }
            MyText {
                text: "Green:"
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: 85
            }

            MySlider {
                id: colorGreenSlider
                from: 0.01
                to: 2.0
                stepSize: 0.01
                value: 1.0
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = (this.value * 100)
                    colorGreenText.text = Math.round(val) + "%"
                }
                onValueChanged: {

                    VideoTabController.setColorGreen(value.toFixed(2), false)
                }
            }

            MyTextField {
                id: colorGreenText
                text: "100%"
                keyBoardUID: 913
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 5) {
                            val = 5
                        } else if (val > 100.0) {
                            val = 100.0
                        }
                        var v = (val/100).toFixed(2)
                            colorGreenSlider.value = v
                    }
                    text = Math.round(VideoTabController.colorGreen * 100) + "%"
                }
            }

                MyText {
                    text: "Blue:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 75
                }

                MySlider {
                    id: colorBlueSlider
                    from: 0.01
                    to: 2.0
                    stepSize: 0.01
                    value: 1.0
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = (this.value * 100)
                        colorBlueText.text = Math.round(val) + "%"
                    }
                    onValueChanged: {

                        VideoTabController.setColorBlue(value.toFixed(2), false)
                    }
                }

                MyTextField {
                    id: colorBlueText
                    text: "100%"
                    keyBoardUID: 914
                    Layout.preferredWidth: 100
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseFloat(input)
                        if (!isNaN(val)) {
                            if (val < 5) {
                                val = 5
                            } else if (val > 100.0) {
                                val = 100.0
                            }
                            var v = (val/100).toFixed(2)
                                colorBlueSlider.value = v
                        }
                        text = Math.round(VideoTabController.colorBlue * 100) + "%"
                    }
                }

        }

    }

    Component.onCompleted: {
        colorRedSlider.value = VideoTabController.colorRed
        colorGreenSlider.value = VideoTabController.colorGreen
        colorBlueSlider.value = VideoTabController.colorBlue
    }

    Connections {
        target: VideoTabController
        onColorRedChanged:{
            colorRedSlider.value = VideoTabController.colorRed
        }
        onColorGreenChanged:{
            colorGreenSlider.value = VideoTabController.colorGreen
        }
        onColorBlueChanged:{
            colorBlueSlider.value = VideoTabController.colorBlue
        }
    }
}
