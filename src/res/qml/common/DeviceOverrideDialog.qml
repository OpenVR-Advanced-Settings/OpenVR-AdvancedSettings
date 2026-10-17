import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "."

MyDialogOkCancelPopup {
    id: deviceOverrideDialog
    dialogTitle: qsTr("Override Device")
    dialogWidth: 600
    dialogHeight: 300

    property var acceptCallback: null

    dialogContentItem: ColumnLayout {
        RowLayout {
            Layout.topMargin: 16
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            MyText {
                text: qsTr("Device")
            }
            MyComboBox {
                id: deviceOverrideComboBox
                Layout.preferredWidth: 440
                model: [qsTr("None")]
            }
        }
    }

    onClosed: {
        if (okClicked && acceptCallback) {
            acceptCallback(deviceOverrideComboBox.currentIndex <= 0
                       ? "" : deviceOverrideComboBox.currentText)
        }
        acceptCallback = null
    }

    function openPopup(title, currentDevice, acceptedCallback) {
        dialogTitle = title
        acceptCallback = acceptedCallback
        var devices = MoveCenterTabController.getTrackedDeviceList()
        deviceOverrideComboBox.model = [qsTr("None")].concat(devices)
        var index = devices.indexOf(currentDevice)
        deviceOverrideComboBox.currentIndex = index < 0 ? 0 : index + 1
        open()
    }
}
