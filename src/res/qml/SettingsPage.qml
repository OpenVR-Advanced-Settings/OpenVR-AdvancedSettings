import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "common"

MyStackViewPage {
    headerText: "Application Settings"

    content: ColumnLayout {
        spacing: 18

        MyToggleButton {
            id: settingsAutoStartToggle
            text: "Autostart"
            onCheckedChanged: {
                SettingsTabController.setAutoStartEnabled(checked, false)
            }
        }

        MyToggleButton {
            id: forceReviveToggle
            text: "Force Revive Page"
            onCheckedChanged: {
                SettingsTabController.setForceRevivePage(checked, true)
            }
        }

        MyToggleButton {
            id: allowExternalEditsToggle
            text: "Allow External App Chaperone Edits (Danger)"
            onCheckedChanged: {
                MoveCenterTabController.setAllowExternalEdits(checked, true)
            }
        }

        Item {
            Layout.fillHeight: true
        }

        Component.onCompleted: {
            settingsAutoStartToggle.checked = SettingsTabController.autoStartEnabled
            forceReviveToggle.checked = SettingsTabController.forceRevivePage
            allowExternalEditsToggle.checked = MoveCenterTabController.allowExternalEdits
        }

        Connections {
            target: SettingsTabController
            onAutoStartEnabledChanged: {
                settingsAutoStartToggle.checked = SettingsTabController.autoStartEnabled
            }
            onForceRevivePageChanged: {
                forceReviveToggle.checked = SettingsTabController.forceRevivePage
            }
        }

        Connections {
            target: MoveCenterTabController
            onAllowExternalEditsChanged: {
                allowExternalEditsToggle.checked = MoveCenterTabController.allowExternalEdits
            }
        }
    }
}
