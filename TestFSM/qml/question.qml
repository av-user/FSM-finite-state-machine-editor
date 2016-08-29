import QtQuick 2.3
import TranslationUtil 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "qrc:/singletons/singletons/"

Rectangle {
    id: root

    signal question (string str)
    signal canceled ()

    width: Constants.windowWidth
    height: Constants.windowHeight - Constants.headerheight

    gradient: Gradient {
        GradientStop { position: 0 ; color: Constants.creamy_light }
        GradientStop { position: 1 ; color: Constants.creamy_dark }
    }
    Item {
        id: priv
        property int btn_width: 600
        property int btn_height: 60
    }
    Column {
        id: questions
        spacing: 10
        anchors.left: parent.left
        anchors.leftMargin: Constants.marginLeftStd
        y: Constants.marginY1
        property int selectedIdx: 0
        readonly property variant values: [qsTr("Question one")
            , qsTr("Question two")
            , qsTr("Question three")
            , qsTr("Question four")
            , qsTr("Question five")
            , qsTr("Question six")
            , qsTr("Question seven")
        ]
        Repeater {
            id: questions_repeater
            model: questions.values
            QuestionButton {
                id: question_btn
                text: modelData
                width: priv.btn_width
                height: priv.btn_height
                onClicked: {
                    questions.setQuestion(index)
                }
            }
        }
        function setQuestion (idx){
            root.question (questions.values[idx])
        }
    }


    Button {
        id: cancelbutton
        width: Constants.buttonWidthStd
        height: Constants.buttonHeightStd
        anchors.left: parent.left
        anchors.leftMargin: Constants.marginLeftStd
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Constants.marginBottomStd
        text: qsTr("Cancel") + TranslationUtil.emptyString
        style: ButtonStyle {
            background: BtnStyleWhite {}
            label: BtnStyleLabelKbd {}
        }
        Connections {
            target: cancelbutton
            onClicked: {
                root.canceled()
            }
        }
    }
}

