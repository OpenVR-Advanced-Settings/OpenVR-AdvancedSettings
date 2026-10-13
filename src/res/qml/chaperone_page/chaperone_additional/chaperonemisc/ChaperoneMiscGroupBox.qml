import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../../common"
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
SettingsGroupBox {
    id: chaperoneMiscGroupBox
    Layout.fillWidth: true

    title: "Misc"

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            spacing: 16

            MyToggleButton {
                id: chaperoneFloorToggleBtn
                text: "Floor Bounds Always On"
                Layout.preferredWidth: 375
                onCheckedChanged: {
                    ChaperoneTabController.setChaperoneFloorToggle(this.checked, false)
                }
            }


            MyToggleButton {
                id: legacyCenterMarkerbtn
                text: "Legacy Center Marker"
                Layout.preferredWidth: 375
                onCheckedChanged: {
                    ChaperoneTabController.setCenterMarker(this.checked, false)
                }
            }

            MyPushButton{
                id:btnResetOrientation
                text: "Reset Turn Counter"
                onClicked: {
                    StatisticsTabController.statsRotationResetClicked()
                }
            }

        }

    }

    Component.onCompleted: {
            chaperoneFloorToggleBtn.checked = ChaperoneTabController.chaperoneFloorToggle
            legacyCenterMarkerbtn.checked = ChaperoneTabController.centerMarker
    }

    Connections {
        target: ChaperoneTabController
        function onChaperoneFloorToggleChanged(){
            chaperoneFloorToggleBtn.checked = ChaperoneTabController.chaperoneFloorToggle
        }
        function onCenterMarkerChanged(){
            legacyCenterMarkerbtn.checked = ChaperoneTabController.centerMarker
        }
    }
}
