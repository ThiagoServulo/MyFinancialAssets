#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "event.h"
#include "constants.h"

class Transaction: public Event
{
private:
    TransactionType transactionType;
    int quantity;
    double unitaryPrice;

public:
    Transaction(QDate eventData, TransactionType transactionType, int quantity, double unitaryPrice);
    TransactionType getTransactionType() const;
    int getQuantity() const;
    double getUnitaryPrice() const;
    QDate getEventDate() const;
    bool operator==(const Transaction& other) const;
};

#endif // TRANSACTION_H
