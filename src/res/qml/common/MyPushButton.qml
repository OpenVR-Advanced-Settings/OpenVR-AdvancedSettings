import QtQuick 2.7
import QtQuick.Controls 2.0
import ovras.advsettings
import "." // QTBUG-34418, singletons require explicit import to load qmldir file

Button {
    // Overload this
    // Empty string = no icon
    property string iconPath: ""

    property bool activationSoundEnabled: true
    hoverEnabled: true
    leftPadding: 14
    rightPadding: 14
    topPadding: 8
    bottomPadding: 8
    contentItem: MyText {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: parent.text
        color: parent.enabled ? "#e4e8f0" : "#65676b"
    }
    background: Rectangle {
        radius: 6
        color: parent.enabled ? (parent.down ? "#171922" : (parent.activeFocus ? "#2a2f3a" : "#22242e")) : "#171922"
    }

    Image {
        source: iconPath
        height: parent.height
        width: height
    }

    onHoveredChanged: {
        if (hovered) {
            forceActiveFocus()
        } else {
            focus = false
        }
    }

    onClicked: {
        if (activationSoundEnabled) {
			OverlayController.playActivationSound()
        }
    }
    Connections {
    target: OverlayController
    }
}
