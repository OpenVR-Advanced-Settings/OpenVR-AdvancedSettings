import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: steamDesktopGroupBox
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
    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            color: "#ffffff"
            height: 1
            Layout.fillWidth: true
            Layout.bottomMargin: 5
        }

        ColumnLayout {
            RowLayout {
                MyPushButton2 {
                    id: steamDesktopWidthMinusButton
                    Layout.preferredWidth: 40
                    text: "-"
                    onClicked: {
                        steamDesktopWidthSlider.value -= 1.0
                    }
                }

                MySlider {
                    id: steamDesktopWidthSlider
                    from: 0.1
                    to: 32.0
                    stepSize: 0.1
                    value: 1.0
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = this.from + ( this.position  * (this.to - this.from))
                        steamDesktopWidthText.text = val.toFixed(1)
                    }
                    onValueChanged: {
                        UtilitiesTabController.setSteamDesktopOverlayWidth(value, false)
                        steamDesktopWidthText.text = value.toFixed(1)
                    }
                }

                MyPushButton2 {
                    id: steamDesktopWidthPlusButton
                    Layout.preferredWidth: 40
                    text: "+"
                    onClicked: {
                        steamDesktopWidthSlider.value += 1.0
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
                            UtilitiesTabController.setSteamDesktopOverlayWidth(val, false)
                        }
                        text = UtilitiesTabController.steamDesktopOverlayWidth.toFixed(1)
                    }
                }
            }
        }
    }
    Component.onCompleted: {
        steamDesktopGroupBox.visible = UtilitiesTabController.steamDesktopOverlayAvailable
        if (steamDesktopGroupBox.visible) {
            steamDesktopWidthSlider.value = UtilitiesTabController.steamDesktopOverlayWidth
        }
    }
    Connections {
        target: UtilitiesTabController
        onSteamDesktopOverlayAvailableChanged: {
            steamDesktopGroupBox.visible = UtilitiesTabController.steamDesktopOverlayAvailable
            if (steamDesktopGroupBox.visible) {
                steamDesktopWidthSlider.value = UtilitiesTabController.steamDesktopOverlayWidth
            }
        }
        onSteamDesktopOverlayWidthChanged: {
            steamDesktopWidthSlider.value = UtilitiesTabController.steamDesktopOverlayWidth
        }
    }
}
