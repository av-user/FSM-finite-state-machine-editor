import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: root

    Rectangle {
        id: rect
        border.color: "white"
        border.width: 2
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0
                color: control.enabled ? (control.pressed ? "#ccc" : "#FF8F8D8D" ) : "lightgray"
            }
            GradientStop {
                position: 1
                color: control.enabled ? (control.pressed ? "#eee" : "#FFDCD5D5" ) : "lightgray"
            }
        }
    }
}
