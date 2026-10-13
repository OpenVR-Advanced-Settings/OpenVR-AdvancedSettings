import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

ColumnLayout {
    spacing: 0

    MyToggleButton {
        id: audioPttEnabledToggle
        text: "Enable"
        Layout.fillWidth: true
        Layout.rightMargin: 0
        Layout.preferredHeight: 48
        onClicked: {
            AudioTabController.setPttEnabled(checked, true)
        }
    }
        LineSeparator {
    }
    MyToggleButton {
        id: audioPttReverseToggle
        enabled: audioPttEnabledToggle.checked
        text: "Push-to-Mute"
        Layout.fillWidth: true
        Layout.leftMargin: 32
        Layout.rightMargin: 0
        Layout.preferredHeight: 48
        onClicked: {
            AudioTabController.setMicReversePtt(checked, false)
        }
    }
    LineSeparator {
    }
    MyToggleButton {
        id: audioPttShowNotificationToggle
        enabled: audioPttEnabledToggle.checked
        text: "Show notification in HMD"
        Layout.fillWidth: true
        Layout.leftMargin: 32
        Layout.rightMargin: 0
        Layout.preferredHeight: 48
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
