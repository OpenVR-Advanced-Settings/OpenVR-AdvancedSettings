import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

// UI only for now, not yet connected to a tab controller.
SettingsGroupBox {
    id: spaceOrientGroupBox
    Layout.fillWidth: true

    title: qsTr("Space Orient")

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true

            MyToggleButton {
                id: orientShortcutLeft
                text: qsTr("Left Hand")
            }

            MyToggleButton {
                id: orientShortcutRight
                text: qsTr("Right Hand")
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }
}
