import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "common"

MyStackViewPage {
    headerText: "Bindings Guide"

    content: ColumnLayout {
        Image {
            source: "qrc:/bindings/bindings"
            sourceSize.width: 1120
            sourceSize.height: 659
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
