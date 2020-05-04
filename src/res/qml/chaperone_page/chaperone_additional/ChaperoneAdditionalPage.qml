import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import ovras.advsettings 1.0
import "." // QTBUG-34418, singletons require explicit import to load qmldir file
import "../../common"
import "chaperoneboundscolor"

MyStackViewPage {
    headerText: "Additional Chaperone Settings"
    content: ColumnLayout {
        spacing: 10

        ChaperoneBoundsColorGroupBox{
        id: chaperoneBoundsColorGroupBox
        }
        Item {
            Layout.fillHeight: true
        }

    }


}



    //TODO content

