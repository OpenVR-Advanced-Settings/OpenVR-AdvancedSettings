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
                    MoveCenterTabController.turnBindLeft = this.checked
                }
            }

            Item {
                Layout.fillWidth: true
            }

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
                value: 1.0.
                    var val = (this.position * 100)/2 + 50
                    chaperoneVisibilityText.text = Math.round(val) + "%"
                }
                onValueChanged: {
                    //TODO change value
                    ChaperoneTabController.setBoundsVisibility(value.toFixed(2), false)
                }
            }

            MyTextField {
                id: chaperoneVisibilityText
                text: "100.00"
                keyBoardUID: 301
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 50.0) {
                            val = 50.0
                        } else if (val > 100.0) {
                            val = 100.0
                        }
                        var v = (val/100).toFixed(2)
                        if (v <= chaperoneVisibilitySlider.to) {
                            chaperoneVisibilitySlider.value = v
                        } else {
                            ChaperoneTabController.setBoundsVisibility(v, false)
                        }
                    }
                    text = Math.round(ChaperoneTabController.boundsVisibility * 100) + "%"
                }
            }
        }
    }

    Component.onCompleted: {
        turnBindLeft.checked = MoveCenterTabController.turnBindLeft
        turnBindRight.checked = MoveCenterTabController.turnBindRight
        turnComfortSlider.value = MoveCenterTabController.turnComfortFactor
    }

    Connections {
        target: MoveCenterTabController

        onTurnBindLeftChanged: {
            turnBindLeft.checked = MoveCenterTabController.turnBindLeft
        }
        onTurnBindRightChanged: {
            turnBindRight.checked = MoveCenterTabController.turnBindRight
        }
        onTurnComfortFactorChanged: {
            turnComfortSlider.value = MoveCenterTabController.turnComfortFactor
        }
    }
}
