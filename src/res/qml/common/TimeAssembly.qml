import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0

RowLayout {
    signal timeChanged(int hour, int minute, int second)

    HourComboBox {
        id: hourBox
        Layout.preferredWidth: 60
        onActivated: {
            parent.timeChanged(hourBox.currentIndex,
                               minuteBox.currentIndex,
                               secondBox.currentIndex)
        }
    }
    MyText {
        text: "h"
    }

    MinuteSecondComboBox {
        id: minuteBox
        Layout.preferredWidth: 60
        onActivated: {
            parent.timeChanged(hourBox.currentIndex,
                               minuteBox.currentIndex,
                               secondBox.currentIndex)
        }
    }
    MyText {
        text: "m"
    }

    MinuteSecondComboBox {
        id: secondBox
        Layout.preferredWidth: 60
        onActivated: {
            parent.timeChanged(hourBox.currentIndex,
                               minuteBox.currentIndex,
                               secondBox.currentIndex)
        }
    }
    MyText {
        text: "s"
    }
    function changeTimer(hour, minute, second) {
        hourBox.currentIndex = hour
        minuteBox.currentIndex = minute
        secondBox.currentIndex = second
        hourBox.displayText = hour
        minuteBox.displayText = minute
        secondBox.displayText = second
    }
}
