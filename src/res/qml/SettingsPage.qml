import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "common"

MyStackViewPage {
    headerText: qsTr("Application Settings")

    content: ColumnLayout {
        spacing: 18

        SettingsGroupBox {
            title: qsTr("Sound")
            RowLayout {
                anchors.fill: parent

                MyText {
                    text: qsTr("Application Volume")
                    Layout.rightMargin: 12
                }

                MyPushButton2 {
                    text: "-"
                    Layout.preferredWidth: 40
                    onClicked: {
                        volumeSlider.value -= 0.05
                    }
                }

                MySlider {
                    id: volumeSlider
                    from: 0.0
                    to: 1.0
                    stepSize: 0.01
                    value: 0.7
                    Layout.fillWidth: true
                    onPositionChanged: {
                        var val = (this.value * 100)
                        volumeText.text = Math.round(val) + "%"
                    }
                    onValueChanged: {
                        OverlayController.setSoundVolume(value, false)
                    }
                }

                MyPushButton2 {
                    text: "+"
                    Layout.preferredWidth: 40
                    onClicked: {
                        volumeSlider.value += 0.05
                    }
                }


                MyTextField {
                    id: volumeText
                    text: "70%"
                    keyBoardUID: 503
                    Layout.preferredWidth: 100
                    Layout.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseFloat(input)
                        if (!isNaN(val)) {
                            if (val < 0) {
                                val = 0
                            } else if (val > 100.0) {
                                val = 100.0
                            }

                            var v = (val/100).toFixed(0)
                            if (v <= volumeSlider.to) {
                                chaperoneVisibilitySlider.value = v
                            } else {
                                ChaperoneTabController.setBoundsVisibility(v, false)
                            }
                        }
                        text = Math.round(ChaperoneTabController.boundsVisibility * 100) + "%"
                    }
                }
            }
        }

        SettingsGroupBox {
            title: qsTr("General")
            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                MyToggleButton {
                    id: settingsAutoStartToggle
                    text: qsTr("Autostart")
                    Layout.fillWidth: true
                    Layout.rightMargin: 0
                    onCheckedChanged: {
                        SettingsTabController.setAutoStartEnabled(checked, false)
                    }
                }
                LineSeparator {
                }
                MyToggleButton {
                    id: desktopModeToggleButton
                    text: qsTr("Desktop Mode (restart required)")
                    Layout.fillWidth: true
                    Layout.rightMargin: 0
                    onCheckedChanged: {
                        OverlayController.setDesktopModeToggle(this.checked, false)
                    }
                }
                LineSeparator {
                }
                MyToggleButton {
                    id: universeCenteredRotationToggle
                    text: qsTr("Universe-Centered Rotation (Disables HMD Centering)")
                    Layout.fillWidth: true
                    Layout.rightMargin: 0
                    onCheckedChanged: {
                        MoveCenterTabController.setUniverseCenteredRotation(checked, true)
                    }
                }
                LineSeparator {
                }
                MyToggleButton {
                    id: disableCrashRecoveryToggle
                    text: qsTr("Enable Automatic Crash Recovery of Chaperone Config")
                    Layout.fillWidth: true
                    Layout.rightMargin: 0
                    onCheckedChanged: {
                        OverlayController.setCrashRecoveryDisabled(!checked, true)
                    }
                }
                LineSeparator {
                }
                MyToggleButton {
                    id: disableVersionCheckToggle
                    text: qsTr("Disable Notification of Newer Version Availability")
                    Layout.fillWidth: true
                    Layout.rightMargin: 0
                    onCheckedChanged: {
                        OverlayController.setDisableVersionCheck(checked, true)
                    }
                }
                LineSeparator {
                }
                MyToggleButton {
                    id: exclusiveInputToggleButton
                    text: qsTr("Exclusive Input Toggle (This enables Key Binding to Toggle state)")
                    Layout.fillWidth: true
                    Layout.rightMargin: 0
                    onCheckedChanged: {
                        OverlayController.setExclusiveInputEnabled(this.checked, true)
                    }
                }
            }
        }

        SettingsGroupBox {
            title: qsTr("Chaperone")
            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                MyToggleButton {
                    id: nativeChaperoneToggleButton
                    text: qsTr("Force Use SteamVR Chaperone (experimental + SteamVR restart required)")
                    Layout.fillWidth: true
                    Layout.rightMargin: 0
                    onCheckedChanged: {
                        SettingsTabController.setNativeChaperoneToggle(this.checked, true)
                    }
                }
                LineSeparator {
                }
                RowLayout {
                    spacing: 18

                    MyToggleButton {
                        id: autoApplyChaperoneToggleButton
                        text: qsTr("Auto Apply Chaperone Profile")
                        Layout.fillWidth: true
                        Layout.rightMargin: 0
                        onCheckedChanged: {
                            OverlayController.setAutoApplyChaperoneEnabled(this.checked, true)
                        }
                    }

                    MyComboBox {
                        id: summaryChaperoneProfileComboBox
                        Layout.leftMargin: 30
                        Layout.preferredWidth: 378
                        model: [""]
                        onCurrentIndexChanged: {
                            if (currentIndex > 0) {
                                summaryChaperoneProfileApplyButton.enabled = true
                            } else {
                                summaryChaperoneProfileApplyButton.enabled = false
                            }
                        }
                    }

                    MyPushButton {
                        id: summaryChaperoneProfileApplyButton
                        enabled: false
                        Layout.preferredWidth: 150
                        text: qsTr("Select")
                        onClicked: {
                            if (summaryChaperoneProfileComboBox.currentIndex > 0) {
                                ChaperoneTabController.applyChaperoneProfile(summaryChaperoneProfileComboBox.currentIndex - 1)
                                OverlayController.setAutoChapProfileName(summaryChaperoneProfileComboBox.currentIndex - 1)
                                summaryChaperoneProfileComboBox.currentIndex = 0
                            }
                        }
                    }
                }
            }
        }

        SettingsGroupBox {
            title: qsTr("Advanced")
            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                MyToggleButton {
                    id: oculusSdkToggleButton
                    text: qsTr("Force Use SteamVR (Disable Oculus API [experimental])")
                    Layout.fillWidth: true
                    Layout.rightMargin: 0
                    onCheckedChanged: {
                        SettingsTabController.setOculusSdkToggle(this.checked, true)
                    }
                }
                LineSeparator {
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40

                    MyToggleButton {
                        id: vsyncDisabledToggle
                        text: qsTr("Disable App Vsync")
                        onCheckedChanged: {
                            OverlayController.setVsyncDisabled(checked, true)
                            customTickRateText.visible = checked
                            customTickRateLabel.visible = checked
                            customTickRateMsLabel.visible = checked
                        }
                    }

                    MyText {
                        id: customTickRateLabel
                        text: qsTr("Custom Tick Rate: ")
                        horizontalAlignment: Text.AlignRight
                        Layout.leftMargin: 20
                        Layout.rightMargin: 2
                    }

                    MyTextField {
                        id: customTickRateText
                        text: "20"
                        keyBoardUID: 501
                        Layout.preferredWidth: 140
                        Layout.leftMargin: 10
                        Layout.rightMargin: 1
                        horizontalAlignment: Text.AlignHCenter
                        function onInputEvent(input) {
                            var val = parseInt(input, 10)
                            if (!isNaN(val)) {
                                OverlayController.customTickRateMs = val
                                text = OverlayController.customTickRateMs
                            } else {
                                text = OverlayController.customTickRateMs
                            }
                        }
                    }

                    MyText {
                        id: customTickRateMsLabel
                        text: qsTr("ms")
                        horizontalAlignment: Text.AlignLeft
                        Layout.leftMargin: 1
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            Layout.fillWidth: true

            MyToggleButton {
                // set visible to true here in builds when we need a debug toggle checkbox, otherwise false when on master branch.
                visible: false
                id: debugToggle
                text: qsTr("Debug")
                onCheckedChanged: {
                    OverlayController.setEnableDebug(checked, true)
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MyPushButton {
                id: shutdownButton
                Layout.preferredWidth: 250
                text: qsTr("Shutdown OVRAS")
                onClicked: {
                    OverlayController.exitApp()
                    }
                }
        }

            Component.onCompleted: {
                settingsAutoStartToggle.checked = SettingsTabController.autoStartEnabled
                universeCenteredRotationToggle.checked = MoveCenterTabController.universeCenteredRotation
                disableCrashRecoveryToggle.checked = !OverlayController.crashRecoveryDisabled
                customTickRateText.text = OverlayController.customTickRateMs
                vsyncDisabledToggle.checked = OverlayController.vsyncDisabled
                customTickRateText.visible = vsyncDisabledToggle.checked
                customTickRateLabel.visible = vsyncDisabledToggle.checked
                customTickRateMsLabel.visible = vsyncDisabledToggle.checked
                disableVersionCheckToggle.checked = OverlayController.disableVersionCheck
                nativeChaperoneToggleButton.checked = SettingsTabController.nativeChaperoneToggle
                oculusSdkToggleButton.checked = SettingsTabController.oculusSdkToggle
                exclusiveInputToggleButton.checked = OverlayController.exclusiveInputEnabled
                autoApplyChaperoneToggleButton.checked = OverlayController.autoApplyChaperoneEnabled
                desktopModeToggleButton.checked = OverlayController.desktopModeToggle


                reloadChaperoneProfiles()
                volumeSlider.value = OverlayController.soundVolume
            }

        Connections {
            target: SettingsTabController
            function onAutoStartEnabledChanged() {
                settingsAutoStartToggle.checked = SettingsTabController.autoStartEnabled
            }
            function onNativeChaperoneToggleChanged(){
                nativeChaperoneToggleButton.checked = SettingsTabController.nativeChaperoneToggle
            }
            function onOculusSdkToggleChanged(){
                oculusSdkToggleButton.checked = SettingsTabController.oculusSdkToggle
            }
        }

        Connections {
            target: MoveCenterTabController
            function onUniverseCenteredRotationChanged() {
                universeCenteredRotationToggle.checked = MoveCenterTabController.universeCenteredRotation
            }
        }

        Connections {
            target: OverlayController
            function onVsyncDisabledChanged() {
                vsyncDisabledToggle.checked = OverlayController.vsyncDisabled
                customTickRateText.visible = vsyncDisabledToggle.checked
                customTickRateLabel.visible = vsyncDisabledToggle.checked
                customTickRateMsLabel.visible = vsyncDisabledToggle.checked
            }
            function onCrashRecoveryDisabledChanged() {
                disableCrashRecoveryToggle.checked = !OverlayController.crashRecoveryDisabled
            }

            function onCustomTickRateMsChanged() {
                customTickRateText.text = OverlayController.customTickRateMs
            }

            function onDisableVersionCheckChanged() {
                disableVersionCheckToggle.checked = OverlayController.disableVersionCheck
            }
            function onExclusiveInputEnabledChanged(){
                exclusiveInputToggleButton.checked = OverlayController.exclusiveInputEnabled
            }
            function onAutoApplyChaperoneEnabledChanged() {
               autoApplyChaperoneToggleButton.checked = OverlayController.autoApplyChaperoneEnabled
            }
            function onSoundVolumeChanged(){
                volumeSlider.value = OverlayController.soundVolume
            }
            function onDesktopModeToggleChanged(){
                desktopModeToggleButton.checked = OverlayController.desktopModeToggle
            }
        }
        Connections{
            target: ChaperoneTabController
            function onChaperoneProfilesUpdated() {
                reloadChaperoneProfiles()
            }
        }
    } // end scroll
    function reloadChaperoneProfiles() {
        var profiles = [""]
        var profileCount = ChaperoneTabController.getChaperoneProfileCount()
        for (var i = 0; i < profileCount; i++) {
            profiles.push(ChaperoneTabController.getChaperoneProfileName(i))
        }
        summaryChaperoneProfileComboBox.currentIndex = 0
        summaryChaperoneProfileComboBox.model = profiles
    }
}
