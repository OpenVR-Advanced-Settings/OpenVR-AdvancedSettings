import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
// Necessary for the project specific Components.
import ".."

// Insert inside a ColumnLayout containing this and a RowLayout
// to neatly separate header and buttons.
// Drawn in the page background color so it reads as a gap between setting rows.
Rectangle {
    color: "#1d2026"
    height: 2
    Layout.fillWidth: true
    Layout.leftMargin: -16
    Layout.rightMargin: -16
    Layout.bottomMargin: 4
    Layout.topMargin: 4
}
