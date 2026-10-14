import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: autoTurnGroupBox
    Layout.fillWidth: true

    title: "Auto-Turn (for discrete redirected walking)"

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true

            MyToggleButton {
                id: autoTurn
                text: "Toggle On/Off"
                onCheckedChanged: {
                    RotationTabController.setAutoTurnEnabled(this.checked, true)
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }

        LineSeparator {
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            MyToggleButton {
                id: autoTurnNotificationToggle
                text: "HMD Icon"
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Layout.rightMargin: 0
                onCheckedChanged: {
                    RotationTabController.setAutoTurnShowNotification(this.checked, true)
                }
            }

            VerticalSeparator {
            }

            MyToggleButton {
                id: cornerAngle
                text: "Use Corner Angle"
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Layout.rightMargin: 0
                onCheckedChanged: {
                    RotationTabController.setAutoTurnUseCornerAngle(this.checked, true)
                }
            }

            VerticalSeparator {
            }

            MyToggleButton {
                id: autoTurnModeToggle
                text: "Use Smooth Turn"
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Layout.rightMargin: 0
                onCheckedChanged: {
                    RotationTabController.setAutoTurnMode(this.checked ? 1 : 0, true)
                }
            }
        }

        LineSeparator {
        }

        RowLayout {
            Layout.fillWidth: true

            MyText {
                text: "Activation Distance"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
            }

            MySlider {
                id: activationSlider
                from: 0
                to: 1.0
                stepSize: 0.05
                value: .4
                Layout.preferredWidth: 300
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                onPositionChanged: {
                    var val = this.value
                    activationValueText.text = val.toFixed(2)
                }
                onValueChanged: {
                    RotationTabController.setAutoTurnActivationDistance(value, true)
                }
            }

            MyTextField {
                id: activationValueText
                text: "0.4"
                keyBoardUID: 1001
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0) {
                            val = 0
                        } else if (val > 22) {
                            val = 22
                        }
                        activationSlider.value = val.toFixed(2)
                    }
                    text = RotationTabController.autoTurnActivationDistance.toFixed(2)
                }
            }
        }

        LineSeparator {
        }

        RowLayout {
            Layout.fillWidth: true

            MyText {
                text: "Deactivation Distance"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
            }

            MySlider {
                id: deactivationSlider
                from: 0
                to: 1.0
                stepSize: 0.05
                value: .15
                Layout.preferredWidth: 300
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                onPositionChanged: {
                    var val = this.value
                    deactivationValueText.text = val.toFixed(2)
                }
                onValueChanged: {
                    RotationTabController.setAutoTurnDeactivationDistance(value, true)
                }
            }

            MyTextField {
                id: deactivationValueText
                text: "0.15"
                keyBoardUID: 1002
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0) {
                            val = 0
                        } else if (val > 22) {
                            val = 22
                        }
                        deactivationSlider.value = val.toFixed(2)
                    }
                    text = RotationTabController.autoTurnDeactivationDistance.toFixed(2)
                }
            }
        }

        LineSeparator {
        }

        RowLayout {
            Layout.fillWidth: true

            MyText {
                text: "Turn Speed (deg/sec)"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
            }

            MySlider {
                id: speedSlider
                from: 0
                to: 900
                stepSize: 10
                value: 90
                Layout.preferredWidth: 300
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                onPositionChanged: {
                    var val = this.value
                    speedValueText.text = val.toFixed() + "°"
                }
                onValueChanged: {
                    RotationTabController.setAutoTurnSpeed(parseInt(value*100), true)
                }
            }

            MyTextField {
                id: speedValueText
                text: "90°"
                keyBoardUID: 1003
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 1) {
                            val = 1
                        //caps user set Speed to 1000 deg/Second
                        //used as overflow prevention
                        //arbitrary
                        } else if (val > 1000) {
                            val = 1000
                        }
                        speedSlider.value = val
                    }
                    //converts the centidegrees to degrees
                    text = (((RotationTabController.autoTurnSpeed)/100).toFixed()) + "°"
                }
            }
        }

        LineSeparator {
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredWidth: 1

                MyText {
                    text: "Detangle Min Rotations"
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignLeft
                }

                MyTextField {
                    id: detangleAngleStartText
                    text: "360°"
                    keyBoardUID: 1005
                    Layout.preferredWidth: 100
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseFloat(input)
                        if (!isNaN(val)) {
                            if (val < 0) {
                                val = 0
                            } else if (val > 3600) {
                                val = 3600
                            }
                            RotationTabController.setMinCordTangle((val*(Math.PI/180)), true)
                            text = (Math.round(val).toFixed()) + "°"
                        }
                    }
                }
            }

            VerticalSeparator {
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredWidth: 1

                MyText {
                    text: "Max Wall Angle"
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignLeft
                }

                MyTextField {
                    id: detangleAngleAssistText
                    text: "10°"
                    keyBoardUID: 1006
                    Layout.preferredWidth: 100
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseFloat(input)
                        if (!isNaN(val)) {
                            if (val < 0) {
                                val = 0
                            } else if (val > 3600) {
                                val = 3600
                            }
                            RotationTabController.setCordDetangleAngle((val*(Math.PI/180)), true)
                            text = (Math.round(val).toFixed()) + "°"
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        activationSlider.value = RotationTabController.autoTurnActivationDistance.toFixed(2)
        autoTurn.checked = RotationTabController.autoTurnEnabled
        deactivationSlider.value = RotationTabController.autoTurnDeactivationDistance.toFixed(2)
        cornerAngle.checked = RotationTabController.autoTurnUseCornerAngle
        speedSlider.value = (RotationTabController.autoTurnSpeed/100).toFixed()
        autoTurnNotificationToggle.checked = RotationTabController.autoTurnShowNotification
        autoTurnModeToggle.checked = RotationTabController.autoTurnMode === 1
        detangleAngleStartText.text = parseInt(RotationTabController.minCordTangle*(180/Math.PI))+ "°"
        detangleAngleAssistText.text = parseInt(RotationTabController.cordDetangleAngle*(180/Math.PI))+ "°"
    }

    Connections {
        target: RotationTabController

        function onAutoTurnEnabledChanged() {
            autoTurn.checked = RotationTabController.autoTurnEnabled
        }
        function onAutoTurnActivationDistanceChanged() {
            activationSlider.value = RotationTabController.autoTurnActivationDistance.toFixed(2)
        }
        function onAutoTurnDeactivationDistanceChanged(){
            deactivationSlider.value = RotationTabController.autoTurnDeactivationDistance.toFixed(2)
        }
        function onAutoTurnUseCornerAngleChanged(){
            cornerAngle.checked = RotationTabController.autoTurnUseCornerAngle
        }
        function onAutoTurnSpeedChanged(){
            var val = RotationTabController.autoTurnSpeed
            speedSlider.value = ((val/100).toFixed())
        }
        function onAutoTurnModeChanged(){
            autoTurnModeToggle.checked = RotationTabController.autoTurnMode === 1
        }
        function onMinCordTangleChanged(){
            detangleAngleStartText.text = parseInt(RotationTabController.minCordTangle*(180/Math.PI))+ "°"
        }
        function onCordDetangleAngleChanged(){
            detangleAngleAssistText.text = parseInt(RotationTabController.cordDetangleAngle*(180/Math.PI))+ "°"
        }
        function onAutoTurnShowNotificationChanged(){
            autoTurnNotificationToggle.checked = RotationTabController.autoTurnShowNotification
        }
    }
}
