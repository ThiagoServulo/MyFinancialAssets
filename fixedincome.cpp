#include "fixedincome.h"

FixedIncome::FixedIncome(QDate purchaseDate, QString description, QString yieldExpected, double investedValue, QDate limitDate,  double currentValue, bool status)
{
    this->purchaseDate = purchaseDate;
    this->description = description;
    this->yieldExpected = yieldExpected;
    this->investedValue = investedValue;
    this->currentValue = currentValue;
    this->limitDate = limitDate;
    this->status = status;
}

void FixedIncome::setCurrentValue(double currentValue)
{
    this->currentValue = currentValue;
}

QDate FixedIncome::getPurchaseDate()
{
    return purchaseDate;
}

QString FixedIncome::getDescription()
{
    return description;
}

QString FixedIncome::getYieldExpected()
{
    return yieldExpected;
}

double FixedIncome::getInvestedValue()
{
    return investedValue;
}

double FixedIncome::getCurrentValue()
{
    return currentValue;
}

QDate FixedIncome::getLimitDate()
{
    return limitDate;
}

bool FixedIncome::getStatus()
{
    return status;
}

void FixedIncome::setStatus(bool newStatus)
{
    status = newStatus;
}

