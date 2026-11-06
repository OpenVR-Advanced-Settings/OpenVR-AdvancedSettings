import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: alarmGroupBox
    Layout.fillWidth: true
    
    title: qsTr("Alarm Clock")
    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        RowLayout {
            Layout.preferredHeight: rowHeight
            spacing: 18
            Layout.fillWidth: true

            MyToggleButton {
                id: alarmClockToggle
                text: qsTr("On/Off")
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
        }
        LineSeparator {
        }
        RowLayout {
            Layout.preferredHeight: rowHeight
            spacing: 18
            Layout.fillWidth: true

            MyPushButton {
                text: qsTr("Current Time")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                onClicked: {
                    VrAlarm.setAlarmTimeToCurrentTime()
                    alarmClockTimeAssembly.changeTimer(VrAlarm.getAlarmHour(),
                                                       VrAlarm.getAlarmMinute(),
                                                       VrAlarm.getAlarmSecond())
                }
            }

            MyPushButton {
                text: qsTr("+1 Hour")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                onClicked: {
                    VrAlarm.modAlarmTime(1, 0, 0)
                }
            }

            MyPushButton {
                text: qsTr("-1 Hour")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                onClicked: {
                    VrAlarm.modAlarmTime(-1, 0, 0)
                }
            }

            MyPushButton {
                text: qsTr("+5 Min")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                onClicked: {
                    VrAlarm.modAlarmTime(0, 5, 0)
                }
            }

            MyPushButton {
                text: qsTr("-5 Min")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                onClicked: {
                    VrAlarm.modAlarmTime(0, -5, 0)
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
        function onAlarmEnabledChanged() {
            alarmClockToggle.checked = VrAlarm.alarmEnabled
        }
        function onAlarmTimeChanged() {
            alarmClockTimeAssembly.changeTimer(VrAlarm.getAlarmHour(),
                                               VrAlarm.getAlarmMinute(),
                                               VrAlarm.getAlarmSecond())
        }
    }
}
