import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: spaceDragGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Space Drag"
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
                id: moveShortcutLeft
                text: "Left Hand"
                onCheckedChanged: {
                    MoveCenterTabController.moveShortcutLeft = this.checked
                }
            }

            MyToggleButton {
                id: moveShortcutRight
                text: "Right Hand"
                onCheckedChanged: {
                    MoveCenterTabController.moveShortcutRight = this.checked
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MyText {
                text: "Comfort Mode:"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MySlider {
                id: dragComfortSlider
                from: 0
                to: 10
                stepSize: 1
                value: 0
                Layout.preferredWidth: 180
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
                Layout.rightMargin: 10
            }

            MyToggleButton {
                id: dragBounds
                text: "Force Bounds"
                onCheckedChanged: {
                    MoveCenterTabController.dragBounds = this.checked
                }
            }
        }
        RowLayout{

            Item{
                Layout.fillWidth: true
            }

            MyText {
                text: "Drag Multiplier:"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 2
            }

            MyTextField {
                id: dragMultText
                text: "1.0"
                keyBoardUID: 157
                Layout.preferredWidth: 120
                Layout.leftMargin: 10
                Layout.rightMargin: 10
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

        onMoveShortcutLeftChanged: {
            moveShortcutLeft.checked = MoveCenterTabController.moveShortcutLeft
        }
        onMoveShortcutRightChanged: {
            moveShortcutRight.checked = MoveCenterTabController.moveShortcutRight
        }
        onDragComfortFactorChanged: {
            dragComfortSlider.value = MoveCenterTabController.dragComfortFactor
        }
        onDragBoundsChanged: {
            dragBounds.checked = MoveCenterTabController.dragBounds
        }
        onDragMultChanged: {
            dragMultText.text = MoveCenterTabController.dragMult.toFixed(2)
        }
    }
}
