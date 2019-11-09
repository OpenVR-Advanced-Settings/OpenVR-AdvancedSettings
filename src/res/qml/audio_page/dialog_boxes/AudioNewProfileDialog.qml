import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

MyDialogOkCancelPopup {
    id: audioNewProfileDialog
    dialogTitle: "Create New Audio Profile"
    dialogWidth: 600
    dialogHeight: 300
    y: -300
    x: 100
    dialogContentItem: ColumnLayout {
        RowLayout {
            Layout.topMargin: 16
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            MyText {
                text: "Name: "
            }
            MyTextField {
                id: audioNewProfileName
                keyBoardUID: 590
                color: "#cccccc"
                text: ""
                Layout.fillWidth: true
                font.pointSize: 20
                function onInputEvent(input) {
                    audioNewProfileName.text = input
                }
            }
        }
        RowLayout {
            id: test
            Layout.topMargin: 16
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            MyToggleButton {
                id: audioDefaultProfileToggle
                Layout.preferredWidth: 250
                text: "Make Default"
                onCheckedChanged: {
                    AudioTabController.setAudioProfileDefault(checked, false)
                }
            }
            Component.onCompleted: {
                audioDefaultProfileToggle.checked = AudioTabController.audioProfileDefault
            }
        }
    }
    onClosed: {
        if (okClicked) {
            if (audioNewProfileName.text != "") {
                AudioTabController.addAudioProfile(audioNewProfileName.text)
            } else {
                audioMessageDialog.showMessage("Create New Profile", "ERROR: No name given.")
            }
        }
    }

    function openPopup() {
        audioNewProfileName.text = ""
        open()
    }
}
