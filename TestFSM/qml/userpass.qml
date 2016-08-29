import QtQuick 2.3
import TranslationUtil 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import TranslationUtil 1.0
import "qrc:/singletons/singletons/"

Rectangle {
    id: root

    signal userpassentered (string user, string pass)
    signal userpasscancelled ()

    width: Constants.windowWidth
    height: Constants.windowHeight - Constants.headerheight
    gradient: Gradient {
        GradientStop { position: 0 ; color: Constants.creamy_light }
        GradientStop { position: 1 ; color: Constants.creamy_dark }
    }

    function isvalid(){
        return user.text.length == user.maximumLength && pass.text.length == pass.maximumLength
    }
    Button {
        id: cancelbutton
        width: enterbutton.width
        height: enterbutton.height
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
                root.userpasscancelled()
            }
        }
    }
    DropShadow {
        anchors.fill: enterbutton
        horizontalOffset: 3
        verticalOffset: 3
        samples: 7
        color: "#80000000"
        source: enterbutton
    }

    Button {
        id: enterbutton
        width: Constants.buttonWidthStd
        height: Constants.buttonHeightStd
        enabled: isvalid()
        anchors.left: cancelbutton.left
        anchors.bottom: cancelbutton.top
        anchors.bottomMargin: Constants.marginY1
        text: qsTr("Enter") + TranslationUtil.emptyString
        style: ButtonStyle {
            background: BtnStyleWhite {}
            label: BtnStyleLabelKbd {}
        }
        Connections {
            target: enterbutton
            onClicked: {
                root.userpassentered(user.text, pass.text)
            }
        }
    }
    Timer {
        interval: 1000; running: true; repeat: false
//        onTriggered: root.userpasscancelled()
    }

    Button {
        id: testbutton
        width: Constants.buttonWidthStd
        height: Constants.buttonHeightStd
        anchors.left: enterbutton.right
        anchors.leftMargin: Constants.marginX3
        anchors.bottom: cancelbutton.bottom
        text: "Test"
        style: ButtonStyle {
            background: BtnStyleWhite {}
            label: BtnStyleLabelKbd {}
        }
        Connections {
            target: testbutton
            onClicked: {
                root.userpassentered("33333333", "22222222")
            }
        }
    }
    Rectangle {
        id: userrect
        anchors.top: parent.top
        anchors.topMargin: Constants.marginY3
        anchors.left: parent.left
        anchors.leftMargin: Constants.marginX2 * 5
        width: 200
        height: user.font.pixelSize + 10
        color: "white"
        border.color: Constants.creamytextcolor
        property alias text: user.text
        property alias input: user

        TextInput {
            id: user
            focus: true
            cursorVisible: activeFocus
            color: Constants.creamytextcolor
            selectionColor: "green"
            font.pixelSize: Constants.fontSize2
            font.bold: false
            inputMask: "99999999"
            text: ""
            cursorPosition: text.length
            anchors.fill: parent
            anchors.margins: 4
        }
    }
    Text {
        color: Constants.creamytextcolor
        x: Constants.marginX1
        anchors.top: userrect.top
        font.pointSize: Constants.fontSize2
        text: qsTr("Username:") + TranslationUtil.emptyString
    }
    Rectangle {
        id: passrect
        anchors.top: userrect.bottom
        anchors.topMargin: Constants.marginY3
        anchors.left: userrect.left
        width: 200
        height: pass.font.pixelSize + 10
        color: "white"
        border.color: Constants.creamytextcolor
        property alias text: pass.text
        property alias input: pass

        TextInput {
            id: pass
            focus: true
            cursorVisible: activeFocus
            color: Constants.creamytextcolor
            selectionColor: "green"
            font.pixelSize: Constants.fontSize2
            font.bold: false
            maximumLength: 8
            echoMode: TextInput.Password
            text: ""
            cursorPosition: text.length
            anchors.fill: parent
            anchors.margins: 4
        }
    }
    Text {
        color: Constants.creamytextcolor
        x: Constants.marginX1
        anchors.top: passrect.top
        font.pointSize: Constants.fontSize2
        text: qsTr("Password:") + TranslationUtil.emptyString
    }
    KeyboardNum {
        id: kbd
        anchors.left: userrect.right
        anchors.leftMargin: Constants.marginX2
        anchors.top: userrect.top
    }
    function cursorLeft(input){
        if (input.cursorPosition > 0){
            input.cursorPosition --
        }
    }
    function cursorRight(input){
        if (input.cursorPosition < input.text.length){
            input.cursorPosition ++
        }
    }
    function backspace(input){
        if (input.cursorPosition > 0){
            var pos = input.cursorPosition
            input.text = input.text.substring(0, input.cursorPosition-1)+input.text.substring(input.cursorPosition)
            input.cursorPosition = pos - 1
        }
    }
    function key(input, str){
        if (input.text.length < input.maximumLength){
            var pos = input.cursorPosition
            input.text = input.text.substring(0, input.cursorPosition)+str+input.text.substring(input.cursorPosition)
            input.cursorPosition = pos + 1
            if (input.text.length == input.maximumLength){
                switchfocus()
            }
        }
    }
    function switchfocus(){
        if (user.activeFocus){
            pass.forceActiveFocus()
        } else {
            user.forceActiveFocus()
        }
    }

    function enter(input){
        switchfocus()
    }
    Component.onCompleted: {
        user.forceActiveFocus()
    }

    Connections {
        target: kbd
        onKeyClicked: {
            switch (str){
            case "Backspace":
                backspace(user.activeFocus ? user : pass)
                break;
            case "Enter":
                enter(user.activeFocus ? user : pass)
                break;
            case "Left":
                cursorLeft(user.activeFocus ? user : pass)
                break;
            case "Right":
                cursorRight(user.activeFocus ? user : pass)
                break;
            default:
                key(user.activeFocus ? user : pass, str)
            }
        }
    }
}

