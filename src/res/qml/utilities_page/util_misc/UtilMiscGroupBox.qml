import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: steamVRMiscGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Misc:"
        bottomPadding: -10
    }
    background: Rectangle {
        color: "transparent"
        border.color: "#ffffff"
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            color: "#ffffff"
            height: 1
            Layout.fillWidth: true
            Layout.bottomMargin: 5
        }

        RowLayout {
            spacing: 16

            MyToggleButton {
                id: utilitiesEnableExclusiveToggle
                text: "Enable Exclusive Input (Caution):"
                onCheckedChanged: {
                    UtilitiesTabController.setEnableExclusiveInput(this.checked, false)
                }
            }

        }

    }

    Component.onCompleted: {
            utilitiesEnableExclusiveToggle.checked = UtilitiesTabController.enableExclusiveInput
    }

    Connections {
        target: UtilitiesTabController
        onEnableExclusiveInputChanged:{
           utilitiesEnableExclusiveToggle.checked = UtilitiesTabController.enableExclusiveInput
        }


    }
}
