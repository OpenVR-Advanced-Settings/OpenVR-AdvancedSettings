import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "redirected"


MyStackViewPage {
    width: 1200
    headerText: "Rotation Settings"
    content: ColumnLayout {
        spacing: 10

        RedirectedGroupBox{
        id: redirectedGroupBox
        }

        Item {
            Layout.fillHeight: true
        }

    }
}
