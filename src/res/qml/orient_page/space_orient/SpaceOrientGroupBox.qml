import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

// UI only for now, not yet connected to a tab controller.
SettingsGroupBox {
    id: spaceOrientGroupBox
    Layout.fillWidth: true

    title: qsTr("Space Orient")

    // Empty string means no override device is set.
    property string leftOverrideDevice: ""
    property string rightOverrideDevice: ""
    property string customOverrideDevice: ""

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            MyToggleButton {
                id: orientShortcutLeft
                text: qsTr("Left Hand")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Layout.rightMargin: 0
            }

            VerticalSeparator {
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredWidth: 1

                MyPushButton2 {
                    text: qsTr("Override")
                    onClicked: {
                        spaceOrientDeviceDialog.openPopup(qsTr("Left Hand Override"),
                                                          leftOverrideDevice,
                                                          function(device) { leftOverrideDevice = device })
                    }
                }

                MyText {
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideRight
                    text: leftOverrideDevice === "" ? qsTr("Not Set") : leftOverrideDevice
                }
            }
        }

        LineSeparator {
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            MyToggleButton {
                id: orientShortcutRight
                text: qsTr("Right Hand")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Layout.rightMargin: 0
            }

            VerticalSeparator {
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredWidth: 1

                MyPushButton2 {
                    text: qsTr("Override")
                    onClicked: {
                        spaceOrientDeviceDialog.openPopup(qsTr("Right Hand Override"),
                                                          rightOverrideDevice,
                                                          function(device) { rightOverrideDevice = device })
                    }
                }

                MyText {
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideRight
                    text: rightOverrideDevice === "" ? qsTr("Not Set") : rightOverrideDevice
                }
            }
        }

        LineSeparator {
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            MyToggleButton {
                id: orientShortcutCustom
                text: qsTr("Custom")
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Layout.rightMargin: 0
            }

            VerticalSeparator {
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredWidth: 1

                MyPushButton2 {
                    text: qsTr("Override")
                    onClicked: {
                        spaceOrientDeviceDialog.openPopup(qsTr("Custom Override"),
                                                          customOverrideDevice,
                                                          function(device) { customOverrideDevice = device })
                    }
                }

                MyText {
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideRight
                    text: customOverrideDevice === "" ? qsTr("Not Set") : customOverrideDevice
                }
            }
        }
    }
}
