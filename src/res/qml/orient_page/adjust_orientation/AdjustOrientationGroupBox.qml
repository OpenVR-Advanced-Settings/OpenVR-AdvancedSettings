import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: adjustOrientationGroupBox
    Layout.fillWidth: true

    title: qsTr("Adjust Orientation")

    readonly property int stepDegrees: 5
    // Shared width so the lock switches line up despite different label lengths.
    readonly property real lockToggleWidth: Math.max(lockRollToggle.implicitWidth,
                                                     lockPitchToggle.implicitWidth,
                                                     lockYawToggle.implicitWidth)

    function formatDegrees(val) {
        return Math.round(val) + "°"
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
                    MoveCenterTabController.offsetRoll = 0
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "-"
                onClicked: {
                    MoveCenterTabController.modOffsetRoll(-stepDegrees)
                }
            }

            MyTextField {
                topPadding: 4
                bottomPadding: 4
                id: orientRollText
                text: "0°"
                keyBoardUID: 1101
                Layout.preferredWidth: 140
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        MoveCenterTabController.offsetRoll = val
                    }
                    text = formatDegrees(MoveCenterTabController.offsetRoll)
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "+"
                onClicked: {
                    MoveCenterTabController.modOffsetRoll(stepDegrees)
                }
            }

            MyToggleButton {
                topPadding: 4
                bottomPadding: 4
                id: lockRollToggle
                Layout.preferredWidth: lockToggleWidth
                Layout.leftMargin: 20
                text: qsTr("Lock Roll")
                onCheckedChanged: {
                    MoveCenterTabController.lockRollToggle = this.checked
                }
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
                    MoveCenterTabController.offsetPitch = 0
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "-"
                onClicked: {
                    MoveCenterTabController.modOffsetPitch(-stepDegrees)
                }
            }

            MyTextField {
                topPadding: 4
                bottomPadding: 4
                id: orientPitchText
                text: "0°"
                keyBoardUID: 1102
                Layout.preferredWidth: 140
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        MoveCenterTabController.offsetPitch = val
                    }
                    text = formatDegrees(MoveCenterTabController.offsetPitch)
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "+"
                onClicked: {
                    MoveCenterTabController.modOffsetPitch(stepDegrees)
                }
            }

            MyToggleButton {
                topPadding: 4
                bottomPadding: 4
                id: lockPitchToggle
                Layout.preferredWidth: lockToggleWidth
                Layout.leftMargin: 20
                text: qsTr("Lock Pitch")
                onCheckedChanged: {
                    MoveCenterTabController.lockPitchToggle = this.checked
                }
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
                    MoveCenterTabController.offsetYaw = 0
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "-"
                onClicked: {
                    MoveCenterTabController.modOffsetYaw(-stepDegrees)
                }
            }

            MyTextField {
                topPadding: 4
                bottomPadding: 4
                id: orientYawText
                text: "0°"
                keyBoardUID: 1103
                Layout.preferredWidth: 140
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        MoveCenterTabController.offsetYaw = val
                    }
                    text = formatDegrees(MoveCenterTabController.offsetYaw)
                }
            }

            MyPushButton2 {
                topPadding: 4
                bottomPadding: 4
                Layout.preferredWidth: 40
                text: "+"
                onClicked: {
                    MoveCenterTabController.modOffsetYaw(stepDegrees)
                }
            }

            MyToggleButton {
                topPadding: 4
                bottomPadding: 4
                id: lockYawToggle
                Layout.preferredWidth: lockToggleWidth
                Layout.leftMargin: 20
                text: qsTr("Lock Yaw")
                onCheckedChanged: {
                    MoveCenterTabController.lockYawToggle = this.checked
                }
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }

    Component.onCompleted: {
        orientRollText.text = formatDegrees(MoveCenterTabController.offsetRoll)
        orientPitchText.text = formatDegrees(MoveCenterTabController.offsetPitch)
        orientYawText.text = formatDegrees(MoveCenterTabController.offsetYaw)
        lockRollToggle.checked = MoveCenterTabController.lockRollToggle
        lockPitchToggle.checked = MoveCenterTabController.lockPitchToggle
        lockYawToggle.checked = MoveCenterTabController.lockYawToggle
    }

    Connections {
        target: MoveCenterTabController
        function onOffsetRollChanged() {
            orientRollText.text = formatDegrees(MoveCenterTabController.offsetRoll)
        }
        function onOffsetPitchChanged() {
            orientPitchText.text = formatDegrees(MoveCenterTabController.offsetPitch)
        }
        function onOffsetYawChanged() {
            orientYawText.text = formatDegrees(MoveCenterTabController.offsetYaw)
        }
        function onRequireLockRollChanged() {
            lockRollToggle.checked = MoveCenterTabController.lockRollToggle
        }
        function onRequireLockPitchChanged() {
            lockPitchToggle.checked = MoveCenterTabController.lockPitchToggle
        }
        function onRequireLockYawChanged() {
            lockYawToggle.checked = MoveCenterTabController.lockYawToggle
        }
    }
}
