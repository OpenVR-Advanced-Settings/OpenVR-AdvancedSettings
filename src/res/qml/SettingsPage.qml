import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "common"

MyStackViewPage {
    headerText: "Application Settings"

    content: ScrollView{
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true

            ColumnLayout {
            spacing: 18

            RowLayout{
                MyText {
                    text: "Application Volume:"
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

            MyToggleButton {
                id: settingsAutoStartToggle
                text: "Autostart"
                onCheckedChanged: {
                    SettingsTabController.setAutoStartEnabled(checked, false)
                }
            }
            MyToggleButton {
                id: desktopModeToggleButton
                text: "Desktop Mode (restart required)"
                onCheckedChanged: {
                    OverlayController.setDesktopModeToggle(this.checked, false)
                }
            }

            MyToggleButton {
                id: universeCenteredRotationToggle
                text: "Universe-Centered Rotation (Disables HMD Centering)"
                onCheckedChanged: {
                    MoveCenterTabController.setUniverseCenteredRotation(checked, true)
                }
            }


            MyToggleButton {
                id: disableCrashRecoveryToggle
                text: "Enable Automatic Crash Recovery of Chaperone Config"
                onCheckedChanged: {
                    OverlayController.setCrashRecoveryDisabled(!checked, true)
                }
            }

            MyToggleButton {
                id: disableVersionCheckToggle
                text: "Disable Notification of Newer Version Availability"
                onCheckedChanged: {
                    OverlayController.setDisableVersionCheck(checked, true)
                }
            }
            MyToggleButton {
                id: nativeChaperoneToggleButton
                text: "Force Use SteamVR Chaperone (experimental + SteamVR restart required)"
                onCheckedChanged: {
                    SettingsTabController.setNativeChaperoneToggle(this.checked, true)
                }
            }
            RowLayout {
                spacing: 18

                MyToggleButton {
                    id: autoApplyChaperoneToggleButton
                    text: "Auto Apply Chaperone Profile:"
                    onCheckedChanged: {
                        OverlayController.setAutoApplyChaperoneEnabled(this.checked, true)
                    }
                }

                MyComboBox {
                    id: summaryChaperoneProfileComboBox
                    Layout.leftMargin: 47
                    Layout.maximumWidth: 378
                    Layout.minimumWidth: 378
                    Layout.preferredWidth: 378
                    Layout.fillWidth: true
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
                    text: "Select"
                    onClicked: {
                        if (summaryChaperoneProfileComboBox.currentIndex > 0) {
                            ChaperoneTabController.applyChaperoneProfile(summaryChaperoneProfileComboBox.currentIndex - 1)
                            OverlayController.setAutoChapProfileName(summaryChaperoneProfileComboBox.currentIndex - 1)
                            summaryChaperoneProfileComboBox.currentIndex = 0
                        }
                    }
                }
            }
            MyToggleButton {
                id: oculusSdkToggleButton
                text: "Force Use SteamVR (Disable Oculus API [experimental])"
                onCheckedChanged: {
                    SettingsTabController.setOculusSdkToggle(this.checked, true)
                }
            }
            MyToggleButton {
                id: exclusiveInputToggleButton
                text: "Exclusive Input Toggle (This enables Key Binding to Toggle state)"
                onCheckedChanged: {
                    OverlayController.setExclusiveInputEnabled(this.checked, true)
                }
            }

            RowLayout {
                Layout.fillWidth: true

                MyToggleButton {
                    id: vsyncDisabledToggle
                    text: "Disable App Vsync"
                    onCheckedChanged: {
                        OverlayController.setVsyncDisabled(checked, true)
                        customTickRateText.visible = checked
                        customTickRateLabel.visible = checked
                        customTickRateMsLabel.visible = checked
                    }
                }

                MyText {
                    id: customTickRateLabel
                    text: "Custom Tick Rate: "
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
                    text: "ms"
                    horizontalAlignment: Text.AlignLeft
                    Layout.leftMargin: 1
                }

                Item {
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                id: debugStateRow
                Layout.fillWidth: true

                MyText {
                    id: debugStateLabel
                    text: "Debug State: "
                    horizontalAlignment: Text.AlignRight
                    Layout.leftMargin: 20
                    Layout.rightMargin: 2
                }

                MyTextField {
                    id: debugStateText
                    text: "0"
                    keyBoardUID: 502
                    Layout.preferredWidth: 140
                    Layout.leftMargin: 10
                    Layout.rightMargin: 1
                    horizontalAlignment: Text.AlignHCenter
                    function onInputEvent(input) {
                        var val = parseInt(input, 10)
                        if (!isNaN(val)) {
                            OverlayController.debugState = val
                            text = OverlayController.debugState
                        } else {
                            text = OverlayController.debugState
                        }
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
            }
            RowLayout{
                Item {
                    Layout.fillWidth: true
                }
                MyPushButton {
                    id: shutdownButton
                    Layout.preferredWidth: 250
                    text: "Shutdown OVRAS"
                    onClicked: {
                        OverlayController.exitApp()
                        }
                    }
            }

            Item {
                Layout.fillHeight: true
            }

            RowLayout {
                Layout.fillWidth: true


                Item {
                    Layout.fillWidth: true
                }

                MyToggleButton {
                    // set visible to true here in builds when we need a debug toggle checkbox, otherwise false when on master branch.
                    visible: false
                    id: debugToggle
                    text: "Debug"
                    onCheckedChanged: {
                        OverlayController.setEnableDebug(checked, true)
                    }
                }

            }
        } // end column
            Component.onCompleted: {
                settingsAutoStartToggle.checked = SettingsTabController.autoStartEnabled
                universeCenteredRotationToggle.checked = MoveCenterTabController.universeCenteredRotation
                disableCrashRecoveryToggle.checked = !OverlayController.crashRecoveryDisabled
                customTickRateText.text = OverlayController.customTickRateMs
                vsyncDisabledToggle.checked = OverlayController.vsyncDisabled
                customTickRateText.visible = vsyncDisabledToggle.checked
                customTickRateLabel.visible = vsyncDisabledToggle.checked
                customTickRateMsLabel.visible = vsyncDisabledToggle.checked
                debugStateRow.visible = OverlayController.enableDebug
                debugStateText.text = OverlayController.debugState
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
            onAutoStartEnabledChanged: {
                settingsAutoStartToggle.checked = SettingsTabController.autoStartEnabled
            }
            onNativeChaperoneToggleChanged:{
                nativeChaperoneToggleButton.checked = SettingsTabController.nativeChaperoneToggle
            }
            onOculusSdkToggleChanged:{
                oculusSdkToggleButton.checked = SettingsTabController.oculusSdkToggle
            }
        }

        Connections {
            target: MoveCenterTabController
            onUniverseCenteredRotationChanged: {
                universeCenteredRotationToggle.checked = MoveCenterTabController.universeCenteredRotation
            }
        }

        Connections {
            target: OverlayController
            onVsyncDisabledChanged: {
                vsyncDisabledToggle.checked = OverlayController.vsyncDisabled
                customTickRateText.visible = vsyncDisabledToggle.checked
                customTickRateLabel.visible = vsyncDisabledToggle.checked
                customTickRateMsLabel.visible = vsyncDisabledToggle.checked
            }
            onCrashRecoveryDisabledChanged: {
                disableCrashRecoveryToggle.checked = !OverlayController.crashRecoveryDisabled
            }

            onCustomTickRateMsChanged: {
                customTickRateText.text = OverlayController.customTickRateMs
            }

            onEnableDebugChanged: {
                debugStateRow.visible = OverlayController.enableDebug
            }

            onDebugStateChanged: {
                debugStateText.text = OverlayController.debugState
            }
            onDisableVersionCheckChanged: {
                disableVersionCheckToggle.checked = OverlayController.disableVersionCheck
            }
            onExclusiveInputEnabledChanged:{
                exclusiveInputToggleButton.checked = OverlayController.exclusiveInputEnabled
            }
            onAutoApplyChaperoneEnabledChanged: {
               autoApplyChaperoneToggleButton.checked = OverlayController.autoApplyChaperoneEnabled
            }
            onSoundVolumeChanged:{
                volumeSlider.value = OverlayController.soundVolume
            }
            onDesktopModeToggleChanged:{
                desktopModeToggleButton.checked = OverlayController.desktopModeToggle
            }
        }
        Connections{
            target: ChaperoneTabController
            onChaperoneProfilesUpdated: {
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
