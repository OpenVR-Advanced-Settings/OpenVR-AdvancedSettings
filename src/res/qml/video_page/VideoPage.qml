import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../common"
import "brightness"
import "color"
import "supersampling"
import "miscvid"

MyStackViewPage {
    width: 1200
    headerText: "Video Settings"

    content: ColumnLayout {
        spacing: 16

        BrightnessGroupBox {
        id: brightnessGroupBox}
        ColorGroupBox {
        id:  colorGroupBox}
        SuperSamplingGroupBox{
            id: ssGroupBox
        }
        VideoMiscGroupBox{
            id:miscVideoGroupBox
        }

        Item {
            Layout.fillHeight: true
        }

    }
}
