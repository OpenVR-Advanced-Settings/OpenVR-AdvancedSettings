import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: steamVRBindGroupBox
    Layout.fillWidth: true

    title: "Per-Game Bindings"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        MyToggleButton {
            id: steamvrBindingToggle
            text: "Enable"
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                SteamVRTabController.setPerAppBindEnabled(this.checked, false)
                if(!steamvrBindingToggle.checked){
                    appSelect.enabled = false;
                    setDefaultBtn.enabled = false;
                    setBinding.enabled = false;
                }else{
                    appSelect.enabled = true;
                    setDefaultBtn.enabled = true;
                    setBinding.enabled = true;
                }
            }
        }
        LineSeparator {
        }
        RowLayout {
            spacing: 16

            MyText {
                text: "Application:"
                horizontalAlignment: Text.AlignLeft
            }
            MyTextField {
                id: appSelect
                Layout.fillWidth: true
                text: "steam.overlay.1009850"
                keyBoardUID: 201
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    this.text = input
                }
            }
            MyPushButton {
                id: bindingsButton
                activationSoundEnabled: false
                text: "Open Bindings"
                Layout.preferredWidth: 180
                onClicked: {
                     SteamVRTabController.launchBindingUI()
                }
            }
        }
        LineSeparator {
        }
        RowLayout {
            spacing: 16

            MyText {
                text: "Save Current Binding"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
            }
            MyPushButton {
                id: setBinding
                text: "Game"
                onClicked: {
                    SteamVRTabController.setBindingQMLWrapper(appSelect.text)
                }
            }
            MyPushButton {
                id: setDefaultBtn
                text: "Default"
                onClicked: {
                    SteamVRTabController.setBindingQMLWrapper(appSelect.text,true)
                }
            }
        }
    }

    Component.onCompleted: {
            steamvrBindingToggle.checked = SteamVRTabController.perAppBindEnabled
        if(!steamvrBindingToggle.checked){
            appSelect.enabled = false;
            setDefaultBtn.enabled = false;
            setBinding.enabled = false;
        }else{
            appSelect.enabled = true;
            setDefaultBtn.enabled = true;
            setBinding.enabled = true;
        }
    }

    Connections {
        target: SteamVRTabController
        function onPerformanceGraphChanged() {
            steamvrBindingToggle.checked = SteamVRTabController.perAppBindEnabled
        }

    }
}
