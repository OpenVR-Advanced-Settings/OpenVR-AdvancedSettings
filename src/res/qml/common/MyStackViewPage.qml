import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
import QtQuick.Templates as T
import ovras.advsettings
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
// Needed for OverlayController in the default folder.
import ".."

Rectangle {
    color: "#1d2026"

    // Template type so it accepts the StackView of any style (mainwidget uses Material).
    property T.StackView stackView

    property string headerText: qsTr("Header Title")

    // Top level pages are reached from the sidebar, only sub pages need a back button.
    property bool headerShowBackButton: stackView ? stackView.depth > 1 : false

    property Item header: ColumnLayout {
        RowLayout {
            spacing: 12
            Button {
                id: headerBackButton
                Layout.preferredHeight: 40
                Layout.preferredWidth: 40
                hoverEnabled: true
                enabled: headerShowBackButton
                visible: headerShowBackButton
                contentItem: Image {
                    source: "qrc:/qt/qml/AdvancedSettings/img/common/backarrow.svg"
                    sourceSize.width: 40
                    sourceSize.height: 40
                    anchors.fill: parent
                }
                 background: Rectangle {
                     opacity: parent.down ? 1.0 : (parent.activeFocus ? 0.5 : 0.0)
                     color: "#4b5361"
                     radius: 20
                     anchors.fill: parent
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
                    stackView.pop()
                }
            }
            MyText {
                id: headerTitle
                text: headerText
                font.pointSize: 24
                font.bold: true
                color: "#ffffff"
            }
        }
    }

    property Item content: Frame {
        MyText {
            text: qsTr("Content")
        }
    }

    ColumnLayout {
        id: mainLayout
        spacing: 12
        anchors.fill: parent
    }

    // Content scrolls vertically when it is taller than the page.
    Flickable {
        id: contentFlickable
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.topMargin: 4
        Layout.bottomMargin: 24
        clip: true
        contentWidth: width - leftMargin - rightMargin
        flickableDirection: Flickable.VerticalFlick
        contentHeight: Math.max(height, content.implicitHeight)
        interactive: contentHeight > height
        boundsBehavior: Flickable.StopAtBounds
        leftMargin: 28
        rightMargin: 28

        ScrollBar.vertical: ScrollBar {
            policy: contentFlickable.interactive ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
            contentItem: Rectangle {
                implicitWidth: 6
                radius: 3
                color: parent.pressed ? "#8b929a" : "#4b5361"
            }
        }
    }

    Component.onCompleted: {
        header.parent = mainLayout
        header.Layout.leftMargin = 28
        header.Layout.topMargin = 22
        header.Layout.rightMargin = 28
        contentFlickable.parent = mainLayout
        content.parent = contentFlickable.contentItem
        content.width = Qt.binding(function() { return contentFlickable.width - contentFlickable.leftMargin - contentFlickable.rightMargin })
        content.height = Qt.binding(function() { return contentFlickable.contentHeight })
    }
    Connections {
    target: OverlayController 
    }
}
