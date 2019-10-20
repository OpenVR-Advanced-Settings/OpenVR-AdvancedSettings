import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "common"

MyStackViewPage {
    headerText: "Application Settings"

    content: ColumnLayout {
        spacing: 18

        MyToggleButton {
            id: settingsAutoStartToggle
            text: "Autostart"
            onCheckedChanged: {
                SettingsTabController.setAutoStartEnabled(checked, false)
            }
        }

        MyToggleButton {
            id: allowExternalEditsToggle
            text: "Allow External App Chaperone Edits (Danger)"
            onCheckedChanged: {
                MoveCenterTabController.setAllowExternalEdits(checked, true)
                seatedOldExternalWarning.visible = checked && MoveCenterTabController.oldStyleMotion && MoveCenterTabController.enableSeatedMotion
            }
        }

        MyToggleButton {
            id: oldStyleMotionToggle
            text: "Old-Style Motion (per-frame disk writes)"
            onCheckedChanged: {
                MoveCenterTabController.setOldStyleMotion(checked, true)
                seatedOldExternalWarning.visible = MoveCenterTabController.allowExternalEdits && checked && MoveCenterTabController.enableSeatedMotion
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
            id: enableSeatedMotionToggle
            text: "Enable Motion Features When in Seated Mode (Experimental)"
            onCheckedChanged: {
                MoveCenterTabController.setEnableSeatedMotion(checked, true)
                seatedOldExternalWarning.visible = MoveCenterTabController.allowExternalEdits && MoveCenterTabController.oldStyleMotion && checked
            }
        }

        MyToggleButton {
            id: disableCrashRecoveryToggle
            text: "Disable Automatic Crash Recovery of Chaperone Config"
            onCheckedChanged: {
                OverlayController.setCrashRecoveryDisabled(checked, true)
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
                keyBoardUID: 1001
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
                keyBoardUID: 1002
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

        MyText {
            id: seatedOldExternalWarning
            wrapMode: Text.WordWrap
            font.pointSize: 20
            color: "#FFA500"
            text: "WARNING: 'Allow External App Chaperone Edits' + 'Old-Style Motion' + 'Enable Motion Features When in Seated Mode' active together may cause space center misalignment. Load the «Autosaved Profile» in the 'Chaperone' tab to fix."
            horizontalAlignment: Text.AlignHCenter
            Layout.leftMargin: 20
            Layout.rightMargin: 20
            Layout.fillWidth: true
        }

        Item {
            Layout.fillHeight: true
        }


        Component.onCompleted: {
            settingsAutoStartToggle.checked = SettingsTabController.autoStartEnabled

            allowExternalEditsToggle.checked = MoveCenterTabController.allowExternalEdits
            oldStyleMotionToggle.checked = MoveCenterTabController.oldStyleMotion
            universeCenteredRotationToggle.checked = MoveCenterTabController.universeCenteredRotation
            enableSeatedMotionToggle.checked = MoveCenterTabController.enableSeatedMotion

            disableCrashRecoveryToggle.checked = OverlayController.crashRecoveryDisabled
            customTickRateText.text = OverlayController.customTickRateMs
            vsyncDisabledToggle.checked = OverlayController.vsyncDisabled
            customTickRateText.visible = vsyncDisabledToggle.checked
            customTickRateLabel.visible = vsyncDisabledToggle.checked
            customTickRateMsLabel.visible = vsyncDisabledToggle.checked
            debugStateRow.visible = OverlayController.enableDebug
            debugStateText.text = OverlayController.debugState

            seatedOldExternalWarning.visible = MoveCenterTabController.allowExternalEdits && MoveCenterTabController.oldStyleMotion && MoveCenterTabController.enableSeatedMotion
        }

        Connections {
            target: SettingsTabController
            onAutoStartEnabledChanged: {
                settingsAutoStartToggle.checked = SettingsTabController.autoStartEnabled
            }
        }

        Connections {
            target: MoveCenterTabController
            onAllowExternalEditsChanged: {
                allowExternalEditsToggle.checked = MoveCenterTabController.allowExternalEdits
                seatedOldExternalWarning.visible = MoveCenterTabController.allowExternalEdits && MoveCenterTabController.oldStyleMotion && MoveCenterTabController.enableSeatedMotion
            }
            onOldStyleMotionChanged: {
                oldStyleMotionToggle.checked = MoveCenterTabController.oldStyleMotion
                seatedOldExternalWarning.visible = MoveCenterTabController.allowExternalEdits && MoveCenterTabController.oldStyleMotion && MoveCenterTabController.enableSeatedMotion
            }
            onUniverseCenteredRotationChanged: {
                universeCenteredRotationToggle.checked = MoveCenterTabController.universeCenteredRotation
            }
            onEnableSeatedMotionChanged: {
                enableSeatedMotionToggle.checked = MoveCenterTabController.enableSeatedMotion
                seatedOldExternalWarning.visible = MoveCenterTabController.allowExternalEdits && MoveCenterTabController.oldStyleMotion && MoveCenterTabController.enableSeatedMotion
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
                disableCrashRecoveryToggle.checked = OverlayController.crashRecoveryDisabled
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
        }
    }
}
