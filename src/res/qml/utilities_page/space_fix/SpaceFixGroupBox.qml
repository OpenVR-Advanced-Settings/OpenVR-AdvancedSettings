import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: spaceFixGroupBox
    Layout.fillWidth: true

    title: qsTr("Space Fix")

    ColumnLayout {
        anchors.fill: parent

        MyText {
            text: qsTr("Place one controller on the ground, and ensure it is tracked.")
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        MyText {
            id: seatedWarningText
            text: qsTr("(Disabled in 'Seated' Universe Type)")
            visible: false
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        MyText {
            id: statusMessageText
            enabled: false
            text: ""
            visible: text !== ""
            Layout.fillWidth: true
        }

        MyPushButton {
            id: fixButton
            Layout.fillWidth: true
            text: qsTr("Fix Floor")
            onClicked: {
                FixFloorTabController.fixFloorClicked()
            }
        }

        LineSeparator {
        }

        MyPushButton {
            id: recenterButton
            Layout.fillWidth: true
            text: qsTr("Recenter Space")
            onClicked: {
                FixFloorTabController.recenterClicked()
            }
        }

        LineSeparator {
            visible: undoFixButton.visible
        }

        MyPushButton {
            id: undoFixButton
            enabled: false
            // TODO re-enable undo and remove visible: false
            visible: false
            Layout.fillWidth: true
            text: qsTr("Undo Fix")
            onClicked: {
                FixFloorTabController.undoFixFloorClicked()
            }
        }

        LineSeparator {
        }

        MyPushButton {
            id: zeroSpaceButton
            Layout.fillWidth: true
            text: qsTr("Apply Space Settings Offsets as Center")
            onClicked: {
                MoveCenterTabController.addCurOffsetAsCenter()
            }
        }

        LineSeparator {
        }

        MyPushButton {
            id: revertButton
            Layout.fillWidth: true
            text: qsTr("Revert All Changes from This Session")
            onClicked: {
                ChaperoneTabController.applyAutosavedProfile()
            }
        }
    }

    Component.onCompleted: {
        statusMessageText.text = ""
        //undoFixButton.enabled = FixFloorTabController.canUndo
        fixButton.enabled = true

        if (MoveCenterTabController.trackingUniverse === 0) {
            fixButton.enabled = false
            recenterButton.enabled = false
            zeroSpaceButton.enabled = false
            revertButton.enabled = false
            undoFixButton.enabled = false
            seatedWarningText.visible = true
        }
    }

    Timer {
        id: statusMessageTimer
        repeat: false
        onTriggered: {
            statusMessageText.text = ""
        }
    }

    Connections {
        target: FixFloorTabController
        function onStatusMessageSignal() {
            if (statusMessageTimer.running) {
                statusMessageTimer.stop()
            }
            statusMessageText.text = FixFloorTabController.currentStatusMessage()
            statusMessageTimer.interval = FixFloorTabController.currentStatusMessageTimeout() * 1000
            statusMessageTimer.start()
        }
        function onMeasureStartSignal() {
            fixButton.enabled = false
            undoFixButton.enabled = false
        }
        function onMeasureEndSignal() {
            fixButton.enabled = true
            // undoFixButton.enabled = FixFloorTabController.canUndo
        }
        function onCanUndoChanged() {
            //undoFixButton.enabled = FixFloorTabController.canUndo
            // revert below to this -^
            undoFixButton.enabled = false
        }
    }

    Connections {
        target: MoveCenterTabController
        function onTrackingUniverseChanged(value) {
            fixButton.enabled = true
            recenterButton.enabled = true
            zeroSpaceButton.enabled = true
            revertButton.enabled = true
            // undoFixButton.enabled = true
            // TODO Fix Undo Feature -^
            seatedWarningText.visible = false
        }
    }
}
