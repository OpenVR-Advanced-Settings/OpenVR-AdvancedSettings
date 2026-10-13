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
    headerText: "Audio Settings"

    content: ColumnLayout {
        spacing: 18

        SettingsGroupBox {
            title: "Output"
            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                AudioDeviceSelector {
                    Layout.fillWidth: true
                }
                LineSeparator {
                }
                MirrorVolumeSlider {
                    Layout.fillWidth: true
                }
            }
        }

        SettingsGroupBox {
            title: "Voice"
            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                MicVolumeSlider {
                    Layout.fillWidth: true
                }
                LineSeparator {
                }
                ProximityToggle {
                    Layout.fillWidth: true
                }
            }
        }

        SettingsGroupBox {
            title: "Push-to-Talk"
            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                PttButtons {
                    Layout.fillWidth: true
                }
                LineSeparator {
                }
                ProfileButtons {
                    Layout.fillWidth: true
                }
            }
        }
    }
}
