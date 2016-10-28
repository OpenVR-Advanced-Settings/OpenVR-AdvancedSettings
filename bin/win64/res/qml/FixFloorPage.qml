import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0



MyStackViewPage {
    headerText: "Floor Fix"

    content: ColumnLayout {
        spacing: 18

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        MyText {
            text: "Place one controller on the ground and ensure good visibility to the base stations."
            wrapMode: Text.WordWrap
            font.pointSize: 28
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        MyText {
            id: statusMessageText
            enabled: false
            text: "Status Text"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        MyPushButton {
            id: fixButton
            Layout.fillWidth: true
            text: "Fix Floor"
            Layout.preferredHeight: 100
            onClicked: {
                FixFloorTabController.fixFloorClicked()
            }
        }

        MyPushButton {
            id: undoFixButton
            enabled: false
            Layout.fillWidth: true
            text: "Undo Fix"
            onClicked: {
                FixFloorTabController.undoFixFloorClicked()
            }
        }

        Item {
            Layout.preferredHeight: 32
        }

        Component.onCompleted: {
            statusMessageText.text = ""
            undoFixButton.enabled = FixFloorTabController.canUndo
            fixButton.enabled = true
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
            onStatusMessageSignal: {
                if (statusMessageTimer.running) {
                    statusMessageTimer.stop()
                }
                statusMessageText.text = FixFloorTabController.currentStatusMessage()
                statusMessageTimer.interval = FixFloorTabController.currentStatusMessageTimeout() * 1000
                statusMessageTimer.start()
            }
            onMeasureStartSignal: {
                fixButton.enabled = false
                undoFixButton.enabled = false
            }
            onMeasureEndSignal: {
                fixButton.enabled = true
                undoFixButton.enabled = FixFloorTabController.canUndo
            }
            onCanUndoChanged: {
                undoFixButton.enabled = FixFloorTabController.canUndo
            }
        }

    }

}
