#include "transaction.h"

Transaction::Transaction(QDate eventData, EventType eventType, TransactionType transactionType, int quantity, double unitaryPrice)
        : Event(eventData, eventType)
{
    this->transactionType = transactionType;
    this->quantity =  quantity;
    this->unitaryPrice = unitaryPrice;
}

TransactionType Transaction::getTransactionType() const
{
    return this->transactionType;
}

int Transaction::getQuantity() const
{
    return this->quantity;
}

double Transaction::getUnitaryPrice() const
{
    return this->unitaryPrice;
}
