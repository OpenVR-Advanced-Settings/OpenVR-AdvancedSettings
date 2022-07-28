import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

MyPushButton2 {
    // Public properties
    id: mediaButton
    onClicked: {
        // Overload this
    }
    property string imagePath: ""

    // Private properties
    Layout.preferredHeight: 48
    Layout.preferredWidth: 48
    contentItem: Image {
        source: parent.imagePath
        sourceSize: Qt.size( imgSP.sourceSize.width*2, imgSP.sourceSize.height*2 )
        Image{
            id: imgSP
            source: parent.source
            width:0
            height:0
        }
        anchors.fill: parent
    }

    background: Rectangle {
        // Provides feedback for mousing over and clicking buttons.
        opacity: parent.down ? 1.0 : (parent.activeFocus ? 0.5 : 0.0)
        color: "#3d4450"
        radius: 4
        anchors.fill: parent
    }
}
