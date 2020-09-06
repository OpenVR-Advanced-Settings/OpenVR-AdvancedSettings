import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "redirected"
import "space_turn"
import "snap_turn"
import "view_ratchet"


MyStackViewPage {
    width: 1200
    headerText: "Rotation Settings"
    content: ColumnLayout {
        spacing: 8
        AutoTurnGroupBox{
            id: autoTurnGroupBox
        }

        RedirectedGroupBox{
            id: redirectedGroupBox
        }
        ViewRatchetGroupBox{
            id: vgiiewRatchetGroupBox
        }
        SpaceTurnGroupBox{
            id:spaceTurnGroupBox
        }

        SnapTurnGroupBox{
            id: snapTurnGroupBox
        }


        Item {
            Layout.fillHeight: true
        }

    }
}
