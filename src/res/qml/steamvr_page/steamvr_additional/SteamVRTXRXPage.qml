import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
import "../../common"

MyStackViewPage {
    headerText: qsTr("Device Pairing Information")

    ListModel {
        id: listModelRXTX
    }

    content: ColumnLayout {
        spacing: 16

        SettingsGroupBox {
            id: pairingStatusGroupBox
            Layout.fillWidth: true
            title: qsTr("Pairing")

            ColumnLayout {
                anchors.fill: parent

                RowLayout {
                    Layout.fillWidth: true
                    MyText {
                        text: qsTr("Status:")
                        Layout.preferredWidth: 250
                    }
                    MyText {
                        id: statusText
                        text: qsTr("Ready to Pair")
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                    }
                }

                LineSeparator {
                }

                RowLayout {
                    Layout.fillWidth: true
                    MyText {
                        text: qsTr("Dongles Used:")
                        Layout.preferredWidth: 250
                    }
                    MyText {
                        id: donglesUse
                        text: ""
                        Layout.fillWidth: true
                    }
                    MyPushButton {
                        id: searchButton
                        text: qsTr("Refresh Device List")
                        Layout.preferredWidth: 300
                        onClicked: {
                            getRXTX()
                        }
                    }
                }

                LineSeparator {
                }

                MyText {
                    text: qsTr("Set the device to pairing mode, then hit Pair on the desired unconnected dongle. To pair to a dongle that is currently in use, turn off the attached device and refresh the list.")
                    wrapMode: Text.WordWrap
                    color: "#8b929a"
                    Layout.fillWidth: true
                }
            }
        }

        Repeater {
            model: listModelRXTX
            delegate: SettingsGroupBox {
                Layout.fillWidth: true
                title: Dev

                ColumnLayout {
                    id: deviceInfoLayout
                    anchors.fill: parent
                    spacing: 8

                    // Each row is split into two fixed halves so the right half
                    // always starts at the center, regardless of text length.
                    property real halfWidth: width / 2
                    property real labelWidth: 160
                    property real valueWidth: halfWidth - labelWidth - 32

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 0

                        RowLayout {
                            spacing: 16
                            Layout.preferredWidth: deviceInfoLayout.halfWidth
                            Layout.maximumWidth: deviceInfoLayout.halfWidth
                            MyText {
                                text: qsTr("Device ID:")
                                Layout.preferredWidth: deviceInfoLayout.labelWidth
                            }
                            MyText {
                                text: TX
                                elide: Text.ElideRight
                                Layout.preferredWidth: deviceInfoLayout.valueWidth
                                Layout.maximumWidth: deviceInfoLayout.valueWidth
                            }
                            Item {
                                Layout.fillWidth: true
                            }
                        }

                        RowLayout {
                            spacing: 16
                            Layout.preferredWidth: deviceInfoLayout.halfWidth
                            Layout.maximumWidth: deviceInfoLayout.halfWidth
                            MyText {
                                text: qsTr("Dongle ID:")
                                Layout.preferredWidth: deviceInfoLayout.labelWidth
                            }
                            MyText {
                                text: RX
                                elide: Text.ElideRight
                                Layout.preferredWidth: deviceInfoLayout.valueWidth
                                Layout.maximumWidth: deviceInfoLayout.valueWidth
                            }
                            Item {
                                Layout.fillWidth: true
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 0

                        RowLayout {
                            spacing: 16
                            Layout.preferredWidth: deviceInfoLayout.halfWidth
                            Layout.maximumWidth: deviceInfoLayout.halfWidth
                            MyText {
                                text: qsTr("Dongle Type:")
                                Layout.preferredWidth: deviceInfoLayout.labelWidth
                            }
                            MyText {
                                text: DT
                                elide: Text.ElideRight
                                Layout.preferredWidth: deviceInfoLayout.valueWidth
                                Layout.maximumWidth: deviceInfoLayout.valueWidth
                            }
                            Item {
                                Layout.fillWidth: true
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        MyPushButton {
                            text: qsTr("Pair")
                            enabled: Dev === "No Connection"
                            Layout.preferredWidth: 200
                            onClicked: {
                                SteamVRTabController.pairDevice(RX)
                            }
                        }
                    }
                }
            }
        }

        MyText {
            text: qsTr("No dongles found.")
            visible: listModelRXTX.count === 0
            color: "#8b929a"
            Layout.leftMargin: 2
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    Component.onCompleted: {
        getRXTX()
    }

    Connections {
        target: SteamVRTabController
        function onPairStatusChanged(value) {
            statusText.text = value
            getRXTX()
        }
        function onUpdateRXTX() {
            getRXTX()
        }
    }

    function getRXTX() {
        listModelRXTX.clear()
        SteamVRTabController.searchRXTX()
        donglesUse.text = SteamVRTabController.getDongleUsage()
        var pairCount = SteamVRTabController.getRXTXCount()
        for (var i = 0; i < pairCount; i++) {
            var RXget = SteamVRTabController.getRXList(i)
            var TXget = SteamVRTabController.getTXList(i)
            var nameGet = SteamVRTabController.getDeviceName(i)
            var dongletyp = SteamVRTabController.getDongleType(i)
            listModelRXTX.append({RX: RXget, TX: TXget, Dev: nameGet, DT: dongletyp})
        }
    }
}
