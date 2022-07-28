import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"
import "../dialogboxes"

GroupBox {
    id: videoProfileGroupBox
    Layout.fillWidth: true

    VideoDeleteProfileDialog {
        id: videoDeleteProfileDialog
    }

    VideoNewProfileDialog {
        id: videoNewProfileDialog
    }
    VideoMessageDialog{
        id:videoMessageDialog
    }

    label: MyText {
        leftPadding: 10
        text: "Video Profiles:"
        bottomPadding: -10
    }
    background: Rectangle {
        color: "transparent"
        border.color: "#d9dbe0"
        radius: 8
    }
    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            color: "#d9dbe0"
            height: 1
            Layout.fillWidth: true
            Layout.bottomMargin: 5
        }
        RowLayout {
            spacing: 10

            MyText {
                Layout.preferredWidth: 150
                text: "Profile:  "
            }

            MyComboBox {
                id: videoProfileComboBox
                Layout.fillWidth: true
                model: [""]
                onCurrentIndexChanged: {
                    if (currentIndex > 0) {
                        videoApplyProfileButton.enabled = true
                        videoDeleteProfileButton.enabled = true
                    } else {
                        videoApplyProfileButton.enabled = false
                        videoDeleteProfileButton.enabled = false
                    }
                }
            }

            MyPushButton {
                id: videoApplyProfileButton
                enabled: true
                Layout.preferredWidth: 150
                text: "Apply"
                onClicked: {
                    if (videoProfileComboBox.currentIndex > 0) {
                        VideoTabController.applyVideoProfile(
                                    videoProfileComboBox.currentIndex - 1)
                    }
                }
            }
        }
        RowLayout {
            spacing: 10
            Item{
                Layout.fillWidth: true
            }

            MyPushButton {
                id: videoDeleteProfileButton
                enabled: true
                Layout.preferredWidth: 200

                text: "Delete Profile"
                onClicked: {
                    if (videoProfileComboBox.currentIndex > 0) {
                        videoDeleteProfileDialog.profileIndex = videoProfileComboBox.currentIndex - 1
                        videoDeleteProfileDialog.open()
                    }
                }
            }
            MyPushButton {
                Layout.preferredWidth: 200
                text: "New Profile"
                onClicked: {
                    videoNewProfileDialog.openPopup()
                }
            }
        }
    }
    Component.onCompleted: {
        reloadVideoProfiles()
    }
    Connections {
        target: VideoTabController
        onVideoProfilesUpdated: {
            reloadVideoProfiles()
        }
        onVideoProfileAdded: {
            videoProfileComboBox.currentIndex = VideoTabController.getVideoProfileCount()
        }

    }
    function reloadVideoProfiles() {
        var profiles = [""]
        var profileCount = VideoTabController.getVideoProfileCount()
        for (var i = 0; i < profileCount; i++) {
            profiles.push(VideoTabController.getVideoProfileName(i))
        }
        videoProfileComboBox.currentIndex = 0
        videoProfileComboBox.model = profiles
    }


}
