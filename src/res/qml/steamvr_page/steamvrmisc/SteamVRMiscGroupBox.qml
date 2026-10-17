import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: steamVRMiscGroupBox
    Layout.fillWidth: true

    title: qsTr("General")

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        MyToggleButton {
            id: steamvrPerformanceGraphToggle
            Layout.preferredHeight: rowHeight
            text: qsTr("Timing Overlay")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                SteamVRTabController.setPerformanceGraph(this.checked, false)
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: steamvrNoHMDToggle
            Layout.preferredHeight: rowHeight
            text: qsTr("Require HMD")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                SteamVRTabController.setNoHMD(this.checked, false)
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: steamvrNoFadeToGridToggle
            Layout.preferredHeight: rowHeight
            text: qsTr("No Fade to Grid")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                SteamVRTabController.setNoFadeToGrid(this.checked, false)
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: steamvrMultipleDriverToggle
            Layout.preferredHeight: rowHeight
            text: qsTr("Allow Multiple Drivers")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                SteamVRTabController.setMultipleDriver(this.checked, false)
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: steamvrSystemButtonToggle
            Layout.preferredHeight: rowHeight
            text: qsTr("System Button Binding")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                SteamVRTabController.setSystemButton(this.checked, false)
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: steamvrNotificationToggle
            Layout.preferredHeight: rowHeight
            text: qsTr("Disable Notifications")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                SteamVRTabController.setDND(this.checked, false)
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: steamvrControllerPowerToggle
            Layout.preferredHeight: rowHeight
            text: qsTr("Controller Power Turns on SteamVR")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                SteamVRTabController.setControllerPower(this.checked, false)
            }
        }
    }

    Component.onCompleted: {
            steamvrPerformanceGraphToggle.checked = SteamVRTabController.performanceGraph
        steamvrSystemButtonToggle.checked = SteamVRTabController.systemButton
        steamvrMultipleDriverToggle.checked = SteamVRTabController.multipleDriver
        steamvrNoFadeToGridToggle.checked = SteamVRTabController.noFadeToGrid
        steamvrNotificationToggle.checked = SteamVRTabController.dnd
        steamvrControllerPowerToggle.checked = SteamVRTabController.controllerPower
        steamvrNoHMDToggle.checked = SteamVRTabController.noHMD
    }

    Connections {
        target: SteamVRTabController
        function onPerformanceGraphChanged(){
            steamvrPerformanceGraphToggle.checked = SteamVRTabController.performanceGraph
        }
        function onSystemButtonChanged(){
            steamvrSystemButtonToggle.checked = SteamVRTabController.systemButton
        }
        function onMultipleDriverChanged(){
            steamvrMultipleDriverToggle.checked = SteamVRTabController.multipleDriver
        }
        function onNoFadeToGridChanged(){
            steamvrNoFadeToGridToggle.checked = SteamVRTabController.noFadeToGrid
        }
        function onDNDChanged(){
            steamvrNotificationToggle.checked = SteamVRTabController.dnd
        }
        function onNoHMDChanged(){
            steamvrNoHMDToggle.checked = SteamVRTabController.noHMD
        }
        function onControllerPowerChanged(){
            steamvrControllerPowerToggle.checked = SteamVRTabController.controllerPower
        }

    }
}
