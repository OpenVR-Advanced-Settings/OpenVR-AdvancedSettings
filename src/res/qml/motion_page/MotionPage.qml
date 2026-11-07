import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "space_drag"
import "height_toggle"
import "gravity"
import "tracking_jump"

MyStackViewPage {
    headerText: "Motion Settings"

    content: ScrollView {
        id: motionScrollView
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        ColumnLayout {
            width: motionScrollView.availableWidth

            spacing: 18

            SpaceDragGroupBox { id: spaceDragGroupBox }

            HeightToggleGroupBox { id: heightToggleGroupBox }

            GravityGroupBox { id: gravityGroupBox }

            TrackingJumpGroupBox { id: trackingJumpGroupBox }
        }
    }
}
