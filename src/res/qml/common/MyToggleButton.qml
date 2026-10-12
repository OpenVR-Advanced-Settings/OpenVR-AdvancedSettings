import QtQuick 2.7
import QtQuick.Controls 2.0
import ovras.advsettings
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
// Needed for OverlayControllers in the default folder.
import ".."

CheckBox {
    id: myToggle
    tristate: false
    hoverEnabled: true
    spacing: 16
    leftPadding: 0
    rightPadding: 0

    property color onColor: "#1682e0"

    indicator: Rectangle {
        implicitWidth: 48
        implicitHeight: 26
        x: myToggle.leftPadding
        y: myToggle.height / 2 - height / 2
        radius: implicitHeight / 2
        color: myToggle.checked ? myToggle.onColor : "#2f2f38"
        border.width: 0

        Rectangle {
            id: thumb
            width: 20
            height: 20
            radius: 10
            color: "#ffffff"
            y: (parent.height - height) / 2
            x: myToggle.checked ? (parent.width - width - 3) : 3
            Behavior on x {
                NumberAnimation { duration: 120; easing.type: Easing.InOutQuad }
            }
        }
    }

    contentItem: MyText {
        text: myToggle.text
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        leftPadding: myToggle.indicator.width + myToggle.spacing
        rightPadding: myToggle.rightPadding
        color: myToggle.enabled ? "#e4e8f0" : "#65676b"
    }

    background: Rectangle {
        color: "transparent"
    }

    onHoveredChanged: {
        if (hovered) {
            forceActiveFocus()
        } else {
            focus = false
        }
    }

    onClicked: {
        OverlayController.playActivationSound()
    }

    Connections {
    target: OverlayController
    }
}
