import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "brightness"
import "color"
import "supersampling"
import "miscvid"
import "profiles"
import "overlaycolorbright"

MyStackViewPage {
    id: videoPage
    headerText: qsTr("Video Settings")

    // Every group box row on this page uses this height.
    readonly property int groupBoxRowHeight: 44
    content: ColumnLayout {
        spacing: 8

        OverlayMethodGroupBox{
            id: overlayMethodGroupBox
            rowHeight: groupBoxRowHeight
        }
        BrightnessGroupBox {
            id: brightnessGroupBox
            rowHeight: groupBoxRowHeight
        }
        ColorGroupBox {
            id: colorGroupBox
            rowHeight: groupBoxRowHeight
        }
        SuperSamplingGroupBox{
            id: ssGroupBox
            rowHeight: groupBoxRowHeight
        }
        VideoProfileGroupBox{
            id: videoProfileGroupBox
            rowHeight: groupBoxRowHeight
            dialogParent: videoPage
        }

        VideoMiscGroupBox{
            id: miscVideoGroupBox
            rowHeight: groupBoxRowHeight
        }

        Item {
            Layout.fillHeight: true
        }

    }
    Component.onCompleted: {
        overlayMethodGroupBox.visible = VideoTabController.isOverlayMethodActive
        brightnessGroupBox.visible = !(VideoTabController.isOverlayMethodActive);
        colorGroupBox.visible = !(VideoTabController.isOverlayMethodActive);
    }

    Connections {
        target: VideoTabController
            function onIsOverlayMethodActiveChanged(){
                overlayMethodGroupBox.visible = VideoTabController.isOverlayMethodActive
                brightnessGroupBox.visible = !(VideoTabController.isOverlayMethodActive);
                colorGroupBox.visible = !(VideoTabController.isOverlayMethodActive);
            }

    }

}
