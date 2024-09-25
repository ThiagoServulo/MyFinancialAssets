#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <transaction.h>

class Database
{
public:
    Database();
    bool prepareDatabase();
    bool insertIntoTransactionTable(Transaction transaction);

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
