#ifndef CALENDARDATA_H
#define CALENDARDATA_H

#include <QObject>
#include <QDateTime>
#include <QMap>
#include <QList>


/**
 * Helper class. Holds needed data for events.
 *
 * This class is never changed once initialized.
 */
class EventData : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString eventName READ getEvent NOTIFY propertyChanged)
    Q_PROPERTY(QDateTime when READ getWhen NOTIFY propertyChanged)

private:
    QString _event;
    QDateTime _when;

public:
    explicit EventData(const QString& event, const QDateTime& date, QObject *parent=0);

    QString getEvent();
    QDateTime getWhen();

signals:
    //will never be emitted, but will appease the QML compiler
    //(properties are immutable):
    void propertyChanged();
};



/**
 * This class holds information about the user's scheduled events.
 *
 * Right now it can only store the data in a simple datatype in memory,
 * but it could be extended to store this information into a database
 * for example.
 *
 * @brief The CalendarData class, the model of the Timekeeper application
 */
class CalendarData : public QObject
{
    Q_OBJECT

private:
    QMap<QDateTime, QString> schedule;

public:
    explicit CalendarData(QObject *parent = 0);

    /**
     * Adds a new scheduled event into the calendar.
     * @param event the event to add, represented by a string
     * @param date the date and time when this event takes place.
     */
    void addEvent(const QString& event, const QDateTime& date);

    /**
     * @brief getEvent the event in the given date
     * @param date date to lookup
     * @return the event string associated with the given date. Returns an empty
     *       string if there is no evetn scheduled.
     */
    QString getEvent(const QDateTime& date) const;

    /**
     * @brief getEvents get number of events from the calendar
     * @param startingFrom which date to start from
     * @param howmany how many events to get
     * @return a list of events (represented by EventData objects) that are
     *      schedulet at earliest the given starting date.
     */
    QList<QObject*> getEvents(const QDateTime& startingFrom, int howmany) const;
};

#endif // CALENDARDATA_H
