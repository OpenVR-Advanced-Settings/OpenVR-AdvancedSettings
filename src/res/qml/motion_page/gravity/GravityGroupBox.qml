import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: gravityGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Gravity Settings"
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
                id: gravityToggleButton
                text: "On"
                onCheckedChanged: {
                    MoveCenterTabController.gravityActive = this.checked
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MyText {
                text: "Gravity Strength (+ is down):"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 2
            }

            MyTextField {
                id: gravityStrengthText
                text: "9.80"
                keyBoardUID: 152
                Layout.preferredWidth: 120
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        MoveCenterTabController.gravityStrength = val.toFixed(2)
                        text = MoveCenterTabController.gravityStrength.toFixed(2)
                    } else {
                        text = MoveCenterTabController.gravityStrengtht.toFixed(2)
                    }
                }
            }

            MyPushButton {
                id: gravityMoonButton
                Layout.preferredWidth: 110
                text:"Moon"
                onClicked: {
                    MoveCenterTabController.gravityStrength = 1.62
                }
           }
            MyPushButton {
                id: gravityMarsButton
                Layout.preferredWidth: 110
                text:"Mars"
                onClicked: {
                    MoveCenterTabController.gravityStrength = 3.71
                }
           }
            MyPushButton {
                id: gravityEarthButton
                Layout.preferredWidth: 110
                text:"Earth"
                onClicked: {
                    MoveCenterTabController.gravityStrength = 9.80
                }
           }
            MyPushButton {
                id: gravityJupiterButton
                Layout.preferredWidth: 110
                text:"Jupiter"
                onClicked: {
                    MoveCenterTabController.gravityStrength = 24.79
                }
           }
        }
        RowLayout {
            Layout.fillWidth: true

            MyToggleButton {
                id: momentumToggleButton
                text: "Save Momentum"
                onCheckedChanged: {
                    MoveCenterTabController.momentumSave = this.checked
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MyText {
                text: "Fling Strength:"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 2
            }

            MyTextField {
                id: flingStrengthText
                text: "1.0"
                keyBoardUID: 153
                Layout.preferredWidth: 120
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        MoveCenterTabController.flingStrength = val.toFixed(1)
                        text = MoveCenterTabController.flingStrength.toFixed(1)
                    } else {
                        text = MoveCenterTabController.flingStrength.toFixed(1)
                    }
                }
            }

            MyPushButton2 {
                Layout.preferredWidth: 50
                text: "1x"
                onClicked: {
                    MoveCenterTabController.flingStrength = 1.0
                }
            }

            MyPushButton2 {
                Layout.preferredWidth: 50
                text: "2x"
                onClicked: {
                    MoveCenterTabController.flingStrength = 2.0
                }
            }

            MyPushButton2 {
                Layout.preferredWidth: 50
                text: "3x"
                onClicked: {
                    MoveCenterTabController.flingStrength = 3.0
                }
            }

            MyPushButton2 {
                Layout.preferredWidth: 50
                text: "4x"
                onClicked: {
                    MoveCenterTabController.flingStrength = 4.0
                }
            }


        }
    }

    Component.onCompleted: {
        gravityToggleButton.checked = MoveCenterTabController.gravityActive
        gravityStrengthText.text = MoveCenterTabController.gravityStrength.toFixed(2)
        momentumToggleButton.checked = MoveCenterTabController.momentumSave
        flingStrengthText.text = MoveCenterTabController.flingStrength.toFixed(2)
    }

    Connections {
        target: MoveCenterTabController

        onGravityActiveChanged: {
            gravityToggleButton.checked = MoveCenterTabController.gravityActive
        }
        onGravityStrengthChanged: {
            gravityStrengthText.text = MoveCenterTabController.gravityStrength.toFixed(2)
        }
        onMomentumSaveChanged: {
            momentumToggleButton.checked = MoveCenterTabController.momentumSave
        }
        onFlingStrengthChanged: {
            flingStrengthText.text = MoveCenterTabController.flingStrength.toFixed(2)
        }
    }
}
