import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

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
                    parent.parent.parent.setAlarmHourText(UtilitiesTabController.alarmTimeHour)
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
                    parent.parent.parent.setAlarmMinuteText(UtilitiesTabController.alarmTimeMinute)
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

    Component.onCompleted: {
        alarmClockToggle.checked = UtilitiesTabController.alarmEnabled
        alarmDontAnnoyToggle.checked = !UtilitiesTabController.alarmIsModal
        setAlarmHourText(UtilitiesTabController.alarmTimeHour)
        setAlarmMinuteText(UtilitiesTabController.alarmTimeMinute)
        alarmGroupBox.visible = !OverlayController.desktopMode
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
