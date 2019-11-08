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
    width: 1200
    headerText: "Video Settings"
    content: ColumnLayout {
        spacing: 10

        OverlayMethodGroupBox{
        id: overlayMethodGroupBox
        }
        BrightnessGroupBox {
        id: brightnessGroupBox}
        ColorGroupBox {
        id:  colorGroupBox}
        SuperSamplingGroupBox{
            id: ssGroupBox
        }
        VideoProfileGroupBox{
            id: videoProfileGroupBox
        }

        VideoMiscGroupBox{
            id:miscVideoGroupBox
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
            onIsOverlayMethodActiveChanged:{
                overlayMethodGroupBox.visible = VideoTabController.isOverlayMethodActive
                brightnessGroupBox.visible = !(VideoTabController.isOverlayMethodActive);
                colorGroupBox.visible = !(VideoTabController.isOverlayMethodActive);
            }

    }

}
