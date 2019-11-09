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
    width: 1200
    headerText: "Audio Settings"

    content: ColumnLayout {
        spacing: 24
        AudioDeviceSelector {
        }

        MirrorVolumeSlider {
        }

        MicVolumeSlider {
        }
        ColumnLayout {
            spacing: 18
            ProximityToggle {
            }
            PttButtons {
            }
            ProfileButtons {
            }
        }
    }
}
