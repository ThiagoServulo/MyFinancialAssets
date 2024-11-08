#ifndef FINANCIALINSTITUTIONMONTH_H
#define FINANCIALINSTITUTIONMONTH_H

#include <QDate>

class FinancialInstitutionMonth
{
public:
    FinancialInstitutionMonth();
    FinancialInstitutionMonth(QDate date, double value);
    QDate getDate();
    double getValue();

private:
    QDate date;
    double value;
};

#endif // FINANCIALINSTITUTIONMONTH_H
