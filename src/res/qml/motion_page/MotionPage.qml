import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "space_drag"
import "move_space"
import "height_toggle"
import "gravity"
import "offset_profiles"

MyStackViewPage {
    headerText: qsTr("Motion Settings")

    // Every group box row on this page uses this height (Gravity Strength row).
    readonly property int groupBoxRowHeight: 44

    DeviceOverrideDialog {
        id: spaceDragDeviceDialog
    }

    MyDialogOkPopup {
        id: offsetMessageDialog
        function showMessage(title, text) {
            dialogTitle = title
            dialogText = text
            open()
        }
    }

    MyDialogOkCancelPopup {
        id: offsetDeleteProfileDialog
        property int profileIndex: -1
        dialogTitle: qsTr("Delete Profile")
        dialogText: qsTr("Do you really want to delete this profile?")
        onClosed: {
            if (okClicked) {
                MoveCenterTabController.deleteOffsetProfile(profileIndex)
            }
        }
    }

    MyDialogOkCancelPopup {
        id: offsetNewProfileDialog
        dialogTitle: qsTr("Create New Profile")
        dialogWidth: 800
        dialogHeight: 400
        dialogContentItem: ColumnLayout {
            RowLayout {
                Layout.topMargin: 16
                Layout.leftMargin: 16
                Layout.rightMargin: 16
                MyText {
                    text: qsTr("Name ")
                }
                MyTextField {
                    id: offsetNewProfileName
                    keyBoardUID: 105
                    color: "#ffffff"
                    text: ""
                    Layout.fillWidth: true
                    font.pointSize: 16
                    function onInputEvent(input) {
                        offsetNewProfileName.text = input
                    }
                }
            }
        }
        onClosed: {
            if (okClicked) {
                if (offsetNewProfileName.text == "") {
                    offsetMessageDialog.showMessage(qsTr("Create New Profile"), qsTr("ERROR: Empty profile name."))
                } else {
                    MoveCenterTabController.addOffsetProfile(offsetNewProfileName.text)
                }

            }
        }
        function openPopup() {
            offsetNewProfileName.text = ""
            open()
        }
    }

    content: ColumnLayout {
        spacing: 8

        SpaceDragGroupBox {
            id: spaceDragGroupBox
            rowHeight: groupBoxRowHeight
        }

        MoveSpaceGroupBox { id: moveSpaceGroupBox }

        GravityGroupBox {
            id: gravityGroupBox
            rowHeight: groupBoxRowHeight
        }

        HeightToggleGroupBox {
            id: heightToggleGroupBox
            rowHeight: groupBoxRowHeight
        }

        OffsetProfilesGroupBox {
            id: offsetProfilesGroupBox
            rowHeight: groupBoxRowHeight
            onNewProfileRequested: offsetNewProfileDialog.openPopup()
            onDeleteProfileRequested: function(profileIndex) {
                offsetDeleteProfileDialog.profileIndex = profileIndex
                offsetDeleteProfileDialog.open()
            }
        }


        Item {
            Layout.fillHeight: true
        }

    }
}
