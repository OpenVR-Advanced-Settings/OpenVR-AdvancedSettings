import QtQuick 2.7
import QtQuick.Controls 2.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file


CheckBox {
	hoverEnabled: true
	spacing: 12

	indicator: Rectangle {
		implicitWidth: 26
		implicitHeight: 26
		x: parent.leftPadding
		y: parent.height / 2 - height / 2
		radius: 3
		Rectangle {
			width: 18
			height: 18
			x: 4
			y: 4
			radius: 2
			color: "#1b2939"
			visible: parent.parent.checked
		}
	}

	contentItem: MyText {
        text: parent.text
        horizontalAlignment: Text.AlignLeft
		verticalAlignment: Text.AlignVCenter
        leftPadding: parent.indicator.width + parent.spacing
        color: parent.enabled ? "#ffffff" : "#909090"
	}

	background: Rectangle {
		color: "#2c435d"
        opacity: parent.activeFocus ? 1.0 : 0.0
    }

    onHoveredChanged: {
        if (hovered) {
            forceActiveFocus()
        } else {
            focus = false
        }
    }

    onClicked: {
        MyResources.activationSound.play()
    }
}
