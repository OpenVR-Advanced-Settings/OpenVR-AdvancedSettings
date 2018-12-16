import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0


MyStackViewPage {
    width: 1200
    headerText: "Audio Settings"

    property bool componentCompleted: false


    MyDialogOkPopup {
        id: audioMessageDialog
        function showMessage(title, text) {
            dialogTitle = title
            dialogText = text
            open()
        }
    }

    MyDialogOkCancelPopup {
        id: pttDeleteProfileDialog
        property int profileIndex: -1
        dialogTitle: "Delete Profile"
        dialogText: "Do you really want to delete this profile?"
        onClosed: {
            if (okClicked) {
                AudioTabController.deletePttProfile(profileIndex)
            }
        }
    }

    PttControllerConfigDialog {
        id: pttControllerConfigDialog
        pttControllerConfigClass: AudioTabController
    }

    MyDialogOkCancelPopup {
        id: pttNewProfileDialog
        dialogTitle: "Create New Profile"
        dialogWidth: 600
        dialogHeight: 220
        dialogContentItem: ColumnLayout {
            RowLayout {
                Layout.topMargin: 16
                Layout.leftMargin: 16
                Layout.rightMargin: 16
                MyText {
                    text: "Name: "
                }
                MyTextField {
                    id: pttNewProfileName
                    keyBoardUID: 590
                    color: "#cccccc"
                    text: ""
                    Layout.fillWidth: true
                    font.pointSize: 20
                    function onInputEvent(input) {
                        pttNewProfileName.text = input
                    }
                }
            }
        }
        onClosed: {
            if (okClicked) {
                if (pttNewProfileName.text != "") {
                    AudioTabController.addPttProfile(pttNewProfileName.text)
                } else {
                    audioMessageDialog.showMessage("Create New Profile", "ERROR: No name given.")
                }
            }
        }
        function openPopup() {
            pttNewProfileName.text = ""
            open()
        }
    }

    content: ColumnLayout {
        spacing: 24

        RowLayout {
            MyText {
                text: "Playback Device: "
                Layout.preferredWidth: 260
            }
            MyComboBox {
                id: audioPlaybackNameCombo
                Layout.maximumWidth: 850
                Layout.minimumWidth: 850
                Layout.preferredWidth: 850
                onCurrentIndexChanged: {
                    if (componentCompleted) {
                        AudioTabController.setPlaybackDeviceIndex(currentIndex)
                    }
                }
            }
        }

        ColumnLayout {
            spacing: 18
            RowLayout {
                MyText {
                    text: "Mirror Device: "
                    Layout.preferredWidth: 260
                }
                MyComboBox {
                    id: audioMirrorNameCombo
                    Layout.maximumWidth: 850
                    Layout.minimumWidth: 850
                    Layout.preferredWidth: 850
                    onCurrentIndexChanged: {
                        if (componentCompleted) {
                            AudioTabController.setMirrorDeviceIndex(currentIndex - 1)
                        }
                    }
                }
            }
            RowLayout {
                MyText {
                    text: "Mirror Volume:"
                    Layout.preferredWidth: 260
                }

                MyPushButton2 {
                    id: audioMirrorVolumeMinusButton
                    text: "-"
                    Layout.preferredWidth: 40
                    onClicked: {
                        audioMirrorVolumeSlider.value -= 0.1
                    }
                }

                MySlider {
                    id: audioMirrorVolumeSlider
                    from: 0.0
                    to: 1.0
                    stepSize: 0.01
                    value: 1.0
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = this.position * 100
                        audioMirrorVolumeText.text = Math.round(val) + "%"
                    }
                    onValueChanged: {
                        AudioTabController.setMirrorVolume(this.value.toFixed(2), false)
                    }
                }

                MyPushButton2 {
                    id: audioMirrorVolumePlusButton
                    text: "+"
                    Layout.preferredWidth: 40
                    onClicked: {
                        audioMirrorVolumeSlider.value += 0.1
                    }
                }

                MyText {
                    id: audioMirrorVolumeText
                    text: "100%"
                    Layout.preferredWidth: 85
                    horizontalAlignment: Text.AlignRight
                }

                MyPushButton2 {
                    id: audioMirrorMuteToggle
                    Layout.leftMargin: 12
                    checkable: true
                    contentItem: Image {
                        source: parent.checked ? "speaker_off.svg" : "speaker_on.svg"
                        sourceSize.width: 32
                        sourceSize.height: 32
                        anchors.fill: parent
                    }
                    onCheckedChanged: {
                        AudioTabController.setMirrorMuted(checked, false)
                    }
                }
            }
        }

        ColumnLayout {
            spacing: 18
            RowLayout {
                MyText {
                    text: "Microphone: "
                    Layout.preferredWidth: 260
                }
                MyComboBox {
                    id: audioMicNameCombo
                    Layout.maximumWidth: 850
                    Layout.minimumWidth: 850
                    Layout.preferredWidth: 850
                    onCurrentIndexChanged: {
                        if (componentCompleted) {
                            AudioTabController.setMicDeviceIndex(currentIndex)
                        }
                    }
                }
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
                    Layout.leftMargin: 265
                    text: "Show notification in HMD."
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
                spacing: 18

                MyText {
                    Layout.leftMargin: 270
                    text: "PTT Profile:"
                }

                MyComboBox {
                    id: pttProfileComboBox
                    Layout.fillWidth: true
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
                    Layout.preferredWidth: 200
                    text: "Apply"
                    onClicked: {
                        if (pttProfileComboBox.currentIndex > 0) {
                            AudioTabController.applyPttProfile(pttProfileComboBox.currentIndex - 1)
                            pttProfileComboBox.currentIndex = 0
                        }
                    }
                }
            }
            RowLayout {
                spacing: 18
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
            var devs1 = []
            var devs2 = ["<None>"]
            var playbackDeviceCount = AudioTabController.getPlaybackDeviceCount()
            for (var i = 0; i < playbackDeviceCount; i++) {
                var name = AudioTabController.getPlaybackDeviceName(i)
                devs1.push(name)
                devs2.push(name)
            }
            audioPlaybackNameCombo.model = devs1
            audioPlaybackNameCombo.currentIndex = AudioTabController.playbackDeviceIndex
            audioMirrorNameCombo.model = devs2
            if (AudioTabController.mirrorDeviceIndex < 0) {
                audioMirrorNameCombo.currentIndex = 0
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
                audioMirrorNameCombo.currentIndex = AudioTabController.mirrorDeviceIndex + 1
            }
            var devs3 = []
            var micDeviceCount = AudioTabController.getRecordingDeviceCount()
            for (var i = 0; i < micDeviceCount; i++) {
                var name = AudioTabController.getRecordingDeviceName(i)
                devs3.push(name)
            }
            audioMicNameCombo.model = devs3
            if (AudioTabController.micDeviceIndex < 0) {
                audioMicNameCombo.currentIndex = 0
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
                audioMicNameCombo.currentIndex = AudioTabController.micDeviceIndex
            }
            reloadPttProfiles()
            audioPttEnabledToggle.checked = AudioTabController.pttEnabled
            audioPttLeftControllerToggle.checked = AudioTabController.pttLeftControllerEnabled
            audioPttRightControllerToggle.checked = AudioTabController.pttRightControllerEnabled
            audioPttShowNotificationToggle.checked = AudioTabController.pttShowNotification
            audioPttReverseToggle.checked = AudioTabController.micReversePtt
            componentCompleted = true
        }

        Connections {
            target: AudioTabController
            onPlaybackDeviceIndexChanged: {
                audioPlaybackNameCombo.currentIndex = index
            }
            onMirrorDeviceIndexChanged: {
                if (index < 0) {
                    audioMirrorNameCombo.currentIndex = 0
                    audioMirrorVolumeMinusButton.enabled = false
                    audioMirrorVolumeSlider.enabled = false
                    audioMirrorVolumePlusButton.enabled = false
                    audioMirrorMuteToggle.enabled = false
                } else {
                    audioMirrorVolumeMinusButton.enabled = true
                    audioMirrorVolumeSlider.enabled = true
                    audioMirrorVolumePlusButton.enabled = true
                    audioMirrorMuteToggle.enabled = true
                    audioMirrorNameCombo.currentIndex = index + 1
                }
            }
            onMirrorVolumeChanged: {
                audioMirrorVolumeSlider.value = AudioTabController.mirrorVolume
            }
            onMirrorMutedChanged: {
                audioMirrorMuteToggle.checked = AudioTabController.mirrorMuted
            }
            onMicDeviceIndexChanged: {
                if (index < 0) {
                    audioMicNameCombo.currentIndex = 0
                    audioMicVolumeMinusButton.enabled = false
                    audioMicVolumeSlider.enabled = false
                    audioMicVolumePlusButton.enabled = false
                    audioMicMuteToggle.enabled = false
                } else {
                    audioMicVolumeMinusButton.enabled = true
                    audioMicVolumeSlider.enabled = true
                    audioMicVolumePlusButton.enabled = true
                    audioMicMuteToggle.enabled = true
                    audioMicNameCombo.currentIndex = index
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
            onPttProfilesUpdated: {
                reloadPttProfiles()
            }
            onPlaybackDeviceListChanged: {
                var devs1 = []
                var devs2 = ["<None>"]
                var playbackDeviceCount = AudioTabController.getPlaybackDeviceCount()
                for (var i = 0; i < playbackDeviceCount; i++) {
                    var name = AudioTabController.getPlaybackDeviceName(i)
                    devs1.push(name)
                    devs2.push(name)
                }
                audioPlaybackNameCombo.model = devs1
                audioPlaybackNameCombo.currentIndex = AudioTabController.playbackDeviceIndex
                audioMirrorNameCombo.model = devs2
                if (AudioTabController.mirrorDeviceIndex < 0) {
                    audioMirrorNameCombo.currentIndex = 0
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
                    audioMirrorNameCombo.currentIndex = AudioTabController.mirrorDeviceIndex + 1
                }
            }
            onRecordingDeviceListChanged: {
                var devs3 = []
                var micDeviceCount = AudioTabController.getRecordingDeviceCount()
                for (var i = 0; i < micDeviceCount; i++) {
                    var name = AudioTabController.getRecordingDeviceName(i)
                    devs3.push(name)
                }
                audioMicNameCombo.model = devs3
                if (AudioTabController.micDeviceIndex < 0) {
                    audioMicNameCombo.currentIndex = 0
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
                    audioMicNameCombo.currentIndex = AudioTabController.micDeviceIndex
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
}
