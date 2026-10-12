import QtQuick 2.7
import QtQuick.Controls 2.0
import ovras.advsettings
import "." // QTBUG-34418, singletons require explicit import to load qmldir file

Button {
    hoverEnabled: true
    leftPadding: 12
    rightPadding: 12
    topPadding: 8
    bottomPadding: 8
    contentItem: MyText {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: parent.text
        color: parent.enabled ? "#e4e8f0" : "#65676b"
    }
    background: Rectangle {
        color: parent.down ? "#171922" : (parent.activeFocus ? "#2a2f3a" : "transparent")
        border.width: 0
        radius: 6
    }
    onHoveredChanged: {
        if (hovered) {
            forceActiveFocus()
        } else {
            focus = false
        }
    }

    onClicked: {
		OverlayController.playActivationSound()
    }
    Connections {
    target: OverlayController
    }
}
