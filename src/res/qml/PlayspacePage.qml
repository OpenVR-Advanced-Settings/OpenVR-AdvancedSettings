import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
import "common"


MyStackViewPage {
    headerText: "Room Settings"

    content: ColumnLayout {
        spacing: 18

        RowLayout {
            MyText {
                text: "Tracking Universe:"
                Layout.preferredWidth: 230
            }
            MyText {
                id: roomModeText
                font.bold: true
                text: "Standing"
            }
        }

        GroupBox {
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Move Room"
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

                GridLayout {
                    columns: 6

                    MyText {
                        text: "X-Axis (Left/Right):"
                        Layout.preferredWidth: 340
                    }

                    MyText {
                        text: " "
                        Layout.preferredWidth: 40
                    }

                    MyPushButton2 {
                        id: roomMoveXMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            MoveCenterTabController.modOffsetX(-0.1)
                        }
                    }

					MyTextField {
                        id: roomMoveXText
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
                        id: roomMoveXPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            MoveCenterTabController.modOffsetX(0.1)
                        }
                    }

                    MyToggleButton {
                        id: lockXToggle
                        text: "Lock X"
                        onCheckedChanged: {
                            MoveCenterTabController.lockXToggle = this.checked
                        }
                    }

                    MyText {
                        text: "Y-Axis (Down/Up):"
                        Layout.preferredWidth: 340
                    }

                    MyPushButton2 {
                        Layout.preferredWidth: 40
                        text: "0"
                        onClicked: {
                            MoveCenterTabController.offsetY = 0
                        }
                    }

                    MyPushButton2 {
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            MoveCenterTabController.modOffsetY(-0.1)
                        }
                    }

                    MyTextField {
                        id: roomMoveYText
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
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            MoveCenterTabController.modOffsetY(0.1)
                        }
                    }


                    MyToggleButton {
                        id: lockYToggle
                        text: "Lock Y"
                        onCheckedChanged: {
                            MoveCenterTabController.lockYToggle = this.checked
                        }
                    }

                    MyText {
                        text: "Z-Axis (Forth/Back):"
                        Layout.preferredWidth: 340
                    }

                    MyText {
                        text: " "
                        Layout.preferredWidth: 40
                    }

                    MyPushButton2 {
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            MoveCenterTabController.modOffsetZ(-0.1)
                        }
                    }

                    MyTextField {
                        id: roomMoveZText
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
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            MoveCenterTabController.modOffsetZ(0.1)
                        }
                    }


                    MyToggleButton {
                        id: lockZToggle
                        text: "Lock Z"
                        onCheckedChanged: {
                            MoveCenterTabController.lockZToggle = this.checked
                        }
                    }
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Rotate Room"
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

                ColumnLayout {
                    RowLayout {
                        MyPushButton2 {
                            id: roomRotationMinusButton
                            Layout.preferredWidth: 40
                            text: "-"
                            onClicked: {
                                var val = ( MoveCenterTabController.tempRotation / 100 ) - 45
                                if (val < -180) {
                                    val = val + 360;
                                }
                                MoveCenterTabController.tempRotation = val * 100
                            }
                        }

                        MySlider {
                            id: roomRotationSlider
                            from: -180
                            to: 180
                            stepSize: 0.01
                            value: 0
                            Layout.fillWidth: true
                            onPositionChanged: {
                                var val = this.from + ( this.position  * (this.to - this.from))
                                MoveCenterTabController.tempRotation = Math.round(val * 100)
                            }
                            onValueChanged: {
                                MoveCenterTabController.tempRotation = Math.round(roomRotationSlider.value * 100)
                                //roomRotationText.text = Math.round(roomRotationSlider.value) + "°"
                            }
                        }

                        MyPushButton2 {
                            id: roomRotationPlusButton
                            Layout.preferredWidth: 40
                            text: "+"
                            onClicked: {
                                var val = ( MoveCenterTabController.tempRotation / 100 ) + 45
                                if (val > 180) {
                                    val = val -360;
                                }
                                MoveCenterTabController.tempRotation = val * 100
                            }
                        }

                        MyTextField {
                            id: roomRotationText
                            text: "0°"
                            keyBoardUID: 104
                            Layout.preferredWidth: 150
                            Layout.leftMargin: 10
                            horizontalAlignment: Text.AlignHCenter
                            function onInputEvent(input) {
                                var val = parseFloat(input)
                                if (!isNaN(val)) {
                                    val = val % 180
                                    MoveCenterTabController.tempRotation = Math.round(val * 100)
                                    text = ( MoveCenterTabController.tempRotation / 100 ) + "°"
                                } else {
                                    text = ( MoveCenterTabController.tempRotation / 100 ) + "°"
                                }
                            }
                        }
                        MyPushButton {
                            id: roomRotationApplyButton
                            Layout.preferredWidth: 145
                            text:"Apply"
                            onClicked: {
                                MoveCenterTabController.rotation = MoveCenterTabController.tempRotation
                            }
                       }
                    }

                    MyToggleButton {
                        id: roomRotateHandToggle
                        text: "Enable Room-Turn Binding"
                        onCheckedChanged: {
                            MoveCenterTabController.rotateHand = this.checked
                        }
                    }

                    MyText {
                        text: "Note: TurnSignal will ignore turning during Room-Turn rotation."
                        font.pointSize: 15.0
                    }
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Room Drag Binding"
                bottomPadding: -10
            }
            background: Rectangle {
                color: "transparent"
                border.color: "#ffffff"
                radius: 8
            }
            ColumnLayout {
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
                }
            }
        }

        ColumnLayout {
            RowLayout {
                Layout.fillWidth: true

                MyPushButton {
                    id: roomResetButton
                    Layout.preferredWidth: 250
                    text: "Reset"
                    onClicked: {
                        MoveCenterTabController.reset()
                    }
                }

                MyToggleButton {
                    id: roomAdjustChaperoneToggle
                    text: "Adjust Chaperone"
                    onCheckedChanged: {
                        MoveCenterTabController.adjustChaperone = this.checked
                    }
                }

            }
        }



        Item { Layout.fillHeight: true; Layout.fillWidth: true}

        Component.onCompleted: {
            roomAdjustChaperoneToggle.checked = MoveCenterTabController.adjustChaperone
            roomRotateHandToggle.checked = MoveCenterTabController.rotateHand
            roomMoveXText.text = MoveCenterTabController.offsetX.toFixed(2)
            roomMoveYText.text = MoveCenterTabController.offsetY.toFixed(2)
            roomMoveZText.text = MoveCenterTabController.offsetZ.toFixed(2)
            roomRotationSlider.value = MoveCenterTabController.rotation
            moveShortcutRight.checked = MoveCenterTabController.moveShortcutRight
            moveShortcutLeft.checked = MoveCenterTabController.moveShortcutLeft
			lockXToggle.checked = MoveCenterTabController.lockXToggle
			lockYToggle.checked = MoveCenterTabController.lockYToggle
			lockZToggle.checked = MoveCenterTabController.lockZToggle
			
            if (MoveCenterTabController.trackingUniverse === 0) {
                roomModeText.text = "Sitting"
                roomRotationPlusButton.enabled = false
                roomRotationMinusButton.enabled = false
                roomRotationSlider.enabled = false
                roomRotationText.text = "-"
            } else if (MoveCenterTabController.trackingUniverse === 1) {
                roomModeText.text = "Standing"
                roomRotationPlusButton.enabled = true
                roomRotationMinusButton.enabled = true
                roomRotationSlider.enabled = true
                roomRotationText.text = "0°"
            } else {
                roomModeText.text = "Unknown(" + MoveCenterTabController.trackingUniverse + ")"
            }
        }

        Connections {
            target: MoveCenterTabController
            onOffsetXChanged: {
                roomMoveXText.text = MoveCenterTabController.offsetX.toFixed(2)
            }
            onOffsetYChanged: {
                roomMoveYText.text = MoveCenterTabController.offsetY.toFixed(2)
            }
            onOffsetZChanged: {
                roomMoveZText.text = MoveCenterTabController.offsetZ.toFixed(2)
            }
            onRotationChanged: {
                roomRotationSlider.value = ( MoveCenterTabController.rotation / 100 )
            }
            onTempRotationChanged: {
                roomRotationSlider.value = ( MoveCenterTabController.tempRotation / 100 )
                roomRotationText.text = ( MoveCenterTabController.tempRotation / 100 ) + "°"
            }
            onAdjustChaperoneChanged: {
                roomAdjustChaperoneToggle.checked = MoveCenterTabController.adjustChaperone
            }
            onRotateHandChanged: {
                roomRotateHandToggle.checked = MoveCenterTabController.rotateHand
            }
            onMoveShortcutRightChanged: {
                moveShortcutRight.checked = MoveCenterTabController.moveShortcutRight
            }
            onMoveShortcutLeftChanged: {
                moveShortcutLeft.checked = MoveCenterTabController.moveShortcutLeft
            }
			onLockXToggleChanged: {
				lockXToggle.checked = MoveCenterTabController.lockXToggle
			}
			onLockYToggleChanged: {
				lockYToggle.checked = MoveCenterTabController.lockYToggle
			}
			onLockZToggleChanged: {
				lockZToggle.checked = MoveCenterTabController.lockZToggle
			}
            onTrackingUniverseChanged: {
                if (MoveCenterTabController.trackingUniverse === 0) {
                    roomModeText.text = "Sitting"
                    roomRotationPlusButton.enabled = false
                    roomRotationMinusButton.enabled = false
                    roomRotationSlider.enabled = false
                    roomRotationText.text = "-"
                } else if (MoveCenterTabController.trackingUniverse === 1) {
                    roomModeText.text = "Standing"
                    roomRotationPlusButton.enabled = true
                    roomRotationMinusButton.enabled = true
                    roomRotationSlider.enabled = true
                    roomRotationText.text = "0°"
                } else {
                    roomModeText.text = "Unknown(" + MoveCenterTabController.trackingUniverse + ")"
                }
            }
        }

    }

}
