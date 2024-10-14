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
    bool createCurrentPriceTable();
    bool createYieldTypeTable();
    bool createFixedIncomeTable();
    bool populateReorganizationTypeTable();
    bool populateAssetTypeTable();
    bool populateTransactionTypeTable();
    bool populateYieldTypeTable();
};

#endif // DATABASECREATION_H
