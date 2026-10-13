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
    width: 1450
    height: 800

    // Index into sideBarModel of the page currently shown.
    property int currentSideBarIndex: 0

    readonly property string iconBase: "qrc:/qt/qml/AdvancedSettings/src/res/img/main_menu_icons/"

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
        width: 250
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        color: "#262a31"

        Column {
            anchors.fill: parent
            anchors.topMargin: 18
            spacing: 2

            MyText {
                x: 24
                width: parent.width - 32
                text: "Advanced Settings"
                font.pointSize: 13
                font.bold: true
                color: "#8b929a"
                elide: Text.ElideRight
                bottomPadding: 10
            }

            Repeater {
                model: root.sideBarModel
                delegate: Column {
                    width: sideBar.width
                    spacing: 2

                    Rectangle {
                        visible: modelData.separator === true
                        x: 24
                        width: parent.width - 48
                        height: 1
                        color: "#3a4049"
                    }
                    Item {
                        visible: modelData.separator === true
                        width: 1
                        height: 4
                    }

                    Button {
                        id: sideBarButton
                        width: parent.width
                        height: 42
                        hoverEnabled: true
                        property bool selected: index === root.currentSideBarIndex

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

                        contentItem: Row {
                            leftPadding: 22
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
