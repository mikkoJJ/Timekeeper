#include "calendardata.h"
#include <QtDebug>

CalendarData::CalendarData(QObject *parent) :
    QObject(parent)
{
}

void CalendarData::addEvent(const QString &event, const QDateTime &date)
{
    schedule.insert(date, event);
    //qDebug() << "Inserted: " << event << "into: " << date;
}

QString CalendarData::getEvent(const QDateTime &date) const
{
    QString ret = schedule.value(date, QStringLiteral(""));
    //qDebug() << "Fetched: " << ret << " from: " << date;
    return ret;
}

QList<QObject*> CalendarData::getEvents(const QDateTime &startingFrom, int howmany) const
{
    QList<QObject*> list;
    int i = 0;

    QMap<QDateTime, QString>::const_iterator iter;
    for (iter = schedule.constBegin(); iter != schedule.end(); ++iter) {
        if (iter.key() < startingFrom) continue;
        if (i >= howmany) break;

        list.append(new EventData(iter.value(), iter.key(), (QObject*)this));
        i++;
    }

    return list;
}


//-------------------------
//---------EventData:

EventData::EventData(const QString& event, const QDateTime& date, QObject *parent)
    : QObject(parent), _event(event), _when(date)
{
}

QString EventData::getEvent()
{
    return _event;
}

QDateTime EventData::getWhen()
{
    return _when;
}
