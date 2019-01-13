import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
// Necessary for the project specific Components.
import ".."

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
            property bool playButtonIsPlay: false

            property string playButtonPath: "outline_play_arrow_white_24dp.svg"
            property string pauseButtonPath: "outline_pause_white_24dp.svg"
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
                    playPauseSongButton.contentItem.source = parent.playButtonPath
                    parent.playButtonIsPlay = true
                }
            }

            MediaButton {
                id: playPauseSongButton
                imagePath: parent.pauseButtonPath
                onClicked: {
                    UtilitiesTabController.sendMediaPausePlay()
                    if (parent.playButtonIsPlay) {
                        playPauseSongButton.contentItem.source = parent.pauseButtonPath
                        parent.playButtonIsPlay = false
                    } else {
                        playPauseSongButton.contentItem.source = parent.playButtonPath
                        parent.playButtonIsPlay = true
                    }

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
