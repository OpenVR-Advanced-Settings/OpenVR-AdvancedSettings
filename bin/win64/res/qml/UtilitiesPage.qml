import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0


MyStackViewPage {
    headerText: "Utilities"

    content: ColumnLayout {
        spacing: 18


        GroupBox {
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Keyboard Utilities"
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


                MyText {
                    text: "Send to active application: "
                }

                RowLayout {
                    spacing: 18

                    MyPushButton {
                        text: "VR Keyboard Input"
                        Layout.preferredWidth: 260
                        onClicked: {
                            OverlayController.showKeyboard("", 601)
                        }
                        Connections {
                            target: OverlayController
                            onKeyBoardInputSignal: {
                                if (userValue == 601) {
                                    UtilitiesTabController.sendKeyboardInput(input)
                                }
                            }
                        }
                    }

                    MyPushButton {
                        text: "Enter"
                        Layout.preferredWidth: 260
                        onClicked: {
                            UtilitiesTabController.sendKeyboardEnter()
                        }
                    }

                    MyPushButton {
                        text: "Backspace"
                        Layout.preferredWidth: 260
                        onClicked: {
                            UtilitiesTabController.sendKeyboardBackspace(1)
                        }
                    }

                    MyPushButton {
                        text: "10x Backspace"
                        Layout.preferredWidth: 260
                        onClicked: {
                            UtilitiesTabController.sendKeyboardBackspace(10)
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        /*Component.onCompleted: {
        }

        Connections {
        }*/
    }
}
