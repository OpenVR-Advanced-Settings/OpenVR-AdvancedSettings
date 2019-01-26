import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
import "media_keys"
import ".."

GroupBox {
    id: keyboardGroupBox
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
            Layout.fillWidth: true
            
            MyPushButton {
                text: "Keyboard Input"
                Layout.fillWidth: true
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
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardEnter()
                }
            }
            
            MyPushButton {
                text: "Alt-Tab"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardAltTab()
                }
            }
            
            MyPushButton {
                text: "Backspace"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardBackspace(1)
                }
            }
            
            MyPushButton {
                text: "10x Backspace"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardBackspace(10)
                }
            }
            
            MyPushButton {
                text: "Alt-Enter"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardAltEnter()
                }
            }
        }
    }
    Component.onCompleted: {
        keyboardGroupBox.visible = !OverlayController.desktopMode
    }
}
