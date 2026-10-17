import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
// Necessary for the project specific Components.
import ".."

// Insert inside a RowLayout to separate settings sharing a row.
// Drawn in the page background color so it reads as a gap between settings.
Rectangle {
    color: "#1d2026"
    width: 2
    Layout.preferredWidth: 2
    Layout.fillHeight: true
    // Extend through the row's 4px gaps so it meets the LineSeparators.
    Layout.topMargin: -4
    Layout.bottomMargin: -4
}
