import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

SettingsGroupBox {
    id: offsetProfilesGroupBox
    Layout.fillWidth: true

    title: qsTr("Profiles")

    // The dialogs live on the page so they cover it, the page opens them on these.
    signal newProfileRequested()
    signal deleteProfileRequested(int profileIndex)

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            MyText {
                text: qsTr("Tracking Universe")
                Layout.preferredWidth: 230
            }
            MyText {
                id: spaceModeText
                font.bold: true
                text: qsTr("Standing")
            }
        }

        LineSeparator {
        }

        ColumnLayout {
            id: profileSection
            spacing: 18
            RowLayout {
                spacing: 18

                MyText {
                    text: qsTr("Profile")
                }

                MyComboBox {
                    id: offsetProfileComboBox
                    Layout.fillWidth: true
                    model: [""]
                    onCurrentIndexChanged: {
                        if (currentIndex > 0) {
                            offsetApplyProfileButton.enabled = true
                            offsetDeleteProfileButton.enabled = true
                        } else {
                            offsetApplyProfileButton.enabled = false
                            offsetDeleteProfileButton.enabled = false
                        }
                    }
                }

                MyPushButton {
                    id: offsetApplyProfileButton
                    enabled: false
                    Layout.preferredWidth: 200
                    text: qsTr("Apply")
                    onClicked: {
                        if (offsetProfileComboBox.currentIndex > 0) {
                            MoveCenterTabController.applyOffsetProfile(offsetProfileComboBox.currentIndex - 1)
                            offsetProfileComboBox.currentIndex = 0
                        }
                    }
                }
            }
            RowLayout {
                spacing: 18
                Item {
                    Layout.fillWidth: true
                }
                MyPushButton {
                    id: offsetDeleteProfileButton
                    enabled: false
                    Layout.preferredWidth: 200
                    text: qsTr("Delete Profile")
                    onClicked: {
                        if (offsetProfileComboBox.currentIndex > 0) {
                            offsetProfilesGroupBox.deleteProfileRequested(offsetProfileComboBox.currentIndex - 1)
                        }
                    }
                }
                MyPushButton {
                    Layout.preferredWidth: 200
                    text: qsTr("New Profile")
                    onClicked: {
                        offsetProfilesGroupBox.newProfileRequested()
                    }
                }
            }
        }
    }

    function updateTrackingUniverseText() {
        if (MoveCenterTabController.trackingUniverse === 0) {
            spaceModeText.text = qsTr("Sitting")
        } else if (MoveCenterTabController.trackingUniverse === 1) {
            spaceModeText.text = qsTr("Standing")
        } else {
            spaceModeText.text = qsTr("Unknown(%1)").arg(MoveCenterTabController.trackingUniverse)
        }
    }

    function reloadOffsetProfiles() {
        var profiles = [""]
        var profileCount = MoveCenterTabController.getOffsetProfileCount()
        for (var i = 0; i < profileCount; i++) {
            profiles.push(MoveCenterTabController.getOffsetProfileName(i))
        }
        offsetProfileComboBox.currentIndex = 0
        offsetProfileComboBox.model = profiles
    }

    Component.onCompleted: {
        updateTrackingUniverseText()
        reloadOffsetProfiles()
    }

    Connections {
        target: MoveCenterTabController
        function onTrackingUniverseChanged() {
            updateTrackingUniverseText()
        }
        function onOffsetProfilesUpdated() {
            reloadOffsetProfiles()
        }
    }
}
