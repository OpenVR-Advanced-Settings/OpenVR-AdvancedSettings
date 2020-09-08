import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: viewRatchetGroupBox
    Layout.fillWidth: true

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            MyToggleButton{
                id: viewRatchetToggleBtn
                text: "View Ratchetting"
                Layout.preferredWidth: 275
                onCheckedChanged: {
                    RotationTabController.setViewRatchettingEnabled(this.checked, true);
                }
            }
            Item{
            Layout.preferredWidth: 25
            }

            MySlider {
                id: viewRatchetSlider
                from: 0.00
                to: 0.1
                stepSize: 0.01
                Layout.fillWidth: true
                onMoved: {
                    viewRatchetText.text = this.value.toFixed(2)
                    RotationTabController.setViewRatchettingPercent(this.value, true)
                }

            }

            MyTextField {
                id: viewRatchetText
                text: "0.05"
                keyBoardUID: 1007
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0) {
                            val = 0
                        } else if (val > 1) {
                            val = 1
                        }
                        RotationTabController.setViewRatchettingPercent(val.toFixed(2), true)
                        if(val > 0.1){
                        viewRatchetSlider.value = 0.1
                        }else{
                        viewRatchetSlider.value = val
                        }
                    }
                    text =  RotationTabController.viewRatchettingPercent.toFixed(2);
                }
            }
        }
    }

    Component.onCompleted: {
        viewRatchetToggleBtn.checked = RotationTabController.viewRatchettingEnabled
        var ratchetPercVal = RotationTabController.viewRatchettingPercent
        viewRatchetSlider.value = ratchetPercVal
        viewRatchetText.text = ratchetPercVal.toFixed(2)
    }

    Connections {
        target: RotationTabController

        onViewRatchettingEnabledChanged:{
            viewRatchetToggleBtn.checked = RotationTabController.viewRatchettingEnabled
        }
        onViewRatchettingPercentChanged:{
            var ratchetPercVal = RotationTabController.viewRatchettingPercent
            viewRatchetSlider.value = ratchetPercVal
            viewRatchetText.text = ratchetPercVal.toFixed(2)
        }
    }
}
