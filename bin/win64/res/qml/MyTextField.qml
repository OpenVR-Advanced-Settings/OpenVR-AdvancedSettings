import QtQuick 2.7
import QtQuick.Controls 2.0
import matzman666.advsettings 1.0

TextField {
	property int keyBoardUID: 0
    id: myTextField
    color: "#cccccc"
    text: ""
    font.pointSize: 20
    background: Rectangle {
        color: parent.activeFocus ? "#2c435d" : "#1b2939"
        border.color: "#cccccc"
        border.width: 2
    }
    onEditingFinished: {
        if (OverlayController.desktopMode) {
            myTextField.onInputEvent(text)
        }
    }
    onActiveFocusChanged: {
        if (activeFocus && !OverlayController.desktopMode) {
            OverlayController.showKeyboard(text, keyBoardUID)
        }
    }
	function onInputEvent(input) {
		text = input
	}
    Connections {
        target: OverlayController
        onKeyBoardInputSignal: {
            if (userValue == keyBoardUID) {
                myTextField.onInputEvent(input)
            }
        }
    }
}
