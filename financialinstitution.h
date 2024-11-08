#ifndef FINANCIALINSTITUTION_H
#define FINANCIALINSTITUTION_H

#include "financialinstitutionmonth.h"
#include <QString>
#include <QDate>

class FinancialInstitution: FinancialInstitutionMonth
{
public:
    FinancialInstitution();
    FinancialInstitution(QString name);
    QString getName();
    void addFinancialResult(std::shared_ptr<FinancialInstitutionMonth> result);
    std::vector<std::shared_ptr<FinancialInstitutionMonth>> getFinancialResults();

private:
    QString name;
    std::vector<std::shared_ptr<FinancialInstitutionMonth>> monthsResult;
};

#endif // FINANCIALINSTITUTION_H
