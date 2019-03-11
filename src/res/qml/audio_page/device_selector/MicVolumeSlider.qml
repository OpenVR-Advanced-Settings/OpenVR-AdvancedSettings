import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

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
                source: parent.checked ? "qrc:/microphone/off" : "qrc:/microphone/on"
                sourceSize.width: 32
                sourceSize.height: 32
                anchors.fill: parent
            }
            onCheckedChanged: {
                AudioTabController.setMicMuted(checked)
            }
        }
    }
    Component.onCompleted: {
        if (AudioTabController.micDeviceIndex < 0) {
            audioMicVolumeMinusButton.enabled = false
            audioMicVolumeSlider.enabled = false
            audioMicVolumePlusButton.enabled = false
            audioMicMuteToggle.enabled = false
        }
        else {
            audioMicVolumeMinusButton.enabled = true
            audioMicVolumeSlider.enabled = true
            audioMicVolumePlusButton.enabled = true
            audioMicMuteToggle.enabled = true
            audioMicVolumeSlider.value = AudioTabController.micVolume
            audioMicMuteToggle.checked = AudioTabController.micMuted
        }
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
        onPlaybackDeviceListChanged: {

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
