#ifndef FINANCIALINSTITUTION_H
#define FINANCIALINSTITUTION_H

#include <QString>
#include <QDate>

class FinancialInstitution
{
public:
    FinancialInstitution();
    FinancialInstitution(QString name, QDate date, double value);
    QString getName();
    QDate getDate();
    double getValue();

private:
    QString name;
    QDate date;
    double value;
};

#endif // FINANCIALINSTITUTION_H
