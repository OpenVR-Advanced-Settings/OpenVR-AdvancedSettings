import QtQuick 2.7
import QtQuick.Controls 2.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file


ComboBox {
    id: myComboBox
    hoverEnabled: true

    background: Rectangle {
        color: parent.pressed ? "#406288" : (parent.activeFocus ? "#365473" : "#2c435d")
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
        contentItem: MyText {
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text: parent.text
            color: parent.enabled ? "#ffffff" : "#909090"
        }
        background: Rectangle {
            color: parent.pressed ? "#406288" : (parent.hovered ? "#365473" : "#2c435d")
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
			MyResources.playActivationSound()
		}
    }

    Component.onCompleted: {
        popup.background.color = "#2c435d"
    }
}
