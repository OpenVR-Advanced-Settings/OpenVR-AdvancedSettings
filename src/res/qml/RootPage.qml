import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
import "utilities_page"
import "audio_page"

MyStackViewPage {
    id: rootPage
    width: 1200
    height: 800
    headerText: "OpenVR Advanced Settings"
    headerShowBackButton: false
    stackView: mainView
    content: Item {
           ColumnLayout {
           anchors.fill: parent
           RowLayout {
               Layout.fillHeight: true
               Layout.fillWidth: true
               ColumnLayout {
                   Layout.preferredWidth: 250
                   Layout.maximumWidth: 250
                   Layout.fillHeight: true
                   spacing: 18
                   MyPushButton {
                       id: steamVRButton
                       activationSoundEnabled: false
                       text: "SteamVR"
                       Layout.fillWidth: true
                       onClicked: {
                           MyResources.playFocusChangedSound()
                           mainView.push(steamVRPage)
                       }
                   }

                   MyPushButton {
                       id: chaperoneButton
                       activationSoundEnabled: false
                       text: "Chaperone"
                       Layout.fillWidth: true
                       onClicked: {
                           MyResources.playFocusChangedSound()
                           mainView.push(chaperonePage)
                       }
                   }

                   MyPushButton {
                       id: playSpaceButton
                       activationSoundEnabled: false
                       text: "Room Settings"
                       Layout.fillWidth: true
                       onClicked: {
                           MyResources.playFocusChangedSound()
                           mainView.push(playspacePage)
                       }
                   }

                   MyPushButton {
                       id: fixFloorButton
                       activationSoundEnabled: false
                       text: "Room Fix"
                       Layout.fillWidth: true
                       onClicked: {
                           MyResources.playFocusChangedSound()
                           mainView.push(fixFloorPage)
                       }
                   }

                   MyPushButton {
                       id: audioButton
                       activationSoundEnabled: false
                       text: "Audio"
                       Layout.fillWidth: true
                       onClicked: {
                           MyResources.playFocusChangedSound()
                           mainView.push(audioPage)
                       }
                   }

                   MyPushButton {
                       id: reviveButton
                       activationSoundEnabled: false
                       text: "Revive"
                       Layout.fillWidth: true
                       visible: SettingsTabController.forceRevivePage ? true : ReviveTabController.isOverlayInstalled
                       onClicked: {
                           MyResources.playFocusChangedSound()
                           mainView.push(revivePage)
                       }
                   }

                   MyPushButton {
                       id: utilitiesButton
                       activationSoundEnabled: false
                       text: "Utilities"
                       Layout.fillWidth: true
                       onClicked: {
                           MyResources.playFocusChangedSound()
                           mainView.push(utilitiesPage)
                       }
                   }

                   MyPushButton {
                       id: accessibilityButton
                       activationSoundEnabled: false
                       text: "Accessibility"
                       Layout.fillWidth: true
                       onClicked: {
                           MyResources.playFocusChangedSound()
                           mainView.push(accessibilityPage)
                       }
                   }

                   MyPushButton {
                       id: statisticsButton
                       activationSoundEnabled: false
                       text: "Statistics"
                       Layout.fillWidth: true
                       onClicked: {
                           MyResources.playFocusChangedSound()
                           mainView.push(statisticsPage)
                       }
                   }

                   Item {
                       Layout.fillHeight: true
                       Layout.fillWidth: true
                   }

                   MyPushButton {
                       id: settingsButton
                       activationSoundEnabled: false
                       text: "Settings"
                       Layout.fillWidth: true
                       onClicked: {
                           MyResources.playFocusChangedSound()
                           mainView.push(settingsPage)
                       }
                   }
               }


               ColumnLayout {
                   Layout.fillHeight: true
                   Layout.fillWidth: true
                   Layout.leftMargin: 80
                   spacing: 9

                   RowLayout {
                       spacing: 18

                       MyText {
                           text: "Supersampling Profile:"
                       }

                       MyComboBox {
                           id: summarySteamVRProfileComboBox
                           Layout.maximumWidth: 378
                           Layout.minimumWidth: 378
                           Layout.preferredWidth: 378
                           Layout.fillWidth: true
                           model: [""]
                           onCurrentIndexChanged: {
                               if (currentIndex > 0) {
                                   summarySteamVRProfileApplyButton.enabled = true
                               } else {
                                   summarySteamVRProfileApplyButton.enabled = false
                               }
                           }
                       }

                       MyPushButton {
                           id: summarySteamVRProfileApplyButton
                           enabled: false
                           Layout.preferredWidth: 150
                           text: "Apply"
                           onClicked: {
                               if (summarySteamVRProfileComboBox.currentIndex > 0) {
                                   SteamVRTabController.applySteamVRProfile(summarySteamVRProfileComboBox.currentIndex - 1)
                                   summarySteamVRProfileComboBox.currentIndex = 0
                               }
                           }
                       }
                   }

                   RowLayout {
                       spacing: 18

                       MyText {
                           text: "Chaperone Profile:"
                       }

                       MyComboBox {
                           id: summaryChaperoneProfileComboBox
                           Layout.leftMargin: 47
                           Layout.maximumWidth: 378
                           Layout.minimumWidth: 378
                           Layout.preferredWidth: 378
                           Layout.fillWidth: true
                           model: [""]
                           onCurrentIndexChanged: {
                               if (currentIndex > 0) {
                                   summaryChaperoneProfileApplyButton.enabled = true
                               } else {
                                   summaryChaperoneProfileApplyButton.enabled = false
                               }
                           }
                       }

                       MyPushButton {
                           id: summaryChaperoneProfileApplyButton
                           enabled: false
                           Layout.preferredWidth: 150
                           text: "Apply"
                           onClicked: {
                               if (summaryChaperoneProfileComboBox.currentIndex > 0) {
                                   ChaperoneTabController.applyChaperoneProfile(summaryChaperoneProfileComboBox.currentIndex - 1)
                                   summaryChaperoneProfileComboBox.currentIndex = 0
                               }
                           }
                       }
                   }

                   Item {
                       Layout.fillHeight: true
                       Layout.fillWidth: true
                   }

                   RowLayout {
                       MyText {
                           text: "Tracking Universe:"
                       }
                       MyText {
                           id: summaryPlaySpaceModeText
                           Layout.fillWidth: true
                           horizontalAlignment: Text.AlignRight
                           text: "Unknown"
                       }
                   }

                   RowLayout {
                       MyText {
                           text: "HMD Rotations:"
                       }
                       MyText {
                           id: summaryHmdRotationsText
                           Layout.fillWidth: true
                           horizontalAlignment: Text.AlignRight
                           text: "-999.9"
                       }
                   }

                   Item {
                       Layout.fillHeight: true
                       Layout.fillWidth: true
                   }

                   RowLayout {
                       MyText {
                           text: "Dropped Frames:"
                       }
                       MyText {
                           id: summaryDroppedFramesText
                           Layout.fillWidth: true
                           horizontalAlignment: Text.AlignRight
                           text: "0"
                       }
                   }

                   RowLayout {
                       MyText {
                           text: "Reprojected Frames:"
                       }
                       MyText {
                           id: summaryReprojectedFramesText
                           Layout.fillWidth: true
                           horizontalAlignment: Text.AlignRight
                           text: "0"
                       }
                   }

                   RowLayout {
                       MyText {
                           text: "Timed Out:"
                       }
                       MyText {
                           id: summaryTimedOutText
                           Layout.fillWidth: true
                           horizontalAlignment: Text.AlignRight
                           text: "0"
                       }
                   }

                   RowLayout {
                       MyText {
                           text: "Reprojection Ratio:"
                       }
                       MyText {
                           id: summaryTotalRatioText
                           Layout.fillWidth: true
                           horizontalAlignment: Text.AlignRight
                           text: "0"
                       }
                   }

                   Item {
                       Layout.fillHeight: true
                       Layout.fillWidth: true
                   }



                   RowLayout {
                       MyText {
                           text: "Microphone:"
                       }
                       MySlider {
                           id: summaryMicVolumeSlider
                           from: 0.0
                           to: 1.0
                           stepSize: 0.01
                           value: 1.0
                           Layout.fillWidth: true
                           onPositionChanged: {
                               var val = this.position * 100
                               summaryMicVolumeText.text = Math.round(val) + "%"
                           }
                           onValueChanged: {
                               AudioTabController.setMicVolume(this.value.toFixed(2))
                           }
                       }
                       MyText {
                           id: summaryMicVolumeText
                           text: "100%"
                           Layout.preferredWidth: 85
                           horizontalAlignment: Text.AlignRight
                       }

                       MyPushButton2 {
                           id: summaryMicMuteToggle
                           Layout.leftMargin: 12
                           checkable: true
                           contentItem: Image {
                               source: parent.checked ? "audio_page/mic_off.svg" : "audio_page/mic_on.svg"
                               sourceSize.width: 32
                               sourceSize.height: 32
                               anchors.fill: parent
                           }
                           onCheckedChanged: {
                               AudioTabController.setMicMuted(checked)
                           }
                       }
                   }
                   RowLayout {
                       MyToggleButton {
                           id: summaryPttToggle
                           text: "Push-to-Talk"
                           onClicked: {
                               AudioTabController.pttEnabled = checked
                           }
                       }
                       RowLayout {
                           spacing: 18

                           MyComboBox {
                               Layout.leftMargin: 78
                               id: summaryPttProfileComboBox
                               Layout.maximumWidth: 378
                               Layout.minimumWidth: 378
                               Layout.preferredWidth: 378
                               Layout.fillWidth: true
                               model: [""]
                               onCurrentIndexChanged: {
                                   if (currentIndex > 0) {
                                       summaryPttProfileApplyButton.enabled = true
                                   } else {
                                       summaryPttProfileApplyButton.enabled = false
                                   }
                               }
                           }

                           MyPushButton {
                               id: summaryPttProfileApplyButton
                               enabled: false
                               Layout.preferredWidth: 150
                               text: "Apply"
                               onClicked: {
                                   if (summaryPttProfileComboBox.currentIndex > 0) {
                                       AudioTabController.applyPttProfile(summaryPttProfileComboBox.currentIndex - 1)
                                       summaryPttProfileComboBox.currentIndex = 0
                                   }
                               }
                           }
                       }
                   }

                   Item {
                       Layout.fillHeight: true
                       Layout.fillWidth: true
                   }

                   MyText {
                       id: summaryVersionText
                       text: "v0.0.0"
                       font.pointSize: 16
                       Layout.fillWidth: true
                       horizontalAlignment: Text.AlignRight
                   }
               }
           }
       }
   }

   Component.onCompleted: {
       reloadChaperoneProfiles()
       reloadSteamVRProfiles()
       reloadPttProfiles()

       summaryVersionText.text = applicationVersion


       if (MoveCenterTabController.trackingUniverse === 0) {
           summaryPlaySpaceModeText.text = "Sitting"
       } else if (MoveCenterTabController.trackingUniverse === 1) {
           summaryPlaySpaceModeText.text = "Standing"
       } else {
           summaryPlaySpaceModeText.text = "Unknown(" + MoveCenterTabController.trackingUniverse + ")"
       }
       updateStatistics()
       if (visible) {
           summaryUpdateTimer.start()
       }
       summaryMicVolumeSlider.value = AudioTabController.micVolume
       summaryMicMuteToggle.checked = AudioTabController.micMuted
       summaryPttToggle.checked = AudioTabController.pttEnabled
   }

   Connections {
       target: ChaperoneTabController
       onChaperoneProfilesUpdated: {
           reloadChaperoneProfiles()
       }
   }

   Connections {
       target: SteamVRTabController
       onSteamVRProfilesUpdated: {
           reloadSteamVRProfiles()
       }
   }

   Connections {
       target: AudioTabController
       onPttProfilesUpdated: {
           reloadPttProfiles()
       }
       onMicVolumeChanged: {
           summaryMicVolumeSlider.value = AudioTabController.micVolume
       }
       onMicMutedChanged: {
           summaryMicMuteToggle.checked = AudioTabController.micMuted
       }
       onPttEnabledChanged: {
           summaryPttToggle.checked = AudioTabController.pttEnabled
       }
   }

   Connections {
       target: MoveCenterTabController
       onTrackingUniverseChanged: {
           if (MoveCenterTabController.trackingUniverse === 0) {
               summaryPlaySpaceModeText.text = "Sitting"
           } else if (MoveCenterTabController.trackingUniverse === 1) {
               summaryPlaySpaceModeText.text = "Standing"
           } else {
               summaryPlaySpaceModeText.text = "Unknown(" + MoveCenterTabController.trackingUniverse + ")"
           }
       }
   }

   Connections {
       target: SettingsTabController
       onForceRevivePageChanged: {
           reviveButton.visible = SettingsTabController.forceRevivePage ? true : ReviveTabController.isOverlayInstalled
       }
   }

   function updateStatistics() {
       var rotations = StatisticsTabController.hmdRotations
       if (rotations > 0) {
           summaryHmdRotationsText.text = rotations.toFixed(2) + " CCW"
       } else {
           summaryHmdRotationsText.text = -rotations.toFixed(2) + " CW"
       }
       summaryDroppedFramesText.text = StatisticsTabController.droppedFrames
       summaryReprojectedFramesText.text = StatisticsTabController.reprojectedFrames
       summaryTimedOutText.text = StatisticsTabController.timedOut
       summaryTotalRatioText.text = (StatisticsTabController.totalReprojectedRatio*100.0).toFixed(1) + "%"
   }

   Timer {
       id: summaryUpdateTimer
       repeat: true
       interval: 100
       onTriggered: {
           parent.updateStatistics()
       }
   }

   onVisibleChanged: {
       if (visible) {
           updateStatistics()
           summaryUpdateTimer.start()
       } else {
           summaryUpdateTimer.stop()
       }
   }


    function reloadChaperoneProfiles() {
       var profiles = [""]
       var profileCount = ChaperoneTabController.getChaperoneProfileCount()
       for (var i = 0; i < profileCount; i++) {
           profiles.push(ChaperoneTabController.getChaperoneProfileName(i))
       }
       summaryChaperoneProfileComboBox.currentIndex = 0
       summaryChaperoneProfileComboBox.model = profiles
    }


    function reloadSteamVRProfiles() {
       var profiles = [""]
       var profileCount = SteamVRTabController.getSteamVRProfileCount()
       for (var i = 0; i < profileCount; i++) {
           profiles.push(SteamVRTabController.getSteamVRProfileName(i))
       }
       summarySteamVRProfileComboBox.currentIndex = 0
       summarySteamVRProfileComboBox.model = profiles
    }

    function reloadPttProfiles() {
        var profiles = [""]
        var profileCount = AudioTabController.getPttProfileCount()
        for (var i = 0; i < profileCount; i++) {
            profiles.push(AudioTabController.getPttProfileName(i))
        }
        summaryPttProfileComboBox.currentIndex = 0
        summaryPttProfileComboBox.model = profiles
    }
}
