#include "yield.h"

Yield::Yield(QDate eventData, YieldType yieldType, double value)
        : Event(eventData, EventType::YIELD)
{
    this->yieldType = yieldType;
    this->value =  value;
}

double Yield::getValue() const
{
    return this->value;
}

YieldType Yield::getYieldType() const
{
    return this->yieldType;
}

QDate Yield::getEventDate() const
{
    return getDate();
}
