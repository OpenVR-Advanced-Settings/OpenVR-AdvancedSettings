import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: utilMiscGroupBox
    Layout.fillWidth: true

    title: qsTr("Misc")

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        MyToggleButton {
            id: trackerOvlToggle
            text: qsTr("Show Tracker Batteries")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                UtilitiesTabController.setTrackerOvlEnabled(this.checked, false)
            }
        }
    }

    Component.onCompleted: {
            trackerOvlToggle.checked = UtilitiesTabController.trackerOvlEnabled
    }

    Connections {
        target: UtilitiesTabController
        function onTrackerOvlEnabledChanged(){
            trackerOvlToggle.checked = UtilitiesTabController.trackerOvlEnabled
        }

    }
}

