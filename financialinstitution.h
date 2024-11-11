#ifndef FINANCIALINSTITUTION_H
#define FINANCIALINSTITUTION_H

#include "financialinstitutionmonth.h"
#include <QString>
#include <QDate>

class FinancialInstitution: public FinancialInstitutionMonth
{
public:
    FinancialInstitution();
    FinancialInstitution(QString name);
    QString getName();
    void addFinancialResult(std::shared_ptr<FinancialInstitutionMonth> result);
    std::vector<std::shared_ptr<FinancialInstitutionMonth>> getFinancialResults();
    std::shared_ptr<FinancialInstitutionMonth> getFinancialResult(QDate date);

private:
    QString name;
    std::vector<std::shared_ptr<FinancialInstitutionMonth>> monthsResult;
};

#endif // FINANCIALINSTITUTION_H
