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

    property color fillColor: "#1a9fff"

    background: Item {
        x: parent.leftPadding
        y: parent.topPadding + parent.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: 6
        width: parent.availableWidth
        height: 6
        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width
            height: 4
            radius: 2
            color: "#4a505a"
        }
        Rectangle {
            width: parent.parent.visualPosition * parent.width
            height: parent.height
            radius: 3
            color: parent.parent.enabled ? parent.parent.fillColor : "#5d636b"
        }
    }

    handle: Rectangle {
        x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
        y: parent.topPadding + parent.availableHeight / 2 - height / 2
        implicitWidth: 24
        implicitHeight: 24
        radius: 12
        color: parent.pressed ? "#e6e6e6" : "#ffffff"
        border.width: parent.activeFocus ? 2 : 0
        border.color: "#1a9fff"
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
