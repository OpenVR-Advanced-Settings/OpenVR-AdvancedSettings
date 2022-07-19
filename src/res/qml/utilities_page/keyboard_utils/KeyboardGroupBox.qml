import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

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
        RowLayout {
            spacing: 18
            Layout.fillWidth: true

            MyPushButton {
                text: "Ctrl-C"
                Layout.preferredWidth: 150
                onClicked: {
                    UtilitiesTabController.sendKeyboardCtrlC()
                }
            }

            MyPushButton {
                text: "Ctrl-V"
                Layout.preferredWidth: 150
                onClicked: {
                    UtilitiesTabController.sendKeyboardCtrlV()
                }
             }
            MyPushButton {
                text: "Custom Key 1"
                Layout.preferredWidth: 200
                onClicked: {
                    UtilitiesTabController.sendKeyboardOne();
                }
             }
            MyPushButton {
                text: "Custom Key 2"
                Layout.preferredWidth: 200
                onClicked: {
                    UtilitiesTabController.sendKeyboardTwo();
                }
             }
            MyPushButton {
                text: "Custom Key 3"
                Layout.preferredWidth: 200
                onClicked: {
                    UtilitiesTabController.sendKeyboardThree();
                }
             }

            Item {
                Layout.fillWidth: true
            }
        }
        RowLayout {
            spacing: 18
            Layout.fillWidth: true
            id: vrcDebugRow

            MyPushButton {
                id: rShiftTilde1Button
                text: "RSh~1"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardRShiftTilde1()
                    rShiftTilde1Button.enabled = false
                    rShiftTilde2Button.enabled = false
                    rShiftTilde3Button.enabled = false
                    rShiftTilde4Button.enabled = false
                    rShiftTilde5Button.enabled = false
                    rShiftTilde6Button.enabled = false
                    rShiftTilde7Button.enabled = false
                    rShiftTilde8Button.enabled = false
                    rShiftTilde9Button.enabled = false
                    rShiftTilde0Button.enabled = false
                    timer1.restart()
                }
            }

            MyPushButton {
                id: rShiftTilde2Button
                text: "RSh~2"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardRShiftTilde2()
                    rShiftTilde1Button.enabled = false
                    rShiftTilde2Button.enabled = false
                    rShiftTilde3Button.enabled = false
                    rShiftTilde4Button.enabled = false
                    rShiftTilde5Button.enabled = false
                    rShiftTilde6Button.enabled = false
                    rShiftTilde7Button.enabled = false
                    rShiftTilde8Button.enabled = false
                    rShiftTilde9Button.enabled = false
                    rShiftTilde0Button.enabled = false
                    timer2.restart()
                }
            }

            MyPushButton {
                id: rShiftTilde3Button
                text: "RSh~3"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardRShiftTilde3()
                    rShiftTilde1Button.enabled = false
                    rShiftTilde2Button.enabled = false
                    rShiftTilde3Button.enabled = false
                    rShiftTilde4Button.enabled = false
                    rShiftTilde5Button.enabled = false
                    rShiftTilde6Button.enabled = false
                    rShiftTilde7Button.enabled = false
                    rShiftTilde8Button.enabled = false
                    rShiftTilde9Button.enabled = false
                    rShiftTilde0Button.enabled = false
                    timer3.restart()
                }
            }

            MyPushButton {
                id: rShiftTilde4Button
                text: "RSh~4"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardRShiftTilde4()
                    rShiftTilde1Button.enabled = false
                    rShiftTilde2Button.enabled = false
                    rShiftTilde3Button.enabled = false
                    rShiftTilde4Button.enabled = false
                    rShiftTilde5Button.enabled = false
                    rShiftTilde6Button.enabled = false
                    rShiftTilde7Button.enabled = false
                    rShiftTilde8Button.enabled = false
                    rShiftTilde9Button.enabled = false
                    rShiftTilde0Button.enabled = false
                    timer4.restart()
                }
            }

            MyPushButton {
                id: rShiftTilde5Button
                text: "RSh~5"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardRShiftTilde5()
                    rShiftTilde1Button.enabled = false
                    rShiftTilde2Button.enabled = false
                    rShiftTilde3Button.enabled = false
                    rShiftTilde4Button.enabled = false
                    rShiftTilde5Button.enabled = false
                    rShiftTilde6Button.enabled = false
                    rShiftTilde7Button.enabled = false
                    rShiftTilde8Button.enabled = false
                    rShiftTilde9Button.enabled = false
                    rShiftTilde0Button.enabled = false
                    timer5.restart()
                }
            }

            MyPushButton {
                id: rShiftTilde6Button
                text: "RSh~6"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardRShiftTilde6()
                    rShiftTilde1Button.enabled = false
                    rShiftTilde2Button.enabled = false
                    rShiftTilde3Button.enabled = false
                    rShiftTilde4Button.enabled = false
                    rShiftTilde5Button.enabled = false
                    rShiftTilde6Button.enabled = false
                    rShiftTilde7Button.enabled = false
                    rShiftTilde8Button.enabled = false
                    rShiftTilde9Button.enabled = false
                    rShiftTilde0Button.enabled = false
                    timer6.restart()
                }
            }

            MyPushButton {
                id: rShiftTilde7Button
                text: "RSh~7"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardRShiftTilde7()
                    rShiftTilde1Button.enabled = false
                    rShiftTilde2Button.enabled = false
                    rShiftTilde3Button.enabled = false
                    rShiftTilde4Button.enabled = false
                    rShiftTilde5Button.enabled = false
                    rShiftTilde6Button.enabled = false
                    rShiftTilde7Button.enabled = false
                    rShiftTilde8Button.enabled = false
                    rShiftTilde9Button.enabled = false
                    rShiftTilde0Button.enabled = false
                    timer7.restart()
                }
            }

            MyPushButton {
                id: rShiftTilde8Button
                text: "RSh~8"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardRShiftTilde8()
                    rShiftTilde1Button.enabled = false
                    rShiftTilde2Button.enabled = false
                    rShiftTilde3Button.enabled = false
                    rShiftTilde4Button.enabled = false
                    rShiftTilde5Button.enabled = false
                    rShiftTilde6Button.enabled = false
                    rShiftTilde7Button.enabled = false
                    rShiftTilde8Button.enabled = false
                    rShiftTilde9Button.enabled = false
                    rShiftTilde0Button.enabled = false
                    timer8.restart()
                }
            }

            MyPushButton {
                id: rShiftTilde9Button
                text: "RSh~9"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardRShiftTilde9()
                    rShiftTilde1Button.enabled = false
                    rShiftTilde2Button.enabled = false
                    rShiftTilde3Button.enabled = false
                    rShiftTilde4Button.enabled = false
                    rShiftTilde5Button.enabled = false
                    rShiftTilde6Button.enabled = false
                    rShiftTilde7Button.enabled = false
                    rShiftTilde8Button.enabled = false
                    rShiftTilde9Button.enabled = false
                    rShiftTilde0Button.enabled = false
                    timer9.restart()
                }
            }

            MyPushButton {
                id: rShiftTilde0Button
                text: "RSh~0"
                Layout.fillWidth: true
                onClicked: {
                    UtilitiesTabController.sendKeyboardRShiftTilde0()
                    rShiftTilde1Button.enabled = false
                    rShiftTilde2Button.enabled = false
                    rShiftTilde3Button.enabled = false
                    rShiftTilde4Button.enabled = false
                    rShiftTilde5Button.enabled = false
                    rShiftTilde6Button.enabled = false
                    rShiftTilde7Button.enabled = false
                    rShiftTilde8Button.enabled = false
                    rShiftTilde9Button.enabled = false
                    rShiftTilde0Button.enabled = false
                    timer0.restart()
                }
            }

            Timer {
                id: timer1
                interval: 100
                repeat: false
                running: false
                triggeredOnStart: false

                onTriggered: {
                    UtilitiesTabController.sendKeyboardRShiftTilde1Delayed()
                    rShiftTilde1Button.enabled = true
                    rShiftTilde2Button.enabled = true
                    rShiftTilde3Button.enabled = true
                    rShiftTilde4Button.enabled = true
                    rShiftTilde5Button.enabled = true
                    rShiftTilde6Button.enabled = true
                    rShiftTilde7Button.enabled = true
                    rShiftTilde8Button.enabled = true
                    rShiftTilde9Button.enabled = true
                    rShiftTilde0Button.enabled = true
                }

            }

            Timer {
                id: timer2
                interval: 100
                repeat: false
                running: false
                triggeredOnStart: false

                onTriggered: {
                    UtilitiesTabController.sendKeyboardRShiftTilde2Delayed()
                    rShiftTilde1Button.enabled = true
                    rShiftTilde2Button.enabled = true
                    rShiftTilde3Button.enabled = true
                    rShiftTilde4Button.enabled = true
                    rShiftTilde5Button.enabled = true
                    rShiftTilde6Button.enabled = true
                    rShiftTilde7Button.enabled = true
                    rShiftTilde8Button.enabled = true
                    rShiftTilde9Button.enabled = true
                    rShiftTilde0Button.enabled = true
                }

            }

            Timer {
                id: timer3
                interval: 100
                repeat: false
                running: false
                triggeredOnStart: false

                onTriggered: {
                    UtilitiesTabController.sendKeyboardRShiftTilde3Delayed()
                    rShiftTilde1Button.enabled = true
                    rShiftTilde2Button.enabled = true
                    rShiftTilde3Button.enabled = true
                    rShiftTilde4Button.enabled = true
                    rShiftTilde5Button.enabled = true
                    rShiftTilde6Button.enabled = true
                    rShiftTilde7Button.enabled = true
                    rShiftTilde8Button.enabled = true
                    rShiftTilde9Button.enabled = true
                    rShiftTilde0Button.enabled = true
                }

            }

            Timer {
                id: timer4
                interval: 100
                repeat: false
                running: false
                triggeredOnStart: false

                onTriggered: {
                    UtilitiesTabController.sendKeyboardRShiftTilde4Delayed()
                    rShiftTilde1Button.enabled = true
                    rShiftTilde2Button.enabled = true
                    rShiftTilde3Button.enabled = true
                    rShiftTilde4Button.enabled = true
                    rShiftTilde5Button.enabled = true
                    rShiftTilde6Button.enabled = true
                    rShiftTilde7Button.enabled = true
                    rShiftTilde8Button.enabled = true
                    rShiftTilde9Button.enabled = true
                    rShiftTilde0Button.enabled = true
                }

            }

            Timer {
                id: timer5
                interval: 100
                repeat: false
                running: false
                triggeredOnStart: false

                onTriggered: {
                    UtilitiesTabController.sendKeyboardRShiftTilde5Delayed()
                    rShiftTilde1Button.enabled = true
                    rShiftTilde2Button.enabled = true
                    rShiftTilde3Button.enabled = true
                    rShiftTilde4Button.enabled = true
                    rShiftTilde5Button.enabled = true
                    rShiftTilde6Button.enabled = true
                    rShiftTilde7Button.enabled = true
                    rShiftTilde8Button.enabled = true
                    rShiftTilde9Button.enabled = true
                    rShiftTilde0Button.enabled = true
                }

            }

            Timer {
                id: timer6
                interval: 100
                repeat: false
                running: false
                triggeredOnStart: false

                onTriggered: {
                    UtilitiesTabController.sendKeyboardRShiftTilde6Delayed()
                    rShiftTilde1Button.enabled = true
                    rShiftTilde2Button.enabled = true
                    rShiftTilde3Button.enabled = true
                    rShiftTilde4Button.enabled = true
                    rShiftTilde5Button.enabled = true
                    rShiftTilde6Button.enabled = true
                    rShiftTilde7Button.enabled = true
                    rShiftTilde8Button.enabled = true
                    rShiftTilde9Button.enabled = true
                    rShiftTilde0Button.enabled = true
                }

            }

            Timer {
                id: timer7
                interval: 100
                repeat: false
                running: false
                triggeredOnStart: false

                onTriggered: {
                    UtilitiesTabController.sendKeyboardRShiftTilde7Delayed()
                    rShiftTilde1Button.enabled = true
                    rShiftTilde2Button.enabled = true
                    rShiftTilde3Button.enabled = true
                    rShiftTilde4Button.enabled = true
                    rShiftTilde5Button.enabled = true
                    rShiftTilde6Button.enabled = true
                    rShiftTilde7Button.enabled = true
                    rShiftTilde8Button.enabled = true
                    rShiftTilde9Button.enabled = true
                    rShiftTilde0Button.enabled = true
                }

            }

            Timer {
                id: timer8
                interval: 100
                repeat: false
                running: false
                triggeredOnStart: false

                onTriggered: {
                    UtilitiesTabController.sendKeyboardRShiftTilde8Delayed()
                    rShiftTilde1Button.enabled = true
                    rShiftTilde2Button.enabled = true
                    rShiftTilde3Button.enabled = true
                    rShiftTilde4Button.enabled = true
                    rShiftTilde5Button.enabled = true
                    rShiftTilde6Button.enabled = true
                    rShiftTilde7Button.enabled = true
                    rShiftTilde8Button.enabled = true
                    rShiftTilde9Button.enabled = true
                    rShiftTilde0Button.enabled = true
                }

            }

            Timer {
                id: timer9
                interval: 100
                repeat: false
                running: false
                triggeredOnStart: false

                onTriggered: {
                    UtilitiesTabController.sendKeyboardRShiftTilde9Delayed()
                    rShiftTilde1Button.enabled = true
                    rShiftTilde2Button.enabled = true
                    rShiftTilde3Button.enabled = true
                    rShiftTilde4Button.enabled = true
                    rShiftTilde5Button.enabled = true
                    rShiftTilde6Button.enabled = true
                    rShiftTilde7Button.enabled = true
                    rShiftTilde8Button.enabled = true
                    rShiftTilde9Button.enabled = true
                    rShiftTilde0Button.enabled = true
                }

            }

            Timer {
                id: timer0
                interval: 100
                repeat: false
                running: false
                triggeredOnStart: false

                onTriggered: {
                    UtilitiesTabController.sendKeyboardRShiftTilde0Delayed()
                    rShiftTilde1Button.enabled = true
                    rShiftTilde2Button.enabled = true
                    rShiftTilde3Button.enabled = true
                    rShiftTilde4Button.enabled = true
                    rShiftTilde5Button.enabled = true
                    rShiftTilde6Button.enabled = true
                    rShiftTilde7Button.enabled = true
                    rShiftTilde8Button.enabled = true
                    rShiftTilde9Button.enabled = true
                    rShiftTilde0Button.enabled = true
                }

            }

        }
    }
    Component.onCompleted: {
        keyboardGroupBox.visible = !OverlayController.desktopMode
        vrcDebugRow.visible = UtilitiesTabController.vrcDebug
    }
    Connections {
        target: UtilitiesTabController
        onVrcDebugChanged: {
            vrcDebugRow.visible = UtilitiesTabController.vrcDebug
        }
    }
}
