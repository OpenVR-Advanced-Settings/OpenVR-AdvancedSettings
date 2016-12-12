import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0


MyStackViewPage {
    headerText: "Revive Settings"

    content: ColumnLayout {
        spacing: 32

        RowLayout {
            MyToggleButton {
                text: "Render Target Override:"
                Layout.rightMargin: 12
            }

            MyPushButton2 {
                text: "-"
                Layout.preferredWidth: 40
                onClicked: {
                    pixelsPerDisplayPixelSlider.decrease()
                }
            }

            MySlider {
                id: pixelsPerDisplayPixelSlider
                from: 0.1
                to: 4.0
                stepSize: 0.1
                value: 1.0
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = this.from + ( this.position  * (this.to - this.from))
                    pixelsPerDisplayPixelText.text = val.toFixed(1)
                }
                onValueChanged: {
                }
            }

            MyPushButton2 {
                text: "+"
                Layout.preferredWidth: 40
                onClicked: {
                    pixelsPerDisplayPixelSlider.increase()
                }
            }

            MyText {
                id: pixelsPerDisplayPixelText
                text: "1.0"
                horizontalAlignment: Text.AlignRight
                Layout.minimumWidth: 85
            }
        }


        RowLayout {
            MyText {
                text: "Grip Button Mode:"
                Layout.rightMargin: 12
            }

            MyComboBox {
                id: gripButtonModeComboBox
                Layout.maximumWidth: 300
                Layout.minimumWidth: 300
                Layout.preferredWidth: 300
                Layout.fillWidth: true
                model: ["Normal Mode", "Toggle Mode", "Hybrid Mode"]
                onCurrentIndexChanged: {
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Thumbstick Settings"
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
                        MyText {
                            text: "Deadzone:"
                            Layout.rightMargin: 12
                        }

                        MyPushButton2 {
                            id: thumbDeadzoneMinusButton
                            Layout.preferredWidth: 40
                            text: "-"
                            onClicked: {
                                thumbDeadzoneSlider.decrease()
                            }
                        }

                        MySlider {
                            id: thumbDeadzoneSlider
                            from: 0.0
                            to: 1.0
                            stepSize: 0.1
                            value: 0.5
                            Layout.fillWidth: true
                            onPositionChanged: {
                                var val = this.from + ( this.position  * (this.to - this.from))
                                thumbDeadzoneText.text = val.toFixed(1)
                            }
                        }

                        MyPushButton2 {
                            id: thumbDeadzonePlusButton
                            Layout.preferredWidth: 40
                            text: "+"
                            onClicked: {
                                thumbDeadzoneSlider.increase()
                            }
                        }

                        MyText {
                            id: thumbDeadzoneText
                            Layout.preferredWidth: 85
                            horizontalAlignment: Text.AlignRight
                            text: "1.0"
                        }
                    }

                    RowLayout {
                        MyText {
                            text: "Sensitivity:"
                            Layout.rightMargin: 12
                        }

                        MyPushButton2 {
                            id: thumbRangeMinusButton
                            Layout.preferredWidth: 40
                            text: "-"
                            onClicked: {
                                thumbRangeSlider.decrease()
                            }
                        }

                        MySlider {
                            id: thumbRangeSlider
                            from: 0.0
                            to: 1.0
                            stepSize: 0.1
                            value: 0.5
                            Layout.fillWidth: true
                            onPositionChanged: {
                                var val = this.from + ( this.position  * (this.to - this.from))
                                thumbRangeText.text = val.toFixed(1)
                            }
                        }

                        MyPushButton2 {
                            id: thumbRangePlusButton
                            Layout.preferredWidth: 40
                            text: "+"
                            onClicked: {
                                thumbRangeSlider.increase()
                            }
                        }

                        MyText {
                            id: thumbRangeText
                            Layout.preferredWidth: 85
                            horizontalAlignment: Text.AlignRight
                            text: "1.0"
                        }
                    }
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Controller Offsets"
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
                    columns: 12

                    MyText {
                        text: "Pitch:"
                        horizontalAlignment: Text.AlignRight
                        Layout.preferredWidth: 80
                        Layout.rightMargin: 12
                    }

                    MyPushButton2 {
                        id: touchPitchMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                        }
                    }

                    TextField {
                        id: touchPitchInputField
                        horizontalAlignment: Text.AlignHCenter
                        color: "#cccccc"
                        text: "0.0"
                        Layout.preferredWidth: 120
                        font.pointSize: 20
                        onActiveFocusChanged: {
                            if (activeFocus) {
                                OverlayController.showKeyboard(text, 201)
                            }
                        }
                        Connections {
                            target: OverlayController
                            onKeyBoardInputSignal: {
                                if (userValue == 201) {
                                    touchPitchInputField.text = input
                                }
                            }
                        }
                    }

                    MyPushButton2 {
                        id: touchPitchPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                        }
                    }

                    MyText {
                        text: "Yaw:"
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                        Layout.leftMargin: 12
                        Layout.rightMargin: 12
                    }

                    MyPushButton2 {
                        id: touchYawMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                        }
                    }

                    TextField {
                        id: touchYawInputField
                        horizontalAlignment: Text.AlignHCenter
                        color: "#cccccc"
                        text: "0.0"
                        Layout.preferredWidth: 120
                        font.pointSize: 20
                        onActiveFocusChanged: {
                            if (activeFocus) {
                                OverlayController.showKeyboard(text, 202)
                            }
                        }
                        Connections {
                            target: OverlayController
                            onKeyBoardInputSignal: {
                                if (userValue == 202) {
                                    touchYawInputField.text = input
                                }
                            }
                        }
                    }

                    MyPushButton2 {
                        id: touchYawPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                        }
                    }

                    MyText {
                        text: "Roll:"
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                        Layout.leftMargin: 12
                        Layout.rightMargin: 12
                    }

                    MyPushButton2 {
                        id: touchRollMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                        }
                    }

                    TextField {
                        id: touchRollInputField
                        horizontalAlignment: Text.AlignHCenter
                        color: "#cccccc"
                        text: "0.0"
                        Layout.preferredWidth: 120
                        font.pointSize: 20
                        onActiveFocusChanged: {
                            if (activeFocus) {
                                OverlayController.showKeyboard(text, 203)
                            }
                        }
                        Connections {
                            target: OverlayController
                            onKeyBoardInputSignal: {
                                if (userValue == 203) {
                                    touchRollInputField.text = input
                                }
                            }
                        }
                    }

                    MyPushButton2 {
                        id: touchRollPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                        }
                    }

                    MyText {
                        text: "X:"
                        horizontalAlignment: Text.AlignRight
                        Layout.preferredWidth: 80
                        Layout.rightMargin: 12
                    }

                    MyPushButton2 {
                        id: touchXMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                        }
                    }

                    TextField {
                        id: touchXInputField
                        horizontalAlignment: Text.AlignHCenter
                        color: "#cccccc"
                        text: "0.0"
                        Layout.preferredWidth: 120
                        font.pointSize: 20
                        onActiveFocusChanged: {
                            if (activeFocus) {
                                OverlayController.showKeyboard(text, 204)
                            }
                        }
                        Connections {
                            target: OverlayController
                            onKeyBoardInputSignal: {
                                if (userValue == 204) {
                                    touchXInputField.text = input
                                }
                            }
                        }
                    }

                    MyPushButton2 {
                        id: touchXPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                        }
                    }

                    MyText {
                        text: "Y:"
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                        Layout.leftMargin: 12
                        Layout.rightMargin: 12
                    }

                    MyPushButton2 {
                        id: touchYMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                        }
                    }

                    TextField {
                        id: touchYInputField
                        horizontalAlignment: Text.AlignHCenter
                        color: "#cccccc"
                        text: "0.0"
                        Layout.preferredWidth: 120
                        font.pointSize: 20
                        onActiveFocusChanged: {
                            if (activeFocus) {
                                OverlayController.showKeyboard(text, 205)
                            }
                        }
                        Connections {
                            target: OverlayController
                            onKeyBoardInputSignal: {
                                if (userValue == 205) {
                                    touchYInputField.text = input
                                }
                            }
                        }
                    }

                    MyPushButton2 {
                        id: touchYPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                        }
                    }

                    MyText {
                        text: "Z:"
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                        Layout.leftMargin: 12
                        Layout.rightMargin: 12
                    }

                    MyPushButton2 {
                        id: touchZMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                        }
                    }

                    TextField {
                        id: touchZInputField
                        horizontalAlignment: Text.AlignHCenter
                        color: "#cccccc"
                        text: "0.0"
                        Layout.preferredWidth: 120
                        font.pointSize: 20
                        onActiveFocusChanged: {
                            if (activeFocus) {
                                OverlayController.showKeyboard(text, 206)
                            }
                        }
                        Connections {
                            target: OverlayController
                            onKeyBoardInputSignal: {
                                if (userValue == 206) {
                                    touchZInputField.text = input
                                }
                            }
                        }
                    }

                    MyPushButton2 {
                        id: touchZPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                        }
                    }
                }
            }
        }


        MyDialogOkCancelPopup {
            id: revivePersonalInfoDialog
            dialogWidth: 600
            dialogHeight: 400
            dialogTitle: "Personal Information"
            dialogContentItem: GridLayout {
                columns: 2
                MyText {
                    text: "Username:"
                }
                TextField {
                    id: rpidUsername
                    horizontalAlignment: Text.AlignHCenter
                    color: "#cccccc"
                    text: "Username"
                    Layout.preferredWidth: 250
                    font.pointSize: 20
                    onActiveFocusChanged: {
                        if (activeFocus) {
                            OverlayController.showKeyboard(text, 211)
                        }
                    }
                    Connections {
                        target: OverlayController
                        onKeyBoardInputSignal: {
                            if (userValue == 211) {
                                rpidUsername.text = input
                            }
                        }
                    }
                }
                MyText {
                    text: "Name:"
                }

                TextField {
                    id: rpidName
                    horizontalAlignment: Text.AlignHCenter
                    color: "#cccccc"
                    text: "Name"
                    Layout.preferredWidth: 250
                    font.pointSize: 20
                    onActiveFocusChanged: {
                        if (activeFocus) {
                            OverlayController.showKeyboard(text, 212)
                        }
                    }
                    Connections {
                        target: OverlayController
                        onKeyBoardInputSignal: {
                            if (userValue == 212) {
                                rpidName.text = input
                            }
                        }
                    }
                }
                MyText {
                    text: "Gender:"
                }

                MyComboBox {
                    id: rpidGender
                    Layout.maximumWidth: 300
                    Layout.minimumWidth: 300
                    Layout.preferredWidth: 300
                    Layout.fillWidth: true
                    currentIndex: 2
                    model: ["Male", "Female", "Unknown"]
                    onCurrentIndexChanged: {
                    }
                }
                MyText {
                    text: "Player Height:"
                }
                RowLayout {
                    MyPushButton2 {
                        id: pridPlayerHeighMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                        }
                    }

                    TextField {
                        id: pridPlayerHeighInputField
                        horizontalAlignment: Text.AlignHCenter
                        color: "#cccccc"
                        text: "0.0"
                        Layout.preferredWidth: 120
                        font.pointSize: 20
                        onActiveFocusChanged: {
                            if (activeFocus) {
                                OverlayController.showKeyboard(text, 213)
                            }
                        }
                        Connections {
                            target: OverlayController
                            onKeyBoardInputSignal: {
                                if (userValue == 213) {
                                    pridPlayerHeighInputField.text = input
                                }
                            }
                        }
                    }

                    MyPushButton2 {
                        id: pridPlayerHeighPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                        }
                    }
                }
                MyText {
                    text: "Eye Height:"
                }
                RowLayout {
                    MyPushButton2 {
                        id: pridEyeHeighMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                        }
                    }

                    TextField {
                        id: pridEyeHeighInputField
                        horizontalAlignment: Text.AlignHCenter
                        color: "#cccccc"
                        text: "0.0"
                        Layout.preferredWidth: 120
                        font.pointSize: 20
                        onActiveFocusChanged: {
                            if (activeFocus) {
                                OverlayController.showKeyboard(text, 214)
                            }
                        }
                        Connections {
                            target: OverlayController
                            onKeyBoardInputSignal: {
                                if (userValue == 214) {
                                    pridEyeHeighInputField.text = input
                                }
                            }
                        }
                    }

                    MyPushButton2 {
                        id: pridEyeHeighPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                        }
                    }
                }
            }
            function openPopup() {
                open()
            }
            onClosed: {
            }
        }

        MyPushButton {
            id: revivePersonalInfoButton
            Layout.preferredWidth: 300
            text: "Personal Information"
            onClicked: {
                revivePersonalInfoDialog.openPopup()
            }
        }

        Item {
            Layout.fillHeight: true
        }

        /*Component.onCompleted: {
        }*/

        /*Connections {
        }*/
    }
}
