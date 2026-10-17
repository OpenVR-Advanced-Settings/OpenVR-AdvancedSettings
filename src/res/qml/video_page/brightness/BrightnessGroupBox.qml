import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: brightnessGroupBox
    Layout.fillWidth: true

    title: qsTr("Brightness")

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        RowLayout {
            Layout.preferredHeight: rowHeight
            Layout.fillWidth: true

            MyToggleButton {
                id: brightnessToggle
                text: qsTr("On/Off")
                onCheckedChanged: {
                    VideoTabController.setBrightnessEnabled(this.checked, true)                }
            }

            Item {
                Layout.preferredWidth: 150
            }

            MyText {
                text: qsTr("Brightness")
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MySlider {
                id: brightnessSlider
                from: 0.2
                to: 1.0
                stepSize: 0.01
                value: 1.0
                Layout.fillWidth: true
                onMoved: {
                    VideoTabController.setBrightnessOpacityValue(this.value.toFixed(2), true)
                }
            }

            MyTextField {
                id: brightnessValueText
                text: "100%"
                keyBoardUID: 901
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 3) {
                            val = 3
                        } else if (val > 100.0) {
                            val = 100.0
                        }
                        var v = (val/100)
                        VideoTabController.setBrightnessOpacityValue(v.toFixed(2), true)
                    }
                    text = Math.round(VideoTabController.brightnessOpacityValue * 100) + "%"
                }
            }
        }
    }

    Component.onCompleted: {
        var brightvalue = VideoTabController.brightnessOpacityValue
        brightnessSlider.value = brightvalue
        brightnessValueText.text = Math.round(brightvalue*100)+ "%"
        brightnessToggle.checked = VideoTabController.brightnessEnabled
    }

    Connections {
        target: VideoTabController

        function onBrightnessOpacityValueChanged() {
            var brightvalue = VideoTabController.brightnessOpacityValue
            brightnessSlider.value = brightvalue
            brightnessValueText.text = Math.round(brightvalue*100)+ "%"
        }
        function onBrightnessEnabledChanged() {
            brightnessToggle.checked = VideoTabController.brightnessEnabled
        }
    }
}
