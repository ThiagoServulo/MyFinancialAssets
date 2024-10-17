#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include "transaction.h"
#include "yield.h"
#include "reorganization.h"
#include "investmentcontroller.h"
#include "fixedincome.h"

class Database
{
public:
    Database();
    bool prepareDatabase();
    bool insertAsset(Asset asset);
    bool insertTransaction(QString ticker, Transaction transaction);
    bool insertYield(QString ticker, Yield yield);
    bool insertReorganization(QString ticker, Reorganization reorganization);
    bool selectAllAssets(std::vector<Asset>& assets);
    bool investmentControllerInitialization(InvestmentController* investmentController);
    bool checkLastUpdate();
    bool insertFixedIncome(FixedIncome fixedIncome);
    bool updateTickerCurrentPrice(QString ticker, double currentPrice);

private:
    QSqlDatabase database;
    bool openDatabase();
    void closeDatabase();
    bool checkIfDatabaseExists();
    bool selectEventsForAsset(Asset* asset);
    std::vector<Transaction> getTransactionsByTicker(QString ticker);
    std::vector<Yield> getYieldsByTicker(QString ticker);
    std::vector<Reorganization> getReorganizationsByTicker(QString ticker);
    QDate selectLastUpdateDate();
    bool selectAllTicker(QStringList *tickers);
    bool insertLastUpdateDate(QDate date);
};

#endif // DATABASE_H
