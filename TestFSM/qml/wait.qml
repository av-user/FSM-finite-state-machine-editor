import QtQuick 2.3
import TranslationUtil 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
//import QtGraphicalEffects 1.0
import "qrc:/singletons/singletons/"

Item {
    id: root
    width: Constants.windowWidth
    height: Constants.windowHeight - Constants.headerheight

    BusyIndicator {
        id: busy
        anchors.centerIn: parent
        running: image.status === Image.Loading
    }
}
