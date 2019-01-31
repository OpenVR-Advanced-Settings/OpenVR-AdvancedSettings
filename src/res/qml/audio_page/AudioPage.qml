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

    property bool componentCompleted: false


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
        id: pttNewProfileDialog
    }

    AudioNewProfileDialog {
        id: audioNewProfileDialog
    }

    content: ColumnLayout {
        spacing: 24

        AudioDeviceSelector {
            id: audioPlaybackNameCombo
        }

        MirrorVolumeSlider {
            id: audioMirrorNameCombo
        }

        ColumnLayout {
            spacing: 18
            MicDeviceSelector {
                id: audioMicNameCombo
            }
            RowLayout {
                MyText {
                    text: "Microphone Volume:"
                    Layout.preferredWidth: 260
                }

                MyPushButton2 {
                    id: audioMicVolumeMinusButton
                    text: "-"
                    Layout.preferredWidth: 40
                    onClicked: {
                        audioMicVolumeSlider.value -= 0.1
                    }
                }

                MySlider {
                    id: audioMicVolumeSlider
                    from: 0.0
                    to: 1.0
                    stepSize: 0.01
                    value: 1.0
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = this.position * 100
                        audioMicVolumeText.text = Math.round(val) + "%"
                    }
                    onValueChanged: {
                        AudioTabController.setMicVolume(this.value.toFixed(2))
                    }
                }

                MyPushButton2 {
                    id: audioMicVolumePlusButton
                    text: "+"
                    Layout.preferredWidth: 40
                    onClicked: {
                        audioMicVolumeSlider.value += 0.1
                    }
                }

                MyText {
                    id: audioMicVolumeText
                    text: "100%"
                    Layout.preferredWidth: 85
                    horizontalAlignment: Text.AlignRight
                }

                MyPushButton2 {
                    id: audioMicMuteToggle
                    Layout.leftMargin: 12
                    checkable: true
                    contentItem: Image {
                        source: parent.checked ? "mic_off.svg" : "mic_on.svg"
                        sourceSize.width: 32
                        sourceSize.height: 32
                        anchors.fill: parent
                    }
                    onCheckedChanged: {
                        AudioTabController.setMicMuted(checked)
                    }
                }
            }
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
                audioMicVolumeMinusButton.enabled = false
                audioMicVolumeSlider.enabled = false
                audioMicVolumePlusButton.enabled = false
                audioMicMuteToggle.enabled = false
                audioMuteProximitySensorToggle.enabled = false
            } else {
                audioMicVolumeMinusButton.enabled = true
                audioMicVolumeSlider.enabled = true
                audioMicVolumePlusButton.enabled = true
                audioMicMuteToggle.enabled = true
                audioMuteProximitySensorToggle.enabled = true
                audioMicVolumeSlider.value = AudioTabController.micVolume
                audioMicMuteToggle.checked = AudioTabController.micMuted
                audioMuteProximitySensorToggle.checked = AudioTabController.micProximitySensorCanMute
            }


            reloadPttProfiles()
            reloadAudioProfiles()
            audioPttEnabledToggle.checked = AudioTabController.pttEnabled
            audioPttLeftControllerToggle.checked = AudioTabController.pttLeftControllerEnabled
            audioPttRightControllerToggle.checked = AudioTabController.pttRightControllerEnabled
            audioPttShowNotificationToggle.checked = AudioTabController.pttShowNotification
            audioPttReverseToggle.checked = AudioTabController.micReversePtt
            componentCompleted = true
        }

        Connections {
            target: AudioTabController
            onMicDeviceIndexChanged: {
                if (index < 0) {
                    audioMicVolumeMinusButton.enabled = false
                    audioMicVolumeSlider.enabled = false
                    audioMicVolumePlusButton.enabled = false
                    audioMicMuteToggle.enabled = false
                } else {
                    audioMicVolumeMinusButton.enabled = true
                    audioMicVolumeSlider.enabled = true
                    audioMicVolumePlusButton.enabled = true
                    audioMicMuteToggle.enabled = true
                }
            }
            onMicVolumeChanged: {
                audioMicVolumeSlider.value = AudioTabController.micVolume
            }
            onMicMutedChanged: {
                audioMicMuteToggle.checked = AudioTabController.micMuted
            }
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


            onPlaybackDeviceListChanged: {

                if (AudioTabController.mirrorDeviceIndex < 0) {
                    audioMirrorNameCombo.deviceIndex = 0
                    audioMirrorVolumeMinusButton.enabled = false
                    audioMirrorVolumeSlider.enabled = false
                    audioMirrorVolumePlusButton.enabled = false
                    audioMirrorMuteToggle.enabled = false
                } else {
                    audioMirrorVolumeMinusButton.enabled = true
                    audioMirrorVolumeSlider.enabled = true
                    audioMirrorVolumePlusButton.enabled = true
                    audioMirrorMuteToggle.enabled = true
                    audioMirrorVolumeSlider.value = AudioTabController.mirrorVolume
                    audioMirrorMuteToggle.checked = AudioTabController.mirrorMuted
                    audioMirrorNameCombo.deviceIndex = AudioTabController.mirrorDeviceIndex + 1
                }
            }
            onRecordingDeviceListChanged: {
                if (AudioTabController.micDeviceIndex < 0) {
                    audioMicVolumeMinusButton.enabled = false
                    audioMicVolumeSlider.enabled = false
                    audioMicVolumePlusButton.enabled = false
                    audioMicMuteToggle.enabled = false
                } else {
                    audioMicVolumeMinusButton.enabled = true
                    audioMicVolumeSlider.enabled = true
                    audioMicVolumePlusButton.enabled = true
                    audioMicMuteToggle.enabled = true
                    audioMicVolumeSlider.value = AudioTabController.micVolume
                    audioMicMuteToggle.checked = AudioTabController.micMuted
                }
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
