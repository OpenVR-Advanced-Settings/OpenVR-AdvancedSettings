import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: spaceDragGroupBox
    Layout.fillWidth: true

    title: "Space Drag"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            spacing: 16

            MyToggleButton {
                id: moveShortcutLeft
                text: "Left Hand"
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Layout.rightMargin: 0
                Layout.preferredHeight: 48
                onCheckedChanged: {
                    MoveCenterTabController.moveShortcutLeft = this.checked
                }
            }
            VerticalSeparator {
            }
            MyToggleButton {
                id: moveShortcutRight
                text: "Right Hand"
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Layout.rightMargin: 0
                Layout.preferredHeight: 48
                onCheckedChanged: {
                    MoveCenterTabController.moveShortcutRight = this.checked
                }
            }
        }
        LineSeparator {
        }
        RowLayout {
            spacing: 16
            Layout.leftMargin: 32

            MyText {
                text: "Comfort Mode"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
            }
            MySlider {
                id: dragComfortSlider
                from: 0
                to: 10
                stepSize: 1
                value: 0
                Layout.preferredWidth: 300
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                onValueChanged: {
                    dragComfortText.text = dragComfortSlider.value
                    MoveCenterTabController.dragComfortFactor = dragComfortSlider.value
                }
            }
            MyText {
                id: dragComfortText
                text: "0"
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: 30
            }
        }
        LineSeparator {
        }
        RowLayout {
            spacing: 16
            Layout.leftMargin: 32

            MyText {
                text: "Drag Multiplier"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
            }
            MyTextField {
                id: dragMultText
                text: "1.0"
                keyBoardUID: 157
                Layout.preferredWidth: 120
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        MoveCenterTabController.dragMult = val.toFixed(2)
                        text = MoveCenterTabController.dragMult.toFixed(2)
                    } else {
                        text = MoveCenterTabController.dragMult.toFixed(2)
                    }
                }
            }
            MyPushButton2 {
                Layout.preferredWidth: 50
                text: "1x"
                onClicked: {
                    MoveCenterTabController.dragMult = 1.0
                }
            }
            MyPushButton2 {
                Layout.preferredWidth: 50
                text: "2x"
                onClicked: {
                    MoveCenterTabController.dragMult = 2.0
                }
            }
            MyPushButton2 {
                Layout.preferredWidth: 50
                text: "3x"
                onClicked: {
                    MoveCenterTabController.dragMult = 3.0
                }
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: dragBounds
            text: "Force Bounds"
            Layout.fillWidth: true
            Layout.leftMargin: 32
            Layout.rightMargin: 0
            Layout.preferredHeight: 48
            onCheckedChanged: {
                MoveCenterTabController.dragBounds = this.checked
            }
        }
    }

    Component.onCompleted: {
        moveShortcutLeft.checked = MoveCenterTabController.moveShortcutLeft
        moveShortcutRight.checked = MoveCenterTabController.moveShortcutRight
        dragComfortSlider.value = MoveCenterTabController.dragComfortFactor
        dragBounds.checked = MoveCenterTabController.dragBounds
        dragMultText.text = MoveCenterTabController.dragMult.toFixed(2)
    }

    Connections {
        target: MoveCenterTabController

        function onMoveShortcutLeftChanged() {
            moveShortcutLeft.checked = MoveCenterTabController.moveShortcutLeft
        }
        function onMoveShortcutRightChanged() {
            moveShortcutRight.checked = MoveCenterTabController.moveShortcutRight
        }
        function onDragComfortFactorChanged() {
            dragComfortSlider.value = MoveCenterTabController.dragComfortFactor
        }
        function onDragBoundsChanged() {
            dragBounds.checked = MoveCenterTabController.dragBounds
        }
        function onDragMultChanged() {
            dragMultText.text = MoveCenterTabController.dragMult.toFixed(2)
        }
    }
}
