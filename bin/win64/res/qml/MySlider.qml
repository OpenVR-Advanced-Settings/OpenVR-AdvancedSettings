import QtQuick 2.7
import QtQuick.Controls 2.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file



Slider {
    snapMode: Slider.SnapAlways
    wheelEnabled: true
    hoverEnabled: true

    background: Rectangle {
        x: parent.leftPadding
        y: parent.topPadding + parent.availableHeight / 2 - height / 2
        width: parent.availableWidth
        height: parent.availableHeight
        radius: 2
        color: parent.activeFocus ? "#2c435d" : "#1b2939"
        Rectangle {
            y: parent.height / 2 - height / 2
            implicitHeight: 4
            width: parent.width
            height: implicitHeight
            radius: 2
            color: "#bdbebf"
        }
    }

    handle: Rectangle {
        x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
        y: parent.topPadding + parent.availableHeight / 2 - height / 2
        implicitWidth: 20
        implicitHeight: 40
        radius: 4
        color: parent.pressed ? "#ffffff" : "#eeeeee"
        border.color: "#bdbebf"
    }

    onHoveredChanged: {
        if (hovered) {
            forceActiveFocus()
        } else {
            focus = false
        }
    }

    onValueChanged: {
		if (activeFocus) {
			MyResources.playActivationSound()
		}
    }
}
