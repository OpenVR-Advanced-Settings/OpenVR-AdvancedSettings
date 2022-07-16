import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: steamVRBindGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Binds:"
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
                id: steamvrBindingToggle
                text: "Enable Per-App Bindings"
                Layout.preferredWidth: 350
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
            MyText {
                Layout.preferredWidth: 20
                text: " "
            }

            MyText {
                text: "Application: "
                Layout.preferredWidth: 200
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 2
            }
            MyTextField {
                id: appSelect
                Layout.fillWidth: true
                text: "steam.overlay.1009850"
                keyBoardUID: 201
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    this.text = input
                }
            }
        }
        RowLayout {
            spacing: 16
            MyPushButton {
                    id: setDefaultBtn
                    text:"Set Current Bind as Default"
                    onClicked: {
                        SteamVRTabController.setBindingQMLWrapper(appSelect.text,true)
                    }
            }
            MyText {
                Layout.fillWidth: true
                text: " "
            }
            MyPushButton {
                    id: setBinding
                    text:"Set Current Bind For Current App"
                    onClicked: {
                        SteamVRTabController.setBindingQMLWrapper(appSelect.text)
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
        onPerformanceGraphChanged:{
            steamvrBindingToggle.checked = SteamVRTabController.perAppBindEnabled
        }

    }
}
