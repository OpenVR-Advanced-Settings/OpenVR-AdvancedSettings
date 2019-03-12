import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

RowLayout {
    id: audioMicNameCombo
    property string deviceText: "Microphone: "

    // The entries in the dropdown box.
    property alias devices: selector.model
    // The current index of the chosen device.
    // Add one for use in the dropdown box because of the "None" entry.
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
            if (deviceIndex >= 0) {
                AudioTabController.setMicDeviceIndex(currentIndex)
            }
        }
    }
    function getAudioDeviceList() {
        var devices = []
        var playbackDevicesCount = AudioTabController.getRecordingDeviceCount()
        for (var i = 0; i < playbackDevicesCount; i++) {
            var deviceName = AudioTabController.getRecordingDeviceName(i)
            devices.push(deviceName)
        }
        // Setting the model zeroes out the
        // index so we need to set devices first.
        return devices
    }
    function setShownAudioDevice(index) {
        if (index < 0) {
            deviceIndex = 0
        }
        else {
            deviceIndex = index
        }
    }
    Component.onCompleted: {
        devices = getAudioDeviceList()
        setShownAudioDevice(AudioTabController.micDeviceIndex)
    }
    Connections {
        target: AudioTabController
        onMicDeviceIndexChanged: {
            setShownAudioDevice(index)
        }
        onRecordingDeviceListChanged: {
            devices = getAudioDeviceList()
            setShownAudioDevice(AudioTabController.micDeviceIndex)
        }
    }
}
