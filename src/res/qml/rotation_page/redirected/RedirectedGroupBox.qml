import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: redirectedGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Vestibular Motion (for continuous redirected walking)"
        bottomPadding: -10
    }
    background: Rectangle {
        color: "transparent"
        border.color: "#ffffff"
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            color: "#ffffff"
            height: 1
            Layout.fillWidth: true
            Layout.bottomMargin: 5
        }

      RowLayout{
            MyToggleButton {
                id: redirectedModeToggle
                text: "Toggle On/Off"
                Layout.preferredWidth: 225
                onCheckedChanged: {
                    RotationTabController.setVestibularMotionEnabled(this.checked, true);

                }
            }
            Item{

                Layout.fillWidth: true
            }

            MyPushButton {
                id: imperceptableButton
                Layout.preferredWidth: 180
                text:"Imperceptable"
                onClicked: {
                    RotationTabController.setVestibularMotionRadius(22.0)
                }
           }
            MyPushButton {
                id: lightButton
                Layout.preferredWidth: 125
                text:"Light"
                onClicked: {
                    RotationTabController.setVestibularMotionRadius(11.0)
                }
           }
            MyPushButton {
                id: strongButton
                Layout.preferredWidth: 125
                text:"Strong"
                onClicked: {
                    RotationTabController.setVestibularMotionRadius(5.0)
                }
           }
            MyPushButton {
                id: extremeButton
                Layout.preferredWidth: 125
                text:"Extreme"
                onClicked: {
                    RotationTabController.setVestibularMotionRadius(1.0)
                }
           }

            MyText{
                text: "Radius: "
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 4
            }


            MyTextField {
                id: redirectedWalkingRadiusText
                text: "11.0"
                keyBoardUID: 1004
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0.5) {
                            val = 0.5
                        } else if (val > 50) {
                            val = 50
                        }
                            RotationTabController.setVestibularMotionRadius(val, true);
                    }
                }
            }
      }
    }
    RowLayout{
        MyToggleButton{
            id: viewRatchetToggleBtn
            text: "View Ratchetting"
            Layout.preferredWidth: 225
            onCheckedChanged: {
                RotationTabController.setViewRatchettingEnabled(this.checked, true);

            }
            MySlider {
                id: viewRatchetSlider
                from: 0.00
                to: 0.1
                stepSize: 0.01
                value: .05
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = this.value
                    viewRatchetText.text = val.toFixed(2)
                }
                onValueChanged: {

                    RotationTabController.setViewRatchettingPercent(value, true)
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
                            viewRatchetSlider.value = val.toFixed(2);
                    }
                    text =  RotationTabController.setViewRatchettingPercent.toFixed(2);
                }
            }

        }

    }

    Component.onCompleted: {
        redirectedWalkingRadiusText.text = ((RotationTabController.vestibularMotionRadius).toFixed(2))
        redirectedModeToggle.checked = RotationTabController.vestibularMotionEnabled
        viewRatchetToggleBtn.checked = RotationTabController.viewRatchettingEnabled

    }

    Connections {
        target: RotationTabController

        onVestibularMotionEnabledChanged:{
         redirectedModeToggle.checked = RotationTabController.vestibularMotionEnabled
        }
        onVestibularMotionRadiusChanged:{
            redirectedWalkingRadiusText.text =  ((RotationTabController.vestibularMotionRadius).toFixed(2))
        }
        onViewRatchettingEnabledChanged:{
            viewRatchetToggleBtn.checked = RotationTabController.viewRatchettingEnabled
        }
        onViewRatchettingPercentChanged:{
            viewRatchetText.text = ((RotationTabController.viewRatchettingPercent).toFixed(2))
        }
    }

}
