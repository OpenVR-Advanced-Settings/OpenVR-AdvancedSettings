import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../desktop_overlay_imports"
import "../common"

MyStackViewPage {
    headerText: "Desktop Overlay"

    content: ColumnLayout {
        spacing: 18

        MyText {
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Text.AlignHCenter
            text: "Reopen overlay to apply."
        }

        SteamDesktopGroupBox {
        }

        SteamDesktopTranslationUpDown {
        }

        SteamDesktopTranslationBackForwards {
        }

        SteamDesktopTranslationLeftRight {
        }

        Item {
            Layout.fillHeight: true
        }

    }
}
