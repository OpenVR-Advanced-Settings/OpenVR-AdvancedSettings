import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
import ".." //common imports
import "dialog_boxes"
import "device_selector"

RowLayout {
    MyText {
        text: "Mirror Device: "
        Layout.preferredWidth: 260
    }
    MyComboBox {
        id: audioMirrorNameCombo
        Layout.maximumWidth: 850
        Layout.minimumWidth: 850
        Layout.preferredWidth: 850
        onActivated: {
            if (componentCompleted) {
                AudioTabController.setMirrorDeviceIndex(currentIndex - 1)
            }
        }
    }
}
