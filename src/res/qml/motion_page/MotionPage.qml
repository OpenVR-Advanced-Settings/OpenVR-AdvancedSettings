import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "universe_drag"
import "universe_turn"
import "height_toggle"

MyStackViewPage {
    headerText: "Motion Settings"

    content: ColumnLayout {
        spacing: 18

        UniverseDragGroupBox { id: universeDragGroupBox }

        UniverseTurnGroupBox { id: universeTurnGroupBox }

        HeightToggleGroupBox { id: heightToggleGroupBox }

        Item {
            Layout.fillHeight: true
        }

    }
}
