#include "financialinstitutionmonth.h"

FinancialInstitutionMonth::FinancialInstitutionMonth()
{

}

FinancialInstitutionMonth::FinancialInstitutionMonth(QDate date, double value)
{
    this->date = date;
    this->value = value;
}

QDate FinancialInstitutionMonth::getDate()
{
    return date;
}

double FinancialInstitutionMonth::getValue()
{
    return value;
}
