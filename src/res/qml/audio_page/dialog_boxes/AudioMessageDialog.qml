import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import matzman666.advsettings 1.0
import "../.." //common imports

MyDialogOkPopup {
    id: audioMessageDialog
    function showMessage(title, text) {
        dialogTitle = title
        dialogText = text
        open()
    }
}
