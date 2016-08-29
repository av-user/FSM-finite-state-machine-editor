import QtQuick 2.3
import QtQuick.Window 2.2
import "qrc:/singletons/singletons/"
import TranslationUtil 1.0

Item  {
    id: root
    visible: true

    property alias text: text.text
    property alias text2: text2.text

    Rectangle {
        id: rectlogo
//        color: "white"
        color: Constants.bg_color;
        width: Constants.logoWidth
        height: root.height
        Image {
            id: logo
            anchors.centerIn: parent
            source: "qrc:/images/images/DemoLogo.png"
            z: 2
        }
    }

    Row {
        id: row
        anchors.top: root.top
        anchors.topMargin: 50
        anchors.right: root.right
        anchors.rightMargin: 10

        z: 1
        spacing: 10
        LangButton {
            id: lang0
            Connections {
                target: lang0
                onClicked: {
                    var str = lang0.getFlag();
                    if (lang0.getFlag() === "en"){
                        TranslationUtil.setLanguage ("en")
                        lang0.setFlag(lang1.getFlag() === "it" ? "de" : "it")
                    } else if (lang0.getFlag() === "it"){
                        TranslationUtil.setLanguage ("it")
                        lang0.setFlag(lang1.getFlag() === "en" ? "de" : "en")
                    } else if (lang0.getFlag() === "de"){
                        TranslationUtil.setLanguage ("de")
                        lang0.setFlag(lang1.getFlag() === "it" ? "en" : "it")
                    }
                }
            }
            Component.onCompleted: {
                setFlag("de")
            }
        }
        LangButton {
            id: lang1
            Connections {
                target: lang1
                onClicked: {
                    if (lang1.getFlag() === "en"){
                        TranslationUtil.setLanguage ("en")
                        lang1.setFlag(lang0.getFlag() === "it" ? "de" : "it")
                    } else if (lang1.getFlag() === "it"){
                        TranslationUtil.setLanguage ("it")
                        lang1.setFlag(lang0.getFlag() === "en" ? "de" : "en")
                    } else if (lang1.getFlag() === "de"){
                        TranslationUtil.setLanguage ("de")
                        lang1.setFlag(lang0.getFlag() === "it" ? "en" : "it")
                    }
                }
            }
            Component.onCompleted: {
                setFlag("it")
            }
        }
    }



    Rectangle {
        id: recttext
//        color: "white"
        color: Constants.bg_color;
        width: Constants.windowWidth - Constants.logoWidth
        height: Constants.headerHeight
        anchors.left: rectlogo.right
//        gradient: Gradient {
//            GradientStop { position: 0 ; color: Constants.creamy_light }
//            GradientStop { position: 1 ; color: Constants.creamy_dark }
//        }
        Text {
            id: text
            color: Constants.creamytextcolor
            x: Constants.marginX1
            y: Constants.marginY1
            font.pointSize: Constants.fontSize2
            text: ""
        }
        Text {
            id: text2
            color: text.color
            x: Constants.marginX2
            anchors.top: text.bottom
            font.pointSize: Constants.fontSize0
            text: ""
        }
        Text {
            id: datetime
            color: text.color
            x: Constants.marginX2
            anchors.top: recttext.top
            anchors.topMargin: Constants.marginY0
            anchors.right: recttext.right
            anchors.rightMargin: Constants.marginX0
            font.pointSize: Constants.fontSize0
            font.family: Constants.fontFamily
            text: Qt.formatDateTime(new Date(),"dd.MM.yyyy hh:mm:ss")
        }
        Timer {
            id: timer
            interval: 1000
            repeat: true
            running: true
            onTriggered:{
                datetime.text =  Qt.formatDateTime(new Date(),"dd.MM.yyyy hh:mm:ss")
            }
        }

        MouseArea {
            id: mouse
            anchors.fill: recttext
            onClicked: Qt.quit();
        }
    }
}
