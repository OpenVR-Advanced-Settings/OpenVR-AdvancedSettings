import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: spaceDragGroupBox
    Layout.fillWidth: true

    title: qsTr("Space Drag")

    // Override devices are UI only for now. Empty string means not set.
    property string leftOverrideDevice: ""
    property string rightOverrideDevice: ""
    property string customOverrideDevice: ""

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            MyToggleButton {
                id: moveShortcutLeft
                text: qsTr("Left Hand")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Layout.rightMargin: 0
                onCheckedChanged: {
                    MoveCenterTabController.moveShortcutLeft = this.checked
                }
            }

            VerticalSeparator {
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredWidth: 1

                MyPushButton2 {
                    text: qsTr("Override")
                    onClicked: {
                        spaceDragDeviceDialog.openPopup(qsTr("Left Hand Override"),
                                                        leftOverrideDevice,
                                                        function(device) { leftOverrideDevice = device })
                    }
                }

                MyText {
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideRight
                    text: leftOverrideDevice === "" ? qsTr("Not Set") : leftOverrideDevice
                }
            }
        }

        LineSeparator {
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            MyToggleButton {
                id: moveShortcutRight
                text: qsTr("Right Hand")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Layout.rightMargin: 0
                onCheckedChanged: {
                    MoveCenterTabController.moveShortcutRight = this.checked
                }
            }

            VerticalSeparator {
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredWidth: 1

                MyPushButton2 {
                    text: qsTr("Override")
                    onClicked: {
                        spaceDragDeviceDialog.openPopup(qsTr("Right Hand Override"),
                                                        rightOverrideDevice,
                                                        function(device) { rightOverrideDevice = device })
                    }
                }

                MyText {
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideRight
                    text: rightOverrideDevice === "" ? qsTr("Not Set") : rightOverrideDevice
                }
            }
        }

        LineSeparator {
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            MyToggleButton {
                id: moveShortcutCustom
                text: qsTr("Custom")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Layout.rightMargin: 0
            }

            VerticalSeparator {
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredWidth: 1

                MyPushButton2 {
                    text: qsTr("Override")
                    onClicked: {
                        spaceDragDeviceDialog.openPopup(qsTr("Custom Override"),
                                                        customOverrideDevice,
                                                        function(device) { customOverrideDevice = device })
                    }
                }

                MyText {
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideRight
                    text: customOverrideDevice === "" ? qsTr("Not Set") : customOverrideDevice
                }
            }
        }

        LineSeparator {
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredWidth: 1

                MyText {
                    text: qsTr("Comfort Mode")
                    Layout.rightMargin: 10
                }

                MySlider {
                    id: dragComfortSlider
                    from: 0
                    to: 10
                    stepSize: 1
                    value: 0
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
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

            VerticalSeparator {
            }

            MyToggleButton {
                id: dragBounds
                text: qsTr("Force Bounds")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Layout.rightMargin: 0
                onCheckedChanged: {
                    MoveCenterTabController.dragBounds = this.checked
                }
            }
        }

        LineSeparator {
        }

        RowLayout {
            Layout.fillWidth: true

            MyText {
                text: qsTr("Drag Multiplier")
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
