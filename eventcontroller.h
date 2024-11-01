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
    std::vector<Transaction> getTransactions(QDate *init, QDate *end);
    std::vector<Yield> getYields(QDate *init, QDate *end);
    std::vector<Reorganization> getReorganizations(QDate *init, QDate *end);
    QDate getEspecifiedTransactionDate(TransactionType transactionType, bool type);
};

#endif // EVENTCONTROLLER_H
