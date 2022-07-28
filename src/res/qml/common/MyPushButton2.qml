import QtQuick 2.7
import QtQuick.Controls 2.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file

Button {
    hoverEnabled: true
    contentItem: MyText {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: parent.text
        color: parent.enabled ? "#d9dbe0" : "#65676b"
    }
    background: Rectangle {
        color: parent.down ? "#282b31" : (parent.activeFocus ? "#484f5b" : "transparent")
        border.color: parent.enabled ? "#d9dbe0" : "#65676b"
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
