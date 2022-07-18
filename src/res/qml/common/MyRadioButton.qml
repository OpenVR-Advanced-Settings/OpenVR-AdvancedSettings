import QtQuick 2.7
import QtQuick.Controls 2.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file

RadioButton {
    hoverEnabled: true
    contentItem: MyText {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: parent.text
        color: parent.enabled ? "#d9dbe0" : "#65676b"
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
