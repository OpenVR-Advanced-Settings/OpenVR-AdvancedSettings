import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../../common"
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
GroupBox {
    id: chaperoneMiscGroupBox
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
                id: chaperoneFloorToggleBtn
                text: "Floor Bounds Always On"
                Layout.preferredWidth: 400
                onCheckedChanged: {
                    ChaperoneTabController.setChaperoneFloorToggle(this.checked, false)
                }
            }
            Item{
                Layout.preferredWidth: 20
            }

            MyToggleButton {
                id: legacyCenterMarkerbtn
                text: "Legacy Center Marker"
                Layout.preferredWidth: 400
                onCheckedChanged: {
                    ChaperoneTabController.setCenterMarker(this.checked, false)
                }
            }
        }

    }

    Component.onCompleted: {
            chaperoneFloorToggleBtn.checked = ChaperoneTabController.chaperoneFloorToggle
            legacyCenterMarkerbtn.checked = ChaperoneTabController.centermarker
    }

    Connections {
        target: ChaperoneTabController
        onChaperoneFloorToggleChanged:{
            chaperoneFloorToggleBtn.checked = ChaperoneTabController.chaperoneFloorToggle
        }
        onCenterMarkerChanged:{
            legacyCenterMarkerbtn.checked = ChaperoneTabController.centermarker
        }
    }
}
