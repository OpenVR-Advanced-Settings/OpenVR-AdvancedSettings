import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import ovras.advsettings 1.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
import "../../common"



MyStackViewPage {
    ListModel{
        id: listModelRXTX
        ListElement{
            TX:"Controller"
            RX:"Reciever"
        }
    }
    headerText: "SteamVR Device Pairing Information"
    content:
        ColumnLayout {
                spacing: 10
                Layout.fillWidth: true
                Layout.fillHeight: true
                MyPushButton{
                    text: "Search"
                    id: searchButton
                    onClicked: {
                        getRXTX()
                    }

                }
                RowLayout{
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    ScrollView{
                        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                        ListView{
                            model: listModelRXTX
                            spacing: 20
                            delegate:
                                GroupBox {
                                    id: chaperoneTypeGroupBox
                                    Layout.fillWidth: true

                                    label: MyText {
                                        leftPadding: 10
                                        text: "Device"
                                        bottomPadding: -10
                                    }
                                    background: Rectangle {
                                        color: "transparent"
                                        border.color: "#ffffff"
                                        radius: 8
                                    }
                                ColumnLayout{
                                    anchors.fill: parent

                                    Rectangle {
                                        color: "#ffffff"
                                        height: 1
                                        Layout.fillWidth: true
                                        Layout.bottomMargin: 5
                                    }
                                    RowLayout{
                                        Layout.fillHeight: true
                                        Layout.fillWidth: true
                                        clip: true
                                        MyText{
                                            Layout.preferredWidth: 300
                                            text: "Connected Dongle Type: "
                                        }
                                        Item{
                                            Layout.preferredWidth: 150
                                        }
                                        MyText{
                                            Layout.preferredWidth: 250
                                            text: "TODO"
                                        }
                                    }
                                    RowLayout{
                                        Layout.fillHeight: true
                                        Layout.fillWidth: true
                                        clip: true
                                        MyText{
                                            Layout.preferredWidth: 150
                                            text: "Device ID: "
                                        }
                                        MyText{
                                            Layout.preferredWidth: 250
                                            text: TX
                                        }
                                        Item{
                                            Layout.preferredWidth: 200
                                        }
                                        MyText{
                                            Layout.preferredWidth: 150
                                            text: "Dongle ID: "
                                        }
                                        MyText{
                                            Layout.preferredWidth: 250
                                            text: RX
                                        }
                                    }
                                }
                            }
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                    }
                }
        }

    function getRXTX() {
        listModelRXTX.clear();
        SteamVRTabController.searchRXTX();
         listModelRXTX.append({RX:"rec", TX:"Controller"})
        var pairCount = SteamVRTabController.getRXTXCount()
        for (var i = 0; i < pairCount; i++) {
            var RXget = SteamVRTabController.getRXList(i);
            var TXget = SteamVRTabController.getTXList(i);
            listModelRXTX.append({RX:RXget, TX:TXget})
        }
    }
}



    //TODO content

