import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

ColumnLayout {
    spacing: 0

    // Height of each row, set by the page. -1 leaves rows at their implicit height.
    property real rowHeight: -1

    MyToggleButton {
        id: audioPttEnabledToggle
        Layout.preferredHeight: rowHeight
        text: qsTr("Enable")
        Layout.fillWidth: true
        Layout.rightMargin: 0
        onClicked: {
            AudioTabController.setPttEnabled(checked, true)
        }
    }
        LineSeparator {
    }
    MyToggleButton {
        id: audioPttReverseToggle
        Layout.preferredHeight: rowHeight
        enabled: audioPttEnabledToggle.checked
        text: qsTr("Push-to-Mute")
        Layout.fillWidth: true
        Layout.leftMargin: 32
        Layout.rightMargin: 0
        onClicked: {
            AudioTabController.setMicReversePtt(checked, true)
        }
    }
    LineSeparator {
    }
    MyToggleButton {
        id: audioPttShowNotificationToggle
        Layout.preferredHeight: rowHeight
        enabled: audioPttEnabledToggle.checked
        text: qsTr("Show notification in HMD")
        Layout.fillWidth: true
        Layout.leftMargin: 32
        Layout.rightMargin: 0
        onCheckedChanged: {
            AudioTabController.setPttShowNotification(checked, false)
        }
    }

    Component.onCompleted: {
        audioPttEnabledToggle.checked = AudioTabController.pttEnabled
        audioPttShowNotificationToggle.checked = AudioTabController.pttShowNotification
        audioPttReverseToggle.checked = AudioTabController.micReversePtt
    }
    Connections {
        target: AudioTabController
        function onPttEnabledChanged() {
            audioPttEnabledToggle.checked = AudioTabController.pttEnabled
        }
        function onMicReversePttChanged() {
            audioPttReverseToggle.checked = AudioTabController.micReversePtt
        }
        function onPttShowNotificationChanged() {
            audioPttShowNotificationToggle.checked = AudioTabController.pttShowNotification
        }
    }
}
