#include "reorganization.h"

Reorganization::Reorganization(QDate eventData, ReorganizationType reorganizationType, int ratio)
    : Event(eventData, EventType::REORGANIZATION)
{
    this->reorganizationType = reorganizationType;
    this->ratio = ratio;
}

int Reorganization::getRatio() const
{
    return this->ratio;
}

ReorganizationType Reorganization::getReorganizationType() const
{
    return this->reorganizationType;
}
