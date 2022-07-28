import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0

MyComboBox {

    model: [
        { value: 0, text: "00" },
        { value: 1, text: "01" },
        { value: 2, text: "02" },
        { value: 3, text: "03" },
        { value: 4, text: "04" },
        { value: 5, text: "05" },
        { value: 6, text: "06" },
        { value: 7, text: "07" },
        { value: 8, text: "08" },
        { value: 9, text: "09" },
        { value: 10, text: "10" },
        { value: 11, text: "11" },
        { value: 12, text: "12" },
        { value: 13, text: "13" },
        { value: 14, text: "14" },
        { value: 15, text: "15" },
        { value: 16, text: "16" },
        { value: 17, text: "17" },
        { value: 18, text: "18" },
        { value: 19, text: "19" },
        { value: 20, text: "20" },
        { value: 21, text: "21" },
        { value: 22, text: "22" },
        { value: 23, text: "23" },
        { value: 24, text: "24" },
        { value: 25, text: "25" },
        { value: 26, text: "26" },
        { value: 27, text: "27" },
        { value: 28, text: "28" },
        { value: 29, text: "29" },
        { value: 30, text: "30" },
        { value: 31, text: "31" },
        { value: 32, text: "32" },
        { value: 33, text: "33" },
        { value: 34, text: "34" },
        { value: 35, text: "35" },
        { value: 36, text: "36" },
        { value: 37, text: "37" },
        { value: 38, text: "38" },
        { value: 39, text: "39" },
        { value: 40, text: "40" },
        { value: 41, text: "41" },
        { value: 42, text: "42" },
        { value: 43, text: "43" },
        { value: 44, text: "44" },
        { value: 45, text: "45" },
        { value: 46, text: "46" },
        { value: 47, text: "47" },
        { value: 48, text: "48" },
        { value: 49, text: "49" },
        { value: 50, text: "50" },
        { value: 51, text: "51" },
        { value: 52, text: "52" },
        { value: 53, text: "53" },
        { value: 54, text: "54" },
        { value: 55, text: "55" },
        { value: 56, text: "56" },
        { value: 57, text: "57" },
        { value: 58, text: "58" },
        { value: 59, text: "59" }
    ]

    delegate: ItemDelegate {
        width: parent.width
        text: modelData.text
        hoverEnabled: true
        contentItem: MyText {
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text: parent.text
            color: parent.enabled ? "#d9dbe0" : "#65676b"
        }
        background: Rectangle {
            color: parent.pressed ? "#282b31" : (parent.hovered ? "#484f5b" : "#2a2e35")
        }
    }
}
