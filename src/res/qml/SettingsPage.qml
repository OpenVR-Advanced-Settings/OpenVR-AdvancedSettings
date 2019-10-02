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
            }
        }

        MyToggleButton {
            id: oldStyleMotionToggle
            text: "Old-Style Motion (per-frame disk writes)"
            onCheckedChanged: {
                MoveCenterTabController.setOldStyleMotion(checked, true)
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
            id: enableSeatedOffsetsRecenterToggle
            text: "Enable Auto-set Seated Offsets During Seated Recenter"
            onCheckedChanged: {
                MoveCenterTabController.setEnableSeatedOffsetsRecenter(checked, true)
            }
        }

        MyToggleButton {
            id: disableSeatedMotionToggle
            text: "Disable All Motion Features When in Seated Mode"
            onCheckedChanged: {
                MoveCenterTabController.setDisableSeatedMotion(checked, true)
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

        Item {
            Layout.fillHeight: true
        }


        Component.onCompleted: {
            settingsAutoStartToggle.checked = SettingsTabController.autoStartEnabled

            allowExternalEditsToggle.checked = MoveCenterTabController.allowExternalEdits
            oldStyleMotionToggle.checked = MoveCenterTabController.oldStyleMotion
            universeCenteredRotationToggle.checked = MoveCenterTabController.universeCenteredRotation
            enableSeatedOffsetsRecenterToggle.checked = MoveCenterTabController.enableSeatedOffsetsRecenter
            disableSeatedMotionToggle.checked = MoveCenterTabController.disableSeatedMotion

            customTickRateText.text = OverlayController.customTickRateMs
            vsyncDisabledToggle.checked = OverlayController.vsyncDisabled
            customTickRateText.visible = vsyncDisabledToggle.checked
            customTickRateLabel.visible = vsyncDisabledToggle.checked
            customTickRateMsLabel.visible = vsyncDisabledToggle.checked
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
            }
            onOldStyleMotionChanged: {
                oldStyleMotionToggle.checked = MoveCenterTabController.oldStyleMotion
            }
            onUniverseCenteredRotationChanged: {
                universeCenteredRotationToggle.checked = MoveCenterTabController.universeCenteredRotation
            }
            onEnableSeatedOffsetsRecenterChanged: {
                enableSeatedOffsetsRecenterToggle.checked = MoveCenterTabController.enableSeatedOffsetsRecenter
            }
            onDisableSeatedMotionChanged: {
                disableSeatedMotionToggle.checked = MoveCenterTabController.disableSeatedMotion
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
            onCustomTickRateMsChanged: {
                customTickRateText.text = OverlayController.customTickRateMs
            }
        }
    }
}
