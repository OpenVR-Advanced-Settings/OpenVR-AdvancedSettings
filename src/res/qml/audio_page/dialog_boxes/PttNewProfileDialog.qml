import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

MyDialogOkCancelPopup {
    id: pttNewProfileDialog
    dialogTitle: "Create New PTT Profile"
    dialogWidth: 600
    dialogHeight: 220
    dialogContentItem: ColumnLayout {
        RowLayout {
            Layout.topMargin: 16
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            MyText {
                text: "Name: "
            }
            MyTextField {
                id: pttNewProfileName
                keyBoardUID: 591
                color: "#cccccc"
                text: ""
                Layout.fillWidth: true
                font.pointSize: 20
                function onInputEvent(input) {
                    pttNewProfileName.text = input
                }
            }
        }
    }
    onClosed: {
        if (okClicked) {
            if (pttNewProfileName.text != "") {
                AudioTabController.addPttProfile(pttNewProfileName.text)
                
            } else {
                audioMessageDialog.showMessage("Create New Profile", "ERROR: No name given.")
            }
        }
    }
    function openPopup() {
        pttNewProfileName.text = ""
        open()
    }
}
