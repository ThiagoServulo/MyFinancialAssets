#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>

class Database
{
public:
    Database();
    bool prepareDatabase();

private:
    QSqlDatabase database;
    bool openDatabase();
    void closeDatabase();
    bool createYieldTable();
    bool createAssetTypeTable();
    bool populateAssetTypeTable();
    bool createYieldTypeTable();
    bool populateYieldTypeTable();
    bool createTransactionTypeTable();
    bool createTickerTable();
    bool createTransactionTable();
    bool populateTransactionTypeTable();
    bool createReorganizationTypeTable();
    bool createReorganizationTable();
    bool populateReorganizationTypeTable();
};

#endif // DATABASE_H
