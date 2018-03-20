import QtQuick 2.7
import QtQuick.Controls 2.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file


CheckBox {
    checkState: Qt.Checked
    tristate: false
    hoverEnabled: true
    spacing: 12

	indicator: Rectangle {
		implicitWidth: 28
		implicitHeight: 28
		x: parent.leftPadding
		y: parent.height / 2 - height / 2
        color: parent.enabled ? (parent.down ? "#e0e0e0" : "#ffffff") : "#a0a0a0"
        border.width: 0
        Path {
            startX: 0
            startY: 0
            PathLine {
                x: 40
                y: 40
            }
        }

		Image {
            width: 28
            height: 28
			x: (parent.width - width) / 2
			y: (parent.height - height) / 2
            source: "check.svg"
			sourceSize.width: width
			sourceSize.height: height
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
        opacity: parent.activeFocus ? 1.0 : 0
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
