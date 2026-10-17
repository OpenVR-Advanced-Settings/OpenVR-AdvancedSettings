import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "space_orient"
import "adjust_orientation"

MyStackViewPage {
    headerText: qsTr("Orientation Settings")

    DeviceOverrideDialog {
        id: spaceOrientDeviceDialog
    }

    content: ColumnLayout {
        spacing: 18

        SpaceOrientGroupBox { id: spaceOrientGroupBox }

        AdjustOrientationGroupBox { id: adjustOrientationGroupBox }


        Item {
            Layout.fillHeight: true
        }

    }
}
