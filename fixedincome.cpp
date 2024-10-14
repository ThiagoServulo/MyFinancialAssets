#include "fixedincome.h"

FixedIncome::FixedIncome(QDate purchaseDate, QString description, QString yieldExpected, double investedValue, QDate limitDate)
{
    this->purchaseDate = purchaseDate;
    this->description = description;
    this->yieldExpected = yieldExpected;
    this->investedValue = investedValue;
    this->currentValue = 0;
    this->limitDate = limitDate;
    this->status = VALID;
}

void FixedIncome::setCurrentValue(double currentValue)
{
    this->currentValue = currentValue;
}
