import QtQuick 2.7
import QtQuick.Controls 2.0
import ovras.advsettings
import "." // QTBUG-34418, singletons require explicit import to load qmldir file


ComboBox {
    id: myComboBox
    hoverEnabled: true
    leftPadding: 14
    rightPadding: 30
    topPadding: 8
    bottomPadding: 8
    background: Rectangle {
        radius: 6
        color: parent.pressed ? "#171922" : (parent.activeFocus ? "#2a2f3a" : "#1e2029")
        border.width: 1
        border.color: parent.activeFocus ? "#1682e0" : "#343a46"
    }

    contentItem: MyText {
        leftPadding: 0
        rightPadding: parent.indicator.width + parent.spacing
        text: parent.displayText
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    delegate: ItemDelegate {
        width: myComboBox.width
        text: modelData
        hoverEnabled: true
        leftPadding: 14
        rightPadding: 14
        contentItem: MyText {
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text: parent.text
            color: parent.enabled ? "#e4e8f0" : "#65676b"
        }
        background: Rectangle {
            radius: 6
            color: parent.pressed ? "#171922" : (parent.hovered ? "#2a2f3a" : "#1e2029")
        }
    }

    indicator: Canvas {
        x: parent.width - width - parent.rightPadding
        y: parent.topPadding + (parent.availableHeight - height) / 2
        width: 13
        height: 7
        contextType: "2d"

        onPaint: {
            if (!context) {
                getContext("2d")
            }
            context.reset();
            context.lineWidth = 2
            context.moveTo(1, 1);
            context.lineTo(Math.ceil(width / 2), height - 1);
            context.lineTo(width - 1, 1);
            context.strokeStyle = "#ffffff"
            context.stroke()
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
        popup.background.color = "#1e2029"
    }
    Connections {
    target: OverlayController 
    }
}
