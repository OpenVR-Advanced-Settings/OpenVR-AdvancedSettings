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
    leftPadding: 18
    rightPadding: 18
    topPadding: 8
    bottomPadding: 8
    contentItem: MyText {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: parent.text
        color: parent.enabled ? (parent.activeFocus ? "#ffffff" : "#dcdedf") : "#5d636b"
    }
    background: Rectangle {
        radius: 3
        color: parent.enabled ? (parent.down ? "#1a9fff" : (parent.activeFocus ? "#4b5361" : "#3a4049")) : "#32363e"
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
