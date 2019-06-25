import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "common"

MyStackViewPage {
    id: myStackViewPage1
    headerText: "SteamVR Settings"


    MyDialogOkPopup {
        id: steamvrMessageDialog
        function showMessage(title, text) {
            dialogTitle = title
            dialogText = text
            open()
        }
    }

    MyDialogOkCancelPopup {
        id: steamvrDeleteProfileDialog
        property int profileIndex: -1
        dialogTitle: "Delete Profile"
        dialogText: "Do you really want to delete this profile?"
        onClosed: {
            if (okClicked) {
                SteamVRTabController.deleteSteamVRProfile(profileIndex)
            }
        }
    }

    MyDialogOkCancelPopup {
        id: steamvrNewProfileDialog
        dialogTitle: "Create New Profile"
        dialogWidth: 600
        dialogHeight: 400
        dialogContentItem: ColumnLayout {
            RowLayout {
                Layout.topMargin: 16
                Layout.leftMargin: 16
                Layout.rightMargin: 16
                MyText {
                    text: "Name: "
                }
                MyTextField {
                    id: steamvrNewProfileName
                    keyBoardUID: 490
                    color: "#cccccc"
                    text: ""
                    Layout.fillWidth: true
                    font.pointSize: 20
                    function onInputEvent(input) {
                        steamvrNewProfileName.text = input
                    }
                }
            }
            MyText {
                Layout.topMargin: 24
                text: "What to include:"
            }
            MyToggleButton {
                id: steamvrNewProfileIncludeSupersampling
                Layout.leftMargin: 32
                text: "Application Supersampling"
            }
            MyToggleButton {
                id: steamvrNewProfileIncludeSupersampleFiltering
                Layout.leftMargin: 32
                text: "Supersample Filtering"
            }
            MyToggleButton {
                id: steamvrNewProfileIncludeMotionSmoothing
                Layout.leftMargin: 32
                text: "Motion Smoothing Setting"
            }
        }
        onClosed: {
            if (okClicked) {
                if (steamvrNewProfileName.text == "") {
                    steamvrMessageDialog.showMessage("Create New Profile", "ERROR: Empty profile name.")
                } else if (!steamvrNewProfileIncludeSupersampling.checked
                            && !steamvrNewProfileIncludeSupersampleFiltering.checked
                            && !steamvrNewProfileIncludeMotionSmoothing.checked) {
                    steamvrMessageDialog.showMessage("Create New Profile", "ERROR: Nothing included.")
                } else {
                    SteamVRTabController.addSteamVRProfile(steamvrNewProfileName.text,
                        steamvrNewProfileIncludeSupersampling.checked,
                        steamvrNewProfileIncludeSupersampleFiltering.checked,
                       steamvrNewProfileIncludeMotionSmoothing.checked
                                                           )
                }

            }
        }
        function openPopup() {
            steamvrNewProfileName.text = ""
            steamvrNewProfileIncludeSupersampling.checked = false
            steamvrNewProfileIncludeSupersampleFiltering.checked = false
            steamvrNewProfileIncludeMotionSmoothing.checked = false
            open()
        }
    }



    content: ColumnLayout {
        spacing: 18

        ColumnLayout {
            Layout.bottomMargin: 32
            spacing: 18
            RowLayout {
                spacing: 18

                MyText {
                    text: "Profile:"
                }

                MyComboBox {
                    id: steamvrProfileComboBox
                    Layout.maximumWidth: 799
                    Layout.minimumWidth: 799
                    Layout.preferredWidth: 799
                    Layout.fillWidth: true
                    model: [""]
                    onCurrentIndexChanged: {
                        if (currentIndex > 0) {
                            steamvrApplyProfileButton.enabled = true
                            steamvrDeleteProfileButton.enabled = true
                        } else {
                            steamvrApplyProfileButton.enabled = false
                            steamvrDeleteProfileButton.enabled = false
                        }
                    }
                }

                MyPushButton {
                    id: steamvrApplyProfileButton
                    enabled: false
                    Layout.preferredWidth: 200
                    text: "Apply"
                    onClicked: {
                        if (steamvrProfileComboBox.currentIndex > 0) {
                            SteamVRTabController.applySteamVRProfile(steamvrProfileComboBox.currentIndex - 1)
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
                    id: steamvrDeleteProfileButton
                    enabled: false
                    Layout.preferredWidth: 200
                    text: "Delete Profile"
                    onClicked: {
                        if (steamvrProfileComboBox.currentIndex > 0) {
                            steamvrDeleteProfileDialog.profileIndex = steamvrProfileComboBox.currentIndex - 1
                            steamvrDeleteProfileDialog.open()
                        }
                    }
                }
                MyPushButton {
                    Layout.preferredWidth: 200
                    text: "New Profile"
                    onClicked: {
                        steamvrNewProfileDialog.openPopup()
                    }
                }
            }
        }

        RowLayout {
            spacing: 16

            MyText {
                text: "Application Supersampling:"
                Layout.preferredWidth: 350
                Layout.rightMargin: 12
            }

            MyPushButton2 {
                id: steamvrSupersampleDecrement
                text: "-"
                Layout.preferredWidth: 40
                onClicked: {
                    steamvrSupersamplingSlider.decrease()
                }
            }

            MySlider {
                id: steamvrSupersamplingSlider
                from: 0.2
                to: 5.0
                stepSize: 0.1
                value: 1.0
                snapMode: Slider.SnapAlways
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = this.from + ( this.position  * (this.to - this.from))
                    steamvrSupersamplingText.text = val.toFixed(1)
                }
                onValueChanged: {
                    SteamVRTabController.setSuperSampling(this.value.toFixed(1), false)
                }
            }

            MyPushButton2 {
                id: steamvrSupersampleIncrement
                text: "+"
                Layout.preferredWidth: 40
                onClicked: {
                    steamvrSupersamplingSlider.increase()
                }
            }

            MyTextField {
                id: steamvrSupersamplingText
                text: "0.0"
                keyBoardUID: 401
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0.1) {
                            val = 0.1
                        }
                        var v = val.toFixed(1)
                        if (v <= steamvrSupersamplingSlider.to) {
                            steamvrSupersamplingSlider.value = v
                        } else {
                            SteamVRTabController.setSuperSampling(v, false)
                        }
                    }
                    text = SteamVRTabController.superSampling.toFixed(1)
                }
            }
        }
        MyToggleButton {
            id: steamvrAllowSupersampleOverrideToggle
            text: "Enable Manual Supersampling Override"
            onCheckedChanged: {
                SteamVRTabController.setAllowSupersampleOverride(this.checked, false)
                if(!this.checked){
                    steamvrSupersamplingText.enabled = false
                    steamvrSupersamplingSlider.enabled = false
                    steamvrSupersampleIncrement.enabled = false
                    steamvrSupersampleDecrement.enabled = false
                }else{
                    steamvrSupersamplingText.enabled = true
                    steamvrSupersamplingSlider.enabled = true
                    steamvrSupersampleIncrement.enabled = true
                    steamvrSupersampleDecrement.enabled = true
                }
            }
        }
        MyToggleButton {
            id: steamvrAllowSupersampleFilteringToggle
            text: "Enable Advanced Supersample Filtering"
            onCheckedChanged: {
                SteamVRTabController.setAllowSupersampleFiltering(this.checked, false)
            }
        }

        MyToggleButton {
            id: steamvrMotionSmoothingToggle
            text: "Enable Motion Smoothing"
            onCheckedChanged: {
                SteamVRTabController.setMotionSmoothing(this.checked, false)
            }
        }

        MyToggleButton {
            id: steamvrPerformanceGraphToggle
            text: "Enable Timing Overlay"
            onCheckedChanged: {
                SteamVRTabController.setPerformanceGraph(this.checked, false)
            }
        }

        Item { Layout.fillHeight: true; Layout.fillWidth: true}

        RowLayout {
            Layout.fillWidth: true

            MyPushButton {
                id: steamVRResetButton
                text: "Reset"
                Layout.preferredWidth: 250
                onClicked: {
                    SteamVRTabController.reset()
                }
            }

            Item { Layout.fillWidth: true}

            MyPushButton {
                id: steamVRRestartButton
                text: "Restart SteamVR"
                Layout.preferredWidth: 250
                onClicked: {
                    SteamVRTabController.restartSteamVR()
                }
            }
        }

        Component.onCompleted: {
            var s1 = SteamVRTabController.superSampling.toFixed(1)
            if (s1 <= steamvrSupersamplingSlider.to) {
                steamvrSupersamplingSlider.value = s1
            }
            steamvrAllowSupersampleOverrideToggle.checked = SteamVRTabController.allowSupersampleOverride
            steamvrAllowSupersampleFilteringToggle.checked = SteamVRTabController.allowSupersampleFiltering
            steamvrMotionSmoothingToggle.checked = SteamVRTabController.motionSmoothing
            steamvrPerformanceGraphToggle.checked = SteamVRTabController.performanceGraph
            if(!steamvrAllowSupersampleOverrideToggle.checked){
                steamvrSupersamplingText.enabled = false
                steamvrSupersamplingSlider.enabled = false
                steamvrSupersampleIncrement.enabled = false
                steamvrSupersampleDecrement.enabled = false
            }

            reloadSteamVRProfiles()
        }

        Connections {
            target: SteamVRTabController
            onSuperSamplingChanged: {
                var s1 = SteamVRTabController.superSampling.toFixed(1)
                if (s1 <= steamvrSupersamplingSlider.to && Math.abs(steamvrSupersamplingSlider.value-s1) > 0.08) {
                    steamvrSupersamplingSlider.value = s1
                }
                steamvrSupersamplingText.text = s1
            }
            onAllowSupersampleOverrideChanged: {
                steamvrAllowSupersampleOverrideToggle.checked = SteamVRTabController.allowSupersampleOverride
            }
            onAllowSupersampleFilteringChanged: {
                steamvrAllowSupersampleFilteringToggle.checked = SteamVRTabController.allowSupersampleFiltering
            }

            onMotionSmoothingChanged: {
                steamvrMotionSmoothingToggle.checked = SteamVRTabController.motionSmoothing
            }
            onPerformanceGraphChanged:{
                steamvrPerformanceGraphToggle.checked = SteamVRTabController.performanceGraph
            }

            onSteamVRProfilesUpdated: {
                reloadSteamVRProfiles()
            }
            onSteamVRProfileAdded:{
                steamvrProfileComboBox.currentIndex = SteamVRTabController.getSteamVRProfileCount()
            }
        }
    }

    function reloadSteamVRProfiles() {
        var profiles = [""]
        var profileCount = SteamVRTabController.getSteamVRProfileCount()
        for (var i = 0; i < profileCount; i++) {
            profiles.push(SteamVRTabController.getSteamVRProfileName(i))
        }
        steamvrProfileComboBox.currentIndex = 0
        steamvrProfileComboBox.model = profiles
    }
}
