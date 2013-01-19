import QtQuick 2.0

Rectangle {
    id: root
    width: 800
    height: 600

    Text {
        id: title
        text: "TIMEKEEPER"

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 5
        anchors.leftMargin: 15

        font.pixelSize: 25
        //color:"#777"
    }
    Text {
        id: subtitle
        text: "KEEPS TIME"
        anchors.top: title.bottom
        anchors.left: title.left
        font.italic: true
        color: "#999"
    }



    /* Navigation for switching weeks */
    Rectangle {
        id: weekNavigation
        anchors.bottom: views.top
        anchors.right: parent.right
        anchors.bottomMargin: 5
        anchors.rightMargin: 5
        height:50

        CalendarButton {
            id: buttonNext
            anchors.right: parent.right
            onClicked: {
                cal.hide()
                timekeeper.nextWeek()
                cal.show()
            }
            text: ">>"
            width: 25
        }

        Rectangle {
            id: weekLabelContainer
            anchors.right: buttonNext.left
            height: parent.height
            width: 150

            Text {
                id: weekLabel
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                text: "Week " + timekeeper.currentWeek
            }
            Text {
                id: weekDateLabel
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: weekLabel.bottom

                text: timekeeper.currentWeekDates
                color: "#AAA"
            }
        }

        CalendarButton {
            id: buttonPrev
            anchors.right: weekLabelContainer.left
            onClicked: {
                cal.hide()
                timekeeper.prevWeek()
                cal.show()
            }
            text: "<<"
            width: 25
        }
    }


    /* navigation for switching views: */
    Rectangle {
        height: 35
        width: 200
        anchors.left: parent.left
        anchors.bottom: views.top
        anchors.bottomMargin: 5

        CalendarButton {
            id: buttonCal
            anchors.right: parent.right
            width:100
            height: parent.height
            onClicked: {
                upcoming.hide()
                cal.show()
            }
            text: "Timetable"
        }

        CalendarButton {
            id: buttonList
            anchors.left: parent.left
            width: 100
            height: parent.height
            onClicked: {
                cal.hide()
                showUpcoming()
            }
            text: "Upcoming"
        }
    }

    /* views: */
    Rectangle {
        id: views
        width: root.width
        height: root.height * (5/6)
        anchors.bottom: root.bottom

        /* Timetable view */
        Timetable {
            id:cal
            anchors.fill: parent
            model: timekeeper

            onTimeSlotClicked: {
                //timekeeper.addEvent("Stuff!", day, timeslot);
                if(!input.shown) {
                    input.show(day, timeslot,
                               timekeeper.dateFor(day, timeslot),
                               timekeeper.getEvent(day, timeslot))
                }
            }
        }

        UpcomingList {
            id: upcoming
            anchors.fill: parent
            anchors.leftMargin: 45
            anchors.topMargin: 40
            visible: false
        }
    }

    EventInput {
        id: input
        baseColor: "#DDD"
        onInputReceived: timekeeper.addEvent(input, dayslot, timeslot)
    }

    function showUpcoming() {
        upcoming.show(timekeeper.getUpcomingEvents())
    }

    Component.onCompleted: cal.show()
}
