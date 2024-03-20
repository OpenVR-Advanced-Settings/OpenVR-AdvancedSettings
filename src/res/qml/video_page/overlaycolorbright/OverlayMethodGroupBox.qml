import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: overlayMethodGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Brightness and Color (via overlay)"
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

            MyToggleButton {
                id: brightnessToggle
                text: "Toggle On/Off"
                onCheckedChanged: {
                    VideoTabController.setBrightnessEnabled(this.checked, true)
                }
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
                from: 0.2
                to: 1.0
                stepSize: 0.01
                value: 1.0
                Layout.fillWidth: true
                onMoved: {
                    VideoTabController.setBrightnessOpacityValue(this.value.toFixed(2), true)
                }
            }

            MyTextField {
                id: brightnessValueText
                text: "100%"
                keyBoardUID: 902
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val2 = parseFloat(input)
                    if (!isNaN(val2)) {
                        if (val2 < 3) {
                            val2 = 3
                        } else if (val2 > 100.0) {
                            val2 = 100.0
                        }
                        var v2 = (val2/100)
                        VideoTabController.setBrightnessOpacityValue(v2.toFixed(2), true)
                    }
                    text = Math.round(VideoTabController.brightnessOpacityValue * 100) + "%"
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true

            MyToggleButton {
                id: colorToggle
                text: "Toggle On/Off"
                onCheckedChanged: {
                    VideoTabController.setColorOverlayEnabled(this.checked, true)                }
            }

            Item {
                Layout.preferredWidth: 150
            }

            MyText {
                text: "Opacity:"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MySlider {
                id: opacitySlider
                from: 0.01
                to: 0.85
                stepSize: 0.01
                value: 0.01
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = (this.value * 100)
                    opacityValueText.text = Math.round(val) + "%"
                }
                onValueChanged: {

                    VideoTabController.setColorOverlayOpacity(this.value.toFixed(2), true)
                }
            }

            MyTextField {
                id: opacityValueText
                text: "1%"
                keyBoardUID: 903
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 5) {
                            val = 5
                        } else if (val > 85.0) {
                            val = 85.0
                        }
                        var v = (val/100).toFixed(2)
                            opacitySlider.value = v
                    }
                    text = Math.round(VideoTabController.colorOverlayOpacity * 100) + "%"
                }
            }
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
                keyBoardUID: 915
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
                            VideoTabController.setColorRed(v.toFixed(2), true)
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
                keyBoardUID: 916
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
                            VideoTabController.setColorGreen(v.toFixed(2), true)
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
                    keyBoardUID: 917
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
                                VideoTabController.setColorBlue(v.toFixed(2), true)
                        }
                        text = Math.round(VideoTabController.colorBlue * 100) + "%"
                    }
                }

        }
    }

    Component.onCompleted: {
        brightnessSlider.value = VideoTabController.brightnessOpacityValue
        brightnessToggle.checked = VideoTabController.brightnessEnabled
        colorToggle.checked = VideoTabController.colorOverlayEnabled
        opacitySlider.value = VideoTabController.colorOverlayOpacity
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

        onBrightnessOpacityValueChanged: {
            var brightvalue = VideoTabController.brightnessOpacityValue
            brightnessSlider.value = brightvalue
            brightnessValueText.text = Math.round(brightvalue*100)+ "%"
        }
        onBrightnessEnabledChanged: {
            brightnessToggle.checked = VideoTabController.brightnessEnabled
        }
        onColorOverlayOpacityChanged:{
            opacitySlider.value = VideoTabController.colorOverlayOpacity
        }
        onColorOverlayEnabledChanged:{
            colorToggle.checked = VideoTabController.colorOverlayEnabled
        }

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
