import QtQuick 2.7
import QtQuick.Controls 2.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file

Button {
    hoverEnabled: true
    contentItem: MyText {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: parent.text
        color: parent.enabled ? "#ffffff" : "#909090"
    }
    background: Rectangle {
        color: parent.down ? "#406288" : (parent.activeFocus ? "#365473" : "transparent")
        border.color: parent.enabled ? "#ffffff" : "#909090"
        radius: 8
    }
    onHoveredChanged: {
        if (hovered) {
            forceActiveFocus()
        } else {
            focus = false
        }
    }

    onClicked: {
		MyResources.playActivationSound()
    }
}
