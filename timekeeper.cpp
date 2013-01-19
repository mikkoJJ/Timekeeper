#include "timekeeper.h"
#include <QDate>
#include <QTime>
#include <QByteArray>
#include <QVector>
#include <QModelIndex>
#include <QDebug>


//----------------------------------------------
//----Timekeeper:

Timekeeper::Timekeeper(QObject *parent) :
    QAbstractListModel(parent)
{
    calendar = new CalendarData(this);

    for (int i=0; i<7; i++) {
        dayModels.append(new DayModel(i, this));
    }

    //set currWeek into the monday of the current week:
    QDate curdate = QDate::currentDate();
    curdate = curdate.addDays(1 - curdate.dayOfWeek());
    QTime curtime(8, 0);

    currWeek.setDate(curdate);
    currWeek.setTime(curtime);

    //init weekday names:
    days << "monday" << "tuesday" << "wednesday" << "thursday" << "friday" << "saturday" << "sunday";

    //init roles:
    roles.insert(DayRole, QByteArray("day"));
    roles.insert(DayModelRole, QByteArray("dayModel"));
}

void Timekeeper::addEvent(const QString &event, int dayslot, int timeslot)
{
    QDateTime eventDate = currWeek.addDays(dayslot).addSecs(60*60*2*timeslot);

    calendar->addEvent(event, eventDate);

    //notify model changed:
    DayModel* changed = dayModels.at(dayslot);
    QModelIndex changedIndex = changed->createIndex(timeslot, 0);
    QVector<int> roles(1, EventRole);
    changed->dataChanged(changedIndex, changedIndex, roles);
}

void Timekeeper::nextWeek()
{
    currWeek = currWeek.addDays(7);
    emit currentWeekChanged();
    changeAll();
}

void Timekeeper::prevWeek()
{
    currWeek = currWeek.addDays(-7);
    emit currentWeekChanged();
    changeAll();
}

int Timekeeper::rowCount(const QModelIndex &parent) const
{
    //row size is constant:
    return 7;
}

QVariant Timekeeper::data(const QModelIndex &index, int role) const
{
    if ( role == DayRole ) {
        return QVariant::fromValue(days.at(index.row()));
    }
    if ( role == DayModelRole ) {
        //we return a new model for each day
        return QVariant::fromValue(dayModels.at(index.row()));
    }
    return QVariant::Invalid;
}

QHash<int, QByteArray> Timekeeper::roleNames() const
{
    return roles;
}

int Timekeeper::getCurrentWeekNumber() const
{
    return currWeek.date().weekNumber();
}

QString Timekeeper::dateFor(int dayslot, int timeslot) const
{
    return currWeek.addDays(dayslot).addSecs(60*60*2*timeslot).toString();
}

QString Timekeeper::getWeekString() const
{
    return currWeek.date().toString(Qt::DefaultLocaleShortDate) + " - " + currWeek.addDays(7).date().toString(Qt::DefaultLocaleShortDate);
}

QString Timekeeper::getEvent(int dayslot, int timeslot) const
{
    QDateTime date = currWeek.addDays(dayslot).addSecs(60*60*2*timeslot);
    return calendar->getEvent(date);
}

QVariant Timekeeper::getUpcomingEvents(int howmany) const
{
    return QVariant::fromValue(calendar->getEvents(QDateTime::currentDateTime(), howmany));
}

void Timekeeper::changeAll()
{
    for (int i=0; i<7; i++) {
        DayModel* model = dayModels.at(i);
        QModelIndex from = model->createIndex(0, 0);
        QModelIndex to = model->createIndex(7, 0);
        model->dataChanged(from, to);
    }
}



//---------------------------------------------------
//DayModel:

DayModel::DayModel(int dayIndex, QObject *parent)
    : QAbstractListModel(parent)
{
    //init hour names:
    hours << "08:00" << "10:00" << "12:00" << "14:00" << "16:00" << "18:00" << "20:00";

    //init roles:
    roles.insert(Timekeeper::DayRole, QByteArray("day"));
    roles.insert(Timekeeper::HourRole, QByteArray("hour"));
    roles.insert(Timekeeper::EventRole, QByteArray("dayEvent"));

    this->day = dayIndex;
}

int DayModel::rowCount(const QModelIndex &parent) const
{
    //row size is constant:
    return 7;
}

QVariant DayModel::data(const QModelIndex &index, int role) const
{
    if ( role == Timekeeper::HourRole ) {
        return QVariant::fromValue(hours.at(index.row()));
    }

    if ( role == Timekeeper::DayRole ) {
        return QVariant::fromValue(day);
    }

    if ( role == Timekeeper::EventRole ) {
        Timekeeper* parent = (Timekeeper*)QObject::parent();
        QString event = parent->getEvent(day, index.row());
        return QVariant::fromValue(event);
    }

    return QVariant::Invalid;
}

QHash<int, QByteArray> DayModel::roleNames() const
{
    return roles;
}



