#ifndef FIXEDINCOME_H
#define FIXEDINCOME_H

#include <QDate>

class FixedIncome
{
public:
    FixedIncome(QDate purchaseDate, QString description, QString yieldExpected, double investedValue, QDate limitDate);
    void setCurrentValue(double currentValue);

private:
    QDate purchaseDate;
    QString description;
    QString yieldExpected;
    double investedValue;
    double currentValue;
    QDate limitDate;
    bool status;
    static const int VALID = 0;
    static const int CLOSED = 0;
};

#endif // FIXEDINCOME_H
