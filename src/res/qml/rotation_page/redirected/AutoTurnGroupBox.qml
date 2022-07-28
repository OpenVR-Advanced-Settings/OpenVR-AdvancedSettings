import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

GroupBox {
    id: autoTurnGroupBox
    Layout.fillWidth: true

    label: MyText {
        leftPadding: 10
        text: "Auto-Turn (for discrete redirected walking)"
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

        RowLayout {
            Layout.fillWidth: true

            MyToggleButton {
                id: autoTurn
                text: "Toggle On/Off"
                Layout.preferredWidth: 250
                onCheckedChanged: {
                    RotationTabController.setAutoTurnEnabled(this.checked, true);                }
            }
            MyToggleButton {
                id: autoTurnNotificationToggle
                text: "HMD Icon"
                Layout.preferredWidth: 175
                onCheckedChanged: {
                RotationTabController.setAutoTurnShowNotification(this.checked, true)
                }
            }

            MyText {
                text: "Activation Dist:"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
                Layout.preferredWidth: 215
            }

            MySlider {
                id: activationSlider
                from: 0
                to: 1.0
                stepSize: 0.05
                value: .4
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = this.value
                    activationValueText.text = val.toFixed(2)
                }
                onValueChanged: {

                    RotationTabController.setAutoTurnActivationDistance(value, true)
                }
            }

            MyTextField {
                id: activationValueText
                text: "0.4"
                keyBoardUID: 1001
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0) {
                            val = 0
                        } else if (val > 22) {
                            val = 22
                        }
                            activationSlider.value = val.toFixed(2);
                    }
                    text =  RotationTabController.autoTurnActivationDistance.toFixed(2);
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true

            MyToggleButton {
                id: cornerAngle
                text: "Use Corner Angle"
                Layout.preferredWidth: 300
                onCheckedChanged: {
                    RotationTabController.setAutoTurnUseCornerAngle(this.checked, true);
                }
            }

            MyText {
                text: "Deactivation Dist:"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
                Layout.preferredWidth: 215
            }

            MySlider {
                id: deactivationSlider
                from: 0
                to: 1.0
                stepSize: 0.05
                value: .15
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = this.value
                    deactivationValueText.text = val.toFixed(2)
                }
                onValueChanged: {

                    RotationTabController.setAutoTurnDeactivationDistance(value, true)
                }
            }

            MyTextField {
                id: deactivationValueText
                text: "0.15"
                keyBoardUID: 1002
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 0) {
                            val = 0
                        } else if (val > 22) {
                            val = 22
                        }
                            deactivationSlider.value = val.toFixed(2);
                    }
                    text =  RotationTabController.autoTurnDeactivationDistance.toFixed(2);
                }
            }
        }

        RowLayout {
            MyToggleButton {
                id: autoTurnModeToggle
                text: "Use Smooth Turn"
                Layout.preferredWidth: 300
                onCheckedChanged: {
                    if(this.checked){

                        RotationTabController.setAutoTurnMode(1, true);
                    }else{
                        RotationTabController.setAutoTurnMode(0, true);
                    }

                }
            }

            MyText {
                text: "Turn Speed(deg/sec):"
                horizontalAlignment: Text.AlignRight
                Layout.rightMargin: 10
            }

            MySlider {
                id: speedSlider
                from: 0
                to: 900
                stepSize: 10
                value: 90
                Layout.fillWidth: true
                onPositionChanged: {
                    var val = this.value
                    speedValueText.text = val.toFixed()+ "°"
                }
                onValueChanged: {

                    RotationTabController.setAutoTurnSpeed(parseInt(value*100), true)
                }
            }

            MyTextField {
                id: speedValueText
                text: "90°"
                keyBoardUID: 1003
                Layout.preferredWidth: 100
                Layout.leftMargin: 10
                horizontalAlignment: Text.AlignHCenter
                function onInputEvent(input) {
                    var val = parseFloat(input)
                    if (!isNaN(val)) {
                        if (val < 1) {
                            val = 1
                        //caps user set Speed to 1000 deg/Second
                        //used as overflow prevention
                        //arbitrary
                        } else if (val > 1000) {
                            val = 1000
                        }
                            speedSlider.value = val;
                    }
                    //converts the centidegrees to degrees
                    text =  (((RotationTabController.autoTurnSpeed)/100).toFixed()) + "°";
                }
            }


        }
      RowLayout{

          MyText{
              text: "Detangle Angle: "
              horizontalAlignment: Text.AlignLeft
              Layout.preferredWidth: 200
              Layout.rightMargin: 5

          }
          MyText{
              text:"Min Rotations:"
              Layout.rightMargin: 5

          }

          MyTextField {
              id: detangleAngleStartText
              text: "360°"
              keyBoardUID: 1005
              Layout.preferredWidth: 100
              Layout.leftMargin: 5
              horizontalAlignment: Text.AlignHCenter
              function onInputEvent(input) {
                  var val = parseFloat(input)
                  if (!isNaN(val)) {
                      if (val < 0) {
                          val = 0
                      } else if (val > 3600) {
                          val = 3600
                      }
                  RotationTabController.setMinCordTangle((val*(Math.PI/180)), true);
                  text = (Math.round(val).toFixed())+ "°";
                  }
              }
          }
          Item {
              Layout.preferredWidth: 15
          }

          MyText{
              text: "Max Wall Angle: "
              horizontalAlignment: Text.AlignRight
              Layout.rightMargin: 10
              Layout.fillWidth: true

          }
          MyTextField {
              id: detangleAngleAssistText
              text: "10°"
              keyBoardUID: 1006
              Layout.preferredWidth: 100
              Layout.leftMargin: 10
              horizontalAlignment: Text.AlignHCenter
              function onInputEvent(input) {
                  var val = parseFloat(input)
                  if (!isNaN(val)) {
                      if (val < 0) {
                          val = 0
                      } else if (val > 3600) {
                          val = 3600
                      }
                  RotationTabController.setCordDetangleAngle((val*(Math.PI/180)), true);
                  text = (Math.round(val).toFixed())+ "°";
                  }
              }
          }
      }




    }

    Component.onCompleted: {
        activationSlider.value = RotationTabController.autoTurnActivationDistance.toFixed(2)
        autoTurn.checked = RotationTabController.autoTurnEnabled
        deactivationSlider.value = RotationTabController.autoTurnDeactivationDistance.toFixed(2)
        cornerAngle.checked = RotationTabController.autoTurnUseCornerAngle
        speedSlider.value = (RotationTabController.autoTurnSpeed/100).toFixed()
        autoTurnNotificationToggle.checked = RotationTabController.autoTurnShowNotification
        if(RotationTabController.autoTurnMode === 1){

            autoTurnModeToggle.checked = true;

        }else{

             autoTurnModeToggle.checked = false;
        }
        detangleAngleStartText.text = parseInt(RotationTabController.minCordTangle*(180/Math.PI))+ "°"
        detangleAngleAssistText.text = parseInt(RotationTabController.cordDetangleAngle*(180/Math.PI))+ "°"
    }

    Connections {
        target: RotationTabController

        onAutoTurnEnabledChanged: {
            autoTurn.checked = RotationTabController.autoTurnEnabled
        }
        onAutoTurnActivationDistanceChanged: {
            activationSlider.value = RotationTabController.autoTurnActivationDistance.toFixed(2)
        }
        onAutoTurnDeactivationDistanceChanged:{
            deactivationSlider.value = RotationTabController.autoTurnDeactivationDistance.toFixed(2)
        }
        onAutoTurnUseCornerAngleChanged:{
            cornerAngle.checked = RotationTabController.autoTurnUseCornerAngle
        }
        onAutoTurnSpeedChanged:{
            var val = RotationTabController.autoTurnSpeed
            speedSlider.value = ((val/100).toFixed())
        }
        onAutoTurnModeChanged:{
            if(RotationTabController.autoTurnMode === 1){

                autoTurnModeToggle.checked = true;

            }else{

                 autoTurnModeToggle.checked = false;
            }
        }
        onMinCordTangleChanged:{
            detangleAngleStartText.text = parseInt(RotationTabController.minCordTangle*(180/Math.PI))+ "°"
        }
        onCordDetangleAngleChanged:{
            detangleAngleAssistText.text = parseInt(RotationTabController.cordDetangleAngle*(180/Math.PI))+ "°"
        }
        onAutoTurnShowNotificationChanged:{
            autoTurnNotificationToggle.checked = RotationTabController.autoTurnShowNotification
        }
    }
}
