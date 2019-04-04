import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "dimmer"

MyStackViewPage {
    headerText: "Motion Settings"

    content: ColumnLayout {
        spacing: 18

        DimmerGroupBox { id: dimmerGroupBox }

        Item {
            Layout.fillHeight: true
        }

    }
}
