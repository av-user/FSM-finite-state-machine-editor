import QtQuick 2.3
import TranslationUtil 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import TranslationUtil 1.0
import "qrc:/singletons/singletons/"

Rectangle {
    id: root

    property string text: ""


    signal showdone ()

    width: Constants.windowWidth
    height: Constants.windowHeight - Constants.headerheight
    gradient: Gradient {
        GradientStop { position: 0 ; color: Constants.creamy_light }
        GradientStop { position: 1 ; color: Constants.creamy_dark }
    }

    Text {
        id: text
        color: Constants.creamytextcolor
        anchors.centerIn: parent
        font.pointSize: Constants.fontSize2
        text: root.text
    }

    Button {
        id: enterbutton
        width: Constants.buttonWidthStd
        height: Constants.buttonHeightStd
        anchors.left: parent.left
        anchors.leftMargin: Constants.marginLeftStd
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Constants.marginBottomStd
        text: qsTr("Ok") + TranslationUtil.emptyString
        style: ButtonStyle {
            background: BtnStyleWhite {}
            label: BtnStyleLabelKbd {}
        }
        Connections {
            target: enterbutton
            onClicked: {
                root.showdone()
            }
        }
    }

    Timer {
        interval: 10000; running: true; repeat: false
        onTriggered: root.showdone()
    }


}

