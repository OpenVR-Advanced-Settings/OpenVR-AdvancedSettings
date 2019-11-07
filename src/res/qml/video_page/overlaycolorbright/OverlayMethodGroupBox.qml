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
                from: 0.01
                to: 1.0
                stepSize: 0.01
                value: 1.0
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = (this.position * 100)
                    brightnessValueText.text = Math.round(val) + "%"
                }
                onValueChanged: {

                    VideoTabController.setBrightnessValue(value.toFixed(2), true)
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
                        if (val2 < 5) {
                            val2 = 5
                        } else if (val2 > 100.0) {
                            val2 = 100.0
                        }
                        var v2 = (val2/100).toFixed(2)
                            brightnessSlider.value = v
                    }
                    text = Math.round(VideoTabController.brightnessValue * 100) + "%"
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
                to: 1.0
                stepSize: 0.01
                value: 0.0
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = (this.position * 100)
                    opacityValueText.text = Math.round(val) + "%"
                }
                onValueChanged: {

                    VideoTabController.setColorOverlayOpacity(value.toFixed(2), true)
                }
            }

            MyTextField {
                id: opacityValueText
                text: "100%"
                keyBoardUID: 903
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
                }
                onValueChanged: {

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
                }
                onValueChanged: {

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
                    }
                    onValueChanged: {

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
                        }
                        text = Math.round(VideoTabController.colorBlue * 100) + "%"
                    }
                }

        }
    }

    Component.onCompleted: {
        brightnessSlider.value = VideoTabController.brightnessValue
        brightnessToggle.checked = VideoTabController.brightnessEnabled
        colorToggle.checked = VideoTabController.colorOverlayEnabled
        opacitySlider.value = VideoTabController.colorOverlayOpacity
        //opacityValueText.text = Math.round(VideoTabController.colorOverlayOpacity * 100) + "%"
        colorRedSlider.value = VideoTabController.colorRed
        colorGreenSlider.value = VideoTabController.colorGreen
        colorBlueSlider.value = VideoTabController.colorBlue
    }

    Connections {
        target: VideoTabController

        onBrightnessValueChanged: {
            brightnessSlider.value = VideoTabController.brightnessValue
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
