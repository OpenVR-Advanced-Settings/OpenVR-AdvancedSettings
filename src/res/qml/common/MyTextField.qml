import QtQuick 2.7
import QtQuick.Controls 2.0
import ovras.advsettings 1.0

TextField {
	property int keyBoardUID: 0
    property string savedText: ""
    property bool active: false
    id: myTextField
    color: "#e4e8f0"
    text: ""
    font.pointSize: 20
    leftPadding: 8
    rightPadding: 8
    topPadding: 4
    bottomPadding: 4
    background: Button {
        hoverEnabled: true
        background: Rectangle {
            color: parent.hovered ? "#2a2f3a" : "#1e2029"
            border.width: 1
            border.color: parent.activeFocus ? "#1682e0" : "#343a46"
            radius: 6
        }
        onClicked: {
            myTextField.forceActiveFocus()
            active = true;
        }
    }
    onActiveFocusChanged: {
        if (activeFocus) {
            active = true;
            if (!OverlayController.desktopMode) {
                OverlayController.showKeyboard(text, keyBoardUID)
            }
            //savedText = text
        }
        //When box loses focus apply changes
        else{
            myTextField.onInputEvent(text)
            active = false;
        }
    }
    onEditingFinished: {
        if(OverlayController.desktopMode){
            myTextField.onInputEvent(text)
        }
    }
    function onInputEvent(input) {
        text = input
	}
    Connections {
        target: OverlayController

        onSubmitLastTextField:{
            //value here is UID
            if(value == 0){
                active = false;
                return;
            }

            if(value == keyBoardUID && active == true){
                myTextField.onInputEvent(text)
                active = false;
            }
        }

        onKeyBoardInputSignal: {
            if (userValue == keyBoardUID) {
                if(input == '\b'){
                    myTextField.text = text.slice(0,-1)
                    return
                }
                else if(input == '\e'){
                    myTextField.text = text.slice(0,cursorPosition)+text.slice(cursorposition+1)
                    return;
                }
                //Execute input
                else if(input == '\n'){
                    if(!OverlayController.desktopMode){
                        //myTextField.onInputEvent(input)
                        myTextField.focus = false;
                    }
                    return;
                }
                else if(input=='\r'){
                    return;
                }
                else{
                    myTextField.text = text + input;
                }
                //if (myTextField.text !== input) {
                //    myTextField.onInputEvent(input)
                //}
            }
        }
    }
}
