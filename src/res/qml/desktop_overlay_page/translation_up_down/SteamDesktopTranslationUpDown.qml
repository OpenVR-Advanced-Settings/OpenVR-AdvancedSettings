import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

FullWidthSliderBox {
    headerMessage: "Up/Down Translation"

    lowerLimit: -4.0
    upperLimit: 4.0
    sliderStepSize: 0.10
    kbUID: 602

    function onValueChanged(value) {
        DesktopOverlay.height = value
        sliderText = value.toFixed(2)
    }
    function onComponentComplete() {
        sliderValue = DesktopOverlay.height
    }
}
