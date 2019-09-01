import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import ovras.advsettings 1.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
import "common"

MyStackViewPage {
    headerText: "Chaperone Proximity Warning Settings"

    ColorDialog {
        id: colorDialog
        title: "Please choose a color"
        onAccepted: {
            console.log("You chose: " + colorDialog.color)
            Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
    }

    content: ColumnLayout {
        spacing: 36

        ColumnLayout {
            spacing: 0
            MyToggleButton {
                id: switchBeginnerToggle
                text: "Switch to Beginner Mode"
                onCheckedChanged: {
                    ChaperoneTabController.chaperoneSwitchToBeginnerEnabled = checked
                }
            }
            RowLayout {
                MyText {
                    text: "Activation Distance: "
                    Layout.preferredWidth: 250
                }
                MyPushButton2 {
                    id: switchBeginnerDistanceMinusButton
                    Layout.preferredWidth: 40
                    text: "-"
                    onClicked: {
                        var val = ChaperoneTabController.chaperoneSwitchToBeginnerDistance - 0.1
                        if (val <= 0) {
                            val = 0.1;
                        }
                        ChaperoneTabController.chaperoneSwitchToBeginnerDistance = val.toFixed(2)
                    }
                }

                MySlider {
                    id: switchBeginnerDistanceSlider
                    from: 0
                    to: 2.0
                    stepSize: 0.01
                    value: 0.5
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = this.from + ( this.position  * (this.to - this.from))
                        switchBeginnerDistanceText.text = val.toFixed(2)
                    }
                    onValueChanged: {
                        var val = switchBeginnerDistanceSlider.value.toFixed(2)
                        if (val < 0.01) {
                            val = 0.01
                        }
                        ChaperoneTabController.chaperoneSwitchToBeginnerDistance = val
                        switchBeginnerDistanceText.text = val
                    }
                }

                MyPushButton2 {
                    id: switchBeginnerDistancePlusButton
                    Layout.preferredWidth: 40
                    text: "+"
                    onClicked: {
                        var val = ChaperoneTabController.chaperoneSwitchToBeginnerDistance + 0.1
                        if (val > 2.0) {
                            val = 2.0;
                        }
                        ChaperoneTabController.chaperoneSwitchToBeginnerDistance = val.toFixed(2)
                    }
                }

                MyTextField {
                    id: switchBeginnerDistanceText
                    text: "0.00"
                    keyBoardUID: 801
                    Layout.preferredWidth: 100
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseFloat(input)
                        if (!isNaN(val)) {
                            if (val <= 0.0) {
                                val = 0.01
                            }
                            val = val.toFixed(2)
                            ChaperoneTabController.chaperoneSwitchToBeginnerDistance = val
                            text = val
                        } else {
                            text = ChaperoneTabController.chaperoneSwitchToBeginnerDistance.toFixed(2)
                        }
                    }
                }
            }
        }

        ColumnLayout {
            spacing: 0
            MyToggleButton {
                id: hapticFeedbackToggle
                text: "Trigger Haptic Feedback"
                onCheckedChanged: {
                    ChaperoneTabController.chaperoneHapticFeedbackEnabled = checked
                }
            }
            RowLayout {
                MyText {
                    text: "Activation Distance: "
                    Layout.preferredWidth: 250
                }
                MyPushButton2 {
                    id: hapticFeedbackDistanceMinusButton
                    Layout.preferredWidth: 40
                    text: "-"
                    onClicked: {
                        var val = ChaperoneTabController.chaperoneHapticFeedbackDistance - 0.1
                        if (val <= 0) {
                            val = 0.1;
                        }
                        ChaperoneTabController.chaperoneHapticFeedbackDistance = val.toFixed(2)
                    }
                }

                MySlider {
                    id: hapticFeedbackDistanceSlider
                    from: 0
                    to: 2.0
                    stepSize: 0.01
                    value: 0.5
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = this.from + ( this.position  * (this.to - this.from))
                        hapticFeedbackDistanceText.text = val.toFixed(2)
                    }
                    onValueChanged: {
                        var val = hapticFeedbackDistanceSlider.value.toFixed(2)
                        if (val < 0.01) {
                            val = 0.01
                        }
                        ChaperoneTabController.chaperoneHapticFeedbackDistance = val
                        hapticFeedbackDistanceText.text = val
                    }
                }

                MyPushButton2 {
                    id: hapticFeedbackDistancePlusButton
                    Layout.preferredWidth: 40
                    text: "+"
                    onClicked: {
                        var val = ChaperoneTabController.chaperoneHapticFeedbackDistance + 0.1
                        if (val > 2.0) {
                            val = 2.0;
                        }
                        ChaperoneTabController.chaperoneHapticFeedbackDistance = val.toFixed(2)
                    }
                }

                MyTextField {
                    id: hapticFeedbackDistanceText
                    text: "0.00"
                    keyBoardUID: 805
                    Layout.preferredWidth: 100
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseFloat(input)
                        if (!isNaN(val)) {
                            if (val <= 0.0) {
                                val = 0.01
                            }
                            val = val.toFixed(2)
                            ChaperoneTabController.chaperoneHapticFeedbackDistance = val
                            text = val
                        } else {
                            text = ChaperoneTabController.chaperoneHapticFeedbackDistance.toFixed(2)
                        }
                    }
                }
            }
        }

        ColumnLayout {
            spacing: 0
            RowLayout {
                spacing: 32
                MyToggleButton {
                    id: audioWarningToggle
                    text: "Audio Warning"
                    onCheckedChanged: {
                        ChaperoneTabController.chaperoneAlarmSoundEnabled = checked
                    }
                }
                MyToggleButton {
                    id: audioWarningLoopingToggle
                    text: "Loop Audio"
                    onCheckedChanged: {
                        ChaperoneTabController.chaperoneAlarmSoundLooping = checked
                        audioWarningAdjustVolumeToggle.enabled = checked
                    }
                }
                MyToggleButton {
                    id: audioWarningAdjustVolumeToggle
                    text: "Adjust Volume"
                    enabled: false
                    onCheckedChanged: {
                        ChaperoneTabController.chaperoneAlarmSoundAdjustVolume = checked
                    }
                }
            }
            RowLayout {
                MyText {
                    text: "Activation Distance: "
                    Layout.preferredWidth: 250
                }
                MyPushButton2 {
                    id: audioWarningDistanceMinusButton
                    Layout.preferredWidth: 40
                    text: "-"
                    onClicked: {
                        var val = ChaperoneTabController.chaperoneAlarmSoundDistance - 0.1
                        if (val <= 0.0) {
                            val = 0.01;
                        }
                        ChaperoneTabController.chaperoneAlarmSoundDistance = val.toFixed(2)
                    }
                }

                MySlider {
                    id: audioWarningDistanceSlider
                    from: 0
                    to: 2.0
                    stepSize: 0.01
                    value: 0.5
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = this.from + ( this.position  * (this.to - this.from))
                        audioWarningDistanceText.text = val.toFixed(2)
                    }
                    onValueChanged: {
                        var val = audioWarningDistanceSlider.value.toFixed(2)
                        if (val < 0.01) {
                            val = 0.01
                        }
                        ChaperoneTabController.chaperoneAlarmSoundDistance = val
                        audioWarningDistanceText.text = val
                    }
                }

                MyPushButton2 {
                    id: audioWarningDistancePlusButton
                    Layout.preferredWidth: 40
                    text: "+"
                    onClicked: {
                        var val = ChaperoneTabController.chaperoneAlarmSoundDistance + 0.1
                        if (val > 2.0) {
                            val = 2.0;
                        }
                        ChaperoneTabController.chaperoneAlarmSoundDistance = val.toFixed(2)
                    }
                }

                MyTextField {
                    id: audioWarningDistanceText
                    text: "0.00"
                    keyBoardUID: 802
                    Layout.preferredWidth: 100
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseFloat(input)
                        if (!isNaN(val)) {
                            if (val <= 0.0) {
                                val = 0.01
                            }
                            val = val.toFixed(2)
                            ChaperoneTabController.chaperoneAlarmSoundDistance = val
                            text = val
                        } else {
                            text = ChaperoneTabController.chaperoneAlarmSoundDistance.toFixed(2)
                        }
                    }
                }
            }
        }

        ColumnLayout {
            spacing: 0
            MyToggleButton {
                id: openDashboardToggle
                text: "Open Dashboard"
                onCheckedChanged: {
                    ChaperoneTabController.chaperoneShowDashboardEnabled = checked
                }
            }
            RowLayout {
                MyText {
                    text: "Activation Distance: "
                    Layout.preferredWidth: 250
                }
                MyPushButton2 {
                    id: openDashboardDistanceMinusButton
                    Layout.preferredWidth: 40
                    text: "-"
                    onClicked: {
                        var val = ChaperoneTabController.chaperoneShowDashboardDistance - 0.1
                        if (val <= 0.0) {
                            val = 0.01;
                        }
                        ChaperoneTabController.chaperoneShowDashboardDistance = val.toFixed(2)
                    }
                }

                MySlider {
                    id: openDashboardDistanceSlider
                    from: 0
                    to: 2.0
                    stepSize: 0.01
                    value: 0.5
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = this.from + ( this.position  * (this.to - this.from))
                        openDashboardDistanceText.text = val.toFixed(2)
                    }
                    onValueChanged: {
                        var val = openDashboardDistanceSlider.value.toFixed(2)
                        if (val < 0.01) {
                            val = 0.01
                        }
                        ChaperoneTabController.chaperoneShowDashboardDistance = val
                        openDashboardDistanceText.text = val
                    }
                }

                MyPushButton2 {
                    id: openDashboardDistancePlusButton
                    Layout.preferredWidth: 40
                    text: "+"
                    onClicked: {
                        var val = ChaperoneTabController.chaperoneShowDashboardDistance + 0.1
                        if (val > 2.0) {
                            val = 2.0;
                        }
                        ChaperoneTabController.chaperoneShowDashboardDistance = val.toFixed(2)
                    }
                }

                MyTextField {
                    id: openDashboardDistanceText
                    text: "0.00"
                    keyBoardUID: 803
                    Layout.preferredWidth: 100
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseFloat(input)
                        if (!isNaN(val)) {
                            if (val <= 0.0) {
                                val = 0.01
                            }
                            val = val.toFixed(2)
                            ChaperoneTabController.chaperoneShowDashboardDistance = val
                            text = val
                        } else {
                            text = ChaperoneTabController.chaperoneShowDashboardDistance.toFixed(2)
                        }
                    }
                }
            }
        }

        Component.onCompleted: {
            switchBeginnerToggle.checked = ChaperoneTabController.chaperoneSwitchToBeginnerEnabled
            var d = ChaperoneTabController.chaperoneSwitchToBeginnerDistance.toFixed(2)
            if (d <= switchBeginnerDistanceSlider.to) {
                switchBeginnerDistanceSlider.value = d
            }
            switchBeginnerDistanceText.text = d
            hapticFeedbackToggle.checked = ChaperoneTabController.chaperoneHapticFeedbackEnabled
            var d = ChaperoneTabController.chaperoneHapticFeedbackDistance.toFixed(2)
            if (d <= hapticFeedbackDistanceSlider.to) {
                hapticFeedbackDistanceSlider.value = d
            }
            hapticFeedbackDistanceText.text = d
            audioWarningToggle.checked = ChaperoneTabController.chaperoneAlarmSoundEnabled
            audioWarningLoopingToggle.checked = ChaperoneTabController.chaperoneAlarmSoundLooping
            audioWarningAdjustVolumeToggle.checked = ChaperoneTabController.chaperoneAlarmSoundAdjustVolume
            d = ChaperoneTabController.chaperoneAlarmSoundDistance.toFixed(2)
            if (d <= audioWarningDistanceSlider.to) {
                audioWarningDistanceSlider.value = d
            }
            audioWarningDistanceText.text = d
            openDashboardToggle.checked = ChaperoneTabController.chaperoneShowDashboardEnabled
            d = ChaperoneTabController.chaperoneShowDashboardDistance.toFixed(2)
            if (d <= openDashboardDistanceSlider.to) {
                openDashboardDistanceSlider.value = d
            }
            openDashboardDistanceText.text = d
        }

        Connections {
            target: ChaperoneTabController
            onChaperoneSwitchToBeginnerEnabledChanged: {
                switchBeginnerToggle.checked = ChaperoneTabController.chaperoneSwitchToBeginnerEnabled
            }
            onChaperoneSwitchToBeginnerDistanceChanged: {
                var d = ChaperoneTabController.chaperoneSwitchToBeginnerDistance.toFixed(2)
                if (d <= switchBeginnerDistanceSlider.to && Math.abs(switchBeginnerDistanceSlider.value - d) > 0.0008) {
                    switchBeginnerDistanceSlider.value = d
                }
                switchBeginnerDistanceText.text = d
            }
            onChaperoneHapticFeedbackEnabledChanged: {
                hapticFeedbackToggle.checked = ChaperoneTabController.chaperoneHapticFeedbackEnabled
            }
            onChaperoneHapticFeedbackDistanceChanged: {
                var d = ChaperoneTabController.chaperoneHapticFeedbackDistance.toFixed(2)
                if (d <= hapticFeedbackDistanceSlider.to && Math.abs(hapticFeedbackDistanceSlider.value - d) > 0.0008) {
                    hapticFeedbackDistanceSlider.value = d
                }
                hapticFeedbackDistanceText.text = d
            }
            onChaperoneAlarmSoundEnabledChanged: {
                audioWarningToggle.checked = ChaperoneTabController.chaperoneAlarmSoundEnabled
            }
            onChaperoneAlarmSoundLoopingChanged: {
                audioWarningLoopingToggle.checked = ChaperoneTabController.chaperoneAlarmSoundLooping
            }
            onChaperoneAlarmSoundDistanceChanged: {
                var d = ChaperoneTabController.chaperoneAlarmSoundDistance.toFixed(2)
                if (d <= audioWarningDistanceSlider.to && Math.abs(audioWarningDistanceSlider.value - d) > 0.0008) {
                    audioWarningDistanceSlider.value = d
                }
                audioWarningDistanceText.text = d
            }
            onChaperoneShowDashboardEnabledChanged: {
                openDashboardToggle.checked = ChaperoneTabController.chaperoneShowDashboardEnabled
            }
            onChaperoneShowDashboardDistanceChanged: {
                var d = ChaperoneTabController.chaperoneShowDashboardDistance.toFixed(2)
                if (d <= openDashboardDistanceSlider.to && Math.abs(openDashboardDistanceSlider.value - d) > 0.0008) {
                    openDashboardDistanceSlider.value = d
                }
                openDashboardDistanceText.text = d
            }
        }
    }
}
