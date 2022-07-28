import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: heightToggleGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Height Toggle / Gravity Floor Offset"
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

        RowLayout {
            Layout.fillWidth: true

            MyToggleButton {
                id: heightToggleButton
                text: "On"
                onCheckedChanged: {
                    MoveCenterTabController.heightToggle = this.checked
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MyText {
                text: "Height Offset (+ is down):"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 2
            }

            MyTextField {
                id: heightToggleOffsetText
                text: "-1.00"
                keyBoardUID: 151
                Layout.preferredWidth: 140
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        MoveCenterTabController.heightToggleOffset = val.toFixed(2)
                        text = MoveCenterTabController.heightToggleOffset.toFixed(2)
                    } else {
                        text = MoveCenterTabController.heightToggleOffset.toFixed(2)
                    }
                }
            }

            MyPushButton {
                id: heightOffsetFromYOffsetButton
                Layout.preferredWidth: 230
                text:"Set From Y-Offset"
                onClicked: {
                    MoveCenterTabController.heightToggleOffset = MoveCenterTabController.offsetY
                }
           }
        }
    }

    Component.onCompleted: {
        heightToggleButton.checked = MoveCenterTabController.heightToggle
        heightToggleOffsetText.text = MoveCenterTabController.heightToggleOffset.toFixed(2)
    }

    Connections {
        target: MoveCenterTabController

        onHeightToggleChanged: {
            heightToggleButton.checked = MoveCenterTabController.heightToggle
        }
        onHeightToggleOffsetChanged: {
            heightToggleOffsetText.text = MoveCenterTabController.heightToggleOffset.toFixed(2)
        }
    }
}
