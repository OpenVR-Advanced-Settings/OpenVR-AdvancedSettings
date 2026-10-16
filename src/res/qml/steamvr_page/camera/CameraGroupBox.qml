import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: cameraGroupBox
    Layout.fillWidth: true

    title: qsTr("Camera: (requires SteamVR restart)")

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        MyToggleButton {
            id: steamvrCameraActiveToggle
            text: qsTr("Enable Camera")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                SteamVRTabController.setCameraActive(this.checked, false)
                if(this.checked){
                    steamvrCameraBoundsToggle.enabled = true
                    steamvrCameraContToggle.enabled = true
                }else{
                    steamvrCameraBoundsToggle.enabled = false
                    steamvrCameraContToggle.enabled = false
                }
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: steamvrCameraBoundsToggle
            text: qsTr("Show camera on bounds collision")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                SteamVRTabController.setCameraBounds(this.checked, false)
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: steamvrCameraContToggle
            text: qsTr("Show camera on controller")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                SteamVRTabController.setCameraCont(this.checked, false)
            }
        }
    }

    Component.onCompleted: {
        var c1 = SteamVRTabController.cameraActive
        steamvrCameraActiveToggle.checked = c1
        steamvrCameraBoundsToggle.checked = SteamVRTabController.cameraBounds
        steamvrCameraContToggle.checked = SteamVRTabController.cameraCont

        if(!c1){

            steamvrCameraBoundsToggle.enabled = false;
            steamvrCameraContToggle.enabled = false;
        }

    }

    Connections {
        target: SteamVRTabController
        function onCameraActiveChanged() {
            steamvrCameraActiveToggle.checked = SteamVRTabController.cameraActive
        }
        function onCameraContChanged() {
            steamvrCameraContToggle.checked = SteamVRTabController.cameraCont
        }
        function onCameraBoundsChanged() {
            steamvrCameraBoundsToggle.checked = SteamVRTabController.cameraBounds
        }


    }
}
