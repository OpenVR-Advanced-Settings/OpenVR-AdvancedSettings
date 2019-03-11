import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import ovras.advsettings 1.0
import "../../common"

MyDialogOkPopup {
    id: audioMessageDialog
    function showMessage(title, text) {
        dialogTitle = title
        dialogText = text
        open()
    }
}
