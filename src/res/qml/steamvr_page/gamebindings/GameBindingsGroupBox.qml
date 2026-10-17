import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

// Import and Export are UI only for now.
SettingsGroupBox {
    id: gameBindingsGroupBox
    Layout.fillWidth: true

    title: qsTr("Game Bindings")

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            Layout.preferredHeight: rowHeight
            Layout.fillWidth: true
            spacing: 16

            MyPushButton {
                id: gameBindingsImportButton
                text: qsTr("Import")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
            }

            VerticalSeparator {
            }

            MyPushButton {
                id: gameBindingsExportButton
                text: qsTr("Export")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
            }

            VerticalSeparator {
            }

            MyPushButton {
                id: gameBindingsOpenButton
                activationSoundEnabled: false
                text: qsTr("Open Bindings")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                onClicked: {
                    SteamVRTabController.launchBindingUI()
                }
            }
        }
    }
}
