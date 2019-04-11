import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "common"


MyStackViewPage {
    headerText: "Space Offsets"

    content: ColumnLayout {
        spacing: 18

        RowLayout {
            MyText {
                text: "Tracking Universe:"
                Layout.preferredWidth: 230
            }
            MyText {
                id: spaceModeText
                font.bold: true
                text: "Standing"
            }
        }

        GroupBox {
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Move Space"
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
                        id: spaceMoveXMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            MoveCenterTabController.modOffsetX(-0.1)
                        }
                    }

					MyTextField {
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
                        id: spaceMoveXPlusButton
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
                text: "Rotate Space"
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
                            id: spaceRotationMinusButton
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
                            id: spaceRotationSlider
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
                                MoveCenterTabController.tempRotation = Math.round(spaceRotationSlider.value * 100)
                                //spaceRotationText.text = Math.round(spaceRotationSlider.value) + "°"
                            }
                        }

                        MyPushButton2 {
                            id: spaceRotationPlusButton
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
                            id: spaceRotationText
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
                            id: spaceRotationApplyButton
                            Layout.preferredWidth: 145
                            text:"Apply"
                            onClicked: {
                                MoveCenterTabController.rotation = MoveCenterTabController.tempRotation
                            }
                       }
                    }
                }
            }
        }


        ColumnLayout {
            RowLayout {
                Layout.fillWidth: true

                MyPushButton {
                    id: spaceResetButton
                    Layout.preferredWidth: 250
                    text: "Reset"
                    onClicked: {
                        MoveCenterTabController.reset()
                    }
                }

                MyToggleButton {
                    id: spaceAdjustChaperoneToggle
                    text: "Adjust Chaperone"
                    onCheckedChanged: {
                        MoveCenterTabController.adjustChaperone = this.checked
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                MyPushButton {
                    id: spaceLogMatrices
                    Layout.preferredWidth: 250
                    visible: MoveCenterTabController.showLogMatricesButton
                    text: "Log Matrices"
                    onClicked: {
                        MoveCenterTabController.outputLogPoses()
                    }
                }

                MyPushButton {
                    id: spaceSeatedRecenter
                    Layout.preferredWidth: 250
                    visible: false
                    text: "Seated Recenter"
                    onClicked: {
                        MoveCenterTabController.updateSeatedResetData()
                    }
                }

            }
        }



        Item { Layout.fillHeight: true; Layout.fillWidth: true}

        Component.onCompleted: {
            spaceAdjustChaperoneToggle.checked = MoveCenterTabController.adjustChaperone
            spaceMoveXText.text = MoveCenterTabController.offsetX.toFixed(2)
            spaceMoveYText.text = MoveCenterTabController.offsetY.toFixed(2)
            spaceMoveZText.text = MoveCenterTabController.offsetZ.toFixed(2)
            spaceRotationSlider.value = MoveCenterTabController.rotation
			lockXToggle.checked = MoveCenterTabController.lockXToggle
			lockYToggle.checked = MoveCenterTabController.lockYToggle
			lockZToggle.checked = MoveCenterTabController.lockZToggle
            spaceLogMatrices.visible = MoveCenterTabController.showLogMatricesButton
			
            if (MoveCenterTabController.trackingUniverse === 0) {
                spaceModeText.text = "Sitting"
                spaceSeatedRecenter.visible = true
            } else if (MoveCenterTabController.trackingUniverse === 1) {
                spaceModeText.text = "Standing"
                spaceSeatedRecenter.visible = false
            } else {
                spaceModeText.text = "Unknown(" + MoveCenterTabController.trackingUniverse + ")"
            }
        }

        Connections {
            target: MoveCenterTabController
            onOffsetXChanged: {
                spaceMoveXText.text = MoveCenterTabController.offsetX.toFixed(2)
            }
            onOffsetYChanged: {
                spaceMoveYText.text = MoveCenterTabController.offsetY.toFixed(2)
            }
            onOffsetZChanged: {
                spaceMoveZText.text = MoveCenterTabController.offsetZ.toFixed(2)
            }
            onRotationChanged: {
                spaceRotationSlider.value = ( MoveCenterTabController.rotation / 100 )
            }
            onTempRotationChanged: {
                spaceRotationSlider.value = ( MoveCenterTabController.tempRotation / 100 )
                spaceRotationText.text = ( MoveCenterTabController.tempRotation / 100 ) + "°"
            }
            onAdjustChaperoneChanged: {
                spaceAdjustChaperoneToggle.checked = MoveCenterTabController.adjustChaperone
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
                    spaceModeText.text = "Sitting"
                    spaceSeatedRecenter.visible = true
                } else if (MoveCenterTabController.trackingUniverse === 1) {
                    spaceModeText.text = "Standing"
                    spaceSeatedRecenter.visible = false
                } else {
                    spaceModeText.text = "Unknown(" + MoveCenterTabController.trackingUniverse + ")"
                }
            }
        }

    }

}
