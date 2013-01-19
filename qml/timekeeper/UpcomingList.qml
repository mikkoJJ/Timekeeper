/**
 * A simple component for viewing events in the calendar as a list.
 */
import QtQuick 2.0

Item {
    id: root

    /**
     * Shows the list with the given model.
     */
    function show(model) {
        visible = true
        list.model = model
        list.itemAt(0).show();
    }

    function hide() {
        visible = false
        list.model = undefined
        for(var i=0; i<list.count; i++) {
            list.itemAt(i).opacity = 0
        }
     }

    Column {
        spacing: 2
        anchors.top: label.bottom

        Repeater { //(ListView doesn't really cooperate with me, so I use Repeater)
            id: list
            Rectangle {
                id: listItem
                width: 200
                height: 50
                opacity: 0
                Column {
                    Text { text: "<b>" + modelData.eventName + "</b>" }
                    Text { text: modelData.when; color: "#666" }
                }

                SequentialAnimation {
                    id: animate
                    PropertyAnimation {
                        target: listItem;
                        from:0; to: 0.6; property: "opacity"
                        duration: 50
                    }
                    ScriptAction { script: showNext() }
                    PropertyAnimation {
                        target: listItem;
                        to: 1; property: "opacity"
                        duration: 100
                    }
                }

                function show() {
                    animate.start()
                }

                function showNext() {
                    if (index < list.count-1) list.itemAt(index+1).show();
                }
            }
        }
    }
}
