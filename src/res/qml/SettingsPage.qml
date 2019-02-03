import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
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

        Item {
            Layout.fillHeight: true
        }

        Component.onCompleted: {
            settingsAutoStartToggle.checked = SettingsTabController.autoStartEnabled
            forceReviveToggle.checked = SettingsTabController.forceRevivePage
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
    }
}
