import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
import ".." //common imports
import "dialog_boxes"
import "device_selector"

MyStackViewPage {
    width: 1200
    headerText: "Audio Settings"

    AudioMessageDialog {
        id: audioMessageDialog
    }

    PttDeleteProfileDialog {
        id: pttDeleteProfileDialog
    }

    AudioDeleteProfileDialog {
        id: audioDeleteProfileDialog
    }

    PttControllerConfigDialog {
        id: pttControllerConfigDialog
        pttControllerConfigClass: AudioTabController
    }

    PttNewProfileDialog {
    }

    AudioNewProfileDialog {
    }

    content: ColumnLayout {
        spacing: 24

        AudioDeviceSelector {
        }

        MirrorVolumeSlider {
        }

        MicVolumeSlider {
        }
        ColumnLayout {
            spacing: 18
            MyToggleButton {
                id: audioMuteProximitySensorToggle
                Layout.preferredWidth: 600
                text: "Proximity Sensor Mutes/Unmutes Microphone"
                onClicked: {
                    AudioTabController.setMicProximitySensorCanMute(checked, false)
                }
            }
            RowLayout {
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
                            AudioTabController.applyAudioProfile(audioProfileComboBox.currentIndex - 1)
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
                            AudioTabController.applyPttProfile(pttProfileComboBox.currentIndex - 1)
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
        }

        Component.onCompleted: {
            if (AudioTabController.micDeviceIndex < 0) {
                audioMuteProximitySensorToggle.enabled = false
            } else {
                audioMuteProximitySensorToggle.enabled = true
                audioMuteProximitySensorToggle.checked = AudioTabController.micProximitySensorCanMute
            }
            reloadPttProfiles()
            reloadAudioProfiles()
            audioPttEnabledToggle.checked = AudioTabController.pttEnabled
            audioPttLeftControllerToggle.checked = AudioTabController.pttLeftControllerEnabled
            audioPttRightControllerToggle.checked = AudioTabController.pttRightControllerEnabled
            audioPttShowNotificationToggle.checked = AudioTabController.pttShowNotification
            audioPttReverseToggle.checked = AudioTabController.micReversePtt
        }

        Connections {
            target: AudioTabController
            onMicProximitySensorCanMuteChanged: {
                audioMuteProximitySensorToggle.checked = AudioTabController.micProximitySensorCanMute
            }
            onMicReversePttChanged: {
                audioPttReverseToggle.checked = AudioTabController.micReversePtt
            }
            onPttEnabledChanged: {
                audioPttEnabledToggle.checked = AudioTabController.pttEnabled
            }
            onPttActiveChanged: {

            }
            onPttShowNotificationChanged: {
                audioPttShowNotificationToggle.checked = AudioTabController.pttShowNotification
            }
            onPttLeftControllerEnabledChanged: {
                audioPttLeftControllerToggle.checked = AudioTabController.pttLeftControllerEnabled
            }
            onPttRightControllerEnabledChanged: {
                audioPttRightControllerToggle.checked = AudioTabController.pttRightControllerEnabled
            }
            onAudioProfileDefaultChanged: {
                audioDefaultProfileToggle.checked = AudioTabController.audioProfileDefault
            }
            onPttProfilesUpdated: {
                reloadPttProfiles()
            }
            onAudioProfilesUpdated: {
                reloadAudioProfiles()
            }
            onAudioProfileAdded:{
                audioProfileComboBox.currentIndex = AudioTabController.getAudioProfileCount()
            }
            onPttProfileAdded:{
                pttProfileComboBox.currentIndex = AudioTabController.getPttProfileCount()
            }
            onDefaultProfileDisplay:{
                audioProfileComboBox.currentIndex = AudioTabController.getDefaultAudioProfileIndex() + 1
            }
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

    function reloadAudioProfiles(){
        var profiles = [""]
        var profileCount = AudioTabController.getAudioProfileCount()
        for (var i = 0; i < profileCount; i++) {
            profiles.push(AudioTabController.getAudioProfileName(i))
        }
        audioProfileComboBox.currentIndex = 0;
        audioProfileComboBox.model = profiles
    }
}
