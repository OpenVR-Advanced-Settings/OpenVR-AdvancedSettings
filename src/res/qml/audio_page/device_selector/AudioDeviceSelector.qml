import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
import "../.." // common imports

RowLayout {
    id: audioPlaybackNameCombo
    property string deviceText: "Audio Device: "

    property alias devices: selector.model
    property alias deviceIndex: selector.currentIndex
    MyText {
        text: deviceText
        Layout.preferredWidth: 260
    }
    MyComboBox {
        id: selector
        Layout.maximumWidth: 850
        Layout.minimumWidth: 850
        Layout.preferredWidth: 850
        onActivated: {
            if (currentIndex >= 0) {
                AudioTabController.setPlaybackDeviceIndex(currentIndex, false)
            }
        }
    }
    Component.onCompleted: {
        audioPlaybackNameCombo.devices = getAudioDeviceList()
        audioPlaybackNameCombo.deviceIndex = AudioTabController.playbackDeviceIndex
    }

    Connections {
        target: AudioTabController
        onPlaybackDeviceIndexChanged: {
            audioPlaybackNameCombo.deviceIndex = index
        }
        onPlaybackDeviceListChanged: {
            audioPlaybackNameCombo.devices = getAudioDeviceList()
            audioPlaybackNameCombo.deviceIndex = AudioTabController.playbackDeviceIndex
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
}
