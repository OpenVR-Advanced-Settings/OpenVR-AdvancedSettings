import QtQuick 2.7
import QtQuick.Controls 2.0
import ovras.advsettings
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
//button without a background
Button {
    hoverEnabled: true
    leftPadding: 10
    rightPadding: 10
    topPadding: 6
    bottomPadding: 6
    contentItem: MyText {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: parent.text
        color: parent.enabled ? (parent.activeFocus ? "#ffffff" : "#dcdedf") : "#5d636b"
    }
    background: Rectangle {
        color: parent.down ? "#1a9fff" : (parent.activeFocus ? "#4b5361" : "transparent")
        border.width: 0
        radius: 3
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
