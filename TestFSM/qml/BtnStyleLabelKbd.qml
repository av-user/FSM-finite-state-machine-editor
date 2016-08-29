import QtQuick 2.0
import QtGraphicalEffects 1.0
import "qrc:/singletons/singletons/"

Item {
    id: root

    Text {
        id: txt
        renderType: Text.NativeRendering
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: control.pressed ? 1 : 0
        anchors.verticalCenter: parent.verticalCenter;
        anchors.verticalCenterOffset: control.pressed ? 1 : 0
        font.family: "Helvetica"
        font.pointSize: 20
        color: Constants.creamytextcolor
        text: control.text
    }

}
