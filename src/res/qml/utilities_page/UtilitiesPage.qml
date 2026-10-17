import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "media_keys"
import "alarm_clock"
import "keyboard_utils"
import "../common"
import "misc"
import "space_fix"

MyStackViewPage {
    headerText: qsTr("Utilities")

    // Every group box row on this page uses this height.
    readonly property int groupBoxRowHeight: 44

    content: ColumnLayout {
        spacing: 8

        SpaceFixGroupBox {
            id: spaceFixGroupBox
            rowHeight: groupBoxRowHeight
        }

        KeyboardGroupBox {
            id: keyboardGroupBox
            rowHeight: groupBoxRowHeight
        }

        AlarmGroupBox {
            id: alarmGroupBox
            rowHeight: groupBoxRowHeight
        }

        MediaControllerKeys {
            id: mediaKeysGroupBox
            rowHeight: groupBoxRowHeight
        }

        MiscBox{
            id: utilMiscGroupBox
            rowHeight: groupBoxRowHeight
        }

        Item {
            Layout.fillHeight: true
        }

    }
}
