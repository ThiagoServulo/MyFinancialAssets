#ifndef EVENT_H
#define EVENT_H

#include <QDate>
#include "constants.h"

class Event
{
private:
    QDate date;
    EventType eventType;

public:
    Event();
    virtual ~Event() {};
    Event(QDate eventDate, EventType eventType);
    QDate getDate();
    EventType getEventType() const;
};

#endif // EVENT_H
