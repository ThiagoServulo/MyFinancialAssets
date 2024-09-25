#ifndef EVENTCONTROLLER_H
#define EVENTCONTROLLER_H

#include "event.h"
#include "transaction.h"
#include <vector>

class EventController
{
private:
    std::vector<std::shared_ptr<Event>> eventList;

public:
    EventController();
    void addEvent(std::shared_ptr<Event> event);
    bool removeEvent(Event event);
    std::vector<Transaction> getTransactionList();
};

#endif // EVENTCONTROLLER_H
