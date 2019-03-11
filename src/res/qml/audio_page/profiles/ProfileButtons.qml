import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"
import "../dialog_boxes"

ColumnLayout {
    AudioMessageDialog {
        id: audioMessageDialog
    }

    AudioDeleteProfileDialog {
        id: audioDeleteProfileDialog
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
            // Text continues in next RowLayout.
            text: "Change the Push To Talk settings"
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
        MyText {
            // Text continues from RowLayout above.
            text: "\t\tin the SteamVR bindings menu."
        }
    }
    Component.onCompleted: {
        reloadAudioProfiles()
    }
    Connections {
        target: AudioTabController
        onAudioProfilesUpdated: {
            reloadAudioProfiles()
        }
        onAudioProfileAdded: {
            audioProfileComboBox.currentIndex = AudioTabController.getAudioProfileCount()
        }
        onDefaultProfileDisplay: {
            audioProfileComboBox.currentIndex = AudioTabController.getDefaultAudioProfileIndex(
                        ) + 1
        }
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
