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
        Layout.preferredWidth: 200
    }
    MyToggleButton {
        id: recordingOverrideToggle
        Layout.preferredWidth: 250
        text: "Toggle Override"
        onClicked: {
            AudioTabController.setRecordingOverride(this.checked, false)
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
        recordingOverrideToggle.checked = AudioTabController.recordingOverride
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
        onRecordingOverrideChanged: {
            recordingOverrideToggle.value = AudioTabController.recordingOverride
        }
    }
}
