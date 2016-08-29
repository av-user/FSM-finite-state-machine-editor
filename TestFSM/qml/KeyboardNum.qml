import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.4
import "qrc:/singletons/singletons/"
import TranslationUtil 1.0

Item {
    id: root

    signal keyClicked (string str)
    property string corr: qsTr("Corr") + TranslationUtil.emptyString
    GridLayout {
        id: keys
        readonly property variant values: [1, 2, 3, corr, 4, 5, 6
            , qsTr("O'k") + TranslationUtil.emptyString, 7, 8, 9, "<", 0, ">"]
        rows: 4; columns: 4

        Repeater {
            id: keys_repeater
            model: keys.values
            Button {
                id: keybtn
                Layout.fillHeight: true
                Layout.fillWidth: true
//                Layout.preferredHeight: 52
//                Layout.preferredWidth: 110
                Layout.preferredHeight: Constants.buttonWidth1
                Layout.preferredWidth: Constants.buttonHeight2
                Layout.rowSpan: index == 7 ? 3 : 1
                text: modelData
                style: ButtonStyle {
                    background: BtnStyleWhite {}
                    label: BtnStyleLabelKbd {}
                }
                Connections {
                    target: keybtn
                    onClicked: {
                        switch (index){
                        case 3:
                            root.keyClicked("Backspace")
                            break
                        case 7:
                            root.keyClicked("Enter")
                            break
                        case 11:
                            root.keyClicked("Left")
                            break
                        case 13:
                            root.keyClicked("Right")
                            break
                        default:
                            root.keyClicked(keys_repeater.itemAt(index).text)
                        }
                    }
                }
            }
        }
    }
}
