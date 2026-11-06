import QtQuick 2.7
import QtQuick.Controls 2.0
import ovras.advsettings
import "." // QTBUG-34418, singletons require explicit import to load qmldir file


ComboBox {
    id: myComboBox
    hoverEnabled: true
    leftPadding: 12
    rightPadding: 36
    topPadding: 8
    bottomPadding: 8
    background: Rectangle {
        radius: 3
        color: parent.pressed ? "#1a9fff" : (parent.activeFocus ? "#4b5361" : "#3a4049")
        border.width: 0
    }

    contentItem: MyText {
        leftPadding: 0
        rightPadding: 0
        text: parent.displayText
        color: parent.enabled ? "#ffffff" : "#5d636b"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    delegate: ItemDelegate {
        width: myComboBox.width
        text: modelData
        hoverEnabled: true
        leftPadding: 16
        rightPadding: 16
        contentItem: MyText {
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text: parent.text
            color: parent.enabled ? "#dcdedf" : "#5d636b"
        }
        background: Rectangle {
            radius: 3
            color: parent.pressed ? "#1a9fff" : (parent.hovered ? "#4b5361" : "#2a2e36")
        }
    }

    // Filled downward triangle, as used by Steam dropdowns.
    indicator: Canvas {
        x: parent.width - width - 12
        y: parent.topPadding + (parent.availableHeight - height) / 2
        width: 14
        height: 8
        contextType: "2d"

        onPaint: {
            if (!context) {
                getContext("2d")
            }
            context.reset();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width / 2, height);
            context.closePath();
            context.fillStyle = "#ffffff"
            context.fill()
        }
    }

    onHoveredChanged: {
        if (hovered) {
            forceActiveFocus()
        }
    }

    onActivated: {
		if (activeFocus) {
			OverlayController.playActivationSound()
		}
    }

    Component.onCompleted: {
        popup.background.color = "#2a2e36"
    }
    Connections {
    target: OverlayController 
    }
}
