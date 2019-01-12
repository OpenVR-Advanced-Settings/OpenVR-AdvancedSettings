import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0


MyStackViewPage {
    headerText: "Utilities"

    content: ColumnLayout {
        spacing: 18


        GroupBox {
            id: keyboardGroupBox
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Keyboard Utilities"
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


                MyText {
                    text: "Send to active application: "
                }

                RowLayout {
                    spacing: 18

                    MyPushButton {
                        text: "Keyboard Input"
                        Layout.preferredWidth: 200
                        onClicked: {
                            OverlayController.showKeyboard("", 601)
                        }
                        Connections {
                            target: OverlayController
                            onKeyBoardInputSignal: {
                                if (userValue == 601) {
                                    UtilitiesTabController.sendKeyboardInput(input)
                                }
                            }
                        }
                    }

                    MyPushButton {
                        text: "Enter"
                        Layout.preferredWidth: 200
                        onClicked: {
                            UtilitiesTabController.sendKeyboardEnter()
                        }
                    }

                    MyPushButton {
                        text: "Alt-Tab"
                        Layout.preferredWidth: 200
                        onClicked: {
                            UtilitiesTabController.sendKeyboardAltTab()
                        }
                    }

                    MyPushButton {
                        text: "Backspace"
                        Layout.preferredWidth: 200
                        onClicked: {
                            UtilitiesTabController.sendKeyboardBackspace(1)
                        }
                    }

                    MyPushButton {
                        text: "10x Backspace"
                        Layout.preferredWidth: 200
                        onClicked: {
                            UtilitiesTabController.sendKeyboardBackspace(10)
                        }
                    }
                }
            }
        }


        GroupBox {
            id: alarmGroupBox
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Alarm Clock"
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
                    MyToggleButton {
                        id: alarmClockToggle
                        text: ""
                        onCheckedChanged: {
                            UtilitiesTabController.setAlarmEnabled(checked, false)
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    MyTextField {
                        id: alarmClockHourText
                        text: "00"
                        keyBoardUID: 602
                        Layout.preferredWidth: 50
                        horizontalAlignment: Text.AlignHCenter
                        function onInputEvent(input) {
                            var val = parseInt(input)
                            if (!isNaN(val)) {
                                if (val < 0) {
                                    val = 0
                                } else if (val > 23) {
                                    val = 23
                                }
                                UtilitiesTabController.setAlarmTimeHour(val, false)
                            }
                            setAlarmHourText(UtilitiesTabController.alarmTimeHour)
                        }
                    }

                    MyText {
                        text: ":"
                    }

                    MyTextField {
                        id: alarmClockMinuteText
                        text: "00"
                        keyBoardUID: 603
                        Layout.preferredWidth: 50
                        horizontalAlignment: Text.AlignHCenter
                        function onInputEvent(input) {
                            var val = parseInt(input)
                            if (!isNaN(val)) {
                                if (val < 0) {
                                    val = 0
                                } else if (val > 59) {
                                    val = 59
                                }
                                UtilitiesTabController.setAlarmTimeMinute(val, false)
                            }
                            setAlarmMinuteText(UtilitiesTabController.alarmTimeMinute)
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    MyPushButton2 {
                        Layout.preferredWidth: 200
                        text: "Current Time"
                        onClicked: {
                            UtilitiesTabController.setAlarmTimeToCurrentTime()
                        }
                    }

                    MyPushButton2 {
                        Layout.leftMargin: 50
                        Layout.preferredWidth: 120
                        text: "+1 Hour"
                        onClicked: {
                            UtilitiesTabController.modAlarmTimeHour(1)
                        }
                    }

                    MyPushButton2 {
                        Layout.preferredWidth: 120
                        text: "-1 Hour"
                        onClicked: {
                            UtilitiesTabController.modAlarmTimeHour(-1)
                        }
                    }

                    MyPushButton2 {
                        text: "+5 Min"
                        Layout.leftMargin: 50
                        Layout.preferredWidth: 120
                        onClicked: {
                            UtilitiesTabController.modAlarmTimeMinute(5)
                        }
                    }

                    MyPushButton2 {
                        Layout.preferredWidth: 120
                        text: "-5 Min"
                        onClicked: {
                            UtilitiesTabController.modAlarmTimeMinute(-5)
                        }
                    }
                }

                RowLayout {
                    MyToggleButton {
                        id: alarmDontAnnoyToggle
                        text: "Don't Annoy me"
                        onCheckedChanged: {
                            UtilitiesTabController.setAlarmIsModal(!checked, false)
                        }
                    }
                }
            }
        }

        GroupBox {
            id: steamDesktopGroupBox
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Steam Desktop Overlay Width"
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
                            id: steamDesktopWidthMinusButton
                            Layout.preferredWidth: 40
                            text: "-"
                            onClicked: {
                                steamDesktopWidthSlider.value -= 1.0
                            }
                        }

                        MySlider {
                            id: steamDesktopWidthSlider
                            from: 0.1
                            to: 32.0
                            stepSize: 0.1
                            value: 1.0
                            Layout.fillWidth: true
                            onPositionChanged: {
                                var val = this.from + ( this.position  * (this.to - this.from))
                                steamDesktopWidthText.text = val.toFixed(1)
                            }
                            onValueChanged: {
                                UtilitiesTabController.setSteamDesktopOverlayWidth(value, false)
                                steamDesktopWidthText.text = value.toFixed(1)
                            }
                        }

                        MyPushButton2 {
                            id: steamDesktopWidthPlusButton
                            Layout.preferredWidth: 40
                            text: "+"
                            onClicked: {
                                steamDesktopWidthSlider.value += 1.0
                            }
                        }

                        MyTextField {
                            id: steamDesktopWidthText
                            text: "0.0"
                            keyBoardUID: 611
                            Layout.preferredWidth: 100
                            Layout.leftMargin: 10
                            horizontalAlignment: Text.AlignHCenter
                            function onInputEvent(input) {
                                var val = parseFloat(input)
                                if (!isNaN(val)) {
                                    if (val < 0.1) {
                                        val = 0.1
                                    }
                                    steamDesktopWidthSlider.value = val
                                    UtilitiesTabController.setSteamDesktopOverlayWidth(val, false)
                                }
                                text = UtilitiesTabController.steamDesktopOverlayWidth.toFixed(1)
                            }
                        }
                    }
                }
            }
        }

        GroupBox {
            id: mediaKeysGroupBox
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Media Control Keys"
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
                    property bool playButtonIsPlay: false
                    MyPushButton2 {
                        id: previousSongButton
                        Layout.preferredHeight: 48
                        Layout.preferredWidth: 48
                        contentItem: Image {
                            source: "media_keys/outline_skip_previous_white_24dp.svg"
                            sourceSize: Qt.size( imgSP.sourceSize.width*2, imgSP.sourceSize.height*2 )
                            Image{
                                id: imgSP
                                source: parent.source
                                width:0
                                height:0
                            }
                            anchors.fill: parent
                        }
                        onClicked: {
                            UtilitiesTabController.sendMediaPreviousSong()
                        }

                        background: Rectangle {
                            opacity: parent.down ? 1.0 : (parent.activeFocus ? 0.5 : 0.0)
                            color: "#406288"
                            radius: 4
                            anchors.fill: parent
                        }
                    }
                    MyPushButton2 {
                        id: stopSongButton
                        Layout.preferredHeight: 48
                        Layout.preferredWidth: 48

                        contentItem: Image {
                            source: "media_keys/outline_stop_white_24dp.svg"
                            sourceSize: Qt.size( imgS.sourceSize.width*2, imgS.sourceSize.height*2 )
                            Image{
                                id: imgS
                                source: parent.source
                                width:0
                                height:0
                            }
                            anchors.fill: parent
                        }
                        onClicked: {
                            UtilitiesTabController.sendMediaStopSong()
                            playPauseSongButton.contentItem.source = "media_keys/outline_play_arrow_white_24dp.svg"
                            parent.playButtonIsPlay = true
                        }

                        background: Rectangle {
                            opacity: parent.down ? 1.0 : (parent.activeFocus ? 0.5 : 0.0)
                            color: "#406288"
                            radius: 4
                            anchors.fill: parent
                        }
                    }
                    MyPushButton2 {
                        id: playPauseSongButton
                        Layout.preferredHeight: 48
                        Layout.preferredWidth: 48
                        contentItem: Image {
                            // Assume that music is playing when people enter VR.
                            source: "media_keys/outline_pause_white_24dp.svg"
                            sourceSize: Qt.size( imgP.sourceSize.width*2, imgP.sourceSize.height*2 )
                            Image{
                                id: imgP
                                source: parent.source
                                width:0
                                height:0
                            }

                            anchors.fill: parent
                        }
                        onClicked: {
                            UtilitiesTabController.sendMediaPausePlay()
                            if (parent.playButtonIsPlay) {
                                playPauseSongButton.contentItem.source = "media_keys/outline_pause_white_24dp.svg"
                                parent.playButtonIsPlay = false
                            } else {
                                playPauseSongButton.contentItem.source = "media_keys/outline_play_arrow_white_24dp.svg"
                                parent.playButtonIsPlay = true
                            }

                        }

                        background: Rectangle {
                            opacity: parent.down ? 1.0 : (parent.activeFocus ? 0.5 : 0.0)
                            color: "#406288"
                            radius: 4
                            anchors.fill: parent
                        }
                    }
                    MyPushButton2 {
                        id: nextSongButton
                        Layout.preferredHeight: 48
                        Layout.preferredWidth: 48

                        contentItem: Image {
                            source: "media_keys/outline_skip_next_white_24dp.svg"
                            sourceSize: Qt.size( imgSN.sourceSize.width*2, imgSN.sourceSize.height*2 )
                            Image{
                                id: imgSN
                                source: parent.source
                                width:0
                                height:0
                            }
                            anchors.fill: parent
                        }
                        onClicked: {
                            UtilitiesTabController.sendMediaNextSong()
                        }

                        background: Rectangle {
                            opacity: parent.down ? 1.0 : (parent.activeFocus ? 0.5 : 0.0)
                            color: "#406288"
                            radius: 4
                            anchors.fill: parent
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        Component.onCompleted: {
            alarmClockToggle.checked = UtilitiesTabController.alarmEnabled
            alarmDontAnnoyToggle.checked = !UtilitiesTabController.alarmIsModal
            setAlarmHourText(UtilitiesTabController.alarmTimeHour)
            setAlarmMinuteText(UtilitiesTabController.alarmTimeMinute)
            alarmGroupBox.visible = !OverlayController.desktopMode
            keyboardGroupBox.visible = !OverlayController.desktopMode
            steamDesktopGroupBox.visible = UtilitiesTabController.steamDesktopOverlayAvailable
            if (steamDesktopGroupBox.visible) {
                steamDesktopWidthSlider.value = UtilitiesTabController.steamDesktopOverlayWidth
            }
        }

        Connections {
            target: UtilitiesTabController
            onAlarmEnabledChanged: {
                alarmClockToggle.checked = UtilitiesTabController.alarmEnabled
            }
            onAlarmIsModalChanged: {
                alarmDontAnnoyToggle.checked = !UtilitiesTabController.alarmIsModal
            }
            onAlarmTimeHourChanged: {
                setAlarmHourText(UtilitiesTabController.alarmTimeHour)
            }
            onAlarmTimeMinuteChanged: {
                setAlarmMinuteText(UtilitiesTabController.alarmTimeMinute)
            }
            onSteamDesktopOverlayAvailableChanged: {
                steamDesktopGroupBox.visible = UtilitiesTabController.steamDesktopOverlayAvailable
                if (steamDesktopGroupBox.visible) {
                    steamDesktopWidthSlider.value = UtilitiesTabController.steamDesktopOverlayWidth
                }
            }
            onSteamDesktopOverlayWidthChanged: {
                steamDesktopWidthSlider.value = UtilitiesTabController.steamDesktopOverlayWidth
            }
        }
    }
    function setAlarmHourText(value) {
        if (value < 9) {
            alarmClockHourText.text = "0" + value
        } else {
            alarmClockHourText.text = value
        }
    }
    function setAlarmMinuteText(value) {
        if (value < 9) {
            alarmClockMinuteText.text = "0" + value
        } else {
            alarmClockMinuteText.text = value
        }
    }
}
