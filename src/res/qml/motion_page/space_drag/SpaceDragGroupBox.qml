import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"

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
                id: moveShortcutLeft
                text: "Enable Left Hand"
                onCheckedChanged: {
                    MoveCenterTabController.moveShortcutLeft = this.checked
                }
            }

            MyToggleButton {
                id: moveShortcutRight
                text: "Enable Right Hand"
                onCheckedChanged: {
                    MoveCenterTabController.moveShortcutRight = this.checked
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MyText {
                text: "Drag Comfort Mode:"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MySlider {
                id: dragComfortSlider
                from: 0
                to: 10
                stepSize: 1
                value: 0
                Layout.preferredWidth: 200
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
        }
    }

    Component.onCompleted: {
        moveShortcutLeft.checked = MoveCenterTabController.moveShortcutLeft
        moveShortcutRight.checked = MoveCenterTabController.moveShortcutRight
        dragComfortSlider.value = MoveCenterTabController.dragComfortFactor
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
    }
}
