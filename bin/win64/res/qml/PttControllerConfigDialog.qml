import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0

MyDialogOkCancelPopup {
    id: pttControllerConfigDialog
    property var pttControllerConfigClass
    property int controllerIndex: -1
    dialogTitle: "Controller Configuration"
    dialogWidth: 600
    dialogHeight: 620
    dialogContentItem: ColumnLayout {
        MyText {
            text: "Buttons"
        }
        Rectangle {
            color: "#cccccc"
            height: 1
            Layout.fillWidth: true
        }
        MyToggleButton {
            id: pttControllerConfigGripButtonToggle
            text: "Grip"
        }
        MyToggleButton {
            id: pttControllerConfigMenuButtonToggle
            text: "Menu"
        }
        MyToggleButton {
            id: pttControllerConfigTriggerButtonToggle
            text: "Trigger"
        }

        MyText {
            Layout.topMargin: 32
            text: "Touchpad"
        }
        Rectangle {
            color: "#cccccc"
            height: 1
            Layout.fillWidth: true
        }
        RowLayout {
            spacing: 64
            MyToggleButton {
                id: pttControllerConfigPadLeftToggle
                text: "Left"
            }
            MyToggleButton {
                id: pttControllerConfigPadRightToggle
                text: "Right"
            }
        }
        RowLayout {
            spacing: 64
            MyToggleButton {
                id: pttControllerConfigPadTopToggle
                text: "Top"
            }
            MyToggleButton {
                id: pttControllerConfigPadBottomToggle
                text: "Bottom"
            }
        }
        RowLayout {
            MyText {
                text: "Mode:"
            }
            MyComboBox {
                id: pttControllerConfigPadModeCombo
                Layout.preferredWidth: 250
                model: ["Disabled", "Touched", "Pressed"]
            }
        }
    }
    onClosed: {
        if (okClicked) {
            var buttons = []
            if (pttControllerConfigGripButtonToggle.checked) {
                buttons.push(2)
            }
            if (pttControllerConfigMenuButtonToggle.checked) {
                buttons.push(1)
            }
            var triggerMode = 0
            if (pttControllerConfigTriggerButtonToggle.checked) {
                triggerMode = 1
            }
            var padAreas = 0
            if (pttControllerConfigPadLeftToggle.checked) {
                padAreas |= (1 << 0)
            }
            if (pttControllerConfigPadTopToggle.checked) {
                padAreas |= (1 << 1)
            }
            if (pttControllerConfigPadRightToggle.checked) {
                padAreas |= (1 << 2)
            }
            if (pttControllerConfigPadBottomToggle.checked) {
                padAreas |= (1 << 3)
            }
            pttControllerConfigClass.setPttControllerConfig(controllerIndex, buttons, triggerMode, pttControllerConfigPadModeCombo.currentIndex, padAreas);
        }
    }
    function openPopup(controller) {
        if (controller === 0) {
            dialogTitle = "Left Controller Configuration"
            controllerIndex = 0
        } else {
            dialogTitle = "Right Controller Configuration"
            controllerIndex = 1
        }
        pttControllerConfigGripButtonToggle.checked = false
        pttControllerConfigMenuButtonToggle.checked = false
        pttControllerConfigTriggerButtonToggle.checked = false
        pttControllerConfigPadLeftToggle.checked = false
        pttControllerConfigPadTopToggle.checked = false
        pttControllerConfigPadRightToggle.checked = false
        pttControllerConfigPadBottomToggle.checked = false
        pttControllerConfigPadModeCombo.currentIndex = pttControllerConfigClass.pttTouchpadMode(controllerIndex)
        var buttons = pttControllerConfigClass.pttDigitalButtons(controllerIndex)
        for (var i = 0; i < buttons.length; i++) {
            if (buttons[i] == 1) {
                pttControllerConfigMenuButtonToggle.checked = true
            } else if (buttons[i] == 2) {
                pttControllerConfigGripButtonToggle.checked = true
            }
        }
        if (pttControllerConfigClass.pttTriggerMode(controllerIndex) > 0) {
            pttControllerConfigTriggerButtonToggle.checked = true
        }
        var padAreas = pttControllerConfigClass.pttTouchpadArea(controllerIndex)
        if (padAreas & (1 << 0)) {
            pttControllerConfigPadLeftToggle.checked = true
        }
        if (padAreas & (1 << 1)) {
            pttControllerConfigPadTopToggle.checked = true
        }
        if (padAreas & (1 << 2)) {
            pttControllerConfigPadRightToggle.checked = true
        }
        if (padAreas & (1 << 3)) {
            pttControllerConfigPadBottomToggle.checked = true
        }
        open()
    }
}
