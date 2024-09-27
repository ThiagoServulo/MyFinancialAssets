#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <transaction.h>
#include <yield.h>

#define NOT_FOUND -1
#define DATABASE_ERROR -2
#define DATABASE_SUCCESS 1

class Database
{
public:
    Database();
    bool prepareDatabase();
    bool insertTransaction(QString ticker, AssetType assetType, Transaction transaction);
    int insertYield(QString ticker, Yield yield);
    bool selectAllTickers(std::vector<std::pair<QString, AssetType>>& tickers);
    int getTickerQuantity(QString ticker);
    std::vector<Transaction> getTransactionsByTickerId(int tickerId);

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
    int getYieldTypeId(YieldType yieldType);
    int getTickerId(QString ticker);
    int insertTicker(QString ticker, AssetType assetType);
    int getTransactionTypeId(TransactionType transactionType);
    bool populateReorganizationTypeTable();
};

#endif // DATABASE_H
