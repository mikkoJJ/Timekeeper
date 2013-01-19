/**
 * A little "window" for inputting event data.
 */
import QtQuick 2.0

Item {
    id: root
    property int dayslot
    property int timeslot
    property string datetime
    property alias baseColor: bg.color
    property alias text: input.text
    property bool shown: state == "shown" ? true : false

    signal inputReceived(string input, int dayslot, int timeslot)

    /**
     * Shows the input and "binds" it to the given date and time slots.
     * Actual date and possible event information must also be explicitly given.
     */
    function show(day, time, date, eventData) {
        state = "shown"
        dayslot = day
        timeslot = time
        datetime = date
        text = eventData
        input.focus = true
    }

    /**
     * Hides and clears the input.
     */
    function hide() {
        state = "hidden"
        input.text = ""
        input.changed = false
    }

    width: 300
    height: 200
    y: parent.height + 400
    x: parent.width / 2 - width / 2
    state: "hidden"

    Rectangle {
        id: bg
        anchors.fill: parent
        Rectangle {
            id: dateLabelContainer
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            height: 40
            color: "#888"
            clip:true
            Text {
                id: dateLabel
                text: "Add event for <b>" + root.datetime + "</b>"
                font.pixelSize: 14
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                color: "white"
            }
        }

        Rectangle {
            id: inputContainer
            anchors.top: dateLabelContainer.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 10
            color: "white"
            TextEdit {
                id: input
                property bool changed: false
                anchors.fill: parent
                anchors.margins: 5
                color: "black"
                onTextChanged: changed = true
            }
        }

        CalendarButton {
            id: okButton
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            text: "OK"
            onClicked: {
                if (input.changed) inputReceived(input.text, root.dayslot, root.timeslot)
                root.hide()
            }
        }
    }

    states: [
        State {
            name: "shown"
            PropertyChanges { target: root; y: parent.height / 2 - height / 2 }
        },
        State {
            name: "hidden"
            PropertyChanges { target: root; y: parent.height + height }
        }
    ]

    transitions: Transition {
        NumberAnimation { property: "y"; duration: 400; easing.type: Easing.InOutBack }
    }
}
