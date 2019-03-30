import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Left/Right Translation"
        bottomPadding: -10
    }
    background: Rectangle {
        color: "transparent"
        border.color: "#ffffff"
        radius: 8
    }
    property real sliderStepSize: 0.10
    property real rightLeftLimit: 4.0

    ColumnLayout {
        anchors.fill: parent

        LineSeparator {
        }

        ColumnLayout {
            RowLayout {
                MyPushButton2 {
                    Layout.preferredWidth: 40
                    text: "-"
                    onClicked: {
                        steamDesktopRightSlider.value -= sliderStepSize
                    }
                }

                MySlider {
                    id: steamDesktopRightSlider
                    from: -rightLeftLimit
                    to: rightLeftLimit
                    stepSize: sliderStepSize
                    value: 0.0
                    Layout.fillWidth: true
                    onValueChanged: {
                        DesktopOverlay.rightMovement = this.value
                        steamDesktopRightText.text = this.value.toFixed(2)
                    }
                }

                MyPushButton2 {
                    Layout.preferredWidth: 40
                    text: "+"
                    onClicked: {
                        steamDesktopRightSlider.value += sliderStepSize
                    }
                }

                MyTextField {
                    id: steamDesktopRightText
                    text: "0.0"
                    keyBoardUID: 611
                    Layout.preferredWidth: 100
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    onAcceptableInputChanged: {
                        steamDesktopRightSlider.value = this.value
                    }
                }
            }
        }
    }
    Component.onCompleted: {
        steamDesktopRightSlider.value = DesktopOverlay.rightMovement
    }
}
