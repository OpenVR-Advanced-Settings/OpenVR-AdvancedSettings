import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: heightToggleGroupBox
    Layout.fillWidth: true

    title: qsTr("Height Toggle / Gravity Floor Offset")

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true

            MyToggleButton {
                id: heightToggleButton
                text: qsTr("On/Off")
                onCheckedChanged: {
                    MoveCenterTabController.heightToggle = this.checked
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MyText {
                text: qsTr("Height Offset (+ is down)")
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
                text:qsTr("Set From Y-Offset")
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

        function onHeightToggleChanged() {
            heightToggleButton.checked = MoveCenterTabController.heightToggle
        }
        function onHeightToggleOffsetChanged() {
            heightToggleOffsetText.text = MoveCenterTabController.heightToggleOffset.toFixed(2)
        }
    }
}
