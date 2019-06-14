import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import ".."
import "../utilities_page"
import "../audio_page"
import "../motion_page"
import "../video_page"
import "../desktop_overlay_page"
import "../chaperone_page"

Rectangle {
    id: root
    color: "#1b2939"
    width: 1200
    height: 800

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

    property PlayspacePage playspacePage: PlayspacePage {
        stackView: mainView
        visible: false
    }

    property MotionPage motionPage: MotionPage {
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

    property RevivePage revivePage: RevivePage {
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

    property DesktopOverlayPage desktopOverlayPage: DesktopOverlayPage {
        stackView: mainView
        visible: false
    }

    StackView {
        id: mainView
        anchors.fill: parent

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
