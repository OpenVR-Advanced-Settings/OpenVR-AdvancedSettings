import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"
import "../dialogboxes"

SettingsGroupBox {
    id: videoProfileGroupBox
    Layout.fillWidth: true

    property Item dialogParent: videoProfileGroupBox

    VideoDeleteProfileDialog {
        id: videoDeleteProfileDialog
        parent: videoProfileGroupBox.dialogParent
    }

    VideoNewProfileDialog {
        id: videoNewProfileDialog
        parent: videoProfileGroupBox.dialogParent
    }
    VideoMessageDialog{
        id:videoMessageDialog
        parent: videoProfileGroupBox.dialogParent
    }

    title: qsTr("Video Profiles")
    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            spacing: 10

            MyText {
                Layout.preferredWidth: 150
                text: qsTr("Profile:  ")
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
                text: qsTr("Apply")
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

                text: qsTr("Delete Profile")
                onClicked: {
                    if (videoProfileComboBox.currentIndex > 0) {
                        videoDeleteProfileDialog.profileIndex = videoProfileComboBox.currentIndex - 1
                        videoDeleteProfileDialog.open()
                    }
                }
            }
            MyPushButton {
                Layout.preferredWidth: 200
                text: qsTr("New Profile")
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
        function onVideoProfilesUpdated() {
            reloadVideoProfiles()
        }
        function onVideoProfileAdded() {
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
