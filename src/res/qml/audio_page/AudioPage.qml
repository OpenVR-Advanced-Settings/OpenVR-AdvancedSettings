import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "device_selector"
import "proximity"
import "push_to_talk"
import "profiles"

MyStackViewPage {
    headerText: qsTr("Audio Settings")

    // Every group box row on this page uses this height.
    readonly property int groupBoxRowHeight: 44

    content: ColumnLayout {
        spacing: 8

        SettingsGroupBox {
            rowHeight: groupBoxRowHeight
            title: qsTr("Output")
            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                AudioDeviceSelector {
                    Layout.fillWidth: true
                    Layout.preferredHeight: groupBoxRowHeight
                }
                LineSeparator {
                }
                MirrorVolumeSlider {
                    Layout.fillWidth: true
                    rowHeight: groupBoxRowHeight
                }
            }
        }

        SettingsGroupBox {
            rowHeight: groupBoxRowHeight
            title: qsTr("Voice")
            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                MicVolumeSlider {
                    Layout.fillWidth: true
                    rowHeight: groupBoxRowHeight
                }
                LineSeparator {
                }
                ProximityToggle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: groupBoxRowHeight
                }
            }
        }

        SettingsGroupBox {
            rowHeight: groupBoxRowHeight
            title: qsTr("Push-to-Talk")
            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                PttButtons {
                    Layout.fillWidth: true
                    rowHeight: groupBoxRowHeight
                }
            }
        }
    }
}
