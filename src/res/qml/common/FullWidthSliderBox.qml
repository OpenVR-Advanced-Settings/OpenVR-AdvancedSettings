import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0

GroupBox {
    // Public properties
    // Overload
    property alias headerMessage: headerText.text
    property alias sliderText: steamDesktopForwardText.text
    property alias sliderValue: steamDesktopForwardSlider.value
    property alias lowerLimit: steamDesktopForwardSlider.from
    property alias upperLimit: steamDesktopForwardSlider.to
    property alias kbUID: steamDesktopForwardText.keyBoardUID


    function onValueChanged(value) {

    }

    function onComponentComplete() {

    }


    // Private properties
    id: sliderBox
    Layout.fillWidth: true
    label: MyText {
        id: headerText
        leftPadding: 10
        text: "MISSING HEADER TEXT"
        bottomPadding: -10
    }
    background: Rectangle {
        color: "transparent"
        border.color: "#d9dbe0"
        radius: 8
    }

    property real sliderStepSize: 0.10
    readonly property real rightLeftLimit: 4.0

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
                    Layout.fillWidth: true
                    onValueChanged: {
                        sliderBox.onValueChanged(this.value)
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
                }
            }
        }
    }
    Component.onCompleted: {
        sliderBox.onComponentComplete()
    }
}
