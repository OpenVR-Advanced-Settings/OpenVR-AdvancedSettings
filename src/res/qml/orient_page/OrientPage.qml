import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "space_orient"
import "adjust_orientation"

MyStackViewPage {
    headerText: qsTr("Orientation Settings")

    // Every group box row on this page uses this height.
    readonly property int groupBoxRowHeight: 44

    DeviceOverrideDialog {
        id: spaceOrientDeviceDialog
    }

    content: ColumnLayout {
        spacing: 8

        SpaceOrientGroupBox {
            id: spaceOrientGroupBox
            rowHeight: groupBoxRowHeight
        }

        AdjustOrientationGroupBox {
            id: adjustOrientationGroupBox
        }


        Item {
            Layout.fillHeight: true
        }

    }
}
