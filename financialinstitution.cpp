#include "financialinstitution.h"

FinancialInstitution::FinancialInstitution()
{

}

FinancialInstitution::FinancialInstitution(QString name)
{
    this->name = name;
}

QString FinancialInstitution::getName()
{
    return name;
}

void FinancialInstitution::addFinancialResult(std::shared_ptr<FinancialInstitutionMonth> result)
{
    monthsResult.push_back(result);
}

std::vector<std::shared_ptr<FinancialInstitutionMonth>> FinancialInstitution::getFinancialResults()
{
    return monthsResult;
}

std::shared_ptr<FinancialInstitutionMonth> FinancialInstitution::getFinancialResult(QDate date)
{
    for(auto result: monthsResult)
    {
        if(result->getDate() == date)
        {
            return result;
        }
    }

    return nullptr;
}
