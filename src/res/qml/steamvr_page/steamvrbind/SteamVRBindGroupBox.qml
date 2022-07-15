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
                id: steamvrBindingToggle
                text: "Enable Per-App Bindings For Overlays"
                Layout.preferredWidth: 300
                onCheckedChanged: {
                    //SteamVRTabController.setPerformanceGraph(this.checked, false)
                }
            }
            MyText {
                Layout.preferredWidth: 20
                text: " "
            }

            MyText {
                text: "Application: "
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
                        //TODO
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
                        //TODO
                    }
            }
        }

    }

    Component.onCompleted: {
            steamvrPerformanceGraphToggle.checked = SteamVRTabController.performanceGraph
    }

    Connections {
        target: SteamVRTabController
        onPerformanceGraphChanged:{
            steamvrPerformanceGraphToggle.checked = SteamVRTabController.performanceGraph
        }

    }
}
