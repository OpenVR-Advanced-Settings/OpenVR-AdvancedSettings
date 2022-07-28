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
        bottomPadding: -12
    }
    background: Rectangle {
        color: "transparent"
        border.color: "#d9dbe0"
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            color: "#d9dbe0"
            height: 1
            Layout.fillWidth: true
            Layout.bottomMargin: 3
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


    Component.onCompleted: {
        redirectedWalkingRadiusText.text = ((RotationTabController.vestibularMotionRadius).toFixed(2))
        redirectedModeToggle.checked = RotationTabController.vestibularMotionEnabled
    }

    Connections {
        target: RotationTabController

        onVestibularMotionEnabledChanged:{
         redirectedModeToggle.checked = RotationTabController.vestibularMotionEnabled
        }
        onVestibularMotionRadiusChanged:{
            redirectedWalkingRadiusText.text =  ((RotationTabController.vestibularMotionRadius).toFixed(2))
        }
    }

}
