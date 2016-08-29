import QtQuick 2.3
import QtQuick.Controls 1.2
import "qrc:/singletons/singletons/"
import TranslationUtil 1.0

ApplicationWindow {
    id: root
    visible: true
    width: Constants.windowWidth
    height: Constants.windowHeight
    flags: Qt.FramelessWindowHint
    title: qsTr("SBDevice") + TranslationUtil.emptyString
//    color: Constants.bg_color;

    signal tocpp(string str)
    property int count: 0
    property string args: ""

    Item {
        id: properties
        property bool connected: false
    }

    Item {
        id: stateitem
        state: ""
        states: [
            State {
                name: "START"
                PropertyChanges {
                    target: loader; source: "qrc:/qml/start.qml"
                }
                PropertyChanges {
                    target: header
                    text: qsTr("Welcome!") + TranslationUtil.emptyString
                    text2: ""
                }
            },
            State {
                name: "USERPASS"
                PropertyChanges { target: loader; source: "qrc:/qml/userpass.qml" }
                PropertyChanges {
                    target: header
                    text: qsTr("Please enter your login data") + TranslationUtil.emptyString
                    text2: ""
                }
            },
            State {
                name: "ASK"
                PropertyChanges { target: loader; source: "qrc:/qml/question.qml" }
//                PropertyChanges { target: loader.item; content: "001:Mercury:mercury.jpeg:Mercury is the smallest planet in the Solar System. It is the closest planet to the sun. It makes one trip around the Sun once every 87.969 days." }
                PropertyChanges {
                    target: header
                    text: qsTr("Select your question") + TranslationUtil.emptyString
                    text2: ""
                }
            },
            State {
                name: "BUY"
                PropertyChanges { target: loader; source: "qrc:/qml/buy.qml" }
                PropertyChanges {
                    target: header
                    text: qsTr("Select your product") + TranslationUtil.emptyString
                    text2: ""
                }
            },
            State {
                name: "WAIT"
                PropertyChanges { target: loader; source: "qrc:/qml/wait.qml"}
                StateChangeScript {
                      name: "waitScript"
                      script: {

                      }
                  }
            },
            State {
                name: "SIGNED"
                PropertyChanges { target: loader; source: "qrc:/qml/signed.qml"}
            },
            State {
                name: "SHOW"
                PropertyChanges { target: loader; source: "qrc:/qml/show.qml"}
                StateChangeScript {
                    name: "showScript"
                    script: {
                      loader.item.text = root.args
                    }
                }
            },
            State {
                name: "LOCKED"
                PropertyChanges { target: loader; source: "qrc:/qml/locked.qml"}
            }
        ]
    }

    Component.onCompleted: {
        setX (20)
        setY (30)
        stateitem.state = "START"
    }
    Header{
        id: header
        z: 3
        width: root.width
        height: Constants.headerHeight
    }

    Loader {
        id: loader
        anchors.top: header.bottom
        width: root.width
        height: root.height - Constants.headerHeight
        onLoaded: {
            if (stateitem.state == "START"){
                item.connected = properties.connected
            } else if (stateitem.state == "BUY"){
                var str = "005:Multitool:multitool.png:Multi Tool Plier 13 in 1 - High quality, Strong Pocket plier for indoor and outdoor works"
                str += "<br>006:Shoe:shoe.png:Etnies Men's Jameson Vulc Athletic Shoe:\nLeather and Textile\nRubber sole\nLower toe box\nSTI Foam Lite 1 insole\nVulcanized outsole\nRCT tread pattern"
                str += "<br>007:Book:book.png:Oxford Advanced Learner's Dictionary, 8th edition\n(Oxford Advanced Learner's Dictionary)\nHighlight, take notes, and search in the book\nLength: 1796 pages"
                str += "<br>008:Chair:chair.png:Lifetime 60064 Adirondack Chair\nConstructed of uv-protected simulated wood\nUv-protected will not crack, chip or peel\nQuick-easy assembly\nStain Resistant and Easy to Clean\nStainless steel screws"
                item.txt = str
            }
        }
    }

    Connections {
        target: loader.item
        onSignonclicked:{
            root.tocpp ("signonclicked")
        }
        onUserpassentered: {
            stateitem.state = "WAIT"
            root.tocpp ("userpass:" + user + ";" + pass)
        }
        onUserpasscancelled: {
            stateitem.state = "WAIT"
            root.tocpp ("cancelled")
        }
        onLogoff: {
            stateitem.state = "WAIT"
            root.tocpp ("logoff")
        }
        onAsk: {
            stateitem.state = "ASK"
        }
        onQuestion: {
            stateitem.state = "WAIT"
            root.tocpp ("question:"+str)
        }

        onBuy: {
            stateitem.state = "BUY"
        }
        onBuyorder: {
            stateitem.state = "WAIT"
            root.tocpp ("buy:" + prodID)
        }

        onCanceled: {
            stateitem.state = "SIGNED"
        }

        onShowdone: {
            stateitem.state = "WAIT"
            root.tocpp ("showdone")
        }
    }

    function sendToQml(text){
        var cmd
        var args = ""
        var pos = text.indexOf(':')
        if (pos === -1){
            cmd = text
        } else {
            cmd = text.substr (0, pos)
            args = text.substr (pos+1, text.length - pos - 1)
        }

        console.log("sendToQml: " + text)
        textField1.text = text + " " + count
        switch (cmd){
        case "userpass":
            stateitem.state = "USERPASS"
            count ++
            break
        case "logged off":
            stateitem.state = "START"
            break
        case "start":
            stateitem.state = "START"
            break
        case "comm disconnected":
            properties.connected = false;
            if (stateitem.state = "START"){
                loader.item.connected = false
            } else {
                stateitem.state = "START"
            }
            break
        case "login successful":
            stateitem.state = "SIGNED"
            break
        case "mainmenu":
            stateitem.state = "SIGNED"
            break
        case "show":
            root.args = args
            stateitem.state = "SHOW"
            break
        case "comm connected":
            properties.connected = true;
            if (stateitem.state = "START"){
                loader.item.connected = true
            }
            break
        default:
            //todo?
            break;
        }
    }

    Text {
        id: textField1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        x: 10
        color: "lightblue"
//        placeholderText: qsTr("Some text...") + TranslationUtil.emptyString
    }

//    MouseArea {
//      anchors.fill: parent
//      onClicked: {
//        root.tocpp("blablabla");
//      }
//    }


}
