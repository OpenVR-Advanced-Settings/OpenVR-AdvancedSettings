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
        border.color: "#d9dbe0"
        radius: 8
    }
    ColumnLayout {
        property int activationButtonWidth: 170;
        property int startStopPauseResetButtonWidth: 220

        ColumnLayout {

            LineSeparator {
            }

            RowLayout {
                MyToggleButton {
                    id: alarmClockToggle
                    text: "Alarm"

                    Layout.preferredWidth: parent.parent.parent.activationButtonWidth
                    onCheckedChanged: {
                        VrAlarm.alarmEnabled = checked
                    }
                }

                TimeAssembly {
                    id: alarmClockTimeAssembly
                    onTimeChanged: {
                        VrAlarm.setAlarmTime(hour, minute, second)
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                MyPushButton2 {
                    Layout.preferredWidth: 170
                    text: "Current Time"
                    onClicked: {
                        VrAlarm.setAlarmTimeToCurrentTime()
                        alarmClockTimeAssembly.changeTimer(VrAlarm.getAlarmHour(),
                                                           VrAlarm.getAlarmMinute(),
                                                           VrAlarm.getAlarmSecond())
                    }
                }

                MyPushButton2 {
                    Layout.preferredWidth: 120
                    text: "+1 Hour"
                    onClicked: {
                        VrAlarm.modAlarmTime(1, 0, 0)
                    }
                }

                MyPushButton2 {
                    Layout.preferredWidth: 120
                    text: "-1 Hour"
                    onClicked: {
                        VrAlarm.modAlarmTime(-1, 0, 0)
                    }
                }

                MyPushButton2 {
                    text: "+5 Min"
                    Layout.preferredWidth: 110
                    onClicked: {
                        VrAlarm.modAlarmTime(0, 5, 0)
                    }
                }

                MyPushButton2 {
                    Layout.preferredWidth: 110
                    text: "-5 Min"
                    onClicked: {
                        VrAlarm.modAlarmTime(0, -5, 0)
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        alarmClockToggle.checked = VrAlarm.alarmEnabled
        alarmClockTimeAssembly.changeTimer(VrAlarm.getAlarmHour(),
                                           VrAlarm.getAlarmMinute(),
                                           VrAlarm.getAlarmSecond())
    }
    Connections {
        target: VrAlarm
        onAlarmEnabledChanged: {
            alarmClockToggle.checked = VrAlarm.alarmEnabled
        }
        onAlarmTimeChanged: {
            alarmClockTimeAssembly.changeTimer(VrAlarm.getAlarmHour(),
                                               VrAlarm.getAlarmMinute(),
                                               VrAlarm.getAlarmSecond())
        }
    }
}
