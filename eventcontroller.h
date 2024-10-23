#ifndef EVENTCONTROLLER_H
#define EVENTCONTROLLER_H

#include "transaction.h"
#include "yield.h"
#include "reorganization.h"
#include <vector>

class EventController
{
private:
    std::vector<std::shared_ptr<Event>> eventList;

public:
    EventController();
    void addEvent(std::shared_ptr<Event> event);
    std::vector<Transaction> getTransactions();
    std::vector<Yield> getYields();
    std::vector<Reorganization> getReorganizations();
    QDate getEspecifiedTransactionDate(TransactionType transactionType, bool type);
};

#endif // EVENTCONTROLLER_H
