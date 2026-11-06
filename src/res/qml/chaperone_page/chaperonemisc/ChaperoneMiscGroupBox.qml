import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
SettingsGroupBox {
    id: chaperoneMiscGroupBox
    Layout.fillWidth: true

    title: qsTr("Misc")

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        MyToggleButton {
            id: chaperoneFloorToggleBtn
            Layout.preferredHeight: rowHeight
            text: qsTr("Floor Bounds Always On")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                ChaperoneTabController.setChaperoneFloorToggle(this.checked, false)
            }
        }
        LineSeparator {
        }
        MyToggleButton {
            id: legacyCenterMarkerbtn
            Layout.preferredHeight: rowHeight
            text: qsTr("Legacy Center Marker")
            Layout.fillWidth: true
            Layout.rightMargin: 0
            onCheckedChanged: {
                ChaperoneTabController.setCenterMarker(this.checked, false)
            }
        }
        LineSeparator {
        }
        RowLayout {
            Layout.preferredHeight: rowHeight
            spacing: 16

            MyText {
                text: qsTr("Turn Counter")
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
            }
            MyPushButton {
                id: btnResetOrientation
                text: qsTr("Reset")
                onClicked: {
                    StatisticsTabController.statsRotationResetClicked()
                }
            }
        }
        LineSeparator {
        }
        RowLayout {
            Layout.preferredHeight: rowHeight
            spacing: 16

            MyText {
                text: qsTr("Disable chaperone below height ")
                Layout.preferredWidth: 400
                horizontalAlignment: Text.AlignLeft
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
        LineSeparator {
        }
        RowLayout {
            Layout.preferredHeight: rowHeight
            spacing: 16

            MyText {
                text: qsTr("Chaperone Style")
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
            }
            ButtonGroup {
                id: chaperoneTypeExclGroup
                exclusive: true
            }
            MyRadioButton {
                id: denseRadio
                text: qsTr("Dense")
                ButtonGroup.group: chaperoneTypeExclGroup
                onCheckedChanged: {
                    if(checked){
                        OverlayController.playActivationSound()
                        ChaperoneTabController.setCollisionBoundStyle(0)
                    }
                }
            }
            MyRadioButton {
                id: medRadio
                text: qsTr("Med")
                ButtonGroup.group: chaperoneTypeExclGroup
                onCheckedChanged: {
                    if(checked){
                        OverlayController.playActivationSound()
                        ChaperoneTabController.setCollisionBoundStyle(1)
                    }
                }
            }
            MyRadioButton {
                id: squaresRadio
                text: qsTr("Squares")
                ButtonGroup.group: chaperoneTypeExclGroup
                onCheckedChanged: {
                    if(checked){
                        OverlayController.playActivationSound()
                        ChaperoneTabController.setCollisionBoundStyle(2)
                    }
                }
            }
            MyRadioButton {
                id: sparseRadio
                text: qsTr("Sparse")
                ButtonGroup.group: chaperoneTypeExclGroup
                onCheckedChanged: {
                    if(checked){
                        OverlayController.playActivationSound()
                        ChaperoneTabController.setCollisionBoundStyle(3)
                    }
                }
            }
            MyRadioButton {
                id: floorRadio
                text: qsTr("Floor Only")
                ButtonGroup.group: chaperoneTypeExclGroup
                onCheckedChanged: {
                    if(checked){
                        OverlayController.playActivationSound()
                        ChaperoneTabController.setCollisionBoundStyle(4)
                    }
                }
            }
        }
    }

    Component.onCompleted: {
            chaperoneFloorToggleBtn.checked = ChaperoneTabController.chaperoneFloorToggle
            legacyCenterMarkerbtn.checked = ChaperoneTabController.centerMarker
            var d = ChaperoneTabController.chaperoneDimHeight.toFixed(2)
            dimHeightSlider.value = d
            dimHeightText.text = d
            setCollisionBoundStyleRadio()
    }

    function setCollisionBoundStyleRadio() {
        switch(ChaperoneTabController.collisionBoundStyle){
        case 0: denseRadio.checked = true; break;
        case 1: medRadio.checked = true; break;
        case 2: squaresRadio.checked = true; break;
        case 3: sparseRadio.checked = true; break;
        case 4: floorRadio.checked = true; break;
        }
    }

    Connections {
        target: ChaperoneTabController
        function onChaperoneFloorToggleChanged(){
            chaperoneFloorToggleBtn.checked = ChaperoneTabController.chaperoneFloorToggle
        }
        function onCenterMarkerChanged(){
            legacyCenterMarkerbtn.checked = ChaperoneTabController.centerMarker
        }
        function onChaperoneDimHeightChanged() {
            var d = ChaperoneTabController.chaperoneDimHeight.toFixed(2)
            if (d <= dimHeightSlider.to && Math.abs(dimHeightSlider.value - d) > 0.0008) {
                dimHeightSlider.value = d
            }
            dimHeightText.text = d
        }
        function onCollisionBoundStyleChanged(){
            setCollisionBoundStyleRadio()
        }
    }
}
