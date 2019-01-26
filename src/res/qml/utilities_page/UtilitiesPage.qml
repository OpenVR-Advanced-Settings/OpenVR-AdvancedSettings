import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
import "media_keys"
import ".."

MyStackViewPage {
    headerText: "Utilities"

    content: ColumnLayout {
        spacing: 18


        KeyboardGroupBox {
            id: keyboardGroupBox
        }

        AlarmGroupBox {
            id: alarmGroupBox
        }

        SteamDesktopGroupBox {
            id: steamDesktopGroupBox
        }

        MediaControllerKeys {
            id: mediaKeysGroupBox
        }

        Item {
            Layout.fillHeight: true
        }

    }
}
