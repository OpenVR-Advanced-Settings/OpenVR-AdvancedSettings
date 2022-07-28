import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: cameraGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Camera: (requires SteamVR restart)"
        bottomPadding: -10
    }
    background: Rectangle {
        color: "transparent"
        border.color: "#d9dbe0"
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            color: "#d9dbe0"
            height: 1
            Layout.fillWidth: true
            Layout.bottomMargin: 5
        }

        RowLayout {
            spacing: 16

            MyToggleButton {
                id: steamvrCameraActiveToggle
                text: "Enable Camera"
                Layout.preferredWidth: 400
                onCheckedChanged: {
                       SteamVRTabController.setCameraActive(this.checked, false)
                    if(this.checked){
                        steamvrCameraBoundsToggle.enabled = true
                        steamvrCameraDashboardToggle.enabled = true
                        steamvrCameraRoomToggle.enabled = true
                    }else{
                        steamvrCameraBoundsToggle.enabled = false
                        steamvrCameraDashboardToggle.enabled = false
                        steamvrCameraRoomToggle.enabled = false
                    }
                }
            }


        }
        RowLayout {
            spacing: 16

            MyToggleButton {
                id: steamvrCameraBoundsToggle
                text: "Camera for Bounds"
                Layout.preferredWidth: 300
                onCheckedChanged: {
                       SteamVRTabController.setCameraBounds(this.checked, false)
                }
            }

            MyToggleButton {
                id: steamvrCameraDashboardToggle
                text: "Camera for Dashboard"
                Layout.preferredWidth: 300
                onCheckedChanged: {
                    SteamVRTabController.setCameraDashboard(this.checked, false)
                }
            }

            MyToggleButton {
                id: steamvrCameraRoomToggle
                text: "Camera for Room View"
                onCheckedChanged: {
                    SteamVRTabController.setCameraRoom(this.checked, false)
                }
            }
        }

    }

    Component.onCompleted: {
        var c1 = SteamVRTabController.cameraActive
        steamvrCameraActiveToggle.checked = c1
        steamvrCameraBoundsToggle.checked = SteamVRTabController.cameraBounds
        steamvrCameraDashboardToggle.checked = SteamVRTabController.cameraDashboard
        steamvrCameraRoomToggle.checked = SteamVRTabController.cameraRoom

        if(!c1){
            steamvrCameraDashboardToggle.enabled = false;
            steamvrCameraBoundsToggle.enabled = false;
            steamvrCameraRoomToggle.enabled = false;
        }

    }

    Connections {
        target: SteamVRTabController
        onCameraActiveChanged: {
            steamvrCameraActiveToggle.checked = SteamVRTabController.cameraActive
        }
        onCameraRoomChanged: {
            steamvrCameraRoomToggle.checked = SteamVRTabController.cameraRoom
        }
        onCameraDashboardChanged: {
            steamvrCameraDashboardToggle.checked = SteamVRTabController.cameraDashboard
        }
        onCameraBoundsChanged: {
            steamvrCameraBoundsToggle.checked = SteamVRTabController.cameraBounds
        }


    }
}
