import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"
import "." // QTBUG-34418, singletons require explicit import to load qmldir file

GroupBox {
    id: changeOrientationGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Rotate Orientation (permanent)"
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

        ColumnLayout {
            RowLayout {
                MyPushButton2 {
                    id: spaceRotationMinusButton
                    Layout.preferredWidth: 40
                    text: "-"
                    onClicked: {
                        var val = spaceRotationSlider.value - 45
                        if (val <= -180){
                            val = -180;
                        }
                        spaceRotationSlider.value = val
                        spaceRotationText.text = val.toFixed(2) + "°"
                    }
                }

                MySlider {
                    id: spaceRotationSlider
                    from: -180
                    to: 180
                    stepSize: 1
                    value: 0
                    Layout.fillWidth: true
                    onValueChanged: {
                        var val = this.value
                        spaceRotationText.text = val.toFixed(2) + "°"
                    }
                }

                MyPushButton2 {
                    id: spaceRotationPlusButton
                    Layout.preferredWidth: 40
                    text: "+"
                    onClicked: {
                        var val = spaceRotationSlider.value + 45
                        if (val >= 180){
                            val = 180;
                        }
                        spaceRotationSlider.value = val
                        spaceRotationText.text = val.toFixed(2) + "°"
                    }
                }

                MyTextField {
                    id: spaceRotationText
                    text: "0°"
                    keyBoardUID: 311
                    Layout.preferredWidth: 150
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseFloat(input)
                        if (!isNaN(val)) {
                            spaceRotationText.text = val.toFixed(2) + "°"
                            spaceRotationSlider.value = val
                        }
                    }
                }
                MyPushButton {
                    id: spaceRotationApplyButton
                    Layout.preferredWidth: 145
                    text:"Apply"
                    onClicked: {
                        ChaperoneTabController.flipOrientation(spaceRotationSlider.value)
                        spaceRotationSlider.value = 0
                        spaceRotationText.text = "0°"
                    }
               }
            }
        }
    }
}
