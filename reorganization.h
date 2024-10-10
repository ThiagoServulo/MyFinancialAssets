#ifndef REORGANIZATION_H
#define REORGANIZATION_H

#include "event.h"

class Reorganization: public Event
{
private:
    ReorganizationType reorganizationType;
    int ratio;

public:
    Reorganization(QDate eventData, ReorganizationType reorganizationType, int ratio);
    int getRatio() const;
    ReorganizationType getReorganizationType() const;
};

#endif // REORGANIZATION_H
