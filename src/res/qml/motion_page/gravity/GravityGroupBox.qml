import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: gravityGroupBox
    Layout.fillWidth: true

    title: qsTr("Gravity Settings")

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        MyToggleButton {
            id: gravityToggleButton
            text: qsTr("Enable Gravity")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            Layout.preferredHeight: 48
            onCheckedChanged: {
                MoveCenterTabController.gravityActive = this.checked
            }
        }
        LineSeparator {
        }
        RowLayout {
            spacing: 16
            Layout.leftMargin: 32

            MyText {
                text: qsTr("Gravity Strength (+ is down)")
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
            }
            MyTextField {
                id: gravityStrengthText
                text: "9.80"
                keyBoardUID: 152
                Layout.preferredWidth: 120
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        MoveCenterTabController.gravityStrength = val.toFixed(2)
                        text = MoveCenterTabController.gravityStrength.toFixed(2)
                    } else {
                        text = MoveCenterTabController.gravityStrength.toFixed(2)
                    }
                }
            }
            MyPushButton {
                id: gravityMoonButton
                Layout.preferredWidth: 110
                text: qsTr("Moon")
                onClicked: {
                    MoveCenterTabController.gravityStrength = 1.62
                }
            }
            MyPushButton {
                id: gravityMarsButton
                Layout.preferredWidth: 110
                text: qsTr("Mars")
                onClicked: {
                    MoveCenterTabController.gravityStrength = 3.71
                }
            }
            MyPushButton {
                id: gravityEarthButton
                Layout.preferredWidth: 110
                text: qsTr("Earth")
                onClicked: {
                    MoveCenterTabController.gravityStrength = 9.80
                }
            }
            MyPushButton {
                id: gravityJupiterButton
                Layout.preferredWidth: 110
                text: qsTr("Jupiter")
                onClicked: {
                    MoveCenterTabController.gravityStrength = 24.79
                }
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: momentumToggleButton
            text: qsTr("Save Momentum")
            Layout.fillWidth: true
            Layout.leftMargin: 32
            Layout.rightMargin: 0
            Layout.preferredHeight: 48
            onCheckedChanged: {
                MoveCenterTabController.momentumSave = this.checked
            }
        }
        LineSeparator {
        }
        RowLayout {
            spacing: 16
            Layout.leftMargin: 32

            MyText {
                text: qsTr("Friction")
                horizontalAlignment: Text.AlignLeft
            }
            MyTextField {
                id: frictionPercentText
                text: "0%"
                keyBoardUID: 156
                Layout.preferredWidth: 120
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseInt(input)
                    if (!isNaN(val)) {
                        if ( val > 999 )
                        {
                            val = 999
                        }
                        if ( val < 0 )
                        {
                            val = 0
                        }

                        MoveCenterTabController.frictionPercent = val
                        text = MoveCenterTabController.frictionPercent + "%"
                    } else {
                        text = MoveCenterTabController.frictionPercent + "%"
                    }
                }
            }
            Item {
                Layout.fillWidth: true
            }
            MyText {
                text: qsTr("Fling Strength")
                horizontalAlignment: Text.AlignLeft
            }
            MyTextField {
                id: flingStrengthText
                text: "1.0"
                keyBoardUID: 153
                Layout.preferredWidth: 120
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        MoveCenterTabController.flingStrength = val.toFixed(2)
                        text = MoveCenterTabController.flingStrength.toFixed(2)
                    } else {
                        text = MoveCenterTabController.flingStrength.toFixed(2)
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
        }
    }

    Component.onCompleted: {
        gravityToggleButton.checked = MoveCenterTabController.gravityActive
        gravityStrengthText.text = MoveCenterTabController.gravityStrength.toFixed(2)
        momentumToggleButton.checked = MoveCenterTabController.momentumSave
        flingStrengthText.text = MoveCenterTabController.flingStrength.toFixed(2)
        frictionPercentText.text = MoveCenterTabController.frictionPercent + "%"
    }

    Connections {
        target: MoveCenterTabController

        function onGravityActiveChanged() {
            gravityToggleButton.checked = MoveCenterTabController.gravityActive
        }
        function onGravityStrengthChanged() {
            gravityStrengthText.text = MoveCenterTabController.gravityStrength.toFixed(2)
        }
        function onMomentumSaveChanged() {
            momentumToggleButton.checked = MoveCenterTabController.momentumSave
        }
        function onFlingStrengthChanged() {
            flingStrengthText.text = MoveCenterTabController.flingStrength.toFixed(2)
        }
        function onFrictionPercentChanged() {
            frictionPercentText.text = MoveCenterTabController.frictionPercent + "%"
        }
    }
}
