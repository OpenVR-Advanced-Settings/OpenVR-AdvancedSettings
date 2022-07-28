import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: mediaKeysGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Media Control Keys"
        bottomPadding: -10
    }
    background: Rectangle {
        color: "transparent"
        border.color: "#d9dbe0"
        radius: 8
    }
    ColumnLayout {
        anchors.fill: parent

        LineSeparator {
        }

        RowLayout {
            property string playPauseButtonPath: "qrc:/media_keys/play_pause"
            property string stopButtonPath: "qrc:/media_keys/stop"
            property string previousButtonPath: "qrc:/media_keys/previous"
            property string nextButtonPath: "qrc:/media_keys/next"

            MediaButton {
                id: previousSongButton
                imagePath: parent.previousButtonPath
                onClicked: {
                    UtilitiesTabController.sendMediaPreviousSong()
                }
            }

            MediaButton {
                id: stopSongButton
                imagePath: parent.stopButtonPath
                onClicked: {
                    UtilitiesTabController.sendMediaStopSong()
                }
            }

            MediaButton {
                id: playPauseSongButton
                imagePath: parent.playPauseButtonPath
                onClicked: {
                    UtilitiesTabController.sendMediaPausePlay()
                }
            }

            MediaButton {
                id: nextSongButton
                imagePath: parent.nextButtonPath
                onClicked: {
                    UtilitiesTabController.sendMediaNextSong()
                }
            }
        }
    }
}
