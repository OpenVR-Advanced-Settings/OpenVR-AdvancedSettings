import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import ".."
import "../utilities_page"
import "../audio_page"
import "../motion_page"
import "../video_page"
import "../chaperone_page"
import "../steamvr_page"
import "../rotation_page"
import "../chaperone_page/chaperone_additional"
import "../steamvr_page/steamvr_additional"
import ovras.advsettings

Rectangle {
    id: root
    color: "#1d2026"
    width: 1400
    height: 800

    // Index into sideBarModel of the page currently shown.
    property int currentSideBarIndex: 0

    // Width of the widest sidebar entry (icon + label), used to center the entries.
    property real sideBarMenuWidth: 0

    readonly property string iconBase: "qrc:/qt/qml/AdvancedSettings/src/res/img/main_menu_icons/"

    // Languages offered in the footer language dropdown.
    // TODO: hook up to an actual translation backend.
    property var languageModel: [
        { code: "EN", name: "English" },
        { code: "DE", name: "Deutsch" },
        { code: "ES", name: "Español" },
        { code: "FR", name: "Français" },
        { code: "PT", name: "Português" },
        { code: "RU", name: "Русский" }
    ]
    property int currentLanguageIndex: 0

    // page: top level page to show, action: function to run instead,
    // separator: draw a divider above the entry.
    property var sideBarModel: [
        { text: "Home", icon: "home_tab_icon.svg", page: rootPage },
        { text: "SteamVR", icon: "steamvr_tab_icon.svg", page: steamVRPage },
        { text: "Chaperone", icon: "chaperone_tab_icon.svg", page: chaperonePage },
        { text: "Offsets", icon: "offsets_tab_icon.svg", page: playspacePage },
        { text: "Motion", icon: "motion_tab_icon.svg", page: motionPage },
        { text: "Rotation", icon: "rotation_tab_icon.svg", page: rotationPage },
        { text: "Space Fix", icon: "space_fix_tab_icon.svg", page: fixFloorPage },
        { text: "Audio", icon: "audio_tab_icon.svg", page: audioPage, separator: true },
        { text: "Video", icon: "video_tab_icon.svg", page: videoPage },
        { text: "Utilities", icon: "utilities_tab_icon.svg", page: utilitiesPage },
        { text: "Statistics", icon: "statistics_tab_icon.svg", page: statisticsPage },
        { text: "Bindings", icon: "bindings_tab_icon.svg", action: function() { SteamVRTabController.launchBindingUI() }, separator: true },
        { text: "Settings", icon: "settings_tab_icon.svg", page: settingsPage }
    ]

    function showSideBarPage(index) {
        var entry = sideBarModel[index]
        if (entry.action) {
            OverlayController.playActivationSound()
            entry.action()
            return
        }
        OverlayController.playFocusChangedSound()
        currentSideBarIndex = index
        if (mainView.depth !== 1 || mainView.currentItem !== entry.page) {
            mainView.replace(null, entry.page)
        }
    }

    Rectangle {
        id: sideBar
        width: 200
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        color: "#262a31"

        Column {
            anchors.fill: parent
            anchors.topMargin: 18
            spacing: 2

            // Shrinks the font until the title fits the sidebar width.
            MyText {
                x: 16
                width: parent.width - 32
                text: "OVR Advanced Settings"
                font.pointSize: 20
                minimumPointSize: 8
                fontSizeMode: Text.HorizontalFit
                font.bold: true
                color: "#8b929a"
                horizontalAlignment: Text.AlignHCenter
                bottomPadding: 10
            }

            Repeater {
                model: root.sideBarModel
                delegate: Column {
                    width: sideBar.width
                    spacing: 2

                    Rectangle {
                        visible: modelData.separator == true
                        x: 24
                        width: parent.width - 48
                        height: 1
                        color: "#3a4049"
                    }
                    Item {
                        visible: modelData.separator == true
                        width: 1
                        height: 4
                    }

                    Button {
                        id: sideBarButton
                        width: parent.width
                        height: 42
                        hoverEnabled: true
                        property bool selected: index == root.currentSideBarIndex

                        background: Rectangle {
                            color: sideBarButton.activeFocus && !sideBarButton.selected ? "#323741" : "transparent"
                            Rectangle {
                                anchors.fill: parent
                                visible: sideBarButton.selected
                                gradient: Gradient {
                                    orientation: Gradient.Horizontal
                                    GradientStop { position: 0.0; color: "#2d6cae" }
                                    GradientStop { position: 1.0; color: "#26364a" }
                                }
                            }
                            Rectangle {
                                width: 4
                                height: parent.height
                                visible: sideBarButton.selected
                                color: "#1a9fff"
                            }
                        }

                        // Entries are centered as a block, so icons stay aligned.
                        contentItem: Row {
                            leftPadding: Math.max(0, (sideBar.width - root.sideBarMenuWidth) / 2 - sideBarButton.leftPadding)
                            spacing: 16
                            Image {
                                anchors.verticalCenter: parent.verticalCenter
                                source: root.iconBase + modelData.icon
                                sourceSize.width: 26
                                sourceSize.height: 26
                                opacity: sideBarButton.selected ? 1.0 : 0.85
                            }
                            MyText {
                                anchors.verticalCenter: parent.verticalCenter
                                text: modelData.text
                                color: sideBarButton.selected || sideBarButton.activeFocus ? "#ffffff" : "#b8bcbf"
                                // icon (26) + row spacing (16) + label
                                function reportWidth() {
                                    root.sideBarMenuWidth = Math.max(root.sideBarMenuWidth, 26 + 16 + implicitWidth)
                                }
                                onImplicitWidthChanged: reportWidth()
                                Component.onCompleted: reportWidth()
                            }
                        }

                        onHoveredChanged: {
                            if (hovered) {
                                forceActiveFocus()
                            } else {
                                focus = false
                            }
                        }
                        onClicked: {
                            root.showSideBarPage(index)
                        }
                    }
                }
            }
        }

        // Footer bar: Discord invite link and language indicator.
        Rectangle {
            id: sideBarFooter
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 44
            color: "transparent"

            Rectangle {
                anchors.top: parent.top
                x: 24
                width: parent.width - 48
                height: 1
                color: "#3a4049"
            }

            Button {
                id: discordButton
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.verticalCenter: parent.verticalCenter
                width: 36
                height: 32
                // Material's default padding/insets would leave no room for the icon.
                padding: 6
                topInset: 0
                bottomInset: 0
                hoverEnabled: true
                background: Rectangle {
                    radius: 4
                    color: discordButton.activeFocus ? "#323741" : "transparent"
                }
                // A bare Image as a Material Button's contentItem doesn't draw; wrap it.
                contentItem: Item {
                    Image {
                        anchors.centerIn: parent
                        source: root.iconBase + "discord_tab_icon.svg"
                        sourceSize.width: 24
                        sourceSize.height: 24
                    }
                }
                onHoveredChanged: {
                    if (hovered) {
                        forceActiveFocus()
                    } else {
                        focus = false
                    }
                }
                onClicked: {
                    OverlayController.playActivationSound()
                    Qt.openUrlExternally("https://discord.gg/CX8Kh8G")
                }
            }

            Button {
                id: languageButton
                anchors.right: parent.right
                anchors.rightMargin: 14
                anchors.verticalCenter: parent.verticalCenter
                height: 32
                leftPadding: 8
                rightPadding: 8
                topPadding: 0
                bottomPadding: 0
                topInset: 0
                bottomInset: 0
                hoverEnabled: true
                background: Rectangle {
                    radius: 4
                    color: languageButton.activeFocus || languagePopup.opened ? "#323741" : "transparent"
                }
                contentItem: Row {
                    spacing: 6
                    MyText {
                        anchors.verticalCenter: parent.verticalCenter
                        text: root.languageModel[root.currentLanguageIndex].code
                        font.pointSize: 11
                        color: languageButton.activeFocus || languagePopup.opened ? "#ffffff" : "#b8bcbf"
                    }
                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: root.iconBase + "language_icon.svg"
                        sourceSize.width: 20
                        sourceSize.height: 20
                    }
                }
                onHoveredChanged: {
                    if (hovered) {
                        forceActiveFocus()
                    } else {
                        focus = false
                    }
                }
                onClicked: {
                    OverlayController.playActivationSound()
                    if (languagePopup.opened) {
                        languagePopup.close()
                    } else {
                        languagePopup.open()
                    }
                }

                // Opens upward, right-aligned with the button.
                Popup {
                    id: languagePopup
                    width: 180
                    x: languageButton.width - width
                    y: -height - 6
                    padding: 4
                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
                    background: Rectangle {
                        color: "#2f343c"
                        radius: 4
                        border.color: "#3a4049"
                        border.width: 1
                    }
                    contentItem: Column {
                        spacing: 2
                        Repeater {
                            model: root.languageModel
                            delegate: Button {
                                id: languageItem
                                width: languagePopup.availableWidth
                                height: 36
                                hoverEnabled: true
                                property bool selected: index == root.currentLanguageIndex
                                background: Rectangle {
                                    radius: 3
                                    color: languageItem.selected ? "#2d6cae"
                                         : languageItem.activeFocus ? "#3a4049" : "transparent"
                                }
                                contentItem: Row {
                                    leftPadding: 10
                                    spacing: 12
                                    MyText {
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: 28
                                        text: modelData.code
                                        font.pointSize: 11
                                        color: "#8b929a"
                                    }
                                    MyText {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: modelData.name
                                        font.pointSize: 11
                                        color: languageItem.selected || languageItem.activeFocus ? "#ffffff" : "#b8bcbf"
                                    }
                                }
                                onHoveredChanged: {
                                    if (hovered) {
                                        forceActiveFocus()
                                    } else {
                                        focus = false
                                    }
                                }
                                onClicked: {
                                    OverlayController.playFocusChangedSound()
                                    root.currentLanguageIndex = index
                                    languagePopup.close()
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    property RootPage rootPage: RootPage {
        stackView: mainView
    }

    property SteamVRPage steamVRPage: SteamVRPage {
        stackView: mainView
        visible: false
    }

    property ChaperonePage chaperonePage: ChaperonePage {
        stackView: mainView
        visible: false
    }

    property ChaperoneWarningsPage chaperoneWarningsPage: ChaperoneWarningsPage {
        stackView: mainView
        visible: false
    }

    property ChaperoneAdditionalPage chaperoneAdditionalPage: ChaperoneAdditionalPage{
        stackView: mainView
        visible: false
    }

    property SteamVRTXRXPage steamVRTXRXPage: SteamVRTXRXPage{
        stackView: mainView
        visible: false
    }

    property PlayspacePage playspacePage: PlayspacePage {
        stackView: mainView
        visible: false
    }

    property MotionPage motionPage: MotionPage {
        stackView: mainView
        visible: false
    }

    property RotationPage  rotationPage: RotationPage {
        stackView: mainView
        visible: false
    }

    property FixFloorPage fixFloorPage: FixFloorPage {
        stackView: mainView
        visible: false
    }

    property StatisticsPage statisticsPage: StatisticsPage {
        stackView: mainView
        visible: false
    }

    property SettingsPage settingsPage: SettingsPage {
        stackView: mainView
        visible: false
    }

    property AudioPage audioPage: AudioPage {
        stackView: mainView
        visible: false
    }

    property UtilitiesPage utilitiesPage: UtilitiesPage {
        stackView: mainView
        visible: false
    }

    property VideoPage videoPage: VideoPage {
        stackView: mainView
        visible:false
    }

    StackView {
        id: mainView
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: sideBar.right
        anchors.right: parent.right
        clip: true

		replaceEnter: Transition {
			PropertyAnimation {
				property: "opacity"
				from: 0
				to: 1
				duration: 150
			}
		}
		replaceExit: Transition {
			PropertyAnimation {
				property: "opacity"
				from: 1
				to: 0
				duration: 150
			}
		}

		pushEnter: Transition {
			PropertyAnimation {
				property: "x"
				from: mainView.width
				to: 0
				duration: 200
			}
		}
		pushExit: Transition {
			PropertyAnimation {
				property: "x"
				from: 0
				to: -mainView.width
				duration: 200
			}
		}
		popEnter: Transition {
			PropertyAnimation {
				property: "x"
				from: -mainView.width
				to: 0
				duration: 200
			}
		}
		popExit: Transition {
			PropertyAnimation {
				property: "x"
				from: 0
				to: mainView.width
				duration: 200
			}
		}

        initialItem: rootPage
    }
}
