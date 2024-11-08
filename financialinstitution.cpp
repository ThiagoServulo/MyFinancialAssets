#include "financialinstitution.h"

FinancialInstitution::FinancialInstitution()
{

}

FinancialInstitution::FinancialInstitution(QString name, QDate date, double value)
{
    this->name = name;
    this->date = date;
    this->value = value;
}

QString FinancialInstitution::getName()
{
    return name;
}

QDate FinancialInstitution::getDate()
{
    return date;
}

double FinancialInstitution::getValue()
{
    return value;
}
