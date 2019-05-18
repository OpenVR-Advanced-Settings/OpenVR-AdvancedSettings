import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

RowLayout {
    id: audioMirrorNameCombo
    // Text shown to the left.
    property string deviceText: "Mirror Device: "

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
                // The mirror device list contains a "None" entry, hence
                // we need to subtract one to get the real index.
                AudioTabController.setMirrorDeviceIndex(deviceIndex - 1)
            }
        }
    }
    Component.onCompleted: {
        // Populate dropdown with devices
        audioMirrorNameCombo.devices = getAudioDeviceList()

        var mirrorIndex = AudioTabController.mirrorDeviceIndex
        setShownAudioDevice(mirrorIndex)

    }
    Connections {
        target: AudioTabController
        onMirrorDeviceIndexChanged: {
            setShownAudioDevice(index)
        }
        onPlaybackDeviceListChanged: {
            audioMirrorNameCombo.devices = getAudioDeviceList()
            setShownAudioDevice(AudioTabController.mirrorDeviceIndex)
        }
    }
    function getAudioDeviceList() {
        var devices = ["<None>"]
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
            // The mirror device list contains a "None" entry
            // hence we need to add one to show the correct entry.
            deviceIndex = index + 1
        }
    }
}
