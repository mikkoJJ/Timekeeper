#ifndef TIMEKEEPER_H
#define TIMEKEEPER_H

#include <QObject>
#include <QDateTime>
#include <QAbstractListModel>
#include "calendardata.h"


/**
 * A helper class for implementing the calendar as a two-dimensional
 * array. Each week is represented by the Timekeeper class, whereas the
 * days in the week are represented by DayModels, accessible to delegates
 * through the dayModel-role.
 *
 * @brief The DayModel class
 */
class DayModel : public QAbstractListModel {
    Q_OBJECT

private:
    //constant list of hour "names"
    QList<QString> hours;
    //list of roles for the QAbstractListModel implementation
    QHash<int, QByteArray> roles;
    //the day slot in the Timekeeper given to this daymodel
    int day;
    //Timekeeper needs private access.
    friend class Timekeeper;

public:
    //contains just QAbstractListModel overrides:

    explicit DayModel(int dayIndex, QObject* parent=0);

    int rowCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    QHash<int, QByteArray> roleNames() const;
};


/**
 * This is the main controller of the Timekeeper application. It handles
 * interaction between the CalendarData class and the QML GUI. It is
 * derived from QAbstractListModel for easier integration with the QML
 * Viewers.
 *
 * Timekeeper handles one week at a time (defaults to the current week), and
 * events are scheduled by a preset amount of timeslots in each day. See
 * the addEvent()-method.
 *
 * @brief The Timekeeper class
 */
class Timekeeper : public QAbstractListModel
{
    Q_OBJECT
private:
    //calendar object to use - "the model"
    CalendarData* calendar;

    //first day of the current week in use
    QDateTime currWeek;

    //constant list of day names
    QList<QString> days;

    //roles hash, required for QAbstractListModel
    QHash<int, QByteArray> roles;

    //DayModels for each day
    QList<DayModel*> dayModels;

    //make all models notify that data has changed
    void changeAll();

public:
    /** Role number definitions for the model system */
    enum DayRoles {
        HourRole = Qt::UserRole + 1,
        DayRole,
        EventRole,
        DayModelRole
    };

    /** Instantiates the Timekeeper */
    explicit Timekeeper(QObject* parent = 0);

    /**
     * Reads the current week number.
     */
    Q_PROPERTY(int currentWeek READ getCurrentWeekNumber NOTIFY currentWeekChanged)

    /**
     * Reads the current week as a range of dates (date - date).
     */
    Q_PROPERTY(QString currentWeekDates READ getWeekString NOTIFY currentWeekChanged)

    /**
     * Adds an event into the user's schedule.
     * @param event event to add, represented by a string
     * @param dayslot which day in the current week the event is added into
     * @param timeslot which timeslot in the given day the event is added into
     */
    Q_INVOKABLE void addEvent(const QString& event, int dayslot, int timeslot);

    /**
     * Changes the internal week currently handled to the next one.
     */
    Q_INVOKABLE void nextWeek();

    /**
     * Changes the internal week currently handled to the previous one.
     */
    Q_INVOKABLE void prevWeek();

    /**
     * @brief dateFor a date corresponding to timeslots
     * @return the date and time corresponding to the given dayslot and timeslot on
     *      the current week
     */
    Q_INVOKABLE QString dateFor(int dayslot, int timeslot) const;

    /**
     * @brief getEvent returns the event in the given timeslot
     * @param dayslot which day the event belongs to
     * @param timeslot which hour the event belongs to
     * @return the event in the given timeslot
     */
    Q_INVOKABLE QString getEvent(int dayslot, int timeslot) const;

    /**
     * Gets a number of upcoming events represented by a list of
     * QObject-derived EventData objects.
     */
    Q_INVOKABLE QVariant getUpcomingEvents(int howmany=10) const;

    /**
     * @return the current week represented by a string ("date - date")
     */
    QString getWeekString() const;

    /**
     * @brief getCurrentWeekNumber returns the number of the current week in the year
     * @return the number of the week
     */
    int getCurrentWeekNumber() const;


    //----Data model overrides----:

    int rowCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    QHash<int, QByteArray> roleNames() const;

signals:
    /** Emitted when the currentWeek property changes. */
    void currentWeekChanged();

};

#endif // TIMEKEEPER_H
