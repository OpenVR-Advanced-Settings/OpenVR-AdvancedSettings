import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0



MyStackViewPage {
    headerText: "Play Space Settings"

    MyDialogOkCancelPopup {
        id: pttControllerConfigDialog
        property int controllerIndex: -1
        dialogTitle: "Controller Configuration"
        dialogWidth: 600
        dialogHeight: 620
        dialogContentItem: ColumnLayout {
            MyText {
                text: "Buttons"
            }
            Rectangle {
                color: "#cccccc"
                height: 1
                Layout.fillWidth: true
            }
            MyToggleButton {
                id: pttControllerConfigGripButtonToggle
                text: "Grip"
            }
            MyToggleButton {
                id: pttControllerConfigMenuButtonToggle
                text: "Menu"
            }
            MyToggleButton {
                id: pttControllerConfigTriggerButtonToggle
                text: "Trigger"
            }

            MyText {
                Layout.topMargin: 32
                text: "Touchpad"
            }
            Rectangle {
                color: "#cccccc"
                height: 1
                Layout.fillWidth: true
            }
            RowLayout {
                spacing: 64
                MyToggleButton {
                    id: pttControllerConfigPadLeftToggle
                    text: "Left"
                }
                MyToggleButton {
                    id: pttControllerConfigPadRightToggle
                    text: "Right"
                }
            }
            RowLayout {
                spacing: 64
                MyToggleButton {
                    id: pttControllerConfigPadTopToggle
                    text: "Top"
                }
                MyToggleButton {
                    id: pttControllerConfigPadBottomToggle
                    text: "Bottom"
                }
            }
            RowLayout {
                MyText {
                    text: "Mode:"
                }
                MyComboBox {
                    id: pttControllerConfigPadModeCombo
                    Layout.preferredWidth: 250
                    model: ["Disabled", "Touched", "Pressed"]
                }
            }
        }
        onClosed: {
            if (okClicked) {
                var buttons = []
                if (pttControllerConfigGripButtonToggle.checked) {
                    buttons.push(2)
                }
                if (pttControllerConfigMenuButtonToggle.checked) {
                    buttons.push(1)
                }
                var triggerMode = 0
                if (pttControllerConfigTriggerButtonToggle.checked) {
                    triggerMode = 1
                }
                var padAreas = 0
                if (pttControllerConfigPadLeftToggle.checked) {
                    padAreas |= (1 << 0)
                }
                if (pttControllerConfigPadTopToggle.checked) {
                    padAreas |= (1 << 1)
                }
                if (pttControllerConfigPadRightToggle.checked) {
                    padAreas |= (1 << 2)
                }
                if (pttControllerConfigPadBottomToggle.checked) {
                    padAreas |= (1 << 3)
                }
                MoveCenterTabController.setPttControllerConfig(controllerIndex, buttons, triggerMode, pttControllerConfigPadModeCombo.currentIndex, padAreas);
            }
        }
        function openPopup(controller) {
            if (controller === 0) {
                dialogTitle = "Left Controller Configuration"
                controllerIndex = 0
            } else {
                dialogTitle = "Right Controller Configuration"
                controllerIndex = 1
            }
            pttControllerConfigGripButtonToggle.checked = false
            pttControllerConfigMenuButtonToggle.checked = false
            pttControllerConfigTriggerButtonToggle.checked = false
            pttControllerConfigPadLeftToggle.checked = false
            pttControllerConfigPadTopToggle.checked = false
            pttControllerConfigPadRightToggle.checked = false
            pttControllerConfigPadBottomToggle.checked = false
            pttControllerConfigPadModeCombo.currentIndex = MoveCenterTabController.pttTouchpadMode(controllerIndex)
            var buttons = MoveCenterTabController.pttDigitalButtons(controllerIndex)
            for (var i = 0; i < buttons.length; i++) {
                if (buttons[i] == 1) {
                    pttControllerConfigMenuButtonToggle.checked = true
                } else if (buttons[i] == 2) {
                    pttControllerConfigGripButtonToggle.checked = true
                }
            }
            if (MoveCenterTabController.pttTriggerMode(controllerIndex) > 0) {
                pttControllerConfigTriggerButtonToggle.checked = true
            }
            var padAreas = MoveCenterTabController.pttTouchpadArea(controllerIndex)
            if (padAreas & (1 << 0)) {
                pttControllerConfigPadLeftToggle.checked = true
            }
            if (padAreas & (1 << 1)) {
                pttControllerConfigPadTopToggle.checked = true
            }
            if (padAreas & (1 << 2)) {
                pttControllerConfigPadRightToggle.checked = true
            }
            if (padAreas & (1 << 3)) {
                pttControllerConfigPadBottomToggle.checked = true
            }
            open()
        }
    }

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
                text: "Push-to-Toggle Y-Axis Offset"
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
                        MyToggleButton {
                            id: offsetYPttEnabledToggle
                            Layout.preferredWidth: 260
                            text: "Enabled"
                            onClicked: {
                                MoveCenterTabController.pttEnabled = checked
                            }
                        }
                        MyToggleButton {
                            id: offsetYPttLeftControllerToggle
                            text: "Left Controller"
                            onClicked: {
                                MoveCenterTabController.setPttLeftControllerEnabled(checked, false)
                            }
                        }
                        MyPushButton {
                            text: "Configure"
                            onClicked: {
                                pttControllerConfigDialog.openPopup(0)
                            }
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        MyToggleButton {
                            id: offsetYPttRightControllerToggle
                            text: "Right Controller"
                            onClicked: {
                                MoveCenterTabController.setPttRightControllerEnabled(checked, false)
                            }
                        }
                        MyPushButton {
                            text: "Configure"
                            onClicked: {
                                pttControllerConfigDialog.openPopup(1)
                            }
                        }
                    }
                }
            }
        }

        Item { Layout.fillHeight: true; Layout.fillWidth: true}

        ColumnLayout {
            Layout.fillWidth: true

            RowLayout {
                MyToggleButton {
                    id: playspaceAdjustChaperoneToggle
                    text: "Adjust Chaperone"
                    Layout.preferredWidth: 250
                    onCheckedChanged: {
                        MoveCenterTabController.adjustChaperone = this.checked
                    }
                }

                Item { Layout.fillWidth: true}

                MyPushButton {
                    id: playspaceResetButton
                    Layout.preferredWidth: 250
                    text: "Reset"
                    onClicked: {
                        MoveCenterTabController.reset()
                    }
                }
            }
        }

        Component.onCompleted: {
            playspaceAdjustChaperoneToggle.checked = MoveCenterTabController.adjustChaperone
            playSpaceMoveXText.text = MoveCenterTabController.offsetX.toFixed(2)
            playSpaceMoveYText.text = MoveCenterTabController.offsetY.toFixed(2)
            playSpaceMoveZText.text = MoveCenterTabController.offsetZ.toFixed(2)
            playspaceRotationSlider.value = MoveCenterTabController.rotation
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
            offsetYPttEnabledToggle.checked = MoveCenterTabController.pttEnabled
            offsetYPttLeftControllerToggle.checked = MoveCenterTabController.pttLeftControllerEnabled
            offsetYPttRightControllerToggle.checked = MoveCenterTabController.pttRightControllerEnabled
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
                playspaceAdjustChaperoneToggle = value
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
            onPttEnabledChanged: {
                offsetYPttEnabledToggle.checked = MoveCenterTabController.pttEnabled
            }
            onPttActiveChanged: {

            }
            onPttLeftControllerEnabledChanged: {
                offsetYPttLeftControllerToggle.checked = MoveCenterTabController.pttLeftControllerEnabled
            }
            onPttRightControllerEnabledChanged: {
                offsetYPttRightControllerToggle.checked = MoveCenterTabController.pttRightControllerEnabled
            }
        }

    }

}
