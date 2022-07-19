import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

MyDialogOkCancelPopup {
    id: videoNewProfileDialog
    dialogTitle: "Create New Video Profile"
    dialogWidth: 600
    dialogHeight: 300
    y: -200
    x: 0
    dialogContentItem: ColumnLayout {
        RowLayout {
            Layout.topMargin: 16
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            MyText {
                text: "Name: "
            }
            MyTextField {
                id: videoNewProfileName
                keyBoardUID: 990
                color: "#d9dbe0"
                text: ""
                Layout.fillWidth: true
                font.pointSize: 20
                function onInputEvent(input) {
                    videoNewProfileName.text = input
                }
            }
        }
    }
    onClosed: {
        if (okClicked) {
            if (videoNewProfileName.text != "") {
                VideoTabController.addVideoProfile(videoNewProfileName.text)
            } else {
                videoMessageDialog.showMessage("Create New Profile", "ERROR: No name given.")
            }
        }
    }


    function openPopup() {
        videoNewProfileName.text = ""
        open()
    }
}
