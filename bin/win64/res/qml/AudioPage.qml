import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0


MyStackViewPage {
    width: 1200
    headerText: "Audio Settings"


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

    MyDialogOkCancelPopup {
        id: pttControllerConfigDialog
        property int controllerIndex: -1
        dialogTitle: "Controller Configuration"
        dialogWidth: 600
        dialogHeight: 620
        dialogContentItem: ColumnLayout {
            MyText {
                text: "Buttons"
            }
            Rectangle {
                color: "#cccccc"
                height: 1
                Layout.fillWidth: true
            }
            MyToggleButton {
                id: pttControllerConfigGripButtonToggle
                text: "Grip"
            }
            MyToggleButton {
                id: pttControllerConfigMenuButtonToggle
                text: "Menu"
            }
            MyToggleButton {
                id: pttControllerConfigTriggerButtonToggle
                text: "Trigger"
            }

            MyText {
                Layout.topMargin: 32
                text: "Touchpad"
            }
            Rectangle {
                color: "#cccccc"
                height: 1
                Layout.fillWidth: true
            }
            RowLayout {
                spacing: 64
                MyToggleButton {
                    id: pttControllerConfigPadLeftToggle
                    text: "Left"
                }
                MyToggleButton {
                    id: pttControllerConfigPadRightToggle
                    text: "Right"
                }
            }
            RowLayout {
                spacing: 64
                MyToggleButton {
                    id: pttControllerConfigPadTopToggle
                    text: "Top"
                }
                MyToggleButton {
                    id: pttControllerConfigPadBottomToggle
                    text: "Bottom"
                }
            }
            RowLayout {
                MyText {
                    text: "Mode:"
                }
                MyComboBox {
                    id: pttControllerConfigPadModeCombo
                    Layout.preferredWidth: 250
                    model: ["Disabled", "Touched", "Pressed"]
                }
            }
        }
        onClosed: {
            if (okClicked) {
                var buttons = []
                if (pttControllerConfigGripButtonToggle.checked) {
                    buttons.push(2)
                }
                if (pttControllerConfigMenuButtonToggle.checked) {
                    buttons.push(1)
                }
                var triggerMode = 0
                if (pttControllerConfigTriggerButtonToggle.checked) {
                    triggerMode = 1
                }
                var padAreas = 0
                if (pttControllerConfigPadLeftToggle.checked) {
                    padAreas |= (1 << 0)
                }
                if (pttControllerConfigPadTopToggle.checked) {
                    padAreas |= (1 << 1)
                }
                if (pttControllerConfigPadRightToggle.checked) {
                    padAreas |= (1 << 2)
                }
                if (pttControllerConfigPadBottomToggle.checked) {
                    padAreas |= (1 << 3)
                }
                AudioTabController.setPttControllerConfig(controllerIndex, buttons, triggerMode, pttControllerConfigPadModeCombo.currentIndex, padAreas);
            }
        }
        function openPopup(controller) {
            if (controller === 0) {
                dialogTitle = "Left Controller Configuration"
                controllerIndex = 0
            } else {
                dialogTitle = "Right Controller Configuration"
                controllerIndex = 1
            }
            pttControllerConfigGripButtonToggle.checked = false
            pttControllerConfigMenuButtonToggle.checked = false
            pttControllerConfigTriggerButtonToggle.checked = false
            pttControllerConfigPadLeftToggle.checked = false
            pttControllerConfigPadTopToggle.checked = false
            pttControllerConfigPadRightToggle.checked = false
            pttControllerConfigPadBottomToggle.checked = false
            pttControllerConfigPadModeCombo.currentIndex = AudioTabController.pttTouchpadMode(controllerIndex)
            var buttons = AudioTabController.pttDigitalButtons(controllerIndex)
            for (var i = 0; i < buttons.length; i++) {
                if (buttons[i] == 1) {
                    pttControllerConfigMenuButtonToggle.checked = true
                } else if (buttons[i] == 2) {
                    pttControllerConfigGripButtonToggle.checked = true
                }
            }
            if (AudioTabController.pttTriggerMode(controllerIndex) > 0) {
                pttControllerConfigTriggerButtonToggle.checked = true
            }
            var padAreas = AudioTabController.pttTouchpadArea(controllerIndex)
            if (padAreas & (1 << 0)) {
                pttControllerConfigPadLeftToggle.checked = true
            }
            if (padAreas & (1 << 1)) {
                pttControllerConfigPadTopToggle.checked = true
            }
            if (padAreas & (1 << 2)) {
                pttControllerConfigPadRightToggle.checked = true
            }
            if (padAreas & (1 << 3)) {
                pttControllerConfigPadBottomToggle.checked = true
            }
            open()
        }
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
                TextField {
                    id: pttNewProfileName
                    color: "#cccccc"
                    text: ""
                    Layout.fillWidth: true
                    font.pointSize: 20
                    onActiveFocusChanged: {
                        if (activeFocus) {
                            OverlayController.showKeyboard(text, 101)
                        }
                    }
                    Connections {
                        target: OverlayController
                        onKeyBoardInputSignal: {
                            if (userValue == 101) {
                                pttNewProfileName.text = input
                            }
                        }
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
        spacing: 92

        ColumnLayout {
            spacing: 18
            RowLayout {
                MyText {
                    text: "Mirror Device: "
                }
                MyText {
                    id: audioMirrorNameText
                    text: "<None>"
                }
            }
            RowLayout {
                MyText {
                    text: "Volume:"
                    Layout.preferredWidth: 120
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
                }
                MyText {
                    id: audioMicNameText
                    text: "<None>"
                }
            }
            RowLayout {
                MyText {
                    text: "Volume:"
                    Layout.preferredWidth: 120
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
            MyToggleButton {
                id: audioPttShowNotificationToggle
                Layout.leftMargin: 265
                text: "Show notification in HMD."
                onCheckedChanged: {
                    AudioTabController.setPttShowNotification(checked, false)
                }
            }
            RowLayout {
                spacing: 18

                MyText {
                    Layout.leftMargin: 270
                    text: "Profile:"
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
            reloadPttProfiles()
            if (AudioTabController.mirrorPresent) {
                audioMirrorVolumeMinusButton.enabled = true
                audioMirrorVolumeSlider.enabled = true
                audioMirrorVolumePlusButton.enabled = true
                audioMirrorMuteToggle.enabled = true
                audioMirrorNameText.text = AudioTabController.mirrorDevName
                audioMirrorVolumeSlider.value = AudioTabController.mirrorVolume
                audioMirrorMuteToggle.checked = AudioTabController.mirrorMuted
            } else {
                audioMirrorVolumeMinusButton.enabled = false
                audioMirrorVolumeSlider.enabled = false
                audioMirrorVolumePlusButton.enabled = false
                audioMirrorMuteToggle.enabled = false
                audioMirrorNameText.text = "<None>"
            }
            if (AudioTabController.micPresent) {
                audioMicVolumeMinusButton.enabled = true
                audioMicVolumeSlider.enabled = true
                audioMicVolumePlusButton.enabled = true
                audioMicMuteToggle.enabled = true
                audioMicNameText.text = AudioTabController.micDevName
                audioMicVolumeSlider.value = AudioTabController.micVolume
                audioMicMuteToggle.checked = AudioTabController.micMuted
            } else {
                audioMicVolumeMinusButton.enabled = false
                audioMicVolumeSlider.enabled = false
                audioMicVolumePlusButton.enabled = false
                audioMicMuteToggle.enabled = false
                audioMicNameText.text = "<None>"
            }
            audioPttEnabledToggle.checked = AudioTabController.pttEnabled
            audioPttLeftControllerToggle.checked = AudioTabController.pttLeftControllerEnabled
            audioPttRightControllerToggle.checked = AudioTabController.pttRightControllerEnabled
            audioPttShowNotificationToggle.checked = AudioTabController.pttShowNotification
        }

        Connections {
            target: AudioTabController
            onMirrorPresentChanged: {
                if (value) {
                    audioMirrorVolumeMinusButton.enabled = true
                    audioMirrorVolumeSlider.enabled = true
                    audioMirrorVolumePlusButton.enabled = true
                    audioMirrorMuteToggle.enabled = true
                } else {
                    audioMirrorVolumeMinusButton.enabled = false
                    audioMirrorVolumeSlider.enabled = false
                    audioMirrorVolumePlusButton.enabled = false
                    audioMirrorMuteToggle.enabled = false
                    audioMirrorNameText.text = "<None>"
                }
            }
            onMirrorDevNameChanged: {
                if (AudioTabController.mirrorPresent) {
                    audioMirrorNameText.text = AudioTabController.mirrorDevName
                } else {
                    audioMirrorNameText.text = "<None>"
                }
            }
            onMirrorVolumeChanged: {
                audioMirrorVolumeSlider.value = AudioTabController.mirrorVolume
            }
            onMirrorMutedChanged: {
                audioMirrorMuteToggle.checked = AudioTabController.mirrorMuted
            }
            onMicPresentChanged: {
                if (value) {
                    audioMicVolumeMinusButton.enabled = true
                    audioMicVolumeSlider.enabled = true
                    audioMicVolumePlusButton.enabled = true
                    audioMicMuteToggle.enabled = true
                } else {
                    audioMicVolumeMinusButton.enabled = false
                    audioMicVolumeSlider.enabled = false
                    audioMicVolumePlusButton.enabled = false
                    audioMicMuteToggle.enabled = false
                    audioMicNameText.text = "<None>"
                }
            }
            onMicDevNameChanged: {
                if (AudioTabController.micPresent) {
                    audioMicNameText.text = AudioTabController.micDevName
                } else {
                    audioMicNameText.text = "<None>"
                }
            }
            onMicVolumeChanged: {
                audioMicVolumeSlider.value = AudioTabController.micVolume
            }
            onMicMutedChanged: {
                audioMicMuteToggle.checked = AudioTabController.micMuted
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
