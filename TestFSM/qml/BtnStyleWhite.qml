import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: root

    Rectangle {
        id: rect
        border.color: "#ffcdcdcd"
        border.width: 1
        radius: 10
        anchors.fill: parent

        gradient: Gradient {
            GradientStop {
                position: 0
                color: control.enabled ? (control.pressed ? "#ccc" : "#FFffffff" ) : "lightgray"
            }
            GradientStop {
                position: 1
                color: control.enabled ? (control.pressed ? "#eee" : "#FFdcdcdc" ) : "lightgray"
            }
        }
    }
    DropShadow {
        anchors.fill: rect
        horizontalOffset: 3
        verticalOffset: 3
        samples: 7
        color: "#80000000"
        source: rect
    }
}
