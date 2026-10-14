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
    headerText: "Rotation Settings"
    content: ColumnLayout {
        spacing: 8
        SpaceTurnGroupBox{
            id:spaceTurnGroupBox
        }

        SnapTurnGroupBox{
            id: snapTurnGroupBox
        }

        AutoTurnGroupBox{
            id: autoTurnGroupBox
        }

        RedirectedGroupBox{
            id: redirectedGroupBox
        }
        ViewRatchetGroupBox{
            id: vgiiewRatchetGroupBox
            // Untitled, so it reads as a second row of the box above.
            // Pull it up so the gap matches a LineSeparator (2px).
            Layout.topMargin: -6
        }


        Item {
            Layout.fillHeight: true
        }

    }
}
