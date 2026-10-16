import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: spaceTurnGroupBox
    Layout.fillWidth: true

    title: qsTr("Space Turn (for manual redirected walking)")

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true

            MyToggleButton {
                id: turnBindLeft
                text: qsTr("Left Hand")
                onCheckedChanged: {
                    MoveCenterTabController.turnBindLeft = this.checked
                }
            }

            MyToggleButton {
                id: turnBindRight
                text: qsTr("Right Hand")
                onCheckedChanged: {
                    MoveCenterTabController.turnBindRight = this.checked
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MyText {
                text: qsTr("Comfort Mode:")
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MySlider {
                id: turnComfortSlider
                from: 0
                to: 10
                stepSize: 1
                value: 0
                Layout.preferredWidth: 180
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                onValueChanged: {
                    turnComfortText.text = turnComfortSlider.value
                    MoveCenterTabController.turnComfortFactor = turnComfortSlider.value
                }
            }

            MyText {
                id: turnComfortText
                text: "0"
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: 30
                Layout.rightMargin: 10
            }

            MyToggleButton {
                id: turnBounds
                text: qsTr("Force Bounds")
                onCheckedChanged: {
                    MoveCenterTabController.turnBounds = this.checked
                }
            }
        }
    }

    Component.onCompleted: {
        turnBindLeft.checked = MoveCenterTabController.turnBindLeft
        turnBindRight.checked = MoveCenterTabController.turnBindRight
        turnComfortSlider.value = MoveCenterTabController.turnComfortFactor
        turnBounds.checked = MoveCenterTabController.turnBounds
    }

    Connections {
        target: MoveCenterTabController

        function onTurnBindLeftChanged() {
            turnBindLeft.checked = MoveCenterTabController.turnBindLeft
        }
        function onTurnBindRightChanged() {
            turnBindRight.checked = MoveCenterTabController.turnBindRight
        }
        function onTurnComfortFactorChanged() {
            turnComfortSlider.value = MoveCenterTabController.turnComfortFactor
        }
        function onTurnBoundsChanged() {
            turnBounds.checked = MoveCenterTabController.turnBounds
        }
    }
}
