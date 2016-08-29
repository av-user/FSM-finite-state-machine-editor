import QtQuick 2.0
import TranslationUtil 1.0
import "qrc:/singletons/singletons/"

Item {
    id: root;
    signal clicked()

    property alias text: text.text

    Rectangle {
        id: rect
        property string colorb: "gray"
        property double widthb: 1.0
        width: root.width
        height: root.height
        border.color: rect.colorb
        border.width: rect.widthb
        color: "white"
    }
    Text {
        id: text
        color: "#FFDF0029"
        anchors.centerIn: parent
        font.pointSize: Constants.fontSize1
        font.family: Constants.fontFamily
        text: label
    }
    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: {
            root.clicked()
        }
    }

}
