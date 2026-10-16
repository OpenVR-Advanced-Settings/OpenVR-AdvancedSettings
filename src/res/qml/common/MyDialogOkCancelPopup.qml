import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings

Popup {
    id: myDialogPopup

    implicitHeight: parent.height
    implicitWidth: parent.width

    property string dialogTitle: ""
    property string dialogText: ""
    property int dialogWidth: 600
    property int dialogHeight: 300

    property Item dialogContentItem: MyText {
        text: dialogText
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        Layout.fillWidth: true
    }

    property bool okClicked: false

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    background: Rectangle {
        color: "black"
        opacity: 0.8
     }

    onAboutToShow: okClicked = false

    contentItem: Item {
        // Clicking the dimmed area around the dialog cancels it.
        MouseArea {
            anchors.fill: parent
            onClicked: {
                okClicked = false
                myDialogPopup.close()
            }
        }
        Rectangle {
            implicitWidth: dialogWidth
            implicitHeight: dialogHeight
            anchors.centerIn: parent
            radius: 6
            color: "#262a31"
            border.width: 1
            border.color: "#3a4049"
            // Swallow clicks on the dialog itself so they don't reach the background.
            MouseArea {
                anchors.fill: parent
            }
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 12
                MyText {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    Layout.topMargin: 8
                    text: dialogTitle
                    font.pointSize: 18
                    font.bold: true
                    color: "#ffffff"
                }
                Rectangle {
                    color: "#3a4049"
                    height: 1
                    Layout.fillWidth: true
                }
                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                ColumnLayout {
                    id: dialogContent
                }
                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.leftMargin: 24
                    Layout.rightMargin: 24
                    Layout.bottomMargin: 12
                    MyPushButton {
                        implicitWidth: 200
                        text: qsTr("Ok")
                        onClicked: {
                            okClicked = true
                            myDialogPopup.close()
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    MyPushButton {
                        implicitWidth: 200
                        text: qsTr("Cancel")
                        onClicked: {
                            okClicked = false
                            myDialogPopup.close()
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        dialogContentItem.parent = dialogContent
    }
}
