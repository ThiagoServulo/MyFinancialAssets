#ifndef YIELD_H
#define YIELD_H

#include "event.h"
#include "constants.h"

class Yield: public Event
{
private:
    YieldType yieldType;
    double value;

public:
    Yield(QDate eventData, EventType eventType, YieldType yieldType, double value);
    int getValue() const;
    YieldType getYieldType() const;
};

#endif // YIELD_H
