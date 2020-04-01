import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "redirected"
import "snap_turn"


MyStackViewPage {
    width: 1200
    headerText: "Rotation Settings"
    content: ColumnLayout {
        spacing: 10
        AutoTurnGroupBox{
            id: autoTurnGroupBox
        }

        RedirectedGroupBox{
            id: redirectedGroupBox
        }

        SnapTurnGroupBox{
            id: snapTurnGroupBox
        }


        Item {
            Layout.fillHeight: true
        }

    }
}
