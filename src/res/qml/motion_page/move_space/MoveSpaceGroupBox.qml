import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    Layout.fillWidth: true
    id: moveSpaceGroupBox
    visible: true

    title: qsTr("Move Space")
    ColumnLayout {
        anchors.fill: parent

        GridLayout {
            Layout.fillWidth: true
            columns: 7
            rowSpacing: 0

            MyText {
                text: qsTr("X-Axis (Left/Right)")
                Layout.preferredWidth: 340
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "0"
                onClicked: {
                    MoveCenterTabController.offsetX = 0
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                id: spaceMoveXMinusButton
                Layout.preferredWidth: 40
                text: "-"
                onClicked: {
                    MoveCenterTabController.modOffsetX(-0.1)
                }
            }

					MyTextField {
                topPadding: 4
                bottomPadding: 4
                id: spaceMoveXText
                text: "0.00"
						keyBoardUID: 101
                Layout.preferredWidth: 140
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        MoveCenterTabController.offsetX = val.toFixed(2)
                        text = MoveCenterTabController.offsetX.toFixed(2)
                    } else {
                        text = MoveCenterTabController.offsetX.toFixed(2)
                    }
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                id: spaceMoveXPlusButton
                Layout.preferredWidth: 40
                text: "+"
                onClicked: {
                    MoveCenterTabController.modOffsetX(0.1)
                }
            }

            MyToggleButton {
                topPadding: 4
                bottomPadding: 4
                id: lockXToggle
                Layout.leftMargin: 20
                text: qsTr("Lock X")
                onCheckedChanged: {
                    MoveCenterTabController.lockXToggle = this.checked
                }
            }
            Item {
                Layout.fillWidth: true
            }

            MyText {
                text: qsTr("Y-Axis (Down/Up)")
                Layout.preferredWidth: 340
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "0"
                onClicked: {
                    MoveCenterTabController.offsetY = 0
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "-"
                onClicked: {
                    MoveCenterTabController.modOffsetY(-0.1)
                }
            }

            MyTextField {
                topPadding: 4
                bottomPadding: 4
                id: spaceMoveYText
                text: "0.00"
                keyBoardUID: 102
                Layout.preferredWidth: 140
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        MoveCenterTabController.offsetY = val.toFixed(2)
                        text = MoveCenterTabController.offsetY.toFixed(2)
                    } else {
                        text = MoveCenterTabController.offsetY.toFixed(2)
                    }
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "+"
                onClicked: {
                    MoveCenterTabController.modOffsetY(0.1)
                }
            }


            MyToggleButton {
                topPadding: 4
                bottomPadding: 4
                id: lockYToggle
                Layout.leftMargin: 20
                text: qsTr("Lock Y")
                onCheckedChanged: {
                    MoveCenterTabController.lockYToggle = this.checked
                }
            }
            Item {
                Layout.fillWidth: true
            }

            MyText {
                text: qsTr("Z-Axis (Forth/Back)")
                Layout.preferredWidth: 340
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "0"
                onClicked: {
                    MoveCenterTabController.offsetZ = 0
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "-"
                onClicked: {
                    MoveCenterTabController.modOffsetZ(-0.1)
                }
            }

            MyTextField {
                topPadding: 4
                bottomPadding: 4
                id: spaceMoveZText
                text: "0.00"
                keyBoardUID: 103
                Layout.preferredWidth: 140
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        MoveCenterTabController.offsetZ = val.toFixed(2)
                        text = MoveCenterTabController.offsetZ.toFixed(2)
                    } else {
                        text = MoveCenterTabController.offsetZ.toFixed(2)
                    }
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "+"
                onClicked: {
                    MoveCenterTabController.modOffsetZ(0.1)
                }
            }


            MyToggleButton {
                topPadding: 4
                bottomPadding: 4
                id: lockZToggle
                Layout.leftMargin: 20
                text: qsTr("Lock Z")
                onCheckedChanged: {
                    MoveCenterTabController.lockZToggle = this.checked
                }
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }

    Component.onCompleted: {
        spaceMoveXText.text = MoveCenterTabController.offsetX.toFixed(2)
        spaceMoveYText.text = MoveCenterTabController.offsetY.toFixed(2)
        spaceMoveZText.text = MoveCenterTabController.offsetZ.toFixed(2)
        lockXToggle.checked = MoveCenterTabController.lockXToggle
        lockYToggle.checked = MoveCenterTabController.lockYToggle
        lockZToggle.checked = MoveCenterTabController.lockZToggle
    }

    Connections {
        target: MoveCenterTabController
        function onOffsetXChanged() {
            spaceMoveXText.text = MoveCenterTabController.offsetX.toFixed(2)
        }
        function onOffsetYChanged() {
            spaceMoveYText.text = MoveCenterTabController.offsetY.toFixed(2)
        }
        function onOffsetZChanged() {
            spaceMoveZText.text = MoveCenterTabController.offsetZ.toFixed(2)
        }
        function onLockXToggleChanged() {
            lockXToggle.checked = MoveCenterTabController.lockXToggle
        }
        function onLockYToggleChanged() {
            lockYToggle.checked = MoveCenterTabController.lockYToggle
        }
        function onLockZToggleChanged() {
            lockZToggle.checked = MoveCenterTabController.lockZToggle
        }
    }
}
