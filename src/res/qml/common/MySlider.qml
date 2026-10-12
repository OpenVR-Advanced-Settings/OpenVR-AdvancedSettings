import QtQuick 2.7
import QtQuick.Controls 2.0
import ovras.advsettings
import "." // QTBUG-34418, singletons require explicit import to load qmldir file



Slider {
    snapMode: Slider.SnapAlways
    wheelEnabled: true
    hoverEnabled: true
    to: 1.0
    from: 0.0
    leftPadding: 12
    rightPadding: 12
    topPadding: 12
    bottomPadding: 12

    property color fillColor: "#1682e0"

    background: Rectangle {
        x: parent.leftPadding
        y: parent.topPadding + parent.availableHeight / 2 - height / 2
        width: parent.availableWidth
        height: 4
        radius: 2
        color: "#2f2f38"
        clip: true
        Rectangle {
            width: parent.width * (parent.parent.position)
            height: parent.height
            radius: 2
            color: parent.parent.fillColor
        }
    }

    handle: Rectangle {
        x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
        y: parent.topPadding + parent.availableHeight / 2 - height / 2
        implicitWidth: 20
        implicitHeight: 20
        radius: 10
        color: "#ffffff"
        border.width: 0
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
             //OverlayController.playActivationSound()
		}
    }
}
