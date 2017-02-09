import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0



MyStackViewPage {
    headerText: "Accessibility Tools"

    PttControllerConfigDialog {
        id: pttControllerConfigDialog
        pttControllerConfigClass: AccessibilityTabController
    }

    content: ColumnLayout {
        spacing: 92

        GroupBox {
            Layout.fillWidth: true

            label: MyText {
                leftPadding: 10
                text: "Adjust Player Height"
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
                    MyPushButton2 {
                        Layout.preferredWidth: 40
                        text: "-"
                        onClicked: {
                            accessibilityMoveYSlider.value -= 0.05
                        }
                    }

                    MySlider {
                        id: accessibilityMoveYSlider
                        from: 0
                        to: 1.5
                        stepSize: 0.01
                        value: 0.0
                        Layout.fillWidth: true
                        onPositionChanged: {
                            var val = this.from + ( this.position  * (this.to - this.from))
                            accessibilityMoveYText.text = val.toFixed(2)
                        }
                        onValueChanged: {
                            AccessibilityTabController.heightOffset = this.value.toFixed(2)
                        }
                    }

                    MyPushButton2 {
                        Layout.preferredWidth: 40
                        text: "+"
                        onClicked: {
                            accessibilityMoveYSlider.value += 0.05
                        }
                    }

                    MyTextField {
                        id: accessibilityMoveYText
                        text: "0.00"
                        keyBoardUID: 700
                        Layout.preferredWidth: 140
                        Layout.leftMargin: 10
                        Layout.rightMargin: 10
                        horizontalAlignment: Text.AlignHCenter
                        function onInputEvent(input) {
                            var val = parseFloat(input)
                            if (!isNaN(val)) {
                                if (val >= accessibilityMoveYSlider.from && val <= accessibilityMoveYSlider.to) {
                                    accessibilityMoveYSlider.value = val.toFixed(2);
                                } else {
                                    AccessibilityTabController.setHeightOffset(val.toFixed(2), false);
                                }
                            }
                            text = AccessibilityTabController.heightOffset.toFixed(2)
                        }
                    }
                }

                RowLayout {
                    MyToggleButton {
                        id: heightOffsetPttEnabledToggle
                        Layout.preferredWidth: 260
                        text: "Push-to-Toggle:"
                        onClicked: {
                            AccessibilityTabController.pttEnabled = checked
                        }
                    }
                    MyToggleButton {
                        id: heightOffsetPttLeftControllerToggle
                        text: "Left Controller"
                        onClicked: {
                            AccessibilityTabController.setPttLeftControllerEnabled(checked, false)
                        }
                    }
                    MyPushButton {
                        text: "Configure"
                        onClicked: {
                            pttControllerConfigDialog.openPopup(0)
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    MyToggleButton {
                        id: heightOffsetPttRightControllerToggle
                        text: "Right Controller"
                        onClicked: {
                            AccessibilityTabController.setPttRightControllerEnabled(checked, false)
                        }
                    }
                    MyPushButton {
                        text: "Configure"
                        onClicked: {
                            pttControllerConfigDialog.openPopup(1)
                        }
                    }
                }
            }
        }

        Item { Layout.fillHeight: true; Layout.fillWidth: true}

        MyPushButton {
            id: accessibilityResetButton
            Layout.preferredWidth: 250
            text: "Reset"
            onClicked: {
                AccessibilityTabController.reset()
            }
        }

        Component.onCompleted: {
            var heightOffsetFormatted = AccessibilityTabController.heightOffset.toFixed(2)
            if (heightOffsetFormatted >= accessibilityMoveYSlider.from && heightOffsetFormatted <= accessibilityMoveYSlider.to) {
                accessibilityMoveYSlider.value = heightOffsetFormatted
            }
            accessibilityMoveYText.text = heightOffsetFormatted
            heightOffsetPttEnabledToggle.checked = AccessibilityTabController.pttEnabled
            heightOffsetPttLeftControllerToggle.checked = AccessibilityTabController.pttLeftControllerEnabled
            heightOffsetPttRightControllerToggle.checked = AccessibilityTabController.pttRightControllerEnabled
        }

        Connections {
            target: AccessibilityTabController
            onHeightOffsetChanged: {
                var heightOffsetFormatted = AccessibilityTabController.heightOffset.toFixed(2)
                if (heightOffsetFormatted >= accessibilityMoveYSlider.from && heightOffsetFormatted <= accessibilityMoveYSlider.to) {
                    accessibilityMoveYSlider.value = heightOffsetFormatted
                }
                accessibilityMoveYText.text = heightOffsetFormatted
            }
            onPttEnabledChanged: {
                heightOffsetPttEnabledToggle.checked = AccessibilityTabController.pttEnabled
            }
            onPttActiveChanged: {

            }
            onPttLeftControllerEnabledChanged: {
                heightOffsetPttLeftControllerToggle.checked = AccessibilityTabController.pttLeftControllerEnabled
            }
            onPttRightControllerEnabledChanged: {
                heightOffsetPttRightControllerToggle.checked = AccessibilityTabController.pttRightControllerEnabled
            }
        }

    }

}
