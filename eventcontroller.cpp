#include "eventcontroller.h"

EventController::EventController()
{
}

void EventController::addEvent(std::shared_ptr<Event> event)
{
    eventList.push_back(event);
}

std::vector<Transaction> EventController::getTransactions()
{
    std::vector<Transaction> transactions;

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
                transactions.push_back(*transaction);
            }
        }
    }

    // Return transactions
    return transactions;
}

std::vector<Yield> EventController::getYields()
{
    std::vector<Yield> yields;

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
                yields.push_back(*yield);
            }
        }
    }

    // Return yields
    return yields;
}

std::vector<Reorganization> EventController::getReorganizations()
{
    std::vector<Reorganization> reorganizations;

    for (const std::shared_ptr<Event>& event : eventList)
    {
        if (event->getEventType() == EventType::REORGANIZATION)
        {
            // Use cast to convert event
            const Reorganization* reorganization = dynamic_cast<const Reorganization*>(event.get());

            // Check object
            if (reorganization)
            {
                // Add yield
                reorganizations.push_back(*reorganization);
            }
        }
    }

    // Return reorganizations
    return reorganizations;
}

QDate EventController::getEspecifiedTransactionDate(TransactionType transactionType, bool type)
{
    // Get transactions
    auto transactions = getTransactions();

    // Check if the vector is empty
    if (transactions.empty())
    {
        throw std::runtime_error("Transactions vector is empty");
    }

    // Init the iterator
    auto it = transactions.end();

    // Find the transaction with earliest date
    for (auto transIt = transactions.begin(); transIt != transactions.end(); ++transIt)
    {
        if (transIt->getTransactionType() == transactionType)
        {
            if (type && transIt->getDate() < it->getDate())
            {
                it = transIt;
            }
            else if (transIt->getDate() > it->getDate())
            {
                it = transIt;
            }
        }
    }

    // Check if we found a transaction of the specified type
    if (it == transactions.end())
    {
        throw std::runtime_error("None of the specified transaction type was found");
    }

    // Return the transaction
    return it->getDate();
}
