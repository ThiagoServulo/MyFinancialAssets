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
    Yield(QDate eventData, YieldType yieldType, double value);
    double getValue() const;
    YieldType getYieldType() const;
    QDate getEventDate() const;
};

#endif // YIELD_H
