#ifndef ASSET_H
#define ASSET_H

#include "constants.h"
#include "eventcontroller.h"
#include <QString>

class Asset: public EventController
{
private:
    QString ticker;
    AssetType assetType;
    double currentPrice;

public:
    Asset();
    Asset(QString ticker, AssetType assetType, double currentPrice);
    QString getTicker() const;
    AssetType getAssetType();
    double getCurrentPrice();
    void setCurrentPrice(double price);
    int getQuantity(QDate *init, QDate *end) const;
    double getTotalYield(QDate *init, QDate *end);
    double getAveragePrice();
    double getTotalInvested(QDate *init, QDate *end);
    double getProfitPercentage();
    double getCapitalGain();
    double getTransactionsTotal(TransactionType transactionType, QDate *init, QDate *end);
    double getProfitPercentageTotal();
    double getCapitalGainTotal();
};

#endif // ASSET_H
