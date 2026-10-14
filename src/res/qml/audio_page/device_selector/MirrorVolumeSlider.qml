import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"
import "."

ColumnLayout {
    spacing: 0
    MirrorDeviceSelector {
        id: audioMirrorNameCombo
        Layout.fillWidth: true
    }
    LineSeparator {
    }
    RowLayout {
        Layout.preferredHeight: 48
        MyText {
            text: "Mirror Volume"
            Layout.preferredWidth: 260
        }

        MyPushButton2 {
            id: audioMirrorMuteToggle
            Layout.preferredWidth: 44
            Layout.preferredHeight: 40
            checkable: true
            contentItem: Image {
                source: parent.checked ? "qrc:/qt/qml/AdvancedSettings/src/res/img/audio/speaker/speaker_off.svg" : "qrc:/qt/qml/AdvancedSettings/src/res/img/audio/speaker/speaker_on.svg"
                sourceSize.width: 26
                sourceSize.height: 26
                fillMode: Image.PreserveAspectFit
            }
            onCheckedChanged: {
                AudioTabController.setMirrorMuted(checked, false)
            }
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
    }
    Component.onCompleted: {
        if (AudioTabController.mirrorDeviceIndex < 0) {
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
        }
    }
    Connections {
        target: AudioTabController
        function onMirrorDeviceIndexChanged() {
            if (index < 0) {
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
                audioMirrorNameCombo.deviceIndex = index + 1
            }
        }
        function onMirrorVolumeChanged() {
            audioMirrorVolumeSlider.value = AudioTabController.mirrorVolume
        }
        function onMirrorMutedChanged() {
            audioMirrorMuteToggle.checked = AudioTabController.mirrorMuted
        }
        function onPlaybackDeviceListChanged() {
            if (AudioTabController.mirrorDeviceIndex < 0) {
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
            }
        }
    }
}
