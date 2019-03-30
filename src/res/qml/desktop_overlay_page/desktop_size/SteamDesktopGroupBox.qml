import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Steam Desktop Overlay Width"
        bottomPadding: -10
    }
    background: Rectangle {
        color: "transparent"
        border.color: "#ffffff"
        radius: 8
    }

    property real sliderStepSize: 1.0
    property real sliderStartingValue: 1.0

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
                        steamDesktopWidthSlider.value -= sliderStepSize
                    }
                }

                MySlider {
                    id: steamDesktopWidthSlider
                    from: 0.1
                    to: 32.0
                    stepSize: 0.1
                    value: sliderStartingValue
                    Layout.fillWidth: true
                    onPositionChanged: {
                        //
                    }
                    onValueChanged: {
                        DesktopOverlay.width = this.value
                        steamDesktopWidthText.text = this.value.toFixed(2)
                    }
                }

                MyPushButton2 {
                    Layout.preferredWidth: 40
                    text: "+"
                    onClicked: {
                        steamDesktopWidthSlider.value += sliderStepSize
                    }
                }

                MyTextField {
                    id: steamDesktopWidthText
                    text: "0.0"
                    keyBoardUID: 611
                    Layout.preferredWidth: 100
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseFloat(input)
                        if (!isNaN(val)) {
                            if (val < 0.1) {
                                val = 0.1
                            }
                            steamDesktopWidthSlider.value = val
                        }
                    }
                }
            }
        }
    }
    Component.onCompleted: {
        steamDesktopWidthSlider.value = DesktopOverlay.width
    }
}
