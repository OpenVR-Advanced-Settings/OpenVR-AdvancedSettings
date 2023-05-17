import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import ovras.advsettings 1.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
import "../../common"
import "chaperoneboundscolor"
import "chaperonetype"
import "chaperonemisc"

MyStackViewPage {
    headerText: "Additional Chaperone Settings"
    content: ColumnLayout {
        spacing: 10

        ChaperoneBoundsColorGroupBox{
        id: chaperoneBoundsColorGroupBox
        }
        ChaperoneTypeGroupBox{
            id:chaperoneTypeGroupBox
        }
        ChaperoneMiscGroupBox{
            id:chaperoneMiscGroupBox
        }

        Item {
            Layout.fillHeight: true
        }

        GridLayout {
            columns: 5

            MyText {
                text: "Disable chaperone below height: "
                Layout.preferredWidth: 400
            }
            MyPushButton2 {
                id: dimHeightMinusButton
                Layout.preferredWidth: 40
                text: "-"
                onClicked: {
                    var val = ChaperoneTabController.chaperoneDimHeight - 0.1
                    if (val <= 0.0) {
                        val = 0.0;
                    }
                    ChaperoneTabController.chaperoneDimHeight = val.toFixed(2)
                }
            }

            MySlider {
                id: dimHeightSlider
                from: 0
                to: 2.0
                stepSize: 0.01
                value: 0.0
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = this.from + ( this.position  * (this.to - this.from))
                    dimHeightText.text = val.toFixed(2)
                }
                onValueChanged: {
                    var val = dimHeightSlider.value.toFixed(2)
                    if (val < 0.01) {
                        val = 0.00
                    }
                    ChaperoneTabController.chaperoneDimHeight = val
                    dimHeightText.text = val
                }
            }

            MyPushButton2 {
                id: dimHeightPlusButton
                Layout.preferredWidth: 40
                text: "+"
                onClicked: {
                    var val = ChaperoneTabController.chaperoneDimHeight + 0.1
                    if (val > 2.0) {
                        val = 2.0;
                    }
                    ChaperoneTabController.chaperoneDimHeight = val.toFixed(2)
                }
            }

            MyTextField {
                id: dimHeightText
                text: "0.00"
                keyBoardUID: 802
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val <= 0.0) {
                            val = 0.0
                        }
                        val = val.toFixed(2)
                        ChaperoneTabController.chaperoneDimHeight = val
                        text = val
                    } else {
                        text = ChaperoneTabController.chaperoneDimHeight.toFixed(2)
                    }
                }
            }
        }

        Component.onCompleted: {
            var d = ChaperoneTabController.chaperoneDimHeight.toFixed(2)
            dimHeightSlider.value = d
            dimHeightText.text = d
        }

        Connections {
            target: ChaperoneTabController
            onChaperoneDimHeightChanged: {
                var d = ChaperoneTabController.chaperoneDimHeight.toFixed(2)
                if (d <= dimHeightSlider.to && Math.abs(dimHeightSlider.value - d) > 0.0008) {
                    dimHeightSlider.value = d
                }
                dimHeightText.text = d
            }
        }
    }
}



    //TODO content

