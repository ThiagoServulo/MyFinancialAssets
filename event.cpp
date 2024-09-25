#include "event.h"

Event::Event()
{
}

Event::Event(QDate eventDate, EventType eventType)
{
    this->date = eventDate;
    this->eventType = eventType;
}

QDate Event::getDate()
{
    return this->date;
}

EventType Event::getEventType() const
{
    return this->eventType;
}
