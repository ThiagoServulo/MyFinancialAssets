#include "transaction.h"

Transaction::Transaction(QDate eventData, TransactionType transactionType, int quantity, double unitaryPrice)
        : Event(eventData, EventType::TRANSACTION)
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

QDate Transaction::getEventDate() const
{
    return getDate();
}

bool Transaction::operator==(const Transaction& other) const
{
    return this->getDate() == other.getDate() &&
           this->transactionType == other.transactionType &&
           this->quantity == other.quantity &&
           this->unitaryPrice == other.unitaryPrice;
}
