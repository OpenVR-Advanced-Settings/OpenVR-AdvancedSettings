import QtQuick 2.7
import QtQuick.Controls 2.0
import ovras.advsettings
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
// Needed for OverlayControllers in the default folder.
import ".."
import QtQuick.Layouts 1.3

// Steam style switch: label on the left, switch on the right.
// When given extra width (Layout.fillWidth) the switch sits at the far right.
CheckBox {
    id: myToggle
    tristate: false
    hoverEnabled: true
    spacing: 14
    leftPadding: 0
    rightPadding: 0
    // Keep the switch from touching whatever follows it in a row.
    Layout.rightMargin: 12

    property color onColor: "#1a9fff"

    implicitWidth: contentItem.implicitWidth

    indicator: Rectangle {
        implicitWidth: 46
        implicitHeight: 24
        x: myToggle.width - width - myToggle.rightPadding
        y: myToggle.height / 2 - height / 2
        radius: implicitHeight / 2
        color: myToggle.checked ? myToggle.onColor : (myToggle.activeFocus ? "#5a616c" : "#4a505a")
        border.width: 0

        Rectangle {
            id: thumb
            width: 20
            height: 20
            radius: 10
            color: myToggle.enabled ? "#ffffff" : "#8b929a"
            y: (parent.height - height) / 2
            x: myToggle.checked ? (parent.width - width - 2) : 2
            Behavior on x {
                NumberAnimation { duration: 120; easing.type: Easing.InOutQuad }
            }
        }
    }

    contentItem: MyText {
        text: myToggle.text
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        leftPadding: 0
        // The control sizes contentItem to its full width, reserve the switch area.
        rightPadding: myToggle.indicator.width + (text.length > 0 ? myToggle.spacing : 0)
        elide: Text.ElideRight
        color: myToggle.enabled ? "#dcdedf" : "#5d636b"
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
