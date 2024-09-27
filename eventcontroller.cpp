#include "eventcontroller.h"

EventController::EventController()
{
}

void EventController::addEvent(std::shared_ptr<Event> event)
{
    eventList.push_back(event);
}

std::vector<Transaction> EventController::getTransactionList()
{
    std::vector<Transaction> transactionList;

    for (const std::shared_ptr<Event>& event : eventList)
    {
        if (event->getEventType() == EventType::TRANSACTION)
        {
            // Use cast to convert event
            const Transaction* transaction = dynamic_cast<const Transaction*>(event.get());

            // Check object
            if (transaction)
            {
                // Add transaction
                transactionList.push_back(*transaction);
            }
        }
    }

    // Return transactions
    return transactionList;
}

std::vector<Yield> EventController::getYieldList()
{
    std::vector<Yield> yieldList;

    for (const std::shared_ptr<Event>& event : eventList)
    {
        if (event->getEventType() == EventType::YIELD)
        {
            // Use cast to convert event
            const Yield* yield = dynamic_cast<const Yield*>(event.get());

            // Check object
            if (yield)
            {
                // Add yield
                yieldList.push_back(*yield);
            }
        }
    }

    // Return yields
    return yieldList;
}
