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
            Layout.fillWidth: true

            MyText {
                text: "Red:"
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: 75
            }

            MySlider {
                id: colorRedSlider
                from: 0.01
                to: 1.0
                stepSize: 0.01
                value: 1.0
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = (this.value * 100)
                    colorRedText.text = Math.round(val) + "%"
                    VideoTabController.setColorRed(value.toFixed(2), true)
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
                        if (val < 1) {
                            val = 1
                        } else if (val > 9999.0) {
                            val = 9999.0
                        }

                        var v = (val/100)
                        VideoTabController.setColorRed(v.toFixed(2), true)
                        if (v > 1)
                        {
                            colorRedSlider.value = 1
                        } else {
                            colorRedSlider.value = v
                        }
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
                to: 1.0
                stepSize: 0.01
                value: 1.0
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = (this.value * 100)
                    colorGreenText.text = Math.round(val) + "%"
                    VideoTabController.setColorGreen(value.toFixed(2), true)
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
                        if (val < 1) {
                            val = 1
                        } else if (val > 9999.0) {
                            val = 9999.0
                        }

                        var v = (val/100)
                        VideoTabController.setColorGreen(v.toFixed(2), true)
                        if (v > 1)
                        {
                            colorGreenSlider.value = 1
                        } else {
                            colorGreenSlider.value = v
                        }
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
                    to: 1.0
                    stepSize: 0.01
                    value: 1.0
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = (this.value * 100)
                        colorBlueText.text = Math.round(val) + "%"
                        VideoTabController.setColorBlue(value.toFixed(2), true)
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
                            if (val < 1) {
                                val = 1
                            } else if (val > 9999.0) {
                                val = 9999.0
                            }

                            var v = (val/100)
                            VideoTabController.setColorBlue(v.toFixed(2), true)
                            if (v > 1)
                            {
                                colorBlueSlider.value = 1
                            } else {
                                colorBlueSlider.value = v
                            }
                        }
                        text = Math.round(VideoTabController.colorBlue * 100) + "%"
                    }
                }

        }

    }

    Component.onCompleted: {
        // set up var here to prevent double call to c++ from qml
        var redValue = VideoTabController.colorRed
        var greenValue = VideoTabController.colorGreen
        var blueValue = VideoTabController.colorBlue
        colorRedText.text = Math.round(redValue * 100) + "%"
        colorGreenText.text = Math.round(greenValue * 100) + "%"
        colorBlueText.text = Math.round(blueValue * 100) + "%"
        colorRedSlider.value = redValue
        colorGreenSlider.value = greenValue
        colorBlueSlider.value = blueValue
    }

    Connections {
        target: VideoTabController
        onColorRedChanged:{
            var redValue = VideoTabController.colorRed
            colorRedText.text = Math.round(redValue * 100) + "%"
            colorRedSlider.value = redValue
        }
        onColorGreenChanged:{
            var greenValue = VideoTabController.colorGreen
            colorGreenText.text = Math.round(greenValue * 100) + "%"
            colorGreenSlider.value = greenValue
        }
        onColorBlueChanged:{
            var blueValue = VideoTabController.colorBlue
            colorBlueText.text = Math.round(blueValue * 100) + "%"
            colorBlueSlider.value = blueValue
        }
    }
}
