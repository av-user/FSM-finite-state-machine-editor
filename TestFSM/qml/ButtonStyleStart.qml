import QtQuick 2.0
import QtGraphicalEffects 1.0
import TranslationUtil 1.0

Item {
    property alias fontSize: text.fontSize
    property alias fontFamily: text.fontFamily
    Rectangle {
        anchors.fill: parent
        radius: 20
        gradient: Gradient {
            GradientStop { position: 0 ; color: control.pressed ? "#eee" : "#ffbfbfbf" }
            GradientStop { position: 1 ; color: control.pressed ? "#ccc" : "#ffb9b9b9" }
        }
    }
    Rectangle {
        anchors.centerIn: parent
        border.width: 2
        border.color: "#ffd2d2d2"

        width: parent.width - 10
        height: parent.height - 10
        radius: 16
        gradient: Gradient {
            GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#fffafbfc" }
            GradientStop { position: 1 ; color: control.pressed ? "#eee" : "#ffa9a9a9" }
        }
    }
    Text {
        id: text
        property int fontSize: 20
        property string fontFamily: "Helvetica"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: control.pressed ? 1 : 0
        anchors.verticalCenter: parent.verticalCenter;
        anchors.verticalCenterOffset: control.pressed ? 1 : 0
        font.pointSize: fontSize
        font.family: fontFamily
        visible: false
        text: qsTr("Sign on") + TranslationUtil.emptyString
    }
    LinearGradient  {
        anchors.fill: text
        source: text
        gradient: Gradient {
//            GradientStop { position: 0; color: "#ff757575" }
//            GradientStop { position: 1; color: "#ff989898" }
            GradientStop { position: 0; color: "black" }
            GradientStop { position: 1; color: "grey" }
        }
    }

}
