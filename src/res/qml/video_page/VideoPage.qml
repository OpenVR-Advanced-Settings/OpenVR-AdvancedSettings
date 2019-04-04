import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "brightness"

MyStackViewPage {
    headerText: "Video Settings"

    content: ColumnLayout {
        spacing: 18

        BrightnessGroupBox { id: brightnessGroupBox }

        Item {
            Layout.fillHeight: true
        }

    }
}
