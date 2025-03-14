#ifndef DATABASECREATION_H
#define DATABASECREATION_H


class DatabaseCreation
{
public:
    DatabaseCreation();
    bool createDatabase();

private:
    bool createYieldTable();
    bool createAssetTypeTable();
    bool createLastUpdateTable();
    bool createReorganizationTable();
    bool createReorganizationTypeTable();
    bool createTransactionTable();
    bool createTickerTable();
    bool createTransactionTypeTable();
    bool createYieldTypeTable();
    bool createFixedIncomeTable();
    bool populateReorganizationTypeTable();
    bool populateAssetTypeTable();
    bool populateTransactionTypeTable();
    bool populateYieldTypeTable();
    bool createFinancialInstitutionTable();
    bool createFinancialInstitutionMonthTable();
};

#endif // DATABASECREATION_H
