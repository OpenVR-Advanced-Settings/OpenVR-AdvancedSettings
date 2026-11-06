import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings

Popup {
    id: aiTranslationPopup

    implicitHeight: parent.height
    implicitWidth: parent.width

    // Language that was just selected, from the footer language model.
    property string languageLocale: ""
    property string languageName: ""
    property string discordUrl: ""

    modal: true
    closePolicy: Popup.NoAutoClose

    background: Rectangle {
        color: "black"
        opacity: 0.8
    }

    function openFor(locale, name) {
        languageLocale = locale
        languageName = name
        open()
    }

    function finish(locale) {
        OverlayController.setAiTranslationPromptSeen(languageLocale)
        OverlayController.language = locale
        close()
    }

    contentItem: Item {
        Rectangle {
            implicitWidth: 760
            implicitHeight: dialogColumn.implicitHeight + 24
            anchors.centerIn: parent
            radius: 6
            color: "#262a31"
            border.width: 1
            border.color: "#3a4049"
            ColumnLayout {
                id: dialogColumn
                anchors.fill: parent
                anchors.margins: 12
                spacing: 16
                MyText {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    Layout.topMargin: 8
                    text: qsTr("AI Translation")
                    font.pointSize: 18
                    font.bold: true
                    color: "#ffffff"
                }
                Rectangle {
                    color: "#3a4049"
                    height: 1
                    Layout.fillWidth: true
                }
                MyText {
                    Layout.fillWidth: true
                    Layout.leftMargin: 24
                    Layout.rightMargin: 24
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("The selected language is translated mainly with AI. If you would like to contribute to translation work please reach out in the discord. Or you can continue using English")
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.leftMargin: 24
                    Layout.rightMargin: 24
                    Layout.bottomMargin: 12
                    spacing: 16
                    MyPushButton {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
                        text: qsTr("Discord")
                        onClicked: {
                            Qt.openUrlExternally(discordUrl)
                        }
                    }
                    MyPushButton {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
                        // Not translated, so it always reads as English.
                        text: "English"
                        onClicked: {
                            finish("en")
                        }
                    }
                    MyPushButton {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
                        text: languageName
                        onClicked: {
                            finish(languageLocale)
                        }
                    }
                }
            }
        }
    }
}
