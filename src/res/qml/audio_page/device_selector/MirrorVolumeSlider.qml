import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"
import "."

ColumnLayout {
    spacing: 18
    MirrorDeviceSelector {
        id: audioMirrorNameCombo
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
                source: parent.checked ? "qrc:/speaker/off" : "qrc:/speaker/on"
                sourceSize.width: 32
                sourceSize.height: 32
                anchors.fill: parent
            }
            onCheckedChanged: {
                AudioTabController.setMirrorMuted(checked, false)
            }
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
        onMirrorDeviceIndexChanged: {
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
        onMirrorVolumeChanged: {
            audioMirrorVolumeSlider.value = AudioTabController.mirrorVolume
        }
        onMirrorMutedChanged: {
            audioMirrorMuteToggle.checked = AudioTabController.mirrorMuted
        }
        onPlaybackDeviceListChanged: {
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
