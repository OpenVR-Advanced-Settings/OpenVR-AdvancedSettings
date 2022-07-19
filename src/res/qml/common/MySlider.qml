import QtQuick 2.7
import QtQuick.Controls 2.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file



Slider {
    snapMode: Slider.SnapAlways
    wheelEnabled: true
    hoverEnabled: true
    to: 1.0
    from: 0.0

    background: Rectangle {
        x: parent.leftPadding
        y: parent.topPadding + parent.availableHeight / 2 - height / 2
        width: parent.availableWidth
        height: parent.availableHeight
        radius: 2
        color: parent.activeFocus ? "#484f5b" : "#2a2e35"
        Rectangle {
            y: parent.height / 2 - height / 2
            implicitHeight: 4
            width: parent.width
            height: implicitHeight
            radius: 2
            color: "#181a1e"
        }
    }

    handle: Rectangle {
        x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
        y: parent.topPadding + parent.availableHeight / 2 - height / 2
        implicitWidth: 20
        implicitHeight: 40
        radius: 4
        color: parent.pressed ? "#586170" : "#3d4450"
        border.color: "#181a1e"
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
            //MyResources.playActivationSound()
		}
    }
}
