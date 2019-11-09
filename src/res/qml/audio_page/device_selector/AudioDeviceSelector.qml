import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

RowLayout {
    id: audioPlaybackNameCombo
    property string deviceText: "Audio Device: "

    property alias devices: selector.model
    property alias deviceIndex: selector.currentIndex
    MyText {
        text: deviceText
        Layout.preferredWidth: 200
    }

    MyToggleButton {
        id: playbackOverrideToggle
        Layout.preferredWidth: 250
        text: "Toggle Override"
        onClicked: {
            AudioTabController.setPlaybackOverride(this.checked, false)
        }
        onCheckedChanged: {
            selector.enabled = this.checked
        }
    }

    MyComboBox {
        id: selector
        Layout.fillWidth: true
        onActivated: {
            if (deviceIndex >= 0) {
                AudioTabController.setPlaybackDeviceIndex(deviceIndex)
            }
        }
    }
    Component.onCompleted: {
        audioPlaybackNameCombo.devices = getAudioDeviceList()
        setShownAudioDevice(AudioTabController.playbackDeviceIndex)
        playbackOverrideToggle.checked = AudioTabController.playbackOverride
    }

    Connections {
        target: AudioTabController
        onPlaybackDeviceIndexChanged: {
            setShownAudioDevice(index)
        }
        onPlaybackDeviceListChanged: {
            audioPlaybackNameCombo.devices = getAudioDeviceList()
            setShownAudioDevice(AudioTabController.playbackDeviceIndex)
        }
        onPlaybackOverrideChanged: {
            playbackOverrideToggle.value = AudioTabController.playbackOverride
        }
    }
    function getAudioDeviceList() {
        var devices = []
        var playbackDevicesCount = AudioTabController.getPlaybackDeviceCount()
        for (var i = 0; i < playbackDevicesCount; i++) {
            var deviceName = AudioTabController.getPlaybackDeviceName(i)
            devices.push(deviceName)
        }
        // Setting the model zeroes out the
        // index so we need to set devices first.
        return devices
    }
    function setShownAudioDevice(index) {
        // If incorrect device, show "None".
        if (index < 0) {
            deviceIndex = 0
        }
        else {
            deviceIndex = index
        }
    }
}
