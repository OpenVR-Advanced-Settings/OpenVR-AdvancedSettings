import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: snapTurnGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Snap Turn Angle"
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
            Layout.fillWidth: true

            MyTextField {
                id: snapTurnAngleText
                text: "45°"
                keyBoardUID: 154
                Layout.preferredWidth: 150
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        val = val % 180
                        MoveCenterTabController.snapTurnAngle = val.toFixed(2) * 100
                        text = ( Math.round( MoveCenterTabController.snapTurnAngle / 100 ) ) + "°"
                    } else {
                        text = ( Math.round( MoveCenterTabController.snapTurnAngle / 100 ) ) + "°"
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MyPushButton {
                id: snapTurnButton15
                Layout.preferredWidth: 90
                text:"15°"
                onClicked: {
                    MoveCenterTabController.snapTurnAngle = 1500
                }
           }
            MyPushButton {
                id: snapTurnButton30
                Layout.preferredWidth: 90
                text:"30°"
                onClicked: {
                    MoveCenterTabController.snapTurnAngle = 3000
                }
           }
            MyPushButton {
                id: snapTurnButton45
                Layout.preferredWidth: 90
                text:"45°"
                onClicked: {
                    MoveCenterTabController.snapTurnAngle = 4500
                }
           }
            MyPushButton {
                id: snapTurnButton90
                Layout.preferredWidth: 90
                text:"90°"
                onClicked: {
                    MoveCenterTabController.snapTurnAngle = 9000
                }
           }
            MyPushButton {
                id: snapTurnButton180
                Layout.preferredWidth: 90
                text:"180°"
                onClicked: {
                    MoveCenterTabController.snapTurnAngle = 18000
                }
           }
        }
    }

    Component.onCompleted: {
        snapTurnAngleText.text = ( Math.round( MoveCenterTabController.snapTurnAngle / 100 ) ) + "°"
    }

    Connections {
        target: MoveCenterTabController

        onSnapTurnAngleChanged: {
            snapTurnAngleText.text = ( Math.round( MoveCenterTabController.snapTurnAngle / 100 ) ) + "°"
        }
    }
}
