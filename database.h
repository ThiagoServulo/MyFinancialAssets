#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include "transaction.h"
#include "yield.h"
#include "reorganization.h"
#include "assetcontroller.h"

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
    int insertReorganization(QString ticker, Reorganization reorganization);
    int getTickerQuantity(QString ticker);
    bool selectAllAssets(std::vector<Asset>& assets);
    bool assetControllerInitialization(AssetController* assetController);
    double getTickerAveragePrice(QString ticker);
    double getTickerTotalYield(QString ticker);
    std::vector<Transaction> getTickerTransactions(QString ticker);
    std::vector<Yield> getTickerYields(QString ticker);
    std::vector<Reorganization> getTickerReorganizations(QString ticker);
    bool checkLastUpdate();

private:
    QSqlDatabase database;
    bool openDatabase();
    void closeDatabase();
    bool createYieldTable();
    bool createAssetTypeTable();
    bool createLastUpdateTable();
    bool populateAssetTypeTable();
    bool createYieldTypeTable();
    bool createCurrentPriceTable();
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
    int getReorganizationTypeId(ReorganizationType reorganizationType);
    int getTickerId(QString ticker);
    int insertTicker(QString ticker, AssetType assetType, double currentPrice);
    int getTransactionTypeId(TransactionType transactionType);
    bool populateReorganizationTypeTable();
    bool selectEventsForAsset(Asset* asset);
    std::vector<Transaction> getTransactionsByTickerId(int tickerId);
    std::vector<Yield> getYieldsByTickerId(int tickerId);
    std::vector<Reorganization> getReorganizationsByTickerId(int tickerId);
    QDate selectLastUpdateDate();
    bool selectAllTicker(QStringList *tickers);
    int updateTickerCurrentPrice(QString ticker, double currentPrice);
    int insertLastUpdateDate(QDate date);
};

#endif // DATABASE_H
