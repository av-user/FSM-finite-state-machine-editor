import QtQuick 2.3
import TranslationUtil 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "qrc:/singletons/singletons/"

Rectangle {
    id: root

    signal ask ()
    signal buy ()
    signal logoff ()

    width: Constants.windowWidth
    height: Constants.windowHeight - Constants.headerheight
    gradient: Gradient {
        GradientStop { position: 0 ; color: Constants.creamy_light }
        GradientStop { position: 1 ; color: Constants.creamy_dark }
    }

    Button {
        id: questionbutton
        width: Constants.buttonWidth5
        height: Constants.buttonHeight3
        anchors.right: parent.right
        anchors.rightMargin: Constants.marginX3
        anchors.top: parent.top
        anchors.topMargin: Constants.marginY3
        text: qsTr("Ask a question") + TranslationUtil.emptyString
        style: ButtonStyle {
            background: BtnStyleWhite {}
            label: BtnStyleLabelKbd {}
        }
        Connections {
            target: questionbutton
            onClicked: {
                root.ask()
            }
        }
    }
    Button {
        id: buybutton
        width: questionbutton.width
        height: questionbutton.height
        anchors.left: questionbutton.left
        anchors.top: questionbutton.bottom
        anchors.topMargin: Constants.marginY3
        text: qsTr("Buy something") + TranslationUtil.emptyString
        style: ButtonStyle {
            background: BtnStyleWhite {}
            label: BtnStyleLabelKbd {}
        }
        Connections {
            target: buybutton
            onClicked: {
                root.buy()
            }
        }
    }
    Button {
        id: logoutbutton
        width: Constants.buttonWidthStd
        height: Constants.buttonHeightStd
        anchors.left: parent.left
        anchors.leftMargin: Constants.marginLeftStd
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Constants.marginBottomStd
        text: qsTr("Logout") + TranslationUtil.emptyString
        style: ButtonStyle {
            background: BtnStyleWhite {}
            label: BtnStyleLabelKbd {}
        }
        Connections {
            target: logoutbutton
            onClicked: {
                root.logoff()
            }
        }
    }
}

