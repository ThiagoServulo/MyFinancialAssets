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
    for (const std::shared_ptr<Event>& event : eventList) // Usando range-based for loop com shared_ptr
    {
        qDebug() << (event->getEventType() == EventType::TRANSACTION);
        if (event->getEventType() == EventType::TRANSACTION)
        {
            // Tente fazer um dynamic_cast para verificar se é um Transaction
            const Transaction* transaction = dynamic_cast<const Transaction*>(event.get());
            if (transaction) // Se o cast for bem-sucedido
            {
                transactionList.push_back(*transaction); // Adiciona o objeto Transaction
            }
            else
            {
                qDebug() << "Objeto não é uma Transaction.";
            }
        }
    }
    return transactionList;
}

bool EventController::removeEvent(Event event)
{
    /*
    for (auto it = eventList.begin(); it != eventList.end(); ++it)
    {
        if((it->getId() == event.getId()) && it->getEventType() == event.getEventType())
        {
            eventList.erase(it);
            return true;
        }
    }
    qDebug() << "Error to remove event";
    return false;
    */
}
