#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include "transaction.h"
#include "yield.h"
#include "reorganization.h"
#include "investmentcontroller.h"
#include "fixedincome.h"

#define NOT_FOUND -1
#define DATABASE_ERROR -2
#define DATABASE_SUCCESS 1

class Database
{
public:
    Database();
    bool prepareDatabase();
    bool insertTransaction(QString ticker, AssetType assetType, Transaction transaction);
    bool insertYield(QString ticker, Yield yield);
    bool insertReorganization(QString ticker, Reorganization reorganization);
    bool selectAllAssets(std::vector<Asset>& assets);
    bool investmentControllerInitialization(InvestmentController* investmentController);
    bool checkLastUpdate();
    int insertFixedIncome(FixedIncome fixedIncome);
    int updateTickerCurrentPrice(QString ticker, double currentPrice);

private:
    QSqlDatabase database;
    bool openDatabase();
    void closeDatabase();
    bool checkIfDatabaseExists();
    int getTickerId(QString ticker);
    bool insertTicker(QString ticker, AssetType assetType, double currentPrice);
    bool selectEventsForAsset(Asset* asset);
    std::vector<Transaction> getTransactionsByTicker(QString ticker);
    std::vector<Yield> getYieldsByTickerId(int tickerId);
    std::vector<Reorganization> getReorganizationsByTickerId(int tickerId);
    QDate selectLastUpdateDate();
    bool selectAllTicker(QStringList *tickers);
    int insertLastUpdateDate(QDate date);
};

#endif // DATABASE_H
