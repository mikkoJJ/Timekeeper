/******************************************
 * Timekeeper
 *      by Mikko Jakonen
 *----------------------------------------
 * For the Qt5/QML2 course at Jyväskylä
 * university.
 *
 * This is a simple calendar where the 
 * user can schedule events using a 
 * timetable interface.
 *
 ****************************************/

#include <QtGui/QGuiApplication>
#include <QQmlContext>
#include "qtquick2applicationviewer.h"
#include "timekeeper.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Timekeeper timekeeper;
    QtQuick2ApplicationViewer viewer;

    timekeeper.nextWeek();
    timekeeper.addEvent(QStringLiteral("Jotain"), 2, 2);
    timekeeper.addEvent(QStringLiteral("Muuta"), 2, 3);
    timekeeper.prevWeek();

    viewer.rootContext()->setContextProperty("timekeeper", &timekeeper);
    viewer.setMainQmlFile(QStringLiteral("qml/timekeeper/main.qml"));
    viewer.setTitle(QStringLiteral("Timekeeper"));
    viewer.showExpanded();


    return app.exec();
}
