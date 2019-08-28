import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: steamVRGroupBox
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
                id: steamvrPerformanceGraphToggle
                text: "Enable Timing Overlay"
                onCheckedChanged: {
                    SteamVRTabController.setPerformanceGraph(this.checked, false)
                }
            }
            MyText {
                Layout.preferredWidth: 20
                text: " "
            }

            MyToggleButton {
                id: steamvrSystemButtonToggle
                text: "Enable System Button Binding"
                onCheckedChanged: {
                    SteamVRTabController.setSystemButton(this.checked, false)
                }
            }
        }
        RowLayout {
            spacing: 16

            MyToggleButton {
                id: steamvrMultipleDriverToggle
                text: "Allow Multiple Drivers"
                onCheckedChanged: {
                    SteamVRTabController.setMultipleDriver(this.checked, false)
                }
            }
            MyText {
                Layout.preferredWidth: 20
                text: " "
            }

            MyToggleButton {
                id: steamvrNoFadeToGridToggle
                text: "No Fade to Grid"
                onCheckedChanged: {
                    SteamVRTabController.setNoFadeToGrid(this.checked, false)
                }
            }
        }

    }

    Component.onCompleted: {
            steamvrPerformanceGraphToggle.checked = SteamVRTabController.performanceGraph
        steamvrSystemButtonToggle.checked = SteamVRTabController.systemButton
        steamvrMultipleDriverToggle.checked = SteamVRTabController.multipleDriver
        steamvrNoFadeToGridToggle.checked = SteamVRTabController.noFadeToGrid

    }

    Connections {
        target: SteamVRTabController
        onPerformanceGraphChanged:{
            steamvrPerformanceGraphToggle.checked = SteamVRTabController.performanceGraph
        }
        onSystemButtonChanged:{
            steamvrSystemButtonToggle.checked = SteamVRTabController.systemButton
        }
        onMultipleDriverChanged:{
            steamvrMultipleDriverToggle.checked = SteamVRTabController.multipleDriver
        }
        onNoFadeToGridChanged:{
            steamvrNoFadeToGridToggle.checked = SteamVRTabController.noFadeToGrid
        }

    }
}
