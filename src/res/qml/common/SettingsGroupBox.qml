import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0

// Steam style settings section: bold section title above a flat panel.
GroupBox {
    id: settingsGroupBox
    Layout.fillWidth: true

    // Height of each setting row, set by the page so every row on it matches.
    // -1 leaves rows at their implicit height.
    property real rowHeight: -1

    // With fixed rows the panel edge acts like a LineSeparator, so the first and
    // last rows get the same 4px gap as rows between separators.
    readonly property real edgePadding: rowHeight >= 0 ? 4 : 14

    padding: 16
    topPadding: (title.length > 0 ? groupBoxLabel.implicitHeight + 8 : 0) + edgePadding
    bottomPadding: edgePadding

    label: MyText {
        id: groupBoxLabel
        x: 2
        y: 0
        width: settingsGroupBox.width - 4
        text: settingsGroupBox.title
        visible: settingsGroupBox.title.length > 0
        font.pointSize: 15
        font.bold: true
        color: "#ffffff"
        elide: Text.ElideRight
    }

    background: Rectangle {
        y: settingsGroupBox.title.length > 0 ? groupBoxLabel.implicitHeight + 8 : 0
        width: parent.width
        height: parent.height - y
        color: "#2a2e36"
        radius: 3
    }
}
