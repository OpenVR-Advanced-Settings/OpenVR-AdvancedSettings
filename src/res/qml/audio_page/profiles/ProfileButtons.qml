import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
import "../../common"
import "../dialog_boxes"

ColumnLayout {
    AudioMessageDialog {
        id: audioMessageDialog
    }

    PttDeleteProfileDialog {
        id: pttDeleteProfileDialog
    }

    AudioDeleteProfileDialog {
        id: audioDeleteProfileDialog
    }

    PttNewProfileDialog {
        id: pttNewProfileDialog
    }

    AudioNewProfileDialog {
        id: audioNewProfileDialog
    }

    RowLayout {
        spacing: 10

        MyText {
            Layout.preferredWidth: 150
            text: "Audio Profile:"
        }

        MyComboBox {
            id: audioProfileComboBox
            Layout.preferredWidth: 250
            model: [""]
            onCurrentIndexChanged: {
                if (currentIndex > 0) {
                    audioApplyProfileButton.enabled = true
                    audioDeleteProfileButton.enabled = true
                } else {
                    audioApplyProfileButton.enabled = false
                    audioDeleteProfileButton.enabled = false
                }
            }
        }

        MyPushButton {
            id: audioApplyProfileButton
            enabled: true
            Layout.preferredWidth: 150
            text: "Apply"
            onClicked: {
                if (audioProfileComboBox.currentIndex > 0) {
                    AudioTabController.applyAudioProfile(
                                audioProfileComboBox.currentIndex - 1)
                }
            }
        }

        MyText {
            text: "PTT Profile:"
            Layout.preferredWidth: 150
        }

        MyComboBox {
            id: pttProfileComboBox
            Layout.preferredWidth: 250
            model: [""]
            onCurrentIndexChanged: {
                if (currentIndex > 0) {
                    pttApplyProfileButton.enabled = true
                    pttDeleteProfileButton.enabled = true
                } else {
                    pttApplyProfileButton.enabled = false
                    pttDeleteProfileButton.enabled = false
                }
            }
        }

        MyPushButton {
            id: pttApplyProfileButton
            enabled: true
            Layout.preferredWidth: 150
            text: "Apply"
            onClicked: {
                if (pttProfileComboBox.currentIndex > 0) {
                    AudioTabController.applyPttProfile(
                                pttProfileComboBox.currentIndex - 1)
                }
            }
        }
    }
    RowLayout {
        spacing: 18

        MyPushButton {
            id: audioDeleteProfileButton
            enabled: true
            Layout.preferredWidth: 200
            text: "Delete Profile"
            onClicked: {
                if (audioProfileComboBox.currentIndex > 0) {
                    audioDeleteProfileDialog.profileIndex = audioProfileComboBox.currentIndex - 1
                    audioDeleteProfileDialog.open()
                }
            }
        }
        MyPushButton {
            Layout.preferredWidth: 200
            text: "New Profile"
            onClicked: {
                audioNewProfileDialog.openPopup()
            }
        }
        Item {
            Layout.fillWidth: true
        }
        MyPushButton {
            id: pttDeleteProfileButton
            enabled: true
            Layout.preferredWidth: 200
            text: "Delete Profile"
            onClicked: {
                if (pttProfileComboBox.currentIndex > 0) {
                    pttDeleteProfileDialog.profileIndex = pttProfileComboBox.currentIndex - 1
                    pttDeleteProfileDialog.open()
                }
            }
        }
        MyPushButton {
            Layout.preferredWidth: 200
            text: "New Profile"
            onClicked: {
                pttNewProfileDialog.openPopup()
            }
        }
    }
    Component.onCompleted: {
        reloadPttProfiles()
        reloadAudioProfiles()
    }
    Connections {
        target: AudioTabController
        onPttProfilesUpdated: {
            reloadPttProfiles()
        }
        onAudioProfilesUpdated: {
            reloadAudioProfiles()
        }
        onAudioProfileAdded: {
            audioProfileComboBox.currentIndex = AudioTabController.getAudioProfileCount()
        }
        onPttProfileAdded: {
            pttProfileComboBox.currentIndex = AudioTabController.getPttProfileCount()
        }
        onDefaultProfileDisplay: {
            audioProfileComboBox.currentIndex = AudioTabController.getDefaultAudioProfileIndex(
                        ) + 1
        }
    }
    function reloadPttProfiles() {
        var profiles = [""]
        var profileCount = AudioTabController.getPttProfileCount()
        for (var i = 0; i < profileCount; i++) {
            profiles.push(AudioTabController.getPttProfileName(i))
        }
        pttProfileComboBox.currentIndex = 0
        pttProfileComboBox.model = profiles
    }

    function reloadAudioProfiles() {
        var profiles = [""]
        var profileCount = AudioTabController.getAudioProfileCount()
        for (var i = 0; i < profileCount; i++) {
            profiles.push(AudioTabController.getAudioProfileName(i))
        }
        audioProfileComboBox.currentIndex = 0
        audioProfileComboBox.model = profiles
    }
}
