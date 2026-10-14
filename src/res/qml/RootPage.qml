import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ovras.advsettings
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
import "common"
import "utilities_page"
import "audio_page"
import "video_page"

MyStackViewPage {
    id: rootPage
    headerText: "Home"
    headerShowBackButton: false
    stackView: mainView
    content: ColumnLayout {
        spacing: 18

        SettingsGroupBox {
            title: "Profiles"
            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                RowLayout {
                    spacing: 12
                    MyText {
                        text: "Video Profile"
                        Layout.fillWidth: true
                    }
                    MyComboBox {
                        id: summaryVideoProfileComboBox
                        Layout.preferredWidth: 320
                        model: [""]
                        onCurrentIndexChanged: {
                            if (currentIndex > 0) {
                                summaryVideoProfileApplyButton.enabled = true
                            } else {
                                summaryVideoProfileApplyButton.enabled = false
                            }
                        }
                    }
                    MyPushButton {
                        id: summaryVideoProfileApplyButton
                        enabled: false
                        Layout.preferredWidth: 120
                        text: "Apply"
                        onClicked: {
                            if (summaryVideoProfileComboBox.currentIndex > 0) {
                                VideoTabController.applyVideoProfile(summaryVideoProfileComboBox.currentIndex - 1)
                                summaryVideoProfileComboBox.currentIndex = 0
                            }
                        }
                    }
                }

                LineSeparator {
                }

                RowLayout {
                    spacing: 12
                    MyText {
                        text: "Chaperone Profile"
                        Layout.fillWidth: true
                    }
                    MyComboBox {
                        id: summaryChaperoneProfileComboBox
                        Layout.preferredWidth: 320
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
                        Layout.preferredWidth: 120
                        text: "Apply"
                        onClicked: {
                            if (summaryChaperoneProfileComboBox.currentIndex > 0) {
                                ChaperoneTabController.applyChaperoneProfile(summaryChaperoneProfileComboBox.currentIndex - 1)
                                summaryChaperoneProfileComboBox.currentIndex = 0
                            }
                        }
                    }
                }
            }
        }

        SettingsGroupBox {
            title: "Stats"
            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                RowLayout {
                    Layout.preferredHeight: 40
                    spacing: 16
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
                        MyText {
                            text: "Tracking Universe"
                            color: "#8b929a"
                        }
                        MyText {
                            id: summaryPlaySpaceModeText
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignRight
                            text: "Unknown"
                        }
                    }
                    VerticalSeparator {
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
                        MyText {
                            text: "HMD Rotations"
                            color: "#8b929a"
                        }
                        MyText {
                            id: summaryHmdRotationsText
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignRight
                            text: "-999.9"
                        }
                    }
                }

                LineSeparator {
                }

                RowLayout {
                    Layout.preferredHeight: 40
                    spacing: 16
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
                        MyText {
                            text: "Dropped Frames"
                            color: "#8b929a"
                        }
                        MyText {
                            id: summaryDroppedFramesText
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignRight
                            text: "0"
                        }
                    }
                    VerticalSeparator {
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
                        MyText {
                            text: "Reprojected Frames"
                            color: "#8b929a"
                        }
                        MyText {
                            id: summaryReprojectedFramesText
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignRight
                            text: "0"
                        }
                    }
                }

                LineSeparator {
                }

                RowLayout {
                    Layout.preferredHeight: 40
                    spacing: 16
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
                        MyText {
                            text: "Timed Out"
                            color: "#8b929a"
                        }
                        MyText {
                            id: summaryTimedOutText
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignRight
                            text: "0"
                        }
                    }
                    VerticalSeparator {
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
                        MyText {
                            text: "Reprojection Ratio"
                            color: "#8b929a"
                        }
                        MyText {
                            id: summaryTotalRatioText
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignRight
                            text: "0"
                        }
                    }
                }
            }
        }

        SettingsGroupBox {
            title: "Microphone"
            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                RowLayout {
                    spacing: 8
                    MyPushButton2 {
                        id: summaryMicMuteToggle
                        Layout.preferredWidth: 44
                        Layout.preferredHeight: 40
                        checkable: true
                        contentItem: Image {
                            source: parent.checked ? "qrc:/qt/qml/AdvancedSettings/src/res/img/audio/microphone/mic_off.svg" : "qrc:/qt/qml/AdvancedSettings/src/res/img/audio/microphone/mic_on.svg"
                            sourceSize.width: 26
                            sourceSize.height: 26
                            fillMode: Image.PreserveAspectFit
                        }
                        onCheckedChanged: {
                            AudioTabController.setMicMuted(checked)
                        }
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
                        Layout.preferredWidth: 64
                        horizontalAlignment: Text.AlignRight
                    }
                }

                LineSeparator {
                }

                MyToggleButton {
                    id: summaryPttToggle
                    text: "Push-to-Talk"
                    Layout.fillWidth: true
                    onClicked: {
                         AudioTabController.setPttEnabled(checked, true)
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        RowLayout {
            spacing: 12

            Rectangle {
                id: summaryVersionCheckTextRect
                visible: true
                color: "transparent"
                radius: 3
                MyText {
                    id: summaryVersionCheckText
                    anchors.verticalCenter: parent.verticalCenter
                    text: ""
                    color: "#ffffff"
                    font.pointSize: 15
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit
                    leftPadding: 12
                    rightPadding: 12
                    horizontalAlignment: Text.AlignLeft
                    width: parent.width
                    wrapMode: Text.Wrap
                }
                height: 36
                Layout.fillWidth: true
            }

            MyText {
                id: summaryVersionText
                text: "v0.0.0"
                font.pointSize: 13
                color: "#8b929a"
                horizontalAlignment: Text.AlignRight
            }
        }
    }

   Component.onCompleted: {
       reloadChaperoneProfiles()
       reloadVideoProfiles()

       summaryVersionText.text = applicationVersion
       summaryVersionCheckText.text = OverlayController.versionCheckText
       if (OverlayController.newVersionDetected)
       {
           summaryVersionCheckTextRect.color = "#c4372d"
       } else {
           summaryVersionCheckTextRect.color = "transparent"
       }


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
       target: OverlayController 
       function onNewVersionDetectedChanged() {
           if (OverlayController.newVersionDetected)
           {
               summaryVersionCheckTextRect.color = "#c4372d"
           } else {
               summaryVersionCheckTextRect.color = "transparent"
           }
       }
       function onVersionCheckTextChanged() {
           summaryVersionCheckText.text = OverlayController.versionCheckText
       }
       function onDisableVersionCheckChanged() {
           if (OverlayController.disableVersionCheck)
           {
               summaryVersionCheckText.visible = false
               summaryVersionCheckTextRect.color = "transparent"
           } else {
               summaryVersionCheckText.visible = true
               if (OverlayController.newVersionDetected)
               {
                   summaryVersionCheckTextRect.color = "#c4372d"
               } else {
                   summaryVersionCheckTextRect.color = "transparent"
               }
           }
       }
   }

   Connections {
       target: ChaperoneTabController
       function onChaperoneProfilesUpdated() {
           reloadChaperoneProfiles()
       }
   }

   Connections {
       target: VideoTabController
       function onVideoProfilesUpdated() {
           reloadVideoProfiles()
       }
   }

   Connections {
       target: AudioTabController
       function onMicVolumeChanged() {
           summaryMicVolumeSlider.value = AudioTabController.micVolume
       }
       function onMicMutedChanged() {
           summaryMicMuteToggle.checked = AudioTabController.micMuted
       }
       function onPttEnabledChanged() {
           summaryPttToggle.checked = AudioTabController.pttEnabled
       }
   }

   Connections {
       target: MoveCenterTabController
       function onTrackingUniverseChanged() {
           if (MoveCenterTabController.trackingUniverse === 0) {
               summaryPlaySpaceModeText.text = "Sitting"
           } else if (MoveCenterTabController.trackingUniverse === 1) {
               summaryPlaySpaceModeText.text = "Standing"
           } else {
               summaryPlaySpaceModeText.text = "Unknown(" + MoveCenterTabController.trackingUniverse + ")"
           }
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


    function reloadVideoProfiles() {
       var profiles = [""]
       var profileCount = VideoTabController.getVideoProfileCount()
       for (var i = 0; i < profileCount; i++) {
           profiles.push(VideoTabController.getVideoProfileName(i))
       }
       summaryVideoProfileComboBox.currentIndex = 0
       summaryVideoProfileComboBox.model = profiles
    }
}
