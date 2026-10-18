import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    Layout.fillWidth: true
    id: rotateSpaceGroupBox
    visible: true

    title: qsTr("Rotate Space")

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        ColumnLayout {
            spacing: 0
            RowLayout {
                Layout.preferredHeight: rowHeight
                MyPushButton2 {
                    id: spaceRotationMinusButton
                    Layout.preferredWidth: 40
                    text: "-"
                    onClicked: {
                        var val = ( MoveCenterTabController.tempRotation / 100 ) - 45
                        if (val < -180) {
                            val = val + 360;
                        }
                        MoveCenterTabController.tempRotation = val * 100
                    }
                }

                MySlider {
                    id: spaceRotationSlider
                    from: -180
                    to: 180
                    stepSize: 0.01
                    value: 0
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = this.from + ( this.position  * (this.to - this.from))
                        MoveCenterTabController.tempRotation = Math.round(val * 100)
                    }
                    onValueChanged: {
                        MoveCenterTabController.tempRotation = Math.round(spaceRotationSlider.value * 100)
                        //spaceRotationText.text = Math.round(spaceRotationSlider.value) + "°"
                    }
                }

                MyPushButton2 {
                    id: spaceRotationPlusButton
                    Layout.preferredWidth: 40
                    text: "+"
                    onClicked: {
                        var val = ( MoveCenterTabController.tempRotation / 100 ) + 45
                        if (val > 180) {
                            val = val -360;
                        }
                        MoveCenterTabController.tempRotation = val * 100
                    }
                }

                MyTextField {
                    id: spaceRotationText
                    text: "0°"
                    keyBoardUID: 104
                    Layout.preferredWidth: 150
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseFloat(input)
                        if (!isNaN(val)) {
                            val = val % 180
                            MoveCenterTabController.tempRotation = Math.round(val * 100)
                            text = ( MoveCenterTabController.tempRotation / 100 ) + "°"
                        } else {
                            text = ( MoveCenterTabController.tempRotation / 100 ) + "°"
                        }
                    }
                }
                MyPushButton {
                    id: spaceRotationApplyButton
                    Layout.preferredWidth: 145
                    text:qsTr("Apply")
                    onClicked: {
                        MoveCenterTabController.rotation = MoveCenterTabController.tempRotation
                    }
               }
            }
        }
    }

    Component.onCompleted: {
        spaceRotationSlider.value = MoveCenterTabController.rotation
    }

    Connections {
        target: MoveCenterTabController
        function onRotationChanged() {
            spaceRotationSlider.value = ( MoveCenterTabController.rotation / 100 )
        }
        function onTempRotationChanged() {
            var intTempRotation = MoveCenterTabController.tempRotation
            spaceRotationSlider.value = ( intTempRotation / 100 )
            spaceRotationText.text = ( intTempRotation / 100 ) + "°"
        }
    }
}
