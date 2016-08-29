import QtQuick 2.0
import "qrc:/singletons/singletons/"

Rectangle {
    id: root;
    signal clicked()
    width: Constants.flagW
    height: Constants.flagH
    border.width: 1
    border.color: "gray"

    function getFlag (){
        return priv.lang
    }
    function setFlag (fl){
        if (fl != priv.lang){
            if (fl === "en"){
                flag.source = "qrc:/images/images/FlagEn0.png"
                priv.lang = fl
            } else if (fl === "it"){
                flag.source = "qrc:/images/images/FlagIt0.png"
                priv.lang = fl
            } else if (fl === "de"){
                flag.source = "qrc:/images/images/FlagDe0.png"
                priv.lang = fl
            } else {
                flag.source = ""
                priv.lang = priv.nolang
            }
        }
    }
    QtObject {
        id: priv
        readonly property string nolang: "undefined"
        property string lang: nolang
    }
    Image {
        id: flag
        anchors.centerIn: parent
    }
    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: root.clicked();
    }
}
