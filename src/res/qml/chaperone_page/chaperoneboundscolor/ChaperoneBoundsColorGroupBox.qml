import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"
import "." // QTBUG-34418, singletons require explicit import to load qmldir file

SettingsGroupBox {
    id: chaperoneBoundsColorGroupBox
    Layout.fillWidth: true

    title: qsTr("Chaperone Color")

    function toHexByte(val) {
        var h = Math.round(val).toString(16).toUpperCase()
        return h.length < 2 ? "0" + h : h
    }

    function updateHexText() {
        colorHexText.text = "#" + toHexByte(chaperoneColorRedSlider.value)
                + toHexByte(chaperoneColorGreenSlider.value)
                + toHexByte(chaperoneColorBlueSlider.value)
    }

    GridLayout {
        anchors.fill: parent
        columns: 7

                MyText {
                    text: qsTr("Red")
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 75
                }

                MySlider {
                    id: chaperoneColorRedSlider
                    from: 0
                    to: 255
                    stepSize: 5
                    value: 0
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = (this.value).toFixed(0)
                        colorRedText.text = val
                        ChaperoneTabController.setChaperoneColorR(val, true)
                        updateHexText()
                    }
                }

                MyTextField {
                    id: colorRedText
                    text: "0"
                    keyBoardUID: 320
                    Layout.preferredWidth: 100
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseInt(input)
                        if (!isNaN(val)) {
                            if (val < 0) {
                                val = 0
                            } else if (val > 255) {
                                val = 255
                            }
                            ChaperoneTabController.setChaperoneColorR(val, true)
                            chaperoneColorRedSlider.value = val
                        }
                        text = val
                    }
                }

                Item{
                    Layout.preferredWidth: 200
                }
                MyText {
                    text: qsTr("Green")
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 75
                }

                MySlider {
                    id: chaperoneColorGreenSlider
                    from: 0
                    to: 255
                    stepSize: 5
                    value: 255
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = (this.value).toFixed(0)
                        colorGreenText.text = val
                        ChaperoneTabController.setChaperoneColorG(val, true)
                        updateHexText()
                    }
                }

                MyTextField {
                    id: colorGreenText
                    text: "255"
                    keyBoardUID: 321
                    Layout.preferredWidth: 100
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseInt(input)
                        if (!isNaN(val)) {
                            if (val < 0) {
                                val = 0
                            } else if (val > 255) {
                                val = 255
                            }
                            ChaperoneTabController.setChaperoneColorG(val, true)
                            chaperoneColorGreenSlider.value = val
                        }
                        text = val
                    }
                }

                MyText {
                    text: qsTr("Blue")
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 75
                }

                MySlider {
                    id: chaperoneColorBlueSlider
                    from: 0
                    to: 255
                    stepSize: 5
                    value: 255
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = (this.value).toFixed(0)
                        colorBlueText.text = val
                        ChaperoneTabController.setChaperoneColorB(val, true)
                        updateHexText()
                    }
                }

                MyTextField {
                    id: colorBlueText
                    text: "255"
                    keyBoardUID: 322
                    Layout.preferredWidth: 100
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseInt(input)
                        if (!isNaN(val)) {
                            if (val < 0) {
                                val = 0
                            } else if (val > 255) {
                                val = 255
                            }
                            ChaperoneTabController.setChaperoneColorB(val, true)
                            chaperoneColorBlueSlider.value = val
                        }
                        text = val
                    }
                }

                Item{
                    Layout.preferredWidth: 200
                }
                MyText {
                    text: qsTr("Hex")
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 75
                }

                MyTextField {
                    id: colorHexText
                    text: qsTr("#00FFFF")
                    keyBoardUID: 323
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var hex = input.trim().replace(/^#/, "")
                        if (/^[0-9a-fA-F]{6}$/.test(hex)) {
                            var r = parseInt(hex.substring(0, 2), 16)
                            var g = parseInt(hex.substring(2, 4), 16)
                            var b = parseInt(hex.substring(4, 6), 16)
                            ChaperoneTabController.setChaperoneColorR(r, true)
                            ChaperoneTabController.setChaperoneColorG(g, true)
                            ChaperoneTabController.setChaperoneColorB(b, true)
                            chaperoneColorRedSlider.value = r
                            chaperoneColorGreenSlider.value = g
                            chaperoneColorBlueSlider.value = b
                        }
                        updateHexText()
                    }
                }
    }
    Component.onCompleted: {
        // set up var here to prevent double call to c++ from qml
        var redValue = ChaperoneTabController.chaperoneColorR
        var greenValue = ChaperoneTabController.chaperoneColorG
        var blueValue = ChaperoneTabController.chaperoneColorB
       colorRedText.text = redValue
       colorGreenText.text = greenValue
       colorBlueText.text = blueValue
       chaperoneColorRedSlider.value = redValue
       chaperoneColorGreenSlider.value = greenValue
       chaperoneColorBlueSlider.value = blueValue
       updateHexText()
    }

    Connections {
        target: ChaperoneTabController
        function onChaperoneColorRChanged(){
            var redValue = ChaperoneTabController.chaperoneColorR
            colorRedText.text = redValue
            chaperoneColorRedSlider.value = redValue
            updateHexText()
        }
        function onChaperoneColorGChanged(){
            var greenValue = ChaperoneTabController.chaperoneColorG
            colorGreenText.text = greenValue
            chaperoneColorGreenSlider.value = greenValue
            updateHexText()
        }
        function onChaperoneColorBChanged(){
            var blueValue = ChaperoneTabController.chaperoneColorB
            colorBlueText.text = blueValue
            chaperoneColorBlueSlider.value = blueValue
            updateHexText()
        }
    }
}
