import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
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
        border.color: "#ffffff"
        radius: 8
    }
    ColumnLayout {
        anchors.fill: parent

        LineSeparator {
        }

        RowLayout {
            property string playPauseButtonPath: "outline_play_pause_white_24dp.svg"
            property string stopButtonPath: "outline_stop_white_24dp.svg"
            property string previousButtonPath: "outline_skip_previous_white_24dp.svg"
            property string nextButtonPath: "outline_skip_next_white_24dp.svg"

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
