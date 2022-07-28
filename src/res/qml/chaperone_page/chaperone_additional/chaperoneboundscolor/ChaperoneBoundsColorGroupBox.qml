import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../../common"
import "." // QTBUG-34418, singletons require explicit import to load qmldir file

GroupBox {
    id: chaperoneBoundsColorGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Chaperone Color"
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

            RowLayout{

                MyText {
                    text: "Red:"
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
                    text: "Green:"
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

            }
            RowLayout{
                MyText {
                    text: "Blue:"
                    horizontalAlignment: Text.AlignRight
                    Layout.preferredWidth: 75
                }

                MySlider {
                    id: chaperoneColorBlueSlider
                    from: 0
                    to: 255
                    stepSize: 5
                    value: 255
                    Layout.preferredWidth: 250
                    onPositionChanged: {
                        var val = (this.value).toFixed(0)
                        colorBlueText.text = val
                        ChaperoneTabController.setChaperoneColorB(val, true)
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

    }

    Connections {
        target: ChaperoneTabController
        onChaperoneColorRChanged:{
            var redValue = ChaperoneTabController.chaperoneColorR
            colorRedText.text = redValue
            chaperoneColorRedSlider.value = redValue
        }
        onChaperoneColorGChanged:{
            var greenValue = ChaperoneTabController.chaperoneColorG
            colorGreenText.text = greenValue
            chaperoneColorGreenSlider.value = greenValue
        }
        onChaperoneColorBChanged:{
            var blueValue = ChaperoneTabController.chaperoneColorB
            colorBlueText.text = blueValue
            chaperoneColorBlueSlider.value = blueValue
        }
    }
}
