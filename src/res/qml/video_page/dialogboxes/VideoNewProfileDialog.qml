import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

MyDialogOkCancelPopup {
    id: videoNewProfileDialog
    dialogTitle: qsTr("Create New Video Profile")
    dialogWidth: 600
    dialogHeight: 300
    dialogContentItem: ColumnLayout {
        RowLayout {
            Layout.topMargin: 16
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            MyText {
                text: qsTr("Name ")
            }
            MyTextField {
                id: videoNewProfileName
                keyBoardUID: 990
                color: "#ffffff"
                text: ""
                Layout.fillWidth: true
                font.pointSize: 16
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
                videoMessageDialog.showMessage(qsTr("Create New Profile"), qsTr("ERROR: Empty profile name."))
            }
        }
    }


    function openPopup() {
        videoNewProfileName.text = ""
        open()
    }
}
