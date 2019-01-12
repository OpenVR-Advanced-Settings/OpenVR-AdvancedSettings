import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0



MyStackViewPage {
    headerText: "Play Space Settings"

    content: ColumnLayout {
        spacing: 18

        RowLayout {
            MyText {
                text: "Tracking Universe:"
                Layout.preferredWidth: 230
            }
            MyText {
                id: playspaceModeText
                font.bold: true
                text: "Standing"
            }
        }

        GroupBox {
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Move Play Space"
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
                        id: playSpaceMoveXMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            MoveCenterTabController.modOffsetX(-0.1)
                        }
                    }

					MyTextField {
                        id: playSpaceMoveXText
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
                        id: playSpaceMoveXPlusButton
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
                        id: playSpaceMoveYText
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
                        id: playSpaceMoveZText
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
                text: "Rotate Play Space"
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
                            id: playSpaceRotationMinusButton
                            Layout.preferredWidth: 40
                            text: "-"
                            onClicked: {
                                var val = MoveCenterTabController.tempRotation - 45
                                if (val < -180) {
                                    val = val + 360;
                                }
                                MoveCenterTabController.tempRotation = val
                            }
                        }

                        MySlider {
                            id: playspaceRotationSlider
                            from: -180
                            to: 180
                            stepSize: 1
                            value: 0
                            Layout.fillWidth: true
                            onPositionChanged: {
                                var val = this.from + ( this.position  * (this.to - this.from))
                                MoveCenterTabController.tempRotation = Math.round(val)
                            }
                            onValueChanged: {
                                MoveCenterTabController.tempRotation = Math.round(playspaceRotationSlider.value)
                                //playspaceRotationText.text = Math.round(playspaceRotationSlider.value) + "°"
                            }
                        }

                        MyPushButton2 {
                            id: playSpaceRotationPlusButton
                            Layout.preferredWidth: 40
                            text: "+"
                            onClicked: {
                                var val = MoveCenterTabController.tempRotation + 45
                                if (val > 180) {
                                    val = val -360;
                                }
                                MoveCenterTabController.tempRotation = val
                            }
                        }

                        MyTextField {
                            id: playspaceRotationText
                            text: "0°"
                            keyBoardUID: 104
                            Layout.preferredWidth: 100
                            Layout.leftMargin: 10
                            horizontalAlignment: Text.AlignHCenter
                            function onInputEvent(input) {
                                var val = parseInt(input)
                                if (!isNaN(val)) {
                                    val = val % 180
                                    MoveCenterTabController.tempRotation = val
                                    text = MoveCenterTabController.tempRotation + "°"
                                } else {
                                    text = MoveCenterTabController.tempRotation + "°"
                                }
                            }
                        }
                        MyPushButton {
                            id: playSpaceRotationApplyButton
                            Layout.preferredWidth: 145
                            text:"Apply"
                            onClicked: {
                                MoveCenterTabController.rotation = MoveCenterTabController.tempRotation
                            }
                       }
                    }

                    MyToggleButton {
                        id: playspaceRotateHandToggle
                        text: "Virtual Move Shortcut Controls Rotation !ALPHA TEST!"
                        onCheckedChanged: {
                            MoveCenterTabController.rotateHand = this.checked
                        }
                    }

                    MyText {
                        text: "Note: Stats page / TurnSignal will ignore turning during virtual move rotation."
                        font.pointSize: 15.0
                    }
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Virtual Move Shortcut"
                bottomPadding: -10
            }
            background: Rectangle {
                color: "transparent"
                border.color: "#ffffff"
                radius: 8
            }
            ColumnLayout {
                RowLayout {
                    anchors.fill: parent

                    MyToggleButton {
                        id: moveShortcutLeft
                        text: "Left Menu Button"
                        onCheckedChanged: {
                            MoveCenterTabController.moveShortcutLeft = this.checked
                        }
                    }

                    MyToggleButton {
                        id: moveShortcutRight
                        text: "Right Menu Button"
                        onCheckedChanged: {
                            MoveCenterTabController.moveShortcutRight = this.checked
                        }
                    }

                    MyToggleButton {
                        id: requireDoubleClick
                        text: "Require double click"
                        onCheckedChanged: {
                            MoveCenterTabController.requireDoubleClick = this.checked
                        }
                    }
                }
            }
        }

        ColumnLayout {
            RowLayout {
                anchors.fill: parent

                MyPushButton {
                    id: playspaceResetButton
                    Layout.preferredWidth: 250
                    text: "Reset"
                    onClicked: {
                        MoveCenterTabController.reset()
                    }
                }

                MyToggleButton {
                    id: playspaceAdjustChaperoneToggle
                    text: "Adjust Chaperone"
                    onCheckedChanged: {
                        MoveCenterTabController.adjustChaperone = this.checked
                    }
                }

            }
        }



        Item { Layout.fillHeight: true; Layout.fillWidth: true}

        Component.onCompleted: {
            playspaceAdjustChaperoneToggle.checked = MoveCenterTabController.adjustChaperone
            playspaceRotateHandToggle.checked = MoveCenterTabController.rotateHand
            playSpaceMoveXText.text = MoveCenterTabController.offsetX.toFixed(2)
            playSpaceMoveYText.text = MoveCenterTabController.offsetY.toFixed(2)
            playSpaceMoveZText.text = MoveCenterTabController.offsetZ.toFixed(2)
            playspaceRotationSlider.value = MoveCenterTabController.rotation
            moveShortcutRight.checked = MoveCenterTabController.moveShortcutRight
            moveShortcutLeft.checked = MoveCenterTabController.moveShortcutLeft
			lockXToggle.checked = MoveCenterTabController.lockXToggle
			lockYToggle.checked = MoveCenterTabController.lockYToggle
			lockZToggle.checked = MoveCenterTabController.lockZToggle
			requireDoubleClick.checked = MoveCenterTabController.requireDoubleClick
			
            if (MoveCenterTabController.trackingUniverse === 0) {
                playspaceModeText.text = "Sitting"
                playSpaceRotationPlusButton.enabled = false
                playSpaceRotationMinusButton.enabled = false
                playspaceRotationSlider.enabled = false
                playspaceRotationText.text = "-"
            } else if (MoveCenterTabController.trackingUniverse === 1) {
                playspaceModeText.text = "Standing"
                playSpaceRotationPlusButton.enabled = true
                playSpaceRotationMinusButton.enabled = true
                playspaceRotationSlider.enabled = true
                playspaceRotationText.text = "0°"
            } else {
                playspaceModeText.text = "Unknown(" + MoveCenterTabController.trackingUniverse + ")"
            }
        }

        Connections {
            target: MoveCenterTabController
            onOffsetXChanged: {
                playSpaceMoveXText.text = MoveCenterTabController.offsetX.toFixed(2)
            }
            onOffsetYChanged: {
                playSpaceMoveYText.text = MoveCenterTabController.offsetY.toFixed(2)
            }
            onOffsetZChanged: {
                playSpaceMoveZText.text = MoveCenterTabController.offsetZ.toFixed(2)
            }
            onRotationChanged: {
                playspaceRotationSlider.value = MoveCenterTabController.rotation
            }
            onTempRotationChanged: {
                playspaceRotationSlider.value = MoveCenterTabController.tempRotation
                playspaceRotationText.text = MoveCenterTabController.tempRotation + "°"
            }
            onAdjustChaperoneChanged: {
                playspaceAdjustChaperoneToggle.checked = MoveCenterTabController.adjustChaperone
            }
            onRotateHandChanged: {
                playspaceRotateHandToggle.checked = MoveCenterTabController.rotateHand
            }
            onMoveShortcutRightChanged: {
                moveShortcutRight.checked = MoveCenterTabController.moveShortcutRight
            }
            onMoveShortcutLeftChanged: {
                moveShortcutLeft.checked = MoveCenterTabController.moveShortcutLeft
            }
            onRequireDoubleClickChanged: {
                requireDoubleClick.checked = MoveCenterTabController.requireDoubleClick
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
                    playspaceModeText.text = "Sitting"
                    playSpaceRotationPlusButton.enabled = false
                    playSpaceRotationMinusButton.enabled = false
                    playspaceRotationSlider.enabled = false
                    playspaceRotationText.text = "-"
                } else if (MoveCenterTabController.trackingUniverse === 1) {
                    playspaceModeText.text = "Standing"
                    playSpaceRotationPlusButton.enabled = true
                    playSpaceRotationMinusButton.enabled = true
                    playspaceRotationSlider.enabled = true
                    playspaceRotationText.text = "0°"
                } else {
                    playspaceModeText.text = "Unknown(" + MoveCenterTabController.trackingUniverse + ")"
                }
            }
        }

    }

}
