import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

FullWidthSliderBox {
    headerMessage: "Forwards/Backwards Translation"

    lowerLimit: -4.0
    upperLimit: 4.0
    sliderStepSize: 0.10
    kbUID: 603

    function onValueChanged(value) {
        DesktopOverlay.forwardsMovement = value
        sliderText = value.toFixed(2)
    }
    function onComponentComplete() {
        sliderValue = DesktopOverlay.forwardsMovement
    }
}
