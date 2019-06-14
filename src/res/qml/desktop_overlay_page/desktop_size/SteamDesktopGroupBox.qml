import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

FullWidthSliderBox {
    headerMessage: "Overlay Width"

    lowerLimit: 0.0
    upperLimit: 32.0
    sliderStepSize: 0.5
    kbUID: 601

    function onValueChanged(value) {
        DesktopOverlay.width = value
        sliderText = value.toFixed(2)
    }
    function onComponentComplete() {
        sliderValue = DesktopOverlay.width
    }
}

