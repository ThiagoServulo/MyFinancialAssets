#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <transaction.h>

#define NOT_FOUND -1
#define DATABASE_ERROR -2
#define DATABASE_SUCCESS 1

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
    bool checkIfDatabaseExists();
    int getAssetTypeId(AssetType assetType);
    int getTickerId(QString ticker);
    int insertTicker(QString ticker, AssetType assetType);
    bool populateReorganizationTypeTable();
};

#endif // DATABASE_H
