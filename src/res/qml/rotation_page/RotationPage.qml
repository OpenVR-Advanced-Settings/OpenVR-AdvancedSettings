import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "redirected"
import "space_turn"
import "snap_turn"
import "view_ratchet"
import "rotate_space"


MyStackViewPage {
    headerText: qsTr("Rotation Settings")

    // Every group box row on this page uses this height.
    readonly property int groupBoxRowHeight: 44
    content: ColumnLayout {
        spacing: 8
        SpaceTurnGroupBox{
            id: spaceTurnGroupBox
            rowHeight: groupBoxRowHeight
        }

        SnapTurnGroupBox{
            id: snapTurnGroupBox
            rowHeight: groupBoxRowHeight
        }

        RotateSpaceGroupBox{
            id: rotateSpaceGroupBox
            rowHeight: groupBoxRowHeight
        }

        AutoTurnGroupBox{
            id: autoTurnGroupBox
            rowHeight: groupBoxRowHeight
        }

        RedirectedGroupBox{
            id: redirectedGroupBox
            rowHeight: groupBoxRowHeight
        }
        ViewRatchetGroupBox{
            id: vgiiewRatchetGroupBox
            rowHeight: groupBoxRowHeight
            // Untitled, so it reads as a second row of the box above.
            // Pull it up so the gap matches a LineSeparator (2px).
            Layout.topMargin: -6
        }


        Item {
            Layout.fillHeight: true
        }

    }
}
