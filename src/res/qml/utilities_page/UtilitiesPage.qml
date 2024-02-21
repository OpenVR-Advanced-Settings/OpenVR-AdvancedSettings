import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "media_keys"
import "alarm_clock"
import "keyboard_utils"
import "../common"
import "misc"

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

        MediaControllerKeys {
            id: mediaKeysGroupBox
        }

        MiscBox{
            id:utilMiscGroupBox
        }

        Item {
            Layout.fillHeight: true
        }

    }
}
