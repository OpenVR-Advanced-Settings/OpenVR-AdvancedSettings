import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "common"

MyStackViewPage {
    headerText: "Revive Settings"

    property bool componentCompleted: false

    content: ColumnLayout {
        spacing: 12


        MyDialogOkPopup {
            id: reviveMessageDialog
            function showMessage(title, text) {
                dialogTitle = title
                dialogText = text
                open()
            }
        }

        MyDialogOkCancelPopup {
            id: reviveDeleteProfileDialog
            property int profileIndex: -1
            dialogTitle: "Delete Profile"
            dialogText: "Do you really want to delete this profile?"
            onClosed: {
                if (okClicked) {
                    ReviveTabController.deleteControllerProfile(profileIndex)
                }
            }
        }

        MyDialogOkCancelPopup {
            id: reviveNewProfileDialog
            dialogTitle: "Create New Profile"
            dialogContentItem: ColumnLayout {
                RowLayout {
                    Layout.topMargin: 16
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    MyText {
                        text: "Name: "
                    }
                    MyTextField {
                        id: reviveNewProfileName
                        keyBoardUID: 290
                        color: "#cccccc"
                        text: ""
                        Layout.fillWidth: true
                        font.pointSize: 20
                        function onInputEvent(input) {
                            reviveNewProfileName.text = input
                        }
                    }
                }
            }
            onClosed: {
                if (okClicked) {
                    if (reviveNewProfileName.text == "") {
                        reviveMessageDialog.showMessage("Create New Profile", "ERROR: Empty profile name.")
                    } else {
                        ReviveTabController.addControllerProfile(reviveNewProfileName.text)
                    }

                }
            }
            function openPopup() {
                reviveNewProfileName.text = ""
                open()
            }
        }


        RowLayout {
            MyToggleButton {
                id: pixelsPerDisplayPixelToggle
                text: "Render Target Override:"
                Layout.preferredWidth: 350
                onCheckedChanged: {
                    ReviveTabController.setPixelsPerDisplayPixelOverrideEnabled(checked, false)
                }
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
                    ReviveTabController.setPixelsPerDisplayPixelOverride(value.toFixed(1), false)
                }
            }

            MyPushButton2 {
                text: "+"
                Layout.preferredWidth: 40
                onClicked: {
                    pixelsPerDisplayPixelSlider.increase()
                }
            }

            MyTextField {
                id: pixelsPerDisplayPixelText
                text: "0.00"
                keyBoardUID: 221
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0.1) {
                            val = 0.1
                        }
                        var v = val.toFixed(1)
                        if (v <= pixelsPerDisplayPixelSlider.to) {
                            pixelsPerDisplayPixelSlider.value = v
                        } else {
                            ReviveTabController.setPixelsPerDisplayPixelOverride(v, false)
                        }
                    }
                    text = ReviveTabController.pixelsPerDisplayPixelOverride.toFixed(1)
                }
            }
        }

        RowLayout {
            MyText {
                text: "Grip Button Mode:"
                Layout.preferredWidth: 250
            }

            MyComboBox {
                id: gripButtonModeComboBox
                Layout.preferredWidth: 250
                model: ["Normal Mode", "Toggle Mode", "Hybrid Mode"]
                onCurrentIndexChanged: {
                    if (componentCompleted) {
                        ReviveTabController.setGripButtonMode(currentIndex, false)
                    }
                }
            }

            MyText {
                Layout.preferredWidth: 150
                Layout.leftMargin: 25
                text: "Toggle Delay: "
            }

            MyTextField {
                id: toggleDelayText
                text: "0.50"
                keyBoardUID: 225
                Layout.preferredWidth: 100
                Layout.leftMargin: 25
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0.0) {
                            val = 0.0
                        }
                        ReviveTabController.setToggleDelay(val.toFixed(2), false)
                    }
                    text = ReviveTabController.toggleDelay.toFixed(2)
                }
            }

            MyToggleButton {
                id: triggerAsGripToggle
                text: "Trigger As Grip"
                Layout.preferredWidth: 250
                Layout.leftMargin: 25
                onCheckedChanged: {
                    ReviveTabController.setTriggerAsGrip(checked, false)
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
                            stepSize: 0.01
                            value: 0.5
                            Layout.fillWidth: true
                            onPositionChanged: {
                                var val = this.from + ( this.position  * (this.to - this.from))
                                thumbDeadzoneText.text = val.toFixed(2)
                            }
                            onValueChanged: {
                                ReviveTabController.setThumbDeadzone(value.toFixed(2), false)
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

                        MyTextField {
                            id: thumbDeadzoneText
                            text: "0.00"
                            keyBoardUID: 222
                            Layout.preferredWidth: 100
                            Layout.leftMargin: 10
                            horizontalAlignment: Text.AlignHCenter
                            function onInputEvent(input) {
                                var val = parseFloat(input)
                                if (!isNaN(val)) {
                                    if (val < 0.0) {
                                        val = 0.0
                                    } else if (val > 1.0) {
                                        val = 1.0
                                    }
                                    var v = val.toFixed(2)
                                    if (v <= thumbDeadzoneSlider.to) {
                                        thumbDeadzoneSlider.value = v
                                    } else {
                                        ReviveTabController.setThumbDeadzone(v, false)
                                    }
                                }
                                text = ReviveTabController.thumbDeadzone.toFixed(2)
                            }
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
                                thumbRangeSlider.value -= 0.1
                            }
                        }

                        MySlider {
                            id: thumbRangeSlider
                            from: 1.0
                            to: 5.0
                            stepSize: 0.01
                            value: 2.0
                            Layout.fillWidth: true
                            onPositionChanged: {
                                var val = this.from + ( this.position  * (this.to - this.from))
                                thumbRangeText.text = val.toFixed(2)
                            }
                            onValueChanged: {
                                ReviveTabController.setThumbRange(value.toFixed(2), false)
                            }
                        }

                        MyPushButton2 {
                            id: thumbRangePlusButton
                            Layout.preferredWidth: 40
                            text: "+"
                            onClicked: {
                                thumbRangeSlider.value += 0.1
                            }
                        }

                        MyTextField {
                            id: thumbRangeText
                            text: "0.00"
                            keyBoardUID: 223
                            Layout.preferredWidth: 100
                            Layout.leftMargin: 10
                            horizontalAlignment: Text.AlignHCenter
                            function onInputEvent(input) {
                                var val = parseFloat(input)
                                if (!isNaN(val)) {
                                    if (val < 0.01) {
                                        val = 0.01
                                    }
                                    var v = val.toFixed(2)
                                    if (v <= thumbRangeSlider.to) {
                                        thumbRangeSlider.value = v
                                    } else {
                                        ReviveTabController.setThumbRange(v, false)
                                    }
                                }
                                text = ReviveTabController.thumbRange.toFixed(2)
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
                        text: "Yaw:"
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                        Layout.rightMargin: 12
                    }

                    MyPushButton2 {
                        id: touchYawMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            var val = ReviveTabController.touchYaw - 1.0
                            if (val < -180.0) {
                                val = -180.0
                            }
                            ReviveTabController.setTouchYaw(val.toFixed(1))
                        }
                    }

                    MyTextField {
                        id: touchYawInputField
                        text: "0.00"
                        keyBoardUID: 202
                        Layout.preferredWidth: 140
                        Layout.leftMargin: 10
                        Layout.rightMargin: 10
                        horizontalAlignment: Text.AlignHCenter
                        function onInputEvent(input) {
                            var val = parseFloat(input)
                            if (!isNaN(val)) {
                                if (val < -180.0) {
                                    val = -180.0
                                } else if (val > 180.0) {
                                    val = 180.0
                                }
                                var v = val.toFixed(1)
                                ReviveTabController.setTouchYaw(v, false)
                                text = v + "°"
                            } else {
                                text = ReviveTabController.touchYaw.toFixed(1) + "°"
                            }
                        }
                    }

                    MyPushButton2 {
                        id: touchYawPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            var val = ReviveTabController.touchYaw + 1.0
                            if (val > 180.0) {
                                val = 180.0
                            }
                            ReviveTabController.setTouchYaw(val.toFixed(1))
                        }
                    }

                    MyText {
                        text: "Pitch:"
                        horizontalAlignment: Text.AlignRight
                        Layout.preferredWidth: 80
                        Layout.leftMargin: 12
                        Layout.rightMargin: 12
                    }

                    MyPushButton2 {
                        id: touchPitchMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            var val = ReviveTabController.touchPitch - 1.0
                            if (val < -180.0) {
                                val = -180.0
                            }
                            ReviveTabController.setTouchPitch(val.toFixed(1))
                        }
                    }

                    MyTextField {
                        id: touchPitchInputField
                        text: "0.00"
                        keyBoardUID: 201
                        Layout.preferredWidth: 140
                        Layout.leftMargin: 10
                        Layout.rightMargin: 10
                        horizontalAlignment: Text.AlignHCenter
                        function onInputEvent(input) {
                            var val = parseFloat(input)
                            if (!isNaN(val)) {
                                if (val < -180.0) {
                                    val = -180.0
                                } else if (val > 180.0) {
                                    val = 180.0
                                }
                                var v = val.toFixed(1)
                                ReviveTabController.setTouchPitch(v, false)
                                text = v + "°"
                            } else {
                                text = ReviveTabController.touchPitch.toFixed(1) + "°"
                            }
                        }
                    }

                    MyPushButton2 {
                        id: touchPitchPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            var val = ReviveTabController.touchPitch + 1.0
                            if (val > 180.0) {
                                val = 180.0
                            }
                            ReviveTabController.setTouchPitch(val.toFixed(1))
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
                            var val = ReviveTabController.touchRoll - 1.0
                            if (val < -180.0) {
                                val = -180.0
                            }
                            ReviveTabController.setTouchRoll(val.toFixed(1))
                        }
                    }

                    MyTextField {
                        id: touchRollInputField
                        text: "0.00"
                        keyBoardUID: 203
                        Layout.preferredWidth: 140
                        Layout.leftMargin: 10
                        Layout.rightMargin: 10
                        horizontalAlignment: Text.AlignHCenter
                        function onInputEvent(input) {
                            var val = parseFloat(input)
                            if (!isNaN(val)) {
                                if (val < -180.0) {
                                    val = -180.0
                                } else if (val > 180.0) {
                                    val = 180.0
                                }
                                var v = val.toFixed(1)
                                ReviveTabController.setTouchRoll(v, false)
                                text = v + "°"
                            } else {
                                text = ReviveTabController.touchRoll.toFixed(1) + "°"
                            }
                        }
                    }

                    MyPushButton2 {
                        id: touchRollPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            var val = ReviveTabController.touchRoll + 1.0
                            if (val > 180.0) {
                                val = 180.0
                            }
                            ReviveTabController.setTouchRoll(val.toFixed(1))
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
                            ReviveTabController.setTouchX((ReviveTabController.touchX - 0.005).toFixed(3))
                        }
                    }

                    MyTextField {
                        id: touchXInputField
                        text: "0.00"
                        keyBoardUID: 204
                        Layout.preferredWidth: 140
                        Layout.leftMargin: 10
                        Layout.rightMargin: 10
                        horizontalAlignment: Text.AlignHCenter
                        function onInputEvent(input) {
                            var val = parseFloat(input)
                            if (!isNaN(val)) {
                                var v = val.toFixed(3)
                                ReviveTabController.setTouchX(v, false)
                                text = v
                            } else {
                                text = ReviveTabController.touchX.toFixed(3)
                            }
                        }
                    }

                    MyPushButton2 {
                        id: touchXPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            ReviveTabController.setTouchX((ReviveTabController.touchX + 0.005).toFixed(3))
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
                            ReviveTabController.setTouchY((ReviveTabController.touchY - 0.005).toFixed(3))
                        }
                    }

                    MyTextField {
                        id: touchYInputField
                        text: "0.00"
                        keyBoardUID: 205
                        Layout.preferredWidth: 140
                        Layout.leftMargin: 10
                        Layout.rightMargin: 10
                        horizontalAlignment: Text.AlignHCenter
                        function onInputEvent(input) {
                            var val = parseFloat(input)
                            if (!isNaN(val)) {
                                var v = val.toFixed(3)
                                ReviveTabController.setTouchY(v, false)
                                text = v
                            } else {
                                text = ReviveTabController.touchY.toFixed(3)
                            }
                        }
                    }

                    MyPushButton2 {
                        id: touchYPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            ReviveTabController.setTouchY((ReviveTabController.touchY + 0.005).toFixed(3))
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
                            ReviveTabController.setTouchZ((ReviveTabController.touchZ - 0.005).toFixed(3))
                        }
                    }

                    MyTextField {
                        id: touchZInputField
                        text: "0.00"
                        keyBoardUID: 206
                        Layout.preferredWidth: 140
                        Layout.leftMargin: 10
                        Layout.rightMargin: 10
                        horizontalAlignment: Text.AlignHCenter
                        function onInputEvent(input) {
                            var val = parseFloat(input)
                            if (!isNaN(val)) {
                                var v = val.toFixed(3)
                                ReviveTabController.setTouchZ(v, false)
                                text = v
                            } else {
                                text = ReviveTabController.touchZ.toFixed(3)
                            }
                        }
                    }

                    MyPushButton2 {
                        id: touchZPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            ReviveTabController.setTouchZ((ReviveTabController.touchZ + 0.005).toFixed(3))
                        }
                    }
                }
            }
        }


        MyDialogOkCancelPopup {
            id: revivePersonalInfoDialog
            property double playerHeight: 0.0
            property double eyeHeight: 0.0
            dialogWidth: 600
            dialogHeight: 480
            dialogTitle: "Personal Information"
            dialogContentItem: GridLayout {
                rowSpacing: 6
                columnSpacing: 18
                columns: 2
                MyText {
                    text: "Username:"
                }
                MyTextField {
                    id: rpidUsername
                    text: "0.00"
                    keyBoardUID: 211
                    Layout.fillWidth: true
                    function onInputEvent(input) {
                        rpidUsername.text = input
                    }
                }
                MyText {
                    text: "Name:"
                }
                MyTextField {
                    id: rpidName
                    text: "0.00"
                    keyBoardUID: 212
                    Layout.fillWidth: true
                    function onInputEvent(input) {
                        rpidName.text = input
                    }
                }
                MyText {
                    text: "Gender:"
                }
                MyComboBox {
                    id: rpidGender
                    Layout.fillWidth: true
                    currentIndex: 2
                    model: ["Unknown", "Male", "Female"]
                }
                MyText {
                    text: "Player Height:"
                }
                RowLayout {
                    Item {
                        Layout.fillWidth: true
                    }
                    MyPushButton2 {
                        id: pridPlayerHeighMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            if (revivePersonalInfoDialog.playerHeight > 0.01) {
                                revivePersonalInfoDialog.playerHeight = (revivePersonalInfoDialog.playerHeight - 0.01).toFixed(2)
                            }
                            pridPlayerHeighInputField.text = revivePersonalInfoDialog.playerHeight.toFixed(2)
                        }
                    }
                    MyTextField {
                        id: pridPlayerHeighInputField
                        text: "0.00"
                        keyBoardUID: 213
                        Layout.preferredWidth: 120
                        Layout.leftMargin: 10
                        Layout.rightMargin: 10
                        horizontalAlignment: Text.AlignHCenter
                        function onInputEvent(input) {
                            var val = parseFloat(input)
                            if (!isNaN(val)) {
                                if (val < 0.01) {
                                    val = 0.01
                                }
                                revivePersonalInfoDialog.playerHeight = val.toFixed(2)
                                text = val.toFixed(2)
                            } else {
                                text = val.toFixed(2)
                            }
                        }
                    }
                    MyPushButton2 {
                        id: pridPlayerHeighPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            revivePersonalInfoDialog.playerHeight = (revivePersonalInfoDialog.playerHeight + 0.01).toFixed(2)
                            pridPlayerHeighInputField.text = revivePersonalInfoDialog.playerHeight.toFixed(2)
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                }
                MyText {
                    text: "Eye Height:"
                }
                RowLayout {
                    Item {
                        Layout.fillWidth: true
                    }
                    MyPushButton2 {
                        id: pridEyeHeighMinusButton
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            if (revivePersonalInfoDialog.eyeHeight > 0.01) {
                                revivePersonalInfoDialog.eyeHeight = (revivePersonalInfoDialog.eyeHeight - 0.01).toFixed(2)
                            }
                            pridEyeHeighInputField.text = revivePersonalInfoDialog.eyeHeight.toFixed(2)
                        }
                    }
                    MyTextField {
                        id: pridEyeHeighInputField
                        text: "0.00"
                        keyBoardUID: 214
                        Layout.preferredWidth: 120
                        Layout.leftMargin: 10
                        Layout.rightMargin: 10
                        horizontalAlignment: Text.AlignHCenter
                        function onInputEvent(input) {
                            var val = parseFloat(input)
                            if (!isNaN(val)) {
                                if (val < 0.01) {
                                    val = 0.01
                                }
                                revivePersonalInfoDialog.eyeHeight = val.toFixed(2)
                                text = val.toFixed(2)
                            } else {
                                text = val.toFixed(2)
                            }
                        }
                    }
                    MyPushButton2 {
                        id: pridEyeHeighPlusButton
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            revivePersonalInfoDialog.eyeHeight = (revivePersonalInfoDialog.eyeHeight + 0.01).toFixed(2)
                            pridEyeHeighInputField.text = revivePersonalInfoDialog.eyeHeight.toFixed(2)
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                RowLayout {
                    Item {
                        Layout.fillWidth: true
                    }
                    MyPushButton {
                        id: reviveCalibrateHeightButton
                        Layout.preferredWidth: 230
                        text: "Calibrate Height"
                        onClicked: {
                            var hmdHeight = ReviveTabController.getCurrentHMDHeight()
                            revivePersonalInfoDialog.eyeHeight = hmdHeight.toFixed(2)
                            pridEyeHeighInputField.text = revivePersonalInfoDialog.eyeHeight
                            revivePersonalInfoDialog.playerHeight = (hmdHeight + 0.103).toFixed(2)
                            pridPlayerHeighInputField.text = revivePersonalInfoDialog.playerHeight
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                }

            }
            function openPopup() {
                rpidUsername.text = ReviveTabController.piUsername
                rpidName.text = ReviveTabController.piName
                rpidGender.currentIndex = ReviveTabController.piGender
                playerHeight = ReviveTabController.piPlayerHeight
                pridPlayerHeighInputField.text = playerHeight.toFixed(2)
                eyeHeight = ReviveTabController.piEyeHeight
                pridEyeHeighInputField.text = eyeHeight.toFixed(2)
                open()
            }
            onClosed: {
                if (okClicked) {
                    ReviveTabController.setPiUsername(rpidUsername.text, false)
                    ReviveTabController.setPiName(rpidName.text, false)
                    ReviveTabController.setPiGender(rpidGender.currentIndex, false)
                    ReviveTabController.setPiPlayerHeight(playerHeight, false)
                    ReviveTabController.setPiEyeHeight(eyeHeight, false)
                }
            }
        }



        ColumnLayout {
            spacing: 8
            Layout.topMargin: 12
            Layout.bottomMargin: 8
            RowLayout {
                spacing: 8

                MyText {
                    Layout.preferredWidth: 347
                    text: "Controller Profile:"
                }

                MyComboBox {
                    id: reviveControllerProfileComboBox
                    Layout.maximumWidth: 557
                    Layout.minimumWidth: 557
                    Layout.preferredWidth: 557
                    Layout.fillWidth: true
                    model: [""]
                    onCurrentIndexChanged: {
                        if (currentIndex > 0) {
                            reviveControllerApplyProfileButton.enabled = true
                            reviveControllerDeleteProfileButton.enabled = true
                        } else {
                            reviveControllerApplyProfileButton.enabled = false
                            reviveControllerDeleteProfileButton.enabled = false
                        }
                    }
                }

                MyPushButton {
                    id: reviveControllerApplyProfileButton
                    enabled: false
                    Layout.preferredWidth: 200
                    text: "Apply"
                    onClicked: {
                        if (reviveControllerProfileComboBox.currentIndex > 0) {
                            ReviveTabController.applyControllerProfile(reviveControllerProfileComboBox.currentIndex - 1)
                            reviveControllerProfileComboBox.currentIndex = 0
                        }
                    }
                }
            }
            RowLayout {
                spacing: 8
                Item {
                    Layout.fillWidth: true
                }
                MyPushButton {
                    id: reviveControllerDeleteProfileButton
                    enabled: false
                    Layout.preferredWidth: 200
                    text: "Delete Profile"
                    onClicked: {
                        if (reviveControllerProfileComboBox.currentIndex > 0) {
                            reviveDeleteProfileDialog.profileIndex = reviveControllerProfileComboBox.currentIndex - 1
                            reviveDeleteProfileDialog.open()
                        }
                    }
                }
                MyPushButton {
                    Layout.preferredWidth: 200
                    text: "New Profile"
                    onClicked: {
                        reviveNewProfileDialog.openPopup()
                    }
                }
            }
        }


        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            MyPushButton {
                id: revivePersonalInfoButton
                Layout.preferredWidth: 300
                text: "Personal Information"
                onClicked: {
                    revivePersonalInfoDialog.openPopup()
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MyPushButton {
                id: reviveResetButton
                Layout.preferredWidth: 200
                text: "Reset"
                onClicked: {
                    ReviveTabController.reset()
                }
            }
        }

        Component.onCompleted: {
            pixelsPerDisplayPixelToggle.checked = ReviveTabController.pixelsPerDisplayPixelOverrideEnabled
            var v = ReviveTabController.pixelsPerDisplayPixelOverride.toFixed(1)
            if (v <= pixelsPerDisplayPixelSlider.to) {
                pixelsPerDisplayPixelSlider.value = v
            }
            pixelsPerDisplayPixelText.text = v

            gripButtonModeComboBox.currentIndex = ReviveTabController.gripButtonMode
            toggleDelayText.text = ReviveTabController.toggleDelay.toFixed(2)

            v = ReviveTabController.thumbDeadzone.toFixed(2)
            if (v <= thumbDeadzoneSlider.to) {
                thumbDeadzoneSlider.value = v
            }
            thumbDeadzoneText.text = v

            v = ReviveTabController.thumbRange.toFixed(2)
            if (v <= thumbRangeSlider.to) {
                thumbRangeSlider.value = v
            }
            thumbRangeText.text = v

            touchPitchInputField.text = ReviveTabController.touchPitch.toFixed(1) + "°"
            touchYawInputField.text = ReviveTabController.touchYaw.toFixed(1) + "°"
            touchRollInputField.text = ReviveTabController.touchRoll.toFixed(1) + "°"
            touchXInputField.text = ReviveTabController.touchX.toFixed(3)
            touchYInputField.text = ReviveTabController.touchY.toFixed(3)
            touchZInputField.text = ReviveTabController.touchZ.toFixed(3)
            reloadControllerProfiles()
            componentCompleted = true
        }

        Connections {
            target: ReviveTabController
            onGripButtonModeChanged : {
                gripButtonModeComboBox.currentIndex = ReviveTabController.gripButtonMode
            }
            onToggleDelayChanged : {
                toggleDelayText.text = ReviveTabController.toggleDelay.toFixed(2)
            }
            onPixelsPerDisplayPixelOverrideEnabledChanged : {
                pixelsPerDisplayPixelToggle.checked = ReviveTabController.pixelsPerDisplayPixelOverrideEnabled
            }
            onPixelsPerDisplayPixelOverrideChanged : {
                var v = ReviveTabController.pixelsPerDisplayPixelOverride.toFixed(1)
                if (v <= pixelsPerDisplayPixelSlider.to) {
                    pixelsPerDisplayPixelSlider.value = v
                }
                pixelsPerDisplayPixelText.text = v
            }
            onThumbDeadzoneChanged : {
                var v = ReviveTabController.thumbDeadzone.toFixed(2)
                if (v <= thumbDeadzoneSlider.to) {
                    thumbDeadzoneSlider.value = v
                }
                thumbDeadzoneText.text = v
            }
            onThumbRangeChanged : {
                var v = ReviveTabController.thumbRange.toFixed(2)
                if (v >= thumbRangeSlider.from && v <= thumbRangeSlider.to) {
                    thumbRangeSlider.value = v
                }
                thumbRangeText.text = v
            }
            onTouchPitchChanged : {
                touchPitchInputField.text = ReviveTabController.touchPitch.toFixed(1) + "°"
            }
            onTouchYawChanged : {
                touchYawInputField.text = ReviveTabController.touchYaw.toFixed(1) + "°"
            }
            onTouchRollChanged : {
                touchRollInputField.text = ReviveTabController.touchRoll.toFixed(1) + "°"
            }
            onTouchXChanged : {
                touchXInputField.text = ReviveTabController.touchX.toFixed(3)
            }
            onTouchYChanged : {
                touchYInputField.text = ReviveTabController.touchY.toFixed(3)
            }
            onTouchZChanged : {
                touchZInputField.text = ReviveTabController.touchZ.toFixed(3)
            }
            onControllerProfilesUpdated: {
                reloadControllerProfiles()
            }
        }
    }

    function reloadControllerProfiles() {
        var profiles = [""]
        var profileCount = ReviveTabController.getControllerProfileCount()
        for (var i = 0; i < profileCount; i++) {
            profiles.push(ReviveTabController.getControllerProfileName(i))
        }
        reviveControllerProfileComboBox.currentIndex = 0
        reviveControllerProfileComboBox.model = profiles
    }
}
