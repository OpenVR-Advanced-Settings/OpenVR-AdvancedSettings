import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Backwards/Forwards Translation"
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
                        steamDesktopForwardSlider.value -= sliderStepSize
                    }
                }

                MySlider {
                    id: steamDesktopForwardSlider
                    from: -rightLeftLimit
                    to: rightLeftLimit
                    stepSize: sliderStepSize
                    value: 0.0
                    Layout.fillWidth: true
                    onValueChanged: {
                        DesktopOverlay.forwardsMovement = this.value
                        steamDesktopForwardText.text = this.value.toFixed(2)
                    }
                }

                MyPushButton2 {
                    Layout.preferredWidth: 40
                    text: "+"
                    onClicked: {
                        steamDesktopForwardSlider.value += sliderStepSize
                    }
                }

                MyTextField {
                    id: steamDesktopForwardText
                    text: "0.0"
                    keyBoardUID: 611
                    Layout.preferredWidth: 100
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    onAcceptableInputChanged: {
                        steamDesktopForwardSlider.value = this.value
                    }
                }
            }
        }
    }
    Component.onCompleted: {
        steamDesktopForwardSlider.value = DesktopOverlay.forwardsMovement
    }
}
