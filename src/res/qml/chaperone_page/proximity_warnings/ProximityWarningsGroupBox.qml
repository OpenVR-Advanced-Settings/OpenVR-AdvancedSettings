import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"
import "." // QTBUG-34418, singletons require explicit import to load qmldir file

SettingsGroupBox {
    id: proximityWarningsGroupBox
    Layout.fillWidth: true

    title: "Boundary Warnings"

    // A single activation distance drives every warning type.
    function setActivationDistance(val) {
        if (val < 0.01) {
            val = 0.01
        } else if (val > 2.0) {
            val = 2.0
        }
        val = val.toFixed(2)
        ChaperoneTabController.chaperoneSwitchToBeginnerDistance = val
        ChaperoneTabController.chaperoneHapticFeedbackDistance = val
        ChaperoneTabController.chaperoneAlarmSoundDistance = val
        ChaperoneTabController.chaperoneShowDashboardDistance = val
    }

    function updateActivationDistance(distance) {
        var d = distance.toFixed(2)
        if (d <= activationDistanceSlider.to && Math.abs(activationDistanceSlider.value - d) > 0.0008) {
            activationDistanceSlider.value = d
        }
        activationDistanceText.text = d
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        MyToggleButton {
            id: switchBeginnerToggle
            text: "Switch to Beginner Mode"
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                ChaperoneTabController.chaperoneSwitchToBeginnerEnabled = checked
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: hapticFeedbackToggle
            text: "Trigger Haptic Feedback"
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                ChaperoneTabController.chaperoneHapticFeedbackEnabled = checked
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: audioWarningToggle
            text: "Audio Warning"
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                ChaperoneTabController.chaperoneAlarmSoundEnabled = checked
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: audioWarningLoopingToggle
            text: "Loop Audio"
            enabled: audioWarningToggle.checked
            Layout.fillWidth: true
            Layout.leftMargin: 32
            Layout.rightMargin: 0
            onCheckedChanged: {
                ChaperoneTabController.chaperoneAlarmSoundLooping = checked
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: audioWarningAdjustVolumeToggle
            text: "Volume by Proximity"
            enabled: audioWarningToggle.checked && audioWarningLoopingToggle.checked
            Layout.fillWidth: true
            Layout.leftMargin: 32
            Layout.rightMargin: 0
            onCheckedChanged: {
                ChaperoneTabController.chaperoneAlarmSoundAdjustVolume = checked
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: openDashboardToggle
            text: "Open Dashboard"
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                ChaperoneTabController.chaperoneShowDashboardEnabled = checked
            }
        }
        LineSeparator {
        }
        RowLayout {
            MyText {
                text: "Activation Distance: "
                Layout.preferredWidth: 250
            }
            MyPushButton2 {
                id: activationDistanceMinusButton
                Layout.preferredWidth: 40
                text: "-"
                onClicked: {
                    setActivationDistance(activationDistanceSlider.value - 0.1)
                }
            }

            MySlider {
                id: activationDistanceSlider
                from: 0
                to: 2.0
                stepSize: 0.01
                value: 0.5
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = this.from + ( this.position  * (this.to - this.from))
                    activationDistanceText.text = val.toFixed(2)
                }
                onValueChanged: {
                    setActivationDistance(activationDistanceSlider.value)
                }
            }

            MyPushButton2 {
                id: activationDistancePlusButton
                Layout.preferredWidth: 40
                text: "+"
                onClicked: {
                    setActivationDistance(activationDistanceSlider.value + 0.1)
                }
            }

            MyTextField {
                id: activationDistanceText
                text: "0.00"
                keyBoardUID: 801
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        setActivationDistance(val)
                    } else {
                        text = ChaperoneTabController.chaperoneSwitchToBeginnerDistance.toFixed(2)
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        switchBeginnerToggle.checked = ChaperoneTabController.chaperoneSwitchToBeginnerEnabled
        hapticFeedbackToggle.checked = ChaperoneTabController.chaperoneHapticFeedbackEnabled
        audioWarningToggle.checked = ChaperoneTabController.chaperoneAlarmSoundEnabled
        audioWarningLoopingToggle.checked = ChaperoneTabController.chaperoneAlarmSoundLooping
        audioWarningAdjustVolumeToggle.checked = ChaperoneTabController.chaperoneAlarmSoundAdjustVolume
        openDashboardToggle.checked = ChaperoneTabController.chaperoneShowDashboardEnabled
        updateActivationDistance(ChaperoneTabController.chaperoneSwitchToBeginnerDistance)
    }

    Connections {
        target: ChaperoneTabController
        function onChaperoneSwitchToBeginnerEnabledChanged() {
            switchBeginnerToggle.checked = ChaperoneTabController.chaperoneSwitchToBeginnerEnabled
        }
        function onChaperoneHapticFeedbackEnabledChanged() {
            hapticFeedbackToggle.checked = ChaperoneTabController.chaperoneHapticFeedbackEnabled
        }
        function onChaperoneAlarmSoundEnabledChanged() {
            audioWarningToggle.checked = ChaperoneTabController.chaperoneAlarmSoundEnabled
        }
        function onChaperoneAlarmSoundLoopingChanged() {
            audioWarningLoopingToggle.checked = ChaperoneTabController.chaperoneAlarmSoundLooping
        }
        function onChaperoneShowDashboardEnabledChanged() {
            openDashboardToggle.checked = ChaperoneTabController.chaperoneShowDashboardEnabled
        }
        function onChaperoneSwitchToBeginnerDistanceChanged() {
            updateActivationDistance(ChaperoneTabController.chaperoneSwitchToBeginnerDistance)
        }
    }
}
