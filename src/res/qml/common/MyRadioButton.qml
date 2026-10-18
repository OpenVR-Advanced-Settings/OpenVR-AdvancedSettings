import QtQuick 2.7
import QtQuick.Controls 2.0
import ovras.advsettings
import "." // QTBUG-34418, singletons require explicit import to load qmldir file

RadioButton {
    id: myRadioButton
    hoverEnabled: true
    spacing: 10
    indicator: Rectangle {
        implicitWidth: 22
        implicitHeight: 22
        x: myRadioButton.leftPadding
        y: myRadioButton.height / 2 - height / 2
        radius: 11
        color: myRadioButton.activeFocus ? "#4b5361" : "#3a4049"
        border.width: myRadioButton.checked ? 0 : 1
        border.color: "#5d636b"
        Rectangle {
            anchors.centerIn: parent
            width: 12
            height: 12
            radius: 6
            color: myRadioButton.enabled ? "#1a9fff" : "#5d636b"
            visible: myRadioButton.checked
        }
    }
    contentItem: MyText {
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        leftPadding: myRadioButton.indicator.width + myRadioButton.spacing
        text: parent.text
        color: parent.enabled ? "#dcdedf" : "#5d636b"
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
