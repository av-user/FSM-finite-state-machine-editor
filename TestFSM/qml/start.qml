import QtQuick 2.3
import TranslationUtil 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import "qrc:/singletons/singletons/"

Rectangle {
    id: root

    signal signonclicked ()
    property bool connected: false

    width: Constants.windowWidth
    height: Constants.windowHeight - Constants.headerheight
//    color: Constants.bg_color;
    gradient: Gradient {
        GradientStop { position: 0 ; color: Constants.creamy_light }
        GradientStop { position: 1 ; color: Constants.creamy_dark }
    }


    Timer {
        interval: 1000; running: true; repeat: false
//        onTriggered: root.signonclicked()
    }

    Text {
        id: text
        color: Constants.creamytextcolor
        anchors.centerIn: parent
        font.pointSize: Constants.fontSize6
        font.family: Constants.fontFamily2
        text: (connected ? qsTr("Touch the screen to logon"):qsTr("Waiting for connection ...")) + TranslationUtil.emptyString
        transform: Scale { origin.x: text.width/2; origin.y: text.height/2; yScale: 3}
    }
    MouseArea {
        anchors.fill: parent
        onClicked: { if (connected) {root.signonclicked()} }
    }

//    Button {
//        id: startbutton
//        width: Constants.buttonWidth5
//        height: Constants.buttonHeight5
//        anchors.horizontalCenter: root.horizontalCenter
//        anchors.top: parent.top
//        anchors.topMargin: Constants.marginX5
//        enabled: root.connected
//        text: qsTr("Enter") + TranslationUtil.emptyString
//        style: ButtonStyle {
//            background: BtnStyleWhite {}
//            label: BtnStyleLabelKbd {}

//        }
//        Connections {
//            target: startbutton
//            onClicked: {
//                root.signonclicked()
//            }
//        }
//    }
//    DropShadow {
//         anchors.fill: startbutton
//         horizontalOffset: 3
//         verticalOffset: 3
//         samples: 7
//         color: "#80000000"
//         source: startbutton
//     }


}

