#ifndef EVENTCONTROLLER_H
#define EVENTCONTROLLER_H

#include "transaction.h"
#include "yield.h"
#include <vector>

class EventController
{
private:
    std::vector<std::shared_ptr<Event>> eventList;

public:
    EventController();
    void addEvent(std::shared_ptr<Event> event);
    void addEvents(std::vector<std::shared_ptr<Event>>& events);
    std::vector<Transaction> getTransactionList();
    std::vector<Yield> getYieldList();
};

#endif // EVENTCONTROLLER_H
