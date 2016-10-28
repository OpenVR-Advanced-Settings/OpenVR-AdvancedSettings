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
                        KeyNavigation.down: playSpaceMoveXPlusButton
                        focus : true
                    }

                    MyText {
                        id: playSpaceMoveXText
                        horizontalAlignment: Text.AlignHCenter
                        text: "0.0"
                        Layout.preferredWidth: 120
                    }

                    MyPushButton2 {
                        id: playSpaceMoveXPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            MoveCenterTabController.modOffsetX(0.5)
                        }
                        KeyNavigation.up: playSpaceMoveXMinusButton
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

                    MyText {
                        id: playSpaceMoveYText
                        horizontalAlignment: Text.AlignHCenter
                        text: "0.0"
                        Layout.preferredWidth: 120
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

                    MyText {
                        id: playSpaceMoveZText
                        horizontalAlignment: Text.AlignHCenter
                        text: "0.0"
                        Layout.preferredWidth: 120
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
                                playspaceRotationSlider.value -= 45
                            }
                        }

                        MySlider {
                            id: playspaceRotationSlider
                            from: -360
                            to: 360
                            stepSize: 1
                            value: 0
                            Layout.fillWidth: true
                            onPositionChanged: {
                                var val = this.from + ( this.position  * (this.to - this.from))
                                playspaceRotationText.text = Math.round(val) + "°"
                            }
                        }

                        MyPushButton2 {
                            id: playSpaceRotationPlusButton
                            Layout.preferredWidth: 40
                            text: "+"
                            onClicked: {
                                playspaceRotationSlider.value += 45
                            }
                        }

                        MyText {
                            id: playspaceRotationText
                            Layout.preferredWidth: 85
                            horizontalAlignment: Text.AlignRight
                            text: "-360°"
                        }
                    }
                    RowLayout {
                        Item {
                            Layout.fillWidth: true
                        }
                        MyPushButton {
                            id: playspaceApplyButton
                            text: "Apply"
                            Layout.preferredWidth: 250
                            onClicked: {
                                MoveCenterTabController.rotation = playspaceRotationSlider.value
                            }
                        }
                    }
                }
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
            playSpaceMoveXText.text = MoveCenterTabController.offsetX.toFixed(1)
            playSpaceMoveYText.text = MoveCenterTabController.offsetY.toFixed(1)
            playSpaceMoveZText.text = MoveCenterTabController.offsetZ.toFixed(1)
            playspaceRotationSlider.value = MoveCenterTabController.rotation
            if (MoveCenterTabController.trackingUniverse === 0) {
                playspaceModeText.text = "Sitting"
                playspaceApplyButton.enabled = false
                playSpaceRotationPlusButton.enabled = false
                playSpaceRotationMinusButton.enabled = false
                playspaceRotationSlider.enabled = false
                playspaceRotationText.text = "-"
            } else if (MoveCenterTabController.trackingUniverse === 1) {
                playspaceModeText.text = "Standing"
                playspaceApplyButton.enabled = true
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
                playSpaceMoveXText.text = MoveCenterTabController.offsetX.toFixed(1)
            }
            onOffsetYChanged: {
                playSpaceMoveYText.text = MoveCenterTabController.offsetY.toFixed(1)
            }
            onOffsetZChanged: {
                playSpaceMoveZText.text = MoveCenterTabController.offsetZ.toFixed(1)
            }
            onRotationChanged: {
                playspaceRotationSlider.value = MoveCenterTabController.rotation
            }
            onTrackingUniverseChanged: {
                if (MoveCenterTabController.trackingUniverse === 0) {
                    playspaceModeText.text = "Sitting"
                    playspaceApplyButton.enabled = false
                    playSpaceRotationPlusButton.enabled = false
                    playSpaceRotationMinusButton.enabled = false
                    playspaceRotationSlider.enabled = false
                    playspaceRotationText.text = "-"
                } else if (MoveCenterTabController.trackingUniverse === 1) {
                    playspaceModeText.text = "Standing"
                    playspaceApplyButton.enabled = true
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
