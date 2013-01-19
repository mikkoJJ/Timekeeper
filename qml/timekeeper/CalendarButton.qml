/**
 * A simple button element for the calendar.
 */
import QtQuick 2.0

Item {
    id: root
    signal clicked;
    property alias text: buttonText.text

    width: 50
    height: 50

    Rectangle {
        id: buttonContainer
        anchors.fill: parent
        color: buttonArea.containsMouse ? "#DDD" : "#BCBCBC"
        radius: 5
        border.width: 1
        border.color: "#666"
        smooth: true

        Text {
            id: buttonText
            anchors.centerIn: parent
            color: "#666"
        }

        MouseArea {
            id: buttonArea
            anchors.fill: parent
            onClicked: root.clicked()
            hoverEnabled: true
        }
    }
}
