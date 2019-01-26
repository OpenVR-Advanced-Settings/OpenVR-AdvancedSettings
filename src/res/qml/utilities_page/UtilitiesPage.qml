import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
import "media_keys"
import "alarm_clock"
import "desktop_size"
import "keyboard_utils"
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
