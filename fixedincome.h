#ifndef FIXEDINCOME_H
#define FIXEDINCOME_H

#include <QDate>

class FixedIncome
{
public:
    static const int VALID = 1;
    static const int CLOSED = 0;
    FixedIncome(QDate purchaseDate, QString description, QString yieldExpected, double investedValue,
                QDate limitDate, double currentValue, bool status);
    void setCurrentValue(double currentValue);
    QDate getPurchaseDate();
    QString getDescription();
    QString getYieldExpected();
    double getInvestedValue();
    double getYield();
    double getCurrentValue();
    double getYieldPercentage();
    QDate getLimitDate();
    bool getStatus();
    void setStatus(bool newStatus);

private:
    QDate purchaseDate;
    QString description;
    QString yieldExpected;
    double investedValue;
    double currentValue;
    QDate limitDate;
    bool status;
};

#endif // FIXEDINCOME_H
