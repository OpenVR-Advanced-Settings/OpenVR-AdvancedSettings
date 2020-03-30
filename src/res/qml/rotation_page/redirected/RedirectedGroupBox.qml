import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: redirectedGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Redirected Walking"
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
                id: autoTurn
                text: "Toggle On/Off"
                onCheckedChanged: {
                    RotationTabController.setAutoTurnEnabled(this.checked, true);                }
            }

            Item {
                Layout.preferredWidth: 150
            }

            MyText {
                text: "Activation Dist:"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MySlider {
                id: activationSlider
                from: 0
                to: 1.0
                stepSize: 0.1
                value: .4
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = this.value
                    activationValueText.text = val.toFixed(2)
                }
                onValueChanged: {

                    RotationTabController.setAutoTurnActivationDistance(value, true)
                }
            }

            MyTextField {
                id: activationValueText
                text: "0.4"
                keyBoardUID: 1001
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0) {
                            val = 0
                        } else if (val > 22) {
                            val = 22
                        }
                            activationSlider.value = v;
                    }
                    text =  RotationTabController.autoTurnActivationDistance();
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true

            MyToggleButton {
                id: cornerAngle
                text: "Use Corner Angle"
                onCheckedChanged: {
                    RotationTabController.setAutoTurnUseCornerAngle(this.checked, true);                }
            }

            Item {
                Layout.preferredWidth: 150
            }

            MyText {
                text: "DeActivation Dist:"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MySlider {
                id: deactivationSlider
                from: 0
                to: 1.0
                stepSize: 0.1
                value: .15
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = this.value
                    deactivationValueText.text = val.toFixed(2)
                }
                onValueChanged: {

                    RotationTabController.setAutoTurnDeactivationDistance(value, true)
                }
            }

            MyTextField {
                id: deactivationValueText
                text: "0.15"
                keyBoardUID: 1002
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0) {
                            val = 0
                        } else if (val > 22) {
                            val = 22
                        }
                            deactivationSlider.value = v;
                    }
                    text =  RotationTabController.autoTurnDeactivationDistance();
                }
            }
        }
        //TODO duplicate and change
        /*RowLayout {
            Layout.fillWidth: true


            MyText {
                text: "Cord De-tangle start angle:"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }


            MyTextField {
                id: cordDetanglingAngle
                text: "360"
                keyBoardUID: 1003
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0) {
                            val = 0
                        } else if (val > 22) {
                            val = 22
                        }
                            deactivationSlider.value = v;
                    }
                    text =  RotationTabController.autoTurnDeactivationDistance();
                }
            }
        }*/


    }

    Component.onCompleted: {
        activationSlider.value = RotationTabController.autoTurnActivationDistance
        autoTurn.checked = RotationTabController.autoTurnEnabled
        deactivationSlider.value = RotationTabController.autoTurnDeactivationDistance
        cornerAngle.value = RotationTabController.autoTurnUseCornerAngle
    }

    Connections {
        target: RotationTabController

        onAutoTurnEnabledChanged: {
            activationSlider.value = RotationTabController.autoTurnActivationDistance
        }
        onAutoTurnActivationDistanceChanged: {
            autoTurn.checked = RotationTabController.autoTurnEnabled
        }
        onAutoTurnDeactivationDistanceChanged:{
            deactivationSlider.value = RotationTabController.autoTurnDeactivationDistance
        }
        onAutoTurnUseCornerAngleChanged:{
            cornerAngle.checked = RotationTabController.setAutoTurnUseCornerAngle
        }
    }
}
