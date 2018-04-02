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
                    columns: 4

                    MyText {
                        text: "X-Axis (Left/Right):"
                        Layout.preferredWidth: 500
                    }

                    MyPushButton2 {
                        id: playSpaceMoveXMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            MoveCenterTabController.modOffsetX(-0.5)
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
                            MoveCenterTabController.modOffsetX(0.5)
                        }
                    }

                    MyText {
                        text: "Y-Axis (Down/Up):"
                        Layout.preferredWidth: 380
                    }

                    MyPushButton2 {
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            MoveCenterTabController.modOffsetY(-0.5)
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
                            MoveCenterTabController.modOffsetY(0.5)
                        }
                    }

                    MyText {
                        text: "Z-Axis (Forth/Back):"
                        Layout.preferredWidth: 380
                    }

                    MyPushButton2 {
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            MoveCenterTabController.modOffsetZ(-0.5)
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
                            MoveCenterTabController.modOffsetZ(0.5)
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
                                var val = MoveCenterTabController.rotation - 45
                                if (val < -180) {
                                    val = val + 360;
                                }
                                MoveCenterTabController.rotation = val
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
                                playspaceRotationText.text = Math.round(val) + "°"
                            }
                            onValueChanged: {
                                MoveCenterTabController.rotation = playspaceRotationSlider.value
                                playspaceRotationText.text = Math.round(playspaceRotationSlider.value) + "°"
                            }
                        }

                        MyPushButton2 {
                            id: playSpaceRotationPlusButton
                            Layout.preferredWidth: 40
                            text: "+"
                            onClicked: {
                                var val = MoveCenterTabController.rotation + 45
                                if (val > 180) {
                                    val = val -360;
                                }
                                MoveCenterTabController.rotation = val
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
                                    MoveCenterTabController.rotation = val
                                    text = MoveCenterTabController.rotation + "°"
                                } else {
                                    text = MoveCenterTabController.rotation + "°"
                                }
                            }
                        }
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
            }
        }

        MyToggleButton {
            id: playspaceAdjustChaperoneToggle
            text: "Adjust Chaperone"
            onCheckedChanged: {
                MoveCenterTabController.adjustChaperone = this.checked
            }
        }

        Item { Layout.fillHeight: true; Layout.fillWidth: true}

        MyPushButton {
            id: playspaceResetButton
            Layout.preferredWidth: 250
            text: "Reset"
            onClicked: {
                MoveCenterTabController.reset()
            }
        }

        Component.onCompleted: {
            playspaceAdjustChaperoneToggle.checked = MoveCenterTabController.adjustChaperone
            playSpaceMoveXText.text = MoveCenterTabController.offsetX.toFixed(2)
            playSpaceMoveYText.text = MoveCenterTabController.offsetY.toFixed(2)
            playSpaceMoveZText.text = MoveCenterTabController.offsetZ.toFixed(2)
            playspaceRotationSlider.value = MoveCenterTabController.rotation
            moveShortcutRight.checked = MoveCenterTabController.moveShortcutRight;
            moveShortcutLeft.checked = MoveCenterTabController.moveShortcutLeft;
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
            onAdjustChaperoneChanged: {
                playspaceAdjustChaperoneToggle.checked = MoveCenterTabController.adjustChaperone
            }
            onMoveShortcutRightChanged: {
                moveShortcutRight.checked = MoveCenterTabController.moveShortcutRight
            }
            onMoveShortcutLeftChanged: {
                moveShortcutLeft.checked = MoveCenterTabController.moveShortcutLeft
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
