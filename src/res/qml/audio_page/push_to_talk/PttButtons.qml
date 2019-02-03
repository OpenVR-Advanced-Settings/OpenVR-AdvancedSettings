import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
import "../../common"

ColumnLayout {
    RowLayout {
        PttControllerConfigDialog {
            id: pttControllerConfigDialog
            pttControllerConfigClass: AudioTabController
        }

        MyToggleButton {
            id: audioPttEnabledToggle
            Layout.preferredWidth: 260
            text: "Push-to-Talk:"
            onClicked: {
                AudioTabController.pttEnabled = checked
            }
        }
        MyToggleButton {
            id: audioPttLeftControllerToggle
            text: "Left Controller"
            onClicked: {
                AudioTabController.setPttLeftControllerEnabled(checked, false)
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
            id: audioPttRightControllerToggle
            text: "Right Controller"
            onClicked: {
                AudioTabController.setPttRightControllerEnabled(checked, false)
            }
        }
        MyPushButton {
            text: "Configure"
            onClicked: {
                pttControllerConfigDialog.openPopup(1)
            }
        }
    }
    RowLayout {

        MyToggleButton {
            id: audioPttShowNotificationToggle
            Layout.leftMargin: 350
            text: "Show notification in HMD"
            onCheckedChanged: {
                AudioTabController.setPttShowNotification(checked, false)
            }
        }
        MyToggleButton {
            id: audioPttReverseToggle
            Layout.leftMargin: 118
            text: "Push-to-Mute"
            onClicked: {
                AudioTabController.setMicReversePtt(checked, false)
            }
        }
    }

    Component.onCompleted: {
        audioPttEnabledToggle.checked = AudioTabController.pttEnabled
        audioPttLeftControllerToggle.checked = AudioTabController.pttLeftControllerEnabled
        audioPttRightControllerToggle.checked = AudioTabController.pttRightControllerEnabled
        audioPttShowNotificationToggle.checked = AudioTabController.pttShowNotification
        audioPttReverseToggle.checked = AudioTabController.micReversePtt
    }
    Connections {
        target: AudioTabController
        onPttEnabledChanged: {
            audioPttEnabledToggle.checked = AudioTabController.pttEnabled
        }
        onPttLeftControllerEnabledChanged: {
            audioPttLeftControllerToggle.checked = AudioTabController.pttLeftControllerEnabled
        }
        onPttRightControllerEnabledChanged: {
            audioPttRightControllerToggle.checked = AudioTabController.pttRightControllerEnabled
        }
        onMicReversePttChanged: {
            audioPttReverseToggle.checked = AudioTabController.micReversePtt
        }
        onPttShowNotificationChanged: {
            audioPttShowNotificationToggle.checked = AudioTabController.pttShowNotification
        }
    }
}
