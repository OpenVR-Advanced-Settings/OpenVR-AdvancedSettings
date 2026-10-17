import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

// UI only for now, values are held here until a tab controller exists.
SettingsGroupBox {
    id: adjustOrientationGroupBox
    Layout.fillWidth: true

    title: qsTr("Adjust Orientation")

    property int roll: 0
    property int pitch: 0
    property int yaw: 0
    readonly property int stepDegrees: 5
    // Shared width so the lock switches line up despite different label lengths.
    readonly property real lockToggleWidth: Math.max(lockRollToggle.implicitWidth,
                                                     lockPitchToggle.implicitWidth,
                                                     lockYawToggle.implicitWidth)

    function clampDegrees(val) {
        return Math.max(-180, Math.min(180, Math.round(val)))
    }

    // Parses keyboard input, falling back to the current value when it isn't a number.
    function parseDegrees(input, current) {
        var val = parseInt(input, 10)
        return isNaN(val) ? current : clampDegrees(val)
    }

    ColumnLayout {
        anchors.fill: parent

        GridLayout {
            Layout.fillWidth: true
            columns: 7
            rowSpacing: 0

            MyText {
                text: qsTr("Roll")
                Layout.preferredWidth: 340
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "0"
                onClicked: {
                    roll = 0
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "-"
                onClicked: {
                    roll = clampDegrees(roll - stepDegrees)
                }
            }

            MyTextField {
                topPadding: 4
                bottomPadding: 4
                id: orientRollText
                text: roll + "°"
                keyBoardUID: 1101
                Layout.preferredWidth: 140
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    roll = parseDegrees(input, roll)
                    text = roll + "°"
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "+"
                onClicked: {
                    roll = clampDegrees(roll + stepDegrees)
                }
            }

            MyToggleButton {
                topPadding: 4
                bottomPadding: 4
                id: lockRollToggle
                Layout.preferredWidth: lockToggleWidth
                Layout.leftMargin: 20
                text: qsTr("Lock Roll")
            }
            Item {
                Layout.fillWidth: true
            }

            MyText {
                text: qsTr("Pitch")
                Layout.preferredWidth: 340
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "0"
                onClicked: {
                    pitch = 0
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "-"
                onClicked: {
                    pitch = clampDegrees(pitch - stepDegrees)
                }
            }

            MyTextField {
                topPadding: 4
                bottomPadding: 4
                id: orientPitchText
                text: pitch + "°"
                keyBoardUID: 1102
                Layout.preferredWidth: 140
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    pitch = parseDegrees(input, pitch)
                    text = pitch + "°"
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "+"
                onClicked: {
                    pitch = clampDegrees(pitch + stepDegrees)
                }
            }

            MyToggleButton {
                topPadding: 4
                bottomPadding: 4
                id: lockPitchToggle
                Layout.preferredWidth: lockToggleWidth
                Layout.leftMargin: 20
                text: qsTr("Lock Pitch")
            }
            Item {
                Layout.fillWidth: true
            }

            MyText {
                text: qsTr("Yaw")
                Layout.preferredWidth: 340
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "0"
                onClicked: {
                    yaw = 0
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "-"
                onClicked: {
                    yaw = clampDegrees(yaw - stepDegrees)
                }
            }

            MyTextField {
                topPadding: 4
                bottomPadding: 4
                id: orientYawText
                text: yaw + "°"
                keyBoardUID: 1103
                Layout.preferredWidth: 140
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    yaw = parseDegrees(input, yaw)
                    text = yaw + "°"
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "+"
                onClicked: {
                    yaw = clampDegrees(yaw + stepDegrees)
                }
            }

            MyToggleButton {
                topPadding: 4
                bottomPadding: 4
                id: lockYawToggle
                Layout.preferredWidth: lockToggleWidth
                Layout.leftMargin: 20
                text: qsTr("Lock Yaw")
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }

    // Typing into a field breaks its text binding, so refresh the text on every change.
    onRollChanged: orientRollText.text = roll + "°"
    onPitchChanged: orientPitchText.text = pitch + "°"
    onYawChanged: orientYawText.text = yaw + "°"
}
