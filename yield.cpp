#include "yield.h"

Yield::Yield(QDate eventData, EventType eventType, YieldType yieldType, double value)
        : Event(eventData, eventType)
{
    this->yieldType = yieldType;
    this->value =  value;
}

int Yield::getValue() const
{
    return this->value;
}

YieldType Yield::getYieldType() const
{
    return this->yieldType;
}
