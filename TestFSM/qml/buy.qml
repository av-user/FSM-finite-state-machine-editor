import QtQuick 2.0
import TranslationUtil 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "qrc:/singletons/singletons/"

Item {
    id: root
    width: parent.width
    height: parent.height

    signal buyorder (string prodID)
    signal canceled ()
    signal buyLoaded ()

    property string txt: ""
    function setProd(prod){
        var pos = prod.indexOf(':')
        var retval = true
        var prodid
        var prodname
        var prodimagename
        var prodtext
        if (pos !== -1){
            prodid = prod.substr(0, pos)
            prod = prod.substr (pos+1, prod.length - pos - 1)
            pos = prod.indexOf(':')
            if (pos !== -1){
                prodname = prod.substr(0, pos)
                prod = prod.substr (pos+1, prod.length - pos - 1)
                pos = prod.indexOf(':')
                if (pos !== -1){
                    prodimagename = prod.substr(0, pos)
                    prodtext = prod.substr (pos+1, prod.length - pos - 1)
                    products.append({"productID": prodid, "name": prodname, "imageSource": installPath + "images/" + prodimagename, "producttext": prodtext});
                } else {
                    retval = false
                }
            } else {
                retval = false
            }
        } else {
            retval = false
        }
        return retval
    }

    function setCont(){
        var str = root.txt
        var prod
        var pos = str.indexOf("<br>")
        var sizeofLB = 4
        while (pos !== -1) {
            prod = str.substr(0, pos)
            str = str.substr (pos+sizeofLB, str.length - pos - sizeofLB)
            if (!setProd(prod)){
                str = ""
                break
            }
            pos = str.indexOf("<br>")
        }
        setProd(str)
    }

    onTxtChanged: {
        setCont()
    }

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0 ; color: Constants.creamy_light }
            GradientStop { position: 1 ; color: Constants.creamy_dark }
        }
    }
    ListView {
        id: listView
        anchors.fill: parent
        delegate: detailsDelegate
        model: products
    }
    ListModel {
        id: products
    }
    Component {
        id: detailsDelegate
        Item {
            id: wrapper
            width: listView.width
            height: Constants.listViewHeight
            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: Constants.listViewHeight
                gradient: Gradient {
                    GradientStop { position: 0 ; color: Constants.creamy_light }
                    GradientStop { position: 1 ; color: Constants.creamy_dark }
                }
                border.color: Qt.lighter(color, 1.2)
                Text {
                    id: prodtext
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 4
                    font.pixelSize: parent.height-4
                    color: Constants.creamytextcolor
                    text: name
                }
            }
            Rectangle {
                id: image
                width: Constants.listViewHeight - 2
                height: Constants.listViewHeight - 2
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: 2
                anchors.topMargin: 2
                color: "black"
                Image {
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: imageSource
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: parent.state = "expanded"
            }
            Item {
                id: prodtextView
                anchors.top: image.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                opacity: 0
                property int prodID: prodid
                Rectangle {
                    anchors.fill: parent
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#fed958" }
                        GradientStop { position: 1.0; color: "#fecc2f" }
                    }
                    border.color: '#000000'
                    border.width: 2
                    Text {
                        anchors.fill: parent
                        anchors.margins: 5
                        clip: true
                        wrapMode: Text.WordWrap
                        color: '#1f1f21'
                        font.pixelSize: 18
                        text: producttext
                    }
                }
            }

            Rectangle {
                id: closeButton
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: 2
                anchors.topMargin: 2
                width: Constants.listViewHeight * 3
                height: Constants.listViewHeight - 2
                color: "#157efb"
                border.color: Qt.lighter(color, 1.1)
                opacity: 0
                Text {
                    anchors.fill: parent
                    anchors.margins: 5
                    clip: true
                    wrapMode: Text.WordWrap
                    color: '#1f1f21'
                    font.pixelSize: 18
                    text: qsTr("Close") + TranslationUtil.emptyString
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: wrapper.state = ""
                }
            }
            Rectangle {
                id: buyButton
                anchors.right: closeButton.left
                anchors.top: parent.top
                anchors.rightMargin: 2
                anchors.topMargin: 2
                width: Constants.listViewHeight * 3
                height: Constants.listViewHeight - 2
                color: "red"
                border.color: Qt.lighter(color, 1.1)
                opacity: 0
                Text {
                    anchors.fill: parent
                    anchors.margins: 5
                    clip: true
                    wrapMode: Text.WordWrap
                    color: '#1f1f21'
                    font.pixelSize: 18
                    text: qsTr("Buy") + TranslationUtil.emptyString
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: root.buyorder (productID)
                }
            }
            states: [
                State {
                    name: "expanded"
                    PropertyChanges { target: wrapper; height: listView.height }
                    PropertyChanges { target: image; width: listView.width/3; height: listView.width/3; anchors.rightMargin: 0; anchors.topMargin: Constants.listViewHeight }
                    PropertyChanges { target: prodtextView; opacity: 1 }
                    PropertyChanges { target: closeButton; opacity: 1 }
                    PropertyChanges { target: buyButton; opacity: 1 }
                    PropertyChanges { target: wrapper.ListView.view; contentY: wrapper.y; interactive: false }
                }
            ]
            transitions: [
                Transition {
                    NumberAnimation {
                        duration: 200;
                        properties: "height,width,anchors.rightMargin,anchors.topMargin,opacity,contentY"
                    }
                }
            ]
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
