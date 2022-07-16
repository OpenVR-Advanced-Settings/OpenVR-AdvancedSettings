import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "camera"
import "steamvrmisc"
import "steamvrbind"

MyStackViewPage {
    width: 1200
    headerText: "SteamVR"

    content: ColumnLayout {
        spacing: 16

        SteamVRMiscGroupBox {
        id: steamVRMiscGroupBox}
        CameraGroupBox {
        id:  cameraGroupBox}
        SteamVRBindGroupBox{
            id: steamVRBindGroupBox
        }

        Item { Layout.fillHeight: true; Layout.fillWidth: true}

        RowLayout {
            Layout.fillWidth: true

            MyPushButton {
                text: "Device Pairing Information"
                Layout.preferredWidth: 350

                onClicked: {
                    MyResources.playFocusChangedSound()
                    mainView.push(steamVRTXRXPage)
                    SteamVRTabController.updateRXTXList()
                }
            }

            Item { Layout.fillWidth: true}

            MyPushButton {
                id: steamVRRestartButton
                text: "Restart SteamVR"
                Layout.preferredWidth: 250
                onClicked: {
                    SteamVRTabController.restartSteamVR()
                }
            }
        }
    }
}
