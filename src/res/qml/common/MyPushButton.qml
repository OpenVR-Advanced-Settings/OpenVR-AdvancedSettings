import QtQuick 2.7
import QtQuick.Controls 2.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file

Button {
    // Overload this
    // Empty string = no icon
    property string iconPath: ""

    property bool activationSoundEnabled: true
	hoverEnabled: true
	contentItem: MyText {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: parent.text
        color: parent.enabled ? "#d9dbe0" : "#65676b"
	}
	background: Rectangle {
        color: parent.enabled? (parent.down ? "#181a1e" : (parent.activeFocus ? "#586170" : "#3d4450")): "#181a1e"
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
			MyResources.playActivationSound()
        }
    }
}
